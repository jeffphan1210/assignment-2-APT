#include "utility.h"

void *safeMalloc(size_t size){
    void *p = malloc(size);
    if(p == NULL) {
        perror(NULL);
        exit(EXIT_FAILURE);
    }
    return p;
}

void swap (int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void checkInvalidString(char *a){
    if(a == NULL){
        perror("Error: invector string invalid");
        exit(EXIT_FAILURE);
    }
}
void printInvalidInput(){
    perror("Invalid input");
}
int checkStrtol(char *input){
    char *endPtr;
    int tmp = strtol(input,&endPtr,10);
    if(input == endPtr || *endPtr != '\0') {
        checkInvalidString(input);
        return -1;
    }
    return tmp;
}
void checkOverflow(char *str){
    if(str[strlen(str) - 1] != '\n') {
        printInvalidInput();
        exit(EXIT_FAILURE);
    }
}

int randomNumber100(){
    return (rand() % MAX + 1);
}
void printMismatchsize(){
    perror("allelesize mismatch with vector size\n");
}

