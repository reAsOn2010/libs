#include "NotsqlGUI.h"
#include <iostream>
#include <QMainWindow>
#include <QDialog>
#include <fstream>
using namespace std;

NotsqlGUI::NotsqlGUI(QMainWindow * parent) : 
    QMainWindow(parent)
{
    setupUi(this);

    connect(clearBtn, SIGNAL(clicked()), this, SLOT(slotClear()));
    connect(submitBtn, SIGNAL(clicked()), this, SLOT(slotSubmit()));
    connect(loadBtn, SIGNAL(clicked()), this, SLOT(slotLoad()));
}

void NotsqlGUI::slotClear(){
    inputTextEdit->clear();
}

void NotsqlGUI::slotSubmit(){
    outputTextEdit->clear();
    outputTableWidget->setRowCount(0);
    outputTableWidget->setColumnCount(1);
    string stmt = inputTextEdit->toPlainText().toStdString();
    QTextCursor cursor = outputTextEdit->textCursor();
    unsigned int start_index = 0;
    interpreter.reset();
    for(unsigned int i = 0; i != stmt.length(); ++i){
        //cursor.movePosition(QTextEdit::MoveEnd);
        if(stmt[i] == ';'){
            try{
                interpreter.interpret(stmt.substr(start_index, 
                            i - start_index + 1));
            }catch(NotsqlBaseException & e){
                cursor.insertHtml("<font color=\"Red\">" + 
                        QString::fromStdString(e.getMsg()) + 
                        "</font>");
                break;
            }
            if(interpreter.getResStr() != ""){
                cursor.insertHtml("<font color=\"Black\">" + 
                        QString::fromStdString(interpreter.getResStr())
                        + "</font>");
            }
            start_index = i + 1;
        }
    }
    NotsqlTable res_table = interpreter.getResTable();
    vector<string> field_names = res_table.getFieldNames();
    if(field_names.size()){
        vector< vector<string> > values = res_table.getStringValues();
        unsigned int records_num = res_table.getRecordsNum();
        outputTableWidget->setRowCount(records_num);
        outputTableWidget->setColumnCount(field_names.size());
        for(unsigned int i = 0; i != field_names.size(); ++i){
            unsigned int col_i = i;
            outputTableWidget->setHorizontalHeaderItem(col_i, 
                    new QTableWidgetItem(QString::fromStdString(field_names[i])));
            for(unsigned int row_j = 0; row_j != records_num; ++row_j){
                outputTableWidget->setItem(row_j, col_i,
                        new QTableWidgetItem(
                            QString::fromStdString(values[i][row_j])));
            }
        }
    }
}

void NotsqlGUI::slotLoad(){
    QString s = QFileDialog::getOpenFileName(
            this,
            "Select a SQL file",
            ".");
    if(s == NULL){
        return;
    }
    ifstream fin(s.toStdString().c_str());
    if(!fin.is_open()){
        outputTextEdit->clear();
        QTextCursor cursor = outputTextEdit->textCursor();
        cursor.insertHtml("<font color=\"Red\">" + 
                QString::fromStdString( string() + 
                    "Error open file \"" + s.toStdString() + "\"") +
                "</font>");
        return;
    }
    string line;
    inputTextEdit->clear();
    while(getline(fin, line)){
        inputTextEdit->append(QString::fromStdString(
                    line + "\n"));
    }
    fin.close();
}
