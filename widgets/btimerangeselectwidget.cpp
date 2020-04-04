#include "btimerangeselectwidget.h"
#include <QPainter>
#include <QRadialGradient>
#include <QtMath>
#include <QMouseEvent>
#include <QDebug>

BTimeRangeSelectWidget::BTimeRangeSelectWidget(QWidget* parent) : QWidget(parent)
{
    Color = Qt::red;
    Start1 = 1000;
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
    grad.setColorAt(0.85, Color.darker(120));
    grad.setColorAt(0.92, Color.darker(90));
    grad.setColorAt(1, Color.darker(120));
    p.setBrush(QBrush(grad));
    p.drawEllipse(rcF);

    p.setBrush(QBrush(Qt::white));
    p.drawEllipse(ctr, 0.36*Radius, 0.36*Radius);

    p.setPen(QPen(Qt::black, 2));
    p.setBrush(CurrentDrag == 1 ? Qt::green : Qt::white);
    p.drawEllipse(makeRect(Start1));
}

void BTimeRangeSelectWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    QPointF pClic = event->localPos();
    qDebug()<<pClic;
    if(makeRect(Start1).contains(pClic))
    {
        CurrentDrag = 1;
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
        qreal y = event->localPos().x() - Center.y();
        if(qAbs(x)<1e-5)
        {
            Start1 = y<0 ? 0 : 620;
        }
        else
        {
        }
    }
}

QRectF BTimeRangeSelectWidget::makeRect(int val)
{
    qreal cSize = 0.07*Radius;
    QPointF ctr = Center + QPointF(0.37*Radius*qCos(1.0*val / 1440), 0.37*Radius*qSin(1.0*val / 1440));
    return QRectF(ctr.rx() - cSize,ctr.ry()-cSize, 2*cSize, 2*cSize);
}
