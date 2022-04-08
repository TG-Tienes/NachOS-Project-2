#include "syscall.h"

int main()
{
    char *fileName;
    int createFile = 1;
    int id;

    PrintString("Enter file name to read: ");
    ReadString(fileName);
    id = Open(fileName);
    if ( id == -1) {
        PrintString("Error can not open file: ");
        PrintString(fileName);
    }
    else
        PrintString("Se read or write file o day ");
    
    Close(id);

    Halt();
}