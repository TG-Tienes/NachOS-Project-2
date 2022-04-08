#include "syscall.h"

int main()
{
    char *fileName;
    int createFile = 1;

    PrintString("Enter file name: ");
    ReadString(fileName);
    
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