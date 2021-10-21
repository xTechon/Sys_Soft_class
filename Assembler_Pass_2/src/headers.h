#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
//#include <regex.h>

typedef struct opLink OPLIST;
extern OPLIST *OpcodeTable[29]; // array used often

// Hex value things
int ValHEX(char *eval);
int checkOverflow(int count);

// Symbol things
struct symbols {
  int SourceLine;
  int Address;
  char Name[7];
};
typedef struct symbols SYMBOL;
int IsAValidSymbol(char *TestSymbol);

// opcode things
struct opcodes {
  int OpCode;
  char Name[8];
  int Args;
};
typedef struct opcodes OPCODES;

// OP code linked list for hash table
struct opLink {
  OPCODES node;
  struct opLink *next;
};
int PrintOPList(OPLIST *HEAD);          // returns 1 if successful
OPLIST *FindOp(OPLIST *root, char *op); // returns pointer to found op
OPLIST *PushLinkOP(OPLIST *HEAD,
                   OPCODES addition); // returns pointer to new op head

// binary tree things
struct branch {
  SYMBOL node;
  struct branch *left;
  struct branch *right;
};
typedef struct branch TREE;
int PushLeaf(SYMBOL leaf);             // returns 1 if successful
SYMBOL FindSymbol(char *leaf);         // returns SYMBOL if symbol found
TREE *TraversInOrder(TREE *localRoot); // recursively traverses tree given root
int PrintTree();                       // returns 1 if successful
TREE *getTitleNode();

// Hash table things
int CmpHash(int size, char *key); // returns index to insert at
int PushHash(OPLIST *arr[], int size, OPCODES *op); // returns 1 if successful
OPCODES *
FindHash(OPLIST *arr[], int size,
         char *op); // returns the address of the opcode if opcode exists
int PrintHash(OPLIST *arr[], int size); // prints 1 if successful
int ReadOpCodeFile();

int CmprDir(char *Symbol);      // returns -1 if dir, 1 if not
void KillWhiteChar(char *eval); // trucates string at given character

//Records list
struct strRec{
  char* record;
  struct strRec *next;
};
typedef struct strRec RECLIST;
RECLIST *PushLinkREC(RECLIST *TAIL, char* storage); //Pushes a new link to the list
void ClearList(RECLIST *HEAD);                      //Recursively clears the list given the head
char* RetrieveREC(RECLIST *HEAD);                   //Returns a proper string of the record and clears the list
