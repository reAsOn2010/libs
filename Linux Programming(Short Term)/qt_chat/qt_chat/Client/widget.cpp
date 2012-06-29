#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->talkButton->hide();
    ui->talkLineEdit->hide();
    ui->nameShowLabel->hide();
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()),this, SLOT(readMessage()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));

    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(on_connectButton_clicked()));
    connect(ui->talkButton,SIGNAL(clicked()),this, SLOT(talk()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::newConnect()
{
    blockSize = 0;
    tcpSocket->abort();
    tcpSocket->connectToHost(ui->hostLineEdit->text(),
                             ui->portLineEdit->text().toInt());

}

void Widget::readMessage()
{
    QByteArray byte;
    byte = tcpSocket -> readAll();
    qDebug() << byte;
    QString result = QVariant(byte).toString();
    ui->textBrowser->append(result.toStdString().c_str());
    ui->nameShowLabel->show();
}

void Widget::displayError(QAbstractSocket::SocketError)
{
    qDebug() << tcpSocket->errorString();
    ui->textBrowser->append(tcpSocket->errorString());
}

void Widget::on_connectButton_clicked()
{
    newConnect();
    ui->talkLineEdit->show();
    ui->talkButton->show();
}
void Widget::talk()
{
    name = ui->nameLineEdit->text();
    ui->nameShowLabel->setText(name);
    QString send_text = ui->talkLineEdit->text();
    tcpSocket->write((name + ": " + send_text).toStdString().c_str());
}
