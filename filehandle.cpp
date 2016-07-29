#include "filehandle.h"

FileHandle::FileHandle()
{
    XML_Path.clear();
}

bool FileHandle::setXMLDir(QString& DirName)
{
    if(XML_Path.isNull())
        XML_Path = DirName;
    return true;
}

double FileHandle::getXMLCount()
{
    if(XML_Path.isNull())
        return 0;
    QDir curXmlPath(XML_Path);
    QStringList  curXMLList = curXmlPath.entryList(QStringList()<<"*.xml"); //只搜索XML文件
    return curXMLList.size();


}

bool FileHandle::getFile(QString& file_name,const int& ordernumber)
{
    if(XML_Path.isNull())
        return false;

    QStringList curXMLList;
    QDir curXmlPath(XML_Path);

    curXMLList = curXmlPath.entryList(QStringList()<<"*.xml"); //只搜索XML文件
    //curXMLList = curXmlPath.entryList(QDir::Files | QDir::NoDotAndDotDot);


    QString temp;
    double allcount = 0;
    foreach( temp,curXMLList ){
        if(allcount != ordernumber) { //检查是不是要的文件
            allcount += 1;
            continue;
        }

        if(temp.size() < 5)  //文件名太短就取消
            return false;

        file_name = XML_Path +"/"+temp;
        break;
    }
    return true;

}

bool FileHandle::writeFile(const QString &fileName, const QString &fileContent){
    QFile outFile(fileName);
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
    QString utf2gbk = gbk->toUnicode(fileContent.toLocal8Bit());
    //QString g2u = gbk->toUnicode(gbk->fromUnicode(fileContent));
    if(outFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream ts(&outFile);
        ts.setCodec(utf8);
        ts << fileContent << endl;
        return true;
    }else
        return false;

}
