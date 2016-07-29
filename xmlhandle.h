#ifndef XMLHANDLE_H
#define XMLHANDLE_H
#include <QString>
#include <QDomDocument>
#include <qfile>


class XMLHandle
{
public:
    XMLHandle();
    bool setXMl(const QString&);
    bool findNodeValue(QDomNode&,const QString&,QString&);
    bool findNodeAttr(QDomNode&,const QString&,const QString&,QString&);
    bool findValues(const QString&,const QStringList &,QStringList&);
    bool findValue(const QString&,QString&);
    bool findAttrValue(const QString&,const QString&,QString&);
private:
    QDomDocument xmlDoc;
};

#endif // XMLHANDLE_H
