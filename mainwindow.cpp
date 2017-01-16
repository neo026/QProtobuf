#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
    Google protocol buffer include
*/
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/compiler/importer.h>

#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/tokenizer.h>
#include <google/protobuf/compiler/parser.h>

//
using namespace std;
using namespace google::protobuf;
using namespace google::protobuf::io;
using namespace google::protobuf::compiler;

void GetMessageTypeFromProtoFile(const string& proto_filename,
                                 FileDescriptorProto* file_desc_proto);
void myPrint(const FileDescriptorProto& file_desc_proto);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

      MyDynamicCompile();
    //DynamicCompile();
    //string proto_filename("D:/workspaces/Qt/pb/build-GoogleProtocolBuffer-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug");

    //FileDescriptorProto file_desc_proto;

    //GetMessageTypeFromProtoFile(proto_filename, &file_desc_proto);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void displayAll(const DescriptorPool* pool, const string& name)
{
    std::string str;
    const Descriptor *descriptor = pool->FindMessageTypeByName(name);
    if(NULL == descriptor)
    {
        DBG() << "cannot get the descriptor";
        return;
    }

    str = descriptor->DebugString();
    DBG() << str.c_str();
}

void CreateMessage(const Descriptor *desc)
{
    DynamicMessageFactory factory;
    const Message *temp_msg, *dest_msg;
    const Reflection *reflection;
    const FieldDescriptor *field = NULL;
    string str;

    str = desc->name();
    DBG() << str.c_str();

    //fill first and then new it, this is different from normal
    temp_msg = factory.GetPrototype(desc);
    if(NULL == temp_msg)
    {
        DBG() << "temp message failed.";
        return;
    }

    dest_msg = temp_msg->New();
    if(NULL == dest_msg)
    {
        DBG() << "new message failed.";
        return;
    }


    // upper is finnished to create a new dynamic message.

    // display
    str = dest_msg->DebugString();
    DBG() << str.c_str();

    delete dest_msg;
}

void MainWindow::add2ComboBox(void *_oneofDesc)
{
    const OneofDescriptor* oneofDesc = (const OneofDescriptor*)_oneofDesc;
    QStringList messageList;
    const FieldDescriptor* fieldDesc;

    for(int i = 0; i < oneofDesc->field_count(); i++)
    {
        fieldDesc = oneofDesc->field(i);
        if(NULL == fieldDesc)
        {
            DBG() << "cannot get this field.";
            return;
        }

        //DBG() << fieldDesc->name().c_str();
        ui->comboBox->addItem(fieldDesc);
    }
}

void MainWindow::MyDynamicCompile()
{
    DiskSourceTree sourceTree;
    const FileDescriptor *pFileDesc;
    const Descriptor *pMessageDesc;      // descriptor of each message in the open file
    const OneofDescriptor *pOneofDesc;   // descriptor of a speacil message "oneOf"

    sourceTree.MapPath("", "D:/workspaces/Qt/pb/build-GoogleProtocolBuffer-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug");
    Importer importer(&sourceTree, NULL);

    pFileDesc = importer.Import("beo_sound_wall.proto");
    if(NULL == pFileDesc)
    {
        DBG() << "cannot import this file";
        return;
    }

    //fileDesc = importer.Import("addressbook.proto");
    DBG() << "fileDesc:" << pFileDesc->name().c_str();

    pFileDesc->CopyTo(&fileDescProto);

    int type_count;
    int oneof_count;
    int i, j;

    // look up all the included messages
    type_count = pFileDesc->message_type_count();
    for(i = 0; i < type_count; i++)
    {
        // get each message descriptor
        pMessageDesc = pFileDesc->message_type(i);
        if(NULL == pMessageDesc)
        {
            DBG() << "cannot get this message";
            return;
        }

        // Find all the "oneOf" message
        oneof_count = pMessageDesc->oneof_decl_count();
        for(j = 0; j < oneof_count; j++)
        {
           pOneofDesc = pMessageDesc->oneof_decl(j);
           if(NULL == pOneofDesc)
           {
             DBG() << "pOneofDesc is NULL.";
             return;
           }

           DBG() << "oneOf message>>\n"<< pMessageDesc->DebugString().c_str();

           // add all the message into the widget "Combo Box"
           add2ComboBox((void *)pOneofDesc);
        }
    }
}


