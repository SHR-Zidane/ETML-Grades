#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
} Subject;

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
    return sumG / sumC;
}
