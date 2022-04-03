#include "syscall.h"

// void swap(int* a, int* b);
// void bubbleSortAsc(int arr[], int n);
// void bubbleSortDesc(int arr[], int n);

int main()
{
    int a[101];
    int n, i, j, choice;
    PrintString("Enter size of array\n");
    n = ReadNum();

    while( n < 0 || n > 100) {
        PrintString("Invalid size\nPlease re-enter ( 0 < n <= 100) n = ");
        n = ReadNum();
    }

    for (i = 0; i < n ; ++i) {
        PrintString("Enter a[");
        PrintNum(i);
        PrintString("] = ");
        a[i] = ReadNum();
    }

    PrintString("Choose mode of sorting: enter 1 to sort ascending, 2 to sort descending\nYour choice: ");
    choice = ReadNum();
    while (choice != 1 && choice != 2) {
        PrintString("Invalid choice!!\n Please re-choice (1/2) : ");
        choice = ReadNum();
    }
    if (choice == 1)
        bubbleSortAsc(a, n);
    else
        bubbleSortDesc(a, n);
    PrintString("\n\nAfter sort:\n");

    for (j = 0; j < n; ++j) {
        PrintNum(a[j]);
        PrintChar(' ');
    }
    
    Halt();
}

void swap(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}
void bubbleSortAsc(int arr[], int n) 
{ 
    int i, j; 
    for (i = 0; i < n-1; i++)     
        for (j = 0; j < n-i-1; j++) 
            if (arr[j] > arr[j+1]) 
                swap(&arr[j], &arr[j+1]); 
} 

void bubbleSortDesc(int arr[], int n) 
{ 
    int i, j; 
    for (i = 0; i < n-1; i++)     
        for (j = 0; j < n-i-1; j++) 
            if (arr[j] < arr[j+1]) 
                swap(&arr[j], &arr[j+1]); 
} 
