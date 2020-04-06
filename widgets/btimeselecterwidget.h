#ifndef BTIMESELECTERWIDGET_H
#define BTIMESELECTERWIDGET_H

#include <QWidget>

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
protected slots:
    void on_modeComboBox_currentIndexChanged(int i);
    void on_doubleIntervalButton_toggled(bool b);
private:
    Ui::BTimeSelecterWidget *ui;
};

#endif // BTIMESELECTERWIDGET_H
