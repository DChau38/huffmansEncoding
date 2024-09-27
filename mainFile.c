#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>

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
    Node *temp = *a; // Use a temporary pointer to hold one of the nodes
    *a = *b;         // Swap the pointers
    *b = temp;      // Assign the temp pointer to the second pointer
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
    int smallest = i; // Start with the current index as the smallest
    int leftChild = (i * 2) + 1; // Left child index
    int rightChild = (i * 2) + 2; // Right child index

    // Check if left child exists and is smaller than current smallest
    if (leftChild < pq->size && pq->array[leftChild]->frequency < pq->array[smallest]->frequency) {
        smallest = leftChild;
    }

    // Check if right child exists and is smaller than current smallest
    if (rightChild < pq->size && pq->array[rightChild]->frequency < pq->array[smallest]->frequency) {
        smallest = rightChild;
    }

    // If smallest is not the current index, swap and continue bubbling down
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
        return; // Base case: If root is NULL, return
    }

    // Check if the node is a leaf node (i.e., it contains a character)
    if (root->left == NULL && root->right == NULL) {
        code[codeLen] = '\0'; // Null-terminate the string
        strcpy(codes[(unsigned char)root->character], code); // Store the code for this character
        return;
    }

    // Traverse left (append '0' to the code)
    if (root->left) {
        code[codeLen] = '0';            // Add '0' to the code buffer
        generateCodes(root->left, code, codeLen + 1, codes); // Recursively go left
    }

    // Traverse right (append '1' to the code)
    if (root->right) {
        code[codeLen] = '1';            // Add '1' to the code buffer
        generateCodes(root->right, code, codeLen + 1, codes); // Recursively go right
    }
}

int main(int argc,char*argv[]) {
    //0. handle CLA
    int opt;
    int debug=0;
    char *inputFile=NULL;
    char *outputFile=NULL;
    while ((opt=getopt(argc,argv,"di:o:"))!=-1){
        switch(opt){
            case 'd':
                debug=1;
                break;
            case 'i':
                inputFile=optarg;
                break;
            case 'o':
                outputFile=optarg;
                break;
        }
    }
    //1. map charToFreq
    int charToFrequency[256]={0};
    char*inputStr="abbccccdddddddddd";
    for (int i=0;i<strlen(inputStr);i++){
        charToFrequency[(unsigned char)inputStr[i]]++;
    }
    if (debug){
        for (int i=0;i<strlen())
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
        codes[i][0] = '\0'; // Set first character to null terminator
    }
    if (pq->size==1){
        Node*root=dequeue(pq);
        generateCodes(root,code,0,codes);

    }
    for (int i=0;i<256;i++){
        if(strlen(codes[i])>0){
            printf("%c: %s\n",i,codes[i]);
        }
    }
    //4. encode input data

    return 0;
}


