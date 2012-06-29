#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork>

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
    Ui::Widget *ui; //User interface class
    QTcpSocket *tcpSocket; //Client Socket pointer
    QString message; //message to send to the server
    QString name; //User's name
private slots:
    void newConnect(); //create an connetion towards server
    void readMessage(); //receive message from server
    void displayError(QAbstractSocket::SocketError); //display all the error to the user
    void on_connectButton_clicked(); //when connect button clicked call this to create new Connection
    void talk(); //send the message to server
};

#endif // WIDGET_H
