#include "syscall.h"

int main()
{
    char *fileName;

    // Nhap ten file
    PrintString("Enter file name to delete: ");
    ReadString(fileName);

    // Tien hanh xoa file
    Remove(fileName);

    Halt();
}