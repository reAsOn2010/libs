#include "NotsqlIndexManager.h"
#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

void generateRecord(char * test, char * name, int age, float salary) {
    memset(test, '\0', 18);
    for(int i = 0; i < 10; i++) {
        if(name[i] != '\0')
            test[i] = name[i];
        else
            break;
    }
    //memmove(test, name, 10);
    memmove(test+10, &age, 4);
    memmove(test+14, &salary, 4);
}

NotsqlIndex::NotsqlIndex(string db_name,
        string table_name,
        string field_name,
        NotsqlBufferManager &buffer,
        int data_offset,
        int data_size,
        int data_type,
        string prefix):bm(buffer) {
    initial(db_name, table_name, field_name, data_offset, data_size, data_type);
    PATH_PREFIX = prefix;
}

NotsqlIndex::~NotsqlIndex() {
    if (data_type == TK_INTEGER) {
        delete (NotsqlBTree<int> *)b_tree;
    }else if (data_type == TK_FLOAT) {
        delete (NotsqlBTree<float> *)b_tree;
    }else if (data_type == TK_STRING) {
        delete (NotsqlBTree<string> *)b_tree;
    }
}

void NotsqlIndex::initial(string db_name,
        string table_name,
        string field_name,
        int data_offset,
        int data_size,
        int data_type) {
    if (data_type == TK_INTEGER) {
        b_tree = new NotsqlBTree<int>(BTREE_M);
    }else if (data_type == TK_FLOAT) {
        b_tree = new NotsqlBTree<float>(BTREE_M);
    }else if(data_type == TK_STRING) {
        b_tree = new NotsqlBTree<string>(BTREE_M);
    }
    this->db_name = db_name;
    this->table_name = table_name;
    this->field_name = field_name;
    this->data_offset = data_offset;
    this->data_size = data_size;
    this->data_type = data_type;
}

void NotsqlIndex::createIndex() {

    int num = bm.getNumOfBlock(db_name, table_name);
    vector<char *> out;
    for (int i = 0; i < num; i++) {
        bm.getRecordList(db_name, table_name, i, out);
        for (unsigned int j = 0; j < out.size(); j++) {
            struct pointer ptr = {i, j};
            if (out[j] != NULL) {
                if (data_type == TK_INTEGER) {
                    int tmp1;
                    memcpy(&tmp1, out[j]+data_offset, data_size);
                    ((NotsqlBTree<int> *)b_tree)->put(tmp1, ptr);
                }else if (data_type == TK_FLOAT) {
                    float tmp2;
                    memcpy(&tmp2, out[j]+data_offset, data_size);
                    ((NotsqlBTree<float> *)b_tree)->put(tmp2, ptr);
                }else if (data_type == TK_STRING) {
                    string tmp3 = string(out[j]+data_offset, data_size);
                    ((NotsqlBTree<string> *)b_tree)->put(tmp3, ptr);
                }
            }
        }
    }
}

void NotsqlIndex::dropIndex() {

    string path = PATH_PREFIX + db_name + "/" + table_name +"_" + field_name + PATH_POSTFIX;
    cout << path << endl;
    ofstream file;
    file.open(path.c_str(), ios_base::binary);
    if (!file.is_open()) {
        throw(NotsqlFileException("Open index file error."));
        return;
    }
    file.close();
}

