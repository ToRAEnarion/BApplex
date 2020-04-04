#include "baddbyipdialog.h"
#include "ui_baddbyipdialog.h"

BAddByIpDialog::BAddByIpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BAddByIpDialog)
{
    ui->setupUi(this);
}

BAddByIpDialog::~BAddByIpDialog()
{
    delete ui;
}

QHostAddress BAddByIpDialog::ip() const
{
    return QHostAddress(ui->ipEdit->text());
}
