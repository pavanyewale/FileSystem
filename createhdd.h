#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "header.h"
#include "diskDriver.h"
int reset(char *hddname)
{ 
    hd *hh=NULL;
    int i,returncode;
    returncode=loadhdd(hddname,&hh);
    if(hh==NULL)
    {
    	fprintf(stdout,"failed to load the harddisk");
	    return 0;
    }
    if(returncode==0)
	    return 0; //no such harddisk present in current directory
    hdd *h;
    h=hh->hdd;
    //setting default allocated blocks
    int blocks,bytes,bits,metastart;  
    blocks=h->root/(h->blockSize*8);
    bytes=h->root%(h->blockSize*8);
    bits=bytes%8;
    bytes=bytes/8;
    printf("blocks=%d bytes=%d bits=%d",blocks,bytes,bits);
    //setting hh->buffer to full high
    memset(hh->buff,0xff,h->blockSize);
    /*for(i=0;i<h->blockSize;i++)
	    hh->buff[i]=0xff;
	    */
    metastart=2;
    while(blocks--)
    {
    	writeBlock(hh,metastart,hh->buff);
	metastart++;
    }
    
    if(bytes>0 || bits>0)
    {
    	i=bytes;
	memset(hh->buff,0xff,bytes);
	bytes=0;
	bits--;
	while(bits>-1)
	{
	bytes|=1<<bits--;
	}
	hh->buff[i++]=(char)bytes;
	fprintf(stdout,"byte position%d",i);
	memset((char*)&hh->buff[i],0x0,h->blockSize-i);

	writeBlock(hh,metastart++,hh->buff);
	fprintf(stdout,"metablock=%d",metastart);
    }
    //setting hh->buffer to zero
	memset(hh->buff,0x0,h->blockSize);	
    while(metastart<=h->root)
    {
    	writeBlock(hh,metastart++,hh->buff);
    }
    
	
    return 1;
}

int createDisk(char *name,int size,int blocksize)//size in GB //blockSize in bytes it must be multiple of ((1024)*2^k) where k>0 and blockSize<size
{
    FILE *fp;
    int x;
    fp=fopen(name,"r");
    if(fp!=NULL)
    {
        return 0;//file allready exists
    }
    char ss[10];
    sprintf(ss,"%d",size);
    char *cmd;
    cmd=(char*)malloc(strlen(name)+50);
    strcpy(cmd,"fallocate -l ");
    strcat(cmd,ss);
    strcat(cmd,"GiB ");
    strcat(cmd,name);
    fprintf(stdout,"%s",cmd);
    x=system(cmd);
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
    h->noofblocks=(1024*1024*size)/(blocksize/1024);
    h->root=ceil((h->noofblocks)/(blocksize*8))+2;//1 block for information of hardDisk
    fprintf(stdout,"size=%d,blockSize=%d,noofblocks=%d,root=%d",h->size,h->blockSize,h->noofblocks,h->root);
    fwrite(h,sizeof(hdd),1,fp);
    fclose(fp);
    free(h);
    reset(name);
    return 1;    
}
