void copy(char *from,char *to,int n)
{	
	int i;
	if(n<0)
		return;
	for(i=0;i<n;i++)
	{
		to[i]=from[i];
	}
}

int createStyle(hd *hh,char *name,char type) //name size should be less than 59
{
    fprintf(hh->lf,"--> createStyle(%s) ",name);
    fflush(hh->lf);
	hdd *h;
	dir *d;
	h=hh->hdd;
	d=hh->dir;
	int i,blockno,k,p,freeblock;
	char buff[h->blockSize];
	if(strlen(name)>59)
			return -1;//name size is more than 59 characters
	for(i=0;i<d->sc;i++)
	{
		if(strcmp((char*)&d->ls[i][5],name)==0)
				return -2;//style allready exists

	}
	k=h->blockSize/64-1;
	unsigned char *c;
	blockno=d->stack[d->sp];
	fprintf(stdout,"blockno=%d",blockno);
	while(1)
	{
		readBlock(hh,blockno);
		for(i=0;i<k;i++)
		{	p=i*64;
			if(hh->buff[p]==0x00)
			{	
				copy(hh->buff,buff,h->blockSize);
				getFirstFreeBlock(hh,&freeblock);
				buff[p]=type;
				c=(unsigned char*)&freeblock;
				strncpy((char*)&buff[p+1],c,4);
				strcpy((char*)&buff[p+5],name);
				copy((char*)&buff[p],(char*)&d->ls[d->sc],64);
    			fprintf(hh->lf,"--> writing directory **%d*** ",i);
    			fflush(hh->lf);
				writeBlock(hh,blockno,buff);
				setBlock(hh,freeblock);
				d->sc++;	
				return freeblock;
			}
		}
		freeblock=btoi((unsigned char*)&hh->buff[h->blockSize-5]);

		if(freeblock==0)
		{	copy(hh->buff,buff,hh->hdd->blockSize);
			getFirstFreeBlock(hh,&freeblock);
			c=(unsigned char*)&freeblock;
			strcpy((char*)&buff[h->blockSize-5],c);
			writeBlock(hh,blockno,buff);
			setBlock(hh,freeblock);
			memset(hh->buff,0x0,h->blockSize);
			writeBlock(hh,freeblock,hh->buff);
		}
		blockno=freeblock;
	}

	
}
int createFile(hd *hh,char *name,char *data,int datalength)
{
    fprintf(hh->lf,"--> createfile() ");
    fflush(hh->lf);
	int blockno,freeblock;
	unsigned char *c;
	blockno=createStyle(hh,name,'f');
	if(blockno<0)
			return blockno;
	while(1)
	{
		if(datalength<(hh->hdd->blockSize-4))
		{	
			memset(hh->buff,0x0,hh->hdd->blockSize);
			copy(data,hh->buff,datalength);
			writeBlock(hh,blockno,hh->buff);
			return 1;
		}
		else
		{
			copy(data,hh->buff,hh->hdd->blockSize-4);
			getFirstFreeBlock(hh,&freeblock);
			c=(unsigned char *)&freeblock;
			strncpy((char*)&hh->buff[hh->hdd->blockSize-5],c,4);
    			fprintf(hh->lf,"--> writing directory ***** ");
    			fflush(hh->lf);
			
			writeBlock(hh,blockno,hh->buff);
			setBlock(hh,freeblock);
			blockno=freeblock;
			data+=hh->hdd->blockSize-4;
			datalength-=hh->hdd->blockSize-4;
		}
	}
}
int createDir(hd *hh,char *name)
{
    fprintf(hh->lf,"-->createDir() ");
    fflush(hh->lf);
	int blockno=createStyle(hh,name,'d');
	if(blockno<0)
			return blockno;
	memset(hh->buff,0x0,hh->hdd->blockSize);
	writeBlock(hh,blockno,hh->buff);
	return 1;
}
void liststyles(hd *hh)
{	int i;
    fprintf(hh->lf,"--> listStyles(noofstyles:%d) ",hh->dir->sc);
    fflush(hh->lf);
	dir *d;
	d=hh->dir;
	fprintf(stdout,"\n Current Dir:%s\t No of Styles:%d",d->currdir,d->sc);
	for(i=0;i<d->sc;i++)
	{
		fprintf(stdout,"\n %c :%s",d->ls[i][0],(char*)&d->ls[i][5]);
	}
}
void addStyles(hd *hh,int blockno)
{
    fprintf(hh->lf,"--> addStyles(%d) ",blockno);
    fflush(hh->lf);
	int i,k,p;
	dir *d;
	d=hh->dir;
	d->sc=0;
	k=(hh->hdd->blockSize/64)-1;
	while(1)
	{
	readBlock(hh,blockno);
		for(i=0;i<k;i++)
		{
			p=i*64;
			if(hh->buff[p]!=0x0)
			{
    				fprintf(hh->lf,"adding:%d[%d] at %d ",blockno,i,d->sc);
    				fflush(hh->lf);
				
				copy((char *)&hh->buff[p],d->ls[d->sc],64);
				d->sc++;
			}
		}
		blockno=btoi((unsigned char *)&hh->buff[hh->hdd->blockSize-5]);
		if(!blockno)
				return;
	}
}
int changedir(hd *hh,char *name)
{
    fprintf(hh->lf,"--> changedir(%s) ",name);
    fflush(hh->lf);
	dir *d;
	d=hh->dir;
	if(strcmp("..",name)==0)
	{
		if(d->sp>0)
		{
			d->sp--;
			addStyles(hh,d->stack[d->sp]);
		}
		strcpy(d->currdir,"..back");
		return 1;
	}
	int i,blockno=-1;
	for(i=0;i<d->sc;i++)
	{
		if(d->ls[i][0]=='d' && strcmp((char*)&d->ls[i][5],name)==0)
		{
			d->sc=0;
			d->sp+=1;
			d->stack[d->sp]=btoi((unsigned char *)&d->ls[i][1]);
			strcpy(d->currdir,(char*)&d->ls[i][5]);
			addStyles(hh,d->stack[d->sp]);
			return 1;
		}
	}
	return -1;
}
int showFile(hd *hh,char *name)
{
	int i,blockno;
	dir *d;
	d=hh->dir;
	for(i=0;i<d->sc;i++)
	{
		if(d->ls[i][0]=='f' && strcmp((char*)&d->ls[i][5],name)==0)
		{
				blockno=btoi((char*)&d->ls[i][1]);
				printf("\n **Start of File**\n");
				while(blockno)
				{
					readBlock(hh,blockno);
					blockno=btoi((char*)&hh->buff[hh->hdd->blockSize-5]);
					hh->buff[hh->hdd->blockSize-5]='\0';
					printf("%s",hh->buff);
				}
				printf("\n *******End of File ******");
		return 1;		
		}
	}
	return -1;

}
int deleteStyle(hd *hh,char *name)
{
	
}
void deleteFile(hd *hh,int blockno)
{
	while(1)
	{
		freeBlock(hh,blockno);
		readBlock(hh,blockno);
		blockno=btoi((char*)&hh->buff[hh->hdd->blockSize-5]);
		if(blockno==0)
			return;
	}
		
}
