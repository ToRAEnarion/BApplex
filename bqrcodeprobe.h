#ifndef BQRCODEPROBE_H
#define BQRCODEPROBE_H

#include <QObject>
#include <QVideoProbe>
#include "QZXing/QZXing.h"
#include <QThread>

class BQrCodeProbe : public QVideoProbe
{
    Q_OBJECT
public:
    BQrCodeProbe(QObject* object=nullptr);
    ~BQrCodeProbe();
public slots:
    void processFrame(QVideoFrame processFrame);
    void onTagFound(QString tag);
    void onDecodingStarted();
    void onDecodingFinished(bool b);
signals:
    void textExtracted(QString);
private:
    QZXing* Engine;
    QThread Thread;
    bool IsDecoding;
    QImage imageFromVideoFrame(const QVideoFrame &buffer) const;
};

#endif // BQRCODEPROBE_H
