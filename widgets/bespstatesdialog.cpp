#include "bespstatesdialog.h"
#include "ui_bespstatesdialog.h"
#include "bapplex.h"

BEspStatesDialog::BEspStatesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BEspStatesDialog)
{
    ui->setupUi(this);
}

BEspStatesDialog::~BEspStatesDialog()
{
    delete ui;
}

QString BEspStatesDialog::v1() const
{
    return ui->value1->stringValue();
}

QString BEspStatesDialog::v2() const
{
    return ui->value2->stringValue();
}

void BEspStatesDialog::setV1(const QString &str)
{
    ui->value1->setValue(str);
}

void BEspStatesDialog::setV2(const QString &str)
{
    ui->value2->setValue(str);
}

bool BEspStatesDialog::editMethod(BConnectedItem *item, QWidget* parent)
{
    BEspStatesDialog d(parent);
    d.setV1((*item)["v1"].toString());
    d.setV2((*item)["v2"].toString());
    if(d.exec())
    {
        QMap<QString, QString> args;
        args.insert("v2", d.v2());
        BApplex::setUrl("value", args);
        return true;
    }
    return false;
}
