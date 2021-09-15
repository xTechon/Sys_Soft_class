#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Symbol things
struct symbols {
    int DefinedOnSourceLine;
    int Address;
    char Name[7];
};

typedef struct symbols SYMBOL;

int IsAValidSymbol(char *TestSymbol);

//opcode things
struct opcodes {
	char	OpCode;
	char	Name[8];

};

typedef struct opcodes OPCODES;
OPCODES OpcodeTable[ 32 ];

//Linked list things
typedef struct symLink LIST;
struct symLink {
    SYMBOL node;
    struct symLink* next;
    struct symLink* prev;
};

int PrintList();
int PushLink(SYMBOL addition);
LIST NewList();

//binary tree things
struct branch {
    SYMBOL node;
    struct branch* left;
    struct branch* right;
};
typedef struct branch TREE;
