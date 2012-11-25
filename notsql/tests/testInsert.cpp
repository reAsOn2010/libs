#include <iostream>
#include "NotsqlInterpreter.h"
#include "NotsqlAPI.h"
#include "NotsqlRecordManager.h"
#include "NotsqlCatalogManager.h"
#include "NotsqlBufferManager.h"
using namespace std;

int main()
{
    NotsqlBufferManager buffer_manager;
    NotsqlRecordManager record_manager("test", &buffer_manager);
    NotsqlCatalogManager catalog("test");
    NotsqlAPI api(&catalog, &record_manager);

 //   record_manager.setAPI(&api);

    NotsqlInterpreter inst(&api);

    cout << "TEST INSERT"<< endl;
    try{
        inst.interpret(string( string() + 
                    "INSERT INTO test_table " + 
                    "VALUES (\"hahaha\", 123, 2.333)"
        ));
        inst.interpret(string( string() +
                    "SELECT col1 from test_table " + 
                    "where col1 = \"hahaha\"");
    }catch(NotsqlSyntaxException & e){
        e.showMsg();
    }catch(NotsqlParseException & e){
        e.showMsg();
    }
    return 0;
}




