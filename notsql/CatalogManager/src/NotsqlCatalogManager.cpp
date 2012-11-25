#include "NotsqlCatalogManager.h"
#include "NotsqlExceptions.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

//initialize static variable
int NotsqlCatalogManager::DBNum = 0;
vector<string> NotsqlCatalogManager::DBSet;

//Load & Save DBSet
void NotsqlCatalogManager::loadDBSet(const string & PREFIX){
	int i;
	int len;
	char * temp = new char [80];
	string  newDB;
	ifstream fin((PREFIX + "DBSet.catalog").c_str());
	if(!fin.is_open()){
		ofstream fout((PREFIX + "DBSet.catalog").c_str());
		fout.close();
	}
	else{
		fin.read((char *)&DBNum, 4);
		for(i = 0; i < DBNum; i++){
			fin.read((char *)&len, 4);
			fin.read(temp, len+1);
			newDB = temp;
			DBSet.push_back(newDB);
		}
		fin.close();
	}
}

void NotsqlCatalogManager::saveDBSet(const string & PREFIX){
	int i;
	int len;
	ofstream fout((PREFIX + "DBSet.catalog").c_str());
	fout.write((char *)&DBNum, 4);
	for(i = 0; i < DBNum; i++){
		len = DBSet[i].size();
		fout.write((char *)&len, 4);
		fout.write(DBSet[i].c_str(), len+1);
	}
	fout.close();
}

//Database Operation
bool NotsqlCatalogManager::DatabaseIsExist(string DBName, const string & PREFIX) 
    throw(CatalogDatabaseNotHaveError){
	fstream DBFile((PREFIX + DBName).c_str(), ios_base::in);
	if(!DBFile.is_open()){
		return 0;
	}
	return 1;
}


void NotsqlCatalogManager::CreateDatabase(string DBName, const string & PREFIX) 
        throw(CatalogDatabaseDuplicateError){
	fstream DBFile((PREFIX + DBName).c_str(), ios_base::in);
	if(DBFile.is_open()){
		throw CatalogDatabaseDuplicateError();
	}
	else{
		mkdir((PREFIX + DBName).c_str(),  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);		
		DBNum++;
		DBSet.push_back(DBName);
	}
}

void NotsqlCatalogManager::DropDatabase(string DBName, const string & PREFIX) 
    throw(CatalogDatabaseNotHaveError){
	int i;
	fstream DBFile((PREFIX + DBName).c_str(), ios_base::in);
	if(!DBFile.is_open()){
		throw CatalogDatabaseNotHaveError();
	}
	else{
		system(("rm -r " + PREFIX + DBName + " >> " + PREFIX + "notsql.log").c_str());
		DBNum--;
		for(i = 0; i < DBNum; i++){
			if(DBName == DBSet[i]){
				break;
			}
		}
		DBSet.erase(DBSet.begin()+i);
	}
}

//Save & Load
void NotsqlCatalogManager::load(){
	int i;
	ifstream fin((PREFIX +DBNow+"/"+DBNow+".catalog").c_str());
	if(!fin.is_open()){
		ofstream fout((PREFIX +DBNow+"/"+DBNow+".catalog").c_str());
		fout.close();
	}
	else{
		fin.read((char *)&TableNum, 4);
		for(i = 0; i < TableNum; i++){
			CatalogTable newTable(PREFIX);
			newTable.load(fin);
			TableList.push_back(newTable);
		}
		fin.close();
	}
}

void NotsqlCatalogManager::save(){
	int i;
	ofstream fout((PREFIX + DBNow+"/"+DBNow+".catalog").c_str());
	fout.write((char *)&TableNum, 4);
	for(i = 0; i < TableNum; i++){
		TableList[i].save(fout);
	}
	fout.close();
}

//Table Operation
void NotsqlCatalogManager::CreateTable(
        string TableName, 
        vector<string> AttrName, 
        vector<bool> AttrIsPrimary, 
        vector<int> AttrType, 
        vector<int> AttrLength,
		vector<string> AttrIndex) throw(
            CatalogTableDuplicateError, 
            CatalogAttrDuplicateError, 
            CatalogAttrTypeNameError){
	unsigned int i;
	if(TableIsExist(TableName)){
		throw CatalogTableDuplicateError(); 
	}
	else{
		CatalogTable newTable(TableName, PREFIX);
		TableNum++;
		try{
			for(i = 0; i < AttrName.size(); i++){
                if(AttrIsPrimary[i]){
                    ofstream fout((PREFIX + "data/" + DBNow + "/" + 
                            AttrName[i] + ".index").c_str());
                    fout.close();
                    AttrIndex[i] = AttrName[i];
                }
				newTable.AddAttr(
                        DBNow, 
                        AttrName[i], 
                        AttrIsPrimary[i], 
                        AttrType[i], 
                        AttrLength[i], 
                        AttrIndex[i]
                        );
			}
            ofstream table_file;
            table_file.open((PREFIX + DBNow + "/" + TableName).c_str());
            table_file.close();
		}
		catch(CatalogAttrDuplicateError & ctd){
			throw;
		}
		catch(CatalogAttrTypeNameError & catn){
			throw;
		}
		TableList.push_back(newTable);
	}
}

