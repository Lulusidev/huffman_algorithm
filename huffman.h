#define CARACTERS_NUMBER 256 //number of carac of table ascci

void getfrequencebytes(FILE *input,int unsigned *listbytes);
bool SearchAscciByte(tree *n,byte c,char *buffer,int size);
tree createnewtree(byte c,int freq,tree *left,tree *rigth);
void InsertOrdList(nodelist *l,tree *n);
void PrintList(nodelist *l);
void list_tab(unsigned int tab_freq[],nodelist *l);


typedef unsigned char byte; //8 bits -> table ascci

typedef struct tree{
        int frequence;
        byte c;
        struct tree *nodeleft;
        struct tree *noderight;
        struct tree *next
}tree;

typedef struct nodelist{
        tree *head;
        int size;        
}nodelist;

int unsigned bytelist[CARACTERS_NUMBER] = {0};
