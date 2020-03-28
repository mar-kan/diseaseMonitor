//Kanellaki Maria Anna - 1115201400060


#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "PatientRecord.h"
#include "Tree.h"
#include "MaxHeap.h"

class BucketEntry {                             //represents a disease/country entry of a bucket

    string key;                                 //diseaseType or country
    BucketEntry * nextEntry;

public:
    BucketEntry(PatientRecord *, string);
    ~BucketEntry();

    BucketEntry *getNextEntry() const;
    void setNextEntry(BucketEntry *nextEntry);
    const string &getKey() const;
};


class Bucket {                                  //represents a bucket of a hash table

    int remainingSpace;

    Tree * tree;
    BucketEntry * first;
    Bucket * nextBucket;

public:
    Bucket(int);
    void InsertInBucket(PatientRecord*, int bucketSize, string, treeNode*);
    ~Bucket();

    BucketEntry *getFirst() const;
    Bucket *getNextBucket() const;
    Tree *getTree() const;
};


class HashTable {                               //represents a hash table with size[size] and bucket size [bucketSize] (in bytes).

    int size;
    int bucketSize;

    Bucket ** table;

public:
    HashTable(int, int);
    void InsertToTable(PatientRecord*, string);
    int HashFunction(string);
    void PrintCountOfDiseased();
    void PrintCountOfDiseasedDates(Date*, Date*);
    void PrintCountOfADiseaseDates(string, Date*, Date*);
    void PrintCountOfCountryOfADiseaseDates(string, string, Date*, Date*);
    void PrintCountOfAdmitted();
    void PrintCountOfAdmittedOfADisease(string);
    void CopyToHeap(string, MaxHeap*);
    void CopyToHeapDates(string, MaxHeap*, Date*, Date*);
    ~HashTable();

    int getSize() const;
    int getBucketSize() const;
};


#endif