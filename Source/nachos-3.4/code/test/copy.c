#include "syscall.h"



int main()

{

    int id1, id2, size1, size2;
    float percent;
    int count = 0, i;
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
            i = Read(inf, 64, id1);
            Write(inf, i, id2);
	        count += i;
        }
	    PrintString("\nCopy susscessful\n");
	Close(id1);
    	Close(id2);
    }

   
    Halt();

}
