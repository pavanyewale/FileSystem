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
       int n;
       char **str;
       char *buf;
       int blk,nblk;
      waitforme(2);
    writename(h,"hello world");
    writename(h,"hello world");
    writename(h,"hello world");
    writename(h,"hello world");
    writename(h,"hello world");
    writename(h,"hello world");
    writename(h,"hello world");
    writename(h,"hello world");
    writename(h,"hello world");
    writename(h,"hello world");
    writename(h,"hello world");
    writename(h,"hello world");
    for(int i=0;i<10;i++)
    {
    writename(h,"pavan lkjadslfj ad;lfkja d;lkfj a;lskdjf a;lksdjf kla;sdjf lk;asjd fk;lajsd f;lkajsdf akjsdf;lk ajsdlkf ja;lksdfj a;lksdjf alksdjf a;lksdjf a;lksdjf a;lksdjf,msdnfm,asdnmf ;lkasdjf;lasdkjf ;klasjdfl;kasjdf;l kajdf; lkajsd fkl;ajsdkl;fjasd;lkfja ;lsdkjfa;l skdjf ;alsdkjf a;lskjdf ;laskjdf ;lkasdjf klasdfm,.dsmfklsdnvklasdj fioajfiojw eflknf a,sdnf ;lkasdjflkajs d;lfmasd f,manodfjaeiofjqwioperuweioprue f,msadfnklasdnfoppejiqorj aekfmasdfpioawejfpoi weklfmoiwefjqpowefmewfiopwefjoq;lwejfm;k,aemfe pioaewjfioqmwefiwe jfpoqweiflqfj iyewjdflkal sdflkasdj f;lkasdj ff a;lksdf a;lksdjf a;lskdf a;lksdf ;alksdjf alskfj a;lskd fa;lksdfj a;lskdf ja;lskdf alksd fa;lksdjf a;lksd f;alksdfj a;lskdf a;lksjf ;alskdfj a;lskdfj a;lskdjf a;lsdkfja ;lskfdj a;ls dkf ja;lsd kfja;lsjdfal;sd fkja;aale this is first name of the file");
    }
      
    readBlock(h,130);
    //getName(h,h->buff,130,3,1,&buf);
    //printf("\n%s",buf);
//    printb(h,130);
   readBlock(h,129);
   //freesubblock(h,&(h->buff),8,&blk);
   //printf("freemeataBlockis:%d",blk);
   printf("block:%d",(int)btoi(&(h->buff[56])));
   displayNames(h,129);
 }
