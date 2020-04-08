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

    NetworkGetId = 10,
    LoadLocalWifi = 11,
    GetValue = 12
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
    void eval();
    void finish(bool updateItem = false);

    void updateAvailableAdresses();
    void loadLocalWifi(BConnectedItem* item, INetworkRegistration *registration);

    void getValue(BConnectedItem* item);

    void sendRequest(BConnectedItem *item, const QString& partialUrl);
private slots:
    void lookedUpHost(QHostInfo info);
    void onCommandReadyRead();
    void onRequestFinished(QNetworkReply *reply);
    void searchIpForCurrentItem();

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
