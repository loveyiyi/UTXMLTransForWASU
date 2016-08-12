#ifndef WASUHANDLE_H
#define WASUHANDLE_H
#include <xmlhandle.h>
#include <QVector>
#include <QPair>
#include <QDateTime>
#include <QMap>

#include "utunit.h"

enum MediaType{

};

class WASUHandle : public XMLHandle
{
public:
    WASUHandle();
    bool setNasLocaltion(const QString &);
    bool findAssetIDfromWASU(QMap<QString,QStringList> &);
    bool findGourpIDfromWASU(QMap<QString,QStringList> &);
    bool findGroupAssetIDfromWASU(const QString&,QStringList&);
    bool WASUID2CNTVID(const int&,const QString&,QString&);
    bool WASUID2CNTVID_HD(const int&,const QString&,QString&);
    bool WASUDatetime2CNTV(const QString&,QString&);
    void createXMLforUT(QStringList & ,QStringList &);
    void createFilePath(QString &);
    void createFilePath_HD(QString &);
    void createPicPath(QString &);
protected:
    void findAddItem(const QString&,const QString&,QStringList&);

    //sd
    void handleSeriesXML(QStringList&);
    void handleMovieXML(QStringList&);

    void createSeriesXML(QString &);

    //hd
    void handleSeriesXML_HD(QStringList&);
    void handleMovieXML_HD(QStringList&);

    void createSeriesXML_HD(QString &);

    const int CNTV_T_Program=1;
    const int CNTV_T_Movie=2;
    const int CNTV_T_Cast=3;
    const int CNTV_T_CastRoleMap=4;
    const int CNTV_T_Channel=5;
    const int CNTV_T_PhysicalChannel=6;
    const int CNTV_T_Schedule=7;
    const int CNTV_T_Picture=8;
    const int CNTV_T_Category= 9;
    const int CNTV_T_Series=10;
    const int CNTV_T_Package=11;
private:
    QMap<QString,QStringList>  idVector;
    QMap<QString,QStringList>  idGroupVector;

    QString nasLocaltion;
};

#endif // WASUHANDLE_H
