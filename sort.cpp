#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

typedef struct node
{
	int data;
	node* next;
}node, * linkList;

// print
void show(int* array, int num)
{
	int i;
	for(i=0;i<num;i++)
		printf("%d ", array[i]);
	printf("\n");
}
void showLinkList(linkList l)
{
	linkList p = l->next;
	while(p)
	{
		printf("%d ", p->data);
		p = p->next;
	}
	printf("\n");
}

// selection sort
void selection(int * array, int num)
{
	int i, count = 0;
	int minIndex, min;
	while(count < num)
	{
		minIndex = count;
		for(i=count+1;i<num;i++)
		{
			if(array[i] < array[minIndex])
			{
				minIndex = i;	
			}
		}
		if(minIndex != count)
		{
			min = array[minIndex];
			array[minIndex] = array[count];
			array[count] = min;	
		}
		count++;
		printf("No.%d - array: ", count);
		show(array, num);	
	}
}
void selectionLinkList(linkList l)
{
	linkList p = l->next;
	node* tmp;
	node* min;
	int k;
	int count = 1;
	while(p)
	{
		min = p;
		tmp = p->next;
		while(tmp)
		{
			if(tmp->data < min->data)
				min = tmp;
			tmp = tmp->next;
		}
		if(min != p)
		{
			k = p->data;
			p->data = min->data;
			min->data = k;
		}
		p = p->next; 
		printf("No.%d - list:", count);
		count++;
		showLinkList(l);
	}
}
// swap tow nodes of a linked list
struct Node
{
	int data;
	Node* next;
	// cpp style 
	Node (int i, Node* p):
		data(i), next(p)
	{
	}	
};
void showList(Node* p)
{
	while(p)
	{
		printf("%d ", p->data);
		p = p->next;	
	}
	printf("\n");
}
void selectionLinkListVer2(Node** np)
{
	Node* p = *np, *pPre = NULL;
	Node* minPre, *min, *tmp, *tmpPre; 
	int count = 1;
	while(p)
	{
		minPre = pPre;
		min = p;
		tmpPre = p;
		tmp = p->next;
		while(tmp)
		{
			if(min->data > tmp->data)
			{
				minPre = tmpPre;
				min = tmp;
			}	
			tmpPre = tmp;
			tmp = tmp->next;
		}
		// swap nodes 
		if(pPre)
			pPre->next = min;
		else
			*np = min;
		tmp = (p->next == min)? p : p->next;
		p->next = min->next;
		min->next = tmp;
		if(p != minPre && minPre != NULL)
			minPre->next = p;
		// move
		pPre = min; 	
		p = pPre->next;	
		printf("No.%d - list:", count);
		count++;	
		showList(*np);
	}
} 

//inserting sort
void insertSort(int* array, int num)
{
	int i, j, pos, tmp;
	for(i=0;i<num-1;i++)
	{
		// find the position to insert the new data
		pos = i+1;
		for(j=0;j<=i;j++)
		{
			if(array[i+1] < array[j])
			{
				pos = j; 
				break;	
			}	
		}
		// insert
		tmp = array[i+1]; 
		memmove(((int*)array)+j+1, ((const int*)array)+j, (i+1-j)*sizeof(int));
		/*
		for(j=i+1;j>pos;j--)
		{
			array[j] = array[j-1];
		}
		*/
		array[pos] = tmp;
		printf("No.%d - array:", i+1);
		show(array, num);
	}
}
// swap nodes will be simple
void insertSortList(Node** np)
{
	Node* pos, *posPre;
	Node* p;
	Node* n;
	p = *np;
	int count = 1;
	Node* tmp;
	while(p->next)
	{
		// find the position
		posPre = p;
		pos = p->next;
		n = *np;
		while(n != p->next)
		{
			if(p->next->data < n->data)
			{
				pos = n; 
				break;	
			}
			posPre = n;
			n = n->next;
		}
		// insert(insert p->next into pos)
		if(pos != p->next)
		{
			if(pos == *np)	
				*np = p->next;	
			else
				posPre->next = p->next;
			tmp = p->next;
			p->next = p->next->next;
			tmp->next = pos;	
		}
		else 
		{			
			p = p->next;
		}
		printf("max = %d\n", p->data);
		printf("No.%d - list: ", count);
		count++;
		showList(*np);
	}
}
int Len = 21;
// shell's sort
void shellSort(int* array, int num, int N)
{
	int step = num/N;
	if(num < N)
	{
		printf("Error: can not divide %d elements into %d groups!\n", num, N);
		return;	
	}
	int k, i, j;
	int * tmp = (int*)malloc(num*sizeof(int));
	int count = 1;
	while(step != 0)
	{
		for(j=0;j<step;j++)
		{
			i = 0;
			for(k=j;k<num;k=k+step)
			{
				tmp[i] = array[k];
				i++;
			}
			insertSort(tmp, i);
			i = 0;
			for(k=j;k<num;k=k+step)
			{
				array[k] = tmp[i];
				i++;
			}	
		}
		if(step < N)
			step = step/2;
		else
			step = step/N;
		printf("\nNo.%d - array: ", count);
		count++;
		show(array, Len);
		printf("\n");
	}
}

