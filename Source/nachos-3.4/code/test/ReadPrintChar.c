#include "syscall.h"

int main(){
    char c;

    PrintString("Enter 1 char: ");
    c = ReadChar();

    PrintString("\nEntered char: ");
    PrintChar(c);

    Halt();
}