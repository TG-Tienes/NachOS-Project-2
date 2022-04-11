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

/*Ham Phu*/
// Close File
void closeFile(int id);

// Get file name from User
char *getFileNameFromUser();

/*Project 1*/
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

/*Project 2*/
void Exception_syscall_Remove();

void Exception_syscall_Create();

void Exception_syscall_Seek();

//open file
void Exception_syscall_OpenFile();

void Exception_syscall_ReadFile();

void Exception_syscall_CloseFile();

void Exception_syscall_WriteFile();
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
                case SC_Create:{
                    Exception_syscall_Create();
                    increaseProgramCounter();
                    break;
                }
                case SC_Remove:{
                    Exception_syscall_Remove();
                    increaseProgramCounter();
                    break;
                }
                case SC_Open:{
                    Exception_syscall_OpenFile();
                    increaseProgramCounter();
                    break;
                }
                case SC_Read:{
                    Exception_syscall_ReadFile();
                    increaseProgramCounter();
                    break;
                }
                case SC_Close:{
                    Exception_syscall_CloseFile();
                    increaseProgramCounter();
                    break;
                }
                case SC_Seek:{
                    Exception_syscall_Seek();
                    increaseProgramCounter();
                    break;
                }
                case SC_Write:{
                    Exception_syscall_WriteFile();
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
    
    // user nhap so nguyen dang chuoi thong qua SynchConsoleIn
    readBytes = ioSynCons->Read(numString, Bytes);

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
    int readBytes;
    const int Bytes = 128;
    char *result = new char[Bytes];
    
    // nhap ky tu - lay so byte tu chuoi da nhap
    readBytes = ioSynCons->Read(result, Bytes);
    
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

    // Truong hop num la so 0
    if(num == 0){
        ioSynCons->Write("0", 1); // print 0 
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

    ioSynCons->Write(numString, totalDigit);

    delete []numString;
    return;
}

// Quest 3.6 - PrintChar
void Exception_sysCall_PrintChar(){
    int charInNum = machine->ReadRegister(4);
    char result = (char)charInNum;

    ioSynCons->Write(&result, 1);

    return;
}

// Quest 3.8 - ReadString
void Exception_syscall_ReadString(){
    int virAddr = machine->ReadRegister(4);
    const int limit = 128;
    int readBytes;
    char *buffer = NULL;
    
    // lay buffer (chuoi) tu vung nho cua nguoi dung
    buffer = User2System(virAddr, limit);
    readBytes = ioSynCons->Read(buffer, limit);
    
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
    char *buffer = NULL;
    
    // lay buffer (chuoi) tu vung nho cua nguoi dung
    buffer = User2System(virAddr, limit);
    
    // Xuat chuoi ra console = ham Write trong lop SynchConsole
    for(int i = 0; buffer[i] != '\0'; ++i)
        ioSynCons->Write(&buffer[i], 1);
    return;
}

// Project 2
// system call create
void Exception_syscall_Create(){
    int result;
    bool createSuccess;
    char *fileName = getFileNameFromUser();

    if(fileName == NULL || strlen(fileName) == 0){
        createSuccess = 0;
        
        DEBUG('a', "\n!!! File's name can't be NULL or empty !!!\n");
    }
    else
        createSuccess = fileSystem->Create(fileName, 0); // Tao file rong (initial size = 0)

    if(createSuccess == 1){
        result = 0;

        DEBUG('a', "\n!!! CREATE FILE SUCCESSFUL !!!\n");
    }
    else{
        result = -1;

        DEBUG('a', "\n!!! CREATE FILE UNSUCCESSFUL !!!\n");
    }
    machine->WriteRegister(2, result);
    
    delete []fileName;
    return;
}

// system call Remove
void Exception_syscall_Remove(){
    int result;
    bool removeSuccess;
    char *fileName = getFileNameFromUser();

    // Chuoi NULL hoac khong co gi --> xoa khong thanh cong
    if(fileName == NULL || strlen(fileName) == 0)
        removeSuccess = 0;
    else{
        int index = oft->fileIndex(fileName);
        
        // truong hop la stdin, stdout
        if(index == 0 || index == 1){ 
            DEBUG('a', "\n!!! Can't remove stdin and stdout !!!\n");
            removeSuccess = 0;
        }
        else{ // Tien hanh remove file
            // close file
            if(index > 1){
                DEBUG('a', "\nCLOSING FILE\n");
                closeFile(index);
            }
            removeSuccess = fileSystem->Remove(fileName); // goi ham remove trong fileSys
        }
    }

    // remove file thanh cong
    if(removeSuccess == 1){
        result = 0;

        DEBUG('a', "\n!!! REMOVE FILE SUCCESSFUL !!!\n");
    }
    else{ // remove file khong thanh cong
        result = -1;

        DEBUG('a', "\n!!! REMOVE FILE UNSUCCESSFUL !!!\n");
    }
    machine->WriteRegister(2, result);
    
    delete []fileName;
    return;
}

// Seek
void Exception_syscall_Seek(){
    int pos = machine->ReadRegister(4), result = -1, length = 0,
        id = machine->ReadRegister(5);

    length = oft->table[id].File->Length();
    if(id < 0 || id >= oft->_numOfFile){
        DEBUG('a', "OpenFileID exceeded file limit");
    }
    else if(id == 0 || id == 1){
        DEBUG('a', "\nCan't Seek stdin and stdout\n");
    }
    else if(oft->isOpen(id) == 0){
        DEBUG('a', "\nFile is not opened\n");
    }
    else if(pos < -1 || pos > length){
        DEBUG('a', "\nSeek position exceeded the limit of this file\n");
    }
    else{
        if(pos == -1)
            pos = length;
        oft->table[id].File->Seek(pos);
        result = pos;
    }

    machine->WriteRegister(2, result);
    return;
}

// open file function
void Exception_syscall_OpenFile()
{
    OpenFile *id;
    int result;
    char *fileName = getFileNameFromUser();

    // tien hanh mo file
    id = fileSystem->Open(fileName);

    if (id == NULL) { 
        result = -1;
        DEBUG('a', "\nFile name not found\n");
    } else 
        for (int i = 2; i < oft->_numOfFile; ++i)
            if (oft->table[i].File == NULL) {
                result = i;
                oft->table[i].File = id;
                oft->table[i].fileName = fileName;
                break;
            }

    machine->WriteRegister(2, result);
    return;
}

//close file function
void Exception_syscall_CloseFile()
{
    int id = machine->ReadRegister(4);

    // tien hanh dong file
    closeFile(id);

    return;
}

void Exception_syscall_ReadFile()
{
    int virAddr = machine->ReadRegister(4),
        size = machine->ReadRegister(5),
        id = machine->ReadRegister(6), readBytes;
    char *buffer = NULL;

    if(size < -1){ // input size am
        DEBUG('a', "\nInvalid Size\n");
        readBytes = -1;
    }
    else if(id == 0 || id == 1){ // Xet TH la stdin, stdout
        DEBUG('a', "\nCan't Read stdin and stdout");
        readBytes = -1;
    }
    else if(id < 0 || id > oft->_numOfFile){ // id vuot qua limit cua openfiletable
        DEBUG('a', "\ninput ID exceeded the limit number of openfile of openfiletable\n");
        readBytes = -1;
    }
    else if (oft->isOpen(id) == 0) {
        DEBUG('a', "\nFile is not Opened\n");
        readBytes = -1;
    }
    else{
        int acctualFileSize = oft->table[id].File->Length(); // File Size

        //size = -1 || size > acctualFileSize -> Doc het nguyen file
        if(size == -1 || size > acctualFileSize)
            size = acctualFileSize;

        // allocate
        buffer = new char[size];

        // tien hanh doc file
        readBytes = oft->table[id].File->Read(buffer, size);

        // tra ve user
        System2User(virAddr, size, buffer);
        delete []buffer;
    }
    machine->WriteRegister(2, readBytes);

    return;
}

void Exception_syscall_WriteFile()
{
    int virAddr = machine->ReadRegister(4),
        size = machine->ReadRegister(5),
        id = machine->ReadRegister(6);

    if(id == 1 || id == 0){ // stdin, stdout
        DEBUG('a', "\nCan't write into stdin, stdout\n");
        return;
    }
    else if(id < 0 || id > oft->_numOfFile){
        DEBUG('a', "\ninput ID exceeded the limit number of openfile of openfiletable\n");
        return;
    }
    else if(oft->isOpen(id) == 0){
        DEBUG('a', "\nThis ID wasn't assigned to any open File\n");
        return;
    }

    const int limit = 128; // gioi han bytes se lay tu vung nho (co the chinh thanh so khac)
    char *buffer = NULL;

    // lay buffer (chuoi) tu vung nho cua nguoi dung
    buffer = User2System(virAddr, limit);
    
    //tien hanh ghi file
    oft->table[id].File->Write(buffer, size);

    return;
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

void closeFile(int id){
    if(id == 2 || id <= 0 || id >= oft->_numOfFile)
        return;

    delete []oft->table[id].File;
    oft->table[id].File = NULL;
    return;
}

char *getFileNameFromUser(){
    int virAddr = machine->ReadRegister(4);
    const int limit = 128; // gioi han bytes se lay tu vung nho (co the chinh thanh so khac)
    char *buffer = NULL;

    // lay buffer (chuoi) tu vung nho cua nguoi dung
    buffer = User2System(virAddr, limit);

    return buffer;
}

