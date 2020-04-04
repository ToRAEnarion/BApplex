#ifndef BTIMERANGESELECTWIDGET_H
#define BTIMERANGESELECTWIDGET_H

#include <QWidget>

class BTimeRangeSelectWidget : public QWidget
{
    Q_OBJECT
public:
    BTimeRangeSelectWidget(QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    QRectF makeRect(int val);

    QColor Color;
    int Start1;
    int End1;

    int CurrentDrag;
    QPointF Center;
    qreal Radius;
};

#endif // BTIMERANGESELECTWIDGET_H
