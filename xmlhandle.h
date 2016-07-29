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



    bool findValueOnCond(const QString & nodeName,
                         const QStringList&nodeConditions,
                         const QString &subNodeName,
                         QString &value);
    bool findAttrValueOnCond(const QString & nodeName,
                             const QStringList&attrConditions,
                             const QString &subNodeName,
                             const QString&attrName,
                             QString &value);

    bool findValue(const QString& nodeName,QString&value);
    bool findAttrValue(const QString&nodeName,const QString&attrName,QString&value);


protected:
    bool nodeHasAttrValue(QDomNode,const QString&);

    QDomDocument xmlDoc;
private:

};

#endif // XMLHANDLE_H
