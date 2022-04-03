#include "syscall.h"
int main()
{
    int result;
    result = RandomNum();

    PrintString("--- RANDOM NUMBER ---\n");
    PrintString("Generated Random num: ");
    PrintNum(result);

    Halt();
}
