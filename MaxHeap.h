//Kanellaki Maria Anna - 1115201400060


#ifndef MAXHEAP_H
#define MAXHEAP_H

#include <string>
#include "Queue.h"

using namespace std;

class HeapNode {                //represents a node of the max heap. contains a key and the count of diseased people for that key

    string key;                 //country or disease
    int count;

    HeapNode * parent;
    HeapNode * right;
    HeapNode * left;

public:
    HeapNode(string, int);
    ~HeapNode();
    void SwapNodeData(HeapNode*);

    int getCount() const;
    const string &getKey() const;
    void setCount(int count);
    HeapNode *getRight() const;
    HeapNode *getLeft() const;
    void setRight(HeapNode *right);
    void setLeft(HeapNode *left);
    HeapNode *getParent() const;
    void setParent(HeapNode *parent);
};



class MaxHeap {                 //represents a dynamic approach of a max binary heap

    HeapNode * root;
    Queue * incompleteNodes; //stores all heap nodes that do not have 2 children. helps keep the heap complete

public:
    MaxHeap();
    void insertNode(HeapNode*);
    void heapify(HeapNode*);
    void PrintTopK(int, string);
    void Exists(HeapNode*, string, HeapNode**);
    void destroyHeap(HeapNode*);
    ~MaxHeap();

    HeapNode *getRoot() const;
};


#endif