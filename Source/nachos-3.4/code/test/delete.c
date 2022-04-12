#include "syscall.h"

int main()
{
    char *fileName;
    int id;

    PrintString("Enter file name to remove: ");
    ReadString(fileName);
    
    Remove(fileName);

    Halt();
}