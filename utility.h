/******************************************************************************
 ** Student name:     ...
 ** Student number:     ...
 ** Course:             Advanced Programming Techniques - S2 2018
 ******************************************************************************/

#ifndef UTILITY_H
#define UTILITY_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define MAX 100

#define MIN_RAND 0.0
#define MAX_RAND 1.0
#define RANGE (MAX_RAND - MIN_RAND)
typedef enum boolean
{
    FALSE = 0,
    TRUE
} Boolean;
void *safeMalloc(size_t size);
void swap (int *a, int *b) ;
void printInvalidInput();
void checkInvalidString(char *a);
int checkStrtol(char *input);
void checkOverflow(char *str);
int randomNumber100();
void printMismatchsize();
#endif


/* TODO not provided */
