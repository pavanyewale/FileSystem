#include<string.h>
#include"extra.h"
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
setBlock(hh,*returnBlock);
writeBlock(hh,*returnBlock,buf);
return 1;
}
/////////////////////////////////////
int writemetablock(hd *hh,int mblock,int block,int nblock)
{
    char *buf;
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
        }
        return writemetablock(hh,mmblock,block,nblock);
    } 
    
    int pos=(mmblock-1)<<3;
    unsigned char *c;
    c=(unsigned char*)&block;
    strcpy(&buf[pos],c);
    c=(unsigned char*)&nblock;
    strcpy(&buf[pos+4],c);
    writeBlock(hh,mblock,buf);
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
        blk= btoi(&buf[hh->hdd->blockSize-4]);
        if(blk==0)
        {   
            allocateBlock(hh,hh->hdd->nameblocksize,&blk);
            char *b;
            b=(char*)&blk;
            strcpy(&buf[hh->hdd->blockSize-4],b);
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
    printf("\n\t next block is :%d",(int)btoi(&s[hh->hdd->nameblocksize-8]));
    strncpy(&s[hh->hdd->nameblocksize-4],c,4);

    printf("\t next name block is :%d",(int)btoi(&s[hh->hdd->nameblocksize-4]));
    ///////////
    printf("\n");
    for(int i=0;i<32;i++)
        printf("  %x",s[i]);

    ////////////////
    readBlock(hh,*returnBlock,&buf);
    int pos=(*returnNameBlock-1)*(hh->hdd->nameblocksize);
    for(int i=0;i<32;i++)
    {
        buf[pos+i]=s[i];
    }
    //strcpy(&buf[(*returnNameBlock-1)*(hh->hdd->nameblocksize)],s);
    //strncpy(s,&buf[(*returnNameBlock-1)*(hh->hdd->nameblocksize)],32);
    
    //printf("\n next name block is :%d",(int)btoi(&s[hh->hdd->nameblocksize-4]));
    printf("\n\n\nnext block block is:%ld",btoi(&buf[((*returnNameBlock-1)*(hh->hdd->nameblocksize))+24]));
    printf("\n\n\nnext name block is:%ld",btoi(&buf[((*returnNameBlock-1)*(hh->hdd->nameblocksize))+28]));
    
    /*for(int i=0;i<32;i++)
        printf("  %x",buf[((*returnNameBlock-1)*(hh->hdd->nameblocksize))+i]);
    */
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
printf("block:%d,nameblock:%d",block,nameblock);

//char *c;

//c=(char*)malloc(sizeof(char)*(hh->hdd->nameblocksize-8)*level);
//free(*returnName);
//strcat(c,*returnName);
//*returnName=c;

printf("\n\nok");
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

int displayNames(hd *hh)
{
    
}
