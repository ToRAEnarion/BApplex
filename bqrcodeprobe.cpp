#include "bqrcodeprobe.h"
#include <QDebug>
#include <QLabel>

BQrCodeProbe::BQrCodeProbe(QObject *object) : QVideoProbe(object), Thread(this), IsDecoding(false)
{    
    Engine = new QZXing(object);
    Engine->moveToThread(&Thread);
    connect(&Thread, SIGNAL(finished()), Engine, SLOT(deleteLater()));
    connect(this, SIGNAL(videoFrameProbed(QVideoFrame)), this, SLOT(processFrame(QVideoFrame)));
    connect(Engine, SIGNAL(tagFound(QString)), this, SLOT(onTagFound(QString)));
    connect(Engine, SIGNAL(decodingStarted()), this, SLOT(onDecodingStarted()));
    connect(Engine, SIGNAL(decodingFinished(bool)), this, SLOT(onDecodingFinished(bool)));
    Engine->setDecoder(QZXing::DecoderFormat_QR_CODE | QZXing::DecoderFormat_EAN_13);
}

BQrCodeProbe::~BQrCodeProbe()
{
    Thread.quit();
    Thread.wait();
}

void BQrCodeProbe::processFrame(QVideoFrame frame)
{
    QImage img = imageFromVideoFrame(frame);

    if(IsDecoding)
        return;
/*
if(ii == 0)
{
    QLabel* label = new QLabel();
    label->setPixmap(QPixmap::fromImage(img));
    label->show();
    ii++;
}*/

    QString res = Engine->decodeImage(img,-1,-1,true);
    if(res != "")
        qDebug()<<"res="<<res;
}

void BQrCodeProbe::onTagFound(QString tag)
{
    if(tag != "")
        qDebug()<<"new tag :"<<tag;

    emit textExtracted(tag);
}

void BQrCodeProbe::onDecodingStarted()
{
    IsDecoding = true;
}

void BQrCodeProbe::onDecodingFinished(bool b)
{
    IsDecoding = false;
}

QImage BQrCodeProbe::imageFromVideoFrame(const QVideoFrame& buffer) const
{
    QImage img;
    QVideoFrame frame(buffer);  // make a copy we can call map (non-const) on
    frame.map(QAbstractVideoBuffer::ReadOnly);
    QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(
                frame.pixelFormat());
    // BUT the frame.pixelFormat() is QVideoFrame::Format_Jpeg, and this is
    // mapped to QImage::Format_Invalid by
    // QVideoFrame::imageFormatFromPixelFormat
    if (imageFormat != QImage::Format_Invalid) {
        img = QImage(frame.bits(),
                     frame.width(),
                     frame.height(),
                     // frame.bytesPerLine(),
                     imageFormat);
    } else {
        // e.g. JPEG
        int nbytes = frame.mappedBytes();
        img = QImage::fromData(frame.bits(), nbytes);
    }
    frame.unmap();
    img = img.mirrored(false, true);
    return img;
}
