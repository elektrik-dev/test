#include "basewidget.h"
#include "ui_basewidget.h"
#include <QApplication>
#include <QIODevice>
#include <QFileInfoList>
#include <QFileInfo>
#include <QDebug>
#include <QChar>
#include <QRegExp>
#include <QtSql>

//Для генерации случайных чисел и символов
#define firs_value_num 48 //ASCII код нуля
#define last_value_num 10

#define firs_value_symb 65//ASCII код буквы А
#define last_value_symb 25

#define strLength 8

baseWidget::baseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::baseWidget)
{
    ui->setupUi(this);

    connect(ui->showFileButton, &QPushButton::clicked, this, &baseWidget::showFile);
    connect(ui->createFileButton, &QPushButton::clicked, this, &baseWidget::createNewFile);
    connect(ui->CsqLite, &QPushButton::clicked, this, &baseWidget::cslot);
}

baseWidget::~baseWidget()
{
    delete ui;
}
void baseWidget::cslot(){
    QFile sqFile(ui->lineEdit->text()+".csv");
    if(!sqFile.exists()){
        qWarning()<<"File not exist, press button A";
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setConnectOptions("QSQLITE_ENABLE_REGEXP");
    db.setDatabaseName("TestDataBase");
    db.setUserName("test");
    db.setPassword("123");
    if(!db.open()){
        qWarning()<<"database error";
    }
    QString quer = "CREATE TABLE csvTable ("
                   "number0 integer PRIMARY KEY NOT NULL, "
                   "number1 VARCHAR(255), "
                   "number2 VARCHAR(255), "
                   "number3 VARCHAR(255), "
                   "number4 VARCHAR(255), "
                   "number5 VARCHAR(255), "
                   "number6 VARCHAR(255) "
                   ");";

    QSqlQuery firstQuery;
    if(!firstQuery.exec(quer)){
        QSqlError er = firstQuery.lastError();
        qWarning()<<"query error"<<er.text();
    }


    QTextStream qts(&sqFile);
    if(sqFile.open(QIODevice::ReadOnly)){
        qDebug()<<"All is OK";
    }
        int colNum=0;
        while (!qts.atEnd()) {
            QStringList str = qts.readLine().split(',');
            QString q = "INSERT INTO csvTable (number0, number1, number2, number3, number4, number5, number6)"
                         "VALUES(%1, '%2', '%3', '%4', '%5', '%6', '%7');";
            QString strTmp = q.arg(colNum).arg(str.at(0).toLocal8Bit().constData())
                                          .arg(str.at(1).toLocal8Bit().constData())
                                          .arg(str.at(2).toLocal8Bit().constData())
                                          .arg(str.at(3).toLocal8Bit().constData())
                                          .arg(str.at(4).toLocal8Bit().constData())
                                          .arg(str.at(5).toLocal8Bit().constData());
            QSqlQuery qr;
            if(!qr.exec(strTmp)){
                QSqlError er = qr.lastError();
                qWarning()<<"query error write"<<er.text();
                break;
            }
            colNum++;
        }
       sqFile.close();

     QString delQ = "delete from csvTable where number2 regexp '^[0-9]';";
     QSqlQuery last;
     if(!last.exec(delQ)){
         qDebug()<<last.lastError().text();
     }
}
int baseWidget::randSumb(){
    return firs_value_symb + rand() % last_value_symb;
}
int baseWidget::randNum(){
    return firs_value_num + rand() % last_value_num;
}
QString baseWidget::genRandomStr(){
    QString randStr;
    int random_num=0;
    for (int i = 0; i < strLength; ++i) {
        random_num = 1 + rand() % 2;
        randStr[i] = random_num==1?randNum():randSumb();
    }
    return randStr;
}
void baseWidget::createNewFile(){
    QFile newFile(ui->lineEdit->text()+".csv");
    if(!newFile.open(QIODevice::ReadWrite))
    {
        qWarning("File not open!");
    }
    QTextStream txs(&newFile);
    QString ourStr;
    for (int i = 0;i<1024;i++) {
        if(i>0)
            txs<<'\n';
        for (int j=0;j<6;j++) {
            txs<<genRandomStr() + ',';
        }
    }
    newFile.close();
}
void baseWidget::showFile(){
    QString MyFileName = ui->lineEdit->text()+".csv";
    QFile ourFile;
    ourFile.setFileName(MyFileName);
    if(!ourFile.open(QIODevice::ReadWrite))
    {
        qWarning("File not opened!");
    }


    QByteArray bf;
    bf = ourFile.readAll();
    ourFile.close();

    QFile newFile(MyFileName.left(MyFileName.length()-4) + "edited"+".csv");
    if(!newFile.open(QIODevice::WriteOnly)){
        qWarning("File not opened!");
    }

    QRegExp NumExp("[13579]");
    QRegExp SymbExp("^[AEIOYU]");
    QTextStream ts(bf);
    while (!ts.atEnd()) {
        QString str = ts.readLine();
        str.replace(NumExp,QString("#"));
        int i=0;
        foreach (QString ts, str.split(',')) {
            if(ts.length()>0){
                if(!ts.contains(SymbExp))
                {
                    if(i>=5)
                        newFile.write(QString(str + '\n').toUtf8());
                }else {
                    i=0;
                    break;
                }
                i++;
            }

        }
    }
    newFile.close();
}
