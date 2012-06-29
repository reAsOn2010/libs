#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork>
#define TCPSOCKET_MAX 10
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    
private:
    Ui::Widget *ui; //User Interface class
    QTcpServer *tcpServer; //Socket server pointer
    QList<QTcpSocket*> tcpSocketList; //to store the connected Sockets
private slots:
    void connect_slot(); //when receive an connect request. call this function
    void recv_slot(); //receive the message and send to all client
};

#endif // WIDGET_H
