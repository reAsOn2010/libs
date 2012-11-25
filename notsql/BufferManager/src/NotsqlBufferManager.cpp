#include "NotsqlBufferManager.h"
using namespace std;


NotsqlBufferBlock::NotsqlBufferBlock(string prefix) {
    DBFILE_PATH_PREFIX = prefix;
    initial();
}

NotsqlBufferBlock::~NotsqlBufferBlock() {
    string table = getFileName();
    int num = getNum();
    try {
        writeBlockToDisk(db_name, table, num);
    }
    catch (NotsqlBufferBlockFileNotFoundException &ex) {
        //ex.showMsg();
    }
}

void NotsqlBufferBlock::initial() {
    memset(block, 0, BLOCK_SIZE);
    file_name = "";
    num = 0;
    dirty = false;
    writen = false;
    used_time = 0;
    int * begin = (int *)block;
    begin[0] = 0;
    begin[1] = BLOCK_SIZE -1;
}

void NotsqlBufferBlock::resort() {
    int count = getRecordCount();
    int final = count;
    //int new_end = getFreeSpaceEnd();
    int moved_space =0;
    int deleted_block_count = 0;

    for (int i = 1; i <= count; i++) {
        int temp_size = getRecordSize(i);
        if (!temp_size) {
            final--;
            deleted_block_count++;
            if (i == 1)
                moved_space += BLOCK_SIZE - getRecordPosition(1);
            else if (i) {
                moved_space += getRecordPosition(i-1) - getRecordPosition(i);
            }
            /*
            if (i < count) {
                moved_space += getRecordSize(i) - getRecordSize(i+1);
            }
            */
        }
        else {
            int old_position = getRecordPosition(i);
            int new_position = old_position + moved_space;
            move(new_position, old_position, temp_size);
            setRecordSize(i - deleted_block_count, temp_size);
            setRecordPosition(i - deleted_block_count, new_position);
        }
    }
    setRecordCount(final);
    setFreeSpaceEnd(getRecordPosition(final) - 1);

}

void NotsqlBufferBlock::move(int new_position, int old_position, int size) {
    for (int i = size - 1; i >= 0; i--) {
        block[new_position + i] = block[old_position + i];
    }
}

int NotsqlBufferBlock::getSizeOfFreeSpace() {
    int * begin = (int *)block;
    int end_of_free_space = begin[1];
    int record_count = begin[0];
    // all free space - other record entry - block head - new record entry
    return end_of_free_space - 8*record_count - 8 - 8;
}

void NotsqlBufferBlock::readBlockFromDisk(string db_name, string table, int block_num) {
    ifstream file;
    string table_path = DBFILE_PATH_PREFIX + db_name + "/" + table;
    file.open(table_path.c_str());
    if (!file.is_open()) {
        throw NotsqlBufferBlockFileNotFoundException();
        return;
    }

    struct stat filestatus;
    stat(table_path.c_str(), &filestatus);
    float tmp = (float)filestatus.st_size/BLOCK_SIZE;
    int total_block_num = ceil(tmp);

    if (block_num >= total_block_num || block_num < 0) {
        throw NotsqlBufferBlockWrongBlockToLoad();
        return;
    }

    int file_offset = block_num * BLOCK_SIZE;
    file.seekg(file_offset);
    file.read(block, BLOCK_SIZE);
    cleanDirty();
    setWriten();
    setDBName(db_name);
    setFileName(table);
    setNum(block_num);
    file.close();
    return;
}

void NotsqlBufferBlock::writeBlockToDisk(string db_name, string table, int block_num) {
    if (!isDirty())
        return;
    //ofstream file;
    fstream file;
    string table_path = DBFILE_PATH_PREFIX + db_name + "/" + table;
    //file.open(table_path.c_str(), ios_base::app);
    file.open(table_path.c_str(), ios_base::in|ios_base::out);

    if (!file.is_open()) {
        throw NotsqlBufferBlockFileNotFoundException();
        return;
    }
    int file_offset = block_num * BLOCK_SIZE;
    //file.seekp(ios_base::beg);
    file.seekp(file_offset);
    file.write(block, BLOCK_SIZE);
    cleanDirty();
    file.close();
    return;
}

