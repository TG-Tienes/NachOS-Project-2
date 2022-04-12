#include "syscall.h"

int main()
{
    int id, size;
    int count = 0;
    char* fileName, *inf;
    PrintString("\nPlease enter your file name: ");
    ReadString(fileName);
    id = Open(fileName);
    if ( id == -1) {
        PrintString("\n!!! ERROR !!! Can not find this file\n\n");
        PrintString(fileName);
    } else {
        size = FileSize(id);
        while ( count <= size) {
            Read(inf, 64, id );
            PrintString(inf);
            count += 64;
        }
    }
    Halt();
}