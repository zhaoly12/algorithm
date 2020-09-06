#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

/*
dynamic programming
example 3:
find the longest common subsequence(LCS) of two lists
*/


// test
int main()
{
	// initialization
	puts("find the longest common subsequence of two strings\n both of the strings have no more than 1000 characters\n characters in a string should be different from one another:");
	puts("please input the first string:");
	char s1[1001];
	char s2[1001];
	scanf("%s", s1);
	puts("please input the second string:");
	scanf("%s", s2);
	
	int len1 = 0, len2 = 0;
	int i;
	for(i=0;s1[i] != '\0';i++)
		len1++;
	for(i=0;s2[i] != '\0';i++)
		len2++;
	
	/*
	main part
	dynamic programming
	*/
	// length[i][j] means the length of longest common subsequence of s1[0 ~ i-1] and s2[0 ~ j-1]
	// length[0][j] means empty and s2[0 ~ j-1], length[i][0] means s1[0 ~ i-1] and empty  
	int length[len1+1][len2+1];
	for(i=0;i<=len1;i++)
		length[i][0] = 0;
	for(i=0;i<=len2;i++)
		length[0][i] = 0;
	
	// pos[i][j]  
	// 0: the common element is s1[i] == s2[j]
	// -1: s1[i] != s2[j]  commen element is the last common element of s1[0 ~ i] and s2[0 ~ j-1]
	// 1: s1[i] != s2[j] common element is the last common element of s1[0 ~ i-1] and s2[0 ~ j]
	int pos[len1][len2];
	
	int j;
	for(i=1;i<=len1;i++)
	{
		for(j=1;j<=len2;j++)
		{
			if(s2[j-1] == s1[i-1])
			{
				length[i][j] = length[i][j-1] + 1;
				pos[i-1][j-1] = 0;	
			}
			else if(length[i][j-1] > length[i-1][j])
			{
				length[i][j] = length[i][j-1];	
				pos[i-1][j-1] = -1;
			}
			else
			{
				length[i][j] = length[i-1][j];	
				pos[i-1][j-1] = 1;
			}			
		}
	}
	
	
	// show the result
	printf("the length of the longest common subsequence is %d\n", length[len1][len2]);
	
	int len = 0;
	if(len1 > len2)
		len = len2;
	else
		len = len1;
	char common[len];
	i = len1-1, j = len2-1;
	int k = 0;
	while(i >= 0 && j >= 0)
	{
		if(pos[i][j] == -1)
			j--;
		else if (pos[i][j] == 1)
			i--;
		else
		{
			common[k] = s1[i];
			//printf("get:%d->%c\n", i, s1[i]);
			k++;
			j--;
		}
	}
	len = k;
	
	printf("the longest common subsequence of\n %s\n and\n %s\n is:\n", s1, s2);
	for(k=len-1;k>=0;k--)
	{
		printf("%c", common[k]);
	}

	return 0;
} 
