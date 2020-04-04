#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class MainWindow;

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(MainWindow *parent = nullptr);
    ~SettingsDialog();
    QString ssid() const;
    QString password() const;
    int keytype() const;
protected slots:
    void on_qr_button_clicked();
private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
