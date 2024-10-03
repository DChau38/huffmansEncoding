#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

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