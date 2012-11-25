#include "NotsqlTable.h"
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;
NotsqlTable::NotsqlTable( 
        vector<char* > & records, 
        vector<string> & field_names, 
        vector<unsigned int> & sizes,
        vector<int> & types,
        vector<bool> & indices,
        const string & table_name){ 
    this->field_names = field_names;
    for(unsigned int i = 0; i != this->field_names.size(); ++i){
        this->field_names[i] = table_name + "." + this->field_names[i];
    }
    this->sizes = sizes;
    this->types = types;
    this->indices = indices;
    unsigned int total_size = 0;
    for(unsigned int i = 0; i != sizes.size(); ++i){
        vector<char *> column;
        for(unsigned int j = 0; j != records.size(); ++j){
            column.push_back(records[j] + total_size);
        }
        values.push_back(column);
        total_size += sizes[i];
    }
}

NotsqlTable::NotsqlTable(const NotsqlTable & table){
    this->values = table.values;
    this->field_names = table.field_names;
    this->sizes = table.sizes;
    this->types = table.types;
    this->indices = table.indices;
}


vector<char *> & NotsqlTable::getFieldData(
        const string & field_name){
    unsigned int index;
    try{
        index = getFieldIndex(field_name);
    }catch(...){
        throw;
    }
    return values[index];
}


void NotsqlTable::appendFieldData(
        const string & field_name,
        char * value){
    unsigned int index;
    try{
        index = getFieldIndex(field_name);
    }catch(...){
        throw;
    }
    values[index].push_back(value);
}


void NotsqlTable::appendField(
        const string & field_name, 
        unsigned int sz, 
        vector<char *> value,
        int type,
        bool has_index
        ){
    field_names.push_back(field_name);
    sizes.push_back(sz);
    values.push_back(value);
    types.push_back(type);
    indices.push_back(has_index);
}

void NotsqlTable::removeField(const string & field_name){
    int index;
    try{
        index = getFieldIndex(field_name);
    }catch(...){
        throw;
    }
    field_names.erase(field_names.begin() + index);
    values.erase(values.begin() + index);
    sizes.erase(sizes.begin() + index);
    types.erase(types.begin() + index);
    indices.erase(indices.begin() + index);
}


void NotsqlTable::project(vector<string> & proj_field_names){
    for(unsigned int i = 0; i != field_names.size(); ++i){
        if(find(proj_field_names.begin(), proj_field_names.end(), field_names[i]) 
                == proj_field_names.end()){
            field_names.erase(field_names.begin() + i);
            values.erase(values.begin() + i);
            sizes.erase(sizes.begin() + i);
            types.erase(types.begin() + i);
            indices.erase(indices.begin() + i);
            i--;
        }
    }
}


void NotsqlTable::appendRecord(char * record){
    unsigned int size = 0;
    for(unsigned int i = 0; i != values.size(); ++i){
        values[i].push_back(record + size);
        size += sizes[i];
    }
}



unsigned int NotsqlTable::getFieldIndex(const string & field_name) const{
    vector<string>::const_iterator it = find(
            field_names.begin(), field_names.end(), field_name);
    if(it == field_names.end())
        throw(NotsqlFieldNotFoundException(field_name));
    return it - field_names.begin();
}


unsigned int NotsqlTable::getFieldPos(const string & field_name) const{
    unsigned int index;
    try{
        index = getFieldIndex(field_name);
    }catch(...){
        throw;
    }
    unsigned int pos = 0;
    for(unsigned int i = 0; i != index; ++i){
        pos += sizes[i];
    }
    return pos;
}

unsigned int NotsqlTable::getFieldSize(const string & field_name) const{
    unsigned int index;
    try{
        index = getFieldIndex(field_name);
    }catch(...){
        throw;
    }
    return sizes[index];
}

int NotsqlTable::getFieldType(const string & field_name) const{
    unsigned int index;
    try{
        index = getFieldIndex(field_name);
    }catch(...){
        throw;
    }
    return types[index];
}


void NotsqlTable::removeRecord(const char * record){
    for(unsigned int i = 0; i != values[0].size(); ++i){
        if(values[0][i] == record){
            for(unsigned int j = 0; j != values.size(); ++j){
                values[j].erase(values[j].begin() + i);
            }
        }
    }
}

void NotsqlTable::removeRecord(unsigned int record_num){
    if(record_num > getRecordsNum()){
        throw(NotsqlIndexOutOfBoundException());
    }
    for(unsigned int i = 0; i != values.size(); ++i){
        values[i].erase(values[i].begin() + record_num);
    }
}

bool NotsqlTable::hasRecord(const char * record) const{
    for(unsigned int i = 0; i != values[0].size(); ++i){
        if(values[0][i] == record){
            return true;
        }
    }
    return false;
}


unsigned int NotsqlTable::getRecordSize() const{
    unsigned int total_size;
    total_size = 0;
    for(unsigned int i = 0; i != sizes.size(); ++i){
        total_size += sizes[i];
    }
    return total_size;
}


