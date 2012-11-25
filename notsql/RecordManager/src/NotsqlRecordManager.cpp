#include "NotsqlRecordManager.h"
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <cstdio>

using namespace std;


NotsqlRecordManager::NotsqlRecordManager(
        const string & db_name,
        NotsqlBufferManager * buffer_manager){
    this->db_name = db_name;
    this->buffer_manager = buffer_manager;
}


void NotsqlRecordManager::fetchAll(
        NotsqlTable & table,
        const string & table_name
){
    vector<char *> out;

    int block_num = buffer_manager->getNumOfBlock(db_name, table_name);
    for(int i = 0; i != block_num; ++i){
        buffer_manager->getRecordList(db_name, table_name, i, out);
        for(unsigned int j = 0; j != out.size(); ++j){
            if(out[j] == NULL){
                continue;
            }
            table.appendRecord(out[j]);
        }
    }
}




void NotsqlRecordManager::filterTable(NotsqlTable & table,
        NotsqlExpr expr){
    vector<char *> & data = table.getFieldData(expr.table + "." + expr.col);
    vector<unsigned int> remove_index;
    for(unsigned int i = 0; i != data.size(); ++i){
        if(!cmp(data[i], expr.value, expr.type, expr.opr, 
                table.getFieldSize(expr.table + "." + expr.col))){
            remove_index.push_back(i);
        }
    }

    int count = 0;
    for(unsigned int i = 0; i != remove_index.size(); ++i){
        table.removeRecord(remove_index[i] - count);
        count++;
    }
}


/**
 * NOTE
 *
 * Select only do the filter work
 * You should first run the fetchAll method to 
 * fetch all the records from BufferManager
 *
 */

void NotsqlRecordManager::select(
    NotsqlTable & table, 
    NotsqlExpr expr,
    int limit
){
    unsigned int col_pos;
    unsigned int col_sz;
    char * p;
    vector<char *> records;

    int blocks_num = buffer_manager->getNumOfBlock(
            db_name, expr.table);

    col_pos = table.getFieldPos(expr.table + "." + expr.col);
    col_sz = table.getFieldSize(expr.table + "." + expr.col);

    for(int i = 0; i != blocks_num; ++i){
        buffer_manager->getRecordList(db_name, expr.table, i, records);

        for(unsigned int ii = 0; ii != records.size(); ++ii){
            if(records[ii] == NULL){
                continue;
            }
            p = records[ii] + col_pos;

            if(cmp(p, expr.value, expr.type, expr.opr, col_sz)){
                table.appendRecord(records[ii]);
            }
        }
    }
}

vector< pair<int, int> > NotsqlRecordManager::delete_( 
        const string & table_name,
        NotsqlTable & table,
        vector<NotsqlExpr> & exprs){
    vector<char *> records;
    vector< pair<int, int> > del_pos;
    int blocks_num = buffer_manager->getNumOfBlock(
            db_name, table_name);
    for(int block_index = 0; block_index != blocks_num; 
            ++block_index){
        buffer_manager->getRecordList(db_name, table_name, block_index, records);
        for(unsigned int record_index = 0; record_index != records.size();
                ++record_index){
            if(records[record_index] == NULL){
                continue;
            }
            bool fit = true;
            char *p;
            for(unsigned int expr_index = 0; expr_index != exprs.size();
                    ++expr_index){
                NotsqlExpr expr = exprs[expr_index];
                unsigned int col_pos = table.getFieldPos(expr.table + "." + expr.col);
                unsigned int col_sz = table.getFieldSize(expr.table + "." + expr.col);
                p = records[record_index] + col_pos;
                if(!cmp(p, expr.value, expr.type, expr.opr, col_sz)){
                    fit = false;
                    break;
                }
            }
            // Fit the contions, delete it
            if(fit){
                buffer_manager->deleteRecord(
                        db_name, table_name, block_index, record_index);
                del_pos.push_back(pair<int, int>(block_index, record_index));
            }
        }
    }
    return del_pos;
}


