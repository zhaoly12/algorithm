#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>

void show(int* array, int num)
{
	for(int i=0;i<num;i++)
		printf("%d ", array[i]);
	printf("\n"); 
}

void DijkstraSort(int* array, int start, int end)
{
	if(start >= end)
		return;
	int l=start,i=start+1,h=end;
	int pivot = array[start], tmp;
	while(i<=h)
	{
		if(array[i] > pivot)
		{
			tmp = array[h];
			array[h] = array[i];
			h--;
			array[i] = tmp;
		}
		else if(array[i] == pivot)
		{
			i++;	
		}
		else
		{
			tmp = array[l];
			array[l] = array[i];
			l++;
			array[i] = tmp;
			i++;
		} 
	}
	DijkstraSort(array, start, l-1);
	DijkstraSort(array, h+1, end); 
} 

void CountSort(int* array , int num, int min, int max)
{
	int* tmp = (int*)malloc((max-min+1)*sizeof(int));
	int i, j, k;
	for(i=0;i<max-min+1;i++) 
		tmp[i] = 0;
	for(j=0;j<num;j++)
	{
		tmp[array[j]-min]++;	
	}
	
	k = 0;
	for(i=0;i<max-min;i++)
	{
		for(j=0;j<tmp[i];j++)
		{
			array[k] = i+min;
			k++;
		}
	}
}

int binarySearch(int* array, int num, int data)
{
	int start = 0, end = num-1, pos;
	while(start <= end)
	{
		pos = (start+end)/2;
		if (array[pos] == data)
			return pos;
		if(array[pos] > data)
			end = pos-1;	
		else
			start = pos+1;
	}
	return -1;	
}

// binary search tree
struct Node
{
	int key;
	int value;
	Node* lChild, *rChild, *parent;
	int num;
	
	Node(int k, int v, Node* l, Node* r, Node* p)
	{
		key = k;
		value = v;
		lChild = l;
		rChild = r;
		parent = p;
		num = 1;
	}
	
	Node(int k, int v, Node* p)
	{
		new (this)Node(k, v, NULL, NULL, p);
	}
};

int size(Node* root)
{
	if(!root)
		return 0;
	else
		return size(root->lChild) + size(root->rChild) + 1; 
}

Node* put(Node* root, int key, int value)
{
	if(root == NULL)
		return new Node(key, value, NULL);
	if(key < root->key)
	{
		root->lChild = put(root->lChild, key, value);
		if(root->lChild->parent == NULL)
			root->lChild->parent = root;	
	}
	else if(key > root->key)
	{
		root->rChild = put(root->rChild, key, value);
		if(root->rChild->parent == NULL)
			root->rChild->parent = root;	
	}
	else
		root->value = value;
	root->num  = size(root->lChild) + size(root->rChild) + 1;
	return root;
}

Node* get(Node* root, int key)
{
	if(root == NULL)
		return NULL;
	if(key < root->key)
		return get(root->lChild, key);
	else if(key > root->key)
		return get(root->rChild, key);
	else
		return root;
}

void printLDR(Node* root)
{
	if(!root)
		return;
	printLDR(root->lChild);
	Node* p = root->parent;
	if(p)
		printf(" %d=>%d(parent:%d=>%d)", root->key, root->value, p->key, p->value);
	else
		printf(" %d=>%d(NULL) ", root->key, root->value);
	printLDR(root->rChild);
}

void printLDRVer2(Node* root)
{
	if(!root)
		return;
	Node* stack[root->num];
	int p = -1;
	Node* n = root;
	printf("\n");
	int flag = 0;
	while(n)
	{
		if(n->rChild && flag == 0)
		{
			p++;
			stack[p] = n->rChild;	
		}
		if(n->lChild && flag == 0)
		{
			p++;
			stack[p] = n;				
			n = n->lChild;
		}
		else
		{
			printf("%d=>%d ", n->key, n->value);
			if(p >= 0)
			{
				if(stack[p] != n->rChild)
					flag = 1;
				else
					flag = 0;
				n = stack[p];
				p--;	
			}
			else 
				n = NULL;	
		} 	
	}
	printf("\n");
}

void showTree(Node* root, int layer)
{
	if(!root)
		return;
	showTree(root->rChild, layer+2);
	int i;
	for(i=0;i<layer;printf("   "),i++);
	printf("%d=>%d\n", root->key, root->value);
	showTree(root->lChild, layer+2);	
} 

Node* min(Node* root)
{
	if(!root)
		return NULL;
	if(!root->lChild)
		return root;
	else
		return min(root->lChild);
}

Node* minNoRecursion(Node* root)
{
	if(!root)
		return NULL;
	Node* n;
	for(n=root;n->lChild;n=n->lChild);
	return n;
}

Node* max(Node* root)
{
	if(!root)
		return NULL;
	Node* n;
	for(n=root;n->rChild;n=n->rChild);
	return n;
}

Node* floor(Node* root, int key)
{
	if(!root)
		return NULL;
	if(root->key == key)
		return root;
	else if(key < root->key)
		return floor(root->lChild, key);
	Node* n = floor(root->rChild, key);
	if(n)
		return n;
	else
		return root;
} 

Node* ceil(Node* root, int key)
{
	if(!root)
		return NULL;
	if(root->key == key)
		return root;
	else if(key > root->key)
		return ceil(root->rChild, key);
	Node* n = ceil(root->lChild, key);
	if(n)
		return n;
	else
		return root;
}

