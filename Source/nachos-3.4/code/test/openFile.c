#include "syscall.h"



void openFile()

{

    char *fileName;
    int createFile;
    
    PrintString("Enter file name to Open: ");
    ReadString(fileName);

    createFile = Open(fileName);
    if (createFile == -1) {
        PrintString("Error can not open file: ");
        PrintString(fileName);
    }
    else if (createFile >= 10){
        PrintString("The number of files allowed to open has reached the limit!!!\n");
    }
    else {
        PrintString("Open succeeded\n");
    }

}

int main()

{

    char n = '1';
    while ( n != '0') {
        openFile();
        PrintString("Do you want to open more\nEnter 1 to open, 0 to close\n ");
        n = ReadChar();
    }

    

    Halt();

}
