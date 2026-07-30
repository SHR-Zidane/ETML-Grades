#include "report.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Subject maths;
Subject english;
Subject ecg;
Subject infoI;
Subject infoC;

Subject maths2;
Subject english2;
Subject ecg2;
Subject infoI2;
Subject infoC2;

void initSubjects(){
    maths.name = "Maths";
    maths.grades = NULL;
    maths.size = 0;
    maths.type = 0;

    english.name = "Anglais";
    english.grades = NULL;
    english.size = 0;
    english.type = 0;

    ecg.name = "ECG";
    ecg.grades = NULL;
    ecg.size = 0;
    ecg.type = 0;

    infoI.name = "InfoI";
    infoI.grades = NULL;
    infoI.size = 0;
    infoI.type = 'I';

    infoC.name = "InfoC";
    infoC.grades = NULL;
    infoC.size = 0;
    infoC.type = 'C';

    maths2.name = "Maths";
    maths2.grades = NULL;
    maths2.size = 0;
    maths2.type = 0;

    english2.name = "Anglais";
    english2.grades = NULL;
    english2.size = 0;
    english2.type = 0;

    ecg2.name = "ECG";
    ecg2.grades = NULL;
    ecg2.size = 0;
    ecg2.type = 0;

    infoI2.name = "InfoI";
    infoI2.grades = NULL;
    infoI2.size = 0;
    infoI2.type = 'I';

    infoC2.name = "InfoC";
    infoC2.grades = NULL;
    infoC2.size = 0;
    infoC2.type = 'C';
}

Subject *getSubjectByName(const char *name, int sem){
    if (!name) return NULL;

    if (sem == 1) {
        if (strcmp(name, "Maths") == 0) return &maths;
        if (strcmp(name, "Anglais") == 0) return &english;
        if (strcmp(name, "ECG") == 0) return &ecg;
        if (strcmp(name, "I") == 0) return &infoI;
        if (strcmp(name, "C") == 0) return &infoC;
    } else {
        if (strcmp(name, "Maths") == 0) return &maths2;
        if (strcmp(name, "Anglais") == 0) return &english2;
        if (strcmp(name, "ECG") == 0) return &ecg2;
        if (strcmp(name, "I") == 0) return &infoI2;
        if (strcmp(name, "C") == 0) return &infoC2;
    }

    return NULL;
}
