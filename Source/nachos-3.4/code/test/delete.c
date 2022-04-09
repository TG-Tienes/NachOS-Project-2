#include "syscall.h"

int main(){
    int check;
    char *fileName;
    
    PrintString("Enter file name: ");
    ReadString(fileName);

    check = Remove(fileName);

    if(check == 1){
        PrintString("\n!!! DELETE ");
        PrintString(fileName);
        PrintString(" SUCCESSFUL !!!\n");
    } 
    else{
         PrintString("\n!!! DELETE ");
        PrintString(fileName);
        PrintString(" UNSUCCESSFUL !!!\n");
    }
    
    Halt();
}