#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QFile>
#include <QBuffer>
#include <QDataStream>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myServ = new QTcpServer(this);
    if(!myServ->listen(QHostAddress::LocalHost, 6000)){
        qDebug()<<"Server not started";
    }else{
        ui->messageHistory->append(QString("Server %1 started at %2").arg(myServ->serverAddress().toString()).arg(QTime::currentTime().toString()));
        qDebug()<<"Server started";
    }
    connect(myServ, &QTcpServer::newConnection, this, &MainWindow::slotNewConnect);
    newFile = new QFile("response.txt");
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::slotNewConnect(){
    QTcpSocket *sock = myServ->nextPendingConnection();
    connect(sock, &QTcpSocket::readyRead, this, &MainWindow::slotRead);
    ui->messageHistory->append(QString("New connection from %1 at %2").arg(sock->peerAddress().toString()).arg(QTime::currentTime().toString()));
    if(!newFile->open(QIODevice::ReadOnly))
    {
        ui->messageHistory->append(QString("File not %1 opened").arg(newFile->fileName()));
        qDebug()<<"File not opened";
    }
    QByteArray arrF;
    QDataStream out(&arrF, QIODevice::WriteOnly);
    out<<quint16(0)<<QTime::currentTime()<<newFile->fileName()<<newFile->readAll();
    out.device()->seek(0);
    messageSize = quint64(static_cast<unsigned long>(arrF.size())- sizeof(quint16));
    ui->messageHistory->append(QString("Send %1 bytes").arg(messageSize));
    out<<quint16(static_cast<unsigned long>(arrF.size()) - sizeof(quint16));
    sock->write(arrF);
    newFile->close();

    connect(sock, &QTcpSocket::disconnected,sock, &QTcpSocket::deleteLater);
}
void MainWindow::slotRead(){
    QTcpSocket *sock = qobject_cast<QTcpSocket*>(sender());
    QDataStream in(sock);
    quint16 blockSize = 0;

    for (;;) {
        if(!blockSize){
            if(sock->bytesAvailable()< static_cast<long long>(sizeof (quint16)))
                break;
            in>>blockSize;
        }
        if(sock->bytesAvailable() < blockSize){
            break;
        }
        QString str;
        quint16 size;
        in>>str>>size;
        ui->messageHistory->append(str);
        blockSize=0;
    }
    slotDisconnect(sock);
}
void MainWindow::slotDisconnect(QTcpSocket *sock){
    sock->close();
}
