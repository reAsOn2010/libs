#include "NotsqlCLI.h"
#include <iostream>
#include <fstream>
using namespace std;


void NotsqlCLI::feedFile(const char * file_name){
    ifstream is(file_name);
    if(!is.is_open()){
        throw(NotsqlFileException("Cannot open file " + string(file_name)));
    }
    is.seekg (0, ios::end);
    unsigned int length = is.tellg();
    is.seekg (0, ios::beg);
    string stmt;
    char buf[NOTSQL_BUF_MAX_LEN];
    unsigned int l_count = 0;
    int read_length;
    // Read data
    while(true){
        if(length - l_count > NOTSQL_BUF_MAX_LEN){
            read_length = NOTSQL_BUF_MAX_LEN;
        }else{
            read_length = length - l_count;
        }
        l_count += read_length;
        is.read(buf, read_length);
        stmt += string(buf, read_length);
        if(l_count >= length){
            break;
        }
    }

    unsigned int start_index = 0;
    for(unsigned int i = 0; i != stmt.length(); ++i){
        if(stmt[i] == ';'){
            try{
                interpreter->interpret(stmt.substr(start_index, 
                            i - start_index + 1));
            }catch(NotsqlQuitException & e){
                break;
            }catch(NotsqlBaseException & e){
                e.showMsg();
                break;
            }
            start_index = i + 1;
        }
    }
}


void NotsqlCLI::interact(){
    bool complete = true;
    string line;
    string tmp_line;
    while(true){
        if(complete){
            cout << PS1;
        }else{
            cout << PS2;
        }
        getline(cin, tmp_line);
        int i;
        for(i = (int)(tmp_line.size()) - 1; 
                i >= 0 && tmp_line[i] != ';'; i--);
        if(i == -1){
            line += tmp_line + " ";
            complete = false;
        }else{
            line += tmp_line.substr(0, i);
            try{
                interpreter->interpret(line);
            }catch(NotsqlQuitException & e){
                cout << "Bye." << endl;
                break;
            }catch(NotsqlBaseException & e){
                e.showMsg();
            }
            line = tmp_line.substr(i + 1, 
                    tmp_line.size() - i - 1);
            unsigned int line_i;
            for(line_i = 0; line_i != line.size(); ++line_i){
                if(line[line_i] != ' ' &&
                        line[line_i] != '\t'){
                    break;
                }
            }
            if(line_i == line.size()){
                complete = true;
            }else{
                complete = false;
                line = line.substr(line_i, line.size() - line_i) + " ";
            }
        }
    }
}

string NotsqlCLI::PS1 = ">>> ";
string NotsqlCLI::PS2 = "... ";
