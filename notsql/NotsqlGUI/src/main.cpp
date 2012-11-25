#include <QApplication>
#include "NotsqlGUI.h"
#include <string>
#include "NotsqlCLI.h"
#include <iostream>

using namespace std;

int main(int argc, char *args[]){
    if(argc == 1){
        QApplication app(argc, args);
        NotsqlGUI *notsql = new NotsqlGUI;
        notsql->show();
        return app.exec();
    }else{
        NotsqlCLI cli;
        string cmd(args[1]);
        if(cmd == "-f" || cmd == "--file"){
            // Run SQL file
            if(argc != 3){
                cerr << "Invalid arguments." << endl;
            }else{
                cli.feedFile(args[2]);
            }
        }else if((cmd == "-c" || cmd == "--command") && argc == 2){
            // Run CLI
            try{
                cli.interact();
            }catch(NotsqlBaseException & e){
                e.showMsg();
            }
        }else if((cmd == "-h" || cmd == "--help") && argc == 2){
            // Show help
            cout << "Notsql" << endl;
            cout << "   A lightweight database manager system" << endl;
            cout << endl;
            cout << "   Usage: notsql [-h] [-f file_name] [-c]" << endl;
            cout << endl;
            cout << "   Command:" << endl;
            cout << "       -h, --help      Show help infomation" << endl;
            cout << "       -c, --command   Run notsql in command line" << endl;
            cout << "       -f, --file      ";
            cout << "Run the SQL statements in the specific file" << endl;
        }else{
            cerr << "Invalid arguments" << endl;
        }
    }
}