// merge sort
void printIndent(int count)
{
	for(int i=0;i<count;i++)
		printf("   ");
}
int count = 0;
void merge(int* array, int start, int mid, int end)
{
	int cursor1 = start, cursor2 = mid+1;
	int i = 0;
	int* tmp = (int*)malloc((end-start+1)*sizeof(int));
	while(cursor1<=mid && cursor2 <= end)
	{
		if(array[cursor1] <= array[cursor2])
		{
			tmp[i] = array[cursor1];
			cursor1++;
		}
		else
		{
			tmp[i] = array[cursor2];
			cursor2++;
		}
		i++;
	}
	if(cursor1 == mid+1)
	{
		while(cursor2 <= end)
		{
			tmp[i] = array[cursor2];
			i++;
			cursor2++;
		}
	}
	else
	{
		while(cursor1 <= mid)
		{
			tmp[i] = array[cursor1];
			i++;
			cursor1++;
		}
	}
	memcpy(((int*)array)+start, (const int*)tmp, (end-start+1)*sizeof(int));
	/* 
	for(i=0;i<=end-start;i++)
	{
		array[i+start] = tmp[i];
	}
	*/ 
	free(tmp);	
}
void mergeSort(int* array, int start, int end)
{
	if(start == end)
		return;
	int mid = (start + end)/2;
	count++;
	printIndent(count);
	printf("merge sort level No.%d, start:%d, end:%d\n", count, start, end);
	mergeSort(array, start, mid);
	mergeSort(array, mid+1, end);
	// merge
	merge(array, start, mid, end);
	printIndent(count);
	printf("level No.%d, start:%d, end:%d - Array: ", count, start, end);
	show(array, Len);
	count--;
}
// merge sort from down to up
void mergeSort2(int* array, int num)
{
	int k = 2;
	int i;
	int count = 1;
	while(k/2 < num)
	{
		for(i=k-1;i+1-k<num-1;i=i+k)
		{
			if(i > num-1)
			{
				if((2*i-k+1)/2 < num-1)
					merge(array, i+1-k, (2*i-k+1)/2, num-1);	
			}		
			else
				merge(array, i+1-k, (2*i-k+1)/2, i);
		}
		if(k >= num)
		{
			merge(array, 0, k/2-1, num-1);
		}
		if(k > num)
			printf("every %d elements sorted!\n", num);
		else
			printf("every %d elements sorted!\n", k);
		printf("No.%d - Array: ", count);
		show(array, num);
		count++;
		k = 2*k;
	}
}
// merge sort 
void mergeSortAll(int* array, int start, int end)
{
	puts("input a character to choose: \n merge sort from up to down: 'u', merge from down to up: 'd'");
	while(char buff = getchar()!= '\n' && buff != EOF)
		printf("%c", buff);
	char choice = getchar();
	switch(choice)
	{
		case 'u':
			mergeSort(array, start, end);
			break;
		case 'd':
			mergeSort2(array, end-start+1);
			break;
		default:
			puts("Error: please input 'u' or 'd'!");
	}
}

