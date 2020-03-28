//Kanellaki Maria Anna - 1115201400060


#include "Tree.h"


treeNode::treeNode(PatientRecord *rec)          //initializes a Tree node with data from given PatientRecord and null children
{
    record = rec;
    date = rec->getEntryDate();

    left = NULL;
    right = NULL;
    height = -1;                                //height is set by Tree class during insertion
}


treeNode::~treeNode()                           //deletes node and pointers without deleting the children
{
    //delete record;                            //patient records destroyed by list
    //delete date;                              //dates deleted by list
    left = NULL;
    right = NULL;
    delete left;
    delete right;
}


Tree::Tree()                                    //initializes empty Tree
{
    root = NULL;
}


Tree::~Tree()                                   //calls custom recursive function to delete Tree
{
    destroyTree(root);
}


void Tree::insertInTree(treeNode *node)         //inserts given treeNode
{
    if (!root)
    {
        root = node;
        node->setHeight(1);              //root height = 1
        return;
    }
    treeNode * temp = root;
    while(temp->getRight() && temp->getLeft())  //finds 1st null child and inserts new node there
    {
        //moves to suitable child depending on the date
        if (node->getDate()->earlierThan(temp->getDate()))
            temp = temp->getLeft();
        else
            temp = temp->getRight();
    }
    if (temp->getRight())
        temp->setLeft(node);
    else
        temp->setRight(node);

    node->setHeight(temp->getHeight() + 1);

    BalanceTree(node);
}


treeNode * Tree::rotateLeft(treeNode *node)     //rotates left from given node
{
    treeNode *curr = node->getRight();
    treeNode *temp = curr->getLeft();

    //rotate
    curr->setLeft(node);
    node->setRight(temp);

    //set new heights
    node->setHeight(max(node->getLeft()->getHeight(), node->getRight()->getHeight()) + 1);
    curr->setHeight(max(curr->getLeft()->getHeight(), curr->getRight()->getHeight()) + 1);

    //if root changed return new root else return null
    if (curr->getHeight() == 1)
        return curr;
    if (node->getHeight() == 1)
        return node;
    if (temp->getHeight() == 1)
        return temp;

    return NULL;
}


treeNode * Tree::rotateRight(treeNode *node)    //rotates right from given node
{
    treeNode *curr = node->getLeft();
    treeNode *temp = curr->getRight();

    //rotate
    curr->setRight(node);
    node->setLeft(temp);

    //set new heights
    node->setHeight(max(node->getLeft()->getHeight(), node->getRight()->getHeight()) + 1);
    curr->setHeight(max(curr->getLeft()->getHeight(), curr->getRight()->getHeight()) + 1);

    //if root changed return new root else return null
    if (curr->getHeight() == 1)
        return curr;
    if (node->getHeight() == 1)
        return node;
    if (temp->getHeight() == 1)
        return temp;

    return NULL;
}


void Tree::BalanceTree(treeNode *node)          //does necessary rotations to keep Tree balanced
{
    if (!node)
        return;

    if (!node->getLeft())
        return;

    if (!node->getRight())
        return;

    int balance = node->getLeft()->getHeight() - node->getRight()->getHeight();
    while (balance < -1 || balance > 1)
    {
        if (balance > 1 && node->getDate()->earlierThan(node->getLeft()->getDate()))
            rotateRight(node);

        if (balance < -1 && node->getDate()->earlierThan(node->getRight()->getDate()))
            rotateLeft(node);

        if (balance > 1 && node->getDate()->earlierThan(node->getLeft()->getDate()))
        {
            rotateLeft(node->getLeft());
            rotateRight(node);
        }

        if (balance < -1 && node->getDate()->earlierThan(node->getRight()->getDate()))
        {
            rotateRight(node->getRight());
            rotateLeft(node);
        }

        balance = node->getLeft()->getHeight() - node->getRight()->getHeight();
    }
}


void Tree::destroyTree(treeNode *node)          //destroys whole Tree recursively
{
    if (!node)
        return;

    destroyTree(node->getLeft());
    destroyTree(node->getRight());
    delete(node);
}


int Tree::countDiseased(treeNode * node)        //accesses recursively all elements of the tree, counts them and returns their count
{
    if (!root)
        return 0;

    int count = 1;

    if (node->getLeft())
        count += countDiseased(node->getLeft());

    if (node->getRight())
        count += countDiseased(node->getRight());

    return count;
}


void Tree::countDiseasedDates(treeNode* node, Date *from, Date *to, int *count) //accesses recursively all elements of the tree and counts those between the 2 dates. returns their count
{
    if (!node)
        return;

    if (from->earlierThan(node->getDate()) && node->getDate()->earlierThan(to))
        *(count) += 1;

    if (node->getLeft())
        countDiseasedDates(node->getLeft(), from, to, count);

    if (node->getRight())
        countDiseasedDates(node->getRight(), from, to, count);
}


