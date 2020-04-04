#ifndef BADDBYIPDIALOG_H
#define BADDBYIPDIALOG_H

#include <QDialog>
#include <QHostAddress>

namespace Ui {
class BAddByIpDialog;
}

class BAddByIpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BAddByIpDialog(QWidget *parent = nullptr);
    ~BAddByIpDialog();

    QHostAddress ip() const;

private:
    Ui::BAddByIpDialog *ui;
};

#endif // BADDBYIPDIALOG_H