void NotsqlIndex::save() {
    string path = PATH_PREFIX + db_name + "/" + table_name +"_" + field_name + PATH_POSTFIX;
    cout << path << endl;
    ofstream file;
    file.open(path.c_str(), ios_base::binary);
    if (!file.is_open()) {
        throw(NotsqlFileException("Open index file error."));
        return;
    }
    if (data_type == TK_INTEGER) {
        NotsqlBTree<int> * t = (NotsqlBTree<int> *)b_tree;
        int ht = t->height();
        NotsqlBTreeNode<int> * x = t->getRoot();
        while (ht > 0) {
            x = x->children[0].next;
            ht--;
        }
        while (x->children[t->M-1].next != NULL) {
            for (int i = 0; i < x->m; i++) {
                file.write((char *)&(x->children[i].key), data_size);
                file.write((char *)&(x->children[i].value),
                        sizeof(struct pointer));
            }
            x = x->children[t->M-1].next;
        }
        for (int i = 0; i < x->m; i++) {
            file.write((char *)&(x->children[i].key), data_size);
            file.write((char *)&(x->children[i].value),
                    sizeof(struct pointer));
        }
        file.close();
    }else if (data_type == TK_FLOAT) {
        NotsqlBTree<float> * t = (NotsqlBTree<float> *)b_tree;
        int ht = t->height();
        NotsqlBTreeNode<float> * x = t->getRoot();
        while (ht > 0) {
            x = x->children[0].next;
            ht--;
        }
        while (x->children[t->M-1].next != NULL) {
            for (int i = 0; i < x->m; i++) {
                file.write((char *)&(x->children[i].key), data_size);
                file.write((char *)&(x->children[i].value),
                        sizeof(struct pointer));
            }
            x = x->children[t->M-1].next;
        }
        for (int i = 0; i < x->m; i++) {
            file.write((char *)&(x->children[i].key), data_size);
            file.write((char *)&(x->children[i].value),
                    sizeof(struct pointer));
        }
        file.close();
    }else if (data_type == TK_STRING) {
        NotsqlBTree<string> * t = (NotsqlBTree<string> *)b_tree;
        int ht = t->height();
        NotsqlBTreeNode<string> * x = t->getRoot();
        while (ht > 0) {
            x = x->children[0].next;
            ht--;
        }
        while (x->children[t->M-1].next != NULL) {
            for (int i = 0; i < x->m; i++) {
                file.write((x->children[i].key.c_str()), data_size);
                file.write((char *)&(x->children[i].value),
                        sizeof(struct pointer));
            }
            x = x->children[t->M-1].next;
        }
        for (int i = 0; i < x->m; i++) {
            file.write((x->children[i].key.c_str()), data_size);
            file.write((char *)&(x->children[i].value),
                    sizeof(struct pointer));
        }
        file.close();
    }
}

void NotsqlIndex::load() {
    if (data_type == TK_INTEGER) {
        delete (NotsqlBTree<int> *)b_tree;
        b_tree = new NotsqlBTree<int>(BTREE_M);
    }else if (data_type == TK_FLOAT) {
        delete (NotsqlBTree<float> *)b_tree;
        b_tree = new NotsqlBTree<float>(BTREE_M);
    }else if (data_type == TK_STRING) {
        delete (NotsqlBTree<string> *)b_tree;
        b_tree = new NotsqlBTree<string>(BTREE_M);
    }
    string path = PATH_PREFIX + db_name + "/" + table_name +"_" + field_name + PATH_POSTFIX;
    cout << path << endl;
    ifstream file;
    file.open(path.c_str(), ios_base::binary);
    if (!file.is_open()) {
        throw(NotsqlFileException("Open index file error."));
        return;
    }
    while(file.peek() != EOF) {
        NOTSQL_BTREE_VALUE ptr;
        if (data_type == TK_INTEGER) {
            int tmp1;
            file.read((char *)&tmp1, data_size);
            file.read((char *)&ptr, 8);
            ((NotsqlBTree<int> *)b_tree)->put(tmp1, ptr);
        }else if (data_type == TK_FLOAT) {
            float tmp2;
            file.read((char *)&tmp2, data_size);
            file.read((char *)&ptr, 8);
            ((NotsqlBTree<float> *)b_tree)->put(tmp2, ptr);
        }else if (data_type == TK_STRING) {
            char * tmp3 = new char[data_size];
            file.read(tmp3, data_size);
            file.read((char *)&ptr, 8);
            string str = string(tmp3);
            ((NotsqlBTree<string> *)b_tree)->put(str, ptr);
        }
    }
}

string NotsqlIndex::traversal() {
    if (data_type == TK_INTEGER) {
        return ((NotsqlBTree<int> *)b_tree)->traversal();
    }else if (data_type == TK_FLOAT) {
        return ((NotsqlBTree<float> *)b_tree)->traversal();
    }else if (data_type == TK_STRING) {
        return ((NotsqlBTree<string> *)b_tree)->traversal();
    }
    return "";
}

string NotsqlIndex::toString() {
    if (data_type == TK_INTEGER) {
        NotsqlBTree<int> *ptr1 = (NotsqlBTree<int>*)b_tree;
        return ptr1->toString();
    }else if (data_type == TK_FLOAT) {
        NotsqlBTree<float> *ptr2 = (NotsqlBTree<float>*)b_tree;
        return ptr2->toString();
    }else if(data_type == TK_STRING) {
        NotsqlBTree<string> *ptr3 = (NotsqlBTree<string>*)b_tree;
        return ptr3->toString();
    }
    return "";
}

