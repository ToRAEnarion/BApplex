#ifndef BNETWORKMANAGER_H
#define BNETWORKMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkConfigurationManager>
#include <QNetworkConfiguration>

class BNetworkManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    BNetworkManager(QObject* parent =nullptr);
    void searchAndConnect(QString name, QString psw);
private:
    QNetworkConfigurationManager Manager;
    QMap<QString, QString> PassWordList;
};

#endif // BNETWORKMANAGER_H
