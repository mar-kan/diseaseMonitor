//Kanellaki Maria Anna - 1115201400060


#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include "PatientList.h"
#include "HashTable.h"
#include "MaxHeap.h"
using namespace std;


int main(int argc, char * argv[])
{
    if (argc != 9)                                                                    //program runs only with input in the format requested
    {
        cout << "Error. Please input from the command line input file, num of disease and country entries and bucket size."<<endl;
        exit(-1);
    }

    FILE * patientFile = fopen(argv[2], "r");
    if (!patientFile)
    {
        cout << "Error. Cannot find input file."<<endl;
        exit(-1);
    }

    //initializes values for hash tables
    int diseaseEntries = atoi(argv[4]);
    int countryEntries = atoi(argv[6]);
    int bucketSize = atoi(argv[8]);

    //initializes data structures
    PatientList * patients = new PatientList();                                       //creates empty list of patients
    HashTable * diseaseTable = new HashTable(diseaseEntries, bucketSize);             //create empty disease hash table
    HashTable * countryTable = new HashTable(countryEntries, bucketSize);             //create empty country hash table

    //reads file line by line
    size_t lsize = 0;
    char * line = NULL;
    while(getline(&line, &lsize, patientFile) != -1)
    {
        PatientRecord * rec = new PatientRecord(line);                                //creates PatientRecord from line of file read
        if (!rec->isValid())                                                          //if record is not valid deletes it and continues to next line
        {
            delete rec;
            continue;
        }
        PatientNode * nodeL = new PatientNode(rec);                                   //creates PatientNode with rec
        patients->push(nodeL);                                                        //inserts node in list

        diseaseTable->InsertToTable(rec, rec->getDiseaseId());                        //inserts record in disease table
        countryTable->InsertToTable(rec, rec->getCountry());                          //inserts record in country table
    }
    fclose(patientFile);

    cout <<endl<< "Your options are the following:"<<endl;                            //menu of options

    cout << "1. /globalDiseaseStats (optional: dates)"<<endl;
    cout << "2. /diseaseFrequency for a disease with dates (optional: country)"<<endl;
    cout << "3. /topk-Diseases k for a country (optional: dates)"<<endl;
    cout << "4. /topk-Countries k for a disease (optional: dates)"<<endl;
    cout << "5. /insertPatientRecord with recordID, firstName, lastName, diseaseID, country, entryDate, exitDate(optional)"<<endl;
    cout << "6. /recordPatientExit with recordID and exitDate"<<endl;
    cout << "7. /numCurrentPatients (optional: disease)"<<endl;
    cout << "8. /exit"<<endl;

    string input;
    char *date1, *date2, *country, *disease;
    char delimit[]=" \t\n";

    do{
        cout << "\nEnter your input:\n";
        getline(cin, input);
        strcpy(line, input.c_str());
        cout << endl;

        if (input.find("/globalDiseaseStats") != string::npos)                     //prints count of diseased people for every disease
        {
            strtok(line, delimit);                                                    //splits command name
            if (!(date1 = strtok(NULL, delimit)))                                  //no date arguments
                diseaseTable->PrintCountOfDiseased();
            else                                                                      //counts cases between given dates
            {
                if (!(date2 = strtok(NULL, delimit)))                              //if there is only one date then the argument is not valid
                {
                    cout << "Please input 2 dates or none. Request aborted."<< endl;
                    continue;
                }
                Date * d1 = new Date(date1);
                Date * d2 = new Date(date2);
                if (!d1->isValid() || !d2->isValid())
                {
                    delete d1;
                    delete d2;
                    continue;
                }

                diseaseTable->PrintCountOfDiseasedDates(d1, d2);                      //prints count of diseased people for every disease between the 2 dates
            }
        }
        else if (input.find("/diseaseFrequency") != string::npos)                  //prints count of diseased people for given disease and given dates
        {
            strtok(line, delimit);                                                    //splits command name
            if (!(disease = strtok(NULL, delimit))) {
                cout << "Needs name of virus to execute" << endl;
                continue;
            }
            if (!(date1 = strtok(NULL, delimit))) {
                cout << "Needs dates to execute" << endl;
                continue;
            }
            if (!(date2 = strtok(NULL, delimit))) {
                cout << "Needs two dates to execute" << endl;
                continue;
            }
            country = strtok(NULL, delimit);

            Date *d1 = new Date(date1);
            Date *d2 = new Date(date2);

            if (!d1->isValid() || !d2->isValid())
            {
                delete d1;
                delete d2;
                continue;
            }

            if (!country)
                diseaseTable->PrintCountOfADiseaseDates(disease, d1, d2);
            else                                                                      //given disease for given country
                countryTable->PrintCountOfCountryOfADiseaseDates(country, disease, d1, d2);
        }
        else if (input.find("/topk-Diseases") != string::npos)
        {
            strtok(line, delimit);                                                    //splits command name
            int k;

            MaxHeap * diseaseHeap = new MaxHeap();

            if (!(country = strtok(NULL, delimit)))
            {
                cout << "Needs number to execute" << endl;
                continue;
            }
            else
                k = atoi(country);

            if (!(country = strtok(NULL, delimit)))
            {
                cout << "Needs country to execute" << endl;
                continue;
            }

            if (!(date1 = strtok(NULL, delimit)))
            {
                cout << "Top "<< k <<" for " << country << " are:" << endl;
                countryTable->CopyToHeap(country, diseaseHeap);
            }
            else
            {
                if (!(date2 = strtok(NULL, delimit)))                              //if there is only one date then the argument is not valid
                {
                    cout << "Please input 2 dates or none. Request aborted."<< endl;
                    continue;
                }
                Date * d1 = new Date(date1);
                Date * d2 = new Date(date2);

                if (!d1->isValid() || !d2->isValid())
                {
                    delete d1;
                    delete d2;
                    delete diseaseHeap;
                    continue;
                }

                countryTable->CopyToHeapDates(country, diseaseHeap, d1, d2);
                cout << "Top "<< k <<" for " << country << " between " << d1->getAsString() << " and " << d2->getAsString() <<" are:" << endl;
            }
            diseaseHeap->PrintTopK(k, country);

            delete diseaseHeap;
        }
        else if (input.find("/topk-Countries") != string::npos)
        {
            strtok(line, delimit);                                                    //splits command name
            int k;

            MaxHeap * countryHeap = new MaxHeap();

            if (!(disease = strtok(NULL, delimit)))
            {
                cout << "Needs number to execute" << endl;
                continue;
            }
            else
                k = atoi(disease);

            if (!(disease = strtok(NULL, delimit)))
            {
                cout << "Needs disease to execute" << endl;
                continue;
            }

            if (!(date1 = strtok(NULL, delimit)))
            {
                cout << "Top "<< k <<" for " << disease << " are:" << endl;
                diseaseTable->CopyToHeap(disease, countryHeap);
            }
            else
            {
                if (!(date2 = strtok(NULL, delimit)))                              //if there is only one date then the argument is not valid
                {
                    cout << "Please input 2 dates or none. Request aborted."<< endl;
                    continue;
                }
                Date * d1 = new Date(date1);
                Date * d2 = new Date(date2);

                if (!d1->isValid() || !d2->isValid())
                {
                    delete d1;
                    delete d2;
                    delete countryHeap;
                    continue;
                }
                diseaseTable->CopyToHeapDates(disease, countryHeap, d1, d2);
                cout << "Top "<< k <<" for " << disease << " between " << date1 << " and " << date2 <<" are:" << endl;
            }
            countryHeap->PrintTopK(k, disease);

            delete  countryHeap;
        }
        else if (input.find("/insertPatientRecord") != string::npos)               //creates new patient record and inserts it in every data structure
        {
            strtok(line, delimit);                                                   //splits command name
            PatientRecord * record = new PatientRecord(strtok(NULL, "\n"));  //creates new record from remaining line
            if (!record->isValid())
            {
                delete record;
                continue;
            }

            PatientNode * nodeL = new PatientNode(record);                           //creates PatientNode with rec
            patients->push(nodeL);                                                   //inserts node in list

            diseaseTable->InsertToTable(record, record->getDiseaseId());             //inserts record in disease table
            countryTable->InsertToTable(record, record->getCountry());               //inserts record in country table
            cout << "Record "<< record->getRecordId() <<" inserted successfully." << endl;
        }
        else if (input.find("/recordPatientExit") != string::npos)                //adds exit date to patient with given record id
        {
            strtok(line, delimit);                                                   //splits command name
            string id = strtok(NULL, delimit);
            date1 = strtok(NULL, delimit);

            PatientRecord * record = patients->SearchRecord(id);                     //since no structure uses the id as their key and offers quicker access, the search is implemented in the list
            if (record)
            {
                if (record->getExitDate())
                    cout << "Patient is already checked out."<< endl;
                else
                {
                    Date * exit = new Date(date1);
                    if (!exit->isValid())
                        continue;

                    if (exit->earlierThan(record->getEntryDate()))
                        cout << "Exit date invalid." << endl;
                    else
                    {
                        record->setExitDate(exit);
                        cout << "Exit date updated successfully."<< endl;
                    }
                }
            }
            else
                cout << "Patient not found."<< endl;
        }
        else if (input.find("/numCurrentPatients") != string::npos)               //prints count of people that are still admitted
        {
            strtok(line, delimit);                                                   //splits command name
            if (!(disease = strtok(NULL, delimit)))
                diseaseTable->PrintCountOfAdmitted();
            else
                diseaseTable->PrintCountOfAdmittedOfADisease(disease);
        }
        else if (input == "/exit")
            break;
        else
            cout << "Wrong input" << endl;
    }
    while (strcmp(line, "/exit") != 0);

    //deletes every structure (trees created and deleted by hash tables)
    delete diseaseTable;
    delete countryTable;
    delete patients;

    free(line);
    return 0;
}