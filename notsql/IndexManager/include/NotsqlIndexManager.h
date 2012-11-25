#ifndef __NOTSQLINDEXMANAGER_H__
#define __NOTSQLINDEXMANAGER_H__

#include "NotsqlBTree.h"
#include "NotsqlBufferManager.h"
#include "NotsqlTable.h"
#include "NotsqlConstants.h"
#include "NotsqlExpr.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <utility>

#define BTREE_M 6
//#define PATH_PREFIX "data/"
#define PATH_POSTFIX ".index"

using namespace std;

class NotsqlIndex {
    void * b_tree;
    string db_name;
    string table_name;
    string field_name;
    int data_offset;
    int data_size;
    int data_type;
    NotsqlBufferManager &bm;
    string PATH_PREFIX;
public:
    friend class NotsqlIndexManager;
    NotsqlIndex(string db_name,
            string table_name,
            string field_name,
            NotsqlBufferManager &buffer,
            int data_offset,
            int data_size,
            int data_type,
            string prefix);
    //void setDBName(string db_name);
    //void setTableName(string table_name);
    void initial(string db_name,
            string table_name,
            string field_name,
            int data_offset,
            int data_size,
            int data_type);
    void createIndex();
    void dropIndex();
    ~NotsqlIndex();
    void save();
    void load();

    void dealWithPointers(vector<NOTSQL_BTREE_VALUE> &pointers, vector<char *> &out);
    void insertToTree(void * data, int data_size, int data_type, NOTSQL_BTREE_VALUE pointer);
    void deleteFromTree(void * data, int data_size, int data_type, NOTSQL_BTREE_VALUE pointer);
    void deleteFromTree(int data_type, NOTSQL_BTREE_VALUE pointer);
    vector<char *> selectEqual(void * data, int data_size, int data_type);
    vector<char *> selectGreatThan(void * data, int data_size, int data_type);
    vector<char *> selectLessThan(void * data, int data_size, int data_type);
    vector<char *> selectGreatOrEqual(void * data, int data_size, int data_type);
    vector<char *> selectLessOrEqual(void * data, int data_size, int data_type);

    vector< pair<char *, int> > getAllIntPairs();
    vector< pair<char *, float> > getAllFloatPairs();
    vector< pair<char *, string> >getAllStringPairs();


    string traversal();

    string toString();
};

class NotsqlIndexManager {
    string db_name;
    NotsqlIndex* main_index;
    NotsqlIndex* vice_index; // only use when need join
    NotsqlBufferManager &bm;
    string PATH_PREFIX;
public:
    NotsqlIndexManager(string db_name, NotsqlBufferManager &buffer, string prefix);
    ~NotsqlIndexManager();
    void setMainIndex(string db_name,
            string table_name,
            string field_name,
            int data_offset,
            int data_size,
            int data_type);
    void setViceIndex(string db_name,
            string table_name,
            string field_name,
            int data_offset,
            int data_size,
            int data_type);
    void createIndex(NotsqlTable &table, string table_name);
    void dropIndex(NotsqlTable &table, string table_name);
    void insertUsingIndex(NotsqlTable &table,
            string table_name,
            int block_num,
            int record_index);
    void selectUsingIndex(NotsqlTable &table,
            NotsqlExpr &expr);
    NotsqlTable joinUsingIndex(NotsqlTable &table1,
            NotsqlTable &table2,
            NotsqlExpr expr);
    /*
    void selectUsingIndex(NotsqlTable &table,
            vector<NotsqlExpr> &exprs,
            vector<int> &rels
            );
    */
    void deleteUsingIndex(NotsqlTable &table,
            string table_name,
            int block_num,
            int record_index);

    void deleteUsingIndex(NotsqlTable &table,
            vector< pair<int, int> > pointers);

    string mainIndexToString();
    string viceIndexToString();
};
#endif
