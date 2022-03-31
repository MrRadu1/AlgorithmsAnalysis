/*
Heap sort e un algoritm de sortare care se bazeaza pe o structura heap. Dupa crearea heap ului se interschimba primul element cu ultimul , 
primul fiind cel mai mare . Apoi se aplica heapify pe heap ul ramas , fara ultima pozitie din array. Se repeta aceasta procedura pt toate 
elemenetele din heap. Complexitatea algoritmului este de O(n*log n) si este instabil

QuickSort
Se alege un pivot, dupa care i se gaseste pozitia in sir. Se face quicksort recursiv pe partea stanga si pe partea dreapta a pivotului, toate 
elementele din stanga fiind mai mici iar cele din dreapta mai mari ca si pivotul.
Complexitatea algoritmului este de O(n * log n) si este instabil.
Sunt 2 metode de implementare a quicksort: Lomuto si Hoare.
Ambele raman la aceeasi complexitate dar Hoare foloseste mai putine atribuiri si comparatii.
In best case quicksort ramane la O(n * log n) dar are nu numar aprox injumatatit de operatii.
Totusi, in worst case quicksort ajunge la O(n^2).
Quickselect foloseste aceeasi aflare a index a pivotului si face recursiv aceeasi cautare pana index ul pivotului se afla pe pozitia cautata.
*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include "Profiler.h"

using namespace std;

Profiler p("heap");
#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

void heapify(int v[], int n, int i, int *count)
{
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n)
    {
        (*count)++;
        if (v[l] > v[largest])
            largest = l;
    }

    if (r < n) {
        (*count)++;
        if (v[r] > v[largest])
            largest = r;
    }

    if (largest != i) {
        (*count)++; (*count)++; (*count)++;
        swap(v[i], v[largest]);
        heapify(v, n, largest,count);
    }
}

void heapSort(int arr[], int n)
{
    int count = 0;
    Operation opBu = p.createOperation("heapsort.op", n);
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i, &count);

    for (int i = n - 1; i > 0; i--) {
        opBu.count(3);
        swap(arr[0], arr[i]);
        heapify(arr, i, 0, &count);
    }
    opBu.count(count);
}

int LomutoPartition(int arr[], int low, int high, int *count)
{
    int pivot = arr[high];  
    int i = (low - 1);  
    for (int j = low; j <= high - 1; j++)
    {
        (*count)++;
        if (arr[j] <= pivot)
        {
            i++;   
            if (i != j) {
                (*count)++; (*count)++; (*count)++;
                swap(arr[i], arr[j]);
            }
        }
    }
    (*count)++; (*count)++; (*count)++;
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

int HoarePartition(int arr[], int low, int high, int *count)
{
  
    int pivot = arr[(high+low)/2];
    int i = low - 1, j = high + 1;
    while (true) {
        do {
            i++;
            (*count)++;
        } while (arr[i] < pivot);

        do {
            j--;
            (*count)++;
        } while (arr[j] > pivot);

        if (i >= j)
            return j;
        (*count)++; (*count)++; (*count)++;
        swap(arr[i], arr[j]);
    }
}

void LomutoQuickSort(int arr[], int low, int high, int *count)
{
    if (low < high) {
        int pi = LomutoPartition(arr, low, high, count);
        LomutoQuickSort(arr, low, pi-1, count);
        LomutoQuickSort(arr, pi + 1, high, count);
    }
   
}

void callLomutoQuickSort(int arr[], int low, int high)
{
    Operation opL = p.createOperation("LomutoQuicksort.op", high + 1);
    int count = 0;
    LomutoQuickSort(arr, low, high, &count);
    opL.count(count);
}

void HoareQuickSort(int arr[], int low, int high, int* count)
{
    if (low < high) {

        int pi = HoarePartition(arr, low, high, count);
        HoareQuickSort(arr, low, pi, count);
        HoareQuickSort(arr, pi + 1, high, count);
    }

}

void callHoareQuickSort(int arr[], int low, int high)
{
    Operation opH = p.createOperation("HoareQuicksort.op", high + 1);
    int count = 0;
    HoareQuickSort(arr, low, high, &count);
    opH.count(count);
}

int QuickSelect(int arr[], int l, int r, int k)
{
    
    if (k > 0 && k <= r - l + 1) {
        int count;
        int index = LomutoPartition(arr, l, r, &count);

        if (index - l == k - 1)
            return arr[index];

        if (index - l > k - 1)
            return QuickSelect(arr, l, index - 1, k);

        return QuickSelect(arr, index + 1, r,
            k - index + l - 1);
    }

    return INT_MAX;
}


void average()
{
    int n = STEP_SIZE;
    p.reset("average");
    int v1[MAX_SIZE];
    int v2[MAX_SIZE];
    int v3[MAX_SIZE];
    for ( n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {
        for (int test = 0; test < NR_TESTS; test++) {
           FillRandomArray(v1, n);
           for (int i = 0; i < n; i++) {
               v2[i] = v1[i];
               v3[i] = v1[i];
           }
           heapSort(v1, n);
           callHoareQuickSort(v2, 0, n - 1);
           callLomutoQuickSort(v3, 0, n - 1);
          
        }
    }
    p.divideValues("heapsort.op", NR_TESTS);
    p.divideValues("HoareQuicksort.op", NR_TESTS);
    p.divideValues("LomutoQuicksort.op", NR_TESTS);
    p.createGroup("Comparare sortari", "heapsort.op", "HoareQuicksort.op", "LomutoQuicksort.op");
    p.showReport();
}

void BestVsWorst(int order)
{
    int n = STEP_SIZE;
    p.reset("best vs worst");
    int v1[MAX_SIZE];
    int v2[MAX_SIZE];
    int v3[MAX_SIZE];
    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {
        for (int test = 0; test < NR_TESTS; test++) {
            FillRandomArray(v1, n, 10, 50000, false, order);
            for (int i = 0; i < n; i++) {
                v2[i] = v1[i];
                v3[i] = v1[i];
            }
            callHoareQuickSort(v2, 0, n - 1);
            callLomutoQuickSort(v3, 0, n - 1);

        }
    }
    p.divideValues("HoareQuicksort.op", NR_TESTS);
    p.divideValues("LomutoQuicksort.op", NR_TESTS);
    p.createGroup("Best vs Worst", "HoareQuicksort.op", "LomutoQuicksort.op");
    p.showReport();
}

void demoSortari()
{
    int arr[] = { 12, 11, 13, 5, 6, 7 };
    int n = sizeof(arr) / sizeof(arr[0]);
    callHoareQuickSort(arr, 0, n - 1);
    cout << "Array ul sortat : ";
    for (int i = 0; i < n; ++i)
        cout << arr[i] << " ";
    cout << "\n";
}

void testQuickSelect() {
    int arr[] = {12, -2, -5, 78, 14, 1, 55 };
    int n = sizeof(arr) / sizeof(arr[0]);
    int k = 3;
    cout << "Al " << k << " lea cel mai mic element este " << QuickSelect(arr, 0, n - 1, k);
}

int main()
{
    //testQuickSelect();
    average();
    BestVsWorst(ASCENDING);
}

