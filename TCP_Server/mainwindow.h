#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QWidget>
#include <QFile>
#include <QVector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void slotNewConnect();
    void slotDisconnect(QTcpSocket*);
    void slotRead();
private:
    Ui::MainWindow *ui;
    QTcpServer *myServ;
    int i;
    quint64 messageSize;
};

#endif // MAINWINDOW_H
