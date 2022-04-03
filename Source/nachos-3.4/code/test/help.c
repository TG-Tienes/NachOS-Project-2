#include "syscall.h"

int main()
{
    PrintString("Project NachOS 1\nThanh vien nhom: \n");
    PrintString("20127643 - Truong Gia Tien\n20127644 - Nguyen Van Viet\n20127669 - Ngo Anh Vu\n");
    
    PrintString("Command: \n1.Read and print string: ./userprog/nachos -rs 1023 -x ./test/ReadPrintString");
    PrintString("\n2. Read and print char: ./userprog/nachos -rs 1023 -x ./test/ReadPrintChar");
    PrintString("\n3. Read and print number (int type): ./userprog/nachos  -rs 1023  -x  ./test/ReadPrintNum");
    PrintString("\n4. Random number: ./userprog/nachos -rs 1023 -x ./test/randomNum");
    PrintString("\n5. Sort: ./userprog/nachos -rs 1023 -x ./test/sortArray");
    PrintString("\n6. Print ASCII: ./userprog/nachos -rs 1023 -x ./test/PrintASCII");
    PrintString("\n7. Help: ./userprog/nachos -rs 1023 -x ./test/help");

    PrintString("\n\n SORT and PRINT ASCII: \n");

    //
    PrintString("---ASCII---\ncau lenh goi: ./userprog/nachos -rs 1023 -x ./test/PrintASCII");
    PrintString("\nOutput: Bang ma ASCII cua cac ky tu co decimal chay tu 33 den 127\n");
    PrintString("Thiet ke bang:\n-Cot 1: Decimal cua ky tu\n-Cot 2: ky tu o dinh dang character\n");

    //
    PrintString("\n\t----SORT----\nHuong dan su dung chuong trinh sort\n");
    PrintString("Su dung cau lenh goi: ./userprog/nachos -x ./test/sortArray\n");
    PrintString("Sau khi goi thanh cong, chuong trinh se yeu cau nhap so luong phan tu cua mang so nguyen\n");
    PrintString("Nhap so luong phan tu ma ban muon (nam trong khoan tu 0 -> 100)");
    PrintString("Sau do chuong trinh se cho ban nhap gia tri cua cac phan tu, ban chi can nhap\n");
    PrintString("Sau khi nhap xong, chuong trinh se cho ban chon 2 che do\n");
    PrintString("Nhap 1 : de sap xep tang dan\nNhap 2 de sap xep giam dan\n");
    PrintString("Sau khi nhap dung che do, chuong trinh se tien hanh sap xep\n");
    PrintString("Va in ra man hinh ket qua mang sau khi da sap xep\n");
    Halt();
}