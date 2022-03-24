#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CARACTERS_NUMBER 255 //number of carac of table ascci

void getfrequencebytes(FILE *input,int unsigned *listbytes);

typedef unsigned char byte; //8 bits -> table ascci

int unsigned bytelist[CARACTERS_NUMBER];

int main(int argc,char *argv[])
{
        if (argc != 2 ){
                printf("usage huffman [opition] [arquive]\n");
                return 1;
        }
        
        char *filename = argv[1];    
        FILE *input = fopen(filename,"r");
        
        if (input == NULL)
        {
            printf("not cloud open %s\n",filename);    
            return 2;
        }

        getfrequencebytes(input,bytelist);        
        for (int i=0;i<CARACTERS_NUMBER;i++){
                printf("%c:%d\n",i,bytelist[i]);
        }
}

void getfrequencebytes(FILE *input,int unsigned *listbytes)
{
    byte c;    
    while( ( fread(&c,sizeof(byte),1,input) ) >= 1 ) {
        listbytes[(int) c]++;
    }
}
