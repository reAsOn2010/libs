#include "NotsqlCatalogDatabase.h"

//Class CatalogTable Function

//load and save
void CatalogTable::load(ifstream & fin){
	int i;
	int len;
	char * temp;
	temp = new char [80];
	fin.read((char *)&len, 4);
	fin.read(temp, len+1);
	TableName = temp;
	fin.read((char *)&AttrNum, 4);
	for(i = 0; i < AttrNum; i++){
		struct CatalogAttr newAttr;
		//read attribute name
		fin.read((char *)&len, 4);
		fin.read(temp, len+1);
		newAttr.AttrName = temp;
		//read AttrIsPrimary
		fin.read((char *)&newAttr.AttrIsPrimary, 1);
		//read attribute type
		fin.read((char *)&newAttr.AttrType, 4);
		//read attribute length
		fin.read((char *)&newAttr.AttrLength, 4);
		//read attribute index name
		fin.read((char *)&len, 4);
		fin.read(temp, len+1);
		newAttr.AttrIndex = temp;
		//push
		AttrList.push_back(newAttr);
	}
	delete [] temp;
}

void CatalogTable::save(ofstream & fout){
	int i;
	int len;
	len = TableName.size();
	fout.write((char *)&len, 4);
	fout.write(TableName.c_str(), len+1);
	fout.write((char *)&AttrNum, 4);
	for(i = 0; i < AttrNum; i++){
		//write attribute name
		len = AttrList[i].AttrName.size();
		fout.write((char *)&len, 4);
		fout.write(AttrList[i].AttrName.c_str(), len+1);
		//write AttrIsPrimary
		fout.write((char *)&AttrList[i].AttrIsPrimary, 1);
		//write attribute type
		fout.write((char *)&AttrList[i].AttrType, 4);
		//write attribute length
		fout.write((char *)&AttrList[i].AttrLength, 4);
		//write attribute index name
		len = AttrList[i].AttrIndex.size();
		fout.write((char *)&len, 4);
		fout.write(AttrList[i].AttrIndex.c_str(), len+1);
	}
}

int CatalogTable::TableAttrNum(){
	return AttrNum;
}

bool CatalogTable::AttrTypeValueValid(string AttrName, string AttrValue){
	regex_t reg;
	regmatch_t match[1];
	char patt_int[] = "[0-9]|[1-9][0-9]+";
	char patt_float[] = "([0-9]|[1-9][0-9]+)|([0-9]|[1-9][0-9]+.[0-9]*)";
	//char patt_double[] = "([0-9]|[1-9][0-9]+)|([0-9]|[1-9][0-9]+.[0-9]*)";
	int AttrType = this->AttrType(AttrName);
	switch(AttrType){ 
		case TK_CHAR: return 1;
		case TK_INTEGER: regcomp(&reg, patt_int, 0);
				if(regexec(&reg, AttrValue.c_str(), 1, match, 0)){
					regfree(&reg);
					return 1;
				}
				regfree(&reg);
				return 0;
		case TK_FLOAT: regcomp(&reg, patt_float, 0);
				if(regexec(&reg, AttrValue.c_str(), 1, match, 0)){
					regfree(&reg);
					return 1;
				}
				regfree(&reg);
				return 0;
/*
		case TK_DOUBLE: regcomp(&reg, patt_double, 0);
				if(regexec(&reg, AttrValue.c_str(), 1, match, 0)){
					regfree(&reg);
					return 1;
				}
				regfree(&reg);
				return 0;
*/
		default: return 0;
	}
}

bool CatalogTable::AttrIsPrimary(string AttrName){
	return AttrList[AttrPosition(AttrName)].AttrIsPrimary;
}

bool CatalogTable::AttrIsExist(string AttrName){
	unsigned int i;
	bool flag = false;
	if(!AttrList.empty()){    //the table has attributes					
		for(i = 0; i < AttrList.size(); i++){
			if(AttrList[i].AttrName == AttrName){
				flag = true;
				break;
			}
		}
		if(flag){    //AttrName duplicated
			return true;
		}
		else{
			return false;
		}
	}
	else{
		return false;
	}
}

