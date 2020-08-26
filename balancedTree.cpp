#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int dataType;
typedef struct data
{
	int key;
	dataType value;
	data* next;
}data;
typedef struct Node
{
	data* keyValue;// use linked list to store the key and value
	Node** child;// if there is no child nodes child = NULL, otherwise the last *child is NULL
	Node* parent;
}node;

/*
initialize a node
*/ 
node* init(int degree)
{
	node* n = (node*)malloc(sizeof(node));
	if(!n)
		return NULL;
	n->keyValue = NULL; 
	n->child = (node**)malloc((degree+1)*sizeof(node*));
	if(!n->child)
		return NULL;
	for(int i=0;i<=degree;i++)
		n->child[i] = NULL;
 	n->parent = NULL;
	return n;
}

/*
return the depth of a balanced tree
*/
int depth(node* root)
{
	if(!root)
		return 0; 
	int d = 1;
	for(node*p=root;p->child[0];d++,p=p->child[0]);
	return d;
}

/*
return the number of leaf nodes of a balanced tree
*/ 
int leafNum(node* root)
{
	if(!root)
		return 0;
	else if(!root->child[0])
		return 1;
	else
	{
		int num=0;
		for(int i=0;root->child[i];num+=leafNum(root->child[i]),i++); 
		return num;	
	}
}

/*
show a balanced tree
*/
void show(node* root, int layer, int degree)
{
	if(!root)
		return;
	if(root->child[0])
	{
		int num;
		for(num=0;root->child[num];num++);
		int i;
		for(i=0;i<=(num-1)/2;i++)
			show(root->child[i], layer+degree, degree);
		int j = i;
		for(i=0;i<layer;printf("   "),i++);
		data* keyValue = root->keyValue;
		while(keyValue)
		{
			printf("%d=>%d ", keyValue->key, keyValue->value);
			keyValue = keyValue->next;	
		}
		printf("\n");
		for(;j<num;j++)
			show(root->child[j], layer+degree, degree);
	}
	else
	{
		int i;
		for(i=0;i<layer;printf("   "),i++);
		data* keyValue = root->keyValue;
		while(keyValue)
		{
			printf("%d=>%d ", keyValue->key, keyValue->value);
			keyValue = keyValue->next;	
		}
		printf("\n");
	}
} 

/*
traverse a balanced tree
*/
void traverse(node* root)
{
	if(!root)
		return;
	else if(!root->child[0])
	{
		data* keyValue = root->keyValue;
		while(keyValue)
		{
			printf("%d=>%d ", keyValue->key, keyValue->value);
			keyValue = keyValue->next;	
		}		
	}
	else
	{
		int i = 0;
		data* keyValue = root->keyValue;
		while(keyValue)
		{
			traverse(root->child[i]);
			printf("%d=>%d ", keyValue->key, keyValue->value);
			keyValue = keyValue->next;
			i++;
		}	
		traverse(root->child[i]);
	}
}

/*
get a node using a key from a balanced tree
*/
node* get(node* root, int key)
{
	if(!root)
		return NULL;
	int i = 0;
	data* keyValue = root->keyValue; 
	while(keyValue)
	{
		if(keyValue->key == key)
			return root;
		else if(keyValue->key > key)
		{
			if(root->child[i])
				return get(root->child[i], key);
			else
				return NULL;
		}
		keyValue = keyValue->next;
		i++;
	}
	if(root->child[i])
		return get(root->child[i], key);
	else
		return NULL;
}

