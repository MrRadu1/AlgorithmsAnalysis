/*
Suciu Radu
Algoritmul are ca scop interclasarea unui nr de k liste sortate.
Pentru o varianta eficienta am ales un algoritm care insereaza toate "head-urile" listelor intr un MinHeap. Se introduce cel mai mic element
in lista finala si se adauga in heap urmatorul element din lista ce detine nodul pus in lista finala.
In analiza algoritmului am observat 2 cazuri:
1.Cand k e fix si n variaza, algoritmul are o Complexitate liniara de O(alfa) unde alfa e produsul dintre n , nr elementelor, si log k , unde 
k e nr listelor sortate
2.Cand n e fix si k variaza, algoritmul tinde spre o complexitate logaritmica

In analiza graficelor listele au aceeasi marime de n/k;
pt demo am folosit un algoritm care genereaza listele de o marime random pt fiecare lista
*/
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <algorithm>
#include "Profiler.h"

Profiler p("Merge K Lists");

using namespace std;

typedef struct NodeT
{
	struct NodeT* next;
	int val;
}NodeT;

typedef struct Lista
{
	NodeT* first;
	NodeT* last;
}Lista;

void MinHeapify(NodeT* a[], int i, int n,int* op)
{

	int Left = i * 2 + 1;
	int Right = i * 2 + 2;
	int largest = i;

	if (Left < n) {
		(*op)++;
		if (a[Left]->val < a[i]->val)

		largest = Left;
	}
	else largest = i;

	if (Right < n)
	{
		(*op)++;
		if (a[Right]->val < a[largest]->val)
			largest = Right;
	}
	if (largest != i)
	{
		NodeT* aux = a[i];
		a[i] = a[largest];
		a[largest] = aux;
		(*op)++; (*op)++; (*op)++;
		MinHeapify(a, largest, n,op);
	}
}

void BuildMinHeap_BU(NodeT* a[], int n, int *op)

{

	for (int i = n / 2 - 1; i >= 0; i--)
	{
		MinHeapify(a, i, n,op);
	}
}

NodeT* alloc_node(int val)
{
	NodeT* p = (NodeT*)malloc(sizeof(NodeT));
	if (p)
	{
		p->next = NULL;
		p->val = val;

	}
	return p;
}

void insert_last(NodeT** first, NodeT** last, int givenkey)
{
	NodeT* p;
	p = alloc_node(givenkey);
	if (*last == NULL)
	{
		*first = p;
	}
	else (*last)->next = p;
	*last = p;

}
void afiseaza_lista(NodeT* q)
{
	while (q != NULL)
	{
		printf("%d ", q->val);
		q = q->next;
	}
	printf("\n");
}

Lista* KwayMerge(Lista* a[], int k,int* op)
{
	Lista* Lout = (Lista*)malloc(sizeof(Lista));
	Lout->first = NULL;
	Lout->last = NULL;
	NodeT** heap = (NodeT**)malloc(k * sizeof(NodeT*));
	for (int i = 0; i < k; i++)
	{
		heap[i] = (NodeT*)malloc(sizeof(NodeT));
		heap[i]->next = NULL;
		heap[i]->val = 0;
	}
	for (int i = 0; i < k; i++)
	{
		heap[i] = a[i]->first;
		(*op)++;
	}


	BuildMinHeap_BU(heap, k, op);


	int heapsize = k;
	while (heapsize > 0)
	{
		NodeT* x = (NodeT*)malloc(sizeof(NodeT));
		x->next = NULL;
		x->val = 0;
		x = heap[0];

		heap[0] = heap[0]->next;
		(*op)++; (*op)++;
		insert_last(&Lout->first, &Lout->last, x->val);
		(*op)++;
		if (heap[0] == NULL)
		{
			(*op)++;
			heap[0] = heap[heapsize - 1];
			heapsize--;
		}
		MinHeapify(heap, 0, heapsize, op);

	}
	return Lout;

}

int random(int* left, int i, int k ) {
	int nr;
	if (i == k - 1)
	{
		nr = (*left);
	}
	else {
		int x = rand();
		x %= (*left);	
		if (x > (*left) / 2)
		{
			x = rand();
			x %= (*left);
		}
		if ((*left) - x < k - i -1)
			x -= k - i-1;
		if (x == 0)
			x++;
		(*left) -= x;
		nr = x;
	}
	return nr;
}

