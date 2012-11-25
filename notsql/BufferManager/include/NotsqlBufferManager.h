#ifndef __NOTSQLBUFFMERMANAGER__
#define __NOTSQLBUFFMERMANAGER__

#include "NotsqlExceptions.h"
#include <time.h>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
#include <vector>

#define BLOCK_SIZE 2048
#define TOTAL_BLOCK_NUMBER 100
#define MAX_FILENAME_LENGTH 256
//#define DBFILE_PATH_PREFIX "data/"

/*
 * ------------------------------------------------------------
 * |total |free   |record |record |         |        |        |
 * |record|space  |size   |size   | free    | record | record |
 * |      |end    |-------|-------| spaces  |        |        |
 * |      |pointer|record |record | ......  |        |        |
 * |      |       |pointer|pointer|         |        |        |
 * ------------------------------------------------------------
 *                |two int|
 *
 */




using namespace std;


class NotsqlBufferBlock;


class NotsqlBufferManager
{
    NotsqlBufferBlock * blocks[TOTAL_BLOCK_NUMBER];
    int getFreeBlock();
    string DBFILE_PATH_PREFIX;
public:
    NotsqlBufferManager(string prefix);
    ~NotsqlBufferManager();

    int getNumOfBlock(string db_name, string table);
    NotsqlBufferBlock * getBlock(string db_name, string table, int num);
    void addRecord(string db_name, string table, const char * record, int size, int * block_num, int * record_index);
    void getRecord(string db_name, string table, int num, int index, char ** records, int *size);
    void deleteRecord(string db_name, string table, int num , int index);
    void getRecordList(string db_name, string table, int num, vector<char *>&out);
    //void drop(string table);
};



class NotsqlBufferBlock
{
    char block[BLOCK_SIZE];
    string db_name;
    string file_name;
    int num;                    //block num in file
    bool dirty;
    bool writen;
    time_t used_time;
    string DBFILE_PATH_PREFIX;
public:
    /*
     * all the block index should be larger than or equal to 1
     */
    NotsqlBufferBlock(string prefix);
    ~NotsqlBufferBlock();
    // interface
    void addRecord(const char *record, int size, int * record_index);
    void deleteRecord(int index);
    char * getRecord(int index);
    vector<char *> &getRecordList();

    // inner operations
    char * getBlockHead();

    int getRecordSize(int index);
    void setRecordSize(int index, int size);

    int getRecordPosition(int index);
    void setRecordPosition(int index, int position);

    void setDBName(string new_db_name);
    string getDBName();
    void setFileName(string new_file_name);
    string getFileName();

    int getNum();
    void setNum(int new_num);

    void setDeleted(int index);
    bool getDeleted(int index);
    void cleanDeleted(int index);

    bool isDirty();
    void setDirty();
    void cleanDirty();

    bool isWriten();
    void setWriten();
    void cleanWriten();

    void move(int new_position, int old_position, int size);

    int getRecordCount();
    void setRecordCount(int count);

    time_t getTime();
    void setTime(time_t new_time);

    int getFreeSpaceEnd();
    void setFreeSpaceEnd(int new_end);

    int getSizeOfFreeSpace();

    void resort();              // restore deleted space

    void readBlockFromDisk(string db_name, string table, int block_num);
    void writeBlockToDisk(string db_name, string table, int block_num);

    void initial();
};


inline char * NotsqlBufferBlock::getBlockHead() {
    return block;
}

inline int NotsqlBufferBlock::getRecordSize(int index) {
    int * begin = (int *)block;
    if(index < 1 /*|| index > begin[0]*/) {
        throw NotsqlBufferBlockWrongRecordToGet();
        return -1;
    }
    return begin[index*2];
}
inline void NotsqlBufferBlock::setRecordSize(int index, int size) {
    int * begin = (int *)block;
    if(index < 1 /*|| index > begin[0]*/) {
        throw NotsqlBufferBlockWrongRecordToGet();
        return;
    }
    begin[index*2] = size;
}

inline int NotsqlBufferBlock::getRecordPosition(int index) {
    int * begin = (int *)block;
    if(index < 1 /*|| index > begin[0]*/) {
        throw NotsqlBufferBlockWrongRecordToGet();
        return -1;
    }
    return begin[index*2 + 1];
}
inline void NotsqlBufferBlock::setRecordPosition(int index, int position) {
    int * begin = (int *)block;
    if(index < 1 /*|| index > begin[0]*/) {
        throw NotsqlBufferBlockWrongRecordToGet();
        return;
    }
    begin[index*2 + 1] = position;
}

inline void NotsqlBufferBlock::setDBName(string new_db_name) {
    db_name = new_db_name;
}
inline string NotsqlBufferBlock::getDBName() {
    return db_name;
}
inline void NotsqlBufferBlock::setFileName(string new_file_name) {
    file_name = new_file_name;
}
inline string NotsqlBufferBlock::getFileName() {
    return file_name;
}

inline int NotsqlBufferBlock::getNum() {
    return num;
}
inline void NotsqlBufferBlock::setNum(int new_num) {
    num = new_num;
}

inline bool NotsqlBufferBlock::isDirty() {
    return dirty;
}
inline void NotsqlBufferBlock::setDirty() {
    dirty = true;
}
inline void NotsqlBufferBlock::cleanDirty() {
    dirty = false;
}

inline bool NotsqlBufferBlock::isWriten() {
    return writen;
}
inline void NotsqlBufferBlock::setWriten() {
    writen = true;
}
inline void NotsqlBufferBlock::cleanWriten() {
    writen = false;
}

inline int NotsqlBufferBlock::getRecordCount() {
    int * begin = (int *)block;
    return begin[0];
}
inline void NotsqlBufferBlock::setRecordCount(int count) {
    int * begin = (int *)block;
    begin[0] = count;
}

inline time_t NotsqlBufferBlock:: getTime() {
    return used_time;
}
inline void NotsqlBufferBlock:: setTime(time_t new_time) {
    used_time = new_time;
}

inline int NotsqlBufferBlock::getFreeSpaceEnd() {
    int * begin = (int *)block;
    return begin[1];
}
inline void NotsqlBufferBlock::setFreeSpaceEnd(int new_end) {
    int * begin = (int *)block;
    begin[1] = new_end;
}

#endif
