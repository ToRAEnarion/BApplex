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
    QString stringValue() const;
    static bool editMethod(BConnectedItem* item, QWidget* parent);

private:
    Ui::BEspStatesDialog *ui;
};

#endif // BESPSTATESDIALOG_H
