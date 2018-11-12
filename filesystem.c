#include<stdio.h>
#include"createhdd.h"
void printb(hd *hh,int bno)
{
	hdd *h;
	int i;
	char *buff;
	fprintf(hh->lf,"--> printing block() ");
	fflush(hh->lf);
	h=hh->hdd;
	readBlock(hh,bno);
	printf("\n");
	for(i=0;i<h->blockSize;i++)
    		printf("%x",(hh->buff)[i]);
}
int main()
{
	int choice,size,blocksize,rc;
	hd *h;
	char name[59];
	char data[1000];
	rc=createDisk("Default",4,1024);
	if(rc==0)
	{	printf("\nDefault Disk allready exists");
	}
	else
	{
		printf("\n Default disk created successfully..!");
	}
	rc=loadhdd("Default",&h);
	if(!rc)
	{
		printf("\n No such harddisk ");
	}
	printf("\n Default Disk Loaded Successfully..!");
	while(1)
	{
		fprintf(stdout,"\n1.createhdd\t2.loadhdd\t3.create directory\t4.liststyles\t5.changedir\t 6.create File \t 7. showFile 0.Exit");
		printf("\n Enter choice:");
		scanf("%d",&choice);
		switch(choice)
		{
			case 1:
				printf("\n Enter name ,Size of hdd in GB and blocksize in bytes");
				scanf("%s%d%d",name,&size,&blocksize);
				rc=createDisk(name,size,blocksize);
				if(rc==0)
				{	printf("\nDisk allready exists");
					continue;
				}
				printf("\n Successful..");
				break;
			case 2:
				printf("\n Enter name to load hadd");
				scanf("%s",name);
				rc=loadhdd(name,&h);
				if(!rc)
				{
					printf("\n No such harddisk ");
					break;
				}
				printf("\n Successful..");
				break;
			case 3:
				printf("\n Enter directory name");
				scanf("%s",name);
				rc=createDir(h,name);
				if(rc<0)
				{
					printf("\n There is a problem to create directory");
					continue;
				}
				printf("\n Successful..");
				break;				
			case 4:	
				liststyles(h);
				break;
			case 5:
				printf("\n Enter dir name");
				scanf("%s",name);
				rc=changedir(h,name);
				if(rc<0)
				{
					printf("\n No such directory present in current dir");
					continue;
				}
				printf("\n Sucessful..");
				break;
				
			case 6:
				   printf("\n Enter file name");
				   scanf("%s",name);
				   printf("\n enter size of file");
				   scanf("%d",&size);
				   printf("\n Enter data");
				   fgets(data,1000, stdin); 
				   fgets(data,1000, stdin); 
				   rc=createFile(h,name,data,size); 
				  if(rc<0)
				  {
				  	printf("\n File allready exits. or problem to create file");
					continue;
				  }
				  printf("\n Successful..");
				   
				  break;
				   
			case 7: printf("\n Enter file name");
				scanf("%s",name);	
				rc=showFile(h,name);
					if(rc<0)
					{
						printf("\n NO such file present in current directory");
						continue;
					}
					printf("\n successfull");
					break;
			case 0:
					exit(0);
					break;
			default:
				printf("\n Invalid choice");

		}
	}
}
