#include<string.h>
#include<stdlib.h>
#include"diskDriver.h"
int freesubblock(hd *hh,char **buf,int subblocksize,int *blk)
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


int allocateBlock(hd *hh,int internalBlockSize,int *returnBlock)
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
return 1;
}


int writemetablock(hd *hh,int mblock,int block,int nblock)
{
    fprintf(hh->lf,"\n\n--> writemetablock() %d  %d \n ",block,nblock);
    fflush(hh->lf);
    unsigned char *c;
    readBlock(hh,mblock);
    int mmblock,x,i;
    x= freesubblock(hh,&(hh->buff),8,&mmblock);
    if(x==0)
    {  mmblock=btoi(&(hh->buff)[20]);
        if(mmblock==0)
        {  char buf1[hh->hdd->blockSize];
            for(i=0;i<hh->hdd->blockSize;i++)
            {
                buf1[i]=(hh->buff)[i];
            }
            x=allocateBlock(hh,8,&mmblock);
            if(!x)
                return 0;
            c=(unsigned char*)&mmblock;
            strcpy(&(buf1)[20],c);
            writeBlock(hh,mblock,buf1); 
        }
        return writemetablock(hh,mmblock,block,nblock);
    } 
    int pos=(mmblock-1)<<3;
    c=(unsigned char*)&block;
    strcpy(&((hh->buff)[pos]),c);
    c=(unsigned char*)&nblock;
    strcpy(&((hh->buff)[pos+4]),c);
    writeBlock(hh,mblock,hh->buff);
    return 1;
}



int writenameblocks(hd *hh,int block,char *buf,char **names,int totalNames,int nextBlock,int nextNameBlock,int *returnBlock,int *returnNameBlock)
{   int i;
    fprintf(hh->lf,"--> writenameblocks1() %d  %d",nextBlock,nextNameBlock);
    fflush(hh->lf);
    if(totalNames<=0)
    {   writeBlock(hh,block,buf); 
        *returnBlock=nextBlock;
        *returnNameBlock=nextNameBlock;
        return 1;
    }
    int flag=freesubblock(hh,&buf,hh->hdd->nameblocksize,returnNameBlock);
    if(!flag)
    {  
        int blk;
        blk= btoi(&buf[hh->hdd->nameblocksize-4]);
        if(blk==0)
        {   
        char buf1[hh->hdd->blockSize];
            for(i=0;i<hh->hdd->blockSize;i++)
            {
                buf1[i]=buf[i];
            }
            allocateBlock(hh,hh->hdd->nameblocksize,&blk);
            char *b;
            b=(char*)&blk;
            strcpy(&buf1[hh->hdd->nameblocksize-4],b);
        writeBlock(hh,block,buf1);
        }
        else
        {
        writeBlock(hh,block,buf);
        }
            readBlock(hh,blk);
            return  writenameblocks(hh,blk,hh->buff,names,totalNames,nextBlock,nextNameBlock,returnBlock,returnNameBlock);
    }
    unsigned char s[hh->hdd->blockSize];
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
    return writenameblocks(hh,block,buf,names,totalNames-1,block,*returnNameBlock,returnBlock,returnNameBlock);

}



int writename(hd *hh,char *name)
{
    fprintf(hh->lf,"\n\n--> writename() name:%s\n ",name);
    fflush(hh->lf);
    int n,i;
    char **names;
    int block,nblock;
    split(name,hh->hdd->nameblocksize-8,&(hh->names),&n);
readBlock(hh,hh->hdd->nameblock);
    writenameblocks(hh,hh->hdd->nameblock,hh->buff,hh->names,n,0,0,&block,&nblock); 
    return  writemetablock(hh,hh->hdd->metablock,block,nblock);
}

int getName(hd *hh,char *buf,int block,int nameblock,int level,char *returnName)//level  by default 1
{
    fprintf(hh->lf,"-->getName() %d  %d",block,nameblock);
    fflush(hh->lf);
    strncat(returnName,&buf[(nameblock-1)*hh->hdd->nameblocksize],hh->hdd->nameblocksize-8);
    int nextblock=btoi(&buf[nameblock*hh->hdd->nameblocksize-8]);
    int nextnameblock=btoi(&buf[nameblock*hh->hdd->nameblocksize-4]);
    if(nextblock==0)
        return 0;
    if(nextblock!=block)
    {
    readBlock(hh,nextblock);
    }
    return getName(hh,hh->buff,nextblock,nextnameblock,level+1,returnName);
}


