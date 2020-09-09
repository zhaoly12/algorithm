#include <stdio.h>
#include <stdlib.h>

/*
greedy algorithm
example 1:
task arrangement
there are several tasks to be finished
different tasks have different deadline
there are different punishment for different tasks when they are not finished on time
every task can be finished in a unit time 
*/

void DijkstraSort(int* array, int start, int end, int* array2)
{
	if(start >= end)
		return;
	int pivot = array[start];
	int pivot2 = array2[start];
	int h = start, m = start+1, l = end;
	int tmp;
	while(m <= l)
	{
		if(array[l] < array[h])
			l--;
		else if(array[l] == array[h])
		{
			tmp = array[m];
			array[m] = array[h];
			array[h] = tmp;
			
			tmp = array2[m];
			array2[m] = array2[h];
			array2[h] = tmp;
				
			m++;
		}
		else
		{
			tmp = array[h];
			array[h] = array[l];
			array[l] = array[m]; 
			array[m] = tmp;
			
			tmp = array2[h];
			array2[h] = array2[l];
			array2[l] = array2[m];
			array2[m] = tmp;
			
			h++;
			m++;			
		}
	}
	array[h] = pivot;
	array2[h] = pivot2;
	DijkstraSort(array, start, h-1, array2);
	DijkstraSort(array, l+1, end, array2);
}

void fastSort(int* array, int start, int end, int* array2)
{
	if(start >= end)
		return;
	int pivot = array[start];
	int pivot2 = array2[start];
	int l = start, m = start+1, h = end;
	int tmp;
	while(m <= h)
	{
		if(array[l] < array[h])
			h--;
		else if(array[l] == array[h])
		{
			tmp = array[m];
			array[m] = array[h];
			array[h] = tmp;
			
			tmp = array2[m];
			array2[m] = array2[h];
			array2[h] = tmp;
				
			m++;		
		}
		else
		{
			tmp = array[l];
			array[l] = array[h];
			array[h] = array[m];
			array[m] = tmp;
			
			tmp = array2[l];
			array2[l] = array2[h];
			array2[h] = array2[m];
			array2[m] = tmp;
			
			l++;
			m++;			
		}		
	}
	array[l] = pivot;
	array2[l] = pivot2;
	fastSort(array, start, l-1, array2);
	fastSort(array, h+1, end, array2);
}

int main()
{
	// initialization
	puts("input the number of tasks you want to arrange:");
	int num;
	scanf("%d", &num);
	if(num < 2)
	{
		puts("the number of tasks should no less than 2!");
		exit(0);
	}
	int deadLine[num];
	int punishment[num];
	int i;
	for(i=0;i<num;i++)
	{
		printf("the No.%d task have to be finished in how many unit time:\n", i);	
		scanf("%d", &deadLine[i]);
		printf("please input the level of the punishment of No.%d task:\n", i);
		scanf("%d", &punishment[i]);
	}
	
	/*
	main part
	*/
	// sort the task in the sequence of punishment
	DijkstraSort(punishment, 0, num-1, deadLine);
	// show the input
	puts("tasks in punishment sequence(desc)");
	printf("punishment:\n");
	for(i=0;i<num;i++)
		printf("%d ", punishment[i]);
	printf("\ndeadLine:\n");
	for(i=0;i<num;i++)
		printf("%d ", deadLine[i]);
	printf("\n");
	
	// add tasks in line
	int count = 1;
	int deadLineArranged[num];
	int punishmentArranged[num];
	int j = num-1;
	for(i=0;i<num;i++)
	{
		// sort in deadLine sequence to check if all tasks can be finished on time
		deadLineArranged[count-1] = deadLine[i];
		punishmentArranged[count-1] = punishment[i];
		fastSort(deadLineArranged, 0, count-1, punishmentArranged);
		if(deadLineArranged[count-1] >= count)
		{
			printf("%d %d added!\n", deadLine[i], punishment[i]);
			count++;	
		}
		else
		{
			int index;
			for(index=0;punishmentArranged[index]!=punishment[i]||deadLineArranged[index]!=deadLine[i];index++);
			int tmp;
			tmp = deadLineArranged[count-1];
			deadLineArranged[count-1] = deadLineArranged[index];
			deadLineArranged[index] = tmp;
			tmp = punishmentArranged[count-1];
			punishmentArranged[count-1] = punishmentArranged[index];
			punishmentArranged[index] = tmp;
			if(count > 1)
				fastSort(deadLineArranged, 0, count-2, punishmentArranged);
			
			deadLineArranged[j] = deadLine[i];
			punishmentArranged[j] = punishment[i];
			j--;
		}
	}
	count--;
	
	// show the result
	puts("arrang tasks as following sequence:");
	for(i=0;i<count;i++)
		printf("the No.%d task to do: deadLine = %d, punishment = %d\n", i, deadLineArranged[i], punishmentArranged[i]);
	printf("\n");
	for(;i<num;i++)
		printf("the task can not be finished on time: deadLine = %d, punishment = %d\n", deadLineArranged[i], punishmentArranged[i]);
	
	return 0;
} 