void NotsqlBufferBlock::addRecord(const char * record, int size, int * record_index) {
    int available_size = getSizeOfFreeSpace();
    if (size > available_size) {
        throw NotsqlBufferBlockFreeSpaceNotEnough();
        return;
    }
    //int * begin = (int *)block;
    for (int i = 1; i <= getRecordCount(); i++) {
        if (getRecordSize(i) == 0) {
            for (int j = 0; j < size; j++)
                block[getRecordPosition(i) + j] = record[j];
            *record_index = i-1;       //when using getRecordList  index begins from 0!
            setRecordSize(i, size);
            setRecordCount(getRecordCount() + 1);
            setTime(time(NULL));
            setDirty();
            setWriten();
            return;
        }
    }

    int old_free_space_end = getFreeSpaceEnd();
    int count = getRecordCount();
    count++;
    setRecordCount(count);
    setRecordPosition(count, old_free_space_end - size + 1);
    for (int i = 0; i < size; i++)
        block[getRecordPosition(count) + i] = record[i];
    *record_index = count - 1;       //when using getRecordList  index begins from 0!
    setRecordSize(count, size);
    setFreeSpaceEnd(old_free_space_end - size);
    setTime(time(NULL));
    setDirty();
    setWriten();
    return;
}

void NotsqlBufferBlock::deleteRecord(int index) {
    //int * begin = (int *)block;
    if (index < 1/* || index > begin[0]*/) {    //avoid delete lead to throw
        throw NotsqlBufferBlockWrongRecordToGet();
        return;
    }
    if (getRecordSize(index) == 0) {
        throw NotsqlBufferBlockWrongRecordToGet();
        return;
    }
    setRecordSize(index, 0);
    setTime(time(NULL));
    setDirty();
    setRecordCount(getRecordCount() - 1);
    //resort();
    return;
}

char * NotsqlBufferBlock::getRecord(int index) {
    int * begin = (int *)block;
    if (/*index > begin[0] ||*/ index < 1) {   //avoid delete lead to throw
        throw NotsqlBufferBlockWrongRecordToGet();
        return NULL;
    }
    if (getRecordSize(index) == 0){
        throw NotsqlBufferBlockWrongRecordToGet();
        return NULL;
    }
    int start_position = begin[2*index + 1];
    setTime(time(NULL));
    return block + start_position;
}

int NotsqlBufferManager::getNumOfBlock(string db_name, string table) {
    //_mkdir("data");

    ifstream file;
    string table_path = DBFILE_PATH_PREFIX + db_name + "/" + table;


    file.open(table_path.c_str());
    if (!file.is_open()) {
        throw NotsqlBufferBlockFileNotFoundException();
        return -1;
    }

    struct stat filestatus;
    stat(table_path.c_str(), &filestatus);
    float tmp = (float)filestatus.st_size/BLOCK_SIZE;
    int block_num = ceil(tmp);

    file.close();
    return block_num;
}



NotsqlBufferManager::NotsqlBufferManager(string prefix) {
    DBFILE_PATH_PREFIX = prefix;
    for (int i = 0; i < TOTAL_BLOCK_NUMBER; i++) {
        blocks[i] = new NotsqlBufferBlock(DBFILE_PATH_PREFIX);
    }
}

NotsqlBufferManager::~NotsqlBufferManager() {
    for (int i = 0; i < TOTAL_BLOCK_NUMBER; i++) {
        delete blocks[i];
    }
    //delete[] blocks;
}

int NotsqlBufferManager::getFreeBlock() {
    int to_replace = 0;
    time_t min_time = blocks[0]->getTime();
    for (int i = 0; i < TOTAL_BLOCK_NUMBER; i++) {
        if (!blocks[i]->isWriten())
            return i;
        else if(blocks[i]->getTime() < min_time) {
            min_time = blocks[i]->getTime();
            to_replace = i;
        }
    }
    blocks[to_replace]->writeBlockToDisk(
            blocks[to_replace]->getDBName(),
            blocks[to_replace]->getFileName(),
            blocks[to_replace]->getNum());
    blocks[to_replace]->initial();
    return to_replace;
}

