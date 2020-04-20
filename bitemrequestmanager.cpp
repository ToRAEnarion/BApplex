#include "bitemrequestmanager.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTimer>
#include <QFile>
#include <QDir>


BItemRequestManager::BItemRequestManager(QObject* parent) : QObject(parent),
  Process(new QProcess(this))
{
    connect(Process, SIGNAL(readyReadStandardOutput()), this, SLOT(onCommandReadyRead()));
    connect(&NetworkManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(onRequestFinished(QNetworkReply *)));
    CurrentRequest = -1;
}

void BItemRequestManager::updateAvailableAdresses()
{
    qDebug()<<"startsearching";
    AvailableLocalAdresses.clear();
    for(int i=0;i<200;i++)
    {
        QHostInfoThread* th = new QHostInfoThread(QString("192.168.1.%1").arg(i));
        connect(th, SIGNAL(lookedUpHost(QHostInfo)), this, SLOT(lookedUpHost(QHostInfo)));
        th->start();
    }
}

void BItemRequestManager::loadLocalWifi(BConnectedItem *item, INetworkRegistration* registration)
{
    Request r;
    r.Item = item;
    r.State = ProcessState::LoadLocalWifi;
    r["SSID"] = registration->getLocalSSID();
    r["psw"] = registration->getLocalPassWord();
    r["type"] = registration->getLocalType();
    Requests.append(r);
}

void BItemRequestManager::addWlanProfile(BConnectedItem *item, const INetworkRegistration *network)
{
    Request r;
    r.Item = item;
    r.State = ProcessState::AddWLanProfile;
    r["SSID"] = network->getLocalSSID();
    r["psw"] = network->getLocalPassWord();
    r["type"] = network->getLocalType();
    Requests.append(r);
}

void BItemRequestManager::changeWlan(BConnectedItem *item, const INetworkRegistration *network)
{
    Request r;
    r.Item = item;
    r.State = ProcessState::ChangeNetworkWlan;
    r["SSID"] = network->getLocalSSID();
    r["psw"] = network->getLocalPassWord();
    r["type"] = network->getLocalType();
    Requests.append(r);
}

void BItemRequestManager::ping(BConnectedItem *item, bool disable)
{
    if(disable)
        item->setEnable(false);
    append(item, ProcessState::NetworkPing);
}
void BItemRequestManager::pingRepeat(BConnectedItem *item, int n)
{
    Request r;
    r.Item = item;
    r.State = ProcessState::NetworkPing;
    r["n"] = n;
    Requests.append(r);
}

void BItemRequestManager::wait(BConnectedItem *item, int n)
{
    Request r;
    r.Item = item;
    r.State = ProcessState::Wait;
    r["t"] = n;
    Requests.append(r);

}

void BItemRequestManager::getValue(BConnectedItem *item)
{
    append(item, ProcessState::GetValue);
}

void BItemRequestManager::sendRequest(BConnectedItem *item, const QString &partialUrl)
{
    Request r;
    r.Item = item;
    r.State = ProcessState::SetValue;
    r["url"] = partialUrl;
    Requests.append(r);
    eval();
}

BItemRequestManager::~BItemRequestManager()
{
    Process->kill();
}

void BItemRequestManager::stackLoadLocalWifi(BConnectedItem *item, INetworkRegistration *registration)
{    
    changeWlan(item, item);
    pingRepeat(item, 6);
    updateFromItemId(item);
    loadLocalWifi(item, registration);
    changeWlan(item, registration);
    wait(item, 8000);
    append(item, ProcessState::SearchCorrectIp);
//      RequestManager->updateAvailableAdresses()
    getValue(item);
}

void BItemRequestManager::updateFromItemId(BConnectedItem *item)
{
    append(item, ProcessState::NetworkGetId);
    append(item, ProcessState::ProcessPing);
    append(item, ProcessState::ProcessArpA);
}

void BItemRequestManager::append(BConnectedItem *item, ProcessState state)
{
    Request r;
    r.Item = item;
    r.State = state;
    Requests.append(r);
}

