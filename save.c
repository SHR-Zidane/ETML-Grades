#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    int sem = 1;
    if (subject == &maths2 || subject == &english2 || subject == &ecg2 || subject == &infoI2 || subject == &infoC2) {
        sem = 2;
    }
    cJSON_AddNumberToObject(json, "sem", sem);
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
    cJSON_AddNumberToObject(root, "current_sem", currentSem);

    Subject *subjects[] = {
        &maths,
        &english,
        &infoI,
        &infoC,
        &ecg,
        &maths2,
        &english2,
        &infoI2,
        &infoC2,
        &ecg2
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

Grade gradeFromJson(cJSON *json) {
    Grade grade;
    cJSON *moduleItem = cJSON_GetObjectItem(json, "module");
    cJSON *valueItem = cJSON_GetObjectItem(json, "value");
    cJSON *hasCoefItem = cJSON_GetObjectItem(json, "has_coef");
    cJSON *coefItem = cJSON_GetObjectItem(json, "coef");

    if (moduleItem && cJSON_IsString(moduleItem) && moduleItem->valuestring != NULL) {
        grade.module = strdup(moduleItem->valuestring);
    } else {
        grade.module = strdup("");
    }

    if (valueItem && cJSON_IsNumber(valueItem)) {
        grade.value = (float)valueItem->valuedouble;
    } else {
        grade.value = 0.0f;
    }

    if (hasCoefItem && cJSON_IsBool(hasCoefItem)) {
        grade.has_coef = cJSON_IsTrue(hasCoefItem);
    } else {
        grade.has_coef = false;
    }

    if (coefItem && cJSON_IsNumber(coefItem)) {
        grade.coef = (float)coefItem->valuedouble;
    } else {
        grade.coef = 1.0f;
    }

    return grade;
}

static Subject *getSubjectFromJson(cJSON *subjectJson) {
    cJSON *nameItem = cJSON_GetObjectItem(subjectJson, "name");
    const char *name = (nameItem && nameItem->valuestring) ? nameItem->valuestring : "";

    cJSON *semItem = cJSON_GetObjectItem(subjectJson, "sem");
    if (!semItem) {
        semItem = cJSON_GetObjectItem(subjectJson, "semester");
    }
    int sem = 1;
    if (semItem && cJSON_IsNumber(semItem)) {
        sem = semItem->valueint;
    }

    if (sem == 2) {
        if (strcmp(name, "Maths") == 0) return &maths2;
        if (strcmp(name, "Anglais") == 0) return &english2;
        if (strcmp(name, "ECG") == 0) return &ecg2;
        if (strcmp(name, "InfoI") == 0 || strcmp(name, "I") == 0) return &infoI2;
        if (strcmp(name, "InfoC") == 0 || strcmp(name, "C") == 0) return &infoC2;

        cJSON *typeItem = cJSON_GetObjectItem(subjectJson, "type");
        if (typeItem && typeItem->valuestring) {
            if (strcmp(typeItem->valuestring, "I") == 0) return &infoI2;
            if (strcmp(typeItem->valuestring, "C") == 0) return &infoC2;
        }
    } else {
        if (strcmp(name, "Maths") == 0) return &maths;
        if (strcmp(name, "Anglais") == 0) return &english;
        if (strcmp(name, "ECG") == 0) return &ecg;
        if (strcmp(name, "InfoI") == 0 || strcmp(name, "I") == 0) return &infoI;
        if (strcmp(name, "InfoC") == 0 || strcmp(name, "C") == 0) return &infoC;

        cJSON *typeItem = cJSON_GetObjectItem(subjectJson, "type");
        if (typeItem && typeItem->valuestring) {
            if (strcmp(typeItem->valuestring, "I") == 0) return &infoI;
            if (strcmp(typeItem->valuestring, "C") == 0) return &infoC;
        }
    }

    return NULL;
}

void loadData(void) {
    FILE *file = fopen("data.json", "r");
    if (file == NULL) {
        return;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (length <= 0) {
        fclose(file);
        return;
    }

    char *buffer = malloc(length + 1);
    if (buffer == NULL) {
        fclose(file);
        return;
    }

    size_t bytesRead = fread(buffer, 1, length, file);
    buffer[bytesRead] = '\0';
    fclose(file);

    cJSON *json = cJSON_Parse(buffer);
    free(buffer);

    if (json == NULL) {
        return;
    }

    cJSON *currentSemItem = cJSON_GetObjectItem(json, "current_sem");
    if (currentSemItem && cJSON_IsNumber(currentSemItem)) {
        currentSem = currentSemItem->valueint;
    }

    cJSON *subjects = cJSON_GetObjectItem(json, "subjects");
    if (cJSON_IsArray(subjects)) {
        int subjectsCount = cJSON_GetArraySize(subjects);
        for (int i = 0; i < subjectsCount; i++) {
            cJSON *subjectJson = cJSON_GetArrayItem(subjects, i);
            Subject *subject = getSubjectFromJson(subjectJson);
            if (subject == NULL) {
                continue;
            }

            cJSON *grades = cJSON_GetObjectItem(subjectJson, "grades");
            if (cJSON_IsArray(grades)) {
                int gradesCount = cJSON_GetArraySize(grades);
                for (int j = 0; j < gradesCount; j++) {
                    cJSON *gradeJson = cJSON_GetArrayItem(grades, j);
                    Grade grade = gradeFromJson(gradeJson);
                    addGrade(subject, grade);
                }
            }
        }
    }

    if (currentSem == 1 && (maths2.size > 0 || english2.size > 0 || ecg2.size > 0 || infoI2.size > 0 || infoC2.size > 0)) {
        currentSem = 2;
    }

    cJSON_Delete(json);
}
