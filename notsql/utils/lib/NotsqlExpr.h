#ifndef _NOTSQL_EXPR_H_
#define _NOTSQL_EXPR_H_
#include <string>
#include <iostream>
#include "NotsqlConstants.h"
using namespace std;

/**
 * Expression class
 *
 * public fields:
 *      table       Name of table
 *      col         Name of column
 *      opr         Operator (Macro)
 *      value       Value of operand
 *      type        Type of operand
 *
 *  if it is a join expression, two extra field:
 *      table2      Name of the table to join 
 *      col2        Name of the field to join
 */
class NotsqlExpr{
public:
    NotsqlExpr(){};
    NotsqlExpr(
            const string & table, 
            const string & col, 
            TK_TYPE opr, 
            const string & value,
            TK_TYPE type,
            const string & table2,
            const string & col2
    );

    NotsqlExpr(const  NotsqlExpr & expr){
        this->table = expr.table;
        this->col = expr.col;
        this->opr = expr.opr;
        this->value = expr.value;
        this->type = expr.type;
        this->table2 = expr.table2;
        this->col2 = expr.col2;
    }

    string table;
    string col;
    TK_TYPE opr;
    string value;
    TK_TYPE type;
    string table2;
    string col2;
};

#endif
