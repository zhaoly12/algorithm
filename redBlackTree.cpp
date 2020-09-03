#include <stdio.h>
#include <stdlib.h>


#define DISABLE -1
/*
define a red-black tree
*/
// a red node must have two black child nodes and its parent node must be a black node, too 
typedef int dataType;
typedef struct Node
{
	int key;
	dataType data;
	int red;// red = 1 the node is a red node and there is a red pointer points at it
	Node* LChild;// (if the left child node is a red node, actually it is a sibling node pointer)
	Node* RChild;// black pointer(real child node)
	Node* parent;// (if the node itself is a red node(parent node is a black node), actually it is a sibling node pointer) 
}node;

/*
initialize a node
*/
node* init()
{
	node* n = (node*)malloc(sizeof(node));
	if(!n)
	{
		puts("memory run out!");
		return NULL;	
	}	
	n->key = DISABLE;
	n->red = 1;
	n->LChild = NULL;
	n->RChild = NULL;
	n->parent = NULL;
	return n;
}

/*
return the depth of a red-black tree
*/
int depth(node* root)
{
	if(!root)
		return 0;
	if(root->key == DISABLE)
		return 0;
	int d;
	node* tmp = root;
	for(d=1;tmp->RChild;tmp=tmp->RChild,d++);
	return d;
}

/*
return the number of leaf nodes of a red-black tree
leaf nodes are NULL nodes
*/
int leafNum(node* root)
{
	if(!root)
		return 0;
	if(root->key == DISABLE)
		return 0;
	if(!root->RChild)
	{
		if(!root->LChild)
			return 2;
		else
			return 3;
	}
	return leafNum(root->LChild) + leafNum(root->RChild);
}

/*
traverse a red-black tree 
*/ 
void traverse(node* root)
{
	if(!root)
		return;
	if(root->key == DISABLE)
		return;
	traverse(root->LChild);
	printf("%d=>%d ", root->key, root->data);
	traverse(root->RChild);	
}

/*
show a red-black tree 
*/
void show(node* root, int layer)
{
	if(!root)
		return;	
	if(root->key == DISABLE)
		return;
	if(root->LChild)
	{
		if(root->LChild->red != 1)
			show(root->LChild, layer+3);
		else
			show(root->LChild, layer);	
	}
	for(int i=0;i<layer;printf("   "),i++);
	printf("%d=>%d", root->key, root->data);
	printf("\n");
	show(root->RChild, layer+3);
}

/*
get value from a red-black tree using a key
*/
node* get(node* root, int k)
{
	if(!root)
		return NULL;
	if(root->key == DISABLE)
		return NULL;
	if(root->key == k)
		return root;
	else if(root->key < k)
		return get(root->RChild, k);
	else if(root->key > k)
		return get(root->LChild, k);
	else
	 	return NULL; 
}

