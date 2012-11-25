#include <iostream>
#include "NotsqlInterpreter.h"
#include "NotsqlAPI.h"
#include "NotsqlRecordManager.h"
#include "NotsqlCatalogManager.h"
#include "NotsqlBufferManager.h"
using namespace std;

int main()
{
 //   record_manager.setAPI(&api);

    NotsqlInterpreter inst;

    cout << "TEST INSERT"<< endl;
    try{
        inst.interpret(string( string() +
                    "USE test"));
        inst.interpret(string( string() + 
                    "INSERT INTO test_table " + 
                    "VALUES (\"hehehe\", 123, 2.333)"
        ));
        inst.interpret(string( string() + 
                    "INSERT INTO test_table " + 
                    "VALUES (\"hehehe\", 123, 2.333)"
        ));
        inst.interpret(string( string() + 
                    "INSERT INTO test_table " + 
                    "VALUES (\"hahaha\", 111, 2.333)"
        ));
        cout << "Before delete" << endl;
        inst.interpret(string( string() +
                    "SELECT * from test_table "));
        inst.interpret(string( string() +
                    "DELETE from test_table " + 
                    "where col1 = \"hahaha\" or col1 = \"hehehe\""));
        cout << "After Delete" << endl;
        inst.interpret(string( string() +
                    "SELECT * from test_table "));
    }catch(NotsqlBaseException & e){
        e.showMsg();
    }
    return 0;
}




