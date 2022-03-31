/*
Suciu Radu
Algoritmul construieste un hashtable care are implementat operatiile de search si insert, coliziunile fiind tratate prin adresare deschisa,
folosind quadratic_probing.
Am testat pt mai multe cazuri de umplerere a tabelei efortul depus, adica nr de cautari a unui element intr o tabela.
Am observat ca pt elementele ce nu se afla in tabela efortul de cautare este semnificativ mai mare in comparatie cu cele ce se afla in tabela.
De asemenea, efortul creste semnificativ cu cat creste factorul de umplere a tabelei. Pt un factor de umplere scazut(<90%) diferenta nu e foarte
mare, dar creste extrem de mult intre 95% si 99%.
*/

#include <iostream>
#include "Profiler.h"
#include<fstream>

using namespace std;
ofstream f("hash_table.csv");

typedef struct HashTable
{
	int* v;
	int size;
	int used_size;
};

int quadratic_probing(int k, int m, int i)
{
	int poz = k % m;
	if (poz < 0)
		poz += m;
	return (poz%m + 2*i + 3*i * i) % m;
}

void insert_key(int k, HashTable* T)
{
	for (int i = 0; i < T->size; i++)
	{
		int poz = quadratic_probing(k, T->size, i);
		if (T->v[poz] == 0)
		{
			T->v[poz] = k;
			T->used_size++;
			return;
		}
	}
}

HashTable* init_hashtable(int size)
{
	HashTable* h = (HashTable*)calloc(1, sizeof(HashTable));
	h->v = (int*)calloc(size, sizeof(int));
	h->size = size;
	h->used_size = 0;
	return h;
}

int search_key(int k, HashTable* T, int* efort) {
		int i = 0;
		int poz = quadratic_probing(k, T->size, i);
		while (i < T->size && T->v[poz] != 0) {
			(*efort)++;
			i++;
			if (T->v[poz] == k)
				return poz;
			poz = quadratic_probing(k, T->size, i);
		}
	return -1;
}

void print_HashTable(HashTable* T) {
	for (int i = 0; i < T->size; i++)
		if (T->v[i] == 0)
			printf("free ");
		else
			printf("%d ", T->v[i]);
}

void demo()
{
	int size = 31, x;
	HashTable* h = init_hashtable(size);
	int v[31];
	int used_size = 0.95*size;
	FillRandomArray(v, used_size, 1, 100);

	for (int i = 0; i < used_size; i++)
		insert_key(v[i], h);

	printf("HashTable:\n");
	print_HashTable(h);
	printf("\nCheia: %d Pozitia: %d\n", v[15], search_key(v[15],h , &x));
	printf("Cheia: %d Pozitia: %d\n", 303, search_key(303,h ,&x));
}

void tabele()
{
	int size = 10007;
	int v[10007];
	float cazuri[] = { 0.8,0.85,0.9,0.95,0.99 };
	float efort_mediu_gasite[5], efort_maxim_gasite[5], efort_mediu_negasite[5], efort_maxim_negasite[5];
	for (int i = 0; i < 5; i++)
	{
		efort_mediu_gasite[i] = 0;
		efort_mediu_negasite[i] = 0;
		efort_maxim_negasite[i] = 0;
		efort_maxim_gasite[i] = 0;
	}
	
	for (int teste = 0; teste < 5; teste++)
	{
	
		for (int caz = 0; caz < 5; caz++)
		{

			HashTable* h = init_hashtable(size);
			FillRandomArray(v, size, 1, 50000, false, UNSORTED);
			int i = 0;

			while ((float)h->used_size / h->size < cazuri[caz])
			{
				insert_key(v[i],h);
				i++;
			}
			for ( i = 0; i < 3000; i++)
			{
				int efort = 0;
				if (i % 2)
				{
					search_key(v[rand() % h->used_size],h, &efort);
					efort_mediu_gasite[caz] += efort;
					if (efort > efort_maxim_gasite[caz])
						efort_maxim_gasite[caz] = efort;
				}
				else
				{
					
					search_key( v[rand() % h->used_size] + 50000,h, &efort);
					efort_mediu_negasite[caz] += efort;
					if (efort > efort_maxim_negasite[caz])
						efort_maxim_negasite[caz] = efort;
				}
			}
			free(h->v);
			free(h);
		}
	
	}

	for (int i = 0; i < 5; i++)
	{
		efort_mediu_negasite[i] /= (1500 * 5);
		efort_mediu_gasite[i] /= (1500 * 5);
	}

	printf("Caz  | Efort mediu gasite | Efort maxim gasite | Efort mediu negasite | Efort maxim negasite\n");
	for (int i = 0; i < 5; i++)
	{
		printf("%-5.2f| %-19.2f| %-19.0f| %-21.2f| %.0f\n", cazuri[i], efort_mediu_gasite[i], efort_maxim_gasite[i], efort_mediu_negasite[i], efort_maxim_negasite[i]);
	}

	f << "Caz" << ',' << "avg_found" << ',' << "max_found" << ',' << "avg_not_found" << ',' << "max_not_found" << endl;

	for (int i = 0; i < 5; i++)
		f << cazuri[i] << ',' << efort_mediu_gasite[i] << ',' << efort_maxim_gasite[i] << ',' << efort_mediu_negasite[i] << ',' << efort_maxim_negasite[i] << endl;

	f.close();
}

int main() {
	demo();
	tabele();
	return 0;
}