#include <iostream>
#include <stdlib.h>
#include "Profiler.h"
Profiler p("Lab 8");
using namespace std;

typedef struct Node {
	int size=0;
	int key;
	Node* left;
	Node* right;
}Node;

Node* buildTree(int s, int e)
{
	if (s > e) return NULL;
	int mid = (s + e) / 2;
	Node* node = (Node*)malloc(sizeof(Node));
	node->key = mid;
	node->size = e - s + 1;
	node->left = buildTree(s, mid - 1);
	node->right = buildTree(mid + 1, e);
	return node;
}

void prettyPrint(Node* p, int nivel)
{
	if (p != NULL)
	{
		prettyPrint(p->right, nivel + 1);
		for (int i = 0; i < nivel; i++)
			printf("   ");
		printf("%d\n", p->key);

		prettyPrint(p->left, nivel + 1);
	}
}

Node* OS_Select(Node* node, int n, int* op) {
	if (node == NULL)
		return NULL;

	int size;
	if (node->left == NULL) {
		size = 1;
	}
	else {
		size = node->left->size + 1;
	}
	(*op)++;
	if (size == n) 
		return node;
	(*op)++;

	 if (n < size) 
		OS_Select(node->left, n, op);
	else
		OS_Select(node->right, n-size,op);
}

Node* OS_Delete(Node* x, int i, int* op)
{	
	int r;

	if (x->left == NULL)
		r = 1;
	else 
		r = x->left->size + 1;
	(*op)++;
	if (i == r) {
		Node* temp = NULL;
		if (x->left == NULL && x->right == NULL)
			x = NULL;
		else if (x->right == NULL)
			(*x) = (*x->left);
		else if (x->left == NULL)
			(*x) =*(x->right);
		else
		{

			Node* succesor = (Node*)malloc(sizeof(Node));
			succesor = OS_Select( x->right, 1,op);
			x->key = succesor->key;
			(*op)++;
			(x->right) = OS_Delete(x->right, 1, op);
			(*op)++;
			(x->size)--;
		}
		
	}
	else {

		(*op)++;
		if (i < r) {
			OS_Delete(x->left, i, op);
			(*op)++;
			(x->size)--;
		}
		else {
			OS_Delete(x->right, i - r, op);
			(*op)++;
			(x->size)--;
		}
	}
	return x;
}

void demo()
{
	Node* root = buildTree(2, 11);
	prettyPrint(root, 0);
	cout << endl; 
	int x = 0;
	printf("\n\n\n%d\n\n\n", OS_Select(root, 3,&x)->key);
	root=OS_Delete(root, 3,&x);
	cout << endl;
	prettyPrint(root, 0);

	printf("\n\n\n%d\n\n\n", OS_Select(root, 2,&x)->key);
	 root = OS_Delete(root, 2 ,&x);
	cout << endl;
	prettyPrint(root, 0);
}

void grafice()
{
	Node* root;
	for (int rounds = 0; rounds < 5; rounds++)
	{
		for (int n = 100; n < 10000; n += 100)
		{
			root = buildTree(1, n);
			int op_search = 0, op_delete = 0;
			for (int i = 0; i < n; i++)
			{
				int index = rand() % (n - i) + 1;
				int key = OS_Select(root, index,&op_search)->key;
				root=OS_Delete(root, index, &op_delete);
			}
			Operation search_op = p.createOperation("Search operatii", n);
			Operation delete_op = p.createOperation("Delete operatii", n);

			search_op.count(op_search);
			delete_op.count(op_delete);

		}
	}
	p.divideValues("Search operatii", 5);
	p.divideValues("Delete operatii", 5);
	p.createGroup("OS", "Search operatii", "Delete operatii");
	p.showReport();
}

int main() {
	//demo();
	grafice();
	return 0;
 }