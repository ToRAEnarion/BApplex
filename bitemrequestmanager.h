#ifndef BITEMREQUESTMANAGER_H
#define BITEMREQUESTMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QProcess>
#include <QHostInfo>
#include <QThread>
#include <QMutex>
#include "bconnecteditem.h"

enum ProcessState
{
    None =0,
    ProcessPing = 1,
    ProcessArpA = 2,
    AddWLanProfile = 3,
    ChangeNetworkWlan = 4,
    Wait = 5,

    NetworkPing = 10,
    NetworkGetId = 11,
    LoadLocalWifi = 12,
    GetValue = 13,
    SetValue = 14,


    SearchCorrectIp = 22
};

class Request : public QMap<QString, QVariant>
{
public:
    BConnectedItem* Item;
    ProcessState State;
};

class BItemRequestManager : public QObject
{
    Q_OBJECT
public:
    BItemRequestManager(QObject* parent = nullptr);

    void updateFromItemId(BConnectedItem* item);

    void append(BConnectedItem* item, ProcessState state);
    void finish(bool updateItem = false);
    void abort(BConnectedItem* item);

    void updateAvailableAdresses();
    void loadLocalWifi(BConnectedItem* item, INetworkRegistration *registration);
    void addWlanProfile(BConnectedItem *item, const INetworkRegistration* network);
    void changeWlan(BConnectedItem *item, const INetworkRegistration *network);

    void ping(BConnectedItem* item, bool disable = true);
    void pingRepeat(BConnectedItem *item, int n);

    void wait(BConnectedItem *item, int n);

    void getValue(BConnectedItem* item);

    void sendRequest(BConnectedItem *item, const QString& partialUrl);
    ~BItemRequestManager();

    void stackLoadLocalWifi(BConnectedItem* item, INetworkRegistration *registration);
public slots:
    void eval();
private slots:
    void lookedUpHost(QHostInfo info);
    void onCommandReadyRead();
    void onRequestFinished(QNetworkReply *reply);
    void searchIpForCurrentItem();
    void pingCurrentItem();
    void waited();

signals:
    void itemUpdated(BConnectedItem*);
private:
    ProcessState cmdState();

    QNetworkAccessManager NetworkManager;
    QProcess* Process;
    QMap<QString, QHostAddress> AvailableLocalAdresses;
    QMutex Mutex;

    int CurrentRequest;
    QList<Request> Requests;
};


class QHostInfoThread : public QThread
{
    Q_OBJECT
public:
    QHostInfoThread(const QString& ip, BItemRequestManager* parent = nullptr);
protected:
    void run();
protected slots:
    void onLookedUpHost(QHostInfo info);
signals:
    void lookedUpHost(QHostInfo info);
private:
    QString Ip;
    int Id;
};


#endif // BITEMREQUESTMANAGER_H
