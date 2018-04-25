typedef struct metadata metadata;
typedef struct filename filename;
typedef struct hdd hdd;
typedef struct hd hd;
struct hdd
{
 int size;
 int blockSize;
int noofblocks;
int metablock;
int nameblock;
int nameblocksize;
int metablocksize;
};

struct hd
{

hdd *hdd;
FILE *lf;
FILE *dp;
char *buff;
char **names;
};

