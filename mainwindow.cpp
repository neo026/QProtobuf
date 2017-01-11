#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
    Google protocol buffer include
*/
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/compiler/importer.h>

//
using namespace std;
using namespace google::protobuf;
using namespace google::protobuf::compiler;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DBG() << "Great!";

    //
    DiskSourceTree sourceTree;

    //look up .proto file in current directory
    sourceTree.MapPath("","./");
    Importer importer(&sourceTree, NULL);

    //runtime compile foo.proto
    importer.Import("foo.proto");

    const Descriptor *descriptor = importer.pool()->FindMessageTypeByName("Pair");
    //DBG() << descriptor->DebugString();

    // build a dynamic message by "Pair" proto
    DynamicMessageFactory factory;
    const Message *message = factory.GetPrototype(descriptor);

    // create a real instance of "Pair"
    Message *pair = message->New();

    // write the "Pair" instance by reflection
    const Reflection *reflection = pair->GetReflection();
    const FieldDescriptor *field = NULL;

    field = descriptor->FindFieldByName("key");
    reflection->SetString(pair, field,"my key");
    field = descriptor->FindFieldByName("value");
    reflection->SetUInt32(pair, field, 1111);
    //DBG() << pair->DebugString();

    delete pair;
}

MainWindow::~MainWindow()
{
    delete ui;
}
