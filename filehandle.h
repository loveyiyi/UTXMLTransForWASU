#ifndef FILEHANDLE_H
#define FILEHANDLE_H
#include <qstring>
#include <QDir>
#include <QStringList>


class FileHandle
{
public:
    FileHandle();
    bool setXMLDir(QString&DirName);
    bool getFile(QString&file_name, const int&);
    double getXMLCount();
private:
    QString XML_Path;
};

#endif // FILEHANDLE_H
