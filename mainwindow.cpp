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
#include <QDateTime>



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

    ui->textEdit->clear();
    creatCNTVVector();
    return;
}


void MainWindow::creatCNTVVector(){
    QString inputDIR = ui->lineEdit->text();
    QString outputDIR = ui->lineEdit_2->text();

    FileHandle fileVector;
    fileVector.setXMLDir(inputDIR);
    showInfo("目录下共有："+QString::number(fileVector.getXMLCount())+"个文件");

    //开始循环文件
    for(int i = 0;i<fileVector.getXMLCount();i++){
        //确认文件以及错误修正
        QString xmlfile;
        bool ok = fileVector.getFile(xmlfile,i);
        if(ok)
            showInfo("处理："+xmlfile );
        else
            continue;

        //设置读取的文件
        QStringList xmlOutputContent,xmlOutputContent_hd;
        WASUHandle thisXML;
        thisXML.setXMl(xmlfile);
        thisXML.createXMLforUT(xmlOutputContent,xmlOutputContent_hd);
        QString eachProgram;
        showInfo("包含："+QString::number(xmlOutputContent.size()+xmlOutputContent_hd.size())+"个内容" );
        int subNumber = 1;
        foreach (eachProgram, xmlOutputContent) {
            //showInfo(eachProgram);
            QString tmpFilePath;
            tmpFilePath = xmlfile;
            tmpFilePath.remove(inputDIR+"/");
            tmpFilePath.insert(0,outputDIR+"/"+QString::number(subNumber)+"_sd_");
            //tmpFilePath.append(xmlfile);
            //写入文件
            showInfo("生成标清文件:"+tmpFilePath);
            bool ok = fileVector.writeFile(tmpFilePath,eachProgram);
            if(!ok)
                showInfo("文件出错");
            subNumber++;
        }
        subNumber = 1;
        foreach (eachProgram, xmlOutputContent_hd) {
            //showInfo(eachProgram);
            QString tmpFilePath;
            tmpFilePath = xmlfile;
            tmpFilePath.remove(inputDIR+"/");
            tmpFilePath.insert(0,outputDIR+"/"+QString::number(subNumber)+"_hd_");
            //tmpFilePath.append(xmlfile);
            //写入文件
            showInfo("生成高清文件:"+tmpFilePath);
            bool ok = fileVector.writeFile(tmpFilePath,eachProgram);
            if(!ok)
                showInfo("文件出错");
            subNumber++;
        }



    }

    QString logFilePath = outputDIR+"/"+ QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")+"_trans.log";
    bool ok = fileVector.writeFile(logFilePath,ui->textEdit->toPlainText());

}
