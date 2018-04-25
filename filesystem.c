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
        code=createDisk("Disk4",1024*1024*1024,2048,32,8);
       hd *h;
       loadhdd("Disk4",&h);

    fprintf(h->lf,"--> main function()\n ");
    fflush(h->lf);
       int n;
       char **str;
       char *buf;
       int blk,nblk;
      int mb,smb,nb,snb;
displayNames(h,h->hdd->metablock);
      for(int i=0;i<10;i++)
      {
       writename(h,"smart Boy");
      }
        writename(h,"pavan yewaleksjf kfj l;adjfa; ldjf ;laskdjf ;lakjdf ;alskjd f;lakjsd ;fkajas;ld fjk a;sldjf a;ljsd f;lakjs d;lf jal;ksd jf;lajsdf;lkakjsd;lf jka;ld fjal; kjfals djfl;ajsdf a;lsdkfja;lksdjf a;lksdjfk;lajfioweirjqwioerhjmn laijs[oierj;lwmrweo j hiii");
        writename(h,"pavan yewaleksjf kfj l;adjfa; ldjf ;laskdjf ;lakjdf ;alskjd f;lakjsd ;fkajas;ld fjk a;sldjf a;ljsd f;lakjs d;lf jal;ksd jf;lajsdf;lkakjsd;lf jka;ld fjal; kjfals djfl;ajsdf a;lsdkfja;lksdjf a;lksdjfk;lajfioweirjqwioerhjmn laijs[oierj;lwmrweo j hiii");
        writename(h,"pavan yewaleksjf kfj l;adjfa; ldjf ;laskdjf ;lakjdf ;alskjd f;lakjsd ;fkajas;ld fjk a;sldjf a;ljsd f;lakjs d;lf jal;ksd jf;lajsdf;lkakjsd;lf jka;ld fjal; kjfals djfl;ajsdf a;lsdkfja;lksdjf a;lksdjfk;lajfioweirjqwioerhjmn laijs[oierj;lwmrweo j hiii");
        writename(h,"pavan yewaleksjf kfj l;adjfa; ldjf ;laskdjf ;lakjdf ;alskjd f;lakjsd ;fkajas;ld fjk a;sldjf a;ljsd f;lakjs d;lf jal;ksd jf;lajsdf;lkakjsd;lf jka;ld fjal; kjfals djfl;ajsdf a;lsdkfja;lksdjf a;lksdjfk;lajfioweirjqwioerhjmn laijs[oierj;lwmrweo j hiii");
      /*  displayNames(h,h->hdd->metablock);
        int x=replace(h,"pavan yewale hiii","pavanyewale you are fool............");
        if(x!=0)
        {
        displayNames(h,h->hdd->metablock);
        }
        else{
        printf("\n Not found... to replace");
        }
*/
 }
