#ifndef REPORT_H
#define REPORT_H

#include <stdbool.h>
#include <stdio.h>

typedef struct {
    char *module;
    float value;
    bool has_coef;
    float coef;
} Grade;

typedef struct {
    char *name;
    Grade *grades;
    int size;
    char type;
} Subject;

extern Subject maths;
extern Subject english;
extern Subject ecg;
extern Subject infoI;
extern Subject infoC;

extern Subject maths2;
extern Subject english2;
extern Subject ecg2;
extern Subject infoI2;
extern Subject infoC2;

extern int currentSem;
float Avg(Subject *subject);
float AvgModule(Subject *subject, const char *module);
float AvgInfo(Subject *subject, int nbSubjects);
float AvgGlobal(float avgInfo, float avgCG, float avgCBE);
float round01(float value);
float round05(float value);
void addGrade(Subject *subject, Grade grade);
Subject *getSubjectByName(const char *name, int sem);
float AvgOfModules(Subject *subject);
float AvgCBE(Subject *maths, Subject *english);
float AvgInformatique(Subject *infoI, Subject *infoC);
void saveData(void);
void loadData(void);
void printCalculations(FILE *f);
void dumpCalculations(const char *logFile);
#endif