vector<char *> NotsqlIndex::selectEqual(void * data, int data_size, int data_type) {
    vector<NOTSQL_BTREE_VALUE> pointers;
    vector<char *> ret;
    if (data_type == TK_INTEGER) {
        int tmp1;
        memcpy(&tmp1, data, data_size);
        ((NotsqlBTree<int> *)b_tree)->getEqual(tmp1, pointers);
    }
    else if (data_type == TK_FLOAT) {
        float tmp2;
        memcpy(&tmp2, data, data_size);
        ((NotsqlBTree<float> *)b_tree)->getEqual(tmp2, pointers);
    }
    else if (data_type == TK_STRING) {
        char* tmp3 = new char[data_size];
        memcpy(tmp3, data, data_size);
        string str(tmp3);
        ((NotsqlBTree<string> *)b_tree)->getEqual(str, pointers);
    }
    dealWithPointers(pointers, ret);
    return ret;
}

vector<char *> NotsqlIndex::selectGreatThan(void * data, int data_size, int data_type) {
    vector<NOTSQL_BTREE_VALUE> pointers;
    vector<char *> ret;
    if (data_type == TK_INTEGER) {
        int tmp1;
        memcpy(&tmp1, data, data_size);
        ((NotsqlBTree<int> *)b_tree)->getGreatThan(tmp1, pointers);
    }
    else if (data_type == TK_FLOAT) {
        float tmp2;
        memcpy(&tmp2, data, data_size);
        ((NotsqlBTree<float> *)b_tree)->getGreatThan(tmp2, pointers);
    }
    else if (data_type == TK_STRING) {
        char* tmp3 = new char[data_size];
        memcpy(tmp3, data, data_size);
        string str(tmp3);
        ((NotsqlBTree<string> *)b_tree)->getGreatThan(str, pointers);
    }
    dealWithPointers(pointers, ret);
    return ret;
}

vector<char *> NotsqlIndex::selectLessThan(void * data, int data_size, int data_type) {
    vector<NOTSQL_BTREE_VALUE> pointers;
    vector<char *> ret;
    if (data_type == TK_INTEGER) {
        int tmp1;
        memcpy(&tmp1, data, data_size);
        ((NotsqlBTree<int> *)b_tree)->getLessThan(tmp1, pointers);
    }
    else if (data_type == TK_FLOAT) {
        float tmp2;
        memcpy(&tmp2, data, data_size);
        ((NotsqlBTree<float> *)b_tree)->getLessThan(tmp2, pointers);
    }
    else if (data_type == TK_STRING) {
        char* tmp3 = new char[data_size];
        memcpy(tmp3, data, data_size);
        string str(tmp3);
        ((NotsqlBTree<string> *)b_tree)->getLessThan(str, pointers);
    }
    dealWithPointers(pointers, ret);
    return ret;
}

vector<char *> NotsqlIndex::selectGreatOrEqual(void * data, int data_size, int data_type) {
    vector<NOTSQL_BTREE_VALUE> pointers;
    vector<char *> ret;
    if (data_type == TK_INTEGER) {
        int tmp1;
        memcpy(&tmp1, data, data_size);
        ((NotsqlBTree<int> *)b_tree)->getEqual(tmp1, pointers);
        ((NotsqlBTree<int> *)b_tree)->getGreatThan(tmp1, pointers);
    }
    else if (data_type == TK_FLOAT) {
        float tmp2;
        memcpy(&tmp2, data, data_size);
        ((NotsqlBTree<float> *)b_tree)->getEqual(tmp2, pointers);
        ((NotsqlBTree<float> *)b_tree)->getGreatThan(tmp2, pointers);
    }
    else if (data_type == TK_STRING) {
        char* tmp3 = new char[data_size];
        memcpy(tmp3, data, data_size);
        string str(tmp3);
        ((NotsqlBTree<string> *)b_tree)->getEqual(str, pointers);
        ((NotsqlBTree<string> *)b_tree)->getGreatThan(str, pointers);
    }
    dealWithPointers(pointers, ret);
    return ret;
}

