#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
why we need a balance tree
   balance tree offers a way of quickly search.
   the essence of balance is to decide the root of a tree.
   any nodes in the tree can be used as the root node but only several of them can make the tree a balance tree. 
*/
/* 
why we need B+ tree
   B+ tree offers a way to separate keys and values so that we can use different keys to search for data for different needs in a database.
*/ 
/*
2-3 tree is a kind of B tree whose M = 3
there are three different points between B tree and B+ tree
1. data included in all nodes(B tree) or only in leaf nodes(B+ tree)
2. leaf nodes linked by a link list(B+ tree) or not(B tree)
3. when doing adjustment, a key will be moved from a node to its parent node in B tree, 
	but in B+ tree the key will be copied from a node to its parent node if the node is a leaf node.
	(this node will be splited into two nodes no matter it is B or B+ tree but in B+tree the key pushed upward will also be saved in one of the two nodes if the node is a leaf node) 
*/
/*
define a B+ tree
*/
/*
example M = 4
		       ___________________________  key4  ________________
		   <= |	 <                                          <    |  <     <
	     __ key1->key3 ________                             ___ key6->key7->key8 ____
     <=	|      <= |         < |                         <= |       <=  |  <= |    < |
	key0->key1  key2->key3   key4                         key5->key6  key7  key8   key9->key10
l -> v0 -> v1 -> v2 -> v3 -> v4 ------------------------> v5 -> v6 --> v7 -> v8 -> v9 -> v10 -> NULL
*/
#define DISABLE -1 // define a key value that is annulled
typedef int dataType;
// linked list that stores keys and values
typedef struct kvNode
{
	int key;
	dataType value;
	kvNode* next;
}kvNode;
// node in B+ tree
typedef struct node
{
	int* key;
	node** child;
	node* parent;
	kvNode** data;// for leaf node there is pointers point at the element in the linked list that stores keys and values
}node;

// initialize the linked list
kvNode* initList()
{
	kvNode* kv = (kvNode*)malloc(sizeof(kvNode));
	if(!kv)
	{
		printf("memory run out!\n");
		return NULL;
	}
	kv->next = NULL;
	kv->key = DISABLE;
	kv->value = -1;
	return kv;
}
// initialize the node in B+ tree
node* initNode(int degree)
{
	node* n = (node*)malloc(sizeof(node));
	if(!n)
	{
		printf("memory run out!\n");
		return NULL;
	}
	n->key = (int*)malloc((degree+1)*sizeof(int));
	n->child = (node**)malloc((degree+2)*sizeof(node*));
	n->data = (kvNode**)malloc((degree+1)*sizeof(kvNode*));
	if(!n->key || !n->child || !n->data)
	{
		puts("memory run out!");
		return NULL;
	}
	n->parent = NULL;
	int i;
	for(i=0;i<=degree;i++)
	{
		n->key[i] = DISABLE;
		n->data[i] = NULL;	
	}
	for(i=0;i<=degree+1;i++)
		n->child[i] = NULL;
	return n; 
}

// return the depth of a B+ tree
int depth(node* root)
{
	if(!root)
		return 0;
	else if(!root->child[0])
	{
		if(root->key[0] != DISABLE)
			return 1;
		else
			return 0;
	} 
	else
	{
		int d = 2;
		node* n = root->child[0];
		for(;n->child[0];d++, n=n->child[0]);
		return d;
	}
}

// count the leaf nodes of a B+ tree
int leafNum(node* root)
{
	if(!root)
		return 0;
	else if(!root->child[0])
	{
		if(root->key[0] != DISABLE)
			return 1;
		else
			return 0;
	}
	else
	{
		int num = 0, i = 0;
		for(;root->child[i];i++)
			num = num + leafNum(root->child[i]);	
		return num;
	}	
} 

// show a B+ tree
void show(node* root, int layer)
{
	if(!root)
		return;
	if(!root->child[0])
	{
		int i;
		for(i=0;i<layer;printf("   "),i++);
		for(i=0;root->data[i];i++)
			printf("%d=>%d ", root->data[i]->key, root->data[i]->value);
		printf("\n");	
	}
	else
	{
		int num;
		for(num=0;root->child[num];num++);
		int i,j;
		for(j=0;j<=(num-1)/2;j++)
			show(root->child[j], layer+3);
		for(i=0;i<layer;printf("   "),i++);			
		for(i=0;root->key[i]!=DISABLE;i++)
			printf("%d ", root->key[i]);
		printf("\n");	
		for(;j<num;j++)
			show(root->child[j], layer+3);
	}
}