// rank start from 1 to size(root)
// if rank less than 1 or bigger than size(root) you will get NULL
Node* select(Node* root, int rank)
{
	if(!root)
		return NULL;
	int	k = size(root->lChild);
	if(rank <= k)
		return select(root->lChild, rank);
	else if(rank > k+1)
		return select(root->rChild, rank-k-1);
	else
		return root;
}

// find the rank of key if key is less than the least key you will get 0
// if the key is bigger than the biggest key you will get sizeof(root)
// if the key is between a and b but not exist in the array, you will get the rank of key = a
int rank(Node* root, int key)
{
	if(!root)
		return 0;
	if(root->key > key)
		return rank(root->lChild, key);
	else if(root->key < key)
		return size(root->lChild)+1+rank(root->rChild, key);
	else
		return size(root->lChild)+1;
				
}

// delete a node  
void del(Node* root, int key)
{
	if(!root)
		return;
	if(key < root->key)
		del(root->lChild, key);
	else if(key > root->key)
		del(root->rChild, key);
	else
	{
		if(!root->lChild && !root->rChild)
		{
			if(root->parent)
			{
				if(root == root->parent->lChild)
					root->parent->lChild = NULL;
				else
					root->parent->rChild = NULL;
			}
			delete root;
		}
		else if(!root->lChild)
		{
			if(root->parent)
			{
				if(root == root->parent->lChild)
					root->parent->lChild = root->rChild;
				else
					root->parent->rChild = root->rChild;
			}
			root->rChild->parent = root->parent;
			delete root;
		}
		else if(!root->rChild)
		{
			if(root->parent)
			{
				if(root == root->parent->lChild)
					root->parent->lChild = root->lChild;
				else
					root->parent->rChild = root->lChild;
			}
			root->lChild->parent = root->parent;
			delete root;
		}	
		else
		{
			// find the min node of the right child tree to inplace the root node and update all the size data in the right child tree
			Node* n;
			for(n=root->rChild;n->lChild;n->num--,n=n->lChild);
			// update the child of the node's parent. also if the node has a right child update the parent of the right child		
			if(n != root->rChild)
			{
				n->parent->lChild = n->rChild;
				if(n->rChild)
					n->rChild->parent = n->parent;				
			}
			// update parent node
			if(root->parent)
			{
				if(root == root->parent->lChild)
					root->parent->lChild = n;
				else
					root->parent->rChild = n;
			}
			n->parent = root->parent;
			// update the right child
			if(root->rChild != n)
			{
				root->rChild->parent = n;
				n->rChild = root->rChild;	
			}
			// update the left child
			root->lChild->parent = n;
			n->lChild = root->lChild;
			// update size number of father and ancerstors nodes
			if(!n->rChild)
				n->num = n->lChild->num+1;
			else
				n->num = n->lChild->num+n->rChild->num+1;
			Node* p;
			for(p=n->parent;p;p->num=p->num+n->num,p=p->parent);
			delete root;
		}
	}
}


// test
int main()
{
	
	int keys[] = {6,10,0,16,4,22,16,14,12,18,6};
	int values[] = {0,4,2,5,1,5,7,9,30,12,3};
	int size = sizeof(values)/sizeof(values[0]);
	show(keys, size);
	show(values,size);
	Node* root = NULL;
	for(int i=0;i<size;i++)
	{
		root = put(root, keys[i], values[i]);
		printLDR(root);
		printf("\n");
	}	
	printLDRVer2(root); 
	printf("\ntree is shown as following:\n");
	showTree(root, 1);
	printf("\nmin is %d=>%d\n", min(root)->key, min(root)->value);
	printf("max is %d=>%d\n", max(root)->key, max(root)->value);
	int r = 9, key = 1;
	printf("the No.%d node is %d=>%d\n", r, select(root, r)->key, select(root, r)->value);
	printf("key %d is the key of No.%d node\n", key, rank(root, key));
	while(1)
	{
		puts("\ninput a key that you want to delete, input -100 to end:");
		int d;
		scanf("%d", &d);
		if(d == -100)
			break;
		else if(d == root->key)
			puts("root can not be deleted!");
		else
			del(root, d);
		printLDR(root);
		printf("\nnow tree becomes: \n");
		showTree(root, 1);
		printf("\n");
	}
	
	// clear buffer
	while(char buff=getchar()!='\n' && buff != EOF)
		printf("%c", buff); 
	int array[] = {0,4,2,5,1,5,7,9,30,12,15,1,16,3,0,1,3,9,1,5,16,3,2, 15,14,13, 11,16,18,20,24,100,25,26,80};
	int num = sizeof(array)/sizeof(array[0]);
	int min = 0, max = 100;
	puts("original array:");
	show(array, num);
	puts("choose a way you want to use to sort the array:");
	puts("dijkstra sort: 'd', count sort: 'c'");
start:	char choice = getchar();
	switch(choice)
	{
		case 'd':
			DijkstraSort(array, 0, num-1);
			puts("array sorted:");
			show(array, num);
			break;
		case 'c':
			CountSort(array, num, min, max);
			puts("array sorted:");
			show(array, num);
			break; 
		default:
			puts("Error: input 'd' , 'c', to choose algorithm of sorting!");
			while(char buff=getchar()!='\n'&& buff!=EOF)
				printf("%c", buff);
			goto start;
	}
	
	int data;
	while(1)
	{
		puts("input a number that you want to know the index, input -100 to stop:");
		scanf("%d", &data);
		if(data == -100)
			break;
		if(binarySearch(array, num, data) == -1)
			printf("%d is not in the array!\n", data);
		else
			printf("one of the indices of %d is %d\n", data, binarySearch(array, num, data));	
	} 
	
	return 0;
}
