#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void BruteForce(char* longStr, char* subStr)
{
	int step = 0;
	int pos;
	while(step+strlen(subStr) <= strlen(longStr))
	{
		for(pos=0;pos<strlen(subStr);pos++)
		{
			if(longStr[step+pos] != subStr[pos])
			{
				step++;
				break;
			}
		}
		if(pos == strlen(subStr))
		{
			printf("%s is a substring of %s and the start position is %d\n", subStr, longStr, step);
			return;
		}
	}
	printf("%s is not a substring of %s\n", subStr, longStr);
}

// D.E.Knuth, J.H.Morris, V.R.Pratt algorithm
/*
when using brute force, we always start from the start point of substring
KMP algorithm is a way to find the best start point of substring 
*/
void KMP(char* longStr, char* subStr)
{
	// generate next[]
	int next[strlen(subStr)];
	next[0] = 0;
	next[1] = 0;
	// i-1 is the tail of the suffix and j is the tail of the prefix (prefix == suffix)
	int i = 2, j = 0;// next[i] = j (if subStr[i] != longStr[k], compare subStr[j] with longStr[k])
	for(;i<strlen(subStr);i++)
	{
		if(subStr[j] == subStr[i-1])
		{
			next[i] = next[i-1] + 1;
			j++;
		}
		else
		{
			j = next[j];
			while(j != 0)
			{
				if(subStr[j] == subStr[i-1])
					break;
				j = next[j];
			}
			next[i] = next[j];
		} 
	}
	
	// compare
	int start = 0;
	int pos = 0;// everytime when comparing, we start from longStr[start] and subStr[pos]	
	while((pos < strlen(subStr))&&(start < strlen(longStr)))
	{
		if(subStr[pos] == longStr[start])
		{
			pos++;
			start++;
		}
		else if(pos != 0)
			pos = next[pos];
		else
			start++;
	}
	if(pos == strlen(subStr))
	{
		printf("%s is a substring of %s and the start position is %d\n", subStr, longStr, start-strlen(subStr));
		return;
	}
	printf("%s is not a substring of %s\n", subStr, longStr);	
}

// test
int main()
{
	puts("judge if a string is a substring of another string:");
	puts("input the long string:");
	char longStr[1000];
	scanf("%s", longStr);
	puts("input the substring:");
	char subStr[1000];
	scanf("%s", subStr);
	
	BruteForce(longStr, subStr);
	KMP(longStr, subStr);
	
	system("pause");
	
	return 0;
} 
