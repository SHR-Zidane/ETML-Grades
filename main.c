#include <stdio.h>
#include "report.h"

int main(){
    Grade C295[] = {
        {2.0f, true, 4.0f},
        {3.0f, true, 6.0f}
    };
    Grade I426[] = {
        {4.0f, false, 0.0f},
        {4.0f, false, 0.0f}
    };

    Subject MoyenneInf[] = {
        {"I426", I426, 2, 'I'},
        {"C295", C295, 2, 'C'}
    };

    int NbSubject = sizeof(MoyenneInf) / sizeof(MoyenneInf[0]);
    float AvgInf = AvgInfo(MoyenneInf, NbSubject);
    printf("moyenne en info : %f", AvgInf);
    return 0;
}