// traverse a B+ tree
void traverse(node* root)
{
	if(!root)
		return;
	if(!root->child[0])
	{
		int i;
		for(i=0;root->data[i];i++)
			printf("%d=>%d ", root->data[i]->key, root->data[i]->value);
	}
	else
	{
		int i;
		for(i=0;root->child[i];i++)
			traverse(root->child[i]);
	}
}

//get a leaf node from a B+ tree using a key in it
node* get(node** pRoot, int key)
{
	node* root = *pRoot;
	if(!root)
		return NULL;
	int i;
	if(!root->child[0])
	{
		i = 0;
		while(1)
		{
			if(!root->data[i])
				return NULL;
			if(root->data[i]->key == key)
				return root;
			i++;
		}
	}
	i = 0;
	while(root->key[i] != DISABLE)
	{
		if(root->key[i] >= key)
			return get(&(root->child[i]), key);
		else 
			i++;
	}		
	return get(&(root->child[i]), key);
} 

// find the value from a leaf node using a key in it
kvNode* find(node* leaf, int key)
{
	if(leaf->child[0])
	{
		puts("this is not a leaf node!");
		return NULL;
	}
	int i = 0;
	while(1)
	{
		if(leaf->data[i]->key == key)
			return leaf->data[i];
		i++;
		if(!leaf->data[i])
			return NULL;
	}
}

