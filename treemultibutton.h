#ifndef TREEMULTIBUTTON_H
#define TREEMULTIBUTTON_H

#include <QItemDelegate>
#include <QPushButton>
#include <QTreeWidgetItem>

class TreeMultiButton : public QItemDelegate
{
public:
    TreeMultiButton(QObject* parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // TREEMULTIBUTTON_H