// fast sort 1
void fastSort(int* array, int start, int end)
{
	if(start >= end)
		return;
	int cursorL = start, cursorH = end;
	int tmp;
	while(true)
	{
		while(array[cursorL] <= array[start] && cursorL < end)
			cursorL++;	
		while(array[cursorH] >= array[start] && cursorH > start)
			cursorH--;
		if(cursorL >= cursorH)
			break;
		else
		{
			tmp = array[cursorL];
			array[cursorL] = array[cursorH];
			array[cursorH] = tmp;
		}
	}
	tmp = array[cursorH];
	array[cursorH] = array[start];
	array[start] = tmp;
	
	count++;
	printIndent(count);
	printf("index of %d is %d : ", array[cursorH], cursorH);
	show(array, 11);
	
	fastSort(array, start, cursorH-1);
	fastSort(array, cursorH+1, end);
	
	count--;
}

// fast sort 2
void fastFillSort(int* array, int start, int end)
{
	if(start >= end)
		return;
	int pivot = array[start];
	int cursorL = start, cursorH = end;
	while(true)
	{
		while(array[cursorH] >= pivot && cursorH > cursorL)
		{
			cursorH--;
		}
		array[cursorL] = array[cursorH];
		while(array[cursorL] <= pivot && cursorL < cursorH)
		{
			cursorL++;
		}
		array[cursorH] = array[cursorL];
		if(cursorL >= cursorH)
		{
			array[cursorL] = pivot;
			break;
		}
	}	
	count++;
	printIndent(count);
	printf("No.%d element is %d\n", cursorL, array[cursorL]);
	printIndent(count);
	show(array, 11);
	fastFillSort(array, start, cursorL-1);
	fastFillSort(array, cursorL+1, end);
	count--; 
}

// fast sort 3 
void fastDijkstraSort(int* array, int start, int end)
{
	if(start >= end)
		return;
	int pivot = array[start];
	int l = start, h = end, i = start+1, tmp;
	while(i <= h)
	{
		if(array[i] < pivot)
		{
			tmp = array[l];
			array[l] = array[i];
			array[i] = tmp;
			l++;
			i++;
		}
		else if(array[i] == pivot)
			i++;
		else
		{
			if(array[h] < pivot)
			{
				tmp = array[h];
				array[h] = array[i];
				h--;
				array[i] = array[l];
				i++;
				array[l] = tmp;
				l++;
			}
			else if(array[h] == pivot)
			{
				tmp = array[i];
				array[i] = array[h];
				i++;
				array[h] = tmp;
				h--;
			}
			else
			{
				h--;
			}
		}	
	}
	count++;
	printIndent(count);
	printf("No.%d - No.%d elements are %d\n" , l, h, array[h]);
	printIndent(count);
	show(array, 11);
	fastDijkstraSort(array, start, l-1);
	fastDijkstraSort(array, h+1, end);
	count--; 
}

