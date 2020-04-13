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
        str += QString("%1-%2").arg(ui->start1->time().minute()+60*ui->start1->time().hour())
                .arg(ui->end1->time().minute()+60*ui->end1->time().hour());
        if(ui->doubleIntervalButton->isChecked())
        {
            str += QString("-%1-%2").arg(ui->start2->time().minute()+60*ui->start2->time().hour())
                    .arg(ui->end2->time().minute()+60*ui->end2->time().hour());
        }
        str += "-";
        return str;
    }
    return "";
}

void BTimeSelecterWidget::setValue(const QString str)
{
    qDebug()<<str;
    if(str.length() == 0)
        return;
    if(str == "0")
    {
        ui->modeComboBox->setCurrentIndex(0);
        ui->checkBox->setChecked(false);
        return;
    }
    if(str == "1")
    {
        ui->modeComboBox->setCurrentIndex(0);
        ui->checkBox->setChecked(true);
        return;
    }

    ui->modeComboBox->setCurrentIndex(1);
    ui->checkBox->setChecked(str[0].toUpper() == str[0]);

    if(str[0].toLower() == QChar('i'))
    {
        QRegExp reg("([smtwhfa]?)(\\d+)-(\\d+)(-(\\d+)-(\\d+))?-?");
        if(reg.indexIn(str, 1)>0)
        {
            int s1 = reg.cap(2).toInt();
            int e1 = reg.cap(3).toInt();
            ui->start1->setTime(QTime(s1/60, s1%60));
            ui->end1->setTime(QTime(e1/60, e1%60));
            if(reg.cap(5) != "")
            {
                int s2 = reg.cap(5).toInt();
                int e2 = reg.cap(6).toInt();
                ui->start2->setTime(QTime(s2/60, s2%60));
                ui->end2->setTime(QTime(e2/60, e2%60));
                ui->doubleIntervalButton->setChecked(true);
            }
            else
            {
                ui->doubleIntervalButton->setChecked(false);
            }
        }
    }

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
