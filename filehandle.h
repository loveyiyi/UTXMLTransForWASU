#ifndef FILEHANDLE_H
#define FILEHANDLE_H
#include <qstring>
#include <QDir>
#include <QStringList>
#include <QTextStream>


class FileHandle
{
public:
    FileHandle();
    bool setXMLDir(QString&DirName);
    bool getFile(QString&file_name, const int&);
    double getXMLCount();
    bool writeFile(const QString &,const QString &);
private:
    QString XML_Path;
};

#endif // FILEHANDLE_H
