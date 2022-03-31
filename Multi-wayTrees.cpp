/*Algoritmul realizeaza trecerea dintr un vector de tati intr un arbore multicai in complexitate O(n) iar apoi la o schema de arbore binar sub urmatoarea
forma : primul copil este reprezentat ca first child iar restul sunt legati unul de altul printr o legatura denumita Brother.
Complexitatea ajunge tot la O(n) si in acest caz, algoritmul fiind realizat recursiv.
Sunt implementate si 3 prettyprint-uri pentru fiecare forma a arborelui.*/

#include <iostream>
#include <stdlib.h>

using namespace std;

typedef struct NodR2 {
    int key;
    int nrCh;
    struct NodR2** children;
} NodeR2;

typedef struct NodR3 {
    int key;
    struct NodR3* Child;
    struct NodR3* Sibling;
}NodeR3;


NodeR2* Tr1(int v[],int size)
{
    int rootR2;
    NodeR2** R2=(NodeR2**)malloc(size*sizeof(NodeR2*));
    

    for (int i = 0; i < size; i++)
    {
        R2[i] = (NodeR2*)malloc(sizeof(NodeR2));
        R2[i]->key = i+1;
        R2[i]->nrCh = 0;
        R2[i]->children = (NodeR2**)malloc((size-1) * sizeof(NodeR2*));
        for (int j = 0; j < 9; j++)
            R2[i]->children[j] = (NodeR2*)malloc(sizeof(NodeR2));

    }

    for (int i = 0; i < size; i++)
        if (v[i]!= -1)
        {
            R2[v[i]-1]->children[(R2[v[i]-1]->nrCh)++] = R2[i];
        }

        else rootR2 = i;

    return R2[rootR2];
}

NodeR3* Tr2(NodeR2* x)
{
    NodeR3* last = (NodeR3*)malloc(sizeof(NodeR3));
    last->Sibling = NULL;
    NodeR3* result = (NodeR3*)malloc(sizeof(NodeR3));
    result->key = x->key;
    result->Child = NULL;
    result->Sibling = NULL;
    if (x->nrCh > 0) {
        result->Child = Tr2(x->children[0]);
        last = result->Child;
        for (int j = 1; j < x->nrCh; j++) {
            last->Sibling = Tr2(x->children[j]);
            last = last->Sibling;
        }
    }
    return result;

}

void printR1(int* v, int n, int root, int level)
{
    for (int i = 0; i < level; i++)
        printf(" ");
    printf("%d\n", root);
    for (int i = 0; i < n; i++)
    {
        if (v[i] == root) {
            printR1(v, n, i + 1, level + 1);
        }
    }
}

void gasireRadacina(int* v, int n)
{
    int pos_root=-2;
    for (int i = 0; i < n; i++)
        if (v[i] == -1)
            pos_root = i + 1 ;
    printR1(v, n, pos_root, 0);
}

void prettyprintR2(NodeR2* root, int level)
{
    if (root != NULL)
    {
        for (int i = 0; i < level; i++)
            printf(" ");
        printf("%d\n", root->key);

        for (int i = 0; i < root->nrCh; i++)
            prettyprintR2(root->children[i], level + 1);
    }
}

void prettyprintR3(NodeR3* root, int level)
{
    if (root != NULL)
    {
        for (int i = 0; i < level; i++)
            printf(" ");
        printf("%d\n", root->key);
        prettyprintR3(root->Child, level + 1);
        prettyprintR3(root->Sibling, level);
    }
}



int main()
{

    int v[] = {2,7,5,2,7,7,-1,5,2 };
    int size = sizeof(v) / sizeof(v[0]);
    gasireRadacina(v, size);
    printf("\n");
    NodeR2* root = (NodeR2*)malloc(sizeof(NodeR2));
    root = Tr1(v,size);
    prettyprintR2(root, 0);
    printf("\n");
    NodeR3* root2 = (NodeR3*)malloc(sizeof(NodeR3));
    root2 = Tr2(root);
    prettyprintR3(root2, 0);
}