// add key=>value in a B+ tree if key is already in the tree update the value
void add(node** pRoot, int key, dataType value, int degree, kvNode** head)
{
	node* n = get(pRoot, key);
	kvNode* kv;
	if(n)// if the key is already in the tree update the value
	{
		kv = find(n, key);
		printf("[update]: old %d=>%d new: %d=>%d\n", key, kv->value, key, value);
		kv->value = value;
		return;
	}
	// add new key=>value in linked list
	if((*head)->key == DISABLE)
	{
		(*head)->key = key;
		(*head)->value = value;
		kv = *head;	
	}
	else
	{
		kv = initList();
		kv->key = key;
		kv->value = value;
		kv->next = *head;
		*head = kv;
	}
	/*
	kvNode* kvTmp = *head; 
	printf("linked list updated!\n");
	while(kvTmp)
	{
		printf("%d=>%d ", kvTmp->key, kvTmp->value);
		kvTmp = kvTmp->next;
	}
	printf("\n");
	*/
	// adjust B+ tree
	// find the leaf node to add the key
	n = *pRoot;
	int pos, i;
	while(n->child[0])
	{
		pos = 0;
		while(n->key[pos]!= DISABLE)
		{
			if(n->key[pos] >= key)
				break;
			else
				pos++;	
		}
		n = n->child[pos];
	}
	// add the key=>value no matter it is legal or not
	pos = 0;
	for(;n->data[pos];pos++)
	{
		if(n->data[pos]->key > key)
			break;
	}
	memcpy((kvNode**)(n->data+pos+1), (const kvNode**)(n->data+pos), (degree-pos)*sizeof(kvNode*));
	memcpy((int*)(n->key+pos+1), (const int*)(n->key+pos), (degree-pos)*sizeof(int));
	n->data[pos] = kv;
	n->key[pos] = key;
	
	/*
	printf("\n%d=>%d added in leaf node %d, waiting for check.\n", key, kv->value, n->key[0]);
	show(*pRoot, 1);
	*/
	
	// check if it is illegal
	for(i=0;n->data[i];i++);
	if(i<degree)// legal
		return;
	// illegal
	int pk;
	int j;
	while(i==degree)
	{	
		// find the middle key of node n. it will be pushed to its parent node later		
		pk = n->key[(degree-1)/2];
		
		// split the node into two nodes 
		node* newNode = initNode(degree);
		memcpy((int*)(newNode->key), (const int*)(n->key+(degree-1)/2+1), (degree-(degree-1)/2)*sizeof(int));
		if(!n->child[0]) // if the node is a leaf node, copy middle key to its parent node and save data to its sibling node
		{
			memcpy((kvNode**)(newNode->data), (const kvNode**)(n->data+(degree-1)/2+1), (degree-(degree-1)/2)*sizeof(kvNode*));
			j = (degree-1)/2+1;
			for(;n->data[j];j++)
				n->data[j] = NULL;
			j = (degree-1)/2+1;	
		}
		else
		{
			// adjust the child nodes of n and new nodes
			memcpy((node**)(newNode->child), (const node**)(n->child+(degree-1)/2+1), (degree+1-(degree-1)/2)*sizeof(node*));		
			j = (degree-1)/2+1;
			for(;n->child[j];j++)
			{
				newNode->child[j-1-(degree-1)/2]->parent = newNode;// also all the child nodes of newNode need to change their parent node from n to newNode!
				n->child[j] = NULL;	
			}
			j = (degree-1)/2;	
		}
		for(;n->key[j] != DISABLE;j++)
			n->key[j] = DISABLE;
		
		/*
		int helper=0;
		printf("\nnode splited n: ");
		for(helper=0;n->key[helper]!=DISABLE;helper++)
		{
			if(n->data[helper])
				printf("%d=>%d ", n->data[helper]->key, n->data[helper]->value);
			else
				printf("%d ", n->key[helper]);	
		}
		printf("\nnew node:");
		for(helper=0;newNode->key[helper]!=DISABLE;helper++)
		{
			if(newNode->data[helper])
				printf("%d=>%d ", newNode->data[helper]->key, newNode->data[helper]->value);
			else
				printf("%d ", newNode->key[helper]);
		}	
		*/
		
		// if n is already the root node change the root node
		if(!n->parent)
		{
			*pRoot = initNode(degree);
			(*pRoot)->child[0] = n;
			(*pRoot)->child[1] = newNode;
			n->parent = *pRoot;
			newNode->parent = *pRoot;
			(*pRoot)->key[0] = pk;
			/*
			printf("\nroot changed to :%d\n", pk);
			*/
			return;
		}
		
		// n is not the root node
		// find the position of the node in the child nodes
		pos = 0;
		for(;n->parent->child[pos]!=n;pos++);
		// push the middle key to parent node no matter it is legal or not
		memcpy((int*)(n->parent->key+pos+1), (const int*)(n->parent->key+pos), (degree-pos)*sizeof(int));
		n->parent->key[pos] = pk;
			
		// build the connection between new node and parent node no matter it is legal or not
		newNode->parent = n->parent;
		memcpy((node**)(n->parent->child+pos+2), (const node**)(n->parent->child+pos+1), (degree-pos)*sizeof(node*));
		n->parent->child[pos+1] = newNode;	
		
		/*
		printf("\n%d pushed to parent node %d waiting for check:\n", pk, n->parent->key[0]);
		show(*pRoot, 1);
		*/
		
		n = n->parent;
		// check
		for(i=0;n->key[i]!=DISABLE;i++);
	}
} 

// create a B+ tree
void create(node** pRoot, int degree, kvNode** head)
{
	int key;
	dataType value;
	while(1)
	{
		puts("input key and value in form of 'k v', input '-1 v' to terminate:");
		scanf("%d %d", &key, &value);
		if(key == -1)
		{
			puts("done!");
			break;
		}
		add(pRoot, key, value, degree, head);
		printf("\n%d=>%d added\n", key, value);
		show(*pRoot, 1);
	}	
}

// destroy a B+ tree(except for the leaf nodes)
void destroy(node* root, int degree)
{
	if(!root)
		return;
	if(!(root->child[0]))
	{
		//free(root);// do not free the leaf nodes
		return;
	}
	int i;
	for(i=0;root->child[i];i++)
		destroy(root->child[i], degree);
	//printf("%d freeed\n", root->key[0]);
	free(root);
}


