#ifndef BESPSTATESDIALOG_H
#define BESPSTATESDIALOG_H

#include <QDialog>
#include "bconnecteditem.h"

namespace Ui {
class BEspStatesDialog;
}

class BEspStatesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BEspStatesDialog(QWidget *parent = nullptr);
    ~BEspStatesDialog();
    QString v1() const;
    QString v2() const;
    void setV1(const QString& str) ;
    void setV2(const QString& str);
    static bool editMethod(BConnectedItem* item, QWidget* parent);

private:
    Ui::BEspStatesDialog *ui;
};

#endif // BESPSTATESDIALOG_H
