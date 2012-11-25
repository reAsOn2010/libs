#ifndef _NOTSQL_EXCEPTIONS_H_
#define _NOTSQL_EXCEPTIONS_H_
#include <iostream>
#include <string>
using namespace std;


class NotsqlBaseException{
public:
    virtual ~NotsqlBaseException(){};

    virtual void showMsg() = 0;
    virtual string getMsg() = 0;
};

class NotsqlQuitException{
public:
    void showMsg(){
        cout << "Quit" << endl;
    }
    string getMsg(){
        return "Quit";
    }
    
};


/**
 * Exceptions in Interpreter
 */

class NotsqlDBNotExistException : public NotsqlBaseException{
public:
    NotsqlDBNotExistException(
            const string & db_name,
            const string & msg=""){
        this->db_name = db_name;
        _msg = "No database named \"" + db_name + "\".\n" + msg;
    }
    void showMsg(){
        cerr << _msg << endl;
    }

    string getMsg(){
        return _msg;
    }

private:
    string _msg;
    string db_name;
};

class NotsqlNoDBSelectedException : public NotsqlBaseException{
public:
    NotsqlNoDBSelectedException(
            const string & msg=""){
        _msg = "No database selected.\n" + msg;
    }
    void showMsg(){
        cerr << _msg << endl;
    }

    string getMsg(){
        return _msg;
    }

private:
    string _msg;
};


class NotsqlParseException : public NotsqlBaseException{
public:
    NotsqlParseException(int index, const string & msg="") : _msg(msg){
        this->index = index;
        _msg = "Fail to parse query.\n" + msg;
    }


    void showMsg(){
        cerr << _msg << endl;
    }

    virtual string getMsg(){
        return _msg;
    }

    int getIndex(){
        return index;
    }

private:
    string _msg;
    // The index in the statement where error occurs
    int index;
};

class NotsqlSyntaxException: public NotsqlBaseException {
public:
    NotsqlSyntaxException(const string & last_token, const string & msg=""){
        this->last_token = last_token;
        _msg = "Error after: \"" + last_token + "\".\n" + msg;
    };

    void showMsg(){
        cerr << _msg << endl;
    }

    string getMsg(){
        return _msg;
    }

    string getLastToken(){
        return last_token;
    }

private:
    string last_token;
    string _msg;
};

class NotsqlNotANumException : public NotsqlBaseException{
public: 
    NotsqlNotANumException(const string & num_str, const string & msg="") {
        this->num_str = num_str;
        _msg = "\"" + num_str + "\" is not a number.\n" + msg;
    }
    void showMsg(){
        cerr << _msg << endl;
    }
    string getMsg(){
        return _msg;
    }

    string getNumStr(){
        return num_str;
    }

private:
    string num_str;
    string _msg;
};

class NotsqlMemoryException: public NotsqlBaseException{
public:
    NotsqlMemoryException(const string & msg=""){
        _msg = "Memory error.\n" + msg;
    }
    void msg(){
        cerr << _msg << endl;
    }
private:
    string _msg;    
};


/** 
 * Exceptions in API
 */
class NotsqlFieldDoesNotExistException : public NotsqlBaseException{
public:
    NotsqlFieldDoesNotExistException(
            const string & field_name, const string & msg=""){
        this->field_name = field_name;
        this->msg = string() + "Field \"" + field_name + 
            "\" not exist.\n" + msg;
    }

    void showMsg(){
        cerr << msg << endl;
    }

    string getMsg(){
        return msg;
    }

    string getFieldName(){
        return field_name;
    }

private:
    string field_name;
    string msg;
};


class NotsqlTableDoesNotExistException : public NotsqlBaseException{
public:
    NotsqlTableDoesNotExistException(
            const string & table_name, const string & msg=""){
        this->table_name = table_name;
        this->msg = "Table \"" + table_name + 
            "\" does not exist.\n" + msg;
    }

    void showMsg(){
        cerr << msg << endl;
    }
    
    string getMsg(){
        return msg;
    }

    string getTableName(){
        return table_name;
    }

private:
    string table_name;
    string msg;
};


class NotsqlDuplicateFieldsException : public NotsqlBaseException{
public:
    NotsqlDuplicateFieldsException(
            const string & field_name, const string & msg=""){
        this->field_name = field_name;
        this->msg = "Duplacate field \"" + field_name + "\".\n" + msg;
    }

    void showMsg(){
        cerr << msg << endl;
    }

    string getMsg(){
        return msg;
    }

    string getFieldName(){
        return field_name;
    }
private:
    string field_name;
    string msg;
};


class NotsqlInvalidQueryException : public NotsqlBaseException{
public:
    NotsqlInvalidQueryException(const string & msg=""){
        this->msg = "Invalid query.\n" + msg;
    }
    void showMsg(){
        cerr << msg << endl;
    }
    string getMsg(){
        return msg;
    }

private:
    string msg;
};


class NotsqlFieldNotFoundException : public NotsqlBaseException{
public:
    NotsqlFieldNotFoundException(const string & field_name, 
            const string & msg=""){
        this->field_name = field_name;
        this->msg = string() + "Field \"" + field_name + "\" not found.\n";
        this->msg += msg;
    }
    void showMsg(){
        cerr << msg << endl;
    }

    string getMsg(){
        return msg;
    }

    string getFieldName(){
        return field_name;
    }
private:
    string field_name;
    string msg;
};


class NotsqlIndexOutOfBoundException : public NotsqlBaseException{
public:
    NotsqlIndexOutOfBoundException(const string & msg=""){
        this->msg = "Index out of bound\n" + msg;
    }
    void showMsg(){
        cerr << msg << endl;
    }

    string getMsg(){
        return msg;
    }
private:
    string msg;
};

