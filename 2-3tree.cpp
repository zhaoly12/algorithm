#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
define a 2-3 tree
*/
// for a node in balanced tree(leaf nodes excepted): (the number of child nodes) == 1+(the number of keys) 
typedef int dataType; 
#define DISABLE -1
typedef struct Node
{
	int key[3];
	dataType data[3];
	Node* child[4]; 
	Node* parent;
}node;

/*
initialize a node
*/ 
node* init()
{
	node* t = (node*)malloc(sizeof(node));
	t->parent = NULL;
	int i;
	for(i=0;i<4;t->child[i] = NULL,i++);
	for(i=0;i<3;t->key[i] = DISABLE,i++);
	return t;
}

/*
show a 2-3 tree
*/
void show(node* root, int layer)
{
	if(!root)
		return;
	if(root->child[0])
		show(root->child[0], layer+3);
	int i;
	for(i=0;i<layer;printf("   "),i++);
	for(i=0;root->key[i]!= DISABLE;i++)
		printf("%d=>%d ", root->key[i], root->data[i]);
	printf("\n");
	if(root->child[1])
		show(root->child[1], layer+3);
	if(root->child[2])
		show(root->child[2], layer+3);
}

/*
return the depth of a 2-3 tree
*/
int depth(node* root)
{
	if(!root)	
		return 0;
	else
	{
		int d;
		node* p = root;
		for(d=1;p->child[0];p=p->child[0],d++);
		return d;
	}
}

/*
return the number of leaf nodes of a 2-3 tree
*/
int leafNum(node* root)
{
	if(!root)
		return 0;
	else if(!root->child[0])
		return 1;
	else
	{
		int num = 0;
		int i = 0;
		while(root->child[i])
		{
			num = num + leafNum(root->child[i]);	
			i++;
		}	
		return num;
	}	
} 

/*
add a key=>data into a 2-3 tree. if the key is already in the tree, update it
*/
void add(node** pRoot, int key, dataType data)
{
	node* root = *pRoot;
	int i, pos;
	// find the leaf node to insert data
	node* p = root;
	while(p->child[0])
	{
		for(i=0;p->key[i] != DISABLE;i++)
		{
			if(p->key[i] >= key)
				break;	
		}
		// if the key is already in the tree update it
		if(p->key[i] == key)
		{
			printf("[update] old: %d=>%d new: %d=>%d\n", key, p->data[i], key, data);
			p->data[i] = data;
			return;
		}
		// 		
		p = p->child[i];
	}
	int pk = key;
	dataType pv = data;			
	while(p)
	{
		// find the insert position
		for(pos=0;p->key[pos] != DISABLE;pos++)
		{
			if(p->key[pos] >= pk)
				break;
		}
		// if the key is already in the tree update it
		if(p->key[pos] == pk)
		{
			printf("[update] old: %d=>%d new: %d=>%d\n", pk, p->data[pos], pk, pv);
			p->data[pos] = pv;
			return;
		}
		// 
		// check if the node is illegal
		if(p->key[1] == DISABLE) // if the node is legal, just add data into it
		{
			if(pos == 0)
			{
				p->key[1] = p->key[0];
				p->data[1] = p->data[0];
				p->key[0] = pk;
				p->data[0] = pv;
			}
			else
			{
				p->key[1] = pk;
				p->data[1] = pv;
			}
			break;
		}
		else // if the node is illegal do some adjustment
		{
			int tk = pk, tv = pv;
			// find the data to push to parent node 
			if(pos == 0)
			{
				pk = p->key[0];
				pv = p->data[0];
				p->key[0] = tk;
				p->data[0] = tv;
			}
			else if(pos == 2)
			{
				pk = p->key[1];
				pv = p->data[1];
				p->key[1] = tk;
				p->data[1] = tv;
			} 
			// divide the node into two nodes
			node* newNode = init();
			newNode->key[0] = p->key[1];
			newNode->data[0] = p->data[1];
			p->key[1] = DISABLE;
			// build connection between new node and the child nodes also adjust the connection between p and its child nodes
			if(p->child[0])
			{
				newNode->child[0] = p->child[2];
				newNode->child[1] = p->child[3];
				p->child[2]->parent = newNode;
				p->child[3]->parent = newNode;
				p->child[2] = p->child[3] = NULL;
			}
			// build connection between new node and the parent node
			if(p->parent)
			{	
				newNode->parent = p->parent;
				for(pos=0;p->parent->child[pos]!=p;pos++);
				for(i=0;p->parent->child[i];i++);
				for(;i>pos;i--)
					p->parent->child[i] = p->parent->child[i-1];
				p->parent->child[pos+1] = newNode;				
			}
			else// if the node is root node change root
			{
				root = init();
				*pRoot = root;
				root->key[0] = pk;
				root->data[0] = pv;
				root->child[0] = p;
				root->child[1] = newNode;
				p->parent = root;
				newNode->parent = root;						
				break;					
			}
			// go to parent node and check
			p = p->parent;
		}	
	}
}