void NotsqlCatalogManager::DropTable(string TableName) throw(CatalogTableNotHaveError){
	if(!TableIsExist(TableName)){
		throw CatalogTableNotHaveError();
	}
	else{
		system(("rm -f " + PREFIX + DBNow+"/"+TableName+"* >> " + 
                    PREFIX + "notsql.log").c_str());
		TableList.erase(TableList.begin() + TablePosition(TableName));
		TableNum--;
	}
}
//Create & Drop Index
void NotsqlCatalogManager::CreateIndex(string TableName, string AttrName, string IndexName) 
	throw(CatalogTableNotHaveError, CatalogAttrNotHaveError, CatalogIndexDuplicateError){
	if(!TableIsExist(TableName)){
		throw CatalogTableNotHaveError();
	}
	else{
		try{
		    TableList[TablePosition(TableName)].AddIndex(DBNow, AttrName, IndexName);
		}
		catch(NotsqlBaseException & e){
			throw;
		}
	}	
}

void NotsqlCatalogManager::DropIndex(string TableName, string AttrName) throw(CatalogTableNotHaveError, 
		CatalogAttrNotHaveError, CatalogIndexNotHaveError){
	if(!TableIsExist(TableName)){
		throw CatalogTableNotHaveError();
	}
	else{
		try{
			TableList[TablePosition(TableName)].DeleteIndex(DBNow, AttrName);
		}
		catch(NotsqlBaseException & e){
			throw;
		}
	}
}

//Verified Function
bool NotsqlCatalogManager::TableIsExist(string TableName){
	if(TablePosition(TableName) >= 0){
		return 1;
	}
	else{
		return 0;
	}
}

//check if the attribute is exist
bool NotsqlCatalogManager::AttrNameIsExist(string TableName, string AttrName){
	if(TableList[TablePosition(TableName)].AttrIsExist(AttrName)){
		return true;
	}
	else{
		return false;
	}
}

//check if the value match attribute's type
bool NotsqlCatalogManager::AttrValueIsValid(string TableName, string AttrName, string AttrValue){
	if(TableList[TablePosition(TableName)].AttrTypeValueValid(AttrName, AttrValue)){
		return true;
	}
	else{
		return false;
	}
}

//check if the attribute is primary
bool NotsqlCatalogManager::AttrIsPrimary(string TableName, string AttrName){
	if(TableList[TablePosition(TableName)].AttrIsPrimary(AttrName)){
		return true;
	}
	else{
		return false;
	}
	
}

//check if the attribute has index
bool NotsqlCatalogManager::AttrHasIndex(string TableName, string AttrName){
	if(TableList[TablePosition(TableName)].AttrHasIndex(AttrName)){
		return true;
	}
	else{
		return false;
	}
}

//Return Parameters
int NotsqlCatalogManager::TablePosition(string TableName){
	unsigned int i;
	for(i = 0; i < TableList.size(); i++){
		if(TableList[i].TableName == TableName){
			return i;
		}
	}
	return -1;
}

//Database Info
inline vector<string> NotsqlCatalogManager::ReturnDBList(){
	return DBSet;
}

inline int NotsqlCatalogManager::ReturnDBNum(){
	return DBNum;
}

//Table Info
inline vector<CatalogTable> NotsqlCatalogManager::ReturnTableList(){
	return TableList;
}


int NotsqlCatalogManager::ReturnTableSize(string TableName){
	int i;
	int size = 0; 
	CatalogTable TB = TableList[TablePosition(TableName)];
	for(i = 0; i < TB.AttrNum; i++){
		size += TB.AttrList[i].AttrLength;
	}
	return size;
}

int NotsqlCatalogManager::ReturnTableSizeBefore(string TableName, string AttrName){
	int i;
	int size;
	CatalogTable TB = TableList[TablePosition(TableName)];
	if(TB.AttrPosition(AttrName) == 0){
		size = 0;
	}
	else{
		for(i = 0; i < TB.AttrPosition(AttrName); i++){
			size += TB.AttrList[i].AttrLength;
		}
	}
	return size;
}

//Attribute Info
vector<CatalogAttr> NotsqlCatalogManager::ReturnAttrList(string TableName){
	return TableList[TablePosition(TableName)].AttrList;
}

int NotsqlCatalogManager::ReturnAttrNum(string TableName){
	return TableList[TablePosition(TableName)].AttrNum;
}

int NotsqlCatalogManager::ReturnAttrLength(string TableName, string AttrName){
	CatalogTable TB = TableList[TablePosition(TableName)];
	return TB.AttrList[TB.AttrPosition(AttrName)].AttrLength;
}

int NotsqlCatalogManager::ReturnAttrType(string TableName, string AttrName){
    CatalogTable TB = TableList[TablePosition(TableName)];
    return TB.AttrList[TB.AttrPosition(AttrName)].AttrType;
}