Lista** generareListe(int k, int n)
{
	Lista** a = (Lista**)malloc(k * sizeof(Lista*));
	for (int i = 0; i < k; i++)
	{
		int nr = n/k;
		a[i] = (Lista*)malloc(sizeof(Lista));
		a[i]->first = NULL;
		a[i]->last = NULL;
		int* v = (int*)malloc(nr * sizeof(int));
		FillRandomArray(v, nr, 10, 1000, false, ASCENDING);
		for (int j = 0; j < nr; j++) {
			insert_last(&a[i]->first, &a[i]->last, v[j]);
		}
	}
	return a;
}

Lista** generareListeRand(int k, int n)
{
	Lista** a = (Lista**)malloc(k * sizeof(Lista*));
	for (int i = 0; i < k; i++)
	{
		int nr = random(&n,i,k);
		a[i] = (Lista*)malloc(sizeof(Lista));
		a[i]->first = NULL;
		a[i]->last = NULL;
		int* v = (int*)malloc(nr * sizeof(int));
		FillRandomArray(v, nr, 10, 1000, false, ASCENDING);
		for (int j = 0; j < nr; j++) {
			insert_last(&a[i]->first, &a[i]->last, v[j]);
		}
		printf("Lista %d:\n", i + 1);
		afiseaza_lista(a[i]->first);
	}
	return a;
}

void demo(int k, int n)
{
	printf("Listele sortate:\n");
	Lista** a = (Lista**)malloc(k * sizeof(Lista*));
	a = generareListeRand(k, n);
	Lista* list = (Lista*)malloc(sizeof(Lista));
	int op = 0;
	list = KwayMerge(a, k,&op);
	printf("Lista finala:\n");
	afiseaza_lista(list->first);
}

void grafice()
{
	int k, op;
	
	for (int teste = 0; teste < 5; teste++)
	{
		k = 5;
		for (int n = 100; n <= 10000; n += 100)
		{
			op = 0;
			Operation OpAttrK5 = p.createOperation("Operatii k=5", n);
			Lista** a = (Lista**)malloc(k * sizeof(Lista*));
			a = generareListe(k, n);
			Lista* list = (Lista*)malloc(sizeof(Lista));
			list = KwayMerge(a, k,&op);		
			OpAttrK5.count(op);
		}
		
		k = 10;
		for (int n = 100; n <= 10000; n += 100)
		{
		
			Operation OpAttrK10 = p.createOperation("Operatii k=10", n);
			Lista** a = (Lista**)malloc(k * sizeof(Lista*));
			a = generareListe(k, n);
			Lista* list = (Lista*)malloc(sizeof(Lista));
			int op = 0;
			list = KwayMerge(a, k, &op);
			OpAttrK10.count(op);
		} 
		
		k = 100;
		for (int n = 100; n <= 10000; n += 100)
		{
			Operation OpAttrK100 = p.createOperation("Operatii k=100", n);
			Lista** a = (Lista**)malloc(k * sizeof(Lista*));
			a = generareListe(k, n);
			Lista* list = (Lista*)malloc(sizeof(Lista));
			int op = 0;
			list = KwayMerge(a, k, &op);
			OpAttrK100.count(op);
		}
		
		int n = 10000;
		for (int k = 10; k <= 500; k += 10)
		{
			Operation OpAttrN = p.createOperation("Operatii n=10000", k);
			Lista** a = (Lista**)malloc(k * sizeof(Lista*));
			a = generareListe(k, n);
			Lista* list = (Lista*)malloc(sizeof(Lista));
			int op = 0;
			list = KwayMerge(a, k, &op);
			OpAttrN.count(op);
		}
	}
	
	p.divideValues("Operatii n=10000", 5);
	p.divideValues("Operatii k=5", 5);
	p.divideValues("Operatii k=10", 5);
	p.divideValues("Operatii k=100", 5);
	p.createGroup("Operatii", "Operatii k=5", "Operatii k=10", "Operatii k=100");
	p.showReport();
}

int main()
{
	demo(5,35);
	//grafice();
	return 0;

}