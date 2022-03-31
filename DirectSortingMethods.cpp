/*
*Binary Search--- cauta pozitia unui element intr un array prin cautari logaritmice => Complexitate O(log n)
*Insertion Sort -- algoritm de sortare care imparte array ul in 2 parti, sortat si nesortat, dupa care se ia cate un element din partea
nesortata si i se cauta pozitia in partea sortata dupa care se muta toate elemente mai mari cu o pozitie la dreapta, astfel crescand marimea 
vectorului sortat cu 1, pana ajunge vectorul complet sortat. Complexitatea atribuilor e de O(n) iar cea a compararilor e de 0(n^2) dar care 
poate fi redusa prin cautare binara a carei complexitati va ajunge la O(n*log n). Complexitatea totala este data de complexitatea compararilor.
Algoritmul este stabil.
*Selection Sort -- algoritm de sortare care imparte array ul in 2 parti, sortat si nesortat, dupa care se cauta cel mai mic element din partea 
nesortata si se pozitioneaza la finalul sirului sortat printr un swap, crescand marimea array ului sortat cu 1. Complexitatea atribuirilor 
este O(n) iar cea a compararilor este O(n^2). Algoritmul este instabil.
Se poate implementa si un algoritm de selection stabil dar care creste substantial numarul atribuirilor la O(n^2)
*BubbleSort -- algoritm de sortare care schimba  intre ele,pas cu pas, elementele adiacente aflate in ordine descrescatoare. Algoritmul are 
complexitatea de O(n^2) atat in cazul atribuirilor cat si a comparatilor.

Comparand cei 3 algoritmi se observa ca :
---in average case:
bubble e cel mai costisitor atat din punct de vedere al comparatiilor cat si a atribuirilor, deci evident si total.
insertion are mai multe atribuiri iar selection mai multe comparari. Per total, cei 2 algoritmi sunt foarte apropriati ca nr de operatii, dar
totusi un binary insertion ar iesi cel mai eficient.
---in best case:
bubble si selection au 0 atribuiri, dar si insertion are un numar relativ mic. La capitolul comparari, insertion si bubble au numar destul de
scazut , insa selection are un numar ft crescut. In concluzie, selection sort e cel mai costisitor, iar bubble si insertion cam asemanator.
Surprinzator, un binary insertion e mai costisitor decat insertion in best case.
---in worst case:
selection are un nr scazut de atribuiri, insertion cam average, pe cand bubble are un nr extrem de mare de atribuiri. Compararile sunt extrem 
de apropriate la toate cele 3, doar un binary insertion e mai putin costisitor. Per total, selection sort e cel mai bun in worst case, insertion
average iar bubble e worst.
*/





#include <iostream>
#include "Profiler.h"

Profiler p("Sorting");
#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

void swap(int* x, int* y) 
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

int binarySearch(int v[], int x, int low, int high, int *count)
// cauta pozitia unui element intr un array prin cautari logaritmice => Complexitate O(log n)
{
    if (high <= low) {
        (*count)++;
        return (x > v[low]) ? (low + 1) : low; 
    }
    int mid = (low + high) / 2;
    (*count)++;
    if (x == v[mid])
        return mid + 1;
    (*count)++;
    if (x > v[mid])
        return binarySearch(v, x, mid + 1, high, count);

    return binarySearch(v, x, low, mid - 1, count);
}

void BinaryinsertionSort(int a[], int n)
{   
    Operation opComp = p.createOperation("binsertion-comp", n);
    Operation opAttr = p.createOperation("binsertion-attr", n);

    for (int i = 1; i < n; ++i)
    {
        int j = i - 1;
        opAttr.count();
        int key = a[i];
        int count = 0;
        int pos = binarySearch(a, key, 0, j, &count);
        opComp.count(count);
        while (j >= pos)
        {
            a[j + 1] = a[j];
            opAttr.count();
            j--;
        }
        opAttr.count();
        a[j + 1] = key;
    }
}

void insertionSort(int v[], int n)
{
    Operation opComp = p.createOperation("insertion-comp", n);
    Operation opAttr = p.createOperation("insertion-attr", n);
    for (int i = 1; i < n; i++)
    {
        opAttr.count();
        int key = v[i];
        int j = i - 1;
        opComp.count();
        while (j >= 0 && v[j] > key)
        {
            opComp.count();
            opAttr.count();
            v[j + 1] = v[j];
            j = j - 1;
        }
        opAttr.count();
        v[j + 1] = key;
    }
}

