#include "report.h"

#include <stdio.h>
#include <string.h>

void create_main_window(int argc, char *argv[]);
void initSubjects(void);

static void printUsage(const char *progName) {
    printf("Usage : %s [OPTION] [FICHIER_LOG]\n\n", progName);
    printf("Options disponibles :\n");
    printf("  -c, --calc, --moyennes   Affiche le detail de tous les calculs de moyennes\n");
    printf("                           (charge data.json sans modifier les donnees)\n");
    printf("  -h, --help               Affiche cette aide\n\n");
    printf("Exemples :\n");
    printf("  %s --calc\n", progName);
    printf("  %s --calc mon_log.txt\n\n", progName);
    printf("Sans argument, lance l'application graphique GTK.\n");
}

static int isCalcFlag(const char *arg) {
    return strcmp(arg, "--calc") == 0 || strcmp(arg, "-c") == 0 ||
           strcmp(arg, "--moyennes") == 0 || strcmp(arg, "-m") == 0;
}

int main(int argc, char *argv[]) {
    if (argc > 1 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)) {
        printUsage(argv[0]);
        return 0;
    }

    if (argc > 1 && argv[1][0] == '-') {
        if (!isCalcFlag(argv[1])) {
            fprintf(stderr, "Option inconnue : %s\n\n", argv[1]);
            printUsage(argv[0]);
            return 1;
        }

        const char *logFile = (argc > 2) ? argv[2] : "calc_debug.log";

        initSubjects();
        loadData();
        dumpCalculations(logFile);
        return 0;
    }

    initSubjects();
    loadData();
    create_main_window(argc, argv);
    return 0;
}