vector<char *> NotsqlIndex::selectLessOrEqual(void * data, int data_size, int data_type) {
    vector<NOTSQL_BTREE_VALUE> pointers;
    vector<char *> ret;
    if (data_type == TK_INTEGER) {
        int tmp1;
        memcpy(&tmp1, data, data_size);
        ((NotsqlBTree<int> *)b_tree)->getLessThan(tmp1, pointers);
        ((NotsqlBTree<int> *)b_tree)->getEqual(tmp1, pointers);
    }
    else if (data_type == TK_FLOAT) {
        float tmp2;
        memcpy(&tmp2, data, data_size);
        ((NotsqlBTree<float> *)b_tree)->getLessThan(tmp2, pointers);
        ((NotsqlBTree<float> *)b_tree)->getEqual(tmp2, pointers);
    }
    else if (data_type == TK_STRING) {
        char* tmp3 = new char[data_size];
        memcpy(tmp3, data, data_size);
        string str(tmp3);
        ((NotsqlBTree<string> *)b_tree)->getLessThan(str, pointers);
        ((NotsqlBTree<string> *)b_tree)->getEqual(str, pointers);
    }
    dealWithPointers(pointers, ret);
    return ret;
}

vector< pair<char *, int> > NotsqlIndex::getAllIntPairs() {
    vector< pair<NOTSQL_BTREE_VALUE, int> > tmp;

    vector<NOTSQL_BTREE_VALUE> pointers;

    vector<char *> out;

    ((NotsqlBTree<int> *)b_tree)->getAllPairs(tmp);
    for (unsigned int i = 0; i < tmp.size(); i++) {
        pointers.push_back(tmp[i].first);
    }
    dealWithPointers(pointers, out);

    vector< pair<char *, int> > ret;
    for (unsigned int i = 0; i < out.size(); i++) {
        pair<char *, int> tmp_pair(out[i], tmp[i].second);
        ret.push_back(tmp_pair);
    }

    return ret;
}

vector< pair<char *, float> > NotsqlIndex::getAllFloatPairs() {
    vector< pair<NOTSQL_BTREE_VALUE, float> > tmp;

    vector<NOTSQL_BTREE_VALUE> pointers;

    vector<char *> out;

    ((NotsqlBTree<float> *)b_tree)->getAllPairs(tmp);
    for (unsigned int i = 0; i < tmp.size(); i++) {
        pointers.push_back(tmp[i].first);
    }
    dealWithPointers(pointers, out);

    vector< pair<char *, float> > ret;
    for (unsigned int i = 0; i < out.size(); i++) {
        pair<char *, float> tmp_pair(out[i], tmp[i].second);
        ret.push_back(tmp_pair);
    }

    return ret;
}

vector< pair<char *, string> > NotsqlIndex::getAllStringPairs() {
    vector< pair<NOTSQL_BTREE_VALUE, string> > tmp;

    vector<NOTSQL_BTREE_VALUE> pointers;

    vector<char *> out;

    ((NotsqlBTree<string> *)b_tree)->getAllPairs(tmp);
    for (unsigned int i = 0; i < tmp.size(); i++) {
        pointers.push_back(tmp[i].first);
    }
    dealWithPointers(pointers, out);

    vector< pair<char *, string> > ret;
    for (unsigned int i = 0; i < out.size(); i++) {
        pair<char *, string> tmp_pair(out[i], tmp[i].second);
        ret.push_back(tmp_pair);
    }

    return ret;
}

void NotsqlIndex::dealWithPointers(vector<NOTSQL_BTREE_VALUE> &pointers,
        vector<char *> &out) {

    out.clear();
    if (!pointers.size())
        return;

    vector<char *> record_list;
    int num = pointers[0].block_num;
    bm.getRecordList(db_name, table_name, num, record_list);
    for (unsigned int i = 0; i < pointers.size(); i++) {
        if (num == pointers[i].block_num)
            out.push_back(record_list[pointers[i].record_index]);
        else{
            record_list.clear();
            num = pointers[i].block_num;
            bm.getRecordList(db_name, table_name, num, record_list);
            out.push_back(record_list[pointers[i].record_index]);
        }
    }
    return;
}

