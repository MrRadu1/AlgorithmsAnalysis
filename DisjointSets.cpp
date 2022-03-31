#include <iostream>
#include "Profiler.h"
using namespace std;
Profiler p("Tema 8");

typedef struct Node {

    int key;
    int rank;
    Node* parent;
}Node;

void makeSet(Node* x) {
    x->parent = x;
    x->rank = 0;
}

Node* findSet(Node* x, int* op) {
    (*op)++;
    if (x != x->parent) {
        (*op)++;
        x->parent = findSet(x->parent,op);
    }
    return x->parent;
}


void link(Node* x, Node* y, int* op) {
    (*op)++;
    if (x->rank > y->rank) {
        (*op)++;
        y->parent = x;
    }
    else {
        (*op)++;
        (*op)++;
        x->parent = y;
        if (x->rank == y->rank) {
            y->rank += 1;
            (*op)++;
        }
    }
}


void toUnion(Node* x, Node* y, int* op) {
    link(findSet(x,op), findSet(y,op), op);
}

typedef struct E {
    Node* src;
    Node* dest;
    int w;
}E;

typedef struct G {
    int nrV;
    int nrE;
    Node** vertices;
    E** edges;
}G;

Node* createNode(int key) {
    Node* node =(Node*)malloc(sizeof(Node));
    node->key = key;
    node->parent = nullptr;
    node->rank = 0;
    return node;
}

E* createEdge(Node* src, Node* dest) {
    E* edge = (E*)malloc(sizeof(E));
    edge->src = src;
    edge->dest = dest;
    edge->w = rand() % 30 + 1;
    return edge;
}

G* createGraph(int nrV) {
    G* g = (G*)malloc(sizeof(G));
    int max = nrV * (nrV - 1) / 2;
    g->nrV = nrV;
    if (4 * nrV > max) {
        g->nrE = max;
    }
    else {
        g->nrE = 4 * nrV;
    }

    g->edges = (E**)malloc((g->nrE)*sizeof(E*));
    for (int i = 0; i < g->nrE; i++)
        g->edges[i] = (E*)malloc(sizeof(E));
    g->vertices = (Node**)malloc((g->nrV) * sizeof(Node*));
    for (int i = 0; i < g->nrV; i++)
        g->vertices[i] = (Node*)malloc(sizeof(Node));
    return g;
}

void initGraph(G* graph, int* v) {
    for (int i = 0; i < graph->nrV; i++)
        graph->vertices[i] = createNode(v[i]);
    int k = 0;
    for (int i = 0; i < graph->nrV - 1; i++) {
        if (k == graph->nrV * 4)
            break;
        for (int j = i + 1; j < graph->nrV; j++)
            if (k == graph->nrV * 4)
                break;
            else
                graph->edges[k++] = createEdge(graph->vertices[i], graph->vertices[j]);
    }

}

int compare(const void* a, const void* b) {
    E* x = (E*)a;
    E* y = (E*)b;
    return x->w - y->w;
   
}

E** kruskalMST(G* g, int* k, int* op) {
    E** A = (E**)malloc(sizeof(E*)* g->nrE);
    for (int i = 0; i < g->nrE; i++)
        A[i] = (E*)malloc(sizeof(E));
    for (int i = 0; i < g->nrV; i++)
        makeSet(g->vertices[i]);
    (*k) = 0;
    qsort(g->edges, g->nrE,sizeof(E*), compare);
    for (int i = 0; i < g->nrE; i++) {
        (*op)++;
        if (findSet(g->edges[i]->src, op) != findSet(g->edges[i]->dest, op)) {
            A[(*k)++] = g->edges[i];
            toUnion(g->edges[i]->src, g->edges[i]->dest, op);
        }
    }
    return A;
}

void demo() {
    G* g = createGraph(10);
    int v[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int n = sizeof(v) / sizeof(v[0]);
    initGraph(g, v);
    int op = 0;
    for (int i = 0; i < n; i++)
        makeSet(g->vertices[i]);
    toUnion(g->vertices[0], g->vertices[2],&op);
    toUnion(g->vertices[1], g->vertices[5], &op);
    toUnion(g->vertices[3], g->vertices[8], &op);
    toUnion(g->vertices[4], g->vertices[6], &op);
    toUnion(g->vertices[7], g->vertices[9], &op);

    for (int i=0;i<5;i++)
        cout << findSet(g->vertices[i], &op)->key << '\n';

}

void perf()
{
    int v[10000];
    for (int n = 100; n <= 10000; n += 100)
    {
        Operation operatii = p.createOperation("Op", n);
        G* g = (G*)malloc(sizeof(G));
        FillRandomArray(v, n, 10, 500000, false, UNSORTED);
        g = createGraph(n);
        initGraph(g, v);
        int k = 0;
        int op = 0;
        kruskalMST(g, &k, &op); 
        operatii.count(op);
    }
    p.showReport();
}

int main() {

    demo();
    perf();
}