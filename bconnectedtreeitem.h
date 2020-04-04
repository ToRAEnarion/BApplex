#ifndef BCONNECTEDTREEITEM_H
#define BCONNECTEDTREEITEM_H

#include <QTreeWidgetItem>
#include "bconnecteditem.h"

#include <QStringList>

class TreePushButton;

class BTreeConnectedWidget;

class BConnectedTreeItem : public QTreeWidgetItem
{
public:
    BConnectedTreeItem(BConnectedItem* item);
    void initializeNode(BTreeConnectedWidget* tree);
    void updateNode();
    BConnectedItem* item(){return Item;}
private:
    BConnectedItem* Item;
    TreePushButton* Buttons;
};

#endif // BCONNECTEDTREEITEM_H