void NotsqlTable::extends(const NotsqlTable & table){
    vector<string> ex_field_names = table.field_names;
    vector<unsigned int> ex_sizes = table.sizes;
    vector<TK_TYPE> ex_types = table.types;
    vector<bool> ex_indices = table.indices;
    for(unsigned int i = 0; i != ex_field_names.size(); ++i){
        vector<string>::iterator it = find(
                field_names.begin(),
                field_names.end(),
                ex_field_names[i]);

        if(it == field_names.end()){
            field_names.push_back(ex_field_names[i]);
            sizes.push_back(ex_sizes[i]);
            types.push_back(ex_types[i]);
            indices.push_back(ex_indices[i]);
            values.push_back(vector<char *>());
        }
    }
}


NotsqlTable NotsqlTable::product(
        NotsqlTable table, NotsqlTable table2){
    NotsqlTable res_table;

    if(!table.field_names.size()){
        return table2;
    }else if(!table2.field_names.size()){
        return table;
    }

    res_table.extends(table);
    res_table.extends(table2);

    for(unsigned int i = 0; i != table.getRecordsNum(); ++i){
        vector<char *> table1_record = table.getRecord(i);
        for(unsigned int j = 0; j != table2.getRecordsNum(); ++j){
            vector<char *> new_record = table1_record;
            vector<char *> table2_record = table2.getRecord(j);
            for(unsigned int ii = 0; ii != table2_record.size(); ++ii){
                new_record.push_back(table2_record[ii]);
            }
            res_table.appendRecord(new_record);
        }
    }
    return res_table;
}


vector<char *> NotsqlTable::getRecord(unsigned int record_index){
    if(record_index >= getRecordsNum()){
        throw NotsqlIndexOutOfBoundException();
    }
    vector<char *> res;
    for(unsigned int i = 0; i != values.size(); ++i){
        res.push_back(values[i][record_index]);
    }
    return res;
}


void NotsqlTable::appendRecord(vector<char *> & record){
    if(record.size() != field_names.size()){
        throw(NotsqlLengthTooShortException());
    }
    for(unsigned int i = 0; i != record.size(); ++i){
        values[i].push_back(record[i]);
    }
}


/*

NotsqlTable NotsqlTable::andTable(
        NotsqlTable & table, NotsqlTable & table2){
    vector<string> field_names1 = table.getFieldNames();
    vector<string> field_names2 = table2.getFieldNames();
    for(unsigned int field_index1 = 0; 
            field_index1 != field_names1.size(); ++i){
        vector<string>::iterator it = find(
                field_names2.begin(), 
                field_names2.end(),
                field_names1[field_index1]);

        unsigned int field_index2 = it - field_names2.begin();
        if(it != field_names2.end()){
            unsigned int ii, jj;
            for(ii = 0; ii != table.getRecordsNum(); ++ii){
                for(jj = 0; jj != field_names2.getRecordsNum(); ++jj){
                    if(table.values[field_index1][ii] == 
                            table.values[field_index2][jj]){
                        break;
                    }
                }
                // Remove the two records
                if(jj == field_names2.getRecordsNum()){
                    table.removeRecord(ii);
                    table2.removeRecord(jj);
                    ii--;
                }
            }
        }
    }
}
*/

void NotsqlTable::showTable(){
    for(unsigned int i = 0; i != field_names.size(); ++i){
        cout << field_names[i] << "\t|\t";
    }
    cout << endl;
    for(unsigned int j = 0; j != getRecordsNum(); ++j){
        for(unsigned int i = 0; i != values.size(); ++i){
            TK_TYPE type = getFieldType(field_names[i]);
            if(type == TK_STRING){
                cout << string(values[i][j]) << "\t|\t";
            }else if(type == TK_INTEGER){
                cout << *(int *)values[i][j] << "\t|\t";
            }else if(type == TK_FLOAT){
                cout << *(float *)values[i][j] << "\t|\t";
            }
        }
        cout << endl;
    }
}

vector< vector<string> > NotsqlTable::getStringValues(){
    vector< vector<string> > res;
    for(unsigned int i = 0; i != field_names.size(); ++i){
        res.push_back(vector<string>());
        string value;
        stringstream ss;
        if(getFieldType(field_names[i]) == TK_INTEGER){
            for(unsigned int ii = 0; ii != getRecordsNum(); ++ii){
                ss << *((int *)values[i][ii]);
                ss >> value;
                res[i].push_back(value);
            }
        }else if(getFieldType(field_names[i]) == TK_FLOAT){
            for(unsigned int ii = 0; ii != getRecordsNum(); ++ii){
                ss << *((float *)values[i][ii]);
                ss >> value;
                res[i].push_back(value);
            }
        }else if(getFieldType(field_names[i]) == TK_CHAR){
            for(unsigned int ii = 0; ii != getRecordsNum(); ++ii){
                value = string(values[i][ii]);
                res[i].push_back(value);
            }
        }
    }
    return res;
}
