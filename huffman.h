#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdbool.h>
extern int debug;

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
void generateCodes(Node* root, char* code, int codeLen, char codes[256][256]);
#endif