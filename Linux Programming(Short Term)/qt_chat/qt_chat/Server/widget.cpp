#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    tcpServer = new QTcpServer(this);
    if(!tcpServer->listen(QHostAddress::Any, 9090))
    {
        qDebug() << tcpServer->errorString();
        close();
    }
    ui->textBrowser->append("Waiting for client...");
    //connect(tcpServer, SIGNAL(newConnection()), this, SLOT(sendMessage()));
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(connect_slot()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::connect_slot()
{

    if(tcpSocketList.count() <= TCPSOCKET_MAX)
    {
        QTcpSocket *tcpSocket = tcpServer -> nextPendingConnection();
        connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(recv_slot()));
        tcpSocketList.append(tcpSocket);
        tcpSocket->write(QString("Connected to Server!").toStdString().c_str());
    }

}

void Widget::recv_slot()
{
    for(int i=0 ; i<tcpSocketList.count() ; i++)
    {
        QByteArray byte;
        QTcpSocket *tcpSocket=tcpSocketList.at(i);
        byte = tcpSocket -> readAll();
        qDebug() << byte;
        QString result = QVariant(byte).toString();
        if(result.length()>0)
            ui->textBrowser->append("tcp rev:" +tcpSocket->peerAddress().toString()+":"+ result);
        for(int j = 0; j < tcpSocketList.count(); j++)
        {
            QTcpSocket *tcpSocket = tcpSocketList.at(j);
            tcpSocket->write(result.toStdString().c_str());
        }
    }

}