/*
add key=>value into a balanced tree
*/
void add(node** pRoot, int key, dataType value, int degree)
{
	node* root = *pRoot;
	// find the leaf node to insert the data;
	int pos, i;
	node* n = root;
	data* keyValue;
	while(n->child[0])
	{
		keyValue = n->keyValue;
		i = 0;
		while(keyValue)
		{		
			if(keyValue->key == key)// if the key is already in the tree update it
			{
				printf("[update]: old: %d=>%d new: %d=>%d\n", keyValue->key, keyValue->value, key, value);
				keyValue->value = value;
				return;
			}
			if(keyValue->key > key)
				break;
			keyValue = keyValue->next;
			i++; 
		}
		n = n->child[i];
	}
	// insert key=>data to the node found just now
	int pk = key;
	dataType pv = value;
	while(n)
	{
		//  find the position to insert the key=>value
		keyValue = n->keyValue;
		pos = 0;
		while(keyValue)
		{
			if(keyValue->key == pk)// if the key is already in the tree update it
			{
				printf("[update]: old: %d=>%d new: %d=>%d\n", keyValue->key, keyValue->value, key, value);
				keyValue->value = value;				
				return;
			}
			if(keyValue->key > pk)
				break;
			pos++;
			keyValue = keyValue->next;
		}
		// firstly insert key=>value no matter it is legal or not
		data* newKeyValue = (data*)malloc(sizeof(data));
		if(!newKeyValue)
		{
			printf("failed to apply for memory!\n");
			return;				
		}
		newKeyValue->next = keyValue;
		newKeyValue->key = pk;
		newKeyValue->value = pv;
		if(n->keyValue == keyValue)
			n->keyValue = newKeyValue;
		else
		{
			data* piror = n->keyValue;
			while(piror->next != keyValue)
				piror = piror->next;
			piror->next = newKeyValue;	
		}
		// check if we can insert the key=>value legally
		data* tmp = n->keyValue;
		for(i=0;tmp;tmp=tmp->next,i++);
		if(i < degree)
			return;
		else 
		{
			// push the middle key=>value to parent node
			tmp = n->keyValue;
			for(pos=0;pos<degree/2-1;pos++)
				tmp = tmp->next;
			pos++;// let pos = middle
			pk = tmp->next->key;// update pk and pv
			pv = tmp->next->value;
			tmp->next = tmp->next->next;
			// divide the node into two nodes n:[0~pos-1], newNode:[pos+1~] and adjust key=>value in the two nodes 
			node* newNode = init(degree);
			if(!newNode)
			{
				puts("memory run out!");
				return;
			}
			tmp = n->keyValue;
			for(i=0;i<pos-1;i++)
				tmp = tmp->next;
			newNode->keyValue = tmp->next;// the old middle key=>value has been freed so we do not need another '->next'
			tmp->next = NULL;
			/*
			printf("\nn %d=>%d divided into n %d=>%d and newNode %d=>%d\n", \
			n->keyValue->key, n->keyValue->value, n->keyValue->key, n->keyValue->value, newNode->keyValue->key, newNode->keyValue->value);
			*/
			// build the connection between newNode and the child nodes of n also adjust the connection between n and its child nodes
			if(n->child[0])
			{
				/* be careful here! because when building connection between newNode and the parent node,
					we just add newNode as the child node and didn't check if the number of child nodes of the parent node is legal, 
					which means maybe the number of child nodes of n(parent node in the last circle) is illegal*/
				for(i=pos+1;n->child[i] && i <= degree;i++) 																  
				{
					n->child[i]->parent = newNode;
					newNode->child[i-pos-1] = n->child[i];
					n->child[i] = NULL;	
				}	
				/*
				printf("\nnewNode %d=>%d children:\n", newNode->keyValue->key, newNode->keyValue->value);
				for(i=0;newNode->child[i];i++)
					printf("%d=>%d ", newNode->child[i]->keyValue->key, newNode->child[i]->keyValue->value);
				printf("\nn %d=>%d children:\n", n->keyValue->key, n->keyValue->value);
				for(i=0;n->child[i];i++)
					printf("%d=>%d ", n->child[i]->keyValue->key, n->child[i]->keyValue->value);
				printf("\n");
				*/
			} 
			// build the connection between newNode and the parent node of n 
			// (maybe the number of child nodes will equal degree now, which means the parent node is illegal, but don't worry this node will be divided into two nodes in the next circle)
			if(n->parent)
			{			
				newNode->parent = n->parent;
				for(pos=0;n->parent->child[pos]!=n;pos++);
				for(i=0;n->parent->child[i];i++);				
				memcpy((node**)(n->parent->child+pos+1), (const node**)(n->parent->child+pos), (i-pos)*sizeof(node*));
				n->parent->child[pos+1] = newNode;
			}
			else// if the node is root node change root
			{
				root = init(degree);
				if(!root)
				{
					puts("memory run out!");
					return;
				}
				*pRoot = root;
				root->child[0] = n;
				root->child[1] = newNode;
				n->parent = root;
				newNode->parent = root; 
				root->keyValue = (data*)malloc(sizeof(data));
				root->keyValue->next = NULL;
				root->keyValue->key = pk;
				root->keyValue->value = pv;
				return;
			}
		}
		// go to parent node and check
		//printf("\npush %d=>%d to parent node %d=>%d\n", pk, pv, n->parent->keyValue->key, n->parent->keyValue->value);
		n = n->parent;
	}
}


