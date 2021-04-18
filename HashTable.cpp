//Kanellaki Maria Anna - 1115201400060


#include <iostream>
#include "HashTable.h"


BucketEntry::BucketEntry(PatientRecord * rec, string myKey)     //creates new bucket entry for disease/country from patient record
{
    key = myKey;
    nextEntry = NULL;
}


BucketEntry::~BucketEntry()                                     //deletes one entry
{
    nextEntry = NULL;
    delete nextEntry;
}


Bucket::Bucket(int size)                                        //initializes empty bucket
{
    remainingSpace = size - 4 - 3*8;                            //subtracting the int (size) and the 3 pointers (tree, first and nextBucket)
    tree = new Tree();                                          //initializes empty tree for this disease/country
    first = NULL;
    nextBucket = NULL;
}


Bucket::~Bucket()                                               //deletes a bucket, the tree and its entries without touching the next Bucket
{
    delete tree;

    BucketEntry * temp, * entry = first;
    while (entry)
    {
        temp = entry;
        entry = entry->getNextEntry();
        delete temp;
    }
}


void Bucket::InsertInBucket(PatientRecord *rec, int bucketSize, string myKey, treeNode *nodeT) //creates new BucketEntry and inserts it in Bucket. creates new Bucket if needed.
{
    BucketEntry * entry = new BucketEntry(rec, myKey);

    this->tree->insertInTree(nodeT);                            //inserts node in Tree

    if (!first)
    {
        first = entry;
        remainingSpace -= sizeof(&entry);
        return;
    }

    Bucket * temp = nextBucket;

    if (remainingSpace - sizeof(&entry) < 0)                    //if there is not enough space creates new bucket and inserts entry there
    {
        while (temp->nextBucket)
        {
            if (remainingSpace - sizeof(&entry) < 0)
                temp = temp->nextBucket;
            else                                                //if program finds a bucket where there is enough space, entry is inserted there
            {
                remainingSpace -= sizeof(&entry);
                entry->setNextEntry(first);                     //insertion in the beginning of bucket for speed
                first = entry;
                return;
            }
        }
        //no bucket with enough space: creates new bucket and inserts there
        Bucket * newBucket = new Bucket(bucketSize);
        newBucket->InsertInBucket(rec, bucketSize, myKey, nodeT);
        nextBucket = newBucket;
    }
    else                                                        //enough space here
    {
        remainingSpace -= sizeof(&entry);
        entry->setNextEntry(first);                             //insertion in the beginning of bucket for speed
        first = entry;
    }
}


HashTable::HashTable(int s, int bs)                             //initializes empty hash table with size and bucketsize given by the command line
{
    size = s;
    bucketSize = bs;                                            //in bytes
    table = new Bucket*[size];

    for(int i = 0; i < size; i++)
        table[i] = NULL;
}


HashTable::~HashTable()                                         //deletes whole hash table
{
    int i;
    for (i=0; i<size; i++)
    {
        if (table[i])
            delete table[i];
    }
    delete[] table;
}


void HashTable::InsertToTable(PatientRecord *rec, string myKey) //hashes rec key to find its index. inserts it in index's bucket. creates one if needed
{
    Bucket * bucket;

    int index = HashFunction(myKey);                            //country or disease

    if (!table[index])                                          //creates new bucket if necessary
    {
        bucket = new Bucket(bucketSize);
        table[index] = bucket;
    }
    else
        bucket = table[index];

    treeNode * nodeT = new treeNode(rec);                       //creates tree node from patient record
    bucket->InsertInBucket(rec, bucketSize, myKey, nodeT);      //calls function to insert rec in bucket
}


int HashTable::HashFunction(string id)                          //simple modular hash function
{
    int i, multiplier=1;
    long long int  code=0;

    //builds ascii code of id. stops before overflow if string too big
    for (i=0; i<id.length(); i++)
    {
        char x = id.at(i);
        if ((code*multiplier + int(x)) < 0)
            return code % size;

        code = code*multiplier + int(x);
        multiplier *= 100;
    }
    return code % size;                                         //simple modular hash function
}


void HashTable::PrintCountOfDiseased()                          //prints count of all diseases stored in the hash table through their trees
{
    int i, count = 0;
    string disease, disease2 = " ";

    for (i=0; i<=size; i++)                                     //accesses whole hash table
    {
        if (!table[i])
            continue;

        //prints first entry's data
        BucketEntry * entry = table[i]->getFirst();
        if (!entry)
        {
            delete entry;
            continue;
        }
        disease = entry->getKey();
        cout << "Number of cases recorded for "<< disease <<": "<< table[i]->getTree()->countDiseased(table[i]->getTree()->getRoot()) <<"."<< endl;

        //checks for other diseases hashed in the same bucket
        while (entry)
        {
            //if there are any prints them
            if (entry->getKey() != disease && entry->getKey() != disease2)
            {
                cout << "Number of cases recorded for "<< entry->getKey() <<": "<< table[i]->getTree()->countDiseased(table[i]->getTree()->getRoot()) <<"."<< endl;
                disease2 = entry->getKey();
            }
            entry = entry->getNextEntry();
        }
    }
}


