#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
define a trie
*/
typedef struct node
{
	int value;
	node** childs;
	node* parent;	
} node;

node* init()
{
	node* n = (node*)malloc(sizeof(node));
	n->value = -1;
	n->childs = (node**)malloc(26*sizeof(node*));
	for(int i=0;i<26;i++)
		n->childs[i] = NULL;
	n->parent = NULL;
	return n; 
}

// input a string as a key and get a value
// p start from root node, d start from 1
node* get(node* p, char* str, int d)
{
	if(!p)
		return NULL;
	if(!str)
		return NULL;
	if(d == strlen(str))
		return p->childs[str[d-1]-'A'];
	else
		return get(p->childs[str[d-1]-'A'], str, d+1);
}

// put a key=>value into the tree, update the value if the string already exists 
// p start from root node, d start from 1 
void add(node* root, node* p, char* str, int value, int d)
{
	if(get(root, str, 1))
	{
		printf("[update]: old %s=>%d new %s=>%d\n", str, get(root, str, 1)->value, str, value);
		get(root,str,1)->value = value;
		return;	
	}  
	if(!p)
	{
		puts("root can not be a NULL!");
		return;
	}
	node* newNode;
	if(p->childs[str[d-1]-'A']) 
		newNode = p->childs[str[d-1]-'A'];
	else
	{
		newNode = init();
		newNode->parent = p;
		p->childs[str[d-1]-'A'] = newNode;	
	}
	if(d == strlen(str))
	{
		newNode->value = value;
		return;
	}
	add(root, newNode, str, value, d+1);
}

// show the trie
void show(node* root, int layer)
{
	if(!root)
		return;
	int i;
	for(i=0;i<13;i++)
		show(root->childs[i], layer+1);

	for(i=0;i<layer;printf("   "),i++);
	if(!(root->parent))
		printf("root");
	else
	{
		for(i=0;root->parent->childs[i]!=root;i++);
		printf("%c", 'A'+i);
		if(root->value != -1)	
			printf("=>%d", root->value);	
	}
	printf("\n");

	for(i=13;i<26;i++)
		show(root->childs[i], layer+1);
}


// test
int main()
{
	// create a trie
	puts("create a trie");
	node* root = init();
	char str[1000];
	int value;
	while(1)
	{
		puts("please input the string(all characters should be in capital!):");
		scanf("%s", str);
		puts("please input the value, input -1 to terminate:");
		scanf("%d", &value);
		if(value == -1)
			break;
		add(root, root, str, value, 1);
		puts("trie changed!");
		show(root, 1);
	}
	
	// get a value from the trie
	while(1)
	{
		puts("get a value from the trie using a string");
		puts("please input the string, input 'k' to terminate");
		scanf("%s", str);
		if(str[0] == 'k')
			break;
		if(get(root, str, 1))
			printf("got %d\n", get(root, str, 1)->value);	
		else
			printf("nothing got from the trie!\n");
	} 
	
	return 0;
} 
