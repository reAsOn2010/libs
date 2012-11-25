#include "NotsqlAPI.h"
#include <iostream>
#include <map>
#include <assert.h>
using namespace std;


NotsqlAPI::NotsqlAPI(const string & db_name, const string & PREFIX){
    this->PREFIX = PREFIX;
    catalog = new NotsqlCatalogManager(db_name, PREFIX);
    buffer_manager = new NotsqlBufferManager(PREFIX);
    record_manager = new NotsqlRecordManager(db_name, buffer_manager);
    index_manager = new NotsqlIndexManager(db_name, *buffer_manager, PREFIX);
}

void NotsqlAPI::createTable(
        const string & table_name,
        vector<string> & col_names,
        vector<TK_TYPE> & types,
        vector<int> & type_sizes,
        vector<string> & primary_keys,
        vector<bool> & not_nulls
        ){
    if(catalog->TableIsExist(table_name)){
        throw(NotsqlTableDoesNotExistException(table_name));
    }

    // Check the validation of the primary keys
    for(unsigned int i = 0; i != primary_keys.size(); ++i){
        if(find(col_names.begin(), col_names.end(), 
                    primary_keys[i]) == col_names.end()){
            throw(NotsqlInvalidQueryException("Invalid primary key"));
        }
    }

    vector<bool> attr_is_primary;
    if(primary_keys.size()){
        for(unsigned int i = 0; i != col_names.size(); ++i){
            if(find(primary_keys.begin(), primary_keys.end(),
                        col_names[i]) != primary_keys.end()){
                attr_is_primary.push_back(true);
            }else{
                attr_is_primary.push_back(false);
            }
        }
    }

    vector<string> tmp_indices;
    tmp_indices.resize(col_names.size());
    catalog->CreateTable(
            table_name,
            col_names,
            attr_is_primary,
            types,
            type_sizes,
            tmp_indices
            );
}

void NotsqlAPI::insert(
        const string & table_name,
        vector<string> & col_names,
        vector< pair<TK_TYPE, string> > & values
        ){
    if(!catalog->TableIsExist(table_name)){
        throw(NotsqlTableDoesNotExistException(table_name));
    }

    NotsqlTable table = getTableProto(table_name);

    // If column names are specfic
    // 暂时不支持
    /*
       for(unsigned int i = 0; i != col_names.size(); ++i){
       if(!table.hasField(col_names[i])){
       throw(NotsqlFieldDoesNotExistException(col_names[i]));
       }
       }
       */
    if(!col_names.size()){
        col_names = table.getFieldNames();
    }


    // Check the types
    if(col_names.size() != values.size()){
        throw(NotsqlInvalidQueryException("Value number error"));
    }

    for(unsigned int i = 0; i != col_names.size(); ++i){
        if(values[i].first != table.getFieldType(col_names[i])){
            throw(NotsqlInvalidQueryException("Value type error"));
        }
    }

    // Check the unique field
    checkUnique(table_name, values);

    vector<unsigned int> sizes;
    for(unsigned int i = 0; i != col_names.size(); ++i){
        sizes.push_back(table.getFieldSize(col_names[i]));
    }

    // split joint values
    // TODO 这里没有考虑默认值的问题！
    // 这里默认values包括了所有字段
    pair<int, int> res_data;
    try{
        res_data = record_manager->insert(table_name, table, values, sizes);
        index_manager->insertUsingIndex(table, table_name, res_data.first, res_data.second);
    }catch(...){
        throw;
    }
}


void NotsqlAPI::delete_(
        const string & table_name,
        vector< vector<NotsqlExpr> > & exprs
        ){
    vector<string> tables;

    if(!catalog->TableIsExist(table_name)){
        throw(NotsqlTableDoesNotExistException(table_name));
    }

    tables.push_back(table_name);
    checkAndFixupExprs(exprs, tables);
    NotsqlTable table = getTableProto(table_name);

    // Store the deleted records' position
    vector< pair<int, int> > del_pos;

    if(!exprs.size()){
        vector<NotsqlExpr> tmp_expr = vector<NotsqlExpr>();
        del_pos = record_manager->delete_(table_name, table, tmp_expr);
    }
    for(unsigned int i = 0; i != exprs.size(); ++i){
        vector< pair<int, int> > tmp_del_pos;
        vector<NotsqlExpr> expr_vec = exprs[i];
        tmp_del_pos = record_manager->delete_(table_name, table, expr_vec);
        for(unsigned int j = 0; j != tmp_del_pos.size(); ++j){
            del_pos.push_back(tmp_del_pos[j]);
        }
    }
    // Delete indices
    index_manager->deleteUsingIndex(table, del_pos);
}

