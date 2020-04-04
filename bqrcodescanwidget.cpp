#include "bqrcodescanwidget.h"

#include <QCameraInfo>
#include <QPainter>
#include <QPaintEvent>
#include <QRegion>
#include <QCameraImageCapture>
#include <QMediaPlayer>
#include <QMediaPlaylist>

BQrCodeScanWidget::BQrCodeScanWidget(QWidget *parent) : QCameraViewfinder (parent), Camera(nullptr), LastTag("")
{
    const QList<QCameraInfo> cameras = QCameraInfo::availableCameras();

    for (const QCameraInfo &cameraInfo : cameras)
    {

        qDebug()<<"init "<<cameraInfo.deviceName();
        Camera = new QCamera(cameraInfo, this);
        Camera->setViewfinder(this);


        Probe = new BQrCodeProbe(this);
        Probe->setSource(Camera);
        connect(Probe, SIGNAL(textExtracted(QString)), this, SLOT(onTextExtracted(QString)));

        Camera->setCaptureMode(QCamera::CaptureVideo);

        qDebug()<<"start cam"<<cameraInfo.deviceName();
        Camera->start();
        break;
    }
}

void BQrCodeScanWidget::paintEvent(QPaintEvent *event)
{
    QCameraViewfinder::paintEvent(event);
    if(LastTag != "")
    {
        QPainter painter(this);
        painter.setPen(QColor(10,130,0));
        QFont f = painter.font();
        f.setPixelSize(30);
        painter.setFont(f);

        painter.drawText(10,50, LastTag);
    }

}

void BQrCodeScanWidget::onTextExtracted(QString tag)
{
    LastTag = tag;
    emit textExtracted(tag);
}

