#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
define a TST
*/
typedef struct node
{
	int value;
	char key;
	node* parent;
	node* left;
	node* right;
	node* middle;
}node; 

// initialize the node
node* init()
{
	node* n = (node*)malloc(sizeof(node));
	n->value = -1;
	n->parent = NULL;
	n->left = NULL;
	n->right = NULL;
	n->middle = NULL;
	n->key = 'H';
	return n;
}

// get a value from the TST
// p start from root, d start from 1
node* get(node* p, char* str, int d)
{
	if(!p || !str)
		return NULL;
	if(str[d-1] == p->key)
	{
		if(d == strlen(str))
			return p;
		else
			return get(p->middle, str, d+1);	
	}
	else if(str[d-1] < p->key)
		return get(p->left, str, d);
	else
		return get(p->right, str, d);
}

// add value into TST
// d start from 1, p start from root
void add(node* p, char* str, int value, int d)
{
	node* newNode = init();
	if(p->key == str[d-1])
	{
		if(d == strlen(str))
		{
			p->value = value; 
			return;
		}
		if(!p->middle)
		{
			newNode->key = str[d-1];
			newNode->parent = p;
			p->middle = newNode;
			printf("new mid node %c added!\n", str[d-1]);
		}
		add(p->middle, str, value, d+1);
	}
	else if(p->key > str[d-1])
	{
		if(!p->left)
		{
			newNode->key = str[d-1];
			newNode->parent = p;
			p->left = newNode;
			printf("new left node %c added!\n", str[d-1]);
			if(d == strlen(str))
			{
				newNode->value = value;
				return;	
			}					
		}
		add(p->left, str, value, d);
	}
	else
	{
		if(!p->right)
		{
			newNode->key = str[d-1];
			newNode->parent = p;
			p->right = newNode;
			printf("new right node %c added\n", str[d-1]);
			if(d == strlen(str))
			{
				newNode->value = value;
				return;
			}
		}
		add(p->right, str, value, d);
	}
}

// test
int main()
{
	puts("create TST:");
	char str[1000];
	int value;
	node* root = init();
	// add value into the tree
	while(1)
	{
		puts("add value into the tree:");
		puts("please input string:");
		scanf("%s", str);
		puts("please input value input -1 to terminate");
		scanf("%d", &value);
		if(value == -1)
			break;
		add(root, str, value, 1);	
	}
	
	// get value from the tree
	while(1)
	{
		puts("get value from the tree");
		puts("please input the string, input 'k' to end:");
		scanf("%s", str);
		if(str[0] == 'k')
			break;
		if(get(root, str, 1))
			printf("got %d\n", get(root, str, 1)->value);
		else
			printf("nothing got from the tree!\n");
	}

	return 0;
} 
