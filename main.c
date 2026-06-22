#include "report.h"

void create_main_window(int argc, char *argv[]);
void initSubjects(void);
int main(int argc, char *argv[]) {
    initSubjects();

    create_main_window(argc, argv);
    return 0;
}
