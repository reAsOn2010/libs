#ifndef _NOTSQL_TABLE_H_
#define _NOTSQL_TABLE_H_

#include <map>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iostream>
#include "NotsqlConstants.h"
#include "NotsqlExceptions.h"
using namespace std;

/**
 * A wrapped table class
 * +---------+---------+-------- +---------+
 * | Field 0 | Field 1 | Field 2 |   ...   |
 * +---------+---------+-------- +---------+
 * |         |         |         |         |
 * | Value 0 | Value 1 | Value 2 |   ...   |
 * |         |         |         |         |
 * |   ...   |   ...   |   ...   |   ...   |
 * +---------+---------+---------+---------+
 *
 */


class NotsqlTable{

public:
    /**
     * NOTE
     * 
     * The table must be initialize with all the infomation below
     *
     */
    NotsqlTable(
            vector<char *> & records,
            vector<string> & field_names,
            vector<unsigned int> & sizes,
            vector<TK_TYPE> & types,
            vector<bool> & indices,
            const string & table_name
    );

    NotsqlTable(const NotsqlTable & table);

#ifdef NOTSQL_DEBUG
    NotsqlTable(){};
#endif


    /*
    static NotsqlTable andTable(NotsqlTable & table, NotsqlTable table2);
    */


    /** 
     * Append a new field
     * 
     * field_name: The new field name
     * sz: The size of data in the field
     * value: Some existed data
     * type: The type integer
     */
    void appendField(
            const string & field_name, 
            unsigned int sz, 
            vector<char *> value, 
            int type,
            bool has_index
    );


    /**
     * Append a new record
     *
     * record:  A new, consecutive record
     */
    void appendRecord(char * record);


    void appendRecord(vector<char *> & record);


    void appendFieldData(
            const string & field_name,
            char * value);



    /**
     * Expand the table.
     * Add the new fields in the argument table.
     * If the field is existed,
     * name it like table_a.field_a
     */
    void extends(const NotsqlTable & table);

    /**
     * Delete the specific record
     * if the record is not found
     * nothing happens
     */
    void removeRecord(const char * record);

    /**
     * Delete the specific record
     */
    void removeRecord(unsigned int record_num);


    /**
     * Get the specific field data
     */
    vector<char *> & getFieldData(const string & field_name);

    /**
     * Get the index of the field in the field_names vector
     */
    unsigned int getFieldIndex(const string & field_name) const;

    vector<string> getFieldNames() const{
        return field_names;
    }

    /**
     * Get the field's start position(the offset) in one record
     */
    unsigned int getFieldPos(const string & field_name) const;

     
    /**
     * Get the field's size
     */
    unsigned int getFieldSize(const string & field_name) const;


    TK_TYPE getFieldType(const string & field_name) const;

    vector< vector<char *> > & getRecords(){
        return values;
    }
    /**
     * Get the record size
     */
    unsigned int getRecordSize() const;

    vector<char *> getRecord(unsigned int record_index);
    /*
     * Get the number of records
     */
    unsigned int getRecordsNum() const{
        // Has no field
        if(!field_names.size()){
            return false;
        }else{
            return values[0].size();
        }
    }

    /**
     * Check whether the field is in the table
     */
    bool hasField(const string & field_name) const{
        try{
            getFieldIndex(field_name);
        }catch(NotsqlFieldNotFoundException & e){
            return false;
        }
    }


    /**
     * NOTE
     *
     * This function is valid unless the field has not been modified!!
     *
     * AND
     *
     * if you want to delete a record,
     * please do delRecord directly!
     * Cause this method will take one more search
     *
     */
    bool hasRecord(const char * record) const;

    bool hasIndex(const string & field_name) const{
        int index;
        try{
            index = getFieldIndex(field_name);
        }catch(NotsqlFieldNotFoundException & e){
            return false;
        }
        return indices[index];
    }


    /**
     * Do cartesian product
     */
    static NotsqlTable product(
            NotsqlTable table, NotsqlTable table2);

    /**
     * The project logical operation
     */
    void project(vector<string> & proj_field_names);

    /**
     * Remove the specific field 
     * 
     * field_name:  name of the field to remove
     */
    void removeField(const string & field_name);

    /**
     * Rename the field
     */
    void renameField(const string & field_name, const string & new_name);

    void showTable();

    void clear(){
        values.clear();
    }

    void reset(){
        values.clear();
        field_names.clear();
        sizes.clear();
        types.clear();
        indices.clear();
    }

    static pair<string, string> splitFieldName(const string & field_name) {
        int pos = field_name.find(".");
        string table = field_name.substr(0, pos);
        string field = field_name.substr(pos+1);
        pair<string, string> ret (table, field);
        return ret;
    }

    vector< vector<string> > getStringValues();


private:

    vector< vector<char *> > values;
    vector<string> field_names;
    vector<unsigned int> sizes;
    vector<int> types;
    vector<bool> indices;
}; 
#endif
