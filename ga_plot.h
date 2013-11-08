#ifndef GA_PLOT_H
#define GA_PLOT_H

#include<qwt_plot.h>

class QwtPlotCurve;
class QwtSymbol;

class GAPlot : public QwtPlot
{
    Q_OBJECT

public:
    GAPlot( QWidget *parent = NULL );
    ~GAPlot();

    void setSymbol( QwtSymbol * );
    void setSamples( const QVector<QPointF> &samples );

private:
    QwtPlotCurve *d_curve;
};

#endif // GA_PLOT_H