/*
create a balanced tree
*/
void create(node** pRoot, int degree)
{
	int k,v;
	while(1)
	{
		puts("\ncreate balanced tree. please input key and value in form of 'k v'.\n if you want to end, let key = -1"); 
		scanf("%d %d", &k, &v);
		if(k == -1)
		{
			puts("done!");
			break;
		}
		add(pRoot, k, v, degree);	
		show(*pRoot, 1, degree);
	}
}

/*
delete key=>value from a balanced tree
*/
// delete a leaf node from a balanced tree(the leaf node has only one key=>value and so does its parent node)
// return the next node in LDR sequence
data* next(node* root, int key)
{
	node* n = get(root, key);
	if(!n)
		return NULL;
	data* result = n->keyValue;
	int pos = 0;
	for(;result->key!=key;result=result->next,pos++);
	node* nTmp;
	if(n->child[0]) // if n is not the leaf node, the next key is the first key of the leftmost node of its child[pos+1] tree
	{
		nTmp = n->child[pos+1];
		while(nTmp->child[0])
			nTmp = nTmp->child[0];
		return nTmp->keyValue;			
	}
	else if(result->next)// if n is the leaf node but key is not its last key
	{
		return result->next;
	}
	// n is the leaf node and key is its last key
	nTmp = n;
	while(nTmp->parent)// if n is the leaf node, go to its parent node to check
	{
		for(pos=0;nTmp->parent->child[pos]!=nTmp;pos++);
		if(nTmp->parent->child[pos+1])// if n is not the last child node of its parent node the next kv is in the parent node
		{
			int i;
			result = nTmp->parent->keyValue;
			for(i=0;i!=pos;result=result->next,i++);
			return result;
		}
		else // go to n->parent and check 
		{
			nTmp = nTmp->parent;
		}		
	}
	/* if the tree only has one root node and 'key' is the last key or 'key' is the last key of the rightmost leafnode of the tree, 
		the next is null which means we have finished traversing the tree */
	return NULL;
}
// return the prior node in LDR sequence 
data* prior(node* root, int key)
{
	node* n = get(root, key);
	if(!n)
		return NULL;
	data* result = n->keyValue;
	int pos = 0;
	for(;result->key!=key;result=result->next,pos++);	
	
	node* nTmp;
	if(n->child[0])// if n is not the leaf node the prior key is the last key of the rightmost leaf node of child[pos]
	{
		nTmp = n->child[pos];
		while(nTmp->child[0])
		{
			for(pos=0;nTmp->child[pos+1];pos++);
			nTmp = nTmp->child[pos]; 
		}
		result = nTmp->keyValue;
		while(result->next)
			result = result->next;
		return result;
	}
	else if(pos!=0)// n is the leaf node but key is not the first key
	{
		for(result = n->keyValue;result->next->key!=key;result=result->next);
		return result;
	}
	// n is the leaf node and key is its first key
	while(n->parent)
	{
		if(n!= n->parent->child[0])// if n is not the first child of its parent node
		{
			for(pos=0;n->parent->child[pos+1]!=n;pos++);
			result = n->parent->keyValue;
			for(;pos!=0;result=result->next,pos--);
			return result;
		}
		else // go to parent node and check
			n = n->parent;
	}
	// if the tree only has one root node or 'key' is the first key of its leftmost node, the prior is NULL, which means 'key' is the first key
	return NULL;
}
// collect all the leaf nodes
typedef struct leafNodes
{
	node* leaf;
	leafNodes* next;
}leafNodes;
leafNodes* makeNode()
{
	leafNodes* leaves = (leafNodes*)malloc(sizeof(leafNodes));
	leaves->leaf = NULL;
	leaves->next = NULL;
	return leaves;	
}
leafNodes* leaves = makeNode();
leafNodes* headLeaf = leaves;
void collect(node* root)
{
	if(!root)
		return;
	else if(!root->child[0])
	{
		leaves->leaf = root;
		leaves->next = makeNode();
		leaves = leaves->next;
		//printf("node %d=>%d collected!\n", root->keyValue->key, root->keyValue->value);
	}
	else
	{
		int i;
		for(i=0;root->child[i];i++)
			collect(root->child[i]);
	}
}
void rePush(node** pRoot, int degree, node* child)
{
	collect(*pRoot);// collect all the leaf nodes
	leaves = headLeaf;
	data* kvData;
	//repush all the key=>value in the leaf nodes except for key=>value in node child
	while(leaves->leaf)
	{
		// clear all the leaf nodes
		for(int i=0;leaves->leaf->parent->child[i];i++)
		{
			leaves->leaf->parent->child[i] = NULL;	
		}
		leaves = leaves->next;		
	}
	leaves = headLeaf;
	while(leaves->leaf)
	{
		// repush
		if(leaves->leaf!= child)
		{
			kvData = leaves->leaf->keyValue; 
			while(kvData)
			{
				add(pRoot, kvData->key, kvData->value, degree);
				//printf("%d=>%d repushed\n", kvData->key, kvData->value);
				kvData = kvData->next;		
			}
		}
		leaves = leaves->next;	
	}
	// after repushing, free all the leaves
	leaves = headLeaf;
	leafNodes* tmp;
	while(leaves)
	{
		tmp = leaves->next;
		free(leaves->leaf);
		free(leaves);
		leaves = tmp;
	}
	headLeaf = makeNode();
	leaves = headLeaf;
}

