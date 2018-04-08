#include<string.h>
#include<stdlib.h>
#include"diskDriver.h"

int freesubblock(hd *hh,char **buf,int subblocksize,int *blk)//working 100% correct tested
{   
    hdd *h=hh->hdd;
    fprintf(hh->lf,"--> freesubblock() ");
    fflush(hh->lf);
    int bits,bytes,b,bi,flag=0;
    bits=h->blockSize/subblocksize;
    bytes=bits/8;
    bits=bits-(bytes<<3);
    for(b=0;b<bytes;b++)
    {
        unsigned char c=(*buf)[b];
        if(c<0xff)
        {  
            for(bi=0;bi<8;bi++)
            {  
                if(((*buf)[b]&(1<<bi))==0)
                { 
                    flag=1;
                    (*buf)[b]|=1<<bi;
                    break;
                }
            }
        break;
        }
    }
    if(!flag)
    {  
        for(bi=0;bi<bits;bi++)
        {
            if((*buf)[b]&(1<<b)==0)
            {
                (*buf)[b]|=1<<bi;
                flag=1;
                break;
            }
        }
    }
    if(flag)
        *blk=(b<<3)+bi+1;
    return flag;
}
//////////////


int allocateBlock(hd *hh,int internalBlockSize,int *returnBlock)//working tested
{
    fprintf(hh->lf,"--> allocateBlock() ");
    fflush(hh->lf);
    int blocks,bits;
    getFirstFreeBlock(hh,returnBlock);
    if(!*returnBlock)
        return 0;
    unsigned char buf[hh->hdd->blockSize];
    blocks=(hh->hdd->blockSize/internalBlockSize);
    bits = blocks/(internalBlockSize<<3);
    if(blocks%(internalBlockSize<<3)>=0)
        bits=bits+1;
    //printf("No of bits in allocation %d",bits);
    long int ans=0;
    for(int i=0;i<bits;i++)
        ans+=(1<<i);
    char *s;
    s=(char*)&ans;
    strcpy(&buf[0],s);
    for(bits=1;bits<hh->hdd->blockSize;bits++)
    {
        buf[bits]=0x00;
    }
    setBlock(hh,*returnBlock);
    writeBlock(hh,*returnBlock,buf);
    free(buf);
return 1;
}
/////////////////////////////////////
int writemetablock(hd *hh,int mblock,int block,int nblock)
{

    fprintf(hh->lf,"\n\n--> writemetablock()  \n ");
    fflush(hh->lf);
    //printf("\nwriting meta block %d:nblock:%d  nnblock:%d",mblock,block,nblock);
    char *buf;
    unsigned char *c;
    readBlock(hh,mblock,&buf);
    int mmblock,x;
    x= freesubblock(hh,&buf,8,&mmblock);
    //printf("\n metablockno:%d",mmblock);
    if(x==0)
    {  mmblock=btoi(&buf[20]);
      //  printf("when no free block next block :%d",mmblock);
        if(mmblock==0)
        {
            x=allocateBlock(hh,8,&mmblock);
        //    printf("\n next new block:%d",mmblock);
            if(!x)
                return 0;
            c=(unsigned char*)&mmblock;
            strcpy(&buf[20],c);
            writeBlock(hh,mblock,buf);
          //  printf("freee   1");
            free(buf);
        }
        return writemetablock(hh,mmblock,block,nblock);
    } 
    
    int pos=(mmblock-1)<<3;
    c=(unsigned char*)&block;
    strcpy(&buf[pos],c);
    c=(unsigned char*)&nblock;
    strcpy(&buf[pos+4],c);
    writeBlock(hh,mblock,buf);
    //printf("free 2");
    free(buf);
    return 1;
}

////////////////////////////////

int writenameblocks(hd *hh,int block,char **names,int index,int totalNames,int *returnBlock,int *returnNameBlock)
{
    fprintf(hh->lf,"\n\n--> writenameblocks()\n ");
    fflush(hh->lf);
    char *buf;
    readBlock(hh,block,&buf);
    int flag=freesubblock(hh,&buf,hh->hdd->nameblocksize,returnNameBlock);
    if(!flag)
    {
        int blk;
        blk= btoi(&buf[hh->hdd->nameblocksize-4]);
        if(blk==0)
        {   
            allocateBlock(hh,hh->hdd->nameblocksize,&blk);
            //printf(" \n\n %d",blk);
            char *b;
            b=(char*)&blk;
            strcpy(&buf[hh->hdd->nameblocksize-4],b);
            writeBlock(hh,block,buf);
            free(buf);

            return  writenameblocks(hh,blk,names,index,totalNames,returnBlock,returnNameBlock);
        }
        else
        {   free(buf);
            return writenameblocks(hh,blk,names,index,totalNames,returnBlock,returnNameBlock); 
        }
    }
    *returnBlock=block;
    writeBlock(hh,block,buf);
    free(buf);
    int nextblock=0x00;
    int nextnameblock=0x00;
    if(totalNames-index>1)
        {
         writenameblocks(hh,block,names,index+1,totalNames,&nextblock,&nextnameblock);
        }
    unsigned char *s;
    s=(char*)malloc(sizeof(char)*hh->hdd->nameblocksize);   
    strcpy(&s[0],names[index]);
    char *c;
    c=(char*)&nextblock;
    strncpy(&s[hh->hdd->nameblocksize-8],c,4);
    c=(char*)&nextnameblock;
    strncpy(&s[hh->hdd->nameblocksize-4],c,4);

    readBlock(hh,*returnBlock,&buf);
    int pos=(*returnNameBlock-1)*(hh->hdd->nameblocksize);
    for(int i=0;i<32;i++)
    {
        buf[pos+i]=s[i];
    }
    
    
    writeBlock(hh,*returnBlock,buf);

    free(buf);
    free(s);
    return 1;

}