void NotsqlAPI::createIndex(
        const string & table_name,
        const string & index_name,
        const string & col_name
        ){

    if(!catalog->TableIsExist(table_name)){
        throw(NotsqlTableDoesNotExistException(table_name));
    }
    if(!catalog->AttrNameIsExist(table_name, col_name)){
        throw(NotsqlFieldDoesNotExistException(col_name));
    }

    catalog->CreateIndex(table_name, col_name, index_name);

    NotsqlTable table = getTableProto(table_name);
    index_manager->createIndex(table, table_name);
}


void NotsqlAPI::dropTable(const string & table_name){
    if(!catalog->TableIsExist(table_name)){
        throw(NotsqlTableDoesNotExistException(table_name));
    }

    NotsqlTable table = getTableProto(table_name);
    index_manager->dropIndex(table, table_name);
    catalog->DropTable(table_name);
}


void NotsqlAPI::dropIndex(const string & table_name,
        const string & index_name){
    if(!catalog->TableIsExist(table_name)){
        throw(NotsqlTableDoesNotExistException(table_name));
    }
    NotsqlTable table = getTableProto(table_name);
    index_manager->dropIndex(table, table_name);
    catalog->DropIndex(table_name, index_name);
}

NotsqlTable NotsqlAPI::select(
        bool all_fields,
        bool distinct,
        vector< pair<TABLE_NAME, FIELD_NAME> > & fields,
        vector<TABLE_NAME> & tables,
        vector< vector<NotsqlExpr> > & exprs
        ){

    // The final result table
    NotsqlTable res_table;

    // Filter work
    for(unsigned int i = 0; i != tables.size(); ++i){
        if(!catalog->TableIsExist(tables[i])){
            throw(NotsqlTableDoesNotExistException(tables[i]));
        }
    }
    if(all_fields){
        getAllFields(fields, tables);
    }else{ 
        checkAndFixupFields(fields, tables);
    }
    checkAndFixupExprs(exprs, tables);


    vector<string> proj_field_names;
    for(unsigned int i = 0; i != fields.size(); ++i){
        proj_field_names.push_back(fields[i].first + "." + 
                fields[i].second);
    }

    // Do select first for optimization

    if(!exprs.size()){
        for(unsigned int i = 0; i != tables.size(); ++i){
            NotsqlTable tmp_table = getTableProto(tables[i]);
            record_manager->fetchAll(tmp_table, tables[i]);
            res_table = NotsqlTable::product(res_table, tmp_table);
        }
    }

    // Run each group of expressions 
    for(unsigned int i = 0; i != exprs.size(); ++i){

        vector<NotsqlExpr> curr_exprs = exprs[i];

        // All the table prototypes
        map<string, NotsqlTable> table_protos;

        // Joined table
        NotsqlTable table_joined;

        // Mark whether the table has been joined 
        map<string, bool> join_mark;

        // Get all the prototypes
        for(unsigned int ii = 0; ii != tables.size(); ++ii){
            table_protos[tables[ii]] = getTableProto(tables[ii]);
        }

        // Mark whether the table has been selected
        map<string, bool> select_mark;
        for(unsigned int ii = 0; ii != tables.size(); ++ii){
            select_mark[tables[ii]] = false;
        }

        // First do the select operations 
        // (Indicate that curr_expr.col2 is empty)
        for(unsigned int ii = 0; 
                ii != curr_exprs.size() &&
                !curr_exprs[ii].col2.size(); 
                ++ii){
            NotsqlExpr curr_expr = curr_exprs[ii];
            if(select_mark[curr_expr.table]){
                // The table has been selected 
                // So just do some filter work on it by the record manager
                record_manager->filterTable(table_protos[curr_expr.table], 
                        curr_expr);
            }else{
                // The table has not been selected
                if(table_protos[curr_expr.table].hasIndex(
                            curr_expr.col)){
                    // Has index
                    index_manager->selectUsingIndex(
                            table_protos[curr_expr.table],
                            curr_expr);
                }else{
                    // Has no index
                    record_manager->select(
                            table_protos[curr_expr.table],
                            curr_expr);
                }
                select_mark[curr_expr.table] = true;
            }
        }

        // Do the join operations
        // Fetch all the table which has not been selected yet
        for(map<string, NotsqlTable>::iterator t_it = table_protos.begin();
                t_it != table_protos.end();
                ++t_it){
            if(select_mark[(*t_it).first] == false){
                // The table has not be selected
                record_manager->fetchAll((*t_it).second, (*t_it).first);
            }
        }

        for(unsigned int table_index = 0; table_index != tables.size(); 
                ++table_index){
            res_table = NotsqlTable::product(res_table, 
                    table_protos[tables[table_index]]);
        }

        /*
        // Do AND operation on the tables
        for(unsigned int ii = 0; ii != curr_exprs.size() &&
        curr_exprs[ii].col2.size(); ++ii){
        NotsqlExpr curr_expr = curr_exprs[ii];
        // Has been joined yet?
        if(join_mark[expr]){
        }
        }

        for(map<string, NotsqlTable>::iterator t_it = table_protos.begin() + 1;
        t_it != table_protos.end();
        ++t_it){
        }
        */
    }

    // Do projection
    res_table.project(proj_field_names);
    return res_table;
}



