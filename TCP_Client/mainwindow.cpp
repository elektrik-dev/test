#include "mainwindow.h"
#include <QHostAddress>
#include <QPushButton>
#include "ui_mainwindow.h"
#include <QFile>
#include <QTime>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),nextBlockSize(0)
{
    ui->setupUi(this);
    QTcpSocket *sock = new QTcpSocket;
    sock->connectToHost(QHostAddress::LocalHost, 6000);
    if(sock->waitForConnected()){
        ui->history->append(QString("Connected to host %1 at %2").arg(QHostAddress::LocalHost).arg(QTime::currentTime().toString()));
        connect(sock, &QTcpSocket::readyRead, this, &MainWindow::readTcpData);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::readTcpData(){
    QTcpSocket *sock = qobject_cast<QTcpSocket*>(sender());
    QByteArray arr;
    QFile reseivedFile;
    QDataStream in(sock);
    ui->history->append("start reading");
    for (;;) {
        if(!nextBlockSize){
            if(sock->bytesAvailable() < static_cast<long long>(sizeof(quint16))){
                break;
            }
            in>>nextBlockSize;
        }
        if(sock->bytesAvailable() < nextBlockSize)
            break;
        QTime timeS;
        QString fileName;
        in>>timeS>>fileName;
        reseivedFile.setFileName(fileName);
        ui->history->append(QString("Reseived file %1 at %2").arg(fileName).arg(timeS.toString()));
        in>>arr;
        sendConfirm(sock);
        nextBlockSize = 0;
    }
    if(reseivedFile.open(QIODevice::WriteOnly))
        reseivedFile.write(arr);
    reseivedFile.close();
}
void MainWindow::sendConfirm(QTcpSocket *conn){
    QByteArray confStr;
    QDataStream out(&confStr, QIODevice::WriteOnly);
    out<<quint16(0)<<QString("Client: Reseived %1 bytes").arg(nextBlockSize);
    out.device()->seek(0);
    out<<quint16(static_cast<unsigned long>(confStr.size()) - sizeof(quint16));
    conn->write(confStr);
    slotDisconnect(conn);
    connect(conn, &QTcpSocket::disconnected, conn, &QTcpSocket::deleteLater);
}
void MainWindow::slotDisconnect(QTcpSocket* sock){
    sock->close();
    QTimer::singleShot(3000, this, &MainWindow::close);
}
