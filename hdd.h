#include<stdio.h>
#include<string.h>
#include<stdlib.h>
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



int createDisk(char *name,int size,int blocksize,int nameblocksize,int metablocksize)
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
    h->nameblocksize=nameblocksize;
    h->metablocksize=metablocksize;

    h->noofblocks=size/blocksize;
    h->metablock=((h->noofblocks)/(blocksize*8))+2;
    h->nameblock=h->metablock+1;
    fwrite(h,sizeof(hdd),1,fp);
    fclose(fp);
    free(h);
    reset(name);
    return 1;    
}
