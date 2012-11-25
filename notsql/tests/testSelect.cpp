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
                    "INSERT INTO test_table " + 
                    "VALUES (\"hahaha\", 123, 2.333)"
        ));
        inst.interpret(string( string() +
                    "SELECT * from test_table " + 
                    "where col1 = \"hahaha\""));
    }catch(NotsqlSyntaxException & e){
        e.showMsg();
    }catch(NotsqlParseException & e){
        e.showMsg();
    }catch(NotsqlFieldNotFoundException & e){
        e.showMsg();
    }
    return 0;
}




