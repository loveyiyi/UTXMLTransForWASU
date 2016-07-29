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
        QStringList xmlOutputContent;
        WASUHandle thisXML;
        thisXML.setXMl(xmlfile);
        thisXML.createXMLforUT(xmlOutputContent);
        QString eachProgram;
        showInfo("包含："+QString::number(xmlOutputContent.size())+"个内容" );
        foreach (eachProgram, xmlOutputContent) {
            showInfo(eachProgram);
        }


    }



}
