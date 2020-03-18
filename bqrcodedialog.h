#ifndef BQRCODEDIALOG_H
#define BQRCODEDIALOG_H

#include <QDialog>
#include "bqrcodescanwidget.h"

class BQrCodeDialog : public QDialog
{
    Q_OBJECT
public:
    BQrCodeDialog(QWidget* parent = nullptr);
    QString foundText();
public slots:
    void onTextExtracted(QString tag);
private:
    QString Text;
};

#endif // BQRCODEDIALOG_H
