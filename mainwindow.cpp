#include "mainwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCamera>
#include <QCameraInfo>
#include "bqrcodedialog.h"
#include "bnetworkmanager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAddComponent_triggered()
{
    BQrCodeDialog d(this);
    if(d.exec())
    {
        qDebug()<<"debug = "<<d.foundText();
        ui->label->setText(d.foundText());
    }
}
