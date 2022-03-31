#include <iostream>
#include "Profiler.h"
#include<cstdlib>
#include<fstream>
using namespace std;

int topological_sort[100];
int nTopological=0;
bool hasCycles = false;

typedef struct _Node {
    int key;
    int adjSize;
    struct _Node* adj[300];
    int color;
    int index;
    int lowlink;
    bool onStack;

}Node;

enum {
    COLOR_WHITE = 0,
    COLOR_GRAY,
    COLOR_BLACK
};

void addArc(Node* a, Node* b, int * i)
{
    for (int i = 0; i < a->adjSize; i++)
        if (a->adj[i]->key == b->key) 
            return;
    a->adj[a->adjSize] = b;
    a->adjSize++;
    (*i)++;
}

void dfsVisit(Node* node, int * op) {
    node->color = COLOR_GRAY;
    (*op)++;
    (*op)++;
    for (int i = 0; i < node->adjSize; i++) {
        (*op)++;
        if (node->adj[i]->color == COLOR_WHITE)
            dfsVisit(node->adj[i],op);
        else if (node->adj[i]->color == COLOR_GRAY)
            hasCycles = true;
        
    }
    (*op)++;
    node->color = COLOR_BLACK;
}

void dfs(Node* nodesArray[], int n, int* op) {
    for (int i = 0; i < n; i++) {
        (*op)++;
        nodesArray[i]->color = COLOR_WHITE;
    }
    for (int i = 0; i < n; i++) {
        (*op)++;
        if (nodesArray[i]->color == COLOR_WHITE)
            dfsVisit(nodesArray[i], op);
    }
}

void dfsVisit_demo(Node* node) {
    cout << node->key << " ";
    node->color = COLOR_GRAY;
    for (int i = 0; i < node->adjSize; i++) {
        if (node->adj[i]->color == COLOR_WHITE)
            dfsVisit_demo(node->adj[i]);
        else if (node->adj[i]->color == COLOR_GRAY)
            hasCycles = true;
    }
    node->color = COLOR_BLACK;
    topological_sort[nTopological] = node->key;
    nTopological++;
}

void dfs_demo(Node* nodesArray[], int n) {
    for (int i = 0; i < n; i++) {
        nodesArray[i]->color = COLOR_WHITE;
    }
    for (int i = 0; i < n; i++) {
        if (nodesArray[i]->color == COLOR_WHITE)
            dfsVisit_demo(nodesArray[i]);
    }
    cout << endl << "Topological order: ";
    if (!hasCycles)
        for (int i = nTopological - 1; i >= 0; i--)
            cout << " " << topological_sort[i]<<" ";
    else
        cout << "NO topological sort";
}

Node* tarjanStack[100];
int stSize = 0;
int currentIndex = 0;
 


void tarjanVisit(Node* node) {
    node->index = currentIndex;
    node->lowlink = currentIndex;
    currentIndex++;
    tarjanStack[stSize] = node;
    stSize++;
    node->onStack = true;
    node->color = COLOR_GRAY;
    for (int i = 0; i < node->adjSize; i++) {
        Node* neighb = node->adj[i];
        if (neighb->color == COLOR_WHITE) {
            tarjanVisit(neighb);
            if (neighb->lowlink < node->lowlink)
                node->lowlink = neighb->lowlink;
        }
        else if(neighb->onStack) {
            if (neighb->index < node->lowlink)
                node->lowlink = neighb->index;
        }
    }
    node->color = COLOR_BLACK;
    if (node->index == node->lowlink) {
        while (tarjanStack[stSize - 1] != node) {
            cout << tarjanStack[stSize - 1]->key<<" ";
            tarjanStack[stSize - 1]->onStack = false;
            stSize--;
        }
        cout << tarjanStack[stSize - 1]->key<<" ";
        tarjanStack[stSize - 1]->onStack = false;
        stSize--;
        cout << endl;
    }
}

