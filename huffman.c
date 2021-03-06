#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "huffman.h"
#include <stdbool.h>
#include <math.h>

typedef unsigned char byte; //8 bits -> table ascci

typedef struct nodetree{
        int frequence;
        byte c;
        struct nodetree *left;
        struct nodetree *rigth;
        struct nodetree *next;
}nodetree;

typedef struct nodelist{
        nodetree *head;
        int size;        
}nodelist;

#define CARACTERS_NUMBER 256

int unsigned bytelist[CARACTERS_NUMBER] = {0};

void getfrequencebytes(FILE *input,int unsigned *listbytes);
bool SearchAscciByte(nodetree *n,byte c,char *buffer,int size);
nodetree *createnewtree(byte c,int freq,nodetree *left,nodetree *rigth,nodetree *next);
void InsertOrdList(nodelist *l,nodetree *n);
void PrintList(nodelist *l);
void list_tab(unsigned int tab_freq[],nodelist *l);
nodelist *new_list(nodelist *l);
nodetree *getminelement(nodelist *l);
nodetree *BuildHuffmanTree(int unsigned *listBytes);
void FreeTree(nodetree *n);
void to_comprime(char *input_name,char *output_name);

int main(int argc,char *argv[])
{
        if (argc != 2 ){
                printf("usage huffman [opition] [arquive]\n");
                return 1;
        }
        
        char *filename = argv[1];    
        char *output_Filename; 
        sprintf(output_Filename,"%s.hf",filename);
        
        to_comprime(filename,output_Filename);

}

void getfrequencebytes(FILE *input,int unsigned *listbytes)
{
    byte c;    
    while( ( fread(&c,sizeof(byte),1,input) ) >= 1 ) {
        listbytes[(int) c]++;
    }

}

void to_comprime(char *input_name,char *output_name)
{
//
        char buffer[1024] = {0};
        byte c;
        int size;
        int unsigned bytelist[CARACTERS_NUMBER] = {0}; 
        byte aux = 0;

        FILE *input = fopen(input_name,"r");

        if(input==NULL){
            printf("NOt Could open %s\n",input_name);
            return ;
        }
        FILE *output = fopen(output_name,"w");
        if(output == NULL){
            printf("Error 4\n");
            return ;
        }
        getfrequencebytes(input,bytelist);

        nodetree *huffmanTree = BuildHuffmanTree(bytelist);
        
        fwrite(bytelist,256,sizeof(bytelist[0]),output);
        
        while (fread(&c,1,1,input)>=1)
        {
                SearchAscciByte(huffmanTree,c,buffer,0);

                for (char *i = buffer;*i;i++){
                    if (*i == '1'){
                        //right
                        aux = aux + pow(2,size % 8);    
                    }
                        
                    size++;
                    if(size%8 == 0)
                    {
                        //left
                        fwrite(&aux,1,1,output);
                        aux = 0;
                    }
                }
        }

        fwrite(&aux,1,1,output);
        fwrite(&size,1,sizeof(int),output);
        

        double sizeOutput = ftell(output);
        double sizeInput = ftell(input);

        float compressTaxa = 100 * (sizeOutput/sizeInput);
        printf("Compress tax: %f",compressTaxa);
        
        FreeTree(huffmanTree);

        fclose(input);
        fclose(output);

}

void FreeTree(nodetree *n)
{
    if (!n) return;
    else
    {
        nodetree *left = n->left;
        nodetree *rigth = n->rigth;
        free(n);
        FreeTree(left);
        FreeTree(rigth);
    }
}

nodetree *createnewtree(byte c,int freq,nodetree *left,nodetree *rigth,nodetree *next)
{
        nodetree *newtree = malloc(sizeof(nodetree));
        newtree->c = c;
        newtree->frequence = freq;
        newtree->left = left;
        newtree->rigth = rigth;
        newtree->next =  next;
       
        return newtree;
}

bool SearchAscciByte(nodetree *n,byte c,char *buffer,int size){
    if( !(n->left || n->rigth) &&
        n->c == c){
        buffer[size] = '\0';
        return true;
    }
    else{
        bool find = false;

        if(n->left){
            buffer[size]='0';

            find = SearchAscciByte(n->left,c,buffer,size+1);
        }
        else if (!find && n->rigth){
            buffer[size]='1';
            find = SearchAscciByte(n->rigth,c,buffer,size+1);
        }
        if(!find){
            buffer[size] ='\0';
        }
        return find;
    }
}

nodelist *new_list(nodelist *l){
    l = malloc(sizeof(l));    
    l->head = NULL;
    l->size=0;
    return l;
}

void InsertOrdList(nodelist *l,nodetree *n){
        if (l->head == NULL)
        {
                l->head = n;
        //        list->size++;
        }
        else if (n->frequence < l->head->frequence){
                n->next = l->head;
                l->head = n;
        //        list->size++;
        }
        else{
                nodetree *aux2 = l->head;
                nodetree *aux = l->head->next;

                while( aux &&
                (aux2->next->frequence <= n->frequence) )
                {
                
                aux2 = aux;
                aux = aux2->next;
                
                }
                aux2->next = n;
                n->next = aux;
        //        list->size++;
                }    
        
        l->size++;
}
void list_tab(unsigned int tab_freq[],nodelist *l)
{
    nodetree *n;
    for (int i = 0 ; i<CARACTERS_NUMBER ; i++){
        if(tab_freq[i] > 0){
                n = createnewtree(i,tab_freq[i],NULL,NULL,NULL);
                InsertOrdList(l,n);
        }
    }        

}

nodetree *getminelement(nodelist *l){
        nodetree *aux = l->head;
        nodetree *aux2 = aux->next;
        
        l->head = aux->next;
        free(aux);
        aux=NULL;

        l->size--;
        return aux2;
}

nodetree *BuildHuffmanTree(int unsigned *listBytes){
        //get min frequence
        //make tree of this element 
        //
        //
        nodelist *l = malloc(sizeof(nodelist)); 
        l = new_list(l);      
        for (int i = 0; i < CARACTERS_NUMBER;i++){
                if(listBytes[i])
                {
                        nodetree *n = createnewtree(i,listBytes[i],NULL,NULL,NULL);
                        InsertOrdList(l,n);
                }
        }

        while(l->size > 1)
        {
                nodetree *nleft = getminelement(l);
                nodetree *nrigth = getminelement(l);
                
                        
                int sum_of_frequences = nleft->frequence+nrigth->frequence;
                nodetree *sum_nodes = createnewtree(
                         '#',sum_of_frequences,
                        nleft,nrigth,NULL
                        );
                InsertOrdList(l,sum_nodes);
        }
        return l->head;
}


