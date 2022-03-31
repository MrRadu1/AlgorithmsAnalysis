/*
Heapsort e un algoritm de sortare care se foloseste de o structura heap
Heapify – Reconstituie heap, complexitate O(lgn)
Build-Heap -Construieste intreaga structura heap (pe intregul array), prin aplicarea repetata a heapify, deci complexitatea va ajunge la
n/2*lg n deci O(n lg n) din cauza ca nu se aplica heapify pe frunzele "copacului" deci n se imparte la 2
complexitatea algoritmului ramane n log n in toate cazurile.
Un bottom up aproach duce la un nr mai scazut de operatii, dar totusi complexitatea ramane aceeasi
Heapsort nu este stabil
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



void heapify(int v[], int n, int i)
{

    Operation opBu = p.createOperation("bu.op", n);

    int largest = i; 
    int l = 2 * i + 1;
    int r = 2 * i + 2; 

    if (l < n)
    {
        opBu.count();
        if (v[l] > v[largest])
            largest = l;
    }
 
    if (r < n) {
        opBu.count();
        if (v[r] > v[largest])
            largest = r;
    }

    if (largest != i) {
        opBu.count(3);
        swap(v[i], v[largest]);

      
        heapify(v, n, largest);
    }
}

void build_heap_bu(int v[], int n)
{
    int start = (n / 2) - 1;
    
    for (int i = start; i >= 0; i--) {
        heapify(v, n, i);
    }
}

void build_heap_td(int v[], int n)
{

    Operation opTd = p.createOperation("td.op", n);

    for (int i = 2; i <= n; i++)
    {
        int p = i / 2;
        int j = i;
        opTd.count();
        while ((v[j] > v[p]) && (j != 1))
        {
            opTd.count(4);
            swap(v[j], v[p]);
            j = p;
            p = j / 2;
        }
    }
}

void heapsort(int v[], int n)
{
    build_heap_bu(v, n);
    for (int i = n - 1; i >= 0; i--) {
        swap(v[0], v[i]);
    }
    heapify(v, n, 0);
}

void average()
{   
    p.reset("average");
    int v1[MAX_SIZE];
    int v2[MAX_SIZE];
    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {
        for (int test = 0; test < NR_TESTS; test++) {
           FillRandomArray(v1, n);
           for (int i = 0; i < MAX_SIZE; i++)
                v2[i] = v1[i];
           build_heap_td(v1, n);
           build_heap_bu(v2, n);
        }
    }
    p.divideValues("bu.op", NR_TESTS);
    p.divideValues("td.op", NR_TESTS);
    p.createGroup("Comparare td vs bu","bu.op", "td.op");
    p.showReport();
}

int main()
{
    average();
}
