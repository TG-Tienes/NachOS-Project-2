#include "syscall.h"

int main()
{
    int result;

    PrintString("Enter number (int type): ");
    result = ReadNum();

    PrintString("\nEntered number: ");
    PrintNum(result);
    
    Halt();
}
