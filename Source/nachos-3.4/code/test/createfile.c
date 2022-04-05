#include "syscall.h"

int main()
{
    char *fileName = NULL;
    int createFile = 1;

    PrintString("Enter file name: ");
    fileName = ReadString();
    
    createFile = Create(fileName);
    if(createFile == 0){
        PrintString("Create file ");
        PrintString(fileName);
        PrintString("Successful !!!\n");
    }
    else{
        PrintString("Create file ");
        PrintString(fileName);
        PrintString("Unsuccessful !!!\n");
    }
    
    Halt();
}