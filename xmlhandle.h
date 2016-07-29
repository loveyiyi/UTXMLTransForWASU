#ifndef XMLHANDLE_H
#define XMLHANDLE_H
#include <QString>
#include <QDomDocument>
#include <qfile>
#include <QPair>


class XMLHandle
{
public:
    XMLHandle();
    bool setXMl(const QString&);



    bool findValueOnCond(const QString&,const QStringList&,const QString &,const QString&,QString &);

    bool findValue(const QString&,QString&);
    bool findAttrValue(const QString&,const QString&,QString&);

    bool findValues(const QStringList&,QStringList&);
    bool findAttrValues(const QStringList&,const QStringList &,QStringList&);

protected:
    bool findNodeValue(QDomNode&,const QString&,QString&);
    bool findNodeAttr(QDomNode&,const QString&,const QString&,QString&);
    bool findNodeOnCond(QDomNode&,const QString&,const QStringList&,const QString &,const QString&,QString &);
    bool nodeHasAttrValue(QDomNode&,const QString&);

    QDomDocument xmlDoc;
private:

};

#endif // XMLHANDLE_H
