//Kanellaki Maria Anna - 1115201400060


#ifndef PATIENTRECORD_H
#define PATIENTRECORD_H

#include "Date.h"

class PatientList;

class PatientRecord {                                   //contains one patient's data

    string recordID;
    string patientFirstName;
    string patientLastName;
    string diseaseID;
    string country;
    Date *entryDate;
    Date *exitDate;
    bool valid;

public:
    PatientRecord(char *);
    ~PatientRecord();

    const string &getRecordId() const;
    Date *getEntryDate() const;
    const string &getDiseaseId() const;
    const string &getCountry() const;
    void setExitDate(Date *exitDate);
    Date *getExitDate() const;

    bool isValid() const;
};


#endif