// fast sort 4
void fastHeapSortDown2Up(int* array, int end)
{
	int start = 1;
	if(start >= end)
		return;
	int k, max, tmp;
	while(end > start)
	{
		// down to up
		k = (start+end-1)/2;
		while(k >= start)
		{
			max = 2*k;
			if(2*k+1 <= end)
			{
				if(array[2*k+1] > array[2*k])
					max = 2*k+1;
			}
			if(array[max] > array[k])
			{
				tmp = array[k];
				array[k] = array[max];
				array[max] = tmp;
			}
			k--;
		}
		tmp = array[end];
		array[end] = array[start];
		array[start] = tmp;
		printf("end:%d-%d\n", end, array[end]);
		show(array, Len);
		end--;
	}
}
void fastHeapSortUp2Down(int* array, int end)
{
	int start = 1;
	if(start >= end)
		return;
	int k, max, tmp;
	// build heap(up to down)
	k = (start+end-1)/2;
	int n;
	while(k >= start)
	{
		n = k;
		while(n <= end)
		{
			max = n;
			if(2*n <= end)
			{
				if(array[2*n] > array[n])
					max = 2*n;
			}
			if(2*n+1 <= end)
			{
				if(array[2*n+1] > array[max])
					max = 2*n+1;
			}
			if(n == max)
				break;
			tmp = array[n];
			array[n] = array[max];
			array[max] = tmp;
			n = max;
		}
		k--;
	}
	tmp = array[end];
	array[end] = array[start];
	array[start] = tmp;
	printf("end:%d-%d\n", end, array[end]);
	show(array, Len);
	end--;
	// update heap(up to down)
	while(end > start)
	{
		k = start;
		while(k <= end)
		{
			max = k;
			if(2*k <= end)
			{
				if(array[2*k] > array[k])
					max = 2*k;
			}
			if(2*k+1 <= end)
			{
				if(array[2*k+1] > array[max])
					max = 2*k+1;					
			} 
			if(k == max)
				break;
			tmp = array[max];
			array[max] = array[k];
			array[k] = tmp;
			k = max;
		}
		tmp = array[end];
		array[end] = array[start];
		array[start] = tmp;
		printf("end:%d-%d\n", end, array[end]);
		show(array, Len);
		end--;	
	}
}
void fastHeapSort(int* array, int end)
{
	puts("choose up to down input 'u', choose down to up input 'd'");
	while(char buff=getchar()!= '\n' && buff != EOF)
		printf("%c", buff);
	char choice = getchar();
	switch (choice)
	{
		case 'u':
			fastHeapSortUp2Down(array, end);
			break;
		case 'd':
			fastHeapSortDown2Up(array, end);
			break;
		default:
			puts("Error: please input 'u' or 'd'!");
	}
}

// fast sort
void fastSortAll(int* array, int numArray)
{
	puts("which kind of fast sort do you want to use?");
	puts("recursion sort: 'r', fill position sort: 'f', Dijkstra sort: 'd', heap sort: 'h'");
	while(char buff = getchar()!= '\n' && buff !=  EOF)
	{
		printf("%c", buff);
	}
	char mark = getchar();
	switch(mark)
	{
		case 'r':
			fastSort(array, 0, numArray-1);break;
		case 'f':
			fastFillSort(array, 0, numArray-1);break;			
		case 'd':
			fastDijkstraSort(array, 0, numArray-1);break;		
		case 'h':
			fastHeapSort(array, numArray-1);break;	
		default:
		puts("input error: recursion sort: 'r', fill position sort: 'f', Dijkstra sort: 'd', heap sort: 'h'");
	}
}

// linear sort
void bucketSort(int* array , int num)
{
	
}
void countSort(int* array, int num, int min, int max)
{
	int* tmp = (int*)malloc((max-min+1)*sizeof(int));
	int i;
	for(i=0;i<(max-min+1);i++)
		tmp[i] = 0;
	for(i=0;i<num;i++)
	{
		tmp[array[i]-min]++;
	}

	int j;
	for(i=j=0;j<max-min+1;j++)
	{
		while(tmp[j] != 0)
		{
			array[i] = j+min;
			tmp[j]--;
			i++;
		}
	}
	
	free(tmp);
}
/*
void radixSort()
{
	puts("raidx sort fits strings compare very well.");
	char** array = (char**)malloc(8*sizeof(char*));
	array[0] = "01", array[1] = "00", array[2] = "10", array[3] = "09", array[4] = "08", array[5] = "11", array[6] = "02", array[7] = "10";
	puts("original array:");
	int i;
	for(i=0;i<8;i++)
		printf("%s ", array[i]);
	puts("Array sorted:");
	int j = 1, k;
	char* tmp = (char*)malloc(3*sizeof(char));
	for(;j>0;j--)
	{
		for(i=1;i<8;i++)
		{
			for(k=0;k<i;k++)
			{
				if(array[i][j] < array[k][j])
					break;
			}
			
		}
	}
}
*/
void linearSort(int* array, int num, int min, int max)
{
	puts("input a character to choose a method:");
	puts("bucket sort: 'b', count sort: 'c', radix sort: 'r'");
	while(char buff=getchar()!= '\n' && buff != EOF)
		printf("%c", buff);
	char choice = getchar();
	switch(choice)
	{
		case 'b':
		//bucketSort(array, num);
		break;
		
		case 'c':
		countSort(array, num, min, max);
		break;
		
		case 'r':
		//radixSort();
		break;
				
		default:
		puts("Error: please input 'b', 'c' or 'r'!"); 
	}
}


