#include "syscall.h"

int main(){
    int f1, f2, temp;
    char *buffer;

    // Enter f1's Name
    do{
        PrintString("Enter first file's name: ");
        ReadString(buffer);

        // Open f1
        f1 = Open(buffer);

        if(f1 == -1)
            PrintString("\n!!! ERROR !!! Please Enter file name again\n\n");
    }while(f1 == -1);
    
    // Enter f2's Name
    do{
        PrintString("Enter second file's name: ");
        ReadString(buffer);

        // Open f2
        f2 = Open(buffer);

        if(f2 == -1)
            PrintString("\n!!! ERROR !!! Please Enter file name again\n\n");
    }while(f2 == -1);

    // di chuyen con tro den vi tri thich hop
    Seek(-1, f1), Seek(0, f2);
    
    // doc noi dung file f2 vao buffer, luu so byte da doc vao temp
    temp = Read(buffer, -1, f2);
    if(temp == -1){
        PrintString("\n!!! ERROR !!! Failed to read file f2\n");
    }
    else{
        PrintString("\n!!! CONCATENATE SUCCESSFUL !!!\n");
        Write(buffer, temp, f1);
    }

    Close(f1), Close(f2);
    Halt();
}