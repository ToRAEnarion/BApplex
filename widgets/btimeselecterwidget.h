#ifndef BTIMESELECTERWIDGET_H
#define BTIMESELECTERWIDGET_H

#include <QWidget>
#include "../bconnecteditem.h"

namespace Ui {
class BTimeSelecterWidget;
}

class BTimeSelecterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BTimeSelecterWidget(QWidget *parent = nullptr);
    ~BTimeSelecterWidget();
    void updateVisibility();
    QString stringValue();
    void setValue(const QString str);

    static bool updateMethod(BConnectedItem* item);
protected slots:
    void on_modeComboBox_currentIndexChanged(int i);
    void on_doubleIntervalButton_toggled(bool b);
private:
    Ui::BTimeSelecterWidget *ui;
};

#endif // BTIMESELECTERWIDGET_H
