#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>

#define MAINWINDOW_DEBUG_ENABLE

#ifdef MAINWINDOW_DEBUG_ENABLE
#include <qdebug.h>
#define DBG qDebug
#else
#define DBG
#endif

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
