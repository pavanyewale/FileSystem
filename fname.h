#include<string.h>
#include"diskDriver.h"

int freesubblock(hdd *h,char **buf,int subblocksize,int *blk)//working 100% correct tested
{   
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
{int blocks,bits;
getFirstFreeBlock(hh,returnBlock);
if(!*returnBlock)
    return 0;
unsigned char buf[hh->hdd->blockSize];
 blocks=(hh->hdd->blockSize/internalBlockSize);
 bits = blocks/(internalBlockSize<<3);
if(blocks%(internalBlockSize<<3)>0)
    bits=bits+1;

long int ans=0;
for(int i=0;i<bits;i++)
    ans=(ans<<i)+1;
char *s;
s=(char*)&ans;
strcpy(&buf[0],s);
for(bits=hh->hdd->nameblocksize-4;bits<hh->hdd->nameblocksize;bits++)
{
    buf[bits]=0x00;
}
setBlock(hh,*returnBlock);
writeBlock(hh,*returnBlock,buf);
return 1;
}
/////////////////////////////////////
int writemetablock(hd *hh,int mblock,int block,int nblock)
{

    printf("\nwriting meta block:nblock:%d  nnblock:%d",block,nblock);
    char *buf;
    unsigned char *c;
    readBlock(hh,mblock,&buf);
    int mmblock,x;
    x= freesubblock(hh->hdd,&buf,8,&mmblock);
    if(x==0)
    {  mmblock=btoi(&buf[4]);
        if(mmblock==0)
        {
            x=allocateBlock(hh,8,&mmblock);
            if(!x)
                return 0;
            c=(unsigned char*)&mmblock;
            strcpy(&buf[4],c);
            writeBlock(hh,mblock,buf);
            printf("freee   1");
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
    printf("free 2");
    free(buf);
    return 1;
}

////////////////////////////////

int writenameblocks(hd *hh,int block,char **names,int index,int totalNames,int *returnBlock,int *returnNameBlock)
{
    char *buf;
    readBlock(hh,block,&buf);
    int flag=freesubblock(hh->hdd,&buf,hh->hdd->nameblocksize,returnNameBlock);
    if(!flag)
    {
        int blk;
        blk= btoi(&buf[hh->hdd->nameblocksize-4]);
        if(blk==0)
        {   
            allocateBlock(hh,hh->hdd->nameblocksize,&blk);
            printf(" \n\n %d",blk);
            char *b;
            b=(char*)&blk;
            strcpy(&buf[hh->hdd->nameblocksize-4],b);
            writeBlock(hh,block,buf);
            free(buf);
            return writenameblocks(hh,blk,names,index,totalNames,returnBlock,returnNameBlock);
        }
        else
        {  free(buf);
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

///////////////////////////////////////////////
int writename(hd *hh,char *name)
{
    int n;
    char **names;
    int block,nblock;
    split(name,hh->hdd->nameblocksize,&names,&n);
    writenameblocks(hh,hh->hdd->nameblock,names,0,n,&block,&nblock);
   return  writemetablock(hh,hh->hdd->metablock,block,nblock);

}

///////////////////////////////////////////////
int getName(hd *hh,char *buf,int block,int nameblock,int level,char **returnName)//level  by default 1
{
//printf("block:%d,nameblock:%d",block,nameblock);

//char *c;
//*returnName=(char *)realloc(*returnName,(hh->hdd->nameblocksize-5)*level);
//c=(char*)malloc(sizeof(char)*(hh->hdd->nameblocksize-8)*level);
//strcpy(c,*returnName);
//free(*returnName);
//returnName=&c;

strncat(*returnName,&buf[(nameblock-1)*hh->hdd->nameblocksize],hh->hdd->nameblocksize-8);

int nextblock=btoi(&buf[nameblock*hh->hdd->nameblocksize-8]);
if(nextblock==0)
    return 0;
if(nextblock!=block)
{
    free(buf);
readBlock(hh,nextblock,&buf);
}
return getName(hh,buf,nextblock,btoi(&buf[nameblock*hh->hdd->nameblocksize-4]),level+1,returnName);

}

int displayNames(hd *hh,int mblock)
{
    char *buf,*buf1,*name;
    int i,byte,bit,blocks;
    readBlock(hh,mblock,&buf);
    printf("readblockdone");
    blocks=hh->hdd->blockSize/8;
    printf("%d",mblock);

    for(i=blocks/(8*8);i<blocks;i++)
        {
            printf("  iterate %d",i);
          byte=i/8;
          bit=i%8;
          printf("\nbyte:%d",byte);
          printf("\nbits:%d",bit);
          
          if((buf[byte]&(1<<bit))==1)
          {  printf("iterate 2");
              printf("%d",(int)btoi(&buf[(i+1)*8]));
              return 1;
              readBlock(hh,btoi(&buf[(i+1)*8]),&buf1);
            getName(hh,buf1,btoi(&buf[(i+1)*8]),btoi(&buf[((i+1)*8)+4]),1,&name);
            printf("\n %s",name);
            strcpy(name,"");
          }
        }
    free(buf);
    return 1;
}

