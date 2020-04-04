#ifndef BQRCODEMAKERDIALOG_H
#define BQRCODEMAKERDIALOG_H

#include <QDialog>
#include "QZXing/QZXing.h"

#include <QGraphicsScene>

namespace Ui {
class BQrCodeMakerDialog;
}

class BQrCodeMakerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BQrCodeMakerDialog(QWidget *parent = nullptr);
    ~BQrCodeMakerDialog();
protected slots:
    void on_generate_pressed();
    void on_gradColor1_pressed();
    void on_gradColor2_pressed();
    void on_radius_valueChanged(int i);
private:
    Ui::BQrCodeMakerDialog *ui;
    QGraphicsScene* Scene;
    QZXing* Engine;

    QColor GradColor1;
    QColor GradColor2;
    int Radius;
};

#endif // BQRCODEMAKERDIALOG_H