void BItemRequestManager::eval()
{
    if(CurrentRequest >= 0 || Requests.size() == 0)
        return;

    CurrentRequest = 0;
    qDebug()<<"eval : "<<Requests[CurrentRequest].State;
    switch(Requests[CurrentRequest].State)
    {
        case ProcessState::NetworkGetId:
            NetworkManager.get(QNetworkRequest("http://"+Requests[CurrentRequest].Item->ip().toString()+"/id"));
            break;
        case ProcessState::ProcessArpA:
            Process->start("arp -a");
            Process->waitForStarted(5000);
            Process->waitForFinished(5000);
            Process->terminate();
            break;
        case ProcessState::LoadLocalWifi:
        {
            QString argUrl = "?ssid="+Requests[CurrentRequest]["SSID"].toString()+"&pass="+Requests[CurrentRequest]["psw"].toString();
            if(Requests[CurrentRequest]["SSID"].toInt() == 1)
                argUrl += "&wep=1";
            QNetworkRequest request(QUrl("http://"+Requests[CurrentRequest].Item->ip().toString()+"/setting"+argUrl));
            QNetworkReply* r= NetworkManager.get(request);
            r->waitForReadyRead(5000);
            break;
        }
        case ProcessState::NetworkPing:
        {
            pingCurrentItem();
            break;
        }
        case ProcessState::GetValue:
        {
            QNetworkRequest r(QUrl("http://"+Requests[CurrentRequest].Item->ip().toString()+"/value"));
            NetworkManager.get(r);
            break;
        }
        case ProcessState::SetValue:
        {
            qDebug()<<"sendValue";
            QNetworkRequest r(QUrl("http://"+Requests[CurrentRequest].Item->ip().toString()+"/"+Requests[CurrentRequest]["url"].toString()));
            NetworkManager.get(r);
            break;
        }
        case ProcessState::ProcessPing:
            Process->start("ping "+Requests[CurrentRequest].Item->ip().toString()+" -n 1");
            Process->waitForStarted(5000);
            Process->waitForFinished(5000);
            Process->kill();
            break;
        case ProcessState::ChangeNetworkWlan:
        {
            qDebug()<<"change to wlan "<<Requests[CurrentRequest]["SSID"];
            Process->start("netsh wlan connect name=\""+Requests[CurrentRequest]["SSID"].toByteArray()+"\"");
            Process->waitForStarted(5000);
            Process->waitForFinished(5000);
            Process->kill();
            break;
        }
        case ProcessState::AddWLanProfile:
        {
            QByteArray ssid = Requests[CurrentRequest]["SSID"].toByteArray();
            QByteArray hex;

            for(int i=0;i<ssid.size();i++)
            {
                hex += QString::number((char)ssid[i],16);
            }

            qDebug()<<"add profile netsh";

            QFile source(":/files/emptySSID");
            source.open(QIODevice::ReadOnly);
            QByteArray sourceData = source.readAll();
            sourceData.replace("{SSID}", Requests[CurrentRequest]["SSID"].toByteArray());
            sourceData.replace("{PSW}", Requests[CurrentRequest]["psw"].toByteArray());
            sourceData.replace("{HEX_SSID}", hex);
            QFile t(ssid + "temp.xml");
            t.open(QIODevice::WriteOnly);
            QTextStream stream(&t);
            stream<<sourceData;
            t.close();
            QFileInfo info(t.fileName());

            qDebug()<<"netsh wlan add profile filename=\""+QDir::toNativeSeparators(info.absoluteFilePath())+"\" user=all";
            Process->start("netsh wlan add profile filename=\""+QDir::toNativeSeparators(info.absoluteFilePath())+"\" user=all");
            Process->waitForStarted(5000);
            Process->waitForFinished(5000);
            Process->kill();
            QDir d;
            d.remove(t.fileName());
            break;
        }
        case ProcessState::SearchCorrectIp:
        {
            updateAvailableAdresses();
            break;
        }
        case ProcessState::Wait:
        {
            int n = Requests[CurrentRequest]["t"].toInt();
            QTimer::singleShot(n, this, SLOT(waited()));
        }
    }

    if(CurrentRequest<0)
        eval();
}

void BItemRequestManager::waited()
{
    qDebug()<<"finish waited";
    finish(false);
}

void BItemRequestManager::finish(bool updateItem)
{
    if(CurrentRequest<0)
        return;

    qDebug()<<" finish "<<Requests[CurrentRequest].State;

    if(updateItem)
        emit itemUpdated(Requests[CurrentRequest].Item);

    Requests.removeAt(CurrentRequest);
    CurrentRequest = -1;
    eval();
}

void BItemRequestManager::abort(BConnectedItem *item)
{
    int i=0;
    while(i<Requests.size())
    {
        if(Requests[i].Item == item)
        {
            Requests.removeAt(i);
            if(CurrentRequest == i)
                CurrentRequest = -1;
            else if(CurrentRequest>i)
                CurrentRequest--;
        }
        else
        {
            i++;
        }
    }
}


void BItemRequestManager::lookedUpHost(QHostInfo info)
{
    QMutexLocker locker(&Mutex);
    AvailableLocalAdresses.insert(info.hostName(), info.addresses().first());

    qDebug()<<"ending "<<cmdState();
    if(cmdState() == ProcessState::SearchCorrectIp)
    {
        if(Requests[CurrentRequest].Item->name() == info.hostName())
        {
            Requests[CurrentRequest].Item->setIp(AvailableLocalAdresses[info.hostName()]);
            finish();
        }
    }
}


