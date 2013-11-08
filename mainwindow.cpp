#include"mainwindow.h"
#include"ga_plot.h"
#include"genetic_algorithm.h"
#include<QLabel>
#include<QComboBox>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QGridLayout>
#include<QGroupBox>
#include<QRadioButton>
#include<QDockWidget>
#include<QDoubleSpinBox>
#include<QSpinBox>
#include<QPushButton>
#include<QList>
#include<QPolygonF>
#include<qwt_symbol.h>

MainWindow::MainWindow( QWidget *parent ):
    QMainWindow( parent )
{

    this->createParameterInputWindow();
    this->createMainWindow();

    connect( check_original_fun, SIGNAL( clicked() ), this, SLOT( checkOriginal() ) );
    connect( start_evolve, SIGNAL( clicked() ), this, SLOT( startEvolve() ) );
    connect(dateback_btn, SIGNAL( clicked() ), this, SLOT( dateBack() ) );

    connect( individual_num_input, SIGNAL( valueChanged(int) ), this, SLOT( parameterChange() ) );
    connect( generation_num_input, SIGNAL( valueChanged(int) ), this, SLOT( parameterChange() ) );
    connect( crossover_rate_input, SIGNAL(valueChanged(double)), this, SLOT( parameterChange() ) );
    connect( mutation_rate_input, SIGNAL(valueChanged(double)), this, SLOT( parameterChange() ) );
    connect( range_min_input, SIGNAL(valueChanged(double)), this, SLOT(parameterChange()) );
    connect( range_max_input, SIGNAL(valueChanged(double)), this, SLOT(parameterChange()) );
    connect( min_btn, SIGNAL( clicked() ), this, SLOT( parameterChange() ) );
    connect( max_btn, SIGNAL( clicked() ), this, SLOT( parameterChange() ) );

    connect(range_min_input, SIGNAL(valueChanged(double)), this, SLOT( originalParaChange()) );
    connect(range_max_input, SIGNAL(valueChanged(double)), this, SLOT( originalParaChange()) );
    connect(individual_num_input, SIGNAL( valueChanged(int) ), this, SLOT( originalParaChange() ) );

    connect( dateback_btn, SIGNAL( clicked() ), this, SLOT( dateBack() ) );

}

MainWindow::~MainWindow()
{
    delete original_plot;
    delete nth_gen_population_plot;
    delete select_operator_plot;
    delete crossover_operator_plot;
    delete mutation_operator_plot;

    delete dateback_btn;
    delete dateback_sbox;

    delete function_combo;
    delete parameter_input_window;
    delete individual_num_input;
    delete generation_num_input;
    delete crossover_rate_input;
    delete mutation_rate_input;
    delete range_min_input;
    delete range_max_input;
    delete check_original_fun;
    delete start_evolve;
    delete min_btn;
    delete max_btn;
}

void MainWindow::parameterChange()
{
    start_evolve->setDisabled( false );
}

void MainWindow::originalParaChange()
{
    check_original_fun->setDisabled( false );
}

void MainWindow::checkOriginal()
{
    Population population( this->individual_num_input->value(), this->crossover_rate_input->value(), mutation_rate_input->value(),
                         this->range_min_input->value(), this->range_max_input->value(), min_btn->isChecked() ? SELECT_MIN : SELECT_MAX);

    qint16 size = population.getIndividualNum();
    QPolygonF samples;
    for( qint16 i = 0; i < size; i ++)
    {
        samples += QPointF( population.getValueAt( i ), UtilityFunction::f( population.getValueAt( i ) ) );
    }

    original_plot->setSamples( samples );

    check_original_fun->setDisabled( true );

}