// define a linked list to store nodes
typedef struct nodePointer
{
	node* nod;
	data* kv;
	nodePointer* next;
}np;
np* makeNp()
{
	np* p = (np*)malloc(sizeof(np));
	p->nod = NULL;
	p->kv = NULL;
	p->next = NULL;
	return p;
}
// both node child and its parent node has only one key 
void transform(node** pRoot, int degree, node* child, int key)
{
	np* head = makeNp();
	head->nod = child;
	head->kv = child->keyValue;
	np* n = head;
	data* kvData = child->keyValue;
	node* nTmp;
	int flag = 0;
	// find the next key in sequence of LDR traversing
	while(kvData)
	{
		kvData = next(*pRoot, kvData->key);
		if(!kvData)// all key traversed
			break;
		nTmp = get(*pRoot, kvData->key);
		n->next = makeNp();
		n = n->next;
		n->nod = nTmp;
		n->kv = kvData;
		if(!n->nod->child[0] && n->nod->keyValue->next)// stop at the leaf node that has more than one key	
		{
			flag = 1;
			break;
		}	
	}
	// if leaf nodes after target node all have only one key,  try another direction to find the prior node
	if(flag == 0)
	{
		kvData = child->keyValue;
		n = head;
		while(kvData)
		{
			kvData = prior(*pRoot, kvData->key);
			if(!kvData)// all key traversed
				break;
			nTmp = get(*pRoot, kvData->key);
			if(!n->next)// reuse the np node that has once used before
				n->next = makeNp();
			n = n->next;
			n->nod = nTmp;
			n->kv =  kvData;
			if(!n->nod->child[0] && n->nod->keyValue->next)// stop at the leaf node that has more than one key
			{
				flag = -1;
				break;	
			}	
		}
		// clear all the np node after the last np node
		np* npTmp = n->next;
		np* npTmp2;
		n->next = NULL;
		while(npTmp)
		{
			npTmp2 = npTmp->next; 
			free(npTmp);
			npTmp = npTmp2;
		}
	}
	// if all leaf nodes has only one key repush all the leaf nodes
	if(flag == 0)
	{
		child = get(*pRoot, key);
		data* kv, *nkv = child->keyValue;
		int pos=0;
		while(1)
		{
			if(nkv->key == key)
				break;
			nkv = nkv->next;
			pos++;
		}
		while(child->child[0])
		{
			for(kv=child->child[pos]->keyValue;kv->next;kv=kv->next);
			nkv->key = kv->key;
			nkv->value = kv->value;
			child = child->child[pos];
			nkv = child->keyValue;
			for(pos=0;nkv->next;pos++,nkv=nkv->next);
		}
		rePush(pRoot, degree, child);
		return;	
	}
	// move
	n = head;
	while(n->next)
	{
		n->kv->key = n->next->kv->key;
		n->kv->value = n->next->kv->value;
		n = n->next;
	}
	// the final n point at a leaf node that has more than one key
	data* dataTmp = n->nod->keyValue;
	if(flag == -1)
	{
		while(dataTmp->next->next)
			dataTmp = dataTmp->next;
		free(dataTmp->next);
		dataTmp->next = NULL;
	}
	else// flag = 1
	{
		n->nod->keyValue = n->nod->keyValue->next;
		free(dataTmp);
	}	
}

