// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

#include "synchcons.h"
#include "synch.h"
#include "filesys.h"

FileSystem fs;

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

// System to User
int System2User(int virtAddr,int len,char* buffer);

// User to System
char* User2System(int virtAddr,int limit);

// Increase PC
void increaseProgramCounter();

// Syscalls Quest 3.3 -> 3.9
// Quest 3.3 - ReadNum
void Exception_sysCall_ReadNum();

// Quest 3.4 - PrintNum
void Exception_sysCall_PrintNum();

// Quest 3.5 - ReadChar
void Exception_sysCall_ReadChar();

// Quest 3.6 - PrintChar
void Exception_sysCall_PrintChar();

// Quest 3.8 - ReadString
void Exception_syscall_ReadString();

// Quest 3.9 - PrintString
void Exception_syscall_PrintString();

// Quest 3.7 - RandomNum
void Exception_syscall_randIntNum();


// Project 2
void Exception_syscall_Create();

/* EXCEPTION HANDLER */
void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

    switch(which){
        case NoException:
            break;
        case SyscallException:{
            switch(type){
                case SC_Halt:{
                    DEBUG('a', "\n Shutdown, initiated by user program.\n");
                    printf("\n\nShutdown, initiated by user program.\n");
                    interrupt->Halt();
                    break;
                }
                case SC_Sub:{
                    int op1 = machine->ReadRegister (4);
                    int op2 = machine->ReadRegister (5);
                    int result = op1 - op2;
                    machine->WriteRegister (2, result);
                    increaseProgramCounter();
                    break;
                }
                case SC_ReadNum:{
                    Exception_sysCall_ReadNum();
                    increaseProgramCounter();
                    break;
                }
                case SC_PrintNum:{
                    Exception_sysCall_PrintNum();
                    increaseProgramCounter();
                    break;
                }
                case SC_ReadChar:{
                    Exception_sysCall_ReadChar();
                    increaseProgramCounter();
                    break;
                }
                case SC_RanNum:{
		            Exception_syscall_randIntNum();
                    increaseProgramCounter();
                    break;
                }
                case SC_PrintChar:{
                    Exception_sysCall_PrintChar();
                    increaseProgramCounter();
                    break;
                }
                case SC_ReadString:{
                    Exception_syscall_ReadString();
                    increaseProgramCounter();
                    break;
                }
                case SC_PrintString:{
                    Exception_syscall_PrintString();
                    increaseProgramCounter();
                    break;
                }
            }
            break;
        }
        // runtime-error exceptions (quest 3.1)
        case PageFaultException:{
            printf("User exception: Page Fault Exception\n");
            break;
        }
        case ReadOnlyException:{
            printf("User exception: Read Only Exception\n");
            break;
        }
        case BusErrorException:{
            printf("User exception: Bus Error Exception\n");
            break;
        }
        case AddressErrorException:{
            printf("User exception: Address Error Exception\n");
            break;
        }
        case OverflowException:{
            printf("User exception: Over flow Exception\n");
            break;
        }
        case IllegalInstrException:{
            printf("User exception: Illegal Instr Exception\n");
            break;
        }
        case NumExceptionTypes: {
            printf("User exception: Num Exception Types\n");
            break;
        }
    }
}

// Quest 3.2 - Increase PC
void increaseProgramCounter(){
    machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
    machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
    machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg) + 4);
}

// Quest 3.7 - RandomNum
void Exception_syscall_randIntNum()
{
    int result, ran1, ran2;
    RandomInit(ran1);
    ran2 = Random();

    if (PCReg % 4 == 0)
        result = ran1 - ran2;
    else if (PCReg % 4 == 1)
        result = ran2 + ran1;
    else if (PCReg % 4 == 2)
        result = ran1 % ran2;
    else
        result = ran1 / ran2;

    if(result < 0)
        result = -result;
    machine->WriteRegister(2, result);
}

// Quest 3.3 - ReadNum
void Exception_sysCall_ReadNum(){
    const int Bytes = 32; // max bytes ma nguoi dung co the nhap Ps: max ky tu char 
    int readBytes; // so byte user da input
    int start = 0; // vi tri bat dau cua chuoi so
    int maxLength = 12; // max cua chuoi Ps: int --> -2147483648 -> +2147483647 --> chuoi co the co max la 12 char (tinh truong hop co '-' so am) Ps: chua vi tri cuoi cung cho '\0'
    char *numString = new char[Bytes]; // chuoi so ma nguoi dung nhap
    long long result = 0; // ket qua chuoi sau khi doi tu char -> int
    SynchConsole ioCons;
    
    // user nhap so nguyen dang chuoi thong qua SynchConsoleIn
    readBytes = ioCons.Read(numString, Bytes);

    // truong hop khong phai so am 
    if(numString[0] != '-'){
        // so duong --> do dai chuoi la 11 chay tu 0 -> 10 (numString[maxLength] = '\0')
        if(readBytes >= maxLength - 1){
            machine->WriteRegister(2, 0);
            return;
        }
        else // maxLength = 12 --> --maxLength
            --maxLength;
    }
    else if(numString[0] == '-'){ // la so am
        if(readBytes >= maxLength){
            machine->WriteRegister(2, 0);
            return;
        }
        else // start = 0 and numString[0] = '-' --> start++;
            start = 1;
    }
    
    for(int i = start; i < maxLength && numString[i] != '\0'; ++i){
        // numString[i] khong phai la ky tu so --> chuoi nhap vao k phai so --> result = 0
        if(numString[i] < '0' || numString[i] > '9'){
            result = 0;
            break;
        }
        result = result * 10 + numString[i] - '0';
    }

    // Truong hop bien
    if(result == 2147483648 && numString[0] == '-'){
        machine->WriteRegister(2, (int)-result);
        return;
    }
    if(result > 2147483647)
        result = 0;
    if(numString[0] == '-')
        result = -result;
    
    machine->WriteRegister(2, (int)result);

    delete []numString;
    return;
}

