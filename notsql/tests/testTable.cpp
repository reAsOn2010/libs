#include "NotsqlTable.h"
#include <iostream>
#include <vector>
using namespace std;

int main(){
    string table_name1 = "Table1";
    string table_name2 = "Table2";
    char a[10] = {'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A'};
    char b[10] = {'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B'};
    vector<char *> records;
    //records.push_back(a);
    vector<string> field_names;
    field_names.push_back("field1");
    field_names.push_back("field2");
    field_names.push_back("field3");
    vector<unsigned int> sizes;
    sizes.push_back(3);
    sizes.push_back(4);
    sizes.push_back(3);
    vector<int> types;
    types.push_back(TK_STRING);
    types.push_back(TK_STRING);
    types.push_back(TK_STRING);

    vector<bool> indices;
    indices.push_back(false);
    indices.push_back(false);
    indices.push_back(false);
    NotsqlTable table1(records, field_names, sizes, types, indices, table_name1);
    table1.appendRecord(a);
    table1.appendRecord(b);
    NotsqlTable table2(records, field_names, sizes, types, indices, table_name2);
    table2.appendRecord(a);
    table2.appendRecord(b);
    NotsqlTable table3 = NotsqlTable::product(table1, table2);
    vector<char *> res = table3.getRecord(2);
    vector<string> names = table3.getFieldNames();
    cout << table3.getRecordsNum() << endl;
    cout << res[0][0] << "     " << res[3][0] << endl;
    return 0;
}