void MainWindow::startEvolve()
{
    this->dateback_sbox->setDisabled( true );
    this->dateback_btn->setDisabled( true );

    nth_gen_indi.clear();
    select_operator_indi.clear();
    crossover_operator_indi.clear();
    mutation_operator_indi.clear();

    Population population( this->individual_num_input->value(), this->crossover_rate_input->value(), mutation_rate_input->value(),
                         this->range_min_input->value(), this->range_max_input->value(), min_btn->isChecked() ? SELECT_MIN : SELECT_MAX);

    qint16 gene_num = generation_num_input->value();

    population.calculate_fitness();

    for( qint16 i = 0; i < gene_num; i++)
    {
        saveNthSample( population );
        populateNthPlot( i, nth_gen_indi.at( i ) );

        population.select_operator_elitist_strategy();
        //population.select_operator_roulette();
        saveSelectSample( population );
        populateSelectPlot( select_operator_indi.at( i ) );

        population.crossover_operator();
        population.calculate_fitness();
        saveCrossSample( population );
        populateCrossPlot( crossover_operator_indi.at( i ) );

        population.mutate_operator();
        population.calculate_fitness();
        saveMutationSample( population );
        populateMutationPlot( mutation_operator_indi.at( i ) );
    }

    start_evolve ->setDisabled( true );
    dateback_btn->setDisabled( false );
    dateback_sbox->setDisabled( false );
    dateback_sbox->setRange( 1, gene_num );

    qint16 best_indi_index = population.find_best_individual_index();
    qreal best_x_value = population.getValueAt( best_indi_index );
    qreal best_y_value = UtilityFunction::f( population.getValueAt( best_indi_index ) );

    QString x_value, y_value, result_string;
    x_value.setNum(best_x_value, 'f', 5);
    y_value.setNum(best_y_value, 'f', 5);
    result_string.append( QString("当x=") ).append( x_value ).append(QString("时 ")).append( min_btn->isChecked() ? QString( "最小值y=") : QString("最大值y=") ).append( y_value);
    result_label->setVisible( true );
    result_label->setText( result_string );
}


void MainWindow::dateBack()
{
    qint16 nth = dateback_sbox->value();
    populateNthPlot( nth - 1, nth_gen_indi.at( nth - 1 ) );
    populateSelectPlot( select_operator_indi.at( nth - 1 ) );
    populateCrossPlot( crossover_operator_indi.at( nth - 1 ) );
    populateMutationPlot( mutation_operator_indi.at( nth - 1 ) );
}


void MainWindow::saveNthSample(const Population &p)
{
    QPolygonF samples;
    qint16 size = p.getIndividualNum();
    for( qint16 i = 0; i < size; i ++)
    {
        samples += QPointF( p.getValueAt( i ), p.getFitnessAt( i ) );
    }
    nth_gen_indi.append( samples );
}

void MainWindow::saveSelectSample(const Population &p)
{
    QPolygonF samples;
    qint16 size = p.getIndividualNum();
    for( qint16 i = 0; i < size; i ++)
    {
        samples += QPointF( p.getValueAt( i ), p.getFitnessAt( i ) );
    }
    select_operator_indi.append( samples );
}

void MainWindow::saveCrossSample(const Population &p)
{
    QPolygonF samples;
    qint16 size = p.getIndividualNum();
    for( qint16 i = 0; i < size; i ++)
    {
        samples += QPointF( p.getValueAt( i ), p.getFitnessAt( i ) );
    }
    crossover_operator_indi.append( samples );
}

void MainWindow::saveMutationSample(const Population &p)
{
    QPolygonF samples;
    qint16 size = p.getIndividualNum();
    for( qint16 i = 0; i < size; i ++)
    {
        samples += QPointF( p.getValueAt( i ), p.getFitnessAt( i ) );
    }
    mutation_operator_indi.append( samples );
}

void MainWindow::populateNthPlot( qint16 nth, const QPolygonF &sample )
{
    QString nth_str;
    nth_str.setNum( nth + 1 );
    QString di_str("第");
    QString dai_str("代个体");
    di_str += nth_str;
    di_str += dai_str;

    nth_gen_population_plot->setTitle( di_str );
    nth_gen_population_plot->setSamples( sample );

}

void MainWindow::populateSelectPlot(const QPolygonF &sample)
{
    select_operator_plot->setSamples( sample );
}

void MainWindow::populateCrossPlot(const QPolygonF &sample)
{
    crossover_operator_plot->setSamples( sample );
}

void MainWindow::populateMutationPlot(const QPolygonF &sample)
{
    mutation_operator_plot->setSamples( sample );
}

