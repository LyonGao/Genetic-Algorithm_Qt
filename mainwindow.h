#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>

class GAPlot;
class QComboBox;
class QLabel;
class QDockWidget;
class QDoubleSpinBox;
class QSpinBox;
class QPushButton;
class QRadioButton;
class Population;
#include<QPolygonF>
#include<QList>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget * parent = 0 );
    ~MainWindow();

    void createParameterInputWindow();
    void createMainWindow();

    void populateNthPlot( qint16 nth, const QPolygonF & sample );
    void populateSelectPlot( const QPolygonF & sample );
    void populateCrossPlot( const QPolygonF & sample );
    void populateMutationPlot( const QPolygonF & sample );

    void saveNthSample( const Population & p);
    void saveSelectSample( const Population & p);
    void saveCrossSample( const Population & p );
    void saveMutationSample( const Population & p );

public slots:
    void parameterChange();
    void originalParaChange();
    void checkOriginal();
    void startEvolve();
    void dateBack();

private:
    GAPlot * original_plot; //原函数图表
    GAPlot * nth_gen_population_plot;//n代种群图表
    GAPlot * select_operator_plot;//选择算子后的种群图表
    GAPlot * crossover_operator_plot;//交叉算子后的种群图表
    GAPlot * mutation_operator_plot;//变异算子后的种群图表

    QPushButton * dateback_btn;//回溯按钮
    QSpinBox * dateback_sbox;//回溯选择框

    QComboBox * function_combo;//函数选择框
    QDockWidget * parameter_input_window;//遗传参数输入窗口
    QSpinBox * individual_num_input;//个体数输入控件
    QSpinBox * generation_num_input;//遗传世代数输入控件
    QDoubleSpinBox * crossover_rate_input;//交叉率输入控件
    QDoubleSpinBox * mutation_rate_input;//变异率输入控件
    QDoubleSpinBox * range_min_input;//个体取值下限输入控件
    QDoubleSpinBox * range_max_input;//个体取值上限输入控件
    QPushButton * check_original_fun;//查看原函数按钮
    QPushButton * start_evolve;//开始进化按钮
    QRadioButton * min_btn;//取最小值按钮
    QRadioButton * max_btn;//取最大值按钮
    QLabel * result_label;//结果标签

    QList<QPolygonF> nth_gen_indi;
    QList<QPolygonF> select_operator_indi;
    QList<QPolygonF> crossover_operator_indi;
    QList<QPolygonF> mutation_operator_indi;

};


#endif // MAINWINDOW_H





