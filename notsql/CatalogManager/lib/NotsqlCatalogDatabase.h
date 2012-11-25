#ifndef CATALOGDATABASE_H_
#define  CATALOGDATABASE_H_     

#include "NotsqlExceptions.h"
#include "NotsqlConstants.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <regex.h>
#include <stdlib.h>

using namespace std;

struct CatalogAttr{
	string AttrName;
	bool AttrIsPrimary;
	int AttrType;
	int AttrLength;
	string AttrIndex;
};

class CatalogTable{
public:
	string TableName;
	int AttrNum;
	vector<struct CatalogAttr> AttrList;
    string PREFIX;
public:
	//constructor
	CatalogTable(const string & PREFIX) : PREFIX(PREFIX){}
	CatalogTable(string TBName, const string & PREFIX){
		TableName = TBName;
		AttrNum = 0;	
        this->PREFIX = PREFIX;
	}
	//destructor
	virtual ~CatalogTable(){
	}
	
	
	//function	
	//load and save
	void load(ifstream & fin);
	void save(ofstream & fout);
	
	//return total table attribute number
	int TableAttrNum();	
	//add attribute
	//empty string for no index
	void AddAttr(string DBName, string AttrName, bool AttrIsPrimary, int AttrType, int AttrLentgh = 80,
			string AttrIndex = "") throw(CatalogAttrDuplicateError, CatalogAttrTypeNameError);
	void DeleteAttr(string DBName, string AttrName) throw(CatalogAttrNotHaveError);
	
	void AddIndex(string DBName, string AttrName, string IndexName) throw(CatalogIndexDuplicateError, CatalogAttrNotHaveError);
	void DeleteIndex(string DBName, string IndexName) throw(CatalogAttrNotHaveError, CatalogIndexNotHaveError);
	
	string AttrIndexName(string AttrName);
	bool AttrTypeNameValid(int AttrType);
	int AttrPosition(string AttrName);
	bool AttrIsExist(string AttrName);
	bool AttrTypeValueValid(string AttrName, string AttrValue);
	bool AttrIsPrimary(string AttrName);
	bool AttrHasIndex(string AttrName);
	int AttrType(string AttrName);
	int AttrLength(string AttrName);
	
};

#endif