//improved 
int writenameblocks1(hd *hh,int block,char *buf,char **names,int totalNames,int nextBlock,int nextNameBlock,int *returnBlock,int *returnNameBlock)
{
    fprintf(hh->lf,"--> writenameblocks1()");
    fflush(hh->lf);
  
    if(totalNames<=0)
    {   writeBlock(hh,block,buf); 
        *returnBlock=nextBlock;
        *returnNameBlock=nextNameBlock;
        free(buf);
        return 1;
    }
    int flag=freesubblock(hh,&buf,hh->hdd->nameblocksize,returnNameBlock);
    if(!flag)
    {  
        int blk;
        blk= btoi(&buf[hh->hdd->nameblocksize-4]);
        if(blk==0)
        {   
            allocateBlock(hh,hh->hdd->nameblocksize,&blk);
            char *b;
            b=(char*)&blk;
            strcpy(&buf[hh->hdd->nameblocksize-4],b);
        }
        writeBlock(hh,block,buf);
            free(buf);
        
            readBlock(hh,blk,&buf);
            return  writenameblocks1(hh,blk,buf,names,totalNames,nextBlock,nextNameBlock,returnBlock,returnNameBlock);
    }
    
    unsigned char *s;
    s=(char*)malloc(sizeof(char)*hh->hdd->nameblocksize);   
    strcpy(&s[0],names[totalNames-1]);
    char *c;
    c=(char*)&nextBlock;
    strncpy(&s[hh->hdd->nameblocksize-8],c,4);
    c=(char*)&nextNameBlock;
    strncpy(&s[hh->hdd->nameblocksize-4],c,4);
    int pos=(*returnNameBlock-1)*(hh->hdd->nameblocksize);
    for(int i=0;i<32;i++)
    {
        buf[pos+i]=s[i];
    }

    free(s);

    return writenameblocks1(hh,block,buf,names,totalNames-1,block,*returnNameBlock,returnBlock,returnNameBlock);

}

///////////////////////////////////////////////
int writename(hd *hh,char *name)
{
    fprintf(hh->lf,"\n\n--> writename()\n ");
    fflush(hh->lf);
    int n,i;
    char **names;
    int block,nblock;
    return 1;
    split(name,hh->hdd->nameblocksize-8,&names,&n);
printf("\nno of substrings:%d",n); 
    return 1;
    char *buf;
    readBlock(hh,hh->hdd->nameblock,&buf);
    
    writenameblocks1(hh,hh->hdd->nameblock,buf,names,n,0,0,&block,&nblock);
    free(names); 
    return  writemetablock(hh,hh->hdd->metablock,block,nblock);

}

///////////////////////////////////////////////
int getName(hd *hh,char *buf,int block,int nameblock,int level,char **returnName)//level  by default 1
{
    fprintf(hh->lf,"-->getName()");
    fflush(hh->lf);
    strncat(*returnName,&buf[(nameblock-1)*hh->hdd->nameblocksize],hh->hdd->nameblocksize-8);

    int nextblock=btoi(&buf[nameblock*hh->hdd->nameblocksize-8]);

    int nextnameblock=btoi(&buf[nameblock*hh->hdd->nameblocksize-4]);
    if(nextblock==0)
        return 0;
    if(nextblock!=block)
    {
        
        free(buf);
    readBlock(hh,nextblock,&buf);
    }
    return getName(hh,buf,nextblock,nextnameblock,level+1,returnName);

}
 
void displayNames1(hd *hh,char *buf,int metablock)
{
    fprintf(hh->lf,"\n--> displayNames1111()\n ");
    fflush(hh->lf);
    char *str,*buff;
    str=(char *)malloc(1500);
    str[0]=0x00;
    int block=btoi(&buf[metablock*8]);
    if(block==0)
        return;
    readBlock(hh,block,&buff);
    printf("\nblock:%d \t Name block:%d",block,(int)btoi(&buf[(metablock*8)+4]));
    getName(hh,buff,block,btoi(&buf[(metablock*8)+4]),1,&str);
    printf("\n%s",str);
    free(str);
    return;
}
int displayNames(hd *hh,int mblock)
{
    fprintf(hh->lf,"\n\n--> displayNames()\n ");
    fflush(hh->lf);
    char *buf;
    int i,j,bit,byte;
    //printb(hh,mblock);
    readBlock(hh,mblock,&buf);
    bit=hh->hdd->blockSize/8;
    byte=bit/8;
    bit=bit%8;
    for(j=3;j<8;j++)
    {
        if(((buf[0]>>j)&1))
        {
            displayNames1(hh,buf,j);
        }
       // waitforme(2);
    }
    for(i=1;i<byte;i++)
    {
            for(j=0;j<8;j++)
            {   
                if(((buf[i]>>j)&1)==1)
                {
                    displayNames1(hh,buf,j+(i*8));
                }
        //        waitforme(2);
            }
    
    } 
    for(j=0;j<bit;j++)
    {   
        if(((buf[byte]>>j)&1)==1)
        {
            displayNames1(hh,buf,j+(i*8));
        }
      //  waitforme(2);
    }
   i= btoi(&buf[20]);
   free(buf);
   if(i!=0)
   {
        displayNames(hh,i);
   }
return 0;
}

int deleteName(hd *hh,int block,int nameBlock)
{
    fprintf(hh->lf,"\n\n--> deleteName()\n ");
    fflush(hh->lf);
char *buf;
readBlock(hh,block,&buf);

}