void NotsqlIndex::insertToTree(void * data, int data_size, int data_type, NOTSQL_BTREE_VALUE pointer) {
    if (data_type == TK_INTEGER) {
        int tmp1;
        memcpy(&tmp1, data, data_size);
        ((NotsqlBTree<int> *)b_tree)->put(tmp1, pointer);
    }
    else if (data_type == TK_FLOAT) {
        float tmp2;
        memcpy(&tmp2, data, data_size);
        ((NotsqlBTree<float> *)b_tree)->put(tmp2, pointer);
    }
    else if (data_type == TK_STRING) {
        char* tmp3 = new char[data_size];
        memcpy(tmp3, data, data_size);
        string str(tmp3);
        ((NotsqlBTree<string> *)b_tree)->put(str, pointer);
    }
}

void NotsqlIndex::deleteFromTree(void * data, int data_size, int data_type, NOTSQL_BTREE_VALUE pointer) {
    if (data_type == TK_INTEGER) {
        int tmp1;
        memcpy(&tmp1, data, data_size);
        ((NotsqlBTree<int> *)b_tree)->del(tmp1, pointer);
    }
    else if (data_type == TK_FLOAT) {
        float tmp2;
        memcpy(&tmp2, data, data_size);
        ((NotsqlBTree<float> *)b_tree)->del(tmp2, pointer);
    }
    else if (data_type == TK_STRING) {
        char * tmp3 = new char[data_size];
        memcpy(tmp3, data, data_size);
        string str(tmp3);
        ((NotsqlBTree<string> *)b_tree)->del(str, pointer);
    }
}

void NotsqlIndex::deleteFromTree(int data_type, NOTSQL_BTREE_VALUE pointer) {
    if (data_type == TK_INTEGER) {
        ((NotsqlBTree<int> *)b_tree)->del(pointer);
    }
    else if (data_type == TK_FLOAT) {
        ((NotsqlBTree<float> *)b_tree)->del(pointer);
    }
    else if (data_type == TK_STRING) {
        ((NotsqlBTree<string> *)b_tree)->del(pointer);
    }
}

NotsqlIndexManager::NotsqlIndexManager(string db_name,
        NotsqlBufferManager &buffer,
        string prefix):bm(buffer) {
    this->db_name = db_name;
    this->main_index = NULL;
    this->vice_index = NULL;
    this->PATH_PREFIX = prefix;
}

NotsqlIndexManager::~NotsqlIndexManager() {
    if (main_index)
        delete main_index;
    if (vice_index)
        delete vice_index;
}

void NotsqlIndexManager::setMainIndex(string db_name,
        string table_name,
        string field_name,
        int data_offset,
        int data_size,
        int data_type) {
    main_index = new NotsqlIndex(db_name,
            table_name,
            field_name,
            bm,
            data_offset,
            data_size,
            data_type,
            PATH_PREFIX);
}

void NotsqlIndexManager::setViceIndex(string db_name,
        string table_name,
        string field_name,
        int data_offset,
        int data_size,
        int data_type) {
    vice_index = new NotsqlIndex(db_name,
            table_name,
            field_name,
            bm,
            data_offset,
            data_size,
            data_type,
            PATH_PREFIX);
}

void NotsqlIndexManager::createIndex(NotsqlTable &table, string table_name) {

    vector<string> fields = table.getFieldNames();

    for (unsigned int i = 0; i < fields.size(); i++) {
        int data_offset = table.getFieldPos(fields[i]);
        int data_size = table.getFieldSize(fields[i]);
        int data_type = table.getFieldType(fields[i]);
        pair<string, string> table_and_field = NotsqlTable::splitFieldName(fields[i]);
        if (table.hasIndex(fields[i])) {
            setMainIndex(db_name, table_name, table_and_field.second,
                    data_offset, data_size, data_type);
            main_index->createIndex();
            main_index->save();
        }
    }
}

void NotsqlIndexManager::dropIndex(NotsqlTable &table, string table_name) {

    vector<string> fields = table.getFieldNames();

    for (unsigned int i = 0; i < fields.size(); i++) {
        int data_offset = table.getFieldPos(fields[i]);
        int data_size = table.getFieldSize(fields[i]);
        int data_type = table.getFieldType(fields[i]);
        pair<string, string> table_and_field = NotsqlTable::splitFieldName(fields[i]);
        if (!table.hasIndex(fields[i])) {
            setMainIndex(db_name, table_name, table_and_field.second,
                    data_offset, data_size, data_type);
            main_index->dropIndex();
        }
    }
}