void del(node** pRoot, int key, int degree)
{
	// find the node
	node* n = get(*pRoot, key);
	if(!n)
	{
		printf("key %d not found!\n", key);
		return;
	}
	// get new key=>value from its child node and change the problem to the problem of deleting key=>value from a leaf node 
	data* kv, *nkv;
	nkv = n->keyValue;
	int pos=0;
	while(1)
	{
		if(nkv->key == key)
			break;
		nkv = nkv->next;
		pos++;
	}
	dataType v = nkv->value;
	while(n->child[0])
	{
		for(kv=n->child[pos]->keyValue;kv->next;kv=kv->next);
		n = n->child[pos];
		nkv = n->keyValue;
		for(pos=0;nkv->next;pos++,nkv=nkv->next);
	}
	// why we redo so much here? because for leaf node who has only one key and only one sibling node that also has only one key, we will do key=>value copy in another function 
	if(n->parent)
	{
		if (!(!n->parent->keyValue->next && !n->keyValue->next && !n->parent->child[1]->keyValue->next))  
		{
			n = get(*pRoot, key);
			nkv = n->keyValue;
			pos=0;
			while(1)
			{
				if(nkv->key == key)
					break;
				nkv = nkv->next;
				pos++;
			}
			while(n->child[0])
			{
				for(kv=n->child[pos]->keyValue;kv->next;kv=kv->next);
				nkv->key = kv->key;
				nkv->value = kv->value;
				n = n->child[pos];
				nkv = n->keyValue;
				for(pos=0;nkv->next;pos++,nkv=nkv->next);
			}		
		}		
	}

	// now the leaf node n is the target node, nkv is the target key=>value 
	// check the number of keys in the leaf node
	kv = n->keyValue;
	pos = 0;
	while(kv)
	{
		pos++;
		kv = kv->next;
	}
	// delete key=>value
	if(pos>1)// if this leaf node has more than one key just delete key=>value from this node
	{
		if(n->keyValue == nkv)
			n->keyValue = nkv->next;	
		else
		{
			data* tmp = n->keyValue;
			while(tmp->next != nkv)
				tmp = tmp->next;
			tmp->next = nkv->next;			
		}
		free(nkv);	
	}
	else // if this leaf node has only one key=>value delete this node
	{
		if(!n->parent)// if this leaf node is also the root node
		{
			*pRoot = NULL;
		}
		else
		{
			data* pkv = n->parent->keyValue;
			pos=0;
			while(n->parent->child[pos] != n)
			{
				pos++;
				pkv = pkv->next;
			}
			if(!pkv)// n is the rightmost child node
			{
				pkv = n->parent->keyValue;
				while(pkv->next)
					pkv = pkv->next;
			}
			
			if(n->parent->child[pos+1])// if the node has a right sibling
			{
				if(n->parent->child[pos+1]->keyValue->next)// if the right sibling has more than one key=>value
				{
					n->keyValue->key = pkv->key;
					n->keyValue->value = pkv->value;
					pkv->key = n->parent->child[pos+1]->keyValue->key;
					pkv->value = n->parent->child[pos+1]->keyValue->value;
					data* tmp = n->parent->child[pos+1]->keyValue->next;
					free(n->parent->child[pos+1]->keyValue);
					n->parent->child[pos+1]->keyValue = tmp;
				}
				else
				{
					if(n->parent->keyValue->next)// although the right sibling has less than one key=>value but the parent node has more than one key=>value
					{
						data* prior = n->parent->keyValue; 
						if(prior != pkv)
						{
							while(prior->next != pkv)
								prior = prior->next;
							prior->next = pkv->next;
						}
						else
							n->parent->keyValue = n->parent->keyValue->next;
						pkv->next = n->parent->child[pos+1]->keyValue;
						n->parent->child[pos+1]->keyValue = pkv;
						int i = pos;
						while(n->parent->child[i])
						{
							n->parent->child[i] = n->parent->child[i+1];
							i++;
						} 
						free(n);
					}
					else
					{
						transform(pRoot, degree, n, key);
						//rePush(pRoot, degree, n);
					}
				}
			}
			else // the node is the rightmost child node of its parent node
			{
				if(n->parent->child[pos-1]->keyValue->next)// if the left sibling has more than one key=>value
				{
					data* lastPrior = n->parent->child[pos-1]->keyValue;
					while(lastPrior->next->next)
						lastPrior = lastPrior->next;
					n->keyValue->key = pkv->key;
					n->keyValue->value = pkv->value;
					pkv->key = lastPrior->next->key;
					pkv->value = lastPrior->next->value;
					free(lastPrior->next);
					lastPrior->next = NULL;
				}
				else
				{
					if(n->parent->keyValue->next)// although the left sibling has only one key=>value but the parent node has more than one key=>value
					{
						data* prior = n->parent->keyValue;
						while(prior->next != pkv)
							prior = prior->next;
						prior->next = NULL;
						n->parent->child[pos-1]->keyValue->next = pkv;
						n->parent->child[pos] = NULL;	
						free(n);	
					}
					else
					{
						transform(pRoot, degree, n, key);
						//rePush(pRoot, degree, n);
					}
				}
			}
		}
	}
	printf("%d=>%d deleted!\n", key, v);
}


