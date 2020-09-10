#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// test
int main()
{
	puts("input the number of strings you want to sort:");
	int num;
	scanf("%d", &num);
	puts("input the length of the strings(all strings have to have the same length):");
	int len;
	scanf("%d", &len);
	char* str[num];
	for(int i=0;i<num;i++)
	{
		str[i] = (char*)malloc((len+1)*sizeof(char));
		printf("please input the No.%d string:\n", i);
		scanf("%s", str[i]);
		if(strlen(str[i]) != len)
		{
			printf("string length should be %d, please input again!\n", len);
			i--;
		}
	}

	// least-significant-digit first (LSD)	
	int count[256];// the number of ASCII codes is 256
	int cursor[256];
	int d;// digit of the string
	int i,j;
	// count every characters
	for(d=len-1;d>=0;d--)
	{
		memset(count, 0, 256*sizeof(int));
		memset(cursor, -1, 256*sizeof(int));
		for(i=0;i<num;i++)
		{
			count[(unsigned)str[i][d]]++;
		}	
		// generate the index
		for(i=1;i<256;i++)
			count[i] = count[i] + count[i-1];
		// record the ASCII code that really exist in the string
		for(i=0,j=0;i<256;i++)
		{
			if(count[i] != 0)
				cursor[j] = i, j++;
		}
		// sort
		char* tmp[num];
		int dTmp, k = 0;
		for(i=0;i<num;i++)
		{
			dTmp = (unsigned)str[i][d];
			for(j=0;cursor[j]!=dTmp;j++);
			if(j==0)
			{
				tmp[k] = str[i];
				k++;
			}
			else
			{
				tmp[count[cursor[j-1]]] = str[i];
				count[cursor[j-1]]++;	
			}
		}
		// rewrite
		for(i=0;i<num;i++)
			str[i] = tmp[i];
		// print
		printf("\nsort:\n");
		for(i=0;i<num;i++)
		{
			printf("%s\n", str[i]);
		}
	}
	
	system("pause"); 
	
	return 0;
}