void NotsqlIndexManager::selectUsingIndex(NotsqlTable &table,
        NotsqlExpr &expr) {
    string table_name = expr.table;
    string field_name = expr.col;
    string field_name_in_table = expr.table + "." +expr.col;
    int data_offset = table.getFieldPos(field_name_in_table);
    int data_size = table.getFieldSize(field_name_in_table);
    int data_type = table.getFieldType(field_name_in_table);
    setMainIndex(db_name, table_name, field_name,
            data_offset, data_size, data_type);
    main_index->load();
    char *data = new char[data_size];
    stringstream ss;
    if (data_type == TK_STRING) {
        memcpy(data, expr.value.c_str(), expr.value.size());
        memcpy(data+expr.value.size(), "\0", 1);
    }
    else if (data_type == TK_INTEGER) {
        ss << expr.value;
        ss >> *((int *)data);
    }
    else if (data_type == TK_FLOAT) {
        ss << expr.value;
        ss >> *((float *)data);
    }
    vector<char *> selected;
    if (expr.opr == TK_EQ) {
        selected = main_index->selectEqual(data, data_size, data_type);
    }
    else if (expr.opr == TK_NE) {
        selected = main_index->selectLessThan(data, data_size, data_type);
        vector<char *> tmp;
        tmp = main_index->selectGreatThan(data, data_size, data_type);
        for (unsigned int i = 0; i < tmp.size(); i++) {
            selected.push_back(tmp[i]);
        }
    }
    else if (expr.opr == TK_LT) {
        selected = main_index->selectLessThan(data, data_size, data_type);
    }
    else if (expr.opr == TK_LE) {
        selected = main_index->selectLessOrEqual(data, data_size, data_type);
    }
    else if (expr.opr == TK_GT) {
        selected = main_index->selectGreatThan(data, data_size, data_type);
    }
    else if (expr.opr == TK_GE) {
        selected = main_index->selectGreatOrEqual(data, data_size, data_type);
    }
   for (unsigned int j = 0; j < selected.size(); j++) {
        table.appendRecord(selected[j]);
    }
}

/*
void NotsqlIndexManager::selectUsingIndex(NotsqlTable &table,
        vector<NotsqlExpr> &exprs,
        vector<int> &rels) {

    NotsqlTable tmp_table = table;
    for(unsigned int i = 0; i < exprs.size(); i++) {
        string table_name = exprs[i].table;
        string field_name = exprs[i].col;
        int data_offset = table.getFieldPos(exprs[i].col);
        int data_size = table.getFieldSize(exprs[i].col);
        int data_type = table.getFieldType(exprs[i].col);
        setMainIndex(db_name, table_name, field_name, data_offset, data_size, data_type);
        main_index->load();
        char * data = new char[data_size];
        stringstream ss;
        if (data_type == TK_STRING) {
            memcpy(data, exprs[i].value.c_str(), exprs[i].value.size());
            memcpy(data+exprs[i].value.size(), "\0", 1);
        }
        else if (data_type == TK_INTEGER) {
            ss << exprs[i].value;
            ss >> *((int *)data);
        }
        else if (data_type == TK_FLOAT) {
            ss << exprs[i].value;
            ss >> *((float *)data);
        }
        vector<char *> selected;
        if (exprs[i].opr == TK_EQ) {
            selected = main_index->selectEqual(data, data_size, data_type);
        }
        else if (exprs[i].opr == TK_NE) {
        }
        else if (exprs[i].opr == TK_LT) {
            selected = main_index->selectLessThan(data, data_size, data_type);
        }
        else if (exprs[i].opr == TK_LE) {
            selected = main_index->selectLessOrEqual(data, data_size, data_type);
        }
        else if (exprs[i].opr == TK_GT) {
            selected = main_index->selectGreatThan(data, data_size, data_type);
        }
        else if (exprs[i].opr == TK_GE) {
            selected = main_index->selectGreatOrEqual(data, data_size, data_type);
        }
        if (i == 0) {
            for (unsigned int j = 0; j < selected.size(); j++) {
                table.addRecord(selected[j]);
            }
        }
        else {
            if (rels[i-1] == TK_OR) {
                for (unsigned int j = 0; j < selected.size(); j++) {
                    if (!table.hasRecord(selected[j]))
                        table.addRecord(selected[j]);
                }
            }
            else if (rels[i-1] == TK_AND) {
                for (unsigned int j = 0; j < selected.size(); j++) {
                    if (table.hasRecord(selected[j]))
                        tmp_table.addRecord(selected[j]);
                }
            table = tmp_table;
            tmp_table.clear();
            }
        }
    }
}
*/

