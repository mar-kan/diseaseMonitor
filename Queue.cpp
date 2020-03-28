//Kanellaki Maria Anna - 1115201400060


#include "Queue.h"
#include "MaxHeap.h"


QueueNode::QueueNode(HeapNode * hNode)
{
    heapNode = hNode;
    next = NULL;
}


QueueNode::~QueueNode()
{
    next = NULL;
    delete next;
}


Queue::Queue()
{
    head = NULL;
}


Queue::~Queue()
{
    QueueNode *curr, *temp = head;

    while (temp)
    {
        curr = temp;
        temp = temp->getNext();
        delete curr;
    }
}


void Queue::push(QueueNode* node)
{
    if (!head)
    {
        head = node;
        return;
    }

    QueueNode * temp = head;
    while (temp->getNext())
        temp = temp->getNext();

    temp->setNext(node);
}


void Queue::popFirst()
{
    QueueNode * node = head;
    head = head->getNext();
    delete node;
}


//getters and setters
QueueNode *QueueNode::getNext()
{
    return next;
}


void QueueNode::setNext(QueueNode *next)
{
    QueueNode::next = next;
}


HeapNode *QueueNode::getHeapNode()
{
    return heapNode;
}


QueueNode *Queue::getHead()
{
    return head;
}
