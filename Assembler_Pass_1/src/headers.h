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
	int	OpCode;
	char	Name[8];
    int     Args;
};

typedef struct opcodes OPCODES;

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
int PushLinkOP(OPLIST* HEAD, OPLIST* END, int lSize, OPCODES addition);
OPLIST* NewOpList(int* size);
OPLIST* FindOp(OPLISTHEAD* root, char *op);
int PrintOPList(OPLIST* HEAD);
extern OPLISTHEAD* OpcodeTable[29];
//binary tree things
struct branch {
    SYMBOL node;
    struct branch* left;
    struct branch* right;
};
typedef struct branch TREE;
int PushLeaf(SYMBOL leaf);
SYMBOL FindSymbol(char* leaf);
TREE* TraversInOrder(TREE* localRoot);
int PrintTree();

//Hash table things
int CmpHash(int size, char* key);
int PushHash(OPLISTHEAD *arr[], int size, OPCODES* op);
int FindHash(OPLISTHEAD *arr[], int size, OPCODES* op);
int PrintHash(OPLISTHEAD *arr[], int size);
int ReadOpCodeFile();