void HashTable::PrintCountOfDiseasedDates(Date* date1, Date* date2) //prints count of all diseases between 2 dates stored in the hash table through their trees
{
    int i;
    string disease, disease2 = " ";

    for (i=0; i<=size; i++)                                     //accesses whole hash table
    {
        if (!table[i])
            continue;

        //prints first entry's data
        BucketEntry * entry = table[i]->getFirst();
        if (!entry)
            continue;

        disease = entry->getKey();
        int count = 0;
        table[i]->getTree()->countDiseasedDates(table[i]->getTree()->getRoot(), date1, date2, &count);

        cout << "Number of cases recorded for "<< disease <<": "<< count <<" between "<<date1->getAsString()<<" and "<<date2->getAsString()<<"."<< endl;

        //checks for other diseases hashed in the same bucket
        while (entry)
        {
            //if there are any prints them
            if (entry->getKey() != disease && entry->getKey() != disease2)
            {
                table[i]->getTree()->countDiseasedDates(table[i]->getTree()->getRoot(), date1, date2, &count);
                cout << "Number of cases recorded for "<< entry->getKey() <<": "<< count <<" between "<<date1->getAsString()<<" and "<<date2->getAsString()<<"."<< endl;
                disease2 = entry->getKey();
            }
            entry = entry->getNextEntry();
        }
    }
}


void HashTable::PrintCountOfADiseaseDates(string disease, Date* date1, Date* date2) //prints count of a disease stored in the hash table through their trees
{
    int value = HashFunction(disease);
    int count = 0;
    table[value]->getTree()->countDiseasedDates(table[value]->getTree()->getRoot(), date1, date2, &count);

    cout <<"Number of cases recorded for "<< disease <<": "<< count <<" between "<<date1->getAsString()<<" and "<<date2->getAsString()<<"."<< endl;
}


void HashTable::PrintCountOfCountryOfADiseaseDates(string country, string disease, Date * date1, Date * date2) //prints count of a disease for a country stored in the hash table through their trees
{
    int value = HashFunction(country);
    int count = 0;
    table[value]->getTree()->countDiseasedCountry(table[value]->getTree()->getRoot(), date1, date2, disease, &count);

    cout <<"Number of cases recorded in "<< country<<" for "<< disease <<": "<< count <<" between "<<date1->getAsString()<<" and "<<date2->getAsString()<<"."<< endl;
}


void HashTable::PrintCountOfAdmitted()                          //prints count of all people still admitted for any disease that are stored in the hash table through their trees
{
    int i;
    string disease;

    for (i=0; i<=size; i++)                                     //accesses whole hash table
    {
        if (!table[i])
            continue;

        //prints first entry's data
        BucketEntry * entry = table[i]->getFirst();

        if (!entry)
            continue;

        int count = 0;
        disease = entry->getKey();
        table[i]->getTree()->countAdmitted(table[i]->getTree()->getRoot(), &count);
        cout << "Number of cases recorded for "<< disease <<": "<< count <<"."<< endl;

        //checks for other diseases hashed in the same bucket
        while (entry)
        {
            //if there are any prints them
            if (entry->getKey() != disease)
            {
                table[i]->getTree()->countAdmitted(table[i]->getTree()->getRoot(), &count);
                cout << "Number of cases recorded for "<< entry->getKey() <<": "<< count <<"."<< endl;
            }
            entry = entry->getNextEntry();
        }
    }
}


void HashTable::PrintCountOfAdmittedOfADisease(string disease)  //prints count of all people still admitted for a given disease that are stored in the hash table through their trees
{
    int value = HashFunction(disease);
    int count=0;
    table[value]->getTree()->countAdmitted(table[value]->getTree()->getRoot(), &count);
    cout << "Number of cases recorded for "<< disease <<": "<< count <<"."<< endl;
}


void HashTable::CopyToHeap(string key, MaxHeap * myHeap)        //finds table index of given key and copies its table to the heap
{
    int index = HashFunction(key);
    if (!table[index] || !table[index]->getTree()->getRoot())
    {
        cout << "No data for " << key << "." << endl;
        return;
    }

    string type;
    if (table[index]->getTree()->getRoot()->getRecord()->getCountry() == key)
        type = "disease";
    else if (table[index]->getTree()->getRoot()->getRecord()->getDiseaseId() == key)
        type = "country";
    else
    {
        cout << key << "doesn't exist." << endl;
        return;
    }
    table[index]->getTree()->CopyToHeap(myHeap, table[index]->getTree()->getRoot(), type);
}


void HashTable::CopyToHeapDates(string key, MaxHeap * myHeap, Date * date1, Date * date2) //finds table index of given key and copies its table nodes that are between the given dates to the heap
{
    int index = HashFunction(key);
    if (!table[index])
    {
        cout << "No data for this key" << endl;
        return;
    }

    string type = "disease";
    if (table[index]->getTree()->getRoot()->getRecord()->getDiseaseId() == key)
        type = "country";

    table[index]->getTree()->CopyToHeapDates(myHeap, date1, date2, table[index]->getTree()->getRoot(), type);
}


//setters and getters for all classes related to HashTable
BucketEntry *BucketEntry::getNextEntry() const
{
    return nextEntry;
}


void BucketEntry::setNextEntry(BucketEntry *nextEntry)
{
    BucketEntry::nextEntry = nextEntry;
}


int HashTable::getSize() const
{
    return size;
}


int HashTable::getBucketSize() const
{
    return bucketSize;
}


BucketEntry *Bucket::getFirst() const
{
    return first;
}


Bucket *Bucket::getNextBucket() const
{
    return nextBucket;
}


const string &BucketEntry::getKey() const
{
    return key;
}


Tree *Bucket::getTree() const
{
    return tree;
}