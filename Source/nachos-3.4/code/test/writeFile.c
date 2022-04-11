#include "syscall.h"

int main()
{
    char *fileName;
    int createFile = 1;
    int id;
    char *write; 

    PrintString("Enter file name to open: ");
    ReadString(fileName);
    id = Open(fileName);
    if ( id == -1) {
        PrintString("Error can not open file: ");
        PrintString(fileName);
    }
    else {
        PrintString("write file here: ");
        ReadString(write);
        Write(write, 255, id);
    }
    Close(id);

    Halt();
}