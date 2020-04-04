#include "bconnecteditem.h"

BConnectedItem::BConnectedItem(const QString &name) : Name(name)
{

}

BConnectedItem::BConnectedItem(const QString &name, const NetworkRegistration &reg)
{
    Name = name;
    SSID = reg.SSID;
    Password = reg.Pass;
    Ip = QHostAddress(DEFAULT_AP_IP);
}

QString BConnectedItem::name() const
{
    return Name;
}

QString BConnectedItem::ssid() const
{
    return SSID;
}

QString BConnectedItem::mac() const
{
    return MAC;
}

QHostAddress BConnectedItem::ip() const
{
    return Ip;
}

QSet<BConnectedItem::ActionType> BConnectedItem::getActions()
{
    QSet<BConnectedItem::ActionType> rSet = QSet<BConnectedItem::ActionType>();
    rSet.insert(ActionType::Delete);
    if(Ip.toString() == DEFAULT_AP_IP)
    {
        rSet.insert(BConnectedItem::LoadLocalWifi);
    }
    return rSet;
}

void BConnectedItem::setMAC(const QString &value)
{
    MAC = value;
}

void BConnectedItem::setIp(const QHostAddress &value)
{
    Ip = value;
}

BConnectedItem *BConnectedItem::make(const QHostAddress &adresse)
{
     BConnectedItem* item = new BConnectedItem("");
     item->setIp(adresse);
     return item;
}

QString BConnectedItem::type() const
{
    return Type;
}

void BConnectedItem::setType(const QString &value)
{
    Type = value;
}

void BConnectedItem::setName(const QString &value)
{
    Name = value;
}

QString BConnectedItem::password() const
{
    return Password;
}
