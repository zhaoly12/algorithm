#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

/*
dynamic programming:
example 2: 
matrix multiplication
A*B*C*D*E... 
when doing matrix multiplication, using associative law of multiplication can optimize the calculation
eg: [10*2]*([2*100]*[100*3]) is a better choice in comparison with [10*2]*[2*100]*[100*3]
the problem is to find the best way of doing matrix multiplication
*/

// show the result
void show(int* route, int s, int e, int num)
{
	int k = *(route + s*num + e);
	if(k == -1)
		return;
	printf("%d ~ %d and %d ~ %d\n", s, k, k+1, e);
	show(route, s, k, num);
	show(route, k+1, e, num);
}

// test
int main()
{
	// initialization
	puts("find the best way to doing matrix multiplication, which takes the least cost of calculation:");
	
	int num;
	puts("please input how many matrix there are:");
	scanf("%d", &num);
	if(num < 3)
	{
		puts("the number of matrix should no less than 3!"); 
		exit(0);	
	}	
	
	int i, dimension[num+1];
	memset(dimension, 0, sizeof(dimension));
	puts("please input the dimension of each matrix in sequence:");
	puts("the dimension means the column number of each matrix and the No.0 dimension means the number of rows of the first matrix.");
	for(i=0;i<num+1;i++)
	{
		printf("the No.%d dimension:", i);
		scanf("%d", &dimension[i]);
	}
	
	/*
	main part:
	dynamic programming
	*/
	// cost[i][j] means the minimum calculation cost of Ai*Ai+1*....*Aj-1*Aj
	int cost[num][num];
	// route[i][j] means the position of ( ), which means that Ai*Ai+1*....*Aj-1*Aj should be calculated in the way of (Ai*Ai+1*.....Aroute[i][j])*(Aroute[i][j]+1*....Aj) 
	int route[num][num];
	int j,k;
	for(i=0;i<num-1;i++)
	{
		cost[i][i] = 0;
		cost[i][i+1] = dimension[i]*dimension[i+1]*dimension[i+2];	
	}
	cost[num-1][num-1] = 0;
	for(i=0;i<num;i++)
		for(j=0;j<num;j++)
			route[i][j] = -1;
	
	for(j=2;j<num;j++)
	{
		for(i=0;i+j<num;i++)
		{
			cost[i][i+j] = dimension[i]*dimension[i+1]*dimension[i+j+1] + cost[i+1][i+j];
			route[i][i+j] = i;	
			for(k=i+1;k<i+j;k++)
			{
				if(cost[i][i+j] > cost[i][k] + cost[k+1][i+j] + dimension[i]*dimension[k+1]*dimension[i+j+1])
				{
					cost[i][i+j] = cost[i][k] + cost[k+1][i+j] + dimension[i]*dimension[k+1]*dimension[i+j+1];
					route[i][i+j] = k;
				}
			}	
		} 
	}
	
	// show the result
	printf("the least cost is %d\n", cost[0][num-1]);
	puts("do the calculation in the way of:");
	show((int*)route, 0, num-1, num);
	
	return 0;
} 