/*
create a 2-3 tree
*/
void create(node** pRoot)
{
	int k;
	dataType v;
	while(1)
	{
		puts("input key and value in form of 'k v':");
		puts("if k = -1, process will be ended");
		show(*pRoot, 1);
		scanf("%d %d", &k,&v);
		if(k == -1)
		{
			puts("completed!");
			break;
		}
		add(pRoot, k, v);
	}
}

/*
get a data from 2-3 tree using its key
*/
node* get(node* root, int k)
{
	if(!root)
		return NULL;	
	int i = 0;
	while(root->key[i] != DISABLE)
	{
		if(root->key[i] == k)
			return root;
		else if(root->key[i] > k)
		{
			if(root->child[i])
				return get(root->child[i], k);
			else 
				return NULL;	
		}
		else
			i++;
	}
	if(root->child[i])
		return get(root->child[i], k);
	else
		return NULL;
}

/*
traverse a 2-3 tree
*/
void traverse(node* root)
{
	if(!root)
		return;
	if(!root->child[0])
		for(int i=0;root->key[i]!=DISABLE;printf("%d=>%d ",root->key[i],root->data[i]),i++);
	else
	{
		traverse(root->child[0]);
		printf("%d=>%d ", root->key[0], root->data[0]);
		traverse(root->child[1]);		
	}
	if(root->child[2])
	{
		printf("%d=>%d ", root->key[1], root->data[1]);
		traverse(root->child[2]);
	}
} 

// if a node and its parent both have only one key, collect all the leaf nodes and add them again. 
typedef struct leafNode
{
	node* leaf;
	leafNode* next;
}leaf;
leaf* makeNode()
{
	leaf* n = (leaf*)malloc(sizeof(leaf));
	n->next = NULL;
	return n;	
}
leaf* leaves = makeNode();
leaf* leafList = leaves;
void collect(node* root)
{
	if(!root->child[0])
	{
		leaves->leaf = root;
		leaves->next = makeNode();
		leaves = leaves->next;	
		printf("leaf node %d=>%d added\n", root->key[0], root->data[0]);
	}
	else
	{
		collect(root->child[0]);
		collect(root->child[1]);
		if(root->child[2])
			collect(root->child[2]);
	} 
	return;	
}

void transform(node** pRoot, node* child)
{
	// collect all the leaf nodes
	collect(*pRoot);		
	// add them again
	leaf* p = leafList;
	leaf* tmp;
	int i;
	while(p->next)
	{
		if(p->leaf == child)
		{
			p = p->next;
			continue;	
		}
		// clear all the child nodes
		for(i=0;p->leaf->parent->child[i];i++)
			p->leaf->parent->child[i] = NULL;
		// add child nodes again
		for(i=0;p->leaf->key[i] != DISABLE;i++)
		{
			add(pRoot, p->leaf->key[i], p->leaf->data[i]);	
		} 
		tmp = p->next;
		free(p);
		p = tmp;
	}
	// rebuild leafList
	leaves = makeNode();
	leafList = leaves;
}

