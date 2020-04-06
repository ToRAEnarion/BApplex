#include "btimerangeselectwidget.h"
#include <QPainter>
#include <QRadialGradient>
#include <QtMath>
#include <QMouseEvent>
#include <QDebug>

#define PI 3.141593

BTimeRangeSelectWidget::BTimeRangeSelectWidget(QWidget* parent) : QWidget(parent)
{
    Color = Qt::blue;
    Start1 = 720;
}

void BTimeRangeSelectWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    qreal xOffset = 0;
    qreal yOffset = 0;
    if(width()>height())
    {
        Radius = height();
        xOffset = (width() - height())/2;
    }
    else
    {
        Radius = width();
        yOffset = (height() - width())/2;
    }

    QPointF ctr(0.5*this->width(),0.5*this->height());
    QRectF rcF(xOffset + 0.1*Radius, yOffset + 0.1*Radius, 0.8*Radius, 0.8*Radius);

    Center = rcF.center();

    QPainter p(this);

    QRadialGradient grad(rcF.center(), 0.4*Radius);
    grad.setColorAt(0.85, QColor(25,25,25));
    grad.setColorAt(0.95, QColor(70,70,70));
    grad.setColorAt(1, QColor(25,25,25));
    p.setBrush(QBrush(grad));
    p.drawEllipse(rcF);

    grad = QRadialGradient(rcF.center(), 0.4*Radius);
    grad.setColorAt(0.85, Color.darker(120));
    grad.setColorAt(0.95, Color.darker(90));
    grad.setColorAt(1, Color.darker(120));
    p.setBrush(QBrush(grad));
    p.drawPie(rcF,90*16-16*360*Start1 / 1440, -16*360*(End1-Start1) / 1440);

    // center filler
    p.setBrush(QBrush(Qt::white));
    p.drawEllipse(ctr, 0.36*Radius, 0.36*Radius);

    p.setPen(QPen(Qt::black, 2));


    for (int i=0;i<24;i++)
    {
        qreal r = i%6 == 0 ? 0.47 : 0.45;
        qreal cS = cos(PI*i/12);
        qreal sS = sin(PI*i/12);
        p.drawLine(Center.x() + cS*0.42*Radius, Center.y() + sS*0.42*Radius,
                   Center.x() + cS*r*Radius, Center.y() + sS*r*Radius);
    }

    p.setBrush(CurrentDrag == 1 ? Qt::green : Qt::white);
    p.drawEllipse(makeRect(Start1));
    p.setBrush(CurrentDrag == 2 ? Qt::green : Qt::white);
    p.drawEllipse(makeRect(End1));

    p.drawText(Center, QString("%1 -> %2").arg(toString(Start1)).arg(toString(End1)));
}

QString BTimeRangeSelectWidget::toString(int i)
{
    return QString("%1 : %2").arg(i/60).arg(i%60);
}

void BTimeRangeSelectWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    QPointF pClic = event->localPos();
    if(makeRect(Start1).contains(pClic))
    {
        CurrentDrag = 1;
        update();
    }
    if(makeRect(End1).contains(pClic))
    {
        CurrentDrag = 2;
        update();
    }
    QWidget::mousePressEvent(event);
}

void BTimeRangeSelectWidget::mouseReleaseEvent(QMouseEvent *event)
{
    CurrentDrag = 0;
    update();
    QWidget::mousePressEvent(event);
}

void BTimeRangeSelectWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    if(CurrentDrag >0)
    {
        qreal x = event->localPos().x() - Center.x();
        qreal y = event->localPos().y() - Center.y();

        qreal r = acos(-x/qSqrt(x*x + y*y)) / PI;
        if(y>0)
            r = -r;

        if(CurrentDrag == 1)
            Start1 = qRound(720.0 * (1+r)+360)%1440;
        if(CurrentDrag == 2)
            End1 = qRound(720.0 * (1+r)+360)%1440;

    }
    update();
}

QRectF BTimeRangeSelectWidget::makeRect(int val)
{
    qreal cSize = 0.05*Radius;
    QPointF ctr = Center + QPointF(0.375*Radius*qCos(-0.5*PI+2.0*val*PI / 1440), 0.375*Radius*qSin(-0.5*PI+2.0*val*PI / 1440));
    return QRectF(ctr.rx() - cSize,ctr.ry()-cSize, 2*cSize, 2*cSize);
}
