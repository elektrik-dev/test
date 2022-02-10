#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

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
    void readTcpData();
private:
    void sendConfirm(QTcpSocket*);
    void slotDisconnect(QTcpSocket*);
    Ui::MainWindow *ui;
    quint16 nextBlockSize;
};

#endif // MAINWINDOW_H