NotsqlBufferBlock * NotsqlBufferManager::getBlock(string db_name, string table, int num) {
    if (num < 0) {
        throw NotsqlBufferBlockWrongBlockToLoad();
        return NULL;
    }
    for (int i = 0; i < TOTAL_BLOCK_NUMBER; i++) {
        if ((blocks[i]->getDBName() == db_name) && (blocks[i]->getFileName() == table) && (blocks[i]->getNum() == num)) {
            return blocks[i];
        }
    }

    int free_block = getFreeBlock();
    blocks[free_block]->readBlockFromDisk(db_name, table, num);
    return blocks[free_block];
}

void NotsqlBufferManager::addRecord(string db_name,
        string table,
        const char * record,
        int size,
        int * block_num = NULL,
        int * record_index = NULL) {

    int n = getNumOfBlock(db_name, table);

    for (int i = 0; i < n; i++) {
        NotsqlBufferBlock* tmp = getBlock(db_name, table, i);
        try {
            tmp->addRecord(record, size, record_index);
            *block_num = i;
            return;
        }
        catch (NotsqlBufferBlockFreeSpaceNotEnough &ex) {
            continue;
        }
    }
    /*
    for (int i = 0; i < TOTAL_BLOCK_NUMBER; i++) {
        if (blocks[i]->getFileName() == table && blocks[i]->getDBName() == db_name) {
            try {
                blocks[i]->addRecord(record, size, record_index);
                *block_num = i;
                return;
            }
            catch (NotsqlBufferBlockFreeSpaceNotEnough &ex) {
                continue;
            }
        }
    }
    */
    int free_block = getFreeBlock();
    string table_path = DBFILE_PATH_PREFIX + db_name + "/" + table;

    struct stat filestatus;
    stat(table_path.c_str(), &filestatus);
    float tmp = (float)filestatus.st_size/BLOCK_SIZE;
    int total_block = ceil(tmp);

    *block_num = total_block;
    blocks[free_block]->addRecord(record, size, record_index);
    blocks[free_block]->setFileName(table);
    blocks[free_block]->setDBName(db_name);;
    blocks[free_block]->setNum(total_block);
    blocks[free_block]->setDirty();
    blocks[free_block]->setWriten();
    blocks[free_block]->setTime(time(NULL));
    blocks[free_block]->writeBlockToDisk(db_name, table, total_block);
    //file.close();

}

/*
void NotsqlBufferManager::getRecord(string db_name, string table, int num, int index, char ** records, int *size) {
    NotsqlBufferBlock * temp;
    temp = getBlock(db_name, table, num);
    *records = temp->getRecord(index);
    *size = BLOCK_SIZE - temp->getRecordPosition(index);
}
*/

void NotsqlBufferManager::getRecordList(string db_name, string table, int num, vector<char *> &out) {
    NotsqlBufferBlock * temp;
    vector<char *> record_list;
    temp = getBlock(db_name, table, num);

    int i = 1, j = 1;
    int count = temp->getRecordCount();
    while(i <= count) {
        try {
            record_list.push_back(temp->getRecord(j));
        } catch (NotsqlBufferBlockWrongRecordToGet &ex) {
            i--;
            record_list.push_back(NULL);
        }
        i++;
        j++;
    }
    out = record_list;
    return;
}
void NotsqlBufferManager::deleteRecord(string db_name, string table, int num, int index) {
    NotsqlBufferBlock * temp;
    temp = getBlock(db_name, table, num);

    try {
        temp->deleteRecord(index+1); //let index start from 1
        temp->setDirty();
        temp->setTime(time(NULL));
    } catch (NotsqlBufferBlockWrongRecordToGet &ex) {
        throw(ex);
    }
}

