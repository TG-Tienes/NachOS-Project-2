#include "syscall.h"

int main()
{
    int i = 33;

    PrintString("Dec     Char\n");
    for(; i < 100; ++i){
        PrintNum(i);
        PrintString("  ");
        PrintChar(i);
        PrintChar('\n');
    }
    
    for(; i < 127; ++i){
        PrintNum(i);
        PrintChar(' ');
        PrintChar(i);
        PrintChar('\n');
    }
    
    Halt();
}