/*
add key=>value into a red-black tree(update the value if the key is already in the tree)
*/
void add(node** pRoot, int key , dataType data)
{
	node* n = get(*pRoot, key);
	if(n)// if key is already in the tree, update its value 
	{
		printf("[update]: old: %d->%d, new: %d->%d\n", key, n->data, key, data);
		n->data = data; 
		return; 
	}
	// add new key=>value into the tree
	n = *pRoot;
	if(n->key == DISABLE)// if the tree is empty just add key data into the root node and change the root into a black node
	{
		n->key = key;
		n->data = data;
		n->red = 0;
		return; 
	}
	// go down to leaf node
	while(1)
	{
		if(key < n->key)
		{
			if(!n->LChild)
				break;
			n = n->LChild;			
		}
		if(key > n->key)
		{
			if(!n->RChild)
				break;
			n = n->RChild;
		} 			
	}
	// add key=>value in leaf node no matter it is legal or not
	node* newNode = init();
	newNode->key = key;
	newNode->data = data;
	newNode->parent = n;
	newNode->red = 1;
	if(n->key < key)
		n->RChild = newNode;
	else
		n->LChild = newNode;
	/*
	printf("%d=>%d added, waiting for check\n", key,data);
	show(*pRoot, 1);
	*/
	// adjust the color
	node* ngp, * np;
	while(1)
	{
		if(n->LChild == newNode)
		{
			if(n->red == 0)// legal(red <- black) 
				return;
			// (red <- red <- black)
			/*
			puts("red<-red<-black");
			*/
			ngp = n->parent->parent;
			np = n->parent;
			n->parent = ngp;
			np->LChild = n->RChild;
			if(n->RChild)
				n->RChild->parent = np;
			n->RChild = np;
			np->parent = n;
			newNode->red = 0;
			if(!ngp)// n->parent is root node
			{
				n->red = 0;
				*pRoot = n;
				return;
			}
			if(ngp->LChild == np)
				ngp->LChild = n;
			else
				ngp->RChild = n;
			newNode = n;
			/*
			show(*pRoot, 1);
			*/
		}	
		else
		{	
			if(n->red == 0)
			{
				if(n->LChild)// (red <- black -> red)
				{
					if(n->LChild->red == 1)
					{
						/*
						puts("red<-black->red");
						*/
						n->LChild->red = 0;
						n->RChild->red = 0;
						if(!n->parent)// n is root node
							return;
						n->red = 1;
						newNode = n;
						/*
						show(*pRoot, 1);
						*/	
					}
					else 
						goto blackRed; 
				}
				else // (black -> red)
				{
blackRed:					//puts("black->red");
					n->red = 1;
					newNode->red = 0;
					
					n->RChild = newNode->LChild;
					if(newNode->LChild)
						newNode->LChild->parent = n;
					newNode->LChild = n;
					 
					newNode->parent = n->parent;
					if(!n->parent)// if n is root node
					{
						n->parent = newNode;
						*pRoot = newNode;
						return;
					}
					if(n->parent->LChild == n)
						n->parent->LChild = newNode;
					else
						n->parent->RChild = newNode;
					n->parent = newNode;
					return;
				}
			}
			else// (red( -> red) <- black)
			{
				/*
				puts("red(->red)<-black");
				*/ 
				np = n->parent; 
				newNode->parent = np;
				np->LChild = newNode;
				n->parent = newNode;
				n->RChild = newNode->LChild;
				newNode->LChild = n;
				if(n->RChild)
					n->RChild->parent = n;
				/* 
				puts("changed to red<-red<-black");
				show(*pRoot, 1); 
				*/ 
				ngp = np->parent;
				np->LChild = newNode->RChild;
				if(newNode->RChild)
					newNode->RChild->parent = np;
				newNode->RChild = np;
				np->parent = newNode;				
				newNode->parent = ngp;
				n->red = 0;
				if(!ngp)
				{
					newNode->red = 0;
					*pRoot = newNode;
					return;
				}
				if(ngp->LChild == np)
					ngp->LChild = newNode;
				else
				 	ngp->RChild = newNode;
				n = newNode;
				/*
				puts("red<-red<-black");
				show(*pRoot, 1);
				*/
			}
		}		
		n = n->parent;
	}
}

/*
create a red-black tree
*/
void create(node** pRoot)
{
	puts("create a red-black tree. please input key and value in form of 'k v'. input '-1 v' to terminate.");
	int k,v;
	while(1)
	{
		scanf("%d %d", &k, &v);
		if(k == -1)
		{
			puts("tree created!");
			break;	
		}	
		add(pRoot, k, v);
		printf("%d=>%d added!\n", k,v);
		show(*pRoot, 1);
	}
}

/*
destroy a red-back tree
*/
void destroy(node* root)
{
	if(!root)
		return;
	destroy(root->LChild);
	destroy(root->RChild);
	free(root);
}

/*
return the next key=>value of a key
*/
int next(node* root, int key)
{
	node* n = get(root, key);
	if(!n)
		return -1;
	if(n->RChild)// the next key is in the leftmost leaf node of its right child tree
	{
		for(n=n->RChild;n->LChild;n=n->LChild);
		return n->key;	
	}
	else if(n->parent)
	{
		while(n == n->parent->RChild)
		{
			n = n->parent;
			if(!n->parent)
				return -1;	
		}
		return n->parent->key;			
	}
	else
		return -1;
}

/*
return the prior key=>value of a key
*/
int prior(node* root, int key)
{
	node* n = get(root, key);
	if(!n)
		return -1;
	if(n->LChild)
	{
		for(n=n->LChild;n->RChild;n=n->RChild);
		return n->key;	
	}
	else if(n->parent)
	{
		while(n == n->parent->LChild)
		{
			n = n->parent;
			if(!n->parent)
				return -1;
		}
		return n->parent->key;
	}
	else
		return -1;
} 


// collect all the nodes, stop at a red node that has no child nodes
typedef struct Nodes
{
	node* n;
	Nodes* next; 
}nodes;

nodes* initNodes()
{
	nodes* p = (nodes*)malloc(sizeof(nodes));
	if(!p)
		return NULL;
	p->n = NULL;
	p->next = NULL;
	return p;
}
nodes* phead = initNodes();
nodes* nhead = initNodes();
nodes* priorn = phead;
nodes* nextn = nhead;

