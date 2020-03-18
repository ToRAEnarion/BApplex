#ifndef BVIDEOQRCODESURFACE_H
#define BVIDEOQRCODESURFACE_H

#include <QAbstractVideoSurface>

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QVideoSurfaceFormat>
#include <QWidget>

QImage qt_imageFromVideoFrame( const QVideoFrame& f );

class BVideoQRCodeSurface : public QAbstractVideoSurface
{
public:
    BVideoQRCodeSurface(QWidget *widget, QObject *parent = nullptr);
    bool present(const QVideoFrame &frame);
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType type = QAbstractVideoBuffer::NoHandle) const;
    QImage toImage(const QVideoFrame &videoFrame);
    void updateVideoRect();
    void paint(QPainter *painter);
    bool start(const QVideoSurfaceFormat &format);

    QRect videoRect() const { return TargetRect; }
private:
    QWidget* Widget;
    QRect TargetRect;
    QRect SourceRect;
    QSize ImageSize;
    QVideoFrame CurrentFrame;
    QImage::Format ImageFormat;
};

#endif // BVIDEOQRCODESURFACE_H
