/*
dynamic programming
example 4:
optimal binary search tree
*/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

// calculate total possiblility
float possibility(int i, int j, float* inner, float* leaf)
{
	int k;
	float p = leaf[j+1];
	for(k=i;k<=j;k++)
	{
		p = p + inner[k] + leaf[k];
	}	
	return p;
} 

// show the tree
void show(int* root, int num, int s, int e, float* inner, float* leaf, int layer)
{
	if(s > e)
	{
		for(int i=0;i<=layer;printf("   "),i++);
		printf("d%d:%.4f\n", s, leaf[s]);		
		return;	
	}
	int r = *(root+s*num+e);
	show(root, num, s, r-1, inner, leaf, layer+3);
	for(int i=0;i<=layer;printf("   "),i++);
	printf("k%d:%.4f\n", r, inner[r]);
	show(root, num, r+1, e, inner, leaf, layer+3);
}

int main()
{
	// initialization
	puts("make a optimal binary search tree.");
	puts("please input how many key words(inner leaves) there are:");
	int num;
	scanf("%d", &num);
	if(num < 2) 
	{
		printf("there is no need to make a optimal binary search tree for %d key words!\n", num);
		exit(0);
	}
	float inner[num];
	float leaf[num+1];
	int i;
	float sum=0;
	for(i=0;i<num;i++)
	{
		printf("please input the possibility of the %d key words:\n", i);
		scanf("%f", &inner[i]);
		printf("please input the possibility of less than the %d key words\n", i);
		scanf("%f", &leaf[i]);
		sum = sum + inner[i] + leaf[i];
	}
	leaf[num] = 1 - sum;
	if(leaf[num] < 0)
	{
		puts("ERROR: total possiblility over 1!"); 
		exit(0);
	}
	
	/*
	main part
	*/
	int root[num][num];
	// root[i][j] means the root node of the optimal binary search tree for [di ki di+1 ki+1 ........ dj kj dj+1] 
	float cost[num+1][num];
	//	cost[i][j] means the average search cost of the optimal binary search tree for [di ki di+1 ki+1 ........ dj kj dj+1]
	for(i=0;i<num;i++)
	{
		root[i][i] = i;
		cost[i+1][i] = leaf[i+1];
	}
	int j, k;
	for(i=0;i<num;i++)// steps
	{
		for(j=0;i+j<num;j++)// start point
		{
			cost[j][i+j] = possibility(j, i+j, inner, leaf) + cost[j+1][i+j] + leaf[j];
			root[j][i+j] = j;
			for(k=j+1;k<=i+j;k++)// root position
			{
				if(cost[j][k-1] + cost[k+1][i+j] + possibility(j, i+j, inner, leaf) < cost[j][i+j])
				{
					root[j][i+j] = k;
					cost[j][i+j] = cost[j][k-1] + cost[k+1][i+j] + possibility(j, i+j, inner, leaf);
				}
			}
		}
	}

	// show the result
	printf("the expected average search times is %f\n", cost[0][num-1]);
	puts("the tree is as following:");
	show((int*)root, num, 0, num-1, inner, leaf, 1);
	
	return 0;
}

