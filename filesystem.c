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
readBlock(hh,bno,&buff);
int i;
printf("\n");
for(i=0;i<h->blockSize;i++)
    printf("%c",buff[i]);
free(buff);
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
      readBlock(h,131,&buf);
      printf("block:%d  nameblock:%d ",(int)btoi(&buf[
      56]),(int)btoi(&buf[60]));
free(buf);
     // getName(h,buf,131,30,1,&strr);
      //printf("\n%s",strr);
      //return ;
    /*readBlock(h,h->hdd->nameblock,&buf);
    freesubblock(h,&buf,32,&blk);
    printf("\nfree:%d",blk);
    freesubblock(h,&buf,32,&blk);
    printf("\nfree:%d",blk);
    freesubblock(h,&buf,32,&blk);
    printf("\nfree:%d",blk);
    freesubblock(h,&buf,32,&blk);
    printf("\nfree:%d",blk);
    freesubblock(h,&buf,32,&blk);
    printf("\nfree:%d",blk);
     n
      return;*/
      //displayNames(h,129);
         // writename(h,"pavaakljf alksdjf l;akjdf lajd f;lajkfalkj f;lajd faljdf ;lajfl;a n yewale"); 
    //writename(h,"aksdjf alkjd flakjd fladkjf ;alkdjflakdjfl ;akjfl adkjflakjdf alkdjfl;ajdflakjdfj adlfj al;dfja;ldjfl;adkjf l;askjfl;adkjf;l adkjfl;askdjf;l asd.kjf alsdkjf ;lajdfl;akjdf l;asjdf lakd jf;la kjdfl ajdlfkjasdl;kfja l;sdkjf al;sdkjf asdjflsadkj fl;akdjf ;sdkjf ;lasdkjf ;lakdsjf;laksdjf ;lakdjf ;laksjdf;l ajdlfk al;kdfj a;lskdjf ;laksdjf lajsdf;lkajsd fl;kajs;ldkfjas;lkdfj a;lskdfj l;aksjdfl;kajsd;lfkajsdl;fkj al;dkfja;ldsfk.jaldskfj asl;dkfj a;lsd kfjalskdfjf a;lsdkfja dlfjkajdfsjdf");

       writename(h,"he y gays jwell come thklajdf jthei ljkadljfald fprintf hh ld allocae block blockssize usnginde char return blck kljasdlkfj ,jfakldjflakjdfiowrowuiopeuqpoeiruqpoeiruqopeiurqopieurqpoiewurqpoiwelurpqoiwleurqoiwelruqpowieruqpoweiurqpoiewurqpieryiouqryewqiutrqeryqiuweriqoeruqoewurqiuerqpoieruqiwerqoweruqpoiryqpioeruqpioewurqpioweurpiqewyriquyrqpiorueqioeurqoiuweroiqueroiqeurqioweuroqieurqopweurqpoweireupqoweiruqpoweirupoeiruqpoweiurrpqoewiurqpoeiwurqoiewurpqoieurqoeiruqldkjf;lajfladkfjaldfjladkjfladkjfaldkjfaldkjfalkjfla;djflakdjfa;ldjf;az.,vmnz,mvznxcv,makljdalkdsfl lf;kfaj dlfkjal;dfjal;sd jflk;asdj fl;kadsjf lk;asdj f;laksdjf l;askdjf f;ladkjf ;alskdjf a;lksdjf a;lsdkjf a;lskdjf a;lskdjfflkdsj f;laskdjf lak.jfa lkfdjalskjdfalksdfj alskdjff alksdjf ;alksdjf ;alksjd fa;lsk.jdf falk.sjdf ;lakd.sjf a;lkds.jf ;alksdjf ak;lsdfja kl;sdjf als;kdfja sdlfkjasldk;jf m,zxcnv,zxncv,mznv,zxmnv ,zmvn z,cmvnzx,nv.zvn.zvnz.,xmcvn z,.xcvnz,.cnv,.zxvncz,xnv,zxmvnz,xmcvnz,xcmvfkaljdf al;dkfj a;lksjf ;laskdjf ;laksdjf ;laksjd ;flaksjdf; lasdjf ;lasdj f;alsklkasjd f;lkasdj fklasdj flkjasdklf jaslk;dfj akl;sdjf akl;sdjf k;lasdj fk;lasdj fk;lasdjfkl; asjdk;lf ajsdk;lfj askl;dfj alk;sdjf a;lksdjf al;ksdjf akls;djfkald fkl;asd jfkl;asdj fk;lasdj flkm ;ldkjf alkjdf ladkj flakjdf lajfeeirpl;ejr ern,rf nkjjfl,.fnef jerlk.fns,f ofilek.ljf nme,f l;.,nfk,.a,djnfadklf.jnadf,m adfkla,.d jfskadl,mfh asdfk,aj dlf.,adlkjfasjdnfjalkd,fhnadkjf,mbmadsnfkj,sdmfn kj a,mejfhnejmnfbeklkj,hefolkerhjfisjkdfm,gnvsolrk.fjdhnsiodfkjh snirkldfshnr oifdks,hngflksj,hdfn owskjlrhdfnaildkjfhanklsdfj,halkdfjhalkf,hiekhqoierhweoifhkdvnbcvoielkdfshnierfhsdkjn,misdfkjlnkfdsjn,efskjdlmkjsfd,mkjsfd,mnkjsfdkjdfslnmblrakf,cnsrvkbsjlrmhjsbjalknjgkldwsgjkdfllajadhfklajsdflkamfhfkhajkfhv nzcvnlfjkhfkl,anfjhakdjnf,madnlkajhnfalkcjh,anf");    
     return ; 
      //displayNames(h,129);
       /*for(int i=0;i<100;i++)
            {writename(h,"Ashish pawar is scientist...!"); 
            }*/
    //return ;
    readBlock(h,148,&buf);
   // for(int i=0;i<24;i++)
    //{
      //  printf("%c",28*32+i);
    //}
    char *st;
    getName(h,buf,148,32,1,&st);
    //printb(h,134);
    printf("%s",st);
    return;
    //displayNames(h,129);
    for(int i=0;i<10;i++)
    writename(h," bsjkfahsj kfhak,jfhbkjm,asbdfajksdfhonakf jhan ekjf,haniulenlkaef hllakdsfjkl aldskfj aldksjf alksdjf a;lkdsjf a;lksdjf a;lkdjf a;lkdj fwkefh wnlkfnlker,fh alke.fanelkfd ab yeale this is ltest mab=");
    //writenameblocks(h,130,str,0,n,&blk,&nblk);
        printf("block for name:%d   name block:%d",blk,nblk);
   //for(int i=0;i<75;i++)
        //writemetablock(h,129,140,30);
     
        //allocateBlock(h,8,&blk);
        //printb(h,blk);
        //readBlock(h,blk,&buf);
        //freesubblock(h->hdd,&buf,8,&nblk);
                printf("first free block is %d",nblk);
        
        allocateBlock(h,32,&blk);
        printb(h,blk);

       
       for(int i=0;i<10;i++)
       {
       writename(h,"pavanlajdlfjlkf ladj f;lkaj f;lkaj fkl;adj fk;lajdf asjdlk;f asdkl;f ajf alj yewale this is hello world programlkasdjfljdlfj lkdjf aldjf aldkjfa djflkajd f;lasdjkf al;kdjf a;ldkjf ;lksdjf aldkjf alsdkjf ;alkdjf a;lksdjf a;lksdjf kasdfjalksd jfa;kldfj a;ldkfj a;lksdfj a;lksdjf a;lkdfja l;ksdjf a;lksdjf al;kdjf ed");

    }

   readBlock(h,129,&buf);
   printf("\n");
   for(int i=0;i<3;i++)
   {
    for(int j=0;j<8;j++)
        printf("%d",(buf[0]>>j)&1);
   }

   printb(h,129);

 }