void tarjan(Node* nodesArray[], int n) {
    for (int i = 0; i < n; i++) {
        nodesArray[i]->color = COLOR_WHITE;
        nodesArray[i]->onStack = false;
    }
    for (int i = 0; i < n; i++)
        if (nodesArray[i]->color == COLOR_WHITE)
            tarjanVisit(nodesArray[i]);
}

void demo() {
    int n = 9;
    Node* nodes[9];
    int q=0;

    for (int i = 0; i < n; i++)
    {
        nodes[i] = (Node*)malloc(sizeof(Node));
        nodes[i]->key = i;
        nodes[i]->adjSize = 0;
    }

    addArc(nodes[0], nodes[3],&q);
    addArc(nodes[3], nodes[2],&q);
    addArc(nodes[2], nodes[1],&q);
    addArc(nodes[1], nodes[0],&q);
    addArc(nodes[1], nodes[5],&q);
    addArc(nodes[5], nodes[8],&q);
    addArc(nodes[8], nodes[5],&q);
    addArc(nodes[5], nodes[1],&q);
    addArc(nodes[4], nodes[1],&q);
    addArc(nodes[7], nodes[6],&q);

    cout << "The graph after applying dfs is:" << endl;
    dfs_demo(nodes, n); 
    cout <<endl;


    cout << "The strongly connected components of the graph are:" << endl;
    tarjan(nodes, n);
    cout << endl;

    n = 6;
    Node* nodes1[6];


    for (int i = 0; i < n; i++)
    {
        nodes1[i] = (Node*)malloc(sizeof(Node));
        nodes1[i]->key = i;
        nodes1[i]->adjSize = 0;
    }

    addArc(nodes1[5], nodes1[0],&q);
    addArc(nodes1[0], nodes1[1],&q);
    addArc(nodes1[1], nodes1[2],&q);
    addArc(nodes1[1], nodes1[3],&q);
    addArc(nodes1[1], nodes1[4],&q);
    nTopological = 0;
    hasCycles = false;
    cout << "The graph after applying dfs is:" << endl;
    dfs_demo(nodes1, n);
    cout << endl;


    cout << "The strongly connected components of the graph are:" << endl;
    tarjan(nodes1, n);
    cout << endl;
}

void performance() {
    int n;
    Profiler p("bfs");
    srand(time(NULL));
    int nrNodes = 100;
    Node* nodes[100];
    for (int i = 0; i < nrNodes; i++)
    {
        nodes[i] = (Node*)malloc(sizeof(Node));
        nodes[i]->key = i;
        nodes[i]->adjSize = 0;
    }
    int i = 0;
    for (n = 1000; n <= 4500; n += 100) {
        Operation op = p.createOperation("bfs-edges", n);   
        while (i < n)
        {
            int a = rand() % nrNodes;
            int b = rand() % nrNodes;
            if (a != b)
            {
                addArc(nodes[a], nodes[b],&i);
                i++;
            }
        }
        int op_c = 0;
        dfs(nodes, nrNodes, &op_c);
        op.count(op_c);
    }
    int nrEdges = 4500;
    for (n = 100; n <= 200; n += 10) {
        Node** nodes1=(Node**)malloc(sizeof(Node*)*n);
       for (int i = 0; i < n; i++)
        {
            nodes1[i] = (Node*)malloc(sizeof(Node));
            nodes1[i]->key = i;
            nodes1[i]->adjSize = 0;
        }
        int i = 1;
        while (i < n)
        {
            int x = rand() % i;
            addArc(nodes1[x], nodes1[i],&i);
            
        }
        Operation op2 = p.createOperation("bfs-nodes", n);
        while (i < nrEdges)
        {
            int a = rand() % n;
            int b = rand() % n;
            if (a != b)
            {
                addArc(nodes1[a], nodes1[b],&i);
               
            }
        }
        int op_c = 0;
        dfs(nodes1, n, &op_c);
        op2.count(op_c);
    }


    p.showReport();

}

int main() {
    //demo();
    performance();
}
