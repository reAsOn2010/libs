#include <iostream>
#include "NotsqlInterpreter.h"
#include "NotsqlAPI.h"
#include "NotsqlCatalogManager.h"
using namespace std;

int main()
{
    try{
        NotsqlCatalogManager::CreateDatabase("test");
    }catch(...){}

    NotsqlCatalogManager catalog("test");
	cout << NotsqlCatalogManager::DatabaseIsExist("ABC");

    NotsqlAPI api(&catalog);
    NotsqlInterpreter inst(&api);
    cout << "TEST CREATE TABLE"<< endl;
    if(!catalog.TableIsExist(string("test_table"))){
        try{
            inst.interpret(string( string() + "CREATE table test_table(" + 
                        "col1 char(10 ) not null," +
                        "col2 int not null, " +
                        "col3 float not null, " + 
                        "primary key(col1, col2))"));
        }catch(NotsqlSyntaxException & e){
            e.showMsg();
        }catch(NotsqlParseException & e){
            e.showMsg();
        }catch(...){
            cerr << "Error" << endl;
        }
    }

    cout << "Test ReturnTableNum:   " << catalog.ReturnTableNum() << endl;

    cout << "Check table: " << endl;
    vector<CatalogAttr> attrs = catalog.ReturnAttrList(string("test_table"));
    for(int i = 0; i != attrs.size(); ++i){
        cout << "-----------------" << endl;
        cout << attrs[i].AttrName << endl;
        cout << attrs[i].AttrIsPrimary << endl;
        cout << attrs[i].AttrType << endl;
        cout << attrs[i].AttrLength << endl;
        cout << attrs[i].AttrIndex << endl;
    }

            /*
               cout << endl << "TEST CREATE INDEX" << endl;
               try{
               inst.interpret(string( string() + 
               "CREATE iNdex test_index On test_table(" + 
               " field1, "
               " field2 , "
               "fiel_d3 )"));
               }catch(NotsqlSyntaxException & e){
               e.showMsg();
               }catch(NotsqlParseException & e){
               e.showMsg();
               }
               */
            return 0;
            }



