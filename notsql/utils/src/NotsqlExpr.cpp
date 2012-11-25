#include "NotsqlExpr.h"
#include "NotsqlConstants.h"
#include <iostream>
using namespace std;

NotsqlExpr::NotsqlExpr(
        const string & table, 
        const string & col, 
        TK_TYPE opr, 
        const string & value,
        TK_TYPE type,
        const string & table2,
        const string & col2){
    this->table = table;
    this->col = col;
    this->opr = opr;
    this->value = value;
    this->type = type;
    this->table2 = table2;
    this->col2 = col2;
}