// test
int main()
{
	// create a balanced tree
	puts("input the degree of the balanced tree you want to create:");
	int degree;
	scanf("%d", &degree);
	while(degree < 3)
	{
		puts("please input again degree should be no less than 3!");
		scanf("%d", &degree);	
	} 
	node* root = init(degree);
	if(!root)
	{
		puts("failed to apply for memory for root node!");
		return -1;
	}
	create(&root, degree);
	show(root, 1, degree);
	
	// get data from balanced tree
	int key;
	dataType value;
	while(1)
	{
		puts("choose a key that you want to get from the tree, put -1 to end:");
		scanf("%d", &key);		
		if(key == -1)
			break;
		if(get(root, key))
			printf("get key = %d, data = %d\n", get(root, key)->keyValue->key, get(root, key)->keyValue->value);
	}
	
	// add key=>value into a balanced tree
	while(1)
	{
		puts("add key, value into the balanced tree:");
		puts("input in form of 'k v', enter k = -1 to end");
		scanf("%d %d", &key, &value);
		if(key == -1)
			break;
		add(&root, key, value, degree);
		puts("tree changed:"); 
		show(root, 1, degree);
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
		if(prior(root, key))
			printf("the left key of %d is %d\n", key, prior(root, key)->key);
		if(next(root, key))
			printf("the right key of %d is %d\n", key, next(root, key)->key);	
	}
	
	// delete key=>value from a balanced tree
	while(1)
	{
		puts("delete key, value from the tree:");
		puts("input key to delete the key and its value, enter -1 to end");
		scanf("%d", &key);
		if(key == -1)
			break;
		del(&root, key, degree);
		puts("tree changed:"); 
		show(root, 1, degree);
		puts("\ntraverse this tree");
		traverse(root);
		printf("\n");
		printf("the depth of this tree is %d\n", depth(root));
		printf("the number of the leaf nodes in this tree is %d\n", leafNum(root));
	}
	
	return 0;
} 
