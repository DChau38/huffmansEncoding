//Duksing Chau
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdint.h>
#include "huffman.h"
int debug=0;


int main(int argc,char*argv[]) {
    //0. handle CLA
    int opt;
    //names of files
    char *inFileName=NULL;
    char *outFileName=NULL;
    while ((opt=getopt(argc,argv,"di:o:"))!=-1){
        //iterates through each character in CLA. If meeting condition, opt will be set to value and switch case will activate. Else, it will be ? and not. When it's finished going through all, will return -1
        switch(opt){
            case 'd':
                debug=1;
                break;
            case 'i':
                inFileName=(char*)malloc(sizeof(char)*(strlen(optarg)+1));
                strcpy(inFileName,optarg);
                break;
            case 'o':
                outFileName=(char*)malloc(sizeof(char)*(strlen(optarg)+1));
                strcpy(outFileName,optarg);
                break;
        }
    }
    //A. if inFile is NULL, use default file name for input
    if (inFileName==NULL){
        inFileName=(char*)malloc(sizeof(char)*(strlen("completeShakespeare.txt")+1));
        strcpy(inFileName,"completeShakespeare.txt");
    }
    //B. if outFile is NULL, use default file name for output
    if (outFileName==NULL){
        outFileName=(char*)malloc(sizeof(char)*(strlen("huffman.out")+1));
        strcpy(outFileName,"myHuffman.out");
    }
    //1. map charToFreq
    int charToFrequency[256]={0};
    FILE* inputFile=fopen(inFileName,"r");
    char c;
    while ((c=fgetc(inputFile))!=EOF){
        charToFrequency[(unsigned char)c]++;

    }
    //2. construct huffman tree
    PriorityQueue* pq = createPriorityQueue();
    for (int i=0;i<256;i++){
        if (charToFrequency[i]>0){
            Node* node=createNode((char)i, charToFrequency[i]);
            insert(pq,node);
        }
    }
    while (pq->size>1){
        Node *left=dequeue(pq);
        //printf("%c:%d\n",left->character,left->frequency);
        Node *right=dequeue(pq);
        //printf("%c:%d\n",right->character,right->frequency);
        //create intermediate node
        Node* intermediateNode=createNode('\0',left->frequency+right->frequency);
        intermediateNode->left=left;
        intermediateNode->right=right;
        insert(pq,intermediateNode);
    }
    //3. generate huffman codes
    char codes[256][256];//mapping of char->encoding
    char code[256]; //temporary list for making codes
    for (int i = 0; i < 256; i++) {
        codes[i][0] = '\0'; 
    }
    if (pq->size==1){
        Node*root=dequeue(pq);
        if (debug){
            printf("Input File: %s\n", inFileName);
            printf("Total combined frequency / number of bytes / length of file : %d\n",root->frequency);
        }
        generateCodes(root,code,0,codes);

    }
    //prints out codes
    /*if (debug){
    for (int i=0;i<256;i++){
        if(strlen(codes[i])>0){
            printf("%c: %s\n",i,codes[i]);
        }
    }
    }*/

    //4. iteratively convert -> output
    FILE *fpOut=fopen(outFileName,"wb");
    if (!fpOut){
        fprintf(stderr,"fail to open file\n");
        return 1;
    }
    //reset pointer
    rewind(inputFile);
    unsigned char byteBuffer=0; //holds 8 bits
    int bitCount=0; //tracks our position in our byteBuffer
    while ((c=fgetc(inputFile))!=EOF){
        char*matchingCode=codes[(unsigned char)c];
        //iterate through the matchingCode, add bits to byteBuffer
        for (int i=0;matchingCode[i]!='\0';i++){
            if (matchingCode[i]=='1'){
                //OR-EQUAL with byteBuffer, 00000001 bit-shifted a certain amonut
                byteBuffer |= (1<< (7-bitCount));
            }
            bitCount++;
            //byteBuffer is full -> reset
            if (bitCount==8){
                fwrite(&byteBuffer, sizeof(unsigned char),1, fpOut);
                byteBuffer=0;
                bitCount=0;
            }
        }
    }
    //account for any remaining bits
    if (bitCount>0){
        fwrite(&byteBuffer,sizeof(unsigned char),1,fpOut);
    }
    fclose(inputFile);
    fclose(fpOut);
    free(inFileName);
    free(outFileName);
    return 0;
}