void Tree::countDiseasedCountry(treeNode *node, Date *from, Date *to, string disease, int *count) //accesses recursively all elements of the tree and counts those between the 2 dates that had the given disease. returns their count
{                                                                                                 //created to work with country table
     if (!node)
        return;

    if (from->earlierThan(node->getDate()) && node->getDate()->earlierThan(to) && disease == node->getRecord()->getDiseaseId())
        *(count) += 1;

    if (node->getLeft())
        countDiseasedCountry(node->getLeft(), from, to, disease, count);

    if (node->getRight())
        countDiseasedCountry(node->getRight(), from, to, disease, count);
}


void Tree::DiseaseCount(treeNode* node, string myKey, int* count) //accesses recursively all elements of the tree and counts those with disease/country = key. returns their count
{                                                                 //created to calculate counts for max heap
    if (!node)
        return;

    if (myKey == node->getRecord()->getDiseaseId() || myKey == node->getRecord()->getCountry())
        *(count) += 1;

    if (node->getLeft())
        DiseaseCount(node->getLeft(), myKey, count);

    if (node->getRight())
        DiseaseCount(node->getRight(), myKey, count);
}


void Tree::countAdmitted(treeNode * node, int* count) //accesses recursively all elements of the tree, counts those that are still admitted and returns their count
{
    if (!node)
        return;

    if (!node->getRecord()->getExitDate())
        *(count) += 1;

    if (node->getLeft())
        countAdmitted(node->getLeft(), count);

    if (node->getRight())
        countAdmitted(node->getRight(), count);
}


void Tree::CopyToHeap(MaxHeap *myHeap, treeNode* curr, string type)                     //calculates counts and copies data to heap
{
    string heapKey;

    if (!curr)
        return;

    if (type == "disease")
        heapKey = curr->getRecord()->getDiseaseId();
    else
        heapKey = curr->getRecord()->getCountry();

    HeapNode *tmp = NULL;
    myHeap->Exists(myHeap->getRoot(), heapKey, &tmp);                                   //searches heap for key
    if (!tmp)
    {
        int count=0;
        DiseaseCount(root, heapKey, &count);
        HeapNode *temp = new HeapNode(heapKey, count);                                  //if it doesn't exist creates new node with key and count=1 (since its the 1st element)
        myHeap->insertNode(temp);                                                       //inserts node in heap
    }
    //else does nothing because node already exists with that key

    if (curr->getLeft())
        CopyToHeap(myHeap, curr->getLeft(), type);

    if (curr->getRight())
        CopyToHeap(myHeap, curr->getRight(), type);
}


void Tree::CopyToHeapDates(MaxHeap *myHeap, Date *d1, Date *d2, treeNode* curr, string type) //the same as above but checks that dates are between d1,d2 before insertion
{
    string heapKey;

    if (!root)
        return;

    if (d1->earlierThan(curr->getDate()) && curr->getDate()->earlierThan(d2))               //checks dates before insertion
    {
        if (type == "disease")
            heapKey = curr->getRecord()->getDiseaseId();
        else
            heapKey = curr->getRecord()->getCountry();

        HeapNode *tmp = NULL;
        myHeap->Exists(myHeap->getRoot(), heapKey, &tmp);                                   //searches heap for key
        if (!tmp)
        {
            int count=0;
            DiseaseCount(root, heapKey, &count);
            HeapNode *temp = new HeapNode(heapKey, count);                                  //if it doesn't exist creates new node with key and count=1 (since its the 1st element)
            myHeap->insertNode(temp);                                                       //inserts node in heap
        }
        //else node already exists and does nothing
    }

    if (curr->getLeft())
        CopyToHeap(myHeap, curr->getLeft(), type);

    if (curr->getRight())
        CopyToHeap(myHeap, curr->getRight(), type);
}


//getters and setters for both classes
treeNode *treeNode::getLeft() const
{
    return left;
}


treeNode *treeNode::getRight() const
{
    return right;
}


Date *treeNode::getDate() const
{
    return date;
}


void treeNode::setLeft(treeNode *left)
{
    treeNode::left = left;
}


void treeNode::setRight(treeNode *right)
{
    treeNode::right = right;
}


void treeNode::setHeight(int height)
{
    treeNode::height = height;
}


int treeNode::getHeight() const
{
    return height;
}


PatientRecord *treeNode::getRecord() const
{
    return record;
}


treeNode *Tree::getRoot() const
{
    return root;
}