#ifndef _NOTSQL_LOGGER_H_
#define _NOTSQL_LOGGER_H_
#include <iostream>
#include <fstream>
using namespace std;

class NotsqlLogger{
public:
    NotsqlLogger(const char * log_name, bool time_on=false) :
    	fin(log_name, ios::out|ios::app), time_on(time_on) {};
    virtual ~NotsqlLogger();

    void write(string content);
    void write(string header, string content);

private:
    ofstream fin;
    bool time_on;
};

#endif 
