//Kanellaki Maria Anna - 1115201400060


#ifndef HEAPLIST_HPP
#define HEAPLIST_HPP
class HeapNode;

class QueueNode {                            //node of a heapList. contains a heapNode

    HeapNode * heapNode;
    QueueNode * next;

public:
    QueueNode(HeapNode*);
    ~QueueNode();

    QueueNode *getNext();
    void setNext(QueueNode *next);
    HeapNode *getHeapNode();
};


class Queue {                                //helping list created to store every heap node that does not have 2 children yet
                                             //helps heap insertion to make the tree complete
    QueueNode * head;                        //works as a priority queue (pushes at the end, pops from the front)

public:
    Queue();
    void push(QueueNode*);
    void popFirst();
    ~Queue();

    QueueNode *getHead();
};


#endif