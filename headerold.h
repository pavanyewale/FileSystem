int reset(char *hddname);
int createDisk(char *name,int size,int blocksize)
void printb(hd *hh,int bno)
int loadhdd(char *hname,hd **h)
int readBlock(hd *hh,int blockno,char **buf)
int writeBlock(hd *hh,int blockno,char *buf)
int setBlock(hd *hh,unsigned long blockno)
int freeBlock(hd *hh,unsigned long blockno)
int isFreeBlock(hd *hh,unsigned long blockno)
int getFirstFreeBlock(hd *hh, int *block)
int freesubblock(hdd *h,char **buf,int subblocksize,int *blk)//working 100% correct tested
int allocateBlock(hd *hh,int internalBlockSize,int *returnBlock)//working tested
int writemetablock(hd *hh,int mblock,int block,int nblock)
int writenameblocks(hd *hh,int block,char **names,int index,int totalNames,int *returnBlock,int *returnNameBlock)
int writename(hd *hh,char *name)
int getName(hd *hh,char *buf,int block,int nameblock,int level,char **returnName)//level  by default 1
int displayNames(hd *hh,int mblock)
long int btoi(unsigned char *c)
int split(char *str,int no,char ***result,int *n)
