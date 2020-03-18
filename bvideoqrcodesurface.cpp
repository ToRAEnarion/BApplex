#include "bvideoqrcodesurface.h"
#include <QPainter>
#include <QDebug>

BVideoQRCodeSurface::BVideoQRCodeSurface(QWidget *widget, QObject *parent)
    :QAbstractVideoSurface(parent), Widget(widget),ImageFormat(QImage::Format_Invalid)
{

}

bool BVideoQRCodeSurface::start(const QVideoSurfaceFormat &format)
{
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();

    if (imageFormat != QImage::Format_Invalid && !size.isEmpty()) {
        this->ImageFormat = imageFormat;
        ImageSize = size;
        SourceRect = format.viewport();

        bool b = QAbstractVideoSurface::start(format);

        if(b)
        {
            Widget->updateGeometry();
            updateVideoRect();
        }
        else
        {
            qDebug()<<"Start failed : "<<error();
        }

        return b;
    } else {
        return false;
    }
}

bool BVideoQRCodeSurface::present(const QVideoFrame &frame)
{
    if (surfaceFormat().pixelFormat() != frame.pixelFormat()
            || surfaceFormat().frameSize() != frame.size()) {
        setError(IncorrectFormatError);
        qDebug()<<"stoped";
        stop();

        return false;
    } else {
        CurrentFrame = frame;

        qDebug()<<frame.size();
        Widget->repaint(TargetRect);

        return true;
    }
}


void BVideoQRCodeSurface::updateVideoRect()
{
    QSize size = surfaceFormat().sizeHint();
    size.scale(Widget->size().boundedTo(size), Qt::KeepAspectRatio);

    TargetRect = QRect(QPoint(0, 0), size);
    TargetRect.moveCenter(Widget->rect().center());
}

QList<QVideoFrame::PixelFormat> BVideoQRCodeSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const
{
    if (type == QAbstractVideoBuffer::NoHandle) {
        return QList<QVideoFrame::PixelFormat>()
                << QVideoFrame::Format_RGB32
                << QVideoFrame::Format_ARGB32
                << QVideoFrame::Format_ARGB32_Premultiplied
                << QVideoFrame::Format_RGB565
                << QVideoFrame::Format_RGB555;
    } else {
        return QList<QVideoFrame::PixelFormat>();
    }
}

QImage BVideoQRCodeSurface::toImage( const QVideoFrame& videoFrame )
{
    if ( videoFrame.handleType() == QAbstractVideoBuffer::NoHandle )
    {
        QImage image= qt_imageFromVideoFrame( videoFrame );
        if ( image.isNull() )
        {
            return QImage();
        }

        if ( image.format() != QImage::Format_ARGB32 )
        {
            image = image.convertToFormat( QImage::Format_ARGB32 );
        }

        return image;
    }

    if (videoFrame.handleType() == QAbstractVideoBuffer::GLTextureHandle )
    {
        QImage image( videoFrame.width(), videoFrame.height(), QImage::Format_ARGB32 );
        GLuint textureId = static_cast<GLuint>( videoFrame.handle().toInt() );
        QOpenGLContext* ctx = QOpenGLContext::currentContext();
        QOpenGLFunctions* f = ctx->functions();
        GLuint fbo;
        f->glGenFramebuffers( 1, &fbo );
        GLint prevFbo;
        f->glGetIntegerv( GL_FRAMEBUFFER_BINDING, &prevFbo );
        f->glBindFramebuffer( GL_FRAMEBUFFER, fbo );
        f->glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0 );
        f->glReadPixels( 0, 0,  videoFrame.width(),  videoFrame.height(), GL_RGBA, GL_UNSIGNED_BYTE, image.bits() );
        f->glBindFramebuffer( GL_FRAMEBUFFER, static_cast<GLuint>( prevFbo ) );
        return image.rgbSwapped();
    }

    return QImage();
}


void BVideoQRCodeSurface::paint(QPainter *painter)
{
    if (CurrentFrame.map(QAbstractVideoBuffer::ReadOnly)) {
        const QTransform oldTransform = painter->transform();

        if (surfaceFormat().scanLineDirection() == QVideoSurfaceFormat::BottomToTop) {
           painter->scale(1, -1);
           painter->translate(0, -Widget->height());
        }

        QImage image(
                CurrentFrame.bits(),
                CurrentFrame.width(),
                CurrentFrame.height(),
                CurrentFrame.bytesPerLine(),
                ImageFormat);

        painter->drawImage(TargetRect, image, SourceRect);

        painter->setTransform(oldTransform);

        CurrentFrame.unmap();
    }
}

