#include <iostream>
#include "NotsqlInterpreter.h"
using namespace std;

int main()
{
    NotsqlInterpreter inst;
    try{
        inst.interpret(string("\
                Select * from char where char.char <>123.12334 and testTable.field2= \"123.123\""));
    }catch(NotsqlSyntaxException & e){
        e.showMsg();
    }catch(NotsqlParseException & e){
        e.showMsg();
    }
    return 0;
}