/*
delete a key=>data from a 2-3 tree
*/
void del(node** pRoot, int key)
{
	node* target = get(*pRoot, key);
	if(!target)
	{
		printf("target key %d not found!\n", key);
		return;	
	}
	// find the key
	int pos;
	for(pos=0;target->key[pos]!=key;pos++);
	// get a key from its child tree to update the key
	int k = target->key[pos];
	dataType v = target->data[pos];
	node* child;
	if(target->child[0])
	{
		for(child = target->child[pos+1];child->child[0];child=child->child[0]);
		target->key[pos] = child->key[0];
		target->data[pos] = child->data[0];
		pos = 0;			
	}
	else
		child = target;
	// delete the key of the leaf node found just now
	if(child->key[1] != DISABLE)
	{
		memcpy((int*)(child->key+pos), (const int*)(child->key+pos+1), (2-pos)*(sizeof(int)));
	}
	else// if the leaf node has only one key delete the node
	{
		if(!child->parent)  
			*pRoot = NULL;
		else
		{
			// find the pos of the node
			for(pos=0;child->parent->child[pos] != child;pos++);		
			if(child->parent->child[pos+1])// if the node has a right sibling
			{
				if(child->parent->child[pos+1]->key[1] == DISABLE)
				{
					if(child->parent->key[1] != DISABLE)// the right sibling has only 1 key but parent has more than 1 key      
					{
						child->parent->child[pos+1]->key[1] = child->parent->child[pos+1]->key[0];
						child->parent->child[pos+1]->data[1] = child->parent->child[pos+1]->data[0];
						child->parent->child[pos+1]->key[0] = child->parent->key[pos];
						child->parent->child[pos+1]->data[0] = child->parent->data[pos];
						memcpy((int*)(child->parent->key+pos), (const int*)(child->parent->key+pos+1), (2-pos)*sizeof(int));
						memcpy((dataType*)(child->parent->data+pos), (const dataType*)(child->parent->data+pos+1), (2-pos)*sizeof(dataType));
						memcpy((node**)(child->parent->child+pos), (const node**)(child->parent->child+pos+1), (3-pos)*sizeof(node*));	
					}
					else
					{ 
						transform(pRoot, child);
					}
				}
				else // if the right sibling has more than 1 key        
				{
					child->key[0] = child->parent->key[pos];
					child->data[0] = child->parent->data[pos];
					child->parent->key[pos] = child->parent->child[pos+1]->key[0];
					child->parent->data[pos] = child->parent->child[pos+1]->data[0];
					memcpy((int*)(child->parent->child[pos+1]->key), (const int*)(child->parent->child[pos+1]->key+1), 2*sizeof(int));
					memcpy((dataType*)(child->parent->child[pos+1]->data), (const dataType*)(child->parent->child[pos+1]->data+1), 2*sizeof(dataType));
				}
			}
			else// if the node has a left sibling 
			{
				if(child->parent->child[pos-1]->key[1] == DISABLE)
				{
					if(child->parent->key[1] != DISABLE)// the left sibling has only 1 key but parent has more than 1 key 
					{
						child->parent->child[pos-1]->key[1] = child->parent->key[pos-1];
						child->parent->child[pos-1]->data[1] = child->parent->data[pos-1];					
						memcpy((int*)(child->parent->key+pos-1), (const int*)(child->parent->key+pos), (3-pos)*sizeof(int));
						memcpy((dataType*)(child->parent->data+pos-1), (const dataType*)(child->parent->data+pos), (3-pos)*sizeof(dataType));
						memcpy((node**)(child->parent->child+pos), (const node**)(child->parent->child+pos+1), (3-pos)*sizeof(node*));						
					}
					else
					{
						transform(pRoot, child);					
					}
				}
				else // if the left sibling has more than 1 key          
				{
					child->key[0] = child->parent->key[pos-1];
					child->data[0] = child->parent->data[pos-1];						
					child->parent->key[pos-1] = child->parent->child[pos-1]->key[1];
					child->parent->data[pos-1] = child->parent->child[pos-1]->data[1];
					child->parent->child[pos-1]->key[1] = DISABLE;
				}
			}
		}
	}
	printf("%d=>%d deleted!\n", k, v);
}


// test
int main()
{
	node* root = init();
	// create a 2-3 tree
	create(&root);
	show(root, 1);
	
	// get data from 2-3 tree
	int key;
	dataType value;
	while(1)
	{
		puts("choose a key that you want to get from the tree, put -1 to end:");
		scanf("%d", &key);		
		if(key == -1)
			break;
		if(get(root, key))
			printf("get key = %d, data = %d\n", get(root, key)->key[0], get(root, key)->data[0]);
	}
	
	// add key=>value into a 2-3 tree
	while(1)
	{
		puts("add key, value into the 2-3 tree:");
		puts("input in form of 'k v', enter k = -1 to end");
		scanf("%d %d", &key, &value);
		if(key == -1)
			break;
		add(&root, key, value);
		puts("2-3 tree changed:"); 
		show(root, 1);
		puts("\ntraverse this 2-3 tree");
		traverse(root);
		printf("\n");
		printf("the depth of this 2-3 tree is %d\n", depth(root));
		printf("the number of the leaf nodes in this 2-3 tree is %d\n", leafNum(root));
	}
	
	// delete key=>value from a 2-3 tree
	while(1)
	{
		puts("delete key, value from a 2-3 tree:");
		puts("input key to delete the key and its value, enter -1 to end");
		scanf("%d", &key);
		if(key == -1)
			break;
		del(&root, key);
		puts("2-3 tree changed:"); 
		show(root, 1);
		puts("\ntraverse this 2-3 tree");
		traverse(root);
		printf("\n");
		printf("the depth of this 2-3 tree is %d\n", depth(root));
		printf("the number of the leaf nodes in this 2-3 tree is %d\n", leafNum(root));
	}
	
	return 0;	
} 