int CatalogTable::AttrPosition(string AttrName){
	unsigned int i;
	if(!AttrList.empty()){    //the table has attributes					
		for(i = 0; i < AttrList.size(); i++){
			if(AttrList[i].AttrName == AttrName){
				return i;
			}
		}
		return -1;		
	}
	else{
		return -1;
	}
}

bool CatalogTable::AttrTypeNameValid(int AttrType){
	int i;
	bool flag = false;
	int TYPE[4] = {TK_CHAR, TK_INTEGER, TK_FLOAT, TK_DOUBLE};
	for(i = 0; i < 4; i++){
		if(TYPE[i] == AttrType){
			flag = true;
		}
	}
	if(flag){
		return true;
	}
	else{
		return false;
	}
}

bool CatalogTable::AttrHasIndex(string AttrName){
	if(AttrList[AttrPosition(AttrName)].AttrIndex != ""){
		return true;
	}
	else{
		return false;
	}
}

string CatalogTable::AttrIndexName(string AttrName){
	return AttrList[AttrPosition(AttrName)].AttrIndex;
}

void CatalogTable::AddIndex(string DBName, string AttrName, string IndexName) throw(CatalogIndexDuplicateError, CatalogAttrNotHaveError){
	if(!AttrIsExist(AttrName)){
		throw CatalogAttrNotHaveError();
	}
	else{
		if(AttrHasIndex(AttrName)){
			throw CatalogIndexDuplicateError();
		}
		else{
			AttrList[AttrPosition(AttrName)].AttrIndex = IndexName;
			ofstream fout((PREFIX+"data/"+DBName+"/"+TableName+"_"+IndexName+".index").c_str()); 
		}
	}
}

void CatalogTable::DeleteIndex(string DBName, string IndexName) throw(CatalogIndexNotHaveError, CatalogAttrNotHaveError){
    unsigned int i = 0;
    for(i = 0; i != AttrNum; ++i){
        if(AttrList[i].AttrIndex == IndexName){
            system(("rm " + PREFIX + DBName+"/"+TableName+"_"+
                        IndexName+".index >> " + PREFIX + "notsql.log").c_str());
        }
        break;
    }
    if(i == AttrNum){
        throw CatalogIndexNotHaveError();
    }
}

void CatalogTable::AddAttr(string DBName, string AttrName, bool AttrIsPrimary, int AttrType, int AttrLength, 
        string AttrIndex) throw(CatalogAttrDuplicateError, CatalogAttrTypeNameError){
    ofstream fout;


    struct CatalogAttr newAttr;
    newAttr.AttrName = AttrName;
    newAttr.AttrIsPrimary = AttrIsPrimary;
    newAttr.AttrType = AttrType;
    newAttr.AttrLength = AttrLength;
    newAttr.AttrIndex = AttrIndex;

    if(AttrTypeNameValid(AttrType)){    //attribute type name valid
        if(AttrIsExist(AttrName)){    //attribute name duplicated
            throw CatalogAttrDuplicateError();
        }
        else{
            AttrList.push_back(newAttr);    //add attr struct into catalog
            AttrNum++;
            if(AttrIndex != ""){    //create index file
                fout.open((PREFIX + DBName+"/"+TableName+"_"+AttrIndex+".index").c_str());
            }
        }
    }
    else{
        throw CatalogAttrTypeNameError();
    }
}

void CatalogTable::DeleteAttr(string DBName, string AttrName) throw(CatalogAttrNotHaveError){
    if(AttrIsExist(AttrName)){    //the table has attributes
        if(AttrHasIndex(AttrName)){    //remove index file
            system(("rm " + PREFIX + DBName+"/"+TableName+"_"+
                        AttrIndexName(AttrName)+".index >> " + PREFIX + "notsql.log").c_str());
            system(("del " + PREFIX + DBName+"/"+TableName+"_"+
                        AttrIndexName(AttrName)+".index >> " + PREFIX + "notsql.log").c_str());
        }
        AttrList.erase(AttrList.begin() + AttrPosition(AttrName));  //remove from catalog
        AttrNum--;
    }
    else{
        throw CatalogAttrNotHaveError();
    }
}

int CatalogTable::AttrType(string AttrName){
    return AttrList[AttrPosition(AttrName)].AttrType;
}

int CatalogTable::AttrLength(string AttrName){
    return AttrList[AttrPosition(AttrName)].AttrLength;
}

