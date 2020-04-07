#ifndef STRUCT_H
#define STRUCT_H

#include <QString>
#include <QMap>
#include <QNetworkConfiguration>


#define DEFAULT_AP_IP "192.168.4.10"



struct ElementRegistration
{
    enum Type
    {
        None = 0,
        ESP8266 = 10
    };

    ElementRegistration::Type Type;
    QString SSID;
    QString Pass;
    QString MAC;
};

class INetworkRegistration
{
public:
    QString virtual getLocalSSID() const=0;
    QString virtual getLocalPassWord() const=0;
    int virtual getLocalType() const=0;

    virtual ~INetworkRegistration(){}
};


struct NetworkRegistration : public INetworkRegistration
{
    QString SSID;
    QString Pass;
    int KeyType;

    QString virtual getLocalSSID() const {return SSID;}
    QString virtual getLocalPassWord() const {return Pass;}
    int virtual getLocalType() const {return KeyType;}
};


#endif // STRUCT_H
