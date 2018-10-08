/******************************************************************************
 ** Student name:     ...
 ** Student number:     ...
 ** Course:             Advanced Programming Techniques - S2 2018
 ******************************************************************************/

#include "invector.h"

void invector_init(InVTable *invt) {
    invt->tot =0;
    invt->width = 0;
}


int convertInput(char *input,InVector a,int *counter,int size){
    char *current;
    int tmp;
    *counter = 0;
    current = strtok(input,INV_DELIM1);
    checkInvalidString(current);
    current = strtok(NULL,INV_DELIM2);
    checkInvalidString(current);
    tmp = checkStrtol(current);
    while(current != NULL && (*counter) < size+1){
        current = strtok(NULL,INV_DELIM3);
        checkInvalidString(current);
        a[*counter] = checkStrtol(current);
        (*counter)++;
    }
    (*counter)--;
    return tmp;
}

