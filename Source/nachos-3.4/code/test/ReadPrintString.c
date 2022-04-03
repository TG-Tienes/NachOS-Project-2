#include "syscall.h"

int main()
{
    char x[255];
    
    PrintString("Enter your string: ");
    ReadString(x);

    PrintString("\nEntered String: ");
    PrintString(x);

    Halt();
}