NotsqlTable NotsqlIndexManager::joinUsingIndex(NotsqlTable &table1,
        NotsqlTable &table2,
        NotsqlExpr expr) {

    string table_name1 = expr.table;
    string field_name1 = expr.col;
    string field_name_in_table1 = expr.table + "." + expr.col;
    vector<string> table_fields1 = table1.getFieldNames();

    int data_offset1 = table1.getFieldPos(field_name_in_table1);
    int data_size1 = table1.getFieldSize(field_name_in_table1);
    int data_type1 = table1.getFieldType(field_name_in_table1);

    string table_name2 = expr.table2;
    string field_name2 = expr.col2;
    string field_name_in_table2 = expr.table2 + "." + expr.col2;
    vector<string> table_fields2 = table2.getFieldNames();

    int data_offset2 = table2.getFieldPos(field_name_in_table2);
    int data_size2 = table2.getFieldSize(field_name_in_table2);
    int data_type2 = table2.getFieldType(field_name_in_table2);

    vector<char *> table_data1 = table1.getFieldData(field_name_in_table1);

    NotsqlTable ret;
    ret.extends(table1);
    ret.extends(table2);
    if (data_type1 != data_type2) {
        return ret;
    }
    vector<string> ret_fields = ret.getFieldNames();
    if (table_data1.size() == 0) {

        setMainIndex(db_name, table_name1, field_name1,
                data_offset1, data_size1, data_type1);
        setViceIndex(db_name, table_name2, field_name2,
                data_offset2, data_size2, data_type2);
        main_index->load();
        vice_index->load();

        unsigned int p1 = 0;
        unsigned int p2 = 0;
        unsigned int sp = 0;
        vector< pair<char *, char *> > selected;
        if (data_type1 == TK_INTEGER) {
            vector< pair<char *, int> > pairs1 = main_index->getAllIntPairs();
            vector< pair<char *, int> > pairs2 = vice_index->getAllIntPairs();
            for(p1 = 0; p1 < pairs1.size(); p1++) {
                while(sp < pairs2.size() && pairs1[p1].second > pairs2[sp].second) sp++;
                for(p2 = sp; p2 < pairs2.size(); p2++) {
                    if(pairs1[p1].second != pairs2[p2].second) break;
                    pair <char *, char *> tmp (pairs1[p1].first, pairs2[p2].first);
                    selected.push_back(tmp);
                }
            }
        }
        else if (data_type1 == TK_FLOAT) {
            vector< pair<char *, float> > pairs1 = main_index->getAllFloatPairs();
            vector< pair<char *, float> > pairs2 = vice_index->getAllFloatPairs();
            for(p1 = 0; p1 < pairs1.size(); p1++) {
                while(sp < pairs2.size() && pairs1[p1].second > pairs2[sp].second) sp++;
                for(p2 = sp; p2 < pairs2.size(); p2++) {
                    if(pairs1[p1].second != pairs2[p2].second) break;
                    pair <char *, char *> tmp (pairs1[p1].first, pairs2[p2].first);
                    selected.push_back(tmp);
                }
            }
        }
        else if (data_type1 == TK_STRING) {
            vector< pair<char *, string> > pairs1 = main_index->getAllStringPairs();
            vector< pair<char *, string> > pairs2 = vice_index->getAllStringPairs();
            for(p1 = 0; p1 < pairs1.size(); p1++) {
                while(sp < pairs2.size() && pairs1[p1].second > pairs2[sp].second) sp++;
                for(p2 = sp; p2 < pairs2.size(); p2++) {
                    if(pairs1[p1].second != pairs2[p2].second) break;
                    pair <char *, char *> tmp (pairs1[p1].first, pairs2[p2].first);
                    selected.push_back(tmp);
                }
            }
        }
        for (unsigned int i = 0; i < selected.size(); i++) {
            int size = 0;
            for (unsigned int j = 0; j < table_fields1.size(); j++) {
                ret.appendFieldData(table_fields1[j], selected[i].first + size);
                size += table1.getFieldSize(table_fields1[j]);
            }
            size = 0;
            for (unsigned int j = 0; j < table_fields2.size(); j++) {
                ret.appendFieldData(table_fields2[j], selected[i].second + size);
                size += table2.getFieldSize(table_fields2[j]);
            }
        }
    }

    else {
        setMainIndex(db_name, table_name2, field_name2,
                data_offset2, data_size2, data_type2);
        main_index->load();
        for (unsigned int i = 0; i < table_data1.size(); i++) {
            vector<char *> selected;
            selected = main_index->selectEqual(table_data1[i], data_size1, data_type1);
            for (unsigned int j = 0; j < selected.size(); j++) {
                int size = 0;
                for (unsigned int k = 0; k < table_fields1.size(); k++) {
                    ret.appendFieldData(table_fields1[k], table_data1[i] + size);
                    size += table1.getFieldSize(table_fields1[k]);
                }
                size = 0;
                for (unsigned int k = 0; k < table_fields2.size(); k++) {
                    ret.appendFieldData(table_fields2[k], selected[j] + size);
                    size += table2.getFieldSize(table_fields2[k]);
                }
            }
        }
    }
    return ret;
}

