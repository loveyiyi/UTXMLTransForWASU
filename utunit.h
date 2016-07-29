#ifndef UTUNIT_H
#define UTUNIT_H
#include <QDomDocument>
#include <QString>
#include <QPair>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QVector>



class UTUnit
{
public:
    UTUnit();
    bool createUTXML(QString& );
    bool setAttrValue(const int nType,const QString& nodeName,const QString& attrName,const QString& value);
    bool setNodeValue(const int nType,const QString& nodeName,const QString& value);
protected:

private:
    struct CNTV_XML_Vector{
        QVector<QPair<QString ,QString >> nodeAttrVector;
        QVector<QPair<QString ,QString >> pVector;
        int elementType; //1	Program
                        //2	Movie
                        //3	HTMLContent
                        //4	Channel
                        //5	PhysicalChannel
                        //6	Schedule
                        //7	Picture
                        //8	Category
                        //9	Series

    };
    QVector<CNTV_XML_Vector> objVector;
    QVector<CNTV_XML_Vector> mapVector;

};

#endif // UTUNIT_H
