#include "report.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Subject maths;
Subject english;
Subject ecg;
Subject infoI;
Subject infoC;

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
}

Subject *getSubjectByName(const char *name){
    if (!name) return NULL;

    if (strcmp(name, "Maths") == 0) return &maths;
    if (strcmp(name, "Anglais") == 0) return &english;
    if (strcmp(name, "ECG") == 0) return &ecg;
    if (strcmp(name, "I") == 0) return &infoI;
    if (strcmp(name, "C") == 0) return &infoC;

    return NULL;
}
