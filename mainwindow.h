#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkConfigurationManager>
#include "struct.h"
#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QProcess>
#include "bconnectedtreeitem.h"
#include "bitemrequestmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public INetworkRegistration
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setLocalSSID(const QString& ssid, const QString& passWord, int t = 2);
    QString getLocalSSID() const;
    QString getLocalPassWord() const;
    int getLocalType() const;

    bool tryRegisterElement(const QString &name);

public slots:
    void updateComponentsView();
protected slots:
    void on_actionAddComponent_triggered();
    void on_actionAddIp_triggered();
    void on_actionScanNetwork_triggered();
    void on_actionWifi_local_triggered();
    void on_actionGenerer_QrCode_triggered();

    void onTreeActionPressed(BConnectedItem* item, BConnectedItem::ActionType a);
    void onNetworkConfigurationChanged(const QNetworkConfiguration &config);
private:
    Ui::MainWindow *ui;
    QNetworkConfigurationManager ConfigManager;
    BItemRequestManager* RequestManager;
    QString CurrentElementRegistration;
    QList<BConnectedItem*> ElementsRegistered;
    QSettings Settings;
};

#endif // MAINWINDOW_H
