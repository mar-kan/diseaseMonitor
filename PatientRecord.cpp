//Kanellaki Maria Anna - 1115201400060


#include <iostream>
#include <cstring>
#include "PatientRecord.h"


PatientRecord::PatientRecord(char * line)       //gets a string (whole line), breaks it and stores its data
{
    char delimit[]=" \t\n";
    char * date1, * date2, * temp;

    valid = true;

    if ((temp = strtok(line, delimit)))
        recordID = temp;
    else
        valid = false;

    if ((temp = strtok(NULL, delimit)))
        patientFirstName = temp;
    else
        valid = false;

    if ((temp = strtok(NULL, delimit)))
        patientLastName = temp;
    else
        valid = false;

    if ((temp = strtok(NULL, delimit)))
        diseaseID = temp;
    else
        valid = false;

    if ((temp = strtok(NULL, delimit)))
        country = temp;
    else
        valid = false;

    if (!(date1 = strtok(NULL, delimit)))
        valid = false;

    date2 = strtok(NULL, delimit);

    if (!valid || strtok(NULL, delimit) || strcmp(date1, "-")==0)
    {
        cout << "Invalid patient record"<<endl;
        entryDate = NULL;
        exitDate = NULL;
        valid = false;
        return;
    }

    entryDate = new Date(date1);
    if (!date2 || strcmp(date2, "-") == 0)
        exitDate = NULL;
    else
        exitDate = new Date(date2);

    if (exitDate)
    {
        if (!entryDate->isValid() || !exitDate->isValid())
            valid = false;
        if (exitDate->earlierThan(entryDate))
            valid = false;
    }
    else
    {
        if (!entryDate->isValid())
            valid = false;
    }
}


PatientRecord::~PatientRecord()
{
    delete entryDate;
    delete exitDate;
}


//getters
const string &PatientRecord::getRecordId() const
{
    return recordID;
}


Date *PatientRecord::getEntryDate() const
{
    return entryDate;
}


const string &PatientRecord::getDiseaseId() const
{
    return diseaseID;
}


const string &PatientRecord::getCountry() const
{
    return country;
}


void PatientRecord::setExitDate(Date *exitDate)
{
    PatientRecord::exitDate = exitDate;
}


Date *PatientRecord::getExitDate() const
{
    return exitDate;
}


bool PatientRecord::isValid() const
{
    return valid;
}