void NotsqlIndexManager::insertUsingIndex(NotsqlTable &table,
        string table_name,
        int block_num,
        int record_index) {

    vector<string> fields = table.getFieldNames();
    NOTSQL_BTREE_VALUE pointer = {block_num, record_index};
    for (unsigned int i = 0; i < fields.size(); i++) {
        int data_offset = table.getFieldPos(fields[i]);
        int data_size = table.getFieldSize(fields[i]);
        int data_type = table.getFieldType(fields[i]);
        pair<string, string> table_and_field = NotsqlTable::splitFieldName(fields[i]);
        if (table.hasIndex(fields[i])) {
            setMainIndex(db_name, table_name, table_and_field.second,
                    data_offset, data_size, data_type);
            main_index->load();
            vector<char *> data_set = table.getFieldData(fields[i]);
            main_index->insertToTree(data_set[0], data_size, data_type, pointer);
            main_index->save();
        }
    }
}

void NotsqlIndexManager::deleteUsingIndex(NotsqlTable &table,
        string table_name,
        int block_num,
        int record_index) {
    vector<string> fields = table.getFieldNames();
    NOTSQL_BTREE_VALUE pointer = {block_num, record_index};
    for (unsigned int i = 0; i < fields.size(); i++) {
        int data_offset = table.getFieldPos(fields[i]);
        int data_size = table.getFieldSize(fields[i]);
        int data_type = table.getFieldType(fields[i]);
        pair<string, string> table_and_field = NotsqlTable::splitFieldName(fields[i]);
        if (table.hasIndex(fields[i])) {
            setMainIndex(db_name, table_name, table_and_field.second,
                    data_offset, data_size, data_type);
            main_index->load();
            vector<char *> data_set = table.getFieldData(fields[i]);
            main_index->deleteFromTree(data_set[0], data_size, data_type, pointer);
            main_index->save();
        }
    }
}

void NotsqlIndexManager::deleteUsingIndex(NotsqlTable &table,
        vector< pair<int, int> > pointers) {

    vector<string> fields = table.getFieldNames();
    for (unsigned int i = 0; i < fields.size(); i++) {
        int data_offset = table.getFieldPos(fields[i]);
        int data_size = table.getFieldSize(fields[i]);
        int data_type = table.getFieldType(fields[i]);
        pair<string, string> table_and_field = NotsqlTable::splitFieldName(fields[i]);
        if (table.hasIndex(fields[i])) {
            setMainIndex(db_name, table_and_field.first, table_and_field.second,
                    data_offset, data_size, data_type);
            main_index->load();
            for (unsigned int j = 0; j < pointers.size(); j++) {
                NOTSQL_BTREE_VALUE tmp_pointer = {pointers[j].first, pointers[j].second};
                main_index->deleteFromTree(data_type, tmp_pointer);
            }
            main_index->save();
        }
    }
}

string NotsqlIndexManager::mainIndexToString() {
    return main_index->toString();
}
string NotsqlIndexManager::viceIndexToString() {
    return vice_index->toString();
}