/**
 * Check existence of the tables and fields,
 * and fill up the fileds vector
 */
void NotsqlAPI::checkAndFixupFields(
        vector< pair<TABLE_NAME, FIELD_NAME> > & fields,
        vector<TABLE_NAME> & tables
        ){
    // Check the tables' existence
    for(unsigned int i = 0; i != tables.size(); ++i){
        if(!catalog->TableIsExist(tables[i])){
            throw NotsqlTableDoesNotExistException(tables[i]);
        }
    }

    // Check the fields
    for(unsigned int i = 0; i != fields.size(); ++i){
        if(!fields[i].first.size()){
            // TABLE_NAME is empty
            string _table_name = checkFieldInTables(fields[i].second, tables);
            fields[i] = pair<TABLE_NAME, FIELD_NAME>(_table_name, fields[i].second);
        }else if(!catalog->AttrNameIsExist(fields[i].first, fields[i].second)){
            //No this field found in the specific table name
            throw(NotsqlFieldDoesNotExistException(fields[i].second));
        }
    }
}


/**
 * Check the existence of the fields in the expression,
 * and fill up the expression object
 */
void NotsqlAPI::checkAndFixupExprs(
        vector< vector<NotsqlExpr> > & exprs, 
        vector<TABLE_NAME> & tables
        ){
    for(unsigned int i = 0; i != exprs.size(); ++i){
        for(unsigned int j = 0; j != exprs[i].size(); ++j){
            // Check field
            if(!exprs[i][j].table.size()){
                exprs[i][j].table = checkFieldInTables(exprs[i][j].col, tables);
            }else{
                if(!catalog->AttrNameIsExist(exprs[i][j].table, exprs[i][j].col)){
                    throw(NotsqlFieldDoesNotExistException(exprs[i][j].col));
                }
            }

            // If it's a join expression
            if(exprs[i][j].col2.size()){
                if(!exprs[i][j].table2.size()){
                    exprs[i][j].table2 = checkFieldInTables(exprs[i][j].col2, tables);
                }else{
                    if(!catalog->AttrNameIsExist(exprs[i][j].table2, exprs[i][j].col2)){
                        throw(NotsqlFieldDoesNotExistException(exprs[i][j].col2));
                    }
                }
                TK_TYPE table_type1 = catalog->ReturnAttrType(
                        exprs[i][j].table,
                        exprs[i][j].col);

                TK_TYPE table_type2 = catalog->ReturnAttrType(
                        exprs[i][j].table2,
                        exprs[i][j].col2);

                // Decide the consistency of the types of two fields 
                if(table_type1 != table_type2){
                    throw(NotsqlInvalidQueryException("Field type not consistent"));
                }
            }else{
                // Check value
                if(exprs[i][j].type == TK_STRING &&
                        exprs[i][j].opr != TK_EQ &&
                        exprs[i][j].opr != TK_LIKE_KW){
                    throw NotsqlInvalidQueryException(
                            "Invalid opration"
                            );
                }
            }
        }
    }
}