void MainWindow::createMainWindow()
{
    QLabel * select_fun_label = new QLabel( "请选择目标函数:" );

    this->function_combo = new QComboBox( this );
    this->function_combo->addItem( "f(x)=0.4 + sinc(4x) + 1.1*sinc(4x+2)+0.8*sinc(6x-2)+0.7*sinc(6x-4)" );

    QLabel * dateback_label = new QLabel("回溯至");
    dateback_btn = new QPushButton( this );
    dateback_btn->setDefault( true );
    dateback_btn->setText("回溯");
    dateback_sbox = new QSpinBox( this );
    dateback_sbox->setValue(1);
    dateback_sbox ->setPrefix( "第" );
    dateback_sbox->setSuffix( "代" );
    dateback_btn->setDisabled( true );
    dateback_sbox->setDisabled( true );

    QHBoxLayout * top_layout = new QHBoxLayout;
    top_layout->addWidget( select_fun_label );
    top_layout->addWidget( function_combo );
    top_layout->addStretch();
    top_layout->addWidget( dateback_label );
    top_layout->addWidget( dateback_sbox );
    top_layout->addWidget( dateback_btn );


    QwtSymbol *symbol = new QwtSymbol( QwtSymbol::Ellipse,
                                       QBrush( QColor( "#336666") ), QPen( QColor( "#336666") ), QSize( 4, 4 ) );

    nth_gen_population_plot = new GAPlot( this );
    nth_gen_population_plot->setTitle("第n代个体");
    nth_gen_population_plot->setAxisTitle(QwtPlot::xBottom, "value");
    nth_gen_population_plot->setAxisTitle(QwtPlot::yLeft, "fitness");
    nth_gen_population_plot->setSymbol( symbol );

    select_operator_plot = new GAPlot( this );
    select_operator_plot->setTitle("选择算子");
    select_operator_plot->setAxisTitle(QwtPlot::xBottom, "value");
    select_operator_plot->setAxisTitle(QwtPlot::yLeft, "fitness");
    select_operator_plot->setSymbol( symbol );

    crossover_operator_plot = new GAPlot( this );
    crossover_operator_plot->setTitle("交叉算子");
    crossover_operator_plot->setAxisTitle(QwtPlot::xBottom, "value");
    crossover_operator_plot->setAxisTitle(QwtPlot::yLeft, "fitness");
    crossover_operator_plot->setSymbol( symbol );

    mutation_operator_plot = new GAPlot( this );
    mutation_operator_plot->setTitle("变异算子");
    mutation_operator_plot->setAxisTitle(QwtPlot::xBottom, "value");
    mutation_operator_plot->setAxisTitle(QwtPlot::yLeft, "fitness");
    mutation_operator_plot->setSymbol( symbol );

    QGridLayout * plots_layout = new QGridLayout( this );
    plots_layout->addWidget( nth_gen_population_plot, 0, 0 );
    plots_layout->addWidget( select_operator_plot, 0, 1 );
    plots_layout->addWidget( crossover_operator_plot, 1, 0);
    plots_layout->addWidget(mutation_operator_plot, 1, 1 );

    QVBoxLayout * main_layout = new QVBoxLayout;
    main_layout->addLayout( top_layout );
    main_layout->addLayout( plots_layout );

    QWidget * main_widget = new QWidget( this );
    main_widget->setLayout( main_layout );

    this->setWindowTitle("遗传算法演示程序");

    this->setCentralWidget( main_widget );

}

