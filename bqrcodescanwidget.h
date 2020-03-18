#ifndef BQRCODESCANWIDGET_H
#define BQRCODESCANWIDGET_H

#include <QWidget>
#include <QCameraViewfinder>
#include <QVideoWidget>
#include <QCamera>
#include "bqrcodeprobe.h"

class BQrCodeScanWidget : public QCameraViewfinder
{
    Q_OBJECT
public:
    BQrCodeScanWidget(QWidget* parent =nullptr);
    QCamera* getCamera()const{return Camera;}
protected:
    void paintEvent(QPaintEvent *event);
private slots:
    void onTextExtracted(QString tag);
signals:
    void textExtracted(QString);
private:
    BQrCodeProbe* Probe;
    QCamera* Camera;
    QString LastTag;
};

#endif // BQRCODESCANWIDGET_H
