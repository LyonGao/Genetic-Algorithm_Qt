#include"ga_plot.h"
#include<qwt_plot_panner.h>
#include<qwt_plot_magnifier.h>
#include <qwt_plot_curve.h>
#include<qwt_symbol.h>
#include<qwt_curve_fitter.h>
#include<QColor>

GAPlot::GAPlot(QWidget *parent):
    QwtPlot(parent)
{
    this->setCanvasBackground( Qt::white );
    //设置图表的样式
    canvas()->setStyleSheet(
        "border: 1px solid black;"
        "background-color:  #dddddd;"
                );

    // attach curve
    d_curve = new QwtPlotCurve;
    d_curve->setPen( QColor( "Purple" ) );

    // when using QwtPlotCurve::ImageBuffer simple dots can be
    // rendered in parallel on multicore systems.
    d_curve->setRenderThreadCount( 0 ); // 0: use QThread::idealThreadCount()

    d_curve->attach( this );

    // panning with the left mouse button
    (void )new QwtPlotPanner( canvas() );

    // zoom in/out with the wheel
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier( canvas() );
    magnifier->setMouseButton( Qt::NoButton );

    setAutoReplot( true );

}

GAPlot::~GAPlot()
{
    delete d_curve;
}

void GAPlot::setSymbol( QwtSymbol * symbol )
{
    d_curve->setSymbol( symbol );
    d_curve->setStyle( QwtPlotCurve::NoCurve );

    if ( symbol == NULL )
    {
        QColor curve_color("#336666");
        d_curve->setPen( QPen( curve_color ) );
        d_curve->setStyle( QwtPlotCurve::Lines );
        d_curve->setCurveAttribute( QwtPlotCurve::Fitted, true );
    }
}

void GAPlot::setSamples(const QVector<QPointF> &samples)
{
    d_curve->setPaintAttribute(
        QwtPlotCurve::ImageBuffer, samples.size() > 1000 );

    d_curve->setSamples( samples );
}













