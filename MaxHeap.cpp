//Kanellaki Maria Anna - 1115201400060


#include <iostream>
#include "MaxHeap.h"


HeapNode::HeapNode(string myKey, int c)                     //initializes new node with given values for key and count
{
    key = myKey;                                            //may be a disease or a country
    count = c;

    parent = NULL;                                          //is set by MaxHeap::insertNode
    right = NULL;
    left = NULL;
}


HeapNode::~HeapNode()                                       //deletes node and its pointers without deleting the children
{
    left = NULL;
    right = NULL;
    delete left;
    delete right;
}


void HeapNode::SwapNodeData(HeapNode * node)                //swaps the data of 2 nodes (key, count)
{
    swap(key, node->key);
    swap (count, node->count);
}


MaxHeap::MaxHeap()                                          //initializes empty max heap
{
    root = NULL;
    incompleteNodes = new Queue();
}


MaxHeap::~MaxHeap()                                         //calls custom recursive function to delete Tree
{
    destroyHeap(root);
    delete incompleteNodes;
}


void MaxHeap::destroyHeap(HeapNode * node)                  //destroys whole heap recursively
{
    if (!node)
        return;

    destroyHeap(node->getLeft());
    destroyHeap(node->getRight());
    delete(node);
}


void MaxHeap::insertNode(HeapNode * node)                   //inserts a new node in heap. corrects its position by max count
{                                                           //sorts max heap from top to bottom and from left to right
    QueueNode * newIncomplete = new QueueNode(node);

    HeapNode * temp = NULL;
    Exists(root, node->getKey(), &temp);                     //key already exists
    if (temp)
    {
        delete newIncomplete;
        return;
    }

    if (!root)
    {
        root = node;
        incompleteNodes->push(newIncomplete);
        return;
    }

    if (incompleteNodes)
    {
        temp = incompleteNodes->getHead()->getHeapNode();
        node->setParent(temp);

        if (!temp->getLeft())
            temp->setLeft(node);
        else
        {
            temp->setRight(node);
            incompleteNodes->popFirst();
        }
    }
    else
    {
        cout << "Error. Something went wrong" << endl;
        return;
    }

    incompleteNodes->push(newIncomplete);

    //correct new node's position
    temp = node;

    while (temp->getCount() > temp->getParent()->getCount())
    {
        //swaps node data instead of whole node
        temp->SwapNodeData(temp->getParent());

        if (temp->getParent()->getRight() && temp->getParent()->getLeft())
        {
            if (temp->getParent()->getRight()->getCount() > temp->getParent()->getLeft()->getCount())
                temp->getParent()->getRight()->SwapNodeData(temp->getParent()->getLeft());
        }
        temp = temp->getParent();

        if (temp == root)
            break;
    }

    if (temp != root)
    {
        if (temp->getParent()->getRight() && temp->getParent()->getLeft())
        {
            if (temp->getParent()->getRight()->getCount() > temp->getParent()->getLeft()->getCount())
                temp->getParent()->getRight()->SwapNodeData(temp->getParent()->getLeft());
        }
        if ((node = temp->getParent()->getParent()))
        {
            if (node->getRight()->getCount() < temp->getCount())
                temp->SwapNodeData(node->getRight());

            if (node->getLeft()->getLeft())
            {
                if (node->getLeft()->getLeft()->getCount() < temp->getCount())
                {
                    temp->SwapNodeData(node->getLeft()->getLeft());

                    if (node->getLeft()->getRight())
                        node->getLeft()->getRight()->SwapNodeData(temp);
                }
            }
            if (node->getLeft()->getRight())
            {
                if (node->getLeft()->getRight()->getCount() < temp->getCount())
                    temp->SwapNodeData(node->getLeft()->getRight());

            }
        }
    }
    heapify(root);
}


void MaxHeap::heapify(HeapNode *node)                       //recursively max heapifies heap
{
    HeapNode* maxNode = node;

    if (node->getLeft())
    {
        if (node->getLeft()->getCount() > maxNode->getCount())
            maxNode = node->getLeft();
    }

    if (node->getRight())
    {
        if (node->getRight()->getCount() > maxNode->getCount())
            maxNode = node->getRight();
    }

    if (maxNode != node)
    {
        node->SwapNodeData(maxNode);
        heapify(maxNode);
    }
}


void MaxHeap::PrintTopK(int k, string type)                 //prints top <k> countries/diseases of given disease/country
{
    if (!root)
    {
        cout << "No data for "<< type <<"." << endl;
        return;
    }

    int counter=0;
    Queue * queue = new Queue();
    QueueNode * node = new QueueNode(root);
    queue->push(node);
    HeapNode * temp;

    while(queue)
    {
        if (!queue->getHead())
            break;

        temp = queue->getHead()->getHeapNode();
        queue->popFirst();

        cout << ++counter << ". " << temp->getKey() << ": " <<temp->getCount() << endl;

        if(temp->getLeft())
            queue->push(new QueueNode(temp->getLeft()));
        if(temp->getLeft())
            queue->push(new QueueNode(temp->getRight()));
    }
    delete queue;
}


void MaxHeap::Exists(HeapNode* node, string myKey, HeapNode **found) //searches for node with key = myKey. if found returns true else false
{
    if (!node)
        return;

    if (node->getKey() == myKey)
        *found = node;

    Exists(node->getLeft(), myKey, found);
    Exists(node->getRight(), myKey, found);
}


//getters and setters for both classes
int HeapNode::getCount() const
{
    return count;
}


const string &HeapNode::getKey() const
{
    return key;
}


void HeapNode::setCount(int c)
{
    HeapNode::count = c;
}


HeapNode *HeapNode::getRight() const
{
    return right;
}


HeapNode *HeapNode::getLeft() const
{
    return left;
}


void HeapNode::setRight(HeapNode *r)
{
    HeapNode::right = r;
}


void HeapNode::setLeft(HeapNode *l)
{
    HeapNode::left = l;
}


HeapNode *HeapNode::getParent() const
{
    return parent;
}


void HeapNode::setParent(HeapNode *p)
{
    HeapNode::parent = p;
}


HeapNode *MaxHeap::getRoot() const
{
    return root;
}