void displayNames1(hd *hh,char *buf,int metablock)
{
    fprintf(hh->lf,"\n--> displayNames1111()\n ");
    fflush(hh->lf);
    char str[1000];
    str[0]='\0';
    int block=btoi(&buf[metablock*8]);
    if(block==0)
        return; 
    readBlock(hh,block);
    getName(hh,hh->buff,block,btoi(&buf[(metablock*8)+4]),1,&str[0]);
    printf("\n%s",str);
    return;
}
int displayNames(hd *hh,int mblock)
{
    fprintf(hh->lf,"\n\n--> displayNames()\n ");
    fflush(hh->lf);
    char buf[hh->hdd->blockSize];
    int i,j,bit,byte;
    readBlock(hh,mblock);
    for(i=0;i<hh->hdd->blockSize;i++)
        buf[i]=(hh->buff)[i];
    bit=hh->hdd->blockSize/8;
    byte=bit/8;
    bit=bit%8;
    for(j=3;j<8;j++)
    {
        if(((buf[0]>>j)&1))
        {
            displayNames1(hh,buf,j);
        }
    }
    for(i=1;i<byte;i++)
    {
            for(j=0;j<8;j++)
            {   
                if(((buf[i]>>j)&1)==1)
                {
                    displayNames1(hh,buf,j+(i*8));
                }
            }
    } 
    for(j=0;j<bit;j++)
    {   
        if(((buf[byte]>>j)&1)==1)
        {
            displayNames1(hh,buf,j+(i*8));
        }
    }
   i= btoi(&buf[20]);
   if(i!=0)
   {
        displayNames(hh,i);
   }
return 0;
}

int searchName(hd *hh,char *filename,int mblock,int *mmblock,int *submblock,int *nblock,int *subnblock)
{
    fprintf(hh->lf,"\n\n--> searchName()\n ");
    fflush(hh->lf);
    char buf[hh->hdd->blockSize];
    char str[1000];
    int i,j,bit,byte,block,nnblock;
    readBlock(hh,mblock);
    for(i=0;i<hh->hdd->blockSize;i++)
        buf[i]=(hh->buff)[i];
    bit=hh->hdd->blockSize/8;
    byte=bit/8;
    bit=bit%8;
    for(j=3;j<8;j++)
    {
        if(((buf[0]>>j)&1))
        {
    str[0]='\0';
    block=btoi(&buf[j*8]); 
    readBlock(hh,block);
     nnblock=btoi(&buf[(j*8)+4]);
    getName(hh,hh->buff,block,nnblock,1,&str[0]);
        if(strcmp(filename,&str[0])==0)
        {
            *mmblock=mblock;
            *submblock=j;
            *nblock=block;
            *subnblock=nnblock;
            return 1;
        }
        
        }
    }
    for(i=1;i<byte;i++)
    {
            for(j=0;j<8;j++)
            {   
                if(((buf[i]>>j)&1)==1)
                {
                
    str[0]='\0';
    int block=btoi(&buf[(j+i*8)*8]);
    readBlock(hh,block);
     nnblock=btoi(&buf[((j+i*8)*8)+4]);
    getName(hh,hh->buff,block,nnblock,1,&str[0]);
        if(strcmp(filename,&str[0])==0)
        {
            *mmblock=mblock;
            *submblock=j+(i*8);
            *nblock=block;
            *subnblock=nnblock;
            return 1;
        }
                
                }
            }
    
    } 
    for(j=0;j<bit;j++)
    {   
        if(((buf[byte]>>j)&1)==1)
        {
    str[0]='\0';
    int block=btoi(&buf[(j+i*8)*8]);
    readBlock(hh,block);
    nnblock=btoi(&buf[((j+i*8)*8)+4]);
    getName(hh,hh->buff,block,nnblock,1,&str[0]);
        if(strcmp(filename,&str[0])==0)
        {
            *mmblock=mblock;
            *submblock=j+i*8;
            *nblock=block;
            *subnblock=nnblock;
            return 1;
        }
    }
    }  
    i= btoi(&buf[20]);
   if(i!=0)
   {    
        return searchName(hh,filename,i,mmblock,submblock,nblock,subnblock);
   }
return 0;
    }

void deleteName(hd *hh,int nblock,int snblock,char *buff)
{
    fprintf(hh->lf,"\n\n--> deleteName()%d %d ",nblock,snblock);
    fflush(hh->lf);
    buff[snblock/8]&=(255-(int)(1<<(snblock%8)));
    int nextblock=btoi(&buff[(snblock*hh->hdd->nameblocksize)+hh->hdd->nameblocksize-8]);
    if(nextblock!=0)
    {
        if(nextblock!=nblock)
    {   writeBlock(hh,nblock,buff);
        readBlock(hh,nextblock);
        deleteName(hh,nextblock,btoi(&buff[(snblock*hh->hdd->nameblocksize)+hh->hdd->nameblocksize-4]),hh->buff);
    
    }
    else
    {
        deleteName(hh,nblock,btoi(&buff[(snblock*hh->hdd->nameblocksize)+hh->hdd->nameblocksize-4]),buff);
    
    }
    }
}

int deleteFileName(hd *hh,char *filename)
{
    
    fprintf(hh->lf,"\n\n--> deleteFile()\n ");
    fflush(hh->lf);
    int mblock,smblock,nblock,snblock;
    int x=searchName(hh,filename,hh->hdd->metablock,&mblock,&smblock,&nblock,&snblock);
    if(!x)
    {
        return 0;
    }

    readBlock(hh,mblock);
    int byte=smblock/8;
    int bit=smblock%8;
    (hh->buff)[byte]&=(255-(int)(1<<bit));
    writeBlock(hh,mblock,hh->buff);
    readBlock(hh,nblock);
    deleteName(hh,nblock,snblock,hh->buff);
return 1;
}

int replace(hd *hh,char *to,char *by)
{
    int x=deleteFileName(hh,to);
    if(x!=0)
    {
    writename(hh,by);
    return 1;
    }
return 0;

}
