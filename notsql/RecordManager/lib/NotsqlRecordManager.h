#ifndef _NOTSQL_RECORD_MANAGER_H_
#define _NOTSQL_RECORD_MANAGER_H_


#include <iostream>
#include <string>
#include "NotsqlBufferManager.h"
#include "NotsqlExceptions.h"
#include "NotsqlTable.h"
#include "NotsqlConstants.h"
#include "NotsqlExpr.h"

// No limit
#ifndef NOTSQL_NO_LIMIT
 #define NOTSQL_NO_LIMIT -1
#endif


class NotsqlRecordManager{
public:
    /**
     * The top module should pass a pointer of 
     * the buffer manager object and the API object
     */
    NotsqlRecordManager(
            const string & db_name,
            NotsqlBufferManager * buffer_manager
    );

    virtual ~NotsqlRecordManager(){};

    /**
     * Note
     * The project operation in select clause should be done by the API
     */
    void select(
            NotsqlTable & table, 
            NotsqlExpr expr,
            int limit=NOTSQL_NO_LIMIT
    );

    /**
     * Do filter work on the table
     */
    void filterTable(NotsqlTable & table, NotsqlExpr expr);

    pair<int, int> insert(
            const string & table_name,
            NotsqlTable & table,
            vector< pair<TK_TYPE, string> > & values,
            vector<unsigned int> sizes
    );

    
    /**
     * Delete clause
     */
    vector< pair<int, int> > delete_( 
            const string & table_name,
            NotsqlTable & table,
            vector<NotsqlExpr> & exprs);


/*
    void setAPI(NotsqlAPI * api){
        this->api = api;
    };
*/

    void fetchAll(
            NotsqlTable & table,
            const string & table_name
    );

private:
    /**
     * Compare function 
     */

    //static bool cmp(void * a, void * b, int opr, unsigned int a_sz=4, unsigned int b_sz=4);

    static double toNum(const string & str);
    string db_name;
    NotsqlBufferManager * buffer_manager;
//    NotsqlAPI * api;

    bool cmp(const char * a, 
            const string & b, 
            int type, 
            int opr, 
            unsigned int sz);
};
#endif