void BItemRequestManager::onCommandReadyRead()
{
    if(cmdState() == ProcessState::ProcessArpA)
    {
        qDebug()<<"process arp  a";
        QByteArray res = Process->readAllStandardOutput();
        QList<QByteArray> lines = res.split('\n');

        QRegExp regMac("([0-9a-f]{2}[:-][0-9a-f]{2}[:-][0-9a-f]{2}[:-][0-9a-f]{2}[:-][0-9a-f]{2}[:-][0-9a-f]{2})");
        for(int i=0;i<lines.size();i++)
        {
            qDebug()<<lines[i];
            QString workLine = QString(lines[i]);
            workLine = workLine.remove(" ");
            if(workLine.contains(Requests[CurrentRequest].Item->ip().toString().toLatin1()))
            {
                if(regMac.indexIn(workLine)>=0)
                {
                    Requests[CurrentRequest].Item->setMAC(regMac.cap(1));
                    break;
                }
            }
        }
        Process->waitForFinished(500);
        Process->waitForReadyRead(500);
        Process->kill();
        Process->waitForFinished(500);
        finish(true);
    }
    else if(cmdState() == ProcessState::ProcessPing)
    {
        Process->waitForFinished(500);
        Process->waitForReadyRead(500);
        Process->kill();
        Process->waitForFinished(500);
   //     while (Process->state() == QProcess::Running){qDebug()<<"in";}
        finish(false);
    }
    else if(cmdState() == ProcessState::AddWLanProfile)
    {

        QByteArray res = Process->readAllStandardOutput();
        qDebug()<<res;
        Process->waitForFinished(500);
        Process->waitForReadyRead(500);
        Process->kill();
        Process->waitForFinished(500);
   //     while (Process->state() == QProcess::Running){qDebug()<<"in";}
        finish(false);
    }
    else if(cmdState() == ProcessState::ChangeNetworkWlan)
    {

        QByteArray res = Process->readAllStandardOutput();
        qDebug()<<res;
        Process->waitForFinished(500);
        Process->waitForReadyRead(500);
        Process->kill();
        Process->waitForFinished(500);
        finish(false);
    }
}


void BItemRequestManager::onRequestFinished(QNetworkReply *reply)
{
    qDebug()<<"error"<<reply->error()<<"   "<<reply->errorString();
    QByteArray rep = reply->readAll();

    if(!rep.isEmpty())
    {
        if(cmdState() == ProcessState::NetworkGetId)
        {
            QJsonParseError error;

            QJsonDocument jDoc = QJsonDocument::fromJson(rep, &error);
            QString type = jDoc["id"]["Type"].toString();
            QString name = jDoc["id"]["Name"].toString();

         //   ElementsRegistered.remove(CurrentElementRegistration);
         //   ElementsRegistered.insert(name, item);
            Requests[CurrentRequest].Item->setType(type);
          //  CurrentElementRegistration = name;

            Requests[CurrentRequest].Item->setName(name);
            finish(true);
        }
        else if(cmdState() == ProcessState::LoadLocalWifi)
        {
            finish(true);
        }
        else if(cmdState() == ProcessState::NetworkPing)
        {
            Requests[CurrentRequest].Item->setEnable(true);
            Requests[CurrentRequest]["n"] = 0;
        }
        else if(cmdState() == ProcessState::GetValue || cmdState() == ProcessState::SetValue)
        {
            QJsonParseError error;

            QJsonDocument jDoc = QJsonDocument::fromJson(rep, &error);
            if(jDoc["value"].isObject())
            {
                QStringList keys = jDoc["value"].toObject().keys();
                for (int i=0;i<keys.size();i++)
                {
                    Requests[CurrentRequest].Item->setValue(keys[i], jDoc["value"][keys[i]].toVariant());
                }
                finish(keys.size()>0);
            }
        }
    }

    if(cmdState() == ProcessState::NetworkPing)
    {
        int n = 0;
        if(Requests[CurrentRequest].contains("n"))
        {
            n = Requests[CurrentRequest]["n"].toInt();
            if(n>0)
            {
                n--;
                Requests[CurrentRequest]["n"] = n;
                QTimer::singleShot(2000, this, SLOT(pingCurrentItem()));
            }
        }

        if(n<=0)
            finish(true);
    }

}

void BItemRequestManager::searchIpForCurrentItem()
{
    if(cmdState() == ProcessState::SearchCorrectIp)
    {
        qDebug()<<"startsearching";
        updateAvailableAdresses();
    }
}

void BItemRequestManager::pingCurrentItem()
{
    if(CurrentRequest<0)
        return;

    QNetworkRequest r(QUrl("http://"+Requests[CurrentRequest].Item->ip().toString()+"/ping"));
    NetworkManager.get(r);
    qDebug()<<"network ping";
}

ProcessState BItemRequestManager::cmdState()
{
    if(CurrentRequest>=0)
        return Requests[CurrentRequest].State;
    return ProcessState::None;
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