int collect(node* root, node* start)
{
	node* priorTmp = start;
	while(1)
	{
		priorn->n = priorTmp;
		if(priorTmp->red == 1 && !(priorTmp->RChild))
			return 1;
		priorTmp = get(root, prior(priorTmp, priorTmp->key));
		if(!priorTmp)
			break;
		priorn->next = initNodes();
		priorn = priorn->next;
		if(!priorn)
		{
			puts("memory run out!");
			exit(0);
		}
	}
	
	node* nextTmp = start;
	while(1)
	{
		nextn->n = nextTmp;
		if(nextTmp->red == 1 && !(nextTmp->RChild))
			return 2;
		nextTmp = get(root, next(nextTmp, nextTmp->key));
		if(!nextTmp)
			return 0;
		nextn->next = initNodes();
		nextn = nextn->next;
		if(!nextn)
		{
			puts("memory run out!");
			exit(0);
		}
	}	
}

void repush(node** pRoot, nodes* np, int key)
{
	node* root = *pRoot;
	*pRoot = init();
	while(np)
	{
		if(np->n->key != key)
		{
			add(pRoot, np->n->key, np->n->data);
		}
		np = np->next;
	}
	destroy(root);
}

/*
delete key=>value from a red-black tree 
*/
void del(node** pRoot, int key)
{
	node* n = get(*pRoot, key);
	if(!n)
		return;
	int flag = collect(*pRoot, n);
	nodes* np = NULL;
	if(flag == 1)
		np = phead;
	else if(flag == 2)
		np = nhead;
	else
	{
		priorn->next = nhead->next;
		repush(pRoot, phead, key);		
		return;
	}
	// move
	while(np->next)
	{
		np->n->key = np->next->n->key;
		np->n->data = np->next->n->data;
		np = np->next;
	}
	// delete
	np->n->parent->LChild = NULL;
	free(np->n);
	// reset head
	priorn = phead;
	nodes* tmp = phead;
	while(tmp)
	{
		priorn = tmp->next;
		free(tmp);
		tmp = priorn;
	}
	phead = initNodes();
	priorn = phead;
	
	nextn = nhead;
	tmp = nhead;
	while(tmp)
	{
		nextn = tmp->next;
		free(tmp);
		tmp = nextn;
	}
	nhead = initNodes();
	nextn = nhead;
}


// test
int main()
{
	// create a red-black tree
	node** pRoot = (node**)malloc(sizeof(node*));
	*pRoot = init();
	create(pRoot);
	show(*pRoot, 1);
	printf("the depth of the tree is %d\n", depth(*pRoot));
	printf("the number of leaf nodes is %d\n", leafNum(*pRoot));
	puts("traverse the tree:");
	traverse(*pRoot);
	printf("\n");
	
	int key;
	dataType value;
	// add key value into a red-black tree  
	while(1)
	{
		puts("add key=>value into a red-black tree, please input key=>value in form of 'k v'");
		puts("input '-1 v' to terminate");
		scanf("%d %d", &key, &value);
		if(key == -1)
			break;
		add(pRoot, key, value);
		show(*pRoot, 1);
		printf("the depth of the tree is %d\n", depth(*pRoot));
		printf("the number of leaf nodes is %d\n", leafNum(*pRoot));
		puts("traverse the tree:");
		traverse(*pRoot);
		printf("\n");		
	}
	
	// get value from red-black tree using key
	while(1)
	{
		puts("get value from a red-black tree using key, please input the key(input -1 to terminate)");
		scanf("%d", &key);
		if(key == -1)
			break;
		if(!get(*pRoot, key))
		{
			puts("key not found!");
			continue;
		}
		printf("the value of %d is %d\n", key, get(*pRoot, key)->data);
		printf("the prior key of %d is %d\n", key, prior(*pRoot, key));
		printf("the next key of %d is %d\n", key, next(*pRoot, key));
	}
	
	// delete key value from a red-black tree
	while(1)
	{
		puts("delete key=>value from a red-black tree, please input the key(input -1 to terminate)");
		scanf("%d", &key);
		if(key == -1)
			break;
		del(pRoot, key);
		show(*pRoot, 1);
		printf("the depth of the tree is %d\n", depth(*pRoot));
		printf("the number of leaf nodes is %d\n", leafNum(*pRoot));
		puts("traverse the tree:");
		traverse(*pRoot);
		printf("\n");
	}
	
	return 0;
}
