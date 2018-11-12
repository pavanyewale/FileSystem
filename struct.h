typedef struct metadata metadata;
typedef struct filename filename;
typedef struct hdd hdd;
typedef struct hd hd;
typedef struct dir dir;
struct hdd
{
 int size;
 int blockSize;
int noofblocks;
int root;
};

struct hd
{

hdd *hdd;
FILE *lf;
FILE *dp;
char *buff;
dir *dir;
};


struct dir
{
	int stack[100];
	int sp;
	char ls[100][65]; //[type,start,name]
	int sc; //style count
	char currdir[59];
};