string NotsqlAPI::checkFieldInTables(
        const string & field,
        vector<TABLE_NAME> & tables
        ){
    int count = 0;
    string _table_name;
    for(unsigned int ii = 0; ii != tables.size(); ++ii){
        if(catalog->AttrNameIsExist(tables[ii], field)){
            count++;
            // Duplicate field name found
            if(count > 1){
                throw(NotsqlDuplicateFieldsException(field));
            }
            _table_name = tables[ii];
        }
    }
    if(count == 0){
        throw(NotsqlFieldDoesNotExistException(field));
    }
    return _table_name;
}




/**
 * Get table prototype
 * You should check the table existence above
 */
NotsqlTable NotsqlAPI::getTableProto(const string & table_name){
    vector<CatalogAttr> raw_tables = catalog->ReturnAttrList(table_name);
    vector<char *> tmp_value;
    vector<string> field_names;
    vector<unsigned int> sizes;
    vector<TK_TYPE> types;
    vector<bool> indices;

    for(unsigned int i = 0; i != raw_tables.size(); ++i){
        field_names.push_back(raw_tables[i].AttrName);
        sizes.push_back(raw_tables[i].AttrLength);
        types.push_back(raw_tables[i].AttrType);
        if(raw_tables[i].AttrIndex == ""){
            indices.push_back(false);
        }else{
            indices.push_back(true);
        }
    }
    NotsqlTable table(
            tmp_value,
            field_names,
            sizes,
            types,
            indices,
            table_name
            );
    return table;
}


void NotsqlAPI::getAllFields(
        vector<pair<string, string> > & field_names,
        vector<string> & tables
        ){
    for(unsigned int i = 0; i != tables.size(); ++i){
        vector<CatalogAttr> attrs = catalog->ReturnAttrList(tables[i]);
        for(unsigned int j = 0; j != attrs.size(); ++j){
            field_names.push_back(pair<string, string>(tables[i], attrs[j].AttrName));
        }
    }
}


void NotsqlAPI::checkUnique(
        const string & table_name,
        vector< pair<TK_TYPE, string> > & values
        ){
    NotsqlTable table = getTableProto(table_name);
    vector<string> field_names = table.getFieldNames();
    string primary_field_name;
    unsigned int i;
    for(i = 0; i != field_names.size(); ++i){
        if(catalog->AttrIsPrimary(table_name, 
                    NotsqlTable::splitFieldName(field_names[i]).second)){
            primary_field_name = field_names[i];
            break;
        }
    }
    if(primary_field_name != ""){
        vector< pair<TABLE_NAME, FIELD_NAME> > fields;
        fields.push_back(pair<TABLE_NAME, FIELD_NAME>(
                    table_name, 
                    NotsqlTable::splitFieldName(primary_field_name).second
                    ));
        vector<TABLE_NAME> tables;
        tables.push_back(table_name);
        NotsqlExpr expr(
                table_name,
                NotsqlTable::splitFieldName(primary_field_name).second,
                TK_EQ,
                values[i].second,
                table.getFieldType(primary_field_name),
                "",
                "");

        vector< vector<NotsqlExpr> > expr_groups;
        vector<NotsqlExpr> exprs;
        exprs.push_back(expr);
        expr_groups.push_back(exprs);
        NotsqlTable res_table = select(true, false, fields, tables, expr_groups);
        if(res_table.getRecordsNum()){
            throw(NotsqlInvalidQueryException(
                        "Dumplicate value on field \"" + 
                        primary_field_name + 
                        "\"."
                        ));
        }
    }
}
