//Kanellaki Maria Anna - 1115201400060


#ifndef SYSPRO1_TREE_H
#define SYSPRO1_TREE_H

#include <string>
#include "Date.h"
#include "PatientRecord.h"
#include "MaxHeap.h"

using namespace std;

class treeNode {                    //represents a node of a Tree

    Date * date;                    //key (entry date)
    PatientRecord * record;         //pointer to a record of a patient
    treeNode * left;
    treeNode * right;
    int height;

public:
    treeNode(PatientRecord*);
    ~treeNode();

    treeNode *getLeft() const;
    treeNode *getRight() const;
    Date *getDate() const;
    void setLeft(treeNode *left);
    void setRight(treeNode *right);
    void setHeight(int height);
    int getHeight() const;
    PatientRecord *getRecord() const;
};


class Tree {                        //represents an AVL Tree

    treeNode * root;

public:
    Tree();
    void insertInTree(treeNode*);
    treeNode * rotateLeft(treeNode*);
    treeNode * rotateRight(treeNode*);
    void BalanceTree(treeNode*);
    void destroyTree(treeNode*);
    int countDiseased(treeNode*);
    void countDiseasedDates(treeNode*, Date*, Date*, int*);
    void countDiseasedCountry(treeNode*, Date*, Date*, string, int*);
    void DiseaseCount(treeNode*, string, int*);
    void countAdmitted(treeNode*, int*);
    void CopyToHeap(MaxHeap*, treeNode*, string);
    void CopyToHeapDates(MaxHeap*, Date*, Date*, treeNode*, string);

    ~Tree();

    treeNode *getRoot() const;
};

#endif
