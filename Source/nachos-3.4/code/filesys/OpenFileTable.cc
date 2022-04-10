#include "OpenFileTable.h"

OpenFileTable::OpenFileTable(int numOfFile){
    _numOfFile = numOfFile;

    table = new FileInfor[numOfFile];
    for(int i = 0; i < numOfFile; ++i)
        table[i].File = NULL;
}

OpenFileTable::~OpenFileTable(){
    for(int i = 0; i < _numOfFile; ++i){
        delete table[i].File;
        delete []table[i].fileName;
    }
    delete []table;
}

// Kiem tra input OpenFileID co nam trong table openfileID
bool OpenFileTable::isOpen(int id){
    if(table[id].File == NULL)
        return 0;
    return 1;
}

// Kiem tra input fileName co thuoc table hay khong(ten file cua cac file trong table)
int OpenFileTable::fileIndex(char *name){
    for(int i = 0; i < _numOfFile; ++i)
        if(table[i].fileName == name)
            return i;
    return -1;
}