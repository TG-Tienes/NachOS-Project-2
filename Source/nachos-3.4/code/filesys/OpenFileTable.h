#include "openfile.h"

#define MAX_NUM_OF_FILE 10

struct FileInfor 
{
	OpenFile* File;
	char* fileName;
};

class OpenFileTable{
public:
	FileInfor *table;
	int _numOfFile;
public:
	// Init
	OpenFileTable(int numOfFile);

	// Kiem tra input OpenFileID co nam trong table openfileID
	bool isOpen(int id);

	// Kiem tra input fileName co thuoc table hay khong(ten file cua cac file trong table)
	int fileIndex(char *name);

	~OpenFileTable();
};