void MainWindow::createParameterInputWindow()
{
    parameter_input_window = new QDockWidget( this );
    parameter_input_window->setWindowTitle("输入遗传参数");
    parameter_input_window->setFeatures(QDockWidget::DockWidgetFloatable);

    QLabel * individual_num_label = new QLabel("个体数： ");
    individual_num_input = new QSpinBox( this );
    individual_num_input->setRange( 0, 500000 );
    individual_num_input->setSingleStep( 10 );
    individual_num_input->setValue( 100 );
    QHBoxLayout * individual_layout = new QHBoxLayout( this );
    individual_layout->addWidget( individual_num_label );
    individual_layout->addWidget( individual_num_input );

    QLabel * generation_num_label = new QLabel("世代数：");
    generation_num_input = new QSpinBox( this );
    generation_num_input->setRange( 0, 500000 );
    generation_num_input->setSingleStep( 10 );
    generation_num_input->setValue( 100 );
    QHBoxLayout * generation_layout = new QHBoxLayout( this );
    generation_layout->addWidget( generation_num_label );
    generation_layout->addWidget( generation_num_input );

    QLabel * crossover_rate_lable = new QLabel("交叉率：");
    crossover_rate_input = new QDoubleSpinBox( this );
    crossover_rate_input->setRange( 0.2 , 0.8 );
    crossover_rate_input->setSingleStep( 0.1 );
    crossover_rate_input->setValue( 0.5 );
    QHBoxLayout * crossover_layout = new QHBoxLayout( this );
    crossover_layout->addWidget( crossover_rate_lable );
    crossover_layout->addWidget(crossover_rate_input );

    QLabel * mutation_rate_label = new QLabel("变异率：");
    mutation_rate_input = new QDoubleSpinBox(this);
    mutation_rate_input->setRange(0.01, 0.05);
    mutation_rate_input->setSingleStep( 0.01 );
    mutation_rate_input->setValue( 0.02 );
    QHBoxLayout * mutation_layout = new QHBoxLayout( this );
    mutation_layout->addWidget( mutation_rate_label );
    mutation_layout->addWidget( mutation_rate_input );

    QGroupBox * range_group = new QGroupBox("输入种群取值范围");
    QLabel * range_min_label = new QLabel("下限：");
    QLabel * range_max_label = new QLabel("上限：");
    range_min_input = new QDoubleSpinBox(this);
    range_max_input = new QDoubleSpinBox(this);
    range_min_input->setValue( -1.0 );
    range_max_input->setValue( 1.0 );
    range_min_input->setRange( -1000000, 1000000);
    range_max_input->setRange( -1000000, 1000000);
    range_min_input->setSingleStep( 1.0 );
    range_max_input->setSingleStep( 1.0 );
    QGridLayout * range_layout = new QGridLayout( this );
    range_layout->addWidget(range_min_label, 0, 0);
    range_layout->addWidget(range_min_input, 0, 1);
    range_layout->addWidget(range_max_label, 1, 0);
    range_layout->addWidget(range_max_input, 1, 1);
    range_group->setLayout( range_layout );

    QGroupBox * limits_box = new QGroupBox("求最大值or最小值");
    min_btn = new QRadioButton("最小值" );
    max_btn = new QRadioButton("最大值");
    max_btn->setChecked( true );
    QHBoxLayout * limits_layout = new QHBoxLayout();
    limits_layout->addWidget( min_btn );
    limits_layout->addWidget( max_btn );
    limits_box->setLayout( limits_layout );

    start_evolve = new QPushButton( "开始进化", this );
    check_original_fun = new QPushButton("查看原函数", this );
    QHBoxLayout * btn_layout = new QHBoxLayout;
    btn_layout->addWidget( check_original_fun );
    btn_layout->addWidget( start_evolve );

    original_plot = new GAPlot( this );
    original_plot->setTitle( "原函数" );
    original_plot->setSymbol( NULL );
    original_plot->setAxisTitle( QwtPlot::xBottom, "x" );
    original_plot->setAxisTitle(QwtPlot::yLeft, "y");

    result_label = new QLabel( this );
    result_label->setVisible( false );

    QVBoxLayout * left_main_layout = new QVBoxLayout( this );
    left_main_layout->addLayout( individual_layout );
    left_main_layout->addLayout( generation_layout );
    left_main_layout->addLayout( crossover_layout );
    left_main_layout->addLayout( mutation_layout );
    left_main_layout->addWidget( range_group );
    left_main_layout->addWidget( limits_box );
    left_main_layout->addLayout( btn_layout );
    left_main_layout->addWidget(original_plot);
    left_main_layout->addStretch();
    left_main_layout->addWidget( result_label );

    QWidget * left_main_widget = new QWidget( this );
    left_main_widget->setLayout( left_main_layout );

    parameter_input_window->setWidget( left_main_widget );
    parameter_input_window->setMinimumSize( left_main_layout->sizeHint() );
    //parameter_input_window->setStyleSheet("border:1px solid black;");
    this->addDockWidget( Qt::LeftDockWidgetArea, parameter_input_window );
}

