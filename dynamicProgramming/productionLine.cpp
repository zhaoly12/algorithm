/*
dynamic programming
example 1: production line
there are some production lines
they produce the same production
it takes the same steps to produce the production no matter in which lines
different steps takes different time in different lines
it costs some time to switch from a line to another line
*/ 
/*
why dynamic programming is needed?
actually, the problem can be solved by recursion.
if there is M lines and N steps
the essence of this problem is to find the minimum cost of all M^N kinds of ways that can produce the production
if we use recursion, we have to do M^(N-1) times calculation
if we use dynamic programming, we only have to do M*(N-1) times calculation   
in summary, when the problem is actually a recursion problem but a lot of data is shared between different recursion procedure, dynamic programming is recommended. 
*/
#include <stdio.h> 
#include <stdlib.h>
#include <memory.h>

// calculate the cost
int calCost(int cost1, int cost2, int movingCost, int line1, int line2)
{
	if(line1 > line2)
		return cost1 + cost2 + movingCost * (line1 - line2);
	else
		return cost1 + cost2 + movingCost * (line2 - line1);
}

// test
int main()
{
	// initialize the production lines 
	puts("solve the problem of production lines:");
	int lines, steps, movingCost;
	
	puts("please input the number of production lines:");
	scanf("%d", &lines);
	if(lines < 2)
	{
		puts("lines should more than 1!");
		exit(0);
	}
	
	puts("please input the number of steps to produce this production:");
	scanf("%d", &steps);
	if(steps < 2)
	{
		puts("steps should more than 1!"); 
		exit(0);
	}
	
	puts("please input the cost of moving between adjacent lines:");
	scanf("%d", &movingCost);
	
	int cost[lines][steps];	
	memset(cost, 0, sizeof(cost));
	int i,j;
	for(i=0;i<lines;i++)
	{
		for(j=0;j<steps;j++)
		{
			printf("input the cost of the No.%d step of the No.%d line\n", j, i);
			scanf("%d", &cost[i][j]);
		}
	}
	
	// show the production lines
	for(i=0;i<lines;i++)
	{
		printf("line NO.%d: ", i);
		for(j=0;j<steps;j++)
		{
			printf("%d ", cost[i][j]);
		}
		printf("\n");
	}
	printf("it costs %d when moving between adjacent lines\n", movingCost);
	
	/*
	 main part: 
	  dynamic programming
	*/
	// minTotalCost[i][j] means the least cost from start to the No.j step of No.i line
	// route[i][j] means the line of the No.j-1 step in the route that costs least from start to No.j step of No.i line
	int minTotalCost[lines][steps];
	int route[lines][steps];
	memset(minTotalCost, 0, sizeof(minTotalCost));
	memset(route, -1, sizeof(route));
	for(i=0;i<lines;i++)
		minTotalCost[i][0] = cost[i][0];
	int k;
	for(i=1;i<steps;i++)
	{
		for(j=0;j<lines;j++)
		{
			minTotalCost[j][i] = cost[j][i] + minTotalCost[j][i-1];
			route[j][i] = j;
			for(k=0;k<lines;k++) 
			{
				if(minTotalCost[j][i] > calCost(cost[j][i], minTotalCost[k][i-1], movingCost, j, k))
				{
					minTotalCost[j][i] = calCost(cost[j][i], minTotalCost[k][i-1], movingCost, j, k);				
					route[j][i] = k;
				}					
			}
		}
	}
	
	// show the result
	/*
	puts("minimum total cost matrix:");
	for(i=0;i<lines;i++)
	{
		printf("line NO.%d: ", i);
		for(j=0;j<steps;j++)
		{
			printf("%d ", minTotalCost[i][j]);
		}
		printf("\n");
	}
	puts("route matrix:");
	for(i=0;i<lines;i++)
	{
		printf("line NO.%d: ", i);
		for(j=0;j<steps;j++)
		{
			printf("%d ", route[i][j]);
		}
		printf("\n");
	}
	*/
	int minCost = minTotalCost[0][steps-1];
	int exitPoint = 0;
	for(i=1;i<lines;i++)
	{
		if(minCost > minTotalCost[i][steps-1])
		{
			minCost = minTotalCost[i][steps-1];
			exitPoint = i;			
		}	
	}		
	printf("the least cost is %d\n", minCost);
	puts("the route is: ");
	int r[steps];
	r[steps-1] = exitPoint;
	for(i=steps-2;i>=0;i--)
		r[i] = route[r[i+1]][i];
	for(i=1;i<steps;i++)
		printf("%d->", r[i]);
	printf("%d", exitPoint);
	
	return 0;
}