//find the next key
kvNode* next(node** pRoot, int key)
{
	if(!*pRoot)
		return NULL;
	node* n = get(pRoot, key);
	if(!n)
		return NULL;
	int pos = 0;
	for(;n->key[pos]!= key;pos++);
	if(n->key[pos+1]!= DISABLE)// if key is not the last key 
		return n->data[pos+1];	
	// if key is the last key, the next key must be in another node
	while(n->parent)
	{
		for(pos=0;n->parent->child[pos]!=n;pos++);
		if(n->parent->child[pos+1])// n is not the last child of its parent node
		{
			if(n->parent->key[pos] > key)
				return find(get(pRoot, n->parent->key[pos]),n->parent->key[pos]);	
			else// go to the leftmost leaf node of the next child tree
			{
				n = n->parent->child[pos+1];
				while(n->child[0])	
					n = n->child[0];
				return n->data[0];
			}	
		}
		// n is the last child of its parent node
		n = n->parent;
	}
	return NULL;
}

// find the prior key
kvNode* prior(node** pRoot, int key)
{
	if(!*pRoot)
		return NULL;
	node* n = get(pRoot, key);
	if(!n)
		return NULL;
	int pos;
	for(pos=0;n->key[pos]!=key;pos++);
	if(pos!=0)// if key is not the first key 
		return n->data[pos-1];
	// if key is the first key, the prior key must be in another node
	while(n->parent)
	{
		for(pos=0;n->parent->child[pos]!=n;pos++);
		if(pos!=0)// n is not the first child of its parent node
			return find(get(pRoot, n->parent->key[pos-1]), n->parent->key[pos-1]);
		// n is the first child of its parent node
		n = n->parent;
	}
	return NULL;
}

// collect all the leaf nodes
typedef struct leafNodes
{
	node* leaf;
	leafNodes* next;
}leafNodes;
leafNodes* initLeafNodes()
{
	leafNodes* leaves = (leafNodes*)malloc(sizeof(leafNodes));
	leaves->next = NULL;
	leaves->leaf = NULL;
	return leaves;
}
leafNodes* head = initLeafNodes();
leafNodes* leaves = head;
void collect(node* root)
{
	if(!root)
		return; 
	if(!root->child[0])
	{
		leaves->leaf = root;
		leaves->next = initLeafNodes();
		//printf("leaf node %d=>%d collected\n", root->data[0]->key, root->data[0]->value);
		leaves = leaves->next;
	}
	else
	{
		int i;
		for(i=0;root->child[i];i++)
			collect(root->child[i]); 
	} 
}

// repush all the key=>value except for key
void rePush(node** pRoot, int degree, int key, kvNode** kvHead)
{
	collect(*pRoot);
	int i, j;
	// cut the connection between leaf node and its parent node
	// because for a B+ tree leaf nodes contains all keys, so acutually repush is a processe of rebuilding a tree
	// destory the tree(except for the leaf nodes)
	destroy(*pRoot, degree);
	// rebulid the tree
	kvNode* targetKv;
	leaves = head;
	*pRoot = initNode(degree);
	while(leaves->leaf)
	{
		for(i=0;leaves->leaf->data[i];i++)
		{
			// delete data from linked list
			targetKv = leaves->leaf->data[i];
			if(targetKv == *kvHead)
				*kvHead = targetKv->next;
			else
			{
				kvNode* priorKv = *kvHead;
				for(;priorKv->next!=targetKv;priorKv=priorKv->next);
				priorKv->next = targetKv->next;	
			}
			if(leaves->leaf->key[i] != key)
			{
				add(pRoot, leaves->leaf->data[i]->key, leaves->leaf->data[i]->value, degree, kvHead);
				/*
				printf("%d=>%d repushed\n", leaves->leaf->data[i]->key, leaves->leaf->data[i]->value);
				show(*pRoot, 1);
				*/	
			}
			free(targetKv);			
		}
		leaves = leaves->next;
	}
	// reset leaf node list after repush
	leaves = head;
	leafNodes* tmp;
	while(leaves)
	{
		tmp = leaves->next;
		free(leaves->leaf);
		free(leaves);
		leaves = tmp;
	}
	head = initLeafNodes();
	leaves = head;
}

