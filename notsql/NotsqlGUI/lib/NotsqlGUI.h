#ifndef _NOTSQL_GUI_H_
#define _NOTSQL_GUI_H_
#include "ui_NotsqlGUI.h"
#include <QMainWindow>
#include <QFileDialog>
#include "NotsqlInterpreter.h"

using namespace std;

class NotsqlGUI : public QMainWindow, public Ui::NotsqlGUI
{
    Q_OBJECT

public:
    NotsqlGUI(QMainWindow * parent=NULL);

private slots:
    void slotClear();
    void slotSubmit();
    void slotLoad();

private:
    QFileDialog * fileDialog;
    NotsqlInterpreter interpreter;

};
#endif
