Compilation instructions: $ make compile

Execution instructions: $ make execute

To clean up generated files: $ make clean


Q/A application that monitors diseases in C/C++ with hand-written data structures. The program receives patient data via an input file (input files are included),
fills all the data structures that are described below with the patient records and waits to receive and execute any of the following commands:

1. /globalDiseaseStats (optional: dates)

2. /diseaseFrequency for a disease with dates (optional: country)

3. /topk-Diseases k for a country (optional: dates)

4. /topk-Countries k for a disease (optional: dates)

5. /insertPatientRecord with recordID, firstName, lastName, diseaseID, country, entryDate, exitDate(optional)

6. /recordPatientExit with recordID and exitDate

7. /numCurrentPatients (optional: disease)

8. /exit


The data structures that are used are a simple Linked List, a HashTable with Buckets with fixed size in bytes that resemble Linked Lists in their usage (Dynamic
but can fit a limited amount of bytes), an AVL tree, a MaxHeap and a Priority Queue.

1 list is used which stores all tha patient records. 

2 hash tables are used which stores all tha patient records. The first uses the disease as the key and the second the country. (Used to accelerate the search
or counting results by country and disease.

The trees are used inside the hash tables. One is created for each bucket of each hash table (diseases and countries based). Their key are the dates (an entry
date which points to when the patient was admitted) and they Used to accelerate the search / counting results by date in country and disease questions.

The Max Heaps are the only data structures that are not created when the program starts. They are only used if a user inputs command 3 or 4 (from above).
Depending on the input command, the key is either a disease or a country. They store the count of diseases per country (or the opposite) in descending order,
prints the first K and then destruct on the spot.

The queue was used in the max heaps to help keep the descending order.


Each patient record is stored and used in tha data structures via pointers to avoid data duplication. 
