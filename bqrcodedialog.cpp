#include "bqrcodedialog.h"

#include <QVBoxLayout>

BQrCodeDialog::BQrCodeDialog(QWidget *parent) : QDialog (parent)
{
    BQrCodeScanWidget* w = new BQrCodeScanWidget(this);
    connect(w, SIGNAL(textExtracted(QString)), this, SLOT(onTextExtracted(QString)));
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(w);
    setLayout(layout);
}

QString BQrCodeDialog::foundText()
{
    return Text;
}

void BQrCodeDialog::onTextExtracted(QString tag)
{
    Text = tag;
    accept();
}