void selectionSort(int v[], int n)
{
    Operation opComp = p.createOperation("selection-comp", n);
    Operation opAttr = p.createOperation("selection-attr", n);
    for (int i = 0; i < n - 1; i++)
    {
        int min= i;
        for (int j = i + 1; j < n; j++) {
            opComp.count();
            if (v[j] < v[min])
                min = j;
        }
        if (i != min) {
            opAttr.count(3);
            swap(&v[min], &v[i]);
        }

    }
}

void stableSelectionSort(int v[], int n)
{
    Operation opComp = p.createOperation("stselection-comp", n);
    Operation opAttr = p.createOperation("stselection-attr", n);
    for (int i = 0; i < n - 1; i++)
    {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            opComp.count();
            if (v[min] > v[j])
                min = j;
        }
        opAttr.count();
        int key = v[min];
        while (min > i)
        {
            opAttr.count();
            v[min] = v[min - 1];
            min--;
        }
        opAttr.count();
        v[i] = key;
    }
}
void bubbleSort(int v[], int n){
    Operation opComp = p.createOperation("bubble-comp", n);
    Operation opAttr = p.createOperation("bubble-attr", n);
    int i, j;
    bool swapped;
    for (i = 0; i < n - 1; i++)
    {
        swapped = false;
        for (j = 0; j < n - i - 1; j++)
        {
            opComp.count();
            if (v[j] > v[j + 1])
            {
                swap(&v[j], &v[j + 1]);
                opAttr.count(3);
                swapped = true;
            }
        }
        if (swapped == false)
            break;
    }
}

void tool(int a[], int n)
{

    for (int i = 1; i < n; ++i)
    {
        int j = i - 1;
        int key = a[i];
        int count = 0;
        int pos = binarySearch(a, key, 0, j, &count);
        while (j >= pos)
        {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

void perf(int order) {
    int v[MAX_SIZE];
    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {
        for (int test = 0; test < NR_TESTS; test++) {
            FillRandomArray(v, n,10,50000,false,order);
            int v1[MAX_SIZE];
            for (int i = 0; i < MAX_SIZE; i++)
                v1[i] = v[i];
            BinaryinsertionSort(v, n);
            for (int i = 0; i < MAX_SIZE; i++)
                v[i] = v1[i];
            insertionSort(v, n);
            for (int i = 0; i < MAX_SIZE; i++)
                v[i] = v1[i];
            if (order == DESCENDING)
                tool(v, n - 1);
            selectionSort(v, n);
            for (int i = 0; i < MAX_SIZE; i++)
                v[i] = v1[i];
            stableSelectionSort(v, n);
            bubbleSort(v1, n);

        }
    }
    p.divideValues("binsertion-comp", NR_TESTS);
    p.divideValues("binsertion-attr", NR_TESTS);
    p.addSeries("binsertion", "binsertion-comp", "binsertion-attr");
    p.divideValues("insertion-comp", NR_TESTS);
    p.divideValues("insertion-attr", NR_TESTS);
    p.addSeries("insertion", "insertion-comp", "insertion-attr");
    p.divideValues("selection-comp", NR_TESTS);
    p.divideValues("selection-attr", NR_TESTS);
    p.addSeries("selection", "selection-comp", "selection-attr");
    p.divideValues("stselection-comp", NR_TESTS);
    p.divideValues("stselection-attr", NR_TESTS);
    p.addSeries("stselection", "stselection-comp", "stselection-attr");
    p.divideValues("bubble-comp", NR_TESTS);
    p.divideValues("bubble-attr", NR_TESTS);
    p.addSeries("bubble", "bubble-comp", "bubble-attr");
    p.createGroup("attr", "binsertion-attr", "insertion-attr", "stselection-attr", "selection-attr","bubble-attr");
    p.createGroup("comp", "binsertion-comp", "insertion-comp", "stselection-comp", "selection-comp", "bubble-comp");
    p.createGroup("total", "binsertion", "insertion", "stselection", "selection", "bubble");
}

void demo() {
    int v[] = { 2, 3 , 7 ,2 , 6, 4 };
    BinaryinsertionSort(v, 6);
    for (int i = 0; i < 6; i++)
        printf("%d", v[i]);
}

void perf_all() {
    p.reset("average");
    perf(UNSORTED);
    p.reset("Best");
    perf(ASCENDING);
    p.reset("Worst");
    perf(DESCENDING);
    p.showReport();
    }

int main() {
    //demo();
    perf_all();
}