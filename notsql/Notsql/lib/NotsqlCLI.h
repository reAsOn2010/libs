#ifndef _NOTSQL_CLI_H_
#define _NOTSQL_CLI_H_

#ifndef NOTSQL_BUF_MAX_LEN
 #define NOTSQL_BUF_MAX_LEN 500
#endif

#include "NotsqlInterpreter.h"
#include <iostream>
#include <string>

using namespace std;

class NotsqlCLI{
public:
    NotsqlCLI(){
        interpreter = new NotsqlInterpreter();
    }

    virtual ~NotsqlCLI(){
        delete interpreter;
    }

    void feedFile(const char * file_name);

    void interact();
    //void interact();
private:
    NotsqlInterpreter * interpreter;
    static string PS1;
    static string PS2;
};
#endif
