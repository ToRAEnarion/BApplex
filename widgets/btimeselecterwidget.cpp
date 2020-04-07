#include "btimeselecterwidget.h"
#include "ui_btimeselecterwidget.h"
#include <QDebug>

BTimeSelecterWidget::BTimeSelecterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BTimeSelecterWidget)
{
    ui->setupUi(this);
    updateVisibility();
}

BTimeSelecterWidget::~BTimeSelecterWidget()
{
    delete ui;
}

void BTimeSelecterWidget::updateVisibility()
{
    int i=ui->modeComboBox->currentIndex();
    ui->interval1->setVisible(i == 1);
    ui->interval2->setVisible((i == 1) && ui->doubleIntervalButton->isChecked());

    qDebug()<<stringValue();
}

QString BTimeSelecterWidget::stringValue()
{
    int mode = ui->modeComboBox->currentIndex();
    if(mode == 0)
    {
        return ui->checkBox->checkState() ? "1" : "0";
    }
    if(mode == 1)
    {
        QString str = ui->checkBox->checkState() ? "I" : "i";
        str += QString("%1:%2").arg(ui->start1->time().minute()+60*ui->start1->time().hour())
                .arg(ui->end1->time().minute()+60*ui->end1->time().hour());
        if(ui->doubleIntervalButton->isChecked())
        {
            str += QString(":%1:%2").arg(ui->start2->time().minute()+60*ui->start2->time().hour())
                    .arg(ui->end2->time().minute()+60*ui->end2->time().hour());
        }
        str += ";";
        return str;
    }
    return "";
}

bool BTimeSelecterWidget::updateMethod(BConnectedItem *item)
{

}

void BTimeSelecterWidget::on_modeComboBox_currentIndexChanged(int i)
{
    updateVisibility();
}

void BTimeSelecterWidget::on_doubleIntervalButton_toggled(bool b)
{
    updateVisibility();
}
