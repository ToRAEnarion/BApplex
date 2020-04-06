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
    setFlags(flags() | Qt::ItemIsEditable);
    updateNode();
}

void BConnectedTreeItem::updateNode()
{
    QFont f = font(1);
    if(Item->description() != "")
    {
        setText(0, Item->description());
        f.setItalic(true);
        setFont(0, f);
    }
    else
    {
        setText(0, Item->name());
        f.setItalic(false);
        setFont(0, f);
    }
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



