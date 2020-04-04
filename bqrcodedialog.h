#ifndef BQRCODEDIALOG_H
#define BQRCODEDIALOG_H

#include <QDialog>
#include "bqrcodescanwidget.h"
#include "struct.h"

struct QrElem
{
    QString Key;
    QString Text;
    QList<QrElem> Children;
    QString toString() const;

    QrElem operator[](const QString& str);

    static QList<QrElem> parse(const QString& text);
};

class BQrCodeDialog : public QDialog
{
    Q_OBJECT
public:
    BQrCodeDialog(QWidget* parent = nullptr);
    QString foundText();
    NetworkRegistration foundSSID();
public slots:
    void onTextExtracted(QString tag);
private:
    QString Text;
    bool Errored;
};

#endif // BQRCODEDIALOG_H