// Quest 3.5 - ReadChar
void Exception_sysCall_ReadChar(){
    int i = 0;
    int readBytes;
    const int Bytes = 128;
    char *result = new char[Bytes];
    SynchConsole ioCons;
    
    // nhap ky tu - lay so byte tu chuoi da nhap
    readBytes = ioCons.Read(result, Bytes);
    
    // readBytes == 1 --> nhap 1 ky tu
    if(readBytes == 1)
        machine->WriteRegister(2, (int)result[0]);
    else{
        // Nhap 2 ky tu tro len
        if (readBytes > 1){
            DEBUG('a', "!!! ERROR !!! Num's values exceeds the max and min value of int type.\n");
            printf("ERROR: The number of input character have passed the limit\n");
            
            delete result;
            ASSERT(false);
        }
        else{ // readbytes = 0 --> khong nhap gi ca
            printf("ERROR: No Char has been entered\n");

            delete result;
            ASSERT(false);
        }
    }
    
    delete []result;
    return;
}

// Quest 3.4 - PrintNum
void Exception_sysCall_PrintNum(){
    long long num = machine->ReadRegister(4);
    int temp;
    int totalDigit = 0;
    int end = 0;
    char *numString = NULL;
    SynchConsole ioCons;

    // Truong hop num la so 0
    if(num == 0){
        ioCons.Write("0", 1); // print 0 
        return;
    }
    else if(num < 0){ // kiem tra xem Num la so am hay duong
        numString = new char[12];
        numString[0] = '-';
        end = 1, num = -num, ++totalDigit;
    }
    else // so duong --> khong co dau '-'
        numString = new char[11];
    
    // dem so chu so co trong num
    temp = num;
    while(temp != 0){
        ++totalDigit;
        temp /= 10;
    }
    
    // Doi tu Num o dang so ra char (can cu vao ASCII)
    for(int i = totalDigit - 1; i >= end; --i){
        numString[i] = (char)((num % 10) + (int)'0');
        num /= 10;
    }

    ioCons.Write(numString, totalDigit);

    delete []numString;
    return;
}

// Quest 3.6 - PrintChar
void Exception_sysCall_PrintChar(){
    int charInNum = machine->ReadRegister(4);
    SynchConsole ioCons;
    char result = (char)charInNum;

    ioCons.Write(&result, 1);

    return;
}

// Quest 3.8 - ReadString
void Exception_syscall_ReadString(){
    int virAddr = machine->ReadRegister(4);
    const int limit = 128;
    int readBytes;
    char *buffer = NULL;
    SynchConsole ioCons;
    
    // lay buffer (chuoi) tu vung nho cua nguoi dung
    buffer = User2System(virAddr, limit);
    readBytes = ioCons.Read(buffer, limit);
    
    int i = 0;
    while (buffer[i] != '\0') {
        i++;
    }
    int result = System2User(virAddr, i, buffer);
    machine->WriteRegister(2, result);
}

// Quest 3.9 - PrintString
void Exception_syscall_PrintString(){
    int virAddr = machine->ReadRegister(4);
    const int limit = 128; // gioi han bytes se lay tu vung nho (co the chinh thanh so khac)
    int readBytes;
    char *buffer = NULL;
    SynchConsole ioCons;

    // lay buffer (chuoi) tu vung nho cua nguoi dung
    buffer = User2System(virAddr, limit);
    
    // Xuat chuoi ra console = ham Write trong lop SynchConsole
    for(int i = 0; buffer[i] != '\0'; ++i)
        ioCons.Write(&buffer[i], 1);
    return;
}

// Project 2
void Exception_syscall_Create(){
    int virAddr = machine->ReadRegister(4);
    const int limit = 128;
    int readBytes;
    char *buffer = NULL;
    SynchConsole ioCons;
    bool result;

    // lay buffer (chuoi) tu vung nho cua nguoi dung
    buffer = User2System(virAddr, limit);
    readBytes = ioCons.Read(buffer, limit);
    
    int i = 0;
    while (buffer[i] != '\0') {
        i++;
    }
    // System2User(virAddr, i, buffer);
    result = fs.Create(buffer, 128);

}

int System2User(int virtAddr,int len,char* buffer){
    if (len < 0) 
        return -1;
    if (len == 0)
        return len;

    int i = 0;
    int oneChar = 0 ;

    do{
        oneChar= (int) buffer[i];
        machine->WriteMem(virtAddr+i,1,oneChar);
        i ++;
    }while(i < len && oneChar != 0);

    return i;
}

char* User2System(int virtAddr,int limit){
    int i;// index
    int oneChar;
    char* kernelBuf = NULL;
    
    kernelBuf = new char[limit +1];//need for terminal string
    
    if (kernelBuf == NULL)
        return kernelBuf;

    memset(kernelBuf,0,limit+1);

    //printf("\n Filename u2s:");
    for (i = 0 ; i < limit ;i++){
        machine->ReadMem(virtAddr+i,1,&oneChar);
        kernelBuf[i] = (char)oneChar;

        //printf("%c",kernelBuf[i]);
        if (oneChar == 0)
            break;
    }

    return kernelBuf;
}
