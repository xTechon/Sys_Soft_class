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

//OP code linked list for hash table
struct opLink {
    OPCODES node;
    struct opLink* next;
};
typedef struct opLink OPLIST;
int PrintOPList(OPLIST* HEAD);
OPLIST* FindOp(OPLIST* root, char *op);
int PushLinkOP(OPLIST* HEAD, OPCODES* addition);
OPLIST* NewOpList();
extern OPLIST* OpcodeTable[29];

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
int PushHash(OPLIST* arr[], int size, OPCODES* op);
int FindHash(OPLIST* arr[], int size, OPCODES* op);
int PrintHash(OPLIST* arr[], int size);
int ReadOpCodeFile();