pair<int, int> NotsqlRecordManager::insert(
        const string & table_name,
        NotsqlTable & table,
        vector< pair<TK_TYPE, string> > & values,
        vector<unsigned int> sizes){ 
    int block_num;
    int record_index;

    unsigned int total_size = table.getRecordSize();

    char * record = (char *)malloc(total_size);
    char * tmp;

    unsigned int sz = 0;

    double num;
    int int_value;
    float float_value;

    // Generate a record pointer
    for(unsigned int i = 0; i != values.size(); ++i){
        if(values[i].first == TK_INTEGER){
            num = toNum(values[i].second);
            int_value = (int)num;
            tmp = (char *)(&int_value);
            for(unsigned int j = 0; j != sizes[i]; ++j){
                record[sz + j] = tmp[j];
            }
        }else if(values[i].first == TK_FLOAT){
            num = toNum(values[i].second);
            float_value = (float)num;
            tmp = (char *)(&float_value);
            for(unsigned int j = 0; j != sizes[i]; ++j){
                record[sz + j] = tmp[j];
            }
        }else if(values[i].first == TK_DOUBLE){
            num = toNum(values[i].second);
            tmp = (char *)(&num);
            for(unsigned int j = 0; j != sizes[i]; ++j){
                record[sz + j] = tmp[j];
            }
        }else if(values[i].first == TK_CHAR){
            unsigned int _sz = values[i].second.size() < sizes[i] - 1 ?
                values[i].second.size() : sizes[i] - 1;

            for(unsigned int j = 0; j != _sz; ++j){
                record[sz + j] = values[i].second[j];
            }
            record[sz + _sz] = '\0';
        }
        sz += sizes[i];
    }
    table.appendRecord(record);


    // TODO
    // 修改
    int _block_num = buffer_manager->getNumOfBlock(db_name, table_name);

    if(_block_num <= TOTAL_BLOCK_NUMBER){
        for(int block_index = 0; block_index != _block_num; ++block_index){
            buffer_manager->getBlock(db_name, table_name, block_index);
        }
    }else{
    }

    buffer_manager->addRecord(db_name, table_name, record, 
            total_size, &block_num, &record_index);
    // TODO
    // Update indices

    free(record);
    return pair<int, int>(block_num, record_index);
}



double NotsqlRecordManager::toNum(const string & str){
    stringstream ss;
    double res;
    ss << str;
    ss >> res;
    return res;

}



bool NotsqlRecordManager::cmp(
        const char * a, const string & b, 
        int type, 
        int opr, 
        unsigned int sz=4){
    if(type == TK_STRING){
        switch(opr){
            case TK_EQ:
                if(string(a) != b){
                    return false;
                }
                return true;
            case TK_LIKE_KW:
                //TODO
                return false;
            default:
                return false;
        }
    }
    else{
        int int_a;
        int int_b;
        float float_a;
        float float_b;
        stringstream ss;
        ss << b;
        switch(type){
            case TK_INTEGER:
                int_a = (*((const int *)a));
                ss >> int_b;
                break;
            case TK_FLOAT:
                float_a = (*((const float *)a));
                ss >> float_b;
                break;
                //case TK_DOUBLE:
                //    _a = (double)(*((const double *)a));
                //    ss >> _b;
                //    break;
            default:
                return false;
        }
        switch(opr){
            case TK_GT:
                if(type == TK_INTEGER){
                    return int_a > int_b;
                }else if(type == TK_FLOAT){
                    return float_a > float_b;
                }
            case TK_GE:
                if(type == TK_INTEGER){
                    return int_a >= int_b;
                }else if(type == TK_FLOAT){
                    return float_a >= float_b;
                }
            case TK_LT:
                if(type == TK_INTEGER){
                    return int_a < int_b;
                }else if(type == TK_FLOAT){
                    return float_a < float_b;
                }
            case TK_LE:
                if(type == TK_INTEGER){
                    return int_a <= int_b;
                }else if(type == TK_FLOAT){
                    return float_a <= float_b;
                }
            case TK_EQ:
                if(type == TK_INTEGER){
                    return int_a == int_b;
                }else if(type == TK_FLOAT){
                    return float_a == float_b;
                }
            default:
                return false;
        }
    }
}



