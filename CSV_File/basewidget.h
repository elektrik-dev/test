#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QDir>
#include <QTextStream>

namespace Ui {
class baseWidget;
}

class baseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit baseWidget(QWidget *parent = nullptr);
    QString genRandomStr();
    int randSumb();
    int randNum();
    ~baseWidget();
private slots:
    void showFile();
    void createNewFile();
    void cslot();
private:
    QString directName;
    Ui::baseWidget *ui;
};

#endif // BASEWIDGET_H
