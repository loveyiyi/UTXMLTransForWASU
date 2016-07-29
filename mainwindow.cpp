#include "mainwindow.h"
#include "filehandle.h"
#include "wasuhandle.h"
#include "utunit.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QUrl>
#include <QMessageBox>
#include <QVector>
#include <QPair>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit->setText("c:/xml");
    ui->lineEdit_2->setText("c:/qt");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showInfo(const QString& message)
{
    QString info;
    info = ui->textEdit->toPlainText();
    info.append(message);
    info.append("\n");
    ui->textEdit->setPlainText(info);
}

void MainWindow::on_pushButton_clicked()
{

    QUrl fileName = QFileDialog::getExistingDirectory(this,"选择输入路径","C:/",QFileDialog::ShowDirsOnly);
    if(fileName.toString().size()>0)
        ui->lineEdit->setText(fileName.toString());
}

void MainWindow::on_pushButton_4_clicked()
{
    QUrl fileName = QFileDialog::getExistingDirectory(this,"选择输出路径","C:/",QFileDialog::ShowDirsOnly);
    if(fileName.toString().size()>0)
        ui->lineEdit_2->setText(fileName.toString());
}

void MainWindow::on_pushButton_2_clicked()
{

    //一系列错误检查
    if(!ui->lineEdit->text().isEmpty() && ui->lineEdit->text() ==  ui->lineEdit_2->text()){
        QMessageBox::critical(this," 错误","输入输出不能相同");
        return;
    }
    if(ui->lineEdit->text().isEmpty() ||  ui->lineEdit_2->text().isEmpty()){
        QMessageBox::critical(this," 错误","请选择输入输出地址");
        return;
    }
    creatCNTVVector();
    return;
}


void MainWindow::creatCNTVVector(){
    QString inputDIR = ui->lineEdit->text();
    QString outputDIR = ui->lineEdit_2->text();

    FileHandle fileVector;
    fileVector.setXMLDir(inputDIR);
    showInfo("目录下共有："+QString::number(fileVector.getXMLCount())+"个文件");
    for(int i = 0;i<fileVector.getXMLCount();i++){
        //每次循环需要重新来的变量
        UTUnit tpUTVector;
        CNTV_XML_Vector cntvVector;
        WASUHandle thisXML;

        QString xmlfile;
        bool ok = fileVector.getFile(xmlfile,i);

        if(ok)
            showInfo("处理："+xmlfile );
        else
            continue;

        QString showstring1;
        thisXML.setXMl(xmlfile);

 /*       thisXML.setXMl(xmlfile);
        thisXML.findValueOnCond("adi:AddMetadataAsset",QStringList()<<"CP23010020160308070700"<<"Title","vod:TitleFull","vod:TitleFull",showstring1);
        showInfo(showstring1);
        thisXML.findValueOnCond("adi:AddMetadataAsset",QStringList()<<"CP23010020160308070700"<<"Title","adi:AssetLifetime","endDateTime",showstring1);
        showInfo(showstring1);
        thisXML.findValue("vod:CategoryPath",teststring);
*/
        QString tpID;
        QVector<QPair<QString,QStringList>>  idVector;


        idVector.clear();
        thisXML.findAssetIDfromWASU(idVector);
        showInfo("AssetID number:"+QString::number(idVector.size()));
        for(int i = 0;i<idVector.size();i++){
            QString programID=idVector[i].first;
            showInfo("asset main:"+programID);
            QString key;
            foreach(key,idVector[i].second){
                showInfo("asset id:"+key);
                showstring1.clear();
                thisXML.findValueOnCond("vod:Title",QStringList()<<programID,"vod:TitleFull",showstring1);
                showInfo("video name:"+showstring1);
                showstring1.clear();
                thisXML.findAttrValueOnCond("vod:Video",QStringList()<<key,"","transferContentURL",showstring1);
                showInfo("video path:"+showstring1);

            }
            showInfo("  ");
        }


        idVector.clear();
        thisXML.findGourpIDfromWASU(idVector);
        showInfo("GroupID number:"+QString::number(idVector.size()));
        for(int i = 0;i<idVector.size();i++){
            showInfo("Group ID:"+idVector[i].first);
            QString key;
            foreach(key,idVector[i].second){
                showInfo("Asset ID:"+key);
            }
            showInfo("  ");
        }


/*
        QString showstring;
        tpUTVector.createUTXML(showstring);
        showInfo(showstring);*/
        showInfo("");
    }



}
