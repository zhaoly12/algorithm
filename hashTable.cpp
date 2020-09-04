#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>

/*
define a hash function
*/
#define N1 53
#define HASHSIZE 41
int hashInt(int key)
{
	return key*N1 % HASHSIZE;	
}
int hashString(char* str)
{
	int result = 0;
	for(;*str;str++)	
		result = (result*31 + (unsigned)*str) % HASHSIZE;
	return result;
}
int hashFloat(float f)
{
	int result = 0;
	char *s = (char*)&f;
	for(int i=0;i<sizeof(f);i++)
		result = (result*31 + (unsigned)*(s+i)) % HASHSIZE;
	return result;
}

struct Node
{
	int key;
	int value;
	Node* next;
	Node(int k, int v, Node* p):
		key(k), value(v), next(p){
		}
};

void putChain(Node** hashTable, int key, int value)
{
	int index = hashInt(key);
	Node* p = NULL;
	if(*(hashTable+index) == NULL)
	{
		p = new Node(key, value, NULL);
		printf("new node(key = %d, value = %d), index = %d\n", key, value, index);	
	}		
	else
	{
		for(p=*(hashTable+index);p->next&&p->key!=key;p=p->next);
		if(p->key!= key)
		{
			p->next = new Node(key, value, NULL);
			*(hashTable + index) = p;
			printf("new node(key = %d, value = %d), index = %d\n", key, value, index);
		}
		else
		{
			p->value = value;
			printf("duplicated key = %d value changed to %d\n", key, value);
		}
	}
} 

int getChain(Node** hashTable, int key)
{
	int index = hashInt(key);
	Node* p = *(hashTable+index);
	if(p)
	{
		while(p->key != key && p->next)
		{
			p = p->next;
		}
		if(p->key == key)
			return p->value;
	}
	return -1;
}

// test
int main()
{
	/*
	for(int i=1000000;i<2000000;i+=50000)
		printf("%d ", hashInt(i));
		
	printf("\n%d\n", hashString("hello World!"));
	printf("\n%d\n", hashString("Mark!"));
	
	printf("%d\n", hashFloat(0.15));
	printf("%d\n", hashFloat(0.13));
	*/
	Node** hashTable = (Node**)malloc(HASHSIZE*sizeof(Node*));
	memset(hashTable, 0, HASHSIZE*sizeof(Node*));
	putChain(hashTable, 22, 10); 
	putChain(hashTable, 21, 1); 
	putChain(hashTable, 2, 0); 
	putChain(hashTable, 24, 5); 
	putChain(hashTable, 1, 8); 
	putChain(hashTable, 22, 12);
	printf("\n%d ", getChain(hashTable, 2)); 
	return 0;
}
