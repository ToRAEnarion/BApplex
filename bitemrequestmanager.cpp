#include "bitemrequestmanager.h"
#include <QJsonDocument>
#include <QTimer>


BItemRequestManager::BItemRequestManager(QObject* parent) : QObject(parent),
  Process(new QProcess(this))
{
    connect(Process, SIGNAL(readyReadStandardOutput()), this, SLOT(onCommandReadyRead()));
    connect(&NetworkManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(onRequestFinished(QNetworkReply *)));
}

void BItemRequestManager::updateAvailableAdresses()
{
    AvailableLocalAdresses.clear();
    for(int i=0;i<200;i++)
    {
        QHostInfoThread* th = new QHostInfoThread(QString("192.168.1.%1").arg(i));
        th->start();
    }
}

void BItemRequestManager::loadLocalWifi(BConnectedItem *item, INetworkRegistration* registration)
{
    QString argUrl = "?ssid="+registration->getLocalSSID()+"&pass="+registration->getLocalPassWord();
    if(registration->getLocalType() == 1)
        argUrl += "&wep=1";
    qDebug()<<"request: "<<argUrl;
    QNetworkRequest request(QUrl("http://"+item->ip().toString()+"/setting"+argUrl));
    NetworkManager.get(request);
}

void BItemRequestManager::sendRequest(BConnectedItem *item, const QString &partialUrl)
{
    QNetworkRequest request(QUrl("http://"+item->ip().toString()+"/"+partialUrl));
    NetworkManager.get(request);
}

void BItemRequestManager::updateFromItemId(BConnectedItem *item)
{
    qDebug()<<"requestID";
    Item = item;

    CmdState = ProcessState::NetworkGetId;
    NetworkManager.get(QNetworkRequest("http://"+item->ip().toString()+"/id"));
}


void BItemRequestManager::lookedUpHost(QHostInfo info)
{
    QMutexLocker locker(&Mutex);
    AvailableLocalAdresses.insert(info.hostName(), info.addresses().first());

    if(CmdState == ProcessState::LoadLocalWifi)
    {
        if(Item->name() == info.hostName())
        {
            Item->setIp(AvailableLocalAdresses[info.hostName()]);
            CmdState = ProcessState::None;
        }
    }
}


void BItemRequestManager::onCommandReadyRead()
{
    if(CmdState == ProcessState::ProcessArpA)
    {
        QByteArray res = Process->readAllStandardOutput();
        QList<QByteArray> lines = res.split('\n');

        QRegExp regMac("([0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2})");
        for(int i=0;i<lines.size();i++)
        {
            qDebug()<<lines[i];
            QString workLine = QString(lines[i]);
            workLine = workLine.remove(" ");
            if(workLine.contains(Item->ip().toString().toLatin1()))
            {
                if(regMac.indexIn(workLine)>=0)
                {
                    Item->setMAC(regMac.cap(1));
                    CmdState = ProcessState::None;
                    break;
                }

            }
        }
    }
    else
    {
        QByteArray res = Process->readAllStandardOutput();
        qDebug()<<res;
    }
}


void BItemRequestManager::onRequestFinished(QNetworkReply *reply)
{
    qDebug()<<"error"<<reply->error()<<"   "<<reply->errorString();
    QByteArray rep = reply->readAll();

    if(!rep.isEmpty())
    {
        if(CmdState == ProcessState::NetworkGetId)
        {
            QJsonParseError error;

            QJsonDocument jDoc = QJsonDocument::fromJson(rep, &error);
            QString type = jDoc["id"]["Type"].toString();
            QString name = jDoc["id"]["Name"].toString();

         //   ElementsRegistered.remove(CurrentElementRegistration);
         //   ElementsRegistered.insert(name, item);
            Item->setType(type);
          //  CurrentElementRegistration = name;

            Item->setName(name);


            CmdState = ProcessState::ProcessPing;
            Process->start("ping "+Item->ip().toString()+" -n 1");
            Process->waitForFinished(5000);
            CmdState = ProcessState::ProcessArpA;
            Process->start("arp -a");
            Process->waitForFinished(5000);
            CmdState = ProcessState::None;

            emit itemUpdated(Item);
            Item = nullptr;
        }
        else if(CmdState == ProcessState::LoadLocalWifi)
        {
            QTimer::singleShot(3000, this, SLOT(searchIpForCurrentItem));
        }
    }

    CmdState = ProcessState::None;
}

void BItemRequestManager::searchIpForCurrentItem()
{
    if(CmdState == ProcessState::LoadLocalWifi)
    {
        updateAvailableAdresses();
    }
}

QHostInfoThread::QHostInfoThread(const QString& ip, BItemRequestManager *parent) : QThread (parent), Ip(ip)
{}

void QHostInfoThread::run()
{
    Id = QHostInfo::lookupHost(Ip, this, SLOT(onLookedUpHost(QHostInfo)));
    exec();
}

void QHostInfoThread::onLookedUpHost(QHostInfo info)
{
    quit();
    QHostInfo::abortHostLookup(Id);
    if(info.error() == QHostInfo::NoError && info.hostName() != Ip)
    {
        qDebug()<<"find ip"<<Ip<<" ww "<<info.error()<<info.hostName();
        emit lookedUpHost(info);
    }
    wait(200);
    deleteLater();
}