// test
int main()
{
	int list[] = {1,2,3,7,4,6,2,3,5};
	linkList l = (linkList)malloc(sizeof(node));
	l->next = (linkList)malloc(sizeof(node));
	linkList p = l->next;
	int num = sizeof(list)/sizeof(list[0]);
	int i;
	for(i=0;i<num-1;i++)
	{
		p->data = list[i];
		p->next = (linkList)malloc(sizeof(node));
		p = p->next;
	}
	p->data = list[i];
	p->next = NULL;
	
	Node*n = new Node(list[0], NULL);
	Node ** np = &n;
	for(i=1;i<num;i++)
	{
		n = new Node(list[i], n);
	}
	puts("which way do you want to use to sort the array?\ninput a character:(selection sort: 's', insert sort: 'i', shell's sort: 'h', merge sort: 'm', fast sort: 'f', linear sort: 'l')");
	puts("input 'k' to end");
	char choice = getchar();
	char buff;
	while(choice != 'k')
	{
		int array[] = {0,1,3,9,1,5,16,3,2, 15,14,13, 11,16,18,20,24,100,25,26,80};
		int numArray = sizeof(array)/sizeof(array[0]);
		int min = 0, max = 100;
		switch (choice)
		{
			case 's':
			// selection sort (comparing N^2/2 times, swap 0~N times)
			show(array, numArray);
			selection(array, numArray);
			
			puts("linked list ver:");
			showLinkList(l);
			selectionLinkList(l);
			
			puts("linked list ver2:");
			showList(n);
			selectionLinkListVer2(np);
			puts("selection sort completed!");
			break;
			
			case 'i':
			//inserting sort (comparing 1~N^2/2 times, swap 0~N times)
			show(array, numArray);
			insertSort(array, numArray);
			
			showList(n);
			insertSortList(np);
			puts("inserting sort completed!");
			break;
			
			case 'h':
			// shell's sort (N^1.5)
			show(array, numArray);
			shellSort(array, numArray, 3);
			puts("shell's sort completed!");
			break;
			
			case 'm':
			// merge sort (N*log2(N))
			show(array, numArray);
			mergeSortAll(array, 0, numArray-1);
			show(array, numArray);
			puts("merge sort completed!");
			break;
			
			case 'f':
			// fast sort (N*log2(N))
			show(array, numArray);
			fastSortAll(array, numArray);
			show(array, numArray);
			puts("fast sort completed!");
			break;
			
			case 'l':
			// linear sort (N)
			show(array, numArray);
			linearSort(array, numArray, min, max);
			show(array, numArray);
			puts("linear sort completed!");
			break;
			
			default:
			puts("ERROR: input 's', 'i', 'h', 'm', 'f'!");
		}
		// clear buffer
		while((buff = getchar())!= '\n' && buff != EOF)
			printf("%c", buff);
		puts("which way do you want to use to sort the array?\ninput a character:(selection sort: 's', insert sort: 'i', shell's sort: 'h', merge sort: 'm', fast sort: 'f', linear sort: 'l')");
		puts("input 'k' to end");
		choice = getchar();	
	}
	
	return 0;	
} 
