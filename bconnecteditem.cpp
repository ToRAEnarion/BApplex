#include "bconnecteditem.h"
#include <QDataStream>

BConnectedItem::BConnectedItem() : Enabled(true)
{
}

BConnectedItem::BConnectedItem(const QString &name) : Name(name), Description(""), Enabled(true)
{

}

BConnectedItem::BConnectedItem(const QString &name, const NetworkRegistration &reg)
{
    Name = name;
    SSID = reg.SSID;
    Password = reg.Pass;
    Ip = QHostAddress(DEFAULT_AP_IP);
    Enabled = true;
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
    else if (!Enabled)
    {
        rSet.insert(BConnectedItem::Update);
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

void BConnectedItem::setDescription(const QString &value)
{
    Description = value == Name ? "" : value;
}

void BConnectedItem::setEnable(bool b)
{
    Enabled = b;
}

void BConnectedItem::setValue(const QString &key, const QVariant &value)
{
    Values[key] = value;
}

QVariant BConnectedItem::operator[](const QString &key)
{
    if(Values.contains(key))
        return Values[key];

    return QVariant();
}

int BConnectedItem::valuesCount() const
{
    return Values.count();
}

QString BConnectedItem::type() const
{
    return Type;
}

QString BConnectedItem::description() const
{
    return Description;
}

bool BConnectedItem::enable() const
{
    return Enabled;
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

QString BConnectedItem::getLocalSSID() const
{
    return ssid();
}

QString BConnectedItem::getLocalPassWord() const
{
    return password();
}

int BConnectedItem::getLocalType() const
{
    return type() == "WAP" ? 2 : 0;
}

QDataStream &operator<<(QDataStream &out, const BConnectedItem &item)
{
    out << qint16(item.Version)
        << item.Name
        << item.Type
        << item.Ip
        << item.SSID
        << item.Password
        << item.MAC
        << item.Description
        << item.Values;

    return out;
}

QDataStream &operator>>(QDataStream &in, BConnectedItem &item)
{
    qint16 version;
    in >> version;
    in >> item.Name
       >> item.Type
       >> item.Ip
       >> item.SSID
       >> item.Password
       >> item.MAC
       >> item.Description
       >> item.Values;

    return in;
}
