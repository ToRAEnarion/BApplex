#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include "mainwindow.h"
#include "bqrcodedialog.h"

SettingsDialog::SettingsDialog(MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    ui->localWifi->setText(parent->getLocalSSID());
    ui->passWord->setText(parent->getLocalPassWord());
    ui->keytypecombobox->setCurrentIndex(parent->getLocalType());
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

QString SettingsDialog::ssid() const
{
    return ui->localWifi->text();
}

QString SettingsDialog::password() const
{
    return ui->passWord->text();
}

int SettingsDialog::keytype() const
{
    return ui->keytypecombobox->currentIndex();
}

void SettingsDialog::on_qr_button_clicked()
{
    BQrCodeDialog d(this);
    if(d.exec())
    {
        NetworkRegistration net = d.foundSSID();
        if(!net.SSID.isEmpty())
        {
            ui->localWifi->setText(net.SSID);
            ui->passWord->setText(net.Pass);
            ui->keytypecombobox->setCurrentIndex(net.KeyType);
        }
    }
}
