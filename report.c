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

float AvgGlobal(float avgInfo, float avgECG, float avgCBE) {
    const float INFCOEF = 3.0f;
    const float ECGCOEF = 2.0f;
    const float CBECOEF = 1.0f;
    const float TOTALCOEF = INFCOEF + ECGCOEF + CBECOEF;
    float totalPoints = (avgInfo * INFCOEF) + (avgECG * ECGCOEF) + (avgCBE * CBECOEF);
    if (TOTALCOEF == 0) {
       return 0;
    }
    return round01(totalPoints / TOTALCOEF);
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

static int collectModules(Subject *subject, char ***outModules) {
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
            char **temp = realloc(modules, (nModules + 1) * sizeof(char *));
            if (temp == NULL) {
                fprintf(stderr, "Error, memory allocation failed");
                free(modules);
                return nModules;
            }
            modules = temp;
            modules[nModules] = subject->grades[i].module;
            nModules++;
        }
    }

    *outModules = modules;
    return nModules;
}

float AvgOfModules(Subject *subject) {
    if (subject == NULL || subject->size <= 0) return 0;

    char **modules = NULL;
    int nModules = collectModules(subject, &modules);

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
    if (AvgOfModules(infoC) == 0) {
        return round01(AvgOfModules(infoI));
    }
    if (AvgOfModules(infoI) == 0) {
        return round01(AvgOfModules(infoC));
    }
    else {
        return round01((AvgOfModules(infoI) * I_COEF) + (AvgOfModules(infoC) * C_COEF));
    }
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

static void printSubjectDetails(FILE *f, Subject *subject, const char *subjectLabel) {
    fprintf(f, "--- %s (%d note(s)) ---\n", subjectLabel, subject->size);
    if (subject->size == 0) {
        fprintf(f, "  (aucune note)\n");
        fprintf(f, "  Moyenne: 0.0\n\n");
        return;
    }

    if (subject->type == 'I' || subject->type == 'C') {
        char **modules = NULL;
        int nModules = collectModules(subject, &modules);

        for (int m = 0; m < nModules; m++) {
            fprintf(f, "  Module [%s] :\n", modules[m]);
            float sumG = 0;
            float sumC = 0;
            for (int i = 0; i < subject->size; i++) {
                if (subject->grades[i].module && strcmp(subject->grades[i].module, modules[m]) == 0) {
                    Grade *g = &subject->grades[i];
                    if (g->has_coef) {
                        fprintf(f, "    - Note: %.1f (coef: %.2f)\n", g->value, g->coef);
                        sumG += g->value * g->coef;
                        sumC += g->coef;
                    } else {
                        fprintf(f, "    - Note: %.1f (non ponderee)\n", g->value);
                        sumG += g->value;
                        sumC += 1.0f;
                    }
                }
            }
            float modAvg = AvgModule(subject, modules[m]);
            if (sumC > 0) {
                fprintf(f, "    -> Calcul: %.2f / %.2f = %.3f | Arrondi (0.5): %.1f\n",
                        sumG, sumC, sumG / sumC, modAvg);
            }
        }
        free(modules);

        float avgModules = AvgOfModules(subject);
        fprintf(f, "  => Moyenne des modules %s: %.1f\n\n", subjectLabel, avgModules);
    } else {
        float sumG = 0;
        for (int i = 0; i < subject->size; i++) {
            Grade *g = &subject->grades[i];
            fprintf(f, "  - [%s] Note: %.1f", g->module ? g->module : "-", g->value);
            if (g->has_coef) {
                fprintf(f, " (coef: %.2f)\n", g->coef);
                sumG += g->value;
            } else {
                fprintf(f, " (non ponderee)\n");
                sumG += g->value;
            }
        }
        float avg = Avg(subject);
        fprintf(f, "  -> Somme notes: %.2f, Nb notes: %d | Moyenne simple: %.3f | Arrondi (0.5): %.1f\n",
                sumG, subject->size, sumG / (float)subject->size, avg);
        fprintf(f, "  => Moyenne %s: %.1f\n\n", subjectLabel, avg);
    }
}

static void printSemester(FILE *f, int sem) {
    Subject *subjMaths = (sem == 1) ? &maths : &maths2;
    Subject *subjEnglish = (sem == 1) ? &english : &english2;
    Subject *subjInfoI = (sem == 1) ? &infoI : &infoI2;
    Subject *subjInfoC = (sem == 1) ? &infoC : &infoC2;
    Subject *subjEcg = (sem == 1) ? &ecg : &ecg2;
    const char *tag = (sem == 1) ? "(Semestre 1)" : "(Semestre 2)";
    const char *suffix = (sem == 1) ? "" : "2";

    fprintf(f, "####################################################\n");
    fprintf(f, "                   SEMESTRE %d                       \n", sem);
    fprintf(f, "####################################################\n\n");

    char label[64];

    snprintf(label, sizeof(label), "Maths %s", tag);
    printSubjectDetails(f, subjMaths, label);
    snprintf(label, sizeof(label), "Anglais %s", tag);
    printSubjectDetails(f, subjEnglish, label);

    float avgMaths = Avg(subjMaths);
    float avgEnglish = Avg(subjEnglish);
    float cbe = AvgCBE(subjMaths, subjEnglish);
    fprintf(f, ">>> MOYENNE CBE %s :\n", tag);
    fprintf(f, "    Calcul : (Avg(Maths%s)=%.1f + Avg(Anglais%s)=%.1f) / 2 = %.2f\n",
            suffix, avgMaths, suffix, avgEnglish, (avgMaths + avgEnglish) / 2.0f);
    fprintf(f, "    Arrondi (0.1) : %.1f\n\n", cbe);

    snprintf(label, sizeof(label), "Info I %s", tag);
    printSubjectDetails(f, subjInfoI, label);
    snprintf(label, sizeof(label), "Info C %s", tag);
    printSubjectDetails(f, subjInfoC, label);

    float avgI = AvgOfModules(subjInfoI);
    float avgC = AvgOfModules(subjInfoC);
    float info = AvgInformatique(subjInfoI, subjInfoC);
    fprintf(f, ">>> MOYENNE INFORMATIQUE %s :\n", tag);
    fprintf(f, "    Info I (80%%) = %.1f, Info C (20%%) = %.1f\n", avgI, avgC);
    if (avgC == 0) {
        fprintf(f, "    (Info C = 0 -> Moyenne = Info I)\n");
    } else if (avgI == 0) {
        fprintf(f, "    (Info I = 0 -> Moyenne = Info C)\n");
    } else {
        fprintf(f, "    Calcul : (%.1f * 0.8) + (%.1f * 0.2) = %.3f\n",
                avgI, avgC, (avgI * 0.8f) + (avgC * 0.2f));
    }
    fprintf(f, "    Arrondi (0.1) : %.1f\n\n", info);

    snprintf(label, sizeof(label), "ECG %s", tag);
    printSubjectDetails(f, subjEcg, label);
    float ecg = Avg(subjEcg);

    float glob = AvgGlobal(info, ecg, cbe);
    fprintf(f, ">>> MOYENNE GENERALE %s :\n", tag);
    fprintf(f, "    Coef : Info x3, ECG x2, CBE x1\n");
    fprintf(f, "    Calcul : (Info=%.1f * 3 + ECG=%.1f * 2 + CBE=%.1f * 1) / 6 = %.3f\n",
            info, ecg, cbe, (info * 3.0f + ecg * 2.0f + cbe) / 6.0f);
    fprintf(f, "    Arrondi (0.1) : %.1f\n\n", glob);
}

void printCalculations(FILE *f) {
    fprintf(f, "====================================================\n");
    fprintf(f, "          EXTRACTION & CALCULS DES MOYENNES         \n");
    fprintf(f, "====================================================\n\n");
    fprintf(f, "Semestre actif courant : Semestre %d\n\n", currentSem);

    printSemester(f, 1);
    printSemester(f, 2);

    fprintf(f, "####################################################\n");
    fprintf(f, "                 MOYENNES ANNUELLES                 \n");
    fprintf(f, "####################################################\n\n");

    struct {
        const char *name;
        float s1;
        float s2;
    } rows[] = {
        {"CBE", AvgCBE(&maths, &english), AvgCBE(&maths2, &english2)},
        {"Informatique", AvgInformatique(&infoI, &infoC), AvgInformatique(&infoI2, &infoC2)},
        {"ECG", Avg(&ecg), Avg(&ecg2)},
    };

    float year[3];
    for (int i = 0; i < 3; i++) {
        year[i] = round05((rows[i].s1 + rows[i].s2) / 2.0f);
        fprintf(f, ">>> %s Annuelle :\n", rows[i].name);
        fprintf(f, "    Calcul : (S1=%.1f + S2=%.1f) / 2 = %.2f\n",
                rows[i].s1, rows[i].s2, (rows[i].s1 + rows[i].s2) / 2.0f);
        fprintf(f, "    Arrondi (0.5) : %.1f\n\n", year[i]);
    }

    float genYear = AvgGlobal(year[1], year[2], year[0]);
    fprintf(f, ">>> Moyenne Generale Annuelle :\n");
    fprintf(f, "    Coef : Info x3, ECG x2, CBE x1\n");
    fprintf(f, "    Calcul : (Info=%.1f * 3 + ECG=%.1f * 2 + CBE=%.1f * 1) / 6 = %.3f\n",
            year[1], year[2], year[0], (year[1] * 3.0f + year[2] * 2.0f + year[0]) / 6.0f);
    fprintf(f, "    Arrondi (0.1) : %.1f\n\n", genYear);

    fprintf(f, "====================================================\n");
}

void dumpCalculations(const char *logFile) {
    printCalculations(stdout);

    if (logFile != NULL) {
        FILE *f = fopen(logFile, "w");
        if (f != NULL) {
            printCalculations(f);
            fclose(f);
            printf("\n[DEBUG] Le rapport complet a egalement ete enregistre dans '%s'\n", logFile);
        }
    }
}
