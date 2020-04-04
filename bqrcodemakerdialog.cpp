#include "bqrcodemakerdialog.h"
#include "ui_bqrcodemakerdialog.h"

#include <QColor>
#include <QPainter>
#include <QColorDialog>

BQrCodeMakerDialog::BQrCodeMakerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BQrCodeMakerDialog)
{
    ui->setupUi(this);
    Engine = new QZXing(this);
    Scene = new QGraphicsScene(this);
    ui->resultImage->setScene(Scene);

    GradColor1 = Qt::black;
    GradColor2 = Qt::black;
    Radius = 60;

    QPixmap px1(48,24);
    px1.fill(GradColor1);
    ui->gradColor1->setIcon(QIcon(px1));
    QPixmap px2(48,24);
    px2.fill(GradColor2);
    ui->gradColor2->setIcon(QIcon(px2));

    ui->radius->setValue(Radius);
}

BQrCodeMakerDialog::~BQrCodeMakerDialog()
{
    delete ui;
}

void BQrCodeMakerDialog::on_generate_pressed()
{
    QString typeStr = "";
    if(ui->typeKey->currentIndex() > 0)
    {
        typeStr = "T:WEP;";
    }
    QString wifi = "WIFI:S:"+ui->ssid->text()+";"+typeStr+"P:"+ui->password->text()+";;";
    ui->result->setText(wifi);

    QZXingEncoderConfig configXing(QZXing::EncoderFormat_QR_CODE, QSize(120,120),QZXing::EncodeErrorCorrectionLevel_L, true);
    QImage qrImg = Engine->encodeData(wifi, configXing);
    for(int i=0;i<qrImg.width();i++)
    {
        for(int j=0;j<qrImg.height();j++)
        {
            if(qrImg.pixelColor(i,j).lightness()<125)
            {
                qrImg.setPixelColor(i,j,QColor(0,0,0,0));
            }
        }
    }

    QImage imgResult(qrImg.width(), qrImg.height(), QImage::Format_ARGB32);

    QPainter painter(&imgResult);
    QRadialGradient grad(imgResult.width()/2, imgResult.height()/2, 0.01*imgResult.width()*Radius);
    grad.setColorAt(0, GradColor1);
    grad.setColorAt(1, GradColor2);
    painter.fillRect(imgResult.rect(), QBrush(grad));
    painter.drawImage(qrImg.rect(), qrImg);
    painter.end();

    Scene->clear();
    Scene->addPixmap(QPixmap::fromImage(imgResult));
    ui->resultImage->update();
}

void BQrCodeMakerDialog::on_gradColor1_pressed()
{
    GradColor1 = QColorDialog::getColor(GradColor1, this);
    QPixmap px(48,24);
    px.fill(GradColor1);
    ui->gradColor1->setIcon(QIcon(px));
}

void BQrCodeMakerDialog::on_gradColor2_pressed()
{
    GradColor2 = QColorDialog::getColor(GradColor2, this);
    QPixmap px(48,24);
    px.fill(GradColor2);
    ui->gradColor2->setIcon(QIcon(px));
}

void BQrCodeMakerDialog::on_radius_valueChanged(int i)
{
    Radius = i;
}
