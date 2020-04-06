#ifndef BCONNECTEDITEM_H
#define BCONNECTEDITEM_H

#include <QString>
#include "struct.h"
#include <QHostAddress>

class BConnectedItem
{
public:
    enum ActionType
    {
        Unkown = 0,
        Delete = 1,
        LoadLocalWifi = 10
    };

public:
    BConnectedItem(const QString& name);
    BConnectedItem(const QString& name, const NetworkRegistration& reg);
    QString name() const;
    QString password() const;

    QString ssid() const;
    QString mac() const;    
    QString type() const;

    QString description() const;

    QHostAddress ip() const;

    QSet<BConnectedItem::ActionType> getActions();

    void setName(const QString &value);
    void setMAC(const QString &value);
    void setIp(const QHostAddress &value);    
    void setType(const QString &value);

    void setDescription(const QString &value);
public:
    static BConnectedItem* make(const QHostAddress& adresse);

private:
    QString Name;
    QString Type;
    QHostAddress Ip;
    QString SSID;
    QString Password;
    QString MAC;

    QString Description;
};

#endif // BCONNECTEDITEM_H
