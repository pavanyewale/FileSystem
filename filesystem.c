#include<stdio.h>
#include"header.h"
#include"hdd.h"
void printb(hd *hh,int bno)
{
    hdd *h;
    fprintf(hh->lf,"--> printing block() ");
    fflush(hh->lf);
    h=hh->hdd;
char *buff;
readBlock(hh,bno);
int i;
printf("\n");
for(i=0;i<h->blockSize;i++)
    printf("%c",(hh->buff)[i]);
}


void main()
    {
        int code;
        code=createDisk("Disk4",1024*1024*1024,1024);
       hd *h;
        loadhdd("Disk4",&h);

    fprintf(h->lf,"--> main function()\n ");
    fflush(h->lf);
    int choice;
    char data[1000],by[1000];
    while(1)
    {
        printf("\n 1.insert\t2.delete\t3.replace\t4.display\t5.exit");
        scanf("%d",&choice);
        switch(choice)
        {
            case 1: fscanf(stdin,"%s",data);
                    writename(h,data);
                    break;
            case 2:fscanf(stdin,"%s",data);
                   code=deleteFileName(h,data);
                   if(code)
                       fprintf(stdout,"\ndeleted");
                   else
                       fprintf(stdout,"\n not found");
                   break;
            case 3:fprintf(stdout,"\nEnter name to replace:");
                   fscanf(stdin,"%s",data);
                   fprintf(stdout,"\nEnter name by replace");
                   fscanf(stdin,"%s",by);
                   code=replace(h,data,by);
                   if(code)
                       fprintf(stdout,"\n replaced");
                   else
                       fprintf(stdout,"\n not found");

                   break;


            case 4: displayNames(h,h->hdd->metablock);
                    break;
            case 5: exit(0);
            
                    
        }
    }
 }
