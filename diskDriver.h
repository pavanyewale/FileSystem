#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"extra.h"
int loadhdd(char *hname,hd **h)
{   hdd *new;

    FILE *dp,*lf;
    dp=fopen(hname,"r+");
    if(dp==NULL)
    {  
        return 0;}
    lf=fopen("logfile","w");
    //HDD not found..:(
    fprintf(lf,"--> loadhdd() ");
    fflush(lf);
    new=(hdd*)malloc(sizeof(hdd));
    (*h)=(hd*)malloc(sizeof(hd));
    (*h)->lf=lf;
    (*h)->dp=dp;
    fseek(dp,0,SEEK_SET);
    fread(new,sizeof(hdd),1,dp);
            (*h)->hdd=new;
    return 1; //loaded but not yet validated that file is hdd or not
}

int readBlock(hd *hh,int blockno,char **buf)
{   
    fprintf(hh->lf,"--> readBlock() ");
    fflush(hh->lf);
    hdd *h;
    h=hh->hdd;
    if(blockno>((h->size-1024)/h->blockSize)||blockno<1)
        return 0;   //blockno out of range
    *buf=(char*) malloc(h->blockSize);
    fseek(hh->dp,(blockno-1)*h->blockSize,SEEK_SET);
    for(int i=0;i<h->blockSize;i++)
        (*buf)[i]=fgetc(hh->dp);
    return 1;   //successfully return block
}

int writeBlock(hd *hh,int blockno,char *buf)
{   
    fprintf(hh->lf,"--> writeBlock() ");
    fflush(hh->lf);
    
    hdd *h;
    h=hh->hdd;
    if(buf==NULL||h==NULL)
        return 0; //buffer is empty or no hdd found
    if(blockno>((h->size)/h->blockSize)||blockno<1)
        return 0; //blockno out of range
    fseek(hh->dp,(blockno-1)*h->blockSize,SEEK_SET);
    fwrite(buf,1,h->blockSize,hh->dp);
    fflush(hh->dp);
        return 1;//successfully written
}


int setBlock(hd *hh,unsigned long blockno)
{   
    fprintf(hh->lf,"--> setBlock() ");
    fflush(hh->lf);
        char *buff;
    hdd *h;
    h=hh->hdd;
    if(!(blockno>((h->size)/h->blockSize)||blockno<1))
    {
        int blno=(blockno/(h->blockSize*8))+2;
        int byteno=((blockno%(h->blockSize*8)-1)/8);
        blockno-=1;
        int bitno=(blockno%8);
        readBlock(hh,blno,&buff);
        buff[byteno]=buff[byteno]|(int)(pow(2,bitno));
        writeBlock(hh,blno,buff);

    }
    free(buff);
    return 0;
}
int freeBlock(hd *hh,unsigned long blockno)
{ 
    fprintf(hh->lf,"--> freeBlock() ");
    fflush(hh->lf);
        char *buff;
    hdd *h;
    h=hh->hdd;
    if(!(blockno>((h->size)/h->blockSize)||blockno<1))
    {
        int blno=(blockno/(h->blockSize*8))+2;
        int byteno=((blockno%(h->blockSize*8)-1)/8);
        blockno-=1;
        int bitno=(blockno%8);
        readBlock(hh,blno,&buff);
        buff[byteno]=buff[byteno]&(255-(int)((pow(2,bitno))));
        writeBlock(hh,blno,buff);
    }
    free(buff);
    return 0;
}
int isFreeBlock(hd *hh,unsigned long blockno)
{
    fprintf(hh->lf,"--> isFreeBlock() ");
    fflush(hh->lf);
        char *buff;
        int ret;
    hdd *h;
    h=hh->hdd;
    if(!(blockno>((h->size)/h->blockSize)||blockno<1))
    {
        int blno=(blockno/(h->blockSize*8))+2;
        int byteno=(((blockno%(h->blockSize*8))-1)/8);
        blockno-=1;
        int bitno=((blockno)%8);
        char *buff;
        readBlock(hh,blno,&buff);
        
        ret= !((buff[byteno]>>bitno)&1);
    } 
    free(buff);
    return ret;
}

int getFirstFreeBlock(hd *hh, int *block)
{ 
    fprintf(hh->lf,"--> getFirstFreeBlock() ");
    fflush(hh->lf);
    hdd *h;
    h=hh->hdd;
    char *buff;
    for(long int i=2;i<(h->noofblocks/(h->blockSize*8));i++)
    {   readBlock(hh,i,&buff);
        for(long int j=0;j<h->blockSize;j++)
        {
            if((int)(buff[j])<255)
            {
                for(int k=0;k<8;k++)
                {
                    if((buff[j] & (1<<k))==0)
                    {
                        *block=(k+1)+((j)*8)+((i-2)*(h->blockSize*8));
                        free(buff);
                        return 1;
                    }
                }
            }
        }
        free(buff);
    }
    return 0;
}
