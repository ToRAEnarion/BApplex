#include "bqrcodedialog.h"

#include <QVBoxLayout>
#include <QDebug>

BQrCodeDialog::BQrCodeDialog(QWidget *parent) : QDialog (parent)
{    
    BQrCodeScanWidget* w = new BQrCodeScanWidget(this);
    connect(w, SIGNAL(textExtracted(QString)), this, SLOT(onTextExtracted(QString)));
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(w);
    setLayout(layout);
    setGeometry(0,0,400,400);
    Errored = w->getCamera() == nullptr;
}

QString BQrCodeDialog::foundText()
{
    return Text;
}


NetworkRegistration BQrCodeDialog::foundSSID()
{
    NetworkRegistration net;
    QList<QrElem> elems = QrElem::parse(Text);
    for (int i=0;i<elems.count();i++)
    {
        qDebug()<<elems[i].toString();
        if(elems[i].Key == "WIFI")
        {
            qDebug()<<elems[i].Key;

            net.SSID = elems[i]["S"].Text;
            net.Pass = elems[i]["P"].Text;
            net.KeyType = elems[i]["T"].Text == "WEP" ? 1 : 0;
            break;
        }
    }

    return net;
}

void BQrCodeDialog::onTextExtracted(QString tag)
{
    Text = tag;
    accept();
}

QString QrElem::toString() const
{
    QString str;
    str = Key+":"+Text;
    if(Children.size()>0)
    {
        str+=" (";
        for (int i=0;i<Children.size();i++)
        {
            str.append(Children[i].toString());
        }
        str += ")";
    }
    str+=";";
    return str;
}

QrElem QrElem::operator[](const QString &str)
{
    for (int i=0;i<Children.count();i++)
    {
        if(Children[i].Key == str)
            return Children[i];
    }
    return QrElem();
}

QList<QrElem> QrElem::parse(const QString &text)
{
    int sub = 0;
    QList<QrElem> list;
    QList<QrElem>* workingList;
    QString currentTxt;
    for (int i=0;i<text.size();i++)
    {
        if(text[i] == QChar(':'))
        {
            sub++;
            workingList = &list;
            for(int j=1;j<sub;j++)
            {
                workingList = &(workingList->last().Children);
            }
            workingList->append(QrElem());
            workingList->last().Key = currentTxt;
            currentTxt = "";
        }
        else if(text[i] == QChar(';'))
        {
            if(sub == 0)
                return QList<QrElem>();
            workingList = &list;
            for(int j=1;j<sub;j++)
            {
                workingList = &(workingList->last().Children);
            }
            workingList->last().Text = currentTxt;
            currentTxt = "";

            sub--;
        }
        else if(text[i] == QChar('\\'))
        {
            i++;
            currentTxt += text[i];
        }
        else
        {
            currentTxt += text[i];
        }
    }

    return list;
}
