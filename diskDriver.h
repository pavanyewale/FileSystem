#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"extra.h"
#include"directory.h"
int loadhdd(char *hname,hd **h)
{   
    hdd *new;
    int i;
    FILE *dp,*lf;
    dp=fopen(hname,"r+");
    if(dp==NULL)
    {  
    //HDD not found.
    fprintf(stdout,"\nno such harddisk present ->%s ",hname);
        return 0;
    }
    lf=fopen("logfile","w");
    fprintf(lf,"--> loadhdd() ");
    fflush(lf);
    new=(hdd*)malloc(sizeof(hdd));
    (*h)=(hd*)malloc(sizeof(hd));
    (*h)->lf=lf;
    (*h)->dp=dp;
    fseek(dp,0,SEEK_SET);
    fread(new,sizeof(hdd),1,dp);
            (*h)->hdd=new;
    (*h)->buff=(char*)malloc(sizeof(char)*new->blockSize);
	(*h)->dir=(dir*)malloc(sizeof(dir));
	strcpy((*h)->dir->currdir,"root");
	(*h)->dir->stack[0]=new->root;
	(*h)->dir->sp=0;
	(*h)->dir->sc=0;
	addStyles(*h,new->root);
    return 1; //loaded but not yet validated that file is hdd or not
}

int readBlock(hd *hh,int blockno)
{   
    fprintf(hh->lf,"--> readBlock(%d) ",blockno);
    fflush(hh->lf);
    hdd *h;
    h=hh->hdd;
    if(blockno>h->noofblocks||blockno<1)
        return 0;   //blockno out of range
    fseek(hh->dp,(blockno-1)*h->blockSize,SEEK_SET);
    for(int i=0;i<h->blockSize;i++)
        (hh->buff)[i]=fgetc(hh->dp);
    return 1;   //successfully return block
}

int writeBlock(hd *hh,int blockno,char *buf)
{   
    fprintf(hh->lf,"--> writeBlock(%d) ",blockno);
    fflush(hh->lf);
    hdd *h;
    h=hh->hdd;
    if(h==NULL)
    {
    	fprintf(hh->lf,"--> hdd is nulll");
    	fflush(hh->lf);
    	return 0;
    } //buffer is empty or no hdd found
    if(buf==NULL)
    {
   	 fprintf(hh->lf,"--> buffer is null ");
    	 fflush(hh->lf);
	 return 0;
    } //buffer is empty or no hdd found
    
    if(blockno>h->noofblocks||blockno<1)
    {
    fprintf(hh->lf,"--> second condition true ");
    fflush(hh->lf);
	    return 0; 
	}//blockno out of range
    fseek(hh->dp,(blockno-1)*h->blockSize,SEEK_SET);
    fwrite(buf,1,h->blockSize,hh->dp);
    fflush(hh->dp);
    fprintf(hh->lf,"--> writen() ");
    fflush(hh->lf);
        return 1;//successfully written
}


int setBlock(hd *hh,unsigned long blockno)
{   
    fprintf(hh->lf,"--> setBlock(%ld) ",blockno);
    fflush(hh->lf);
    hdd *h;
    h=hh->hdd;
    if(!(blockno>h->noofblocks||blockno<1))
    {
        int blno=(blockno/(h->blockSize*8))+2;
        int byteno=((blockno%(h->blockSize*8)-1)/8);
        blockno-=1;
        int bitno=(blockno%8);
        readBlock(hh,blno);
        (hh->buff)[byteno]=(hh->buff)[byteno]|((int)(1<<bitno));
        writeBlock(hh,blno,hh->buff);

    }
    return 0;
}
int freeBlock(hd *hh,unsigned long blockno)
{ 
    fprintf(hh->lf,"--> freeBlock(%ld) ",blockno);
    fflush(hh->lf);
    hdd *h;
    h=hh->hdd;
    if(!(blockno>h->noofblocks||blockno<1))
    {
        int blno=(blockno/(h->blockSize*8))+2;
        int byteno=((blockno%(h->blockSize*8)-1)/8);
        blockno-=1;
        int bitno=(blockno%8);
        readBlock(hh,blno);
        (hh->buff)[byteno]=(hh->buff)[byteno]&(255-(int)(1<<bitno));
        writeBlock(hh,blno,hh->buff);
    }
    return 0;
}
int isFreeBlock(hd *hh,unsigned long blockno)
{
    fprintf(hh->lf,"--> isFreeBlock(%ld) ",blockno);
    fflush(hh->lf);
        int ret;
    hdd *h;
    h=hh->hdd;
    if(!(blockno>h->noofblocks||blockno<1))
    {
        int blno=(blockno/(h->blockSize*8))+2;
        int byteno=(((blockno%(h->blockSize*8))-1)/8);
        blockno-=1;
        int bitno=((blockno)%8);
        readBlock(hh,blno);
        
        ret= !(((hh->buff)[byteno]>>bitno)&1);
    } 
    return ret;
}

int getFirstFreeBlock(hd *hh, int *block)
{ 
    fprintf(hh->lf,"--> getFirstFreeBlock() ");
    fflush(hh->lf);
    hdd *h;
    h=hh->hdd;
    for(long int i=2;i<h->root;i++)
    {   readBlock(hh,i);
        for(long int j=0;j<h->blockSize;j++)
        {
            if((int)((hh->buff)[j])<255)
            {
                for(int k=0;k<8;k++)
                {
                    if(((hh->buff)[j] & (1<<k))==0)
                    {
                        *block=(k+1)+((j)*8)+((i-2)*(h->blockSize*8));
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
