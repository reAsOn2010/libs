/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created: Thu Jun 28 06:47:29 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTextBrowser>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QLabel *label;
    QLabel *label_2;
    QLineEdit *hostLineEdit;
    QLineEdit *portLineEdit;
    QLabel *nameShowLabel;
    QPushButton *connectButton;
    QPushButton *talkButton;
    QLineEdit *talkLineEdit;
    QTextBrowser *textBrowser;
    QLineEdit *nameLineEdit;
    QLabel *nameLabel;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(460, 300);
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 0, 71, 31));
        label_2 = new QLabel(Widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(200, 0, 58, 31));
        hostLineEdit = new QLineEdit(Widget);
        hostLineEdit->setObjectName(QString::fromUtf8("hostLineEdit"));
        hostLineEdit->setGeometry(QRect(80, 0, 113, 27));
        portLineEdit = new QLineEdit(Widget);
        portLineEdit->setObjectName(QString::fromUtf8("portLineEdit"));
        portLineEdit->setGeometry(QRect(240, 0, 113, 27));
        nameShowLabel = new QLabel(Widget);
        nameShowLabel->setObjectName(QString::fromUtf8("nameShowLabel"));
        nameShowLabel->setGeometry(QRect(70, 260, 101, 31));
        connectButton = new QPushButton(Widget);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));
        connectButton->setGeometry(QRect(360, 0, 87, 27));
        talkButton = new QPushButton(Widget);
        talkButton->setObjectName(QString::fromUtf8("talkButton"));
        talkButton->setGeometry(QRect(300, 260, 87, 27));
        talkLineEdit = new QLineEdit(Widget);
        talkLineEdit->setObjectName(QString::fromUtf8("talkLineEdit"));
        talkLineEdit->setGeometry(QRect(170, 260, 113, 27));
        textBrowser = new QTextBrowser(Widget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(80, 60, 311, 191));
        nameLineEdit = new QLineEdit(Widget);
        nameLineEdit->setObjectName(QString::fromUtf8("nameLineEdit"));
        nameLineEdit->setGeometry(QRect(150, 30, 113, 27));
        nameLabel = new QLabel(Widget);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));
        nameLabel->setGeometry(QRect(10, 30, 121, 31));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Widget", "Server Host", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Widget", "Port", 0, QApplication::UnicodeUTF8));
        nameShowLabel->setText(QApplication::translate("Widget", "Your Name", 0, QApplication::UnicodeUTF8));
        connectButton->setText(QApplication::translate("Widget", "Connect", 0, QApplication::UnicodeUTF8));
        talkButton->setText(QApplication::translate("Widget", "Talk", 0, QApplication::UnicodeUTF8));
        nameLabel->setText(QApplication::translate("Widget", "What's your Name?", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
