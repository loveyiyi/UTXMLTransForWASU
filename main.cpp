#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QFileInfo>
#include <QStringList>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();/*
    QTextCodec *xcodec = QTextCodec::codecForLocale() ;
    QString exeDir = xcodec->toUnicode( QByteArray(argv[0]) ) ;
    QString BKE_CURRENT_DIR = QFileInfo(exeDir).path() ;
    QStringList  libpath;
    libpath << BKE_CURRENT_DIR+QString::fromLocal8Bit("/plugins/platforms");
    libpath << BKE_CURRENT_DIR <<BKE_CURRENT_DIR+QString::fromLocal8Bit("/plugins/imageformats");
    libpath << BKE_CURRENT_DIR+QString::fromLocal8Bit("/plugins");
    libpath << QApplication::libraryPaths();
    QApplication::setLibraryPaths(libpath) ;*/
    return a.exec();
}
