#include "bapplex.h"
#include "bconnecteditem.h"

QMap<ItemSignature, UpdateMethod> BApplex::BApplex_UpdateMethods = QMap<ItemSignature, UpdateMethod>();
QString BApplex::Url = "";

void BApplex::addMethod(const ItemSignature &key, UpdateMethod method)
{
    BApplex::BApplex_UpdateMethods.insert(key, method);
}

bool BApplex::has(const ItemSignature &key)
{
    return BApplex_UpdateMethods.contains(key);
}

bool BApplex::invoke(const ItemSignature &key, BConnectedItem* item, QWidget* parent)
{
    Url = "";
    return BApplex_UpdateMethods[key](item, parent);
}

QString BApplex::url()
{
    return Url;
}

void BApplex::setUrl(const QString &str, const QMap<QString, QString> &maps)
{
    Url = str;
    if(maps.size()>0)
    {
        qDebug()<<"map";
        Url+= "?";
        QMapIterator<QString, QString> i(maps);
        while (i.hasNext())
        {
            i.next();
            Url += i.key() +"="+i.value();
            if(i.hasNext())
                Url += "&";
        }
    }
}
