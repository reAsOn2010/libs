#ifndef _NOTSQL_CATALOGMANAGER_H_
#define _NOTSQL_CATALOGMANAGER_H_

#include "NotsqlExceptions.h"
#include "NotsqlCatalogDatabase.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

using namespace std;

class NotsqlCatalogManager{
private: string DBNow;
	int TableNum;
	vector<CatalogTable> TableList;
	static vector<string> DBSet;
	static int DBNum;
    string PREFIX;

	int TablePosition(string TableName);
	string ReturnDBNow();
public:
	//constructor
	NotsqlCatalogManager(const string & DB, const string & PREFIX){
		loadDBSet(PREFIX);
		DBNow = DB;
        this->PREFIX = PREFIX;
		TableNum = 0;
		load();
	}
	//destructor
	virtual ~NotsqlCatalogManager(){
		saveDBSet(PREFIX);
		save();
	}
	
	//function
	//Database Operation(static)
	static void loadDBSet(const string & PREFIX);
	static void saveDBSet(const string & PREFIX);
	static bool DatabaseIsExist(string DBName, const string & PREFIX) throw(CatalogDatabaseNotHaveError); 
	static void CreateDatabase(string DBName, const string & PREFIX) throw(CatalogDatabaseDuplicateError);
	static void DropDatabase(string DBName, const string & PREFIX) throw(CatalogDatabaseNotHaveError);
	
	//Load & Save Catalog Data
	void load();
	void save();

	//Table Operation
	//create and drop
	void CreateTable(
            string TableName, 
            vector<string> AttrName, 
            vector<bool> AttrIsPrimary, 
            vector<int> AttrType, 
            vector<int> AttrLength,
		    vector<string> AttrIndex) throw( 
                CatalogTableDuplicateError, 
                CatalogAttrDuplicateError, 
                CatalogAttrTypeNameError);

	void DropTable(string TableName) throw(
            CatalogTableNotHaveError);
	//index operation
	void CreateIndex(
            string TableName, 
            string AttrName, 
            string IndexName) throw(
                CatalogTableNotHaveError, 
                CatalogAttrNotHaveError, 
                CatalogIndexDuplicateError);
	void DropIndex(string TableName, string IndexName) throw(
            CatalogTableNotHaveError, 
            CatalogAttrNotHaveError, 
            CatalogIndexNotHaveError);

	//verified function
	//check if the table is exist
	bool TableIsExist(string TableName);

	//check if the attribute is exist
	bool AttrNameIsExist(string TableName, string AttrName);
	//check if the value match attribute's type
	bool AttrValueIsValid(string TableName, string AttrName, string AttrValue);
	//check if the attribute is primary
	bool AttrIsPrimary(string TableName, string AttrName);
	//check if the attribute has index
	bool AttrHasIndex(string TableName, string AttrName);
	 
	//return parameters
	//Database Info
	static vector<string> ReturnDBList();
	static int ReturnDBNum();
	//Table Info
	vector<CatalogTable> ReturnTableList();
	int ReturnTableNum(){
        return TableNum;
    }

	int ReturnTableSize(string TableName);
	int ReturnTableSizeBefore(string TableName, string AttrName);
	//Attribute Info
	vector<CatalogAttr> ReturnAttrList(string TableName);
	int ReturnAttrNum(string TableName);
	int ReturnAttrLength(string TableName, string AttrName);
	int ReturnAttrType(string TableName, string AttrName);
};


#endif
