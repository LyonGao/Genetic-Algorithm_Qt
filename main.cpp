#include <QApplication>
#include<QString>
#include<QLabel>
#include<ctime>
#include<cstdlib>
#include<QtGlobal>
#include"utility_fun.h"
#include"genetic_algorithm.h"
#include"mainwindow.h"

int main( int argc, char *argv[] )
{
    QApplication a(argc, argv);

    QFont myFont("Microsoft YaHei");

    a.setFont(myFont);
    //a.setStyleSheet("font-size:35px;");

    /*Population p(10.0, 0.4, 0.1, -2, 2, SELECT_MAX);

    qint16 generation_num = 500;

    for(qint16 i = 0; i < generation_num; i++)
    {
        p.calculate_fitness();
       p.select_operator_elitist_strategy();
        p.crossover_operator();
        p.mutate_operator();
    }

    p.calculate_fitness();

    qint16 best = p.find_best_individual_index();

    qreal best_value = UtilityFunction::f( p.getValueAt( best ) );

    QString str;
    str.setNum( best_value );

    QLabel * la = new QLabel(str);

    la->show();*/

    MainWindow mainWindow;
    //mainWindow.resize();
    //mainWindow.setWindowState(Qt::WindowFullScreen);
    //mainWindow.setWindowState(Qt::WindowMaximized);
    mainWindow.show();

    return a.exec();
}