void MainWindow::DynamicCompile()
{
    DBG() << ">1";
    string str;
    //
    DiskSourceTree sourceTree;

    //look up .proto file in current directory
    sourceTree.MapPath("","D:/workspaces/Qt/pb/build-GoogleProtocolBuffer-Desktop_Qt_5_7_0_MinGW_32bit-Debug/debug");
    Importer importer(&sourceTree, NULL);

    //runtime compile foo.proto
    importer.Import("foo.proto");
    const Descriptor *descriptor = importer.pool()->FindMessageTypeByName("SoundWallData");

    if(NULL == descriptor)
    {
        DBG() << ">create descriptor fail";
        return;
    }

    str = descriptor->DebugString();
    DBG() << str.c_str();

    DBG() << ">2";
    // build a dynamic message by "Pair" proto
    DynamicMessageFactory factory;
    const Message *message = factory.GetPrototype(descriptor);

    DBG() << ">3";
    // create a real instance of "Pair"
    Message *pair = message->New();

    // write the "Pair" instance by reflection
    const Reflection *reflection = pair->GetReflection();
    const FieldDescriptor *field = NULL;

    DBG() << ">4";
    field = descriptor->FindFieldByName("key");
    reflection->SetString(pair, field,"my key");

    field = descriptor->FindFieldByName("value");
    reflection->SetUInt32(pair, field, 1111);

    str =  pair->DebugString();
    DBG() << str.c_str();
    //delete pair;
    DBG() << ">5";
}

void MainWindow::PbSerialize()
{
#if 0
    const unsigned int MAGIC_NUM = 2988;
    int fd = open("dpb.msg", O_WRONLY|O_CREAT, 0666);

    ZeroCopyOutputStream* raw_output = new FileOutputStream(fd);

    CodedOutputStream* coded_output = new CodedOutputStream(raw_output);
    coded_output->WriteLittleEndian32(MAGIC_NUM);

    string data;

    file_proto.SerializeToString(&data);
    coded_output->WriteVarint32(data.size());
    coded_output->WriteString(data);

    data.clear();
    pair->SerializeToString(&data);
    coded_output->WriteVarint32(data.size());
    coded_output->WriteString(data);

    delete coded_output;
    delete raw_output;

    close(fd);
#endif
}

void MainWindow::PbUnserialize()
{


}

//-----------------------------------------------------------------------------
// Parsing given .proto file for Descriptor of the given message (by
// name).  The returned message descriptor can be used with a
// DynamicMessageFactory in order to create prototype message and
// mutable messages.  For example:
/*
  DynamicMessageFactory factory;
  const Message* prototype_msg = factory.GetPrototype(message_descriptor);
  const Message* mutable_msg = prototype_msg->New();
*/
//-----------------------------------------------------------------------------
void GetMessageTypeFromProtoFile(const string& proto_filename,
                                 FileDescriptorProto* file_desc_proto)
{
    FILE* proto_file = fopen(proto_filename.c_str(), "r");

    if (proto_file == NULL)
    {
        DBG() << "Cannot open .proto file: ";
    }

    FileInputStream proto_input_stream(fileno(proto_file));
    Tokenizer tokenizer(&proto_input_stream, NULL);
    Parser parser;

    if (!parser.Parse(&tokenizer, file_desc_proto))
    {
      DBG() << "Cannot parse .proto file:";
    }

    fclose(proto_file);

  // Here we walk around a bug in protocol buffers that
  // |Parser::Parse| does not set name (.proto filename) in
  // file_desc_proto.
    if (!file_desc_proto->has_name())
    {
        file_desc_proto->set_name(proto_filename);
    }
}

void myPrint(const FileDescriptorProto& file_desc_proto)
{
    string str;

    str = file_desc_proto.DebugString();
    DBG() << "myPrint" << str.c_str();
}

