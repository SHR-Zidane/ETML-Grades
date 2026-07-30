#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "report.h"
#include <string.h>


float Avg(Subject *subject) {
    if (subject == NULL || subject->size <= 0) {
        return 0;
    }
    float sumG = 0;
    float sumC = 0;
    for (int i = 0; i < subject->size; i++) {

        sumG += subject->grades[i].value;
        sumC += 1;
    }
    if (sumC == 0) return 0;
    return round05(sumG / sumC);
}

float AvgModule(Subject *subject, const char *module) {
    if (subject == NULL || subject->size <= 0) {
        return 0;
    }
    float sumG = 0;
    float sumC = 0;
    for (int i = 0; i < subject->size; i++) {
        if (subject->grades[i].module == NULL) {
            continue;
        }
        if (module != NULL && strcmp(subject->grades[i].module, module) != 0) {
            continue;
        }

        if (subject->grades[i].has_coef) {
            sumG += subject->grades[i].value * subject->grades[i].coef;
            sumC += subject->grades[i].coef;
        }
        else {
            sumG += subject->grades[i].value;
            sumC += 1;
        }
    }
    if (sumC == 0) return 0;
    return round05(sumG / sumC);
}
float AvgInfo(Subject *subject, int nbSubjects) {
    float sumI = 0;
    int Icount = 0;
    float AvgI = 0;
    float sumC = 0;
    int Ccount = 0;
    float AvgC = 0;
    const float ICOEF = 0.8;
    const float CCOEF = 0.2;

    for(int i = 0; i < nbSubjects; i++){
        if ((subject + i)->type == 'I') {
        sumI += Avg(&subject[i]);
        Icount++;
        }
        else {
        sumC += Avg(&subject[i]);
        Ccount++;
        }
    }
    if (Icount > 0){
        AvgI = sumI / Icount;
    }
    if (Ccount > 0){
    AvgC = sumC / Ccount;
    }

    return (AvgI * ICOEF) + (AvgC * CCOEF);
}

float AvgGlobal(float avgInfo, float avgCG, float avgCBE){
    const int INFCOEF = 3;
    const int ECGCOEF = 2;
    const int CBECOEF = 1;
    const float TOTALCOEF = INFCOEF + ECGCOEF + CBECOEF;
    float totalPoints = (avgInfo * INFCOEF) + (avgCG * ECGCOEF) + (avgCBE * CBECOEF);
    if (TOTALCOEF == 0){
       return 0;
    }
    return totalPoints / TOTALCOEF;
}

float round05(float value){
    if (value == 0){
        return 0;
    }
    return roundf(value * 2) / 2.0f;
}
float round01(float value){
    if (value == 0){
        return 0;
    }
    value = roundf(value * 10.0f) / 10.0f;
    return value;
}

float AvgOfModules(Subject *subject) {
    if (subject == NULL || subject->size <= 0) return 0;

    int nModules = 0;
    char **modules = NULL;

    for (int i = 0; i < subject->size; i++) {
        if (subject->grades[i].module == NULL) continue;
        int found = 0;
        for (int m = 0; m < nModules; m++) {
            if (strcmp(modules[m], subject->grades[i].module) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            modules = realloc(modules, (nModules + 1) * sizeof(char *));
            modules[nModules] = subject->grades[i].module;
            nModules++;
        }
    }

    if (nModules == 0) {
        free(modules);
        return 0;
    }

    float sum = 0;
    for (int m = 0; m < nModules; m++) {
        sum += AvgModule(subject, modules[m]);
    }

    free(modules);
    return round05(sum / nModules);
}

float AvgCBE(Subject *maths, Subject *english) {
    return round01((Avg(maths) + Avg(english)) / 2.0f);
}

float AvgInformatique(Subject *infoI, Subject *infoC) {
    const float I_COEF = 0.8f;
    const float C_COEF = 0.2f;
    return round01((AvgOfModules(infoI) * I_COEF) + (AvgOfModules(infoC) * C_COEF));
}

float AvgGeneral(float avgCBE, float avgInformatique, float avgECG) {
    return round01((avgCBE + avgInformatique + avgECG) / 3.0f);
}

// Link GUI

void addGrade(Subject *subject, Grade grade) {
    int oldSize = subject->size;
    int newSize = oldSize + 1;
    Grade *temp;
    temp = realloc(subject->grades, newSize * sizeof(Grade));
    if (temp == NULL) {
        fprintf(stderr,"Error, memory allocation failed");
        return;
    }
    subject->grades = temp;
    subject->grades[oldSize] = grade;
    subject->size++;
}
