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
    LoadLocalWifi = 11
};

class BItemRequestManager : public QObject
{
    Q_OBJECT
public:
    BItemRequestManager(QObject* parent = nullptr);

    void updateFromItemId(BConnectedItem* item);

    void updateAvailableAdresses();
    void loadLocalWifi(BConnectedItem* item, INetworkRegistration *registration);

    void sendRequest(BConnectedItem *item, const QString& partialUrl);
private slots:
    void lookedUpHost(QHostInfo info);
    void onCommandReadyRead();
    void onRequestFinished(QNetworkReply *reply);
    void searchIpForCurrentItem();

signals:
    void itemUpdated(BConnectedItem*);
private:
    QNetworkAccessManager NetworkManager;
    QProcess* Process;
    QMap<QString, QHostAddress> AvailableLocalAdresses;
    QMutex Mutex;
    ProcessState CmdState;
    BConnectedItem* Item;
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
