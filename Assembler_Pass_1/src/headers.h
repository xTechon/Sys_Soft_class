#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct symbols {
    int DefinedOnSourceLine;
    int Address;
    char Name[7];
};

typedef struct symbols SYMBOL;

int IsAValidSymbol(char *TestSymbol);