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

    cout << "TEST CREATE INDEX"<< endl;
    try{
        inst.interpret(string( string() +
                    "CREATE INDEX test_index ON test_table (col1, col2)"));
    }catch(NotsqlSyntaxException & e){
        e.showMsg();
    }catch(NotsqlParseException & e){
        e.showMsg();
    }catch(NotsqlFieldNotFoundException & e){
        e.showMsg();
    }
    return 0;
}




