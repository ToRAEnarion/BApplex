#include "btreeconnectedwidget.h"
#include "bconnectedtreeitem.h"
#include <QHBoxLayout>
#include <QAction>
#include "btreeconnectedwidget.h"

TreePushButton::TreePushButton(BConnectedTreeItem *item) : QWidget(), Item(item)
{
    setLayout(new QHBoxLayout());
    layout()->setMargin(0);

    addActionButton(QIcon(":/icons/loadWifi"), tr("Load Wifi"), BConnectedItem::LoadLocalWifi);

    addActionButton(QIcon(":/icons/update"), tr("Update"), BConnectedItem::Update);
    ((QHBoxLayout*)layout())->addStretch();

    addActionButton(QIcon(":/icons/delete"), tr("Supprimer"), BConnectedItem::Delete);

    updateButtons();
}

void TreePushButton::updateButtons()
{
    QSet<BConnectedItem::ActionType> actionsAvailable = Item->item()->getActions();
    for(int i=0;i<Buttons.size();i++)
    {
        BConnectedItem::ActionType act = ((BConnectedItem::ActionType)Buttons[i]->actions().first()->data().toInt());
        Buttons[i]->setVisible(actionsAvailable.contains(act));
    }
}

void TreePushButton::addActionButton(const QIcon &icon, const QString& name, BConnectedItem::ActionType action)
{
    QAction* a = new QAction(icon, name, this);
    connect(a, SIGNAL(triggered()), this, SLOT(onActionTriggrered()));
    QToolButton* button = new QToolButton();
    a->setData(action);
    button->setDefaultAction(a);
    Buttons.append(button);

    layout()->addWidget(button);
}

void TreePushButton::onActionTriggrered()
{
    QAction* a = (QAction*)sender();

    emit actionItem(connectedItem()->item(), (BConnectedItem::ActionType) a->data().toInt());
}

BTreeConnectedWidget::BTreeConnectedWidget(QWidget *parent) : QTreeWidget(parent)
{
    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int )), this, SLOT(onItemDoubleClicled(QTreeWidgetItem *, int)));
}

void BTreeConnectedWidget::onItemDoubleClicled(QTreeWidgetItem *item, int column)
{
    emit actionItem(((BConnectedTreeItem*)item)->item(), BConnectedItem::Edit);
}

