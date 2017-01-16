#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>

#include <google/protobuf/descriptor.pb.h>

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

using namespace google::protobuf;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void MyDynamicCompile();
    void DynamicCompile();
    void PbSerialize();
    void PbUnserialize();
    void add2ComboBox(void* oneofDesc);

private slots:
    void on_comboBox_activated(const QString &arg1);

private:
    Ui::MainWindow *ui;
    FileDescriptorProto fileDescProto;
};

#endif // MAINWINDOW_H
