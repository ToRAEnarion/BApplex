#ifndef BAPPLEX_H
#define BAPPLEX_H

#include <QString>
#include <QWidget>
#include <QMap>

class BConnectedItem;

typedef QString ItemSignature;
typedef bool (*UpdateMethod)(BConnectedItem*, QWidget*);

class BApplex
{
public:
    static void addMethod(const ItemSignature& key, UpdateMethod method);
    static bool has(const ItemSignature& key);
    static bool invoke(const ItemSignature& key, BConnectedItem* item, QWidget* parent);
    static QString url();
    static void setUrl(const QString& str, const QMap<QString, QString>& maps);
private:
    static QMap<ItemSignature, UpdateMethod> BApplex_UpdateMethods;
    static QString Url;
};

#endif // BAPPLEX_H
