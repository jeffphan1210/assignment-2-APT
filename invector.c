/******************************************************************************
 ** Student name:     ...
 ** Student number:     ...
 ** Course:             Advanced Programming Techniques - S2 2018
 ******************************************************************************/

#include "invector.h"
#define CMD_ARG_PCBMILL "pcbmill"

void invector_init(InVTable *invt) {
    invt->tot =0;
    invt->width = 0;
}


int convertInput(char *input,InVector a,int *counter,int numAlle,char  *type){
    char *current;
    int tmp,tmpSize;
    *counter = 0;
    current = strtok(input,INV_DELIM1);
    checkInvalidString(current);
    current = strtok(NULL,INV_DELIM2);
    checkInvalidString(current);
    tmp = checkStrtol(current);
    printf("num alle %d",numAlle);
    if(strcmp(type,CMD_ARG_PCBMILL)==0) tmpSize =numAlle;
    else tmpSize = numAlle+1;
    while(current != NULL && (*counter) < tmpSize){

        printf("i'm here\n");
        current = strtok(NULL,INV_DELIM3);
        checkInvalidString(current);
        a[*counter] = checkStrtol(current);
        (*counter)++;
    }
    if(!(strcmp(type,CMD_ARG_PCBMILL)==0)) (*counter)--;
    return tmp;
}
