#include "syscall.h"



int main()

{

    int id1, id2, size1, size2;
    float percent;
    int count = 0;
    char* fileName1, *fileName2, *inf;

    PrintString("\nPlease enter your origin file name: ");
    ReadString(fileName1);
    id1 = Open(fileName1);
    size1 = FileSize(id1);

    PrintString("\nPlease enter your file name to copy: ");
    ReadString(fileName2);
    id2 = Open(fileName2);
    
    if ( id1 == -1 || id2 == -1 ) {
        PrintString("\n!!! ERROR !!! Can not find this file\n\n");
    } else {
        while ( count <= size1) {
            Read(inf, 64, id1);
            Write(inf, 64, id2);
	        count += 64;
        }
	    PrintString("\nCopy susscessful\n");
    }

    Halt();

}
