#ifndef REPORT_H
#define REPORT_H

#include <stdbool.h>

typedef struct {
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
float AvgInfo(Subject *subject, int nbSubjects);
float AvgGlobal(float avgInfo, float avgCG, float avgCBE);
float round01(float value);
float round05(float value);

#endif
