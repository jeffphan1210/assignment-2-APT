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
#endif


/* TODO not provided */
