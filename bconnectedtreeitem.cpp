#include "bconnectedtreeitem.h"
#include <QPushButton>
#include <QFile>
#include "btreeconnectedwidget.h"

BConnectedTreeItem::BConnectedTreeItem(BConnectedItem *item) : QTreeWidgetItem(0), Item(item)
{
}

void BConnectedTreeItem::initializeNode(BTreeConnectedWidget *tree)
{
    Buttons = new TreePushButton(this);
    QObject::connect(Buttons, SIGNAL(actionItem(BConnectedItem*, BConnectedItem::ActionType)), tree, SIGNAL(actionItem(BConnectedItem*, BConnectedItem::ActionType)));
    tree->setItemWidget(this, 4, Buttons);
    updateNode();
}

void BConnectedTreeItem::updateNode()
{
    setText(0, Item->name());
    setText(1, "0x0");
    setText(2, Item->ip().toString());
    setText(3, Item->mac());

    if(!Item->type().isEmpty() && QFile::exists(":/components/"+Item->type()))
    {
        setIcon(0, QIcon(":/components/"+Item->type()));
    }
    else
    {
        setIcon(0, QIcon(":/components/unknown"));
    }
    Buttons->updateButtons();
}



