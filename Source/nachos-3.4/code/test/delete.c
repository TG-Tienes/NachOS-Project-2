#include "syscall.h"

int main()
{
    char *fileName;
    int id;

    PrintString("Enter file name to open: ");
    ReadString(fileName);
    
    Remove(fileName);

    Halt();
}