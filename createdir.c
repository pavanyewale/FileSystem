#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"header.h"
#include"fname.h"
int reset(char *hddname)
{ 
    hd *hh;
    int i;
    loadhdd(hddname,&hh);
    
    hdd *h;
    h=hh->hdd;
    unsigned char buff[h->blockSize];
    int  noblocks,aloblocks,extra,nobytes,nobits;
    noblocks=h->size/h->blockSize;
    aloblocks=noblocks/(h->blockSize*8);
    extra=noblocks%(h->blockSize*8);
    if(extra>0)
        aloblocks+=1;
    nobytes=aloblocks/8;
    nobits=(aloblocks%8);
    for(i=0;i<nobytes;i++)
    {
        buff[i]=0xff;
    }
    int ans=0;
    for(i=0;i<nobits;i++)
        ans|=1<<i;
    buff[nobytes]=ans;
    for(i=nobytes+1;i<h->blockSize;i++)
        buff[i]=0x00;
    writeBlock(hh,2,buff);
    unsigned char *buf;
    buf=(char*)malloc(h->blockSize);
    for(i=0;i<nobytes;i++)
        buf[i]=0x00;
    for(i=3;i<=aloblocks;i++)
    { writeBlock(hh,i,buf);}
    free(buf);
    FILE *fp;
    fp=fopen(hddname,"r+");
    if(fp==NULL)
        return 0;
    fseek(fp,0,SEEK_SET);
    allocateBlock(hh,8,&hh->hdd->metablock);
    allocateBlock(hh,hh->hdd->nameblocksize,&hh->hdd->nameblock);
    fwrite(hh->hdd,sizeof(hdd),1,fp);
    fclose(fp);
    free(hh);
    return 1;
}



int createDisk(char *name,int size,int blocksize)
{
    FILE *fp;
    fp=fopen(name,"r");
    if(fp!=NULL)
    {
        return 0;
    }
    float s;
    s=size/(1024);
    char ss[10];
    sprintf(ss,"%f",s);
    char *cmd;
    cmd=(char*)malloc(strlen(name)+50);
    strcpy(cmd,"fallocate -l ");
    strcat(cmd,ss);
    strcat(cmd,"KB ");
    strcat(cmd,name);
    int x=system(cmd);
    free(cmd);
    if(x==127)
    {
    return 2;    
    }
    
    fp=fopen(name,"r+");
    if(fp==NULL)
        return 2; 
    fseek(fp,0,SEEK_SET);
    hdd *h;
    h=(hdd*)malloc(sizeof(hdd));
    h->size=size;
    h->blockSize=blocksize;
    h->nameblocksize=32;
    h->noofblocks=size/blocksize;
    h->metablock=((h->noofblocks)/(blocksize*8))+2;
    h->nameblock=h->metablock+1;
    fwrite(h,sizeof(hdd),1,fp);
    fclose(fp);
    free(h);
    reset(name);
    return 1;    
}


void printb(hd *hh,int bno)
{
    hdd *h;
    h=hh->hdd;
char *buff;
readBlock(hh,bno,&buff);
int i;
printf("\n");
for(i=0;i<h->blockSize;i++)
    printf("%x",buff[i]);
free(buff);
}

void main()
    {
        int code;
        code=createDisk("Disk4",1024*1024*1024,1024);
       hd *h;
        loadhdd("Disk4",&h);
       int n;
       char **str;
       char *buf;
       int blk,nblk;
       
        split("Hello worlkasjdflkajdf a;lkdjf a;lkdjf a;lkdjf a;lksdjf ;alskdjf ;alksdjf ;alkdsjf a;lkdjf a;lkdjf a;lkdjf ;ladjfa;ldkfja d;flkajd f;lakdjf;lkadjf;lakdjsf;lkasdjf;alkjdf;kladjfalk;jdf a;sldkfja;ldkfja s;ldkfj a;slkdfjas d;lfkjasd flkajsdf lasjflld ,this is the file system of the main porgram so that we can implementn it in the c programming langualge and I am aso glad that I got a chance to do this thank hyou so much Walk in - Financial Analyst -Any Freshers M.com, B.com,mba (0-1 yrs.)WISDOM BRIDGE MANAGEMENT CONSULTANTS",24,&str,&n);
       //split("hkldjflajdflajdlkfjaljdfjsad;fjklasdjfello world",24,&str,&n); 
        //writename(h,"Hello world this is best example");
        allocateBlock(h,8,&blk);

        printb(h,blk);




       //displayNames(h,h->hdd->metablock);
       return;
       //writename(h,"hel alkdjf aldkjf aldj flo pavan yewale"); 
        readBlock(h,129,&buf);
        printf("block:%ld",btoi(&buf[20]));
        printb(h,130);
       printf("\n\n\n");
       printb(h,129);
       return ;
       //printf("\n\n\nno of strings :%d",n);
        //allocateBlock(h,32,&blk);
        //printf("\nsub block free for %d",blk);
        //printb(h,blk);
        readBlock(h,136,&buf);
        freesubblock(h->hdd,&buf,8,&blk);
        printf("sub block free for size 8 is %d",blk);
        //writeBlock(h,136,buf);
        //writemetablock(h,h->hdd->metablock,140,7);
        displayNames(h,h->hdd->metablock);
        //readBlock(h,129,&buf);
        
        return ;
        //writenameblocks(h,h->hdd->nameblock,str,0,n,&blk,&nblk);
        //printf("\n\nblock no %d   nameblock no %d   \n\n",blk,nblk);
        printb(h,h->hdd->metablock);

        /*/ printb(h,130);
        //readBlock(h,130,&buf);
        //printf("next block :%ld",btoi(&buf[56]));
        //printb(h,130);
       */char *st;
       readBlock(h,130,&buf);
       st[0]=0x00;
        getName(h,buf,130,2,1,&st);
        printf("\n\n return name is : \n %s",st);
        //n=freesubblock(h->hdd,buf,32,&blk);
        //printf("\n\n\nisfree %d  nameblock %d",n,blk);
        /*allocnameblock(h,&n);
        char *buf;
        printb(h,n);
        //getFirstFreeBlock(h,&n);
        printf("alocated name block is:%d",n);
        for(int i=1;i<15;i++)
        {
            printf("block %dis free:%d",i,isFreeBlock(h,i));
        }
        int choice; 
        char *buff;
        readBlock(h,2,&buff);
        printf("\n%x",buff[0]);
        printf("\n%x",buff[1]);
         int x;
        getFirstFreeBlock(h,&x);
        printf("\nfirst free block:%d",x);
        switch(code)
        {
            case 0:printf("\n Error:cannot create Disk...!Disk allready exists");
            break;
            case 2:printf("\n Error:Cannot create file");
            break;
        
        }
        //setBlock(h,16);
        while(1)
        {
        printf("\n1.load hdd\t 2:first free \t3:is free \t4:freeBlock\t5:setBlock\n:enter choice: ");
        scanf("%d",&choice);
        switch(choice)
        {
            case 1:break;
            case 2:
                    getFirstFreeBlock(h,&x);
                    printf("\n first free:%d",x);
                    break;
            case 3: 
                    scanf("%d",&x);
                    printf("\n%d",isFreeBlock(h,x));
                    break;
            case 4:
                    scanf("%d",&x);
                    freeBlock(h,x);
                    break;
            case 5: scanf("%d",&x);
                    setBlock(h,x);
                    break;
        }
        }
        /*for(int i=129;i<200;i++)
        {
        printb(h,i);
        }*/
        return;
    
}
