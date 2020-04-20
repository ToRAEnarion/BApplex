#ifndef BCONNECTEDITEM_H
#define BCONNECTEDITEM_H

#include <QString>
#include "struct.h"
#include <QHostAddress>

class BConnectedItem : public INetworkRegistration
{
public:
    enum ActionType
    {
        Unkown = 0,
        Delete = 1,
        Edit = 2,
        GetValue = 3,
        Update = 4,
        LoadLocalWifi = 10
    };

public:
    BConnectedItem();
    BConnectedItem(const QString& name);
    BConnectedItem(const QString& name, const NetworkRegistration& reg);
    QString name() const;
    QString password() const;

    QString getLocalSSID() const;
    QString getLocalPassWord() const;
    int getLocalType() const;

    QString ssid() const;
    QString mac() const;
    QString type() const;

    QString description() const;

    bool enable() const;
    QHostAddress ip() const;

    QSet<BConnectedItem::ActionType> getActions();

    void setName(const QString &value);
    void setMAC(const QString &value);
    void setIp(const QHostAddress &value);    
    void setType(const QString &value);

    void setDescription(const QString &value);
    void setEnable(bool b);

    void setValue(const QString& key, const QVariant &value);
    QVariant operator[](const QString& key);
    int valuesCount() const;

    const int Version = 0;

public:
    static BConnectedItem* make(const QHostAddress& adresse);

private:
    QString Name;
    QString Type;
    QHostAddress Ip;
    QString SSID;
    QString Password;
    QString MAC;

    bool Enabled;

    QString Description;

    QMap<QString, QVariant> Values;


    friend QDataStream &operator<<(QDataStream &out, const BConnectedItem &item);
    friend QDataStream &operator>>(QDataStream &in, BConnectedItem &item);
};


QDataStream &operator<<(QDataStream &out, const BConnectedItem &item);
QDataStream &operator>>(QDataStream &in, BConnectedItem &item);

#endif // BCONNECTEDITEM_H
