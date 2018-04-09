#include<string.h>
#include<stdlib.h>
void copybuffer(char **to,char *from,int length)
{int i;
for(i=0;i<length;i++)
{
*to[i]=from[i];
}
}
long int btoi(unsigned char *c)
{
long int i=c[3];
i=(i<<8)+c[2];
i=(i<<8)+c[1];
i=(i<<8)+c[0];
return i;
}

int split(char *str,int no,char ***result,int *n)
 {  
     int x = strlen(str)/no +1,i,k=0,j=0;
  //   *result = (char**)malloc(sizeof(char*)*x);
     if(*result==NULL)
         return 0;
     while(str[k]!='\0')
     {
        // (*result)[j] = (char*)malloc(sizeof(char)*no);
         for(i=0;i<no&&str[k]!='\0';k++,i++)
         {
            (*result)[j][i]=str[k];
         }
            (*result)[j][i]='\0';
 
         j++;
     }
	*n=j;
     return 1;
}
int waitforme(int i)
{int j,k;
for(j=0;j<20000*i;j++)
{
for(k=0;k<1000;k++)
{
}
}
}
