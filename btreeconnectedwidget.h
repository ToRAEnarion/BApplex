#ifndef BTREECONNECTEDWIDGET_H
#define BTREECONNECTEDWIDGET_H

#include <QTreeWidget>
#include <bconnectedtreeitem.h>
#include <QPushButton>
#include <QToolButton>


class TreePushButton : public QWidget
{
    Q_OBJECT
public:
    TreePushButton(BConnectedTreeItem* item);
    BConnectedTreeItem *connectedItem() const{return Item;}
    void updateButtons();

public slots:
    void onActionTriggrered();
signals:
    void actionItem(BConnectedItem*, BConnectedItem::ActionType);

private:
    BConnectedTreeItem* Item;
    QList<QToolButton*> Buttons;
    void addActionButton(const QIcon &icon, const QString& name, BConnectedItem::ActionType a);
};


class BTreeConnectedWidget : public QTreeWidget
{
    Q_OBJECT
public:
    BTreeConnectedWidget(QWidget* parent = nullptr);
private slots:
    void onItemDoubleClicled(QTreeWidgetItem *item, int column);
signals:
    void actionItem(BConnectedItem*, BConnectedItem::ActionType);
};

#endif // BTREECONNECTEDWIDGET_H