class NotsqlLengthTooShortException : public NotsqlBaseException{
public:
    NotsqlLengthTooShortException(const string & msg=""){
        this->msg = "Length too short.\n" + msg;
    }
    void showMsg(){
        cerr << msg << endl;
    }

    string getMsg(){
        return msg;
    }
private:
    string msg;
};


/**
 * Exceptions in BufferManager
 */
class NotsqlBufferBlockFileNotFoundException : public NotsqlBaseException {
public:
    void showMsg() {
        cerr << "BufferManager Error: Database file or table not found" << endl;
    }
    string getMsg() {
        return "BufferManager Error: Database file or table not found";
    }
};

class NotsqlBufferBlockCanNotCreateFileException : public NotsqlBaseException {
public:
    void showMsg() {
        cerr << "BufferManager Fatal Error: Cannot create database file" << endl;
    }
    string getMsg() {
        return "BufferManager Fatal Error: Cannot create database file";
    }
};

class NotsqlBufferBlockWrongBlockToLoad : public NotsqlBaseException {
public:
    void showMsg() {
        cerr << "BufferManager Error: Wrong block to load" << endl;
        cerr << "\tindex is negative or this table doesn't have so many blocks" << endl;
    }
    string getMsg() {
        return "BufferManager Error: Wrong block to load";
    }
};

class NotsqlBufferBlockWrongRecordToGet : public NotsqlBaseException {
public:
    void showMsg() {
        cerr << "BufferManager Error: Wrong record to load" << endl;
        cerr << "\tmaybe this record is already deleted";
    }
    string getMsg() {
        return "BufferManager Error: Wrong record to load";
    }
};

class NotsqlBufferBlockFreeSpaceNotEnough : public NotsqlBaseException {
public:
    void showMsg() {
        cerr << "BufferManager Error: Block free space is not enough" << endl;
        cerr << "\tusually this error cannot be seen" << endl;
    }
    string getMsg() {
        return "BufferManager Error: Block free space is not enough";
    }
};

/**
 * Exceptions in CatalogManager
 */

class CatalogDatabaseDuplicateError: public NotsqlBaseException{
public:
	CatalogDatabaseDuplicateError(const string & msg=""){
        _msg = "Database name duplicated.\n" + msg;
    }
	void showMsg(){
        cerr << _msg << endl;
	}

    string getMsg(){
        return _msg;
    }
private:
    string _msg;
};


class CatalogDatabaseNotHaveError: public NotsqlBaseException{
public:
	CatalogDatabaseNotHaveError(const string & msg=""){
        _msg = "Database does not exist.\n" + msg;
    }
	void showMsg(){
        cerr << _msg << endl;
	}	
    string getMsg(){
        return _msg;
    }

private:
    string _msg;
};

class CatalogTableDuplicateError: public NotsqlBaseException{
public:
	CatalogTableDuplicateError(const string & msg=""){
        _msg = "Table name duplicate.\n" + msg;
    }
	void showMsg(){
        cerr << _msg << endl;
	}

    string getMsg(){
        return _msg;
    }

private:
    string _msg;
};


class CatalogTableNotHaveError: public NotsqlBaseException{
public:
	CatalogTableNotHaveError(const string & msg=""){
        _msg = "Table does not exist.\n" + msg;
    }
	void showMsg(){
        cerr << _msg << endl;
	}	
    string getMsg(){
        return _msg;
    }
private :
    string _msg;
};


class CatalogAttrDuplicateError: public NotsqlBaseException{
public:
	CatalogAttrDuplicateError(const string & msg=""){
        _msg = "Attribute name duplicated.\n" + msg;
    }
	void showMsg(){
        cerr << _msg << endl;
	}
    string getMsg(){
        return _msg;
    }
private:
    string _msg;
};


class CatalogAttrNotHaveError: public NotsqlBaseException{
public:
	CatalogAttrNotHaveError(const string msg=""){
        _msg = "Attribute name does not exist.\n" + msg;
    }
	void showMsg(){
        cerr << _msg << endl;
	}	
    string getMsg(){
        return _msg;
    }
private:
    string _msg;
};


class CatalogAttrTypeNameError: public NotsqlBaseException{
public:
	CatalogAttrTypeNameError(const string & msg=""){
        _msg = "Attribute name is invalid.\n" + msg;
    }
	void showMsg(){
        cerr << _msg << endl;
	}
    string getMsg(){
        return _msg;
    }

private:
    string _msg;
};


class CatalogAttrTypeValueError: public NotsqlBaseException{
public:
	CatalogAttrTypeValueError(const string & msg=""){
        _msg = "Attribute value does not match its type.\n" + msg;
    }
	void showMsg(){
        cerr << _msg << endl;
	}
    string getMsg(){
        return _msg;
    }

private:
    string _msg;
};

class CatalogIndexDuplicateError: public NotsqlBaseException{
public:
	CatalogIndexDuplicateError(const string & msg=""){
        _msg = "The attribute has had index already.\n" + msg;
    }
	void showMsg(){
        cerr << _msg << endl;
	}
    string getMsg(){
        return _msg;
    }

private:
    string _msg;
};

class CatalogIndexNotHaveError: public NotsqlBaseException{
public:
	CatalogIndexNotHaveError(const string & msg=""){
        _msg = "The attribute does not have index.\n" + msg;
    }
	void showMsg(){
        cerr << _msg << endl;
	}	
    string getMsg(){
        return _msg;
    }
private:
    string _msg;
};


/**
 * Exceptions in index
 */

class NotsqlFileException: public NotsqlBaseException{
public:
    NotsqlFileException(const string & msg=""){
        _msg = msg;
    }
    void showMsg(){
        cerr << _msg << endl;
    }
    string getMsg(){
        return _msg;
    }
private:
    string _msg;
};

#endif
