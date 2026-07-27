#ifndef REPORT_H
#define REPORT_H

#include <stdbool.h>

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

float Avg(Subject *subject);
float AvgModule(Subject *subject, const char *module);
float AvgInfo(Subject *subject, int nbSubjects);
float AvgGlobal(float avgInfo, float avgCG, float avgCBE);
float round01(float value);
float round05(float value);
void addGrade(Subject *subject, Grade grade);
Subject *getSubjectByName(const char *name);
float AvgOfModules(Subject *subject);
#endif
