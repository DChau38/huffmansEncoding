//Duksing Chau
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdint.h>
int debug=0;

typedef struct Node{
    char character;
    int frequency;
    struct Node* left;
    struct Node* right;

} Node;
typedef struct{
    Node **array;
    int size;
} PriorityQueue;

Node* createNode(char character, int frequency);
PriorityQueue* createPriorityQueue();
void insert(PriorityQueue *pq, Node* element);
Node* dequeue(PriorityQueue *pq);
void swap(Node **a, Node **b);
void freePriorityQueue(PriorityQueue *pq);
bool isEmpty(PriorityQueue *pq);
void bubbleUp(PriorityQueue *pq, int i);
void bubbleDown(PriorityQueue *pq, int i);
int getParentIndex(int i);

Node* createNode(char character,int frequency){
    Node* newNode=(Node*)malloc(sizeof(Node));
    newNode->character = character;
    newNode->frequency = frequency;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}
PriorityQueue* createPriorityQueue(){
    PriorityQueue *pq=(PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->size=0;
    pq->array=(Node**)malloc(256*sizeof(Node*));
    return pq;
}
void insert(PriorityQueue*pq, Node* element){
    pq->array[pq->size]=element;
    pq->size++;
    bubbleUp(pq,pq->size-1);
}
Node* dequeue(PriorityQueue*pq){
    Node* root=pq->array[0];
    pq->array[0]=pq->array[pq->size-1];
    pq->size--;
    bubbleDown(pq,0);
    return root;
}
void swap(Node **a, Node **b) {
    Node *temp = *a;
    *a = *b;         
    *b = temp;     
}

void freePriorityQueue(PriorityQueue*pq){
    free(pq->array);
    free(pq);
}
bool isEmpty(PriorityQueue*pq){
    return pq->size==0;
}
void bubbleUp(PriorityQueue *pq, int i) {
    while (i > 0 && pq->array[i]->frequency < pq->array[getParentIndex(i)]->frequency) {
        swap(&pq->array[i], &pq->array[getParentIndex(i)]);
        i = getParentIndex(i);
    }
}

void bubbleDown(PriorityQueue *pq, int i) {
    int smallest = i; 
    int leftChild = (i * 2) + 1; 
    int rightChild = (i * 2) + 2; 

    if (leftChild < pq->size && pq->array[leftChild]->frequency < pq->array[smallest]->frequency) {
        smallest = leftChild;
    }

    if (rightChild < pq->size && pq->array[rightChild]->frequency < pq->array[smallest]->frequency) {
        smallest = rightChild;
    }

    if (smallest != i) {
        swap(&pq->array[i], &pq->array[smallest]);
        bubbleDown(pq, smallest);
    }
}
int getParentIndex(int i){
    return (i-1)/2;
}
void generateCodes(Node* root, char* code, int codeLen, char codes[256][256]) {
    if (!root) {
        return; 
    }

    if (root->left == NULL && root->right == NULL) {
        code[codeLen] = '\0'; 
        strcpy(codes[(unsigned char)root->character], code); 
        if (debug) {
            if (root->character == '\n') {
                printf("LF                 %d  %s\n", root->frequency, code);
            } else if (root->character == ' ') {
                printf("SPACE              %d  %s\n", root->frequency, code);
            } else {
                printf("%c                  %d  %s\n", (unsigned char)root->character, root->frequency, code);
            }
        }
        return;
    }

    if (root->left) {
        code[codeLen] = '0';            
        generateCodes(root->left, code, codeLen + 1, codes); 
    }

    if (root->right) {
        code[codeLen] = '1';           
        generateCodes(root->right, code, codeLen + 1, codes); 
    }
}

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