// get a node that is not the leaf node using a key
node* getInnerNode(node* root, int key)
{
	if(!root)
		return NULL;
	if(!root->child[0])
		return NULL;
	int i = 0;
	while(root->key[i]!=DISABLE)
	{
		if(root->key[i] == key)
			return root;
		if(root->key[i] > key)
			return getInnerNode(root->child[i], key);
		i++;
	}
	return getInnerNode(root->child[i], key);
}

// delete a key=>value from B+ tree
void del(node** pRoot, int key, int degree, kvNode** kvHead)
{
	if(!*pRoot)
		return;
	node* target = get(pRoot, key);
	if(!target)
		return;
	kvNode* targetKv = find(target, key);
	// delete key from B+ tree
	// we should find if there is a inner node which is not the leaf node but has key==target->key(if there is a node like that we need to delete it, too)
	node* inner = getInnerNode(*pRoot, key);
	int pos;
	for(pos=0;target->key[pos]!=key;pos++);
	// if the leaf node has more than one key and there is no inner node has this key, just delete the key from the leaf node
	if(target->key[1] != DISABLE && !inner)
	{
		//printf("directly deleted %d=>%d from leaf node\n", target->key[0], target->data[0]->value);
		memcpy((int*)(target->key+pos), (int*)(target->key+pos+1), (degree-1-pos)*(sizeof(int)));
		memcpy((kvNode**)(target->data+pos), (kvNode**)(target->data+pos+1), (degree-1-pos)*(sizeof(kvNode*)));
		// delete data from lined list
		if(targetKv == *kvHead)
		*kvHead = targetKv->next;
		else
		{
			kvNode* priorKv = *kvHead;
			for(;priorKv->next!=targetKv;priorKv=priorKv->next);
			priorKv->next = targetKv->next;	
		}
		free(targetKv);
		return;
	}
	else
	{
		rePush(pRoot, degree, key, kvHead);
		return;
	}	
}

// test
int main()
{
	// create a B+ tree
	puts("input the degree of the B+ tree you want to create:");
	int degree;
	scanf("%d", &degree);
	while(degree < 3)
	{
		puts("please input again degree should be no less than 3!");
		scanf("%d", &degree);	
	} 
	node* root = initNode(degree);
	kvNode* head = initList();
	if(!root || !head)
	{
		puts("failed to apply for memory for root node!");
		return -1;
	}
	create(&root, degree, &head);
	show(root, 1);
	
	// get data from B+ tree
	int key;
	dataType value;
	while(1)
	{
		puts("choose a key that you want to get from the tree, put -1 to end:");
		scanf("%d", &key);		
		if(key == -1)
			break;
		if(get(&root, key))
			printf("get key = %d, data = %d\n", key, find(get(&root, key), key)->value);
	}
	
	// add key=>value into a B+ tree
	while(1)
	{
		puts("add key, value into the B+ tree:");
		puts("input in form of 'k v', enter k = -1 to end");
		scanf("%d %d", &key, &value);
		if(key == -1)
			break;
		add(&root, key, value, degree, &head);
		puts("tree changed:"); 
		show(root, 1);
		puts("\ntraverse this tree");
		traverse(root);
		printf("\n");
		printf("the depth of this tree is %d\n", depth(root));
		printf("the number of the leaf nodes in this tree is %d\n", leafNum(root));
	}
	
	// find keys near key
	while(1)
	{
		puts("input key that you want to know its left key(the laggest key of keys smaller than the key you input)\nand right key(the smallest key of keys lagger than the key you input)");
		puts("input -1 to end");
		scanf("%d", &key);
		if(key == -1)
			break;
		if(prior(&root, key))
			printf("the left key of %d is %d\n", key, prior(&root, key)->key);
		if(next(&root, key))
			printf("the right key of %d is %d\n", key, next(&root, key)->key);	
	}

	// delete key=>value from a balanced tree
	while(1)
	{
		puts("delete key, value from the tree:");
		puts("input key to delete the key and its value, enter -1 to end");
		scanf("%d", &key);
		if(key == -1)
			break;
		del(&root, key, degree, &head);
		puts("tree changed:"); 
		show(root, 1);
		puts("\ntraverse this tree");
		traverse(root);
		printf("\n");
		printf("the depth of this tree is %d\n", depth(root));
		printf("the number of the leaf nodes in this tree is %d\n", leafNum(root));
	}
	
	return 0;
}
