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

typedef struct symLink LIST;
struct symLink {
    SYMBOL node;
    LIST* next;
};

int printLink(LIST table);

struct opcodes {
	char	OpCode;
	char	Name[8];

};

typedef struct opcodes OPCODES;
OPCODES OpcodeTable[ 32 ];
