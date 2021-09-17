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
//OPCODES OpcodeTable[ 32 ];

//Linked list things
typedef struct symLink SYMLIST;
struct symLink {
    SYMBOL node;
    struct symLink* next;
    struct symLink* prev;
};
typedef struct symList SYMLISTHEAD;
struct symList {
    SYMLIST* HEAD;
    SYMLIST* END;
    int lSize;

};
int PrintSymList(SYMLIST* HEAD);
int PushLinkSym(SYMLIST* HEAD, SYMLIST* END, int lSize, SYMBOL addition);
SYMLIST NewSymList(int *size);
//OP code linked list for hash table
struct opLink {
    OPCODES node;
    struct opLink* next;
    struct opLink* prev;
};
typedef struct opLink OPLIST;
typedef struct opList OPLISTHEAD;
struct opList {
    OPLIST* HEAD;
    OPLIST* END;
    int lSize;

};
//binary tree things
struct branch {
    SYMBOL node;
    struct branch* left;
    struct branch* right;
};
typedef struct branch TREE;
