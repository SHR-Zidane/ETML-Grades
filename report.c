#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "report.h"

float Avg(Subject *subject) {
    if (subject == NULL || subject->size <= 0) {
        return 0;
    }
    float sumG = 0;
    float sumC = 0;
    for (int i = 0; i < subject->size; i++) {
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
