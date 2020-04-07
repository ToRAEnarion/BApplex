#include "mainwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCamera>
#include <QCameraInfo>
#include "bqrcodedialog.h"
#include "bnetworkmanager.h"
#include <QNetworkConfiguration>
#include "settingsdialog.h"
#include "bqrcodemakerdialog.h"
#include <QNetworkReply>
#include <QNetworkInterface>
#include <QProcess>
#include <QPushButton>
#include <QJsonDocument>
#include "bconnecteditem.h"
#include "bconnectedtreeitem.h"
#include "treemultibutton.h"
#include "baddbyipdialog.h"

#include "widgets/btimerangeselectwidget.h"
#include "widgets/btimeselecterwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    ConfigManager(this),
    RequestManager(new BItemRequestManager(this)),
    Settings("BApplex")

{
    ui->setupUi(this);
    ConfigManager.updateConfigurations();

    BTimeSelecterWidget* w = new BTimeSelecterWidget();
    w->show();

    connect(&ConfigManager, SIGNAL(configurationChanged(const QNetworkConfiguration&)), this, SLOT(onNetworkConfigurationChanged(const QNetworkConfiguration&)));
    connect(ui->componentTreeView, SIGNAL(actionItem(BConnectedItem*, BConnectedItem::ActionType)), this, SLOT(onTreeActionPressed(BConnectedItem*, BConnectedItem::ActionType)));

    connect(RequestManager, SIGNAL(itemUpdated(BConnectedItem*)), this, SLOT(updateComponentsView()));

    ui->componentTreeView->resizeColumnToContents(3);
    ui->componentTreeView->resizeColumnToContents(4);

 //   ui->componentTreeView->setItemDelegateForColumn(4, new TreeMultiButton(this));
    updateComponentsView();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::addUpdateMethod(const QString& str, UpdateMethod method)
{
    UpdateMethods.insert(str, method);
}

void MainWindow::setLocalSSID(const QString &ssid, const QString &passWord, int t)
{
    Settings.setValue("NET_SSID", ssid);
    Settings.setValue("NET_password", passWord);
    Settings.setValue("NET_keytype", t);
}

QString MainWindow::getLocalSSID() const
{
    return Settings.value("NET_SSID").toString();
}

QString MainWindow::getLocalPassWord() const
{
    return Settings.value("NET_password").toString();
}

int MainWindow::getLocalType() const
{
    return Settings.value("NET_keytype").toInt();
}

bool MainWindow::tryRegisterElement(const QString &name)
{
    //if(ElementsRegistered.contains(name))
    {
  /*      CurrentElementRegistration = name;

        if(ElementsRegistered[name]->mac() == "")
        {

            CmdState = ProcessState::ProcessPing;
            Process->start("ping 192.168.4.10");
            Process->waitForFinished(5000);
            CmdState = ProcessState::ProcessArpA;
            Process->start("arp -a");
            Process->waitForFinished(5000);
            CmdState = ProcessState::None;
        }

        QString argUrl = "?ssid="+getLocalSSID()+"&pass="+getLocalPassWord();
        if(getLocalType() == 1)
            argUrl += "&wep=1";
        qDebug()<<"request: "<<argUrl;
        QNetworkRequest request(QUrl("http://192.168.4.10/setting"+argUrl));
        //http://192.168.4.10/setting?ssid=atronche&pass=4651889612

        NetworkManager.get(request);
*/
        updateComponentsView();
        return true;
    }
    return false;
}

void MainWindow::updateComponentsView()
{
    ui->componentTreeView->clear();
    foreach(BConnectedItem* elem, ElementsRegistered)
    {
        BConnectedTreeItem* item = new BConnectedTreeItem(elem);
        ui->componentTreeView->addTopLevelItem(item);
        item->initializeNode(ui->componentTreeView);
    }
}

void MainWindow::on_actionAddComponent_triggered()
{
    BQrCodeDialog d(this);
    if(d.exec())
    {
    //    ui->label->setText(d.foundText());
        NetworkRegistration res = d.foundSSID();
        BConnectedItem* item = new BConnectedItem(res.SSID, res);
        ElementsRegistered.append(item);

        RequestManager->updateFromItemId(item);

        updateComponentsView();
    }
}

void MainWindow::on_actionAddIp_triggered()
{
    BAddByIpDialog d(this);
    if(d.exec())
    {
        QHostAddress ip = d.ip();
        BConnectedItem* item = BConnectedItem::make(ip);
        CurrentElementRegistration = "TEMP_"+ElementsRegistered.count();
        ElementsRegistered.append(item);

        RequestManager->updateFromItemId(item);
        qDebug()<<"iso : "<<"http://"+ip.toString()+"/id";
    //    CmdState = ProcessState::NetworkGetId;
    //    NetworkManager.get(QNetworkRequest("http://"+ip.toString()+"/id"));

        updateComponentsView();
    }
}

void MainWindow::on_actionScanNetwork_triggered()
{
    RequestManager->updateAvailableAdresses();
}



void MainWindow::on_actionWifi_local_triggered()
{
    SettingsDialog d(this);
    if(d.exec())
    {        
        Settings.setValue("NET_SSID", d.ssid());
        Settings.setValue("NET_password", d.password());
        Settings.setValue("NET_keytype", d.keytype());
    }
}

void MainWindow::on_actionGenerer_QrCode_triggered()
{
    BQrCodeMakerDialog d(this);
    d.exec();
}

void MainWindow::onTreeActionPressed(BConnectedItem *item, BConnectedItem::ActionType a)
{
    switch(a)
    {
        case BConnectedItem::LoadLocalWifi:
            RequestManager->loadLocalWifi(item, this);
            break;
        case BConnectedItem::Delete:
            if(ElementsRegistered.removeOne(item))
                delete item;

            updateComponentsView();
            break;

    }
}

void MainWindow::onNetworkConfigurationChanged(const QNetworkConfiguration &config)
{
    qDebug()<<"config changed :"<<config.name()<<" : "<<config.state();
 //   if(ElementsRegistered.contains(config.name()) && ((config.state() & QNetworkConfiguration::Active)!=0))
    {
        tryRegisterElement(config.name());
    }
}
