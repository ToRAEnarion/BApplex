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

QString BEspStatesDialog::stringValue() const
{
    return ui->checkBox->checkState() ? "0" : "1";
}

bool BEspStatesDialog::editMethod(BConnectedItem *item, QWidget* parent)
{
    BEspStatesDialog d(parent);
    if(d.exec())
    {
        QMap<QString, QString> args;
        args.insert("v2", d.stringValue());
        BApplex::setUrl("value", args);
        return true;
    }
    return false;
}