//-----------------------------------------------------------------------------
// Print contents of a record file with following format:
//
//   { <int record_size> <KeyValuePair> }
//
// where KeyValuePair is a proto message defined in mrml.proto, and
// consists of two string fields: key and value, where key will be
// printed as a text string, and value will be parsed into a proto
// message given as |message_descriptor|.
//-----------------------------------------------------------------------------
void PrintDataFile(const string& data_filename,
                   const FileDescriptorProto& file_desc_proto,
                   const string& message_name)
{
#if 0
    FILE* proto_file = fopen(proto_filename.c_str(), "wr");

  // open the input file
  ifstream input_stream(data_filename.c_str());
  ifstream input_stream;

  if(!input_stream.is_open())
  {
    //LOG(FATAL) << "Cannot open data file: " << data_filename;
    DBG() << "Cannot open data file: ";
  }
#endif

  google::protobuf::DescriptorPool pool;
  const google::protobuf::FileDescriptor* file_desc = pool.BuildFile(file_desc_proto);

  if (file_desc == NULL)
  {
    //LOG(FATAL) << "Cannot get file descriptor from file descriptor"
    //           << file_desc_proto.DebugString();
      DBG() << "Cannot get file descriptor from file descriptor";
  }

  const google::protobuf::Descriptor* message_desc = file_desc->FindMessageTypeByName(message_name);
  if (message_desc == NULL)
  {
   // LOG(FATAL) << "Cannot get message descriptor of message: " << message_name;
      DBG() << "Cannot Cannot get message descriptor of message";
  }

  google::protobuf::DynamicMessageFactory factory;
  const google::protobuf::Message* prototype_msg = factory.GetPrototype(message_desc);

  if (prototype_msg == NULL)
  {
    //LOG(FATAL) << "Cannot create prototype message from message descriptor";
       DBG() << "Cannot create prototype message from message descriptor";
  }

  google::protobuf::Message* mutable_msg = prototype_msg->New();
  if (mutable_msg == NULL)
  {
    //LOG(FATAL) << "Failed in prototype_msg->New(); to create mutable message";
      DBG() << "Failed in prototype_msg->New(); to create mutable message";
  }

  string key, value;
#if 0
  while (MRML_ReadRecord(input_stream, &key, &value))
  {
    if (!mutable_msg->ParseFromString(value))
    {
      //LOG(FATAL) << "Failed to parse value in KeyValuePair:" << value;
      DBG() << "Failed to parse value in KeyValuePair:";
    }

    //cout << "Key:" << key << "\nValue:" << mutable_msg->DebugString();
    DBG() << "Key:\nValue:";
  }
#endif

  delete mutable_msg;
}

void MainWindow::on_comboBox_activated(const QString &arg1)
{
    string str;
    DescriptorPool pool;
    const Descriptor *pMessageDesc;
    const FileDescriptor *pFileDesc;
    QString qStr;

    // build it back to FileDescritor
    pFileDesc = pool.BuildFile(this->fileDescProto);
    if(NULL == pFileDesc)
    {
        DBG() << "Build Failed";
        return;
    }

    //str = arg1.toStdString();
    //DBG() << str.c_str();

    int type_count = pFileDesc->message_type_count();
    for(int i = 0; i < type_count; i++)
    {
        pMessageDesc = pFileDesc->message_type(i);
        if(NULL == pMessageDesc)
        {
            DBG() << "cannot find this message.";
            return;
        }

        str = pMessageDesc->name();
        DBG() << "comp"<< str.c_str() << arg1.toStdString().c_str();


        //if(0 == str.compare(arg1.toStdString()))
        {
            DBG() << "Found it." << str.compare(arg1.toStdString());
        }

    }


    return;

    // find the message
    pMessageDesc = pFileDesc->FindMessageTypeByName(arg1.toStdString());
    if(NULL == pMessageDesc)
    {
        DBG() << "cannot find the message.";
        return;
    }

    //str = pMessageDesc->name();
    DBG() << "active:" << pMessageDesc->DebugString().c_str();

}
