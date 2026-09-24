#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include "report.h"

cJSON *gradeToJson(Grade *grade) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "module", grade->module ? grade->module : "");
    cJSON_AddNumberToObject(json, "value", grade->value);
    cJSON_AddBoolToObject(json, "has_coef", grade->has_coef);
    cJSON_AddNumberToObject(json, "coef", grade->coef);

    return json;
}

cJSON *subjectToJson(Subject *subject) {
    cJSON *json = cJSON_CreateObject();
    if (subject->type != 0) {
        char type[2] = {subject->type, '\0'};
        cJSON_AddStringToObject(json, "type", type);
    }
    cJSON_AddStringToObject(json, "name", subject->name);
    cJSON_AddNumberToObject(json, "size", subject->size);
    cJSON *grades = cJSON_CreateArray();
    for (int i = 0; i < subject->size; i++) {
        cJSON_AddItemToArray(grades, gradeToJson(&subject->grades[i]));
    }
    cJSON_AddItemToObject(json, "grades", grades);
    return json;
}

cJSON *dataToJson(void) {
    cJSON *root = cJSON_CreateObject();

    Subject *subjects[] = {
        &maths,
        &english,
        &infoI,
        &infoC,
        &ecg
    };

    cJSON *jsonSubjects = cJSON_CreateArray();

    for (int i = 0; i < sizeof(subjects) / sizeof(subjects[0]); i++) {
        cJSON_AddItemToArray(jsonSubjects, subjectToJson(subjects[i]));
    }

    cJSON_AddItemToObject(root, "subjects", jsonSubjects);

    return root;
}

void saveData(void) {
    cJSON *json = dataToJson();
    char *str = cJSON_Print(json);

    FILE *file = fopen("data.json", "w");

    if (file == NULL) {
        perror("Failed to open file");
        free(str);
        cJSON_Delete(json);
        return;
    }

    fputs(str, file);
    fclose(file);

    free(str);
    cJSON_Delete(json);
}
