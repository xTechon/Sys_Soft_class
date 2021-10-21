#include "headers.h"
#define DEBUG 0

OPLIST *PushLinkOP(OPLIST *HEAD, OPCODES addition) {
  OPLIST *newlist = (OPLIST *)malloc(sizeof(OPLIST));
  memset(newlist, '\0', sizeof(OPLIST));
  newlist->node = addition;
  if (HEAD == NULL) {
    HEAD = newlist;
    HEAD->next = NULL;
  } else {
    newlist->next = HEAD;
  }
  return newlist;
} // end PushLink

OPLIST *FindOp(OPLIST *root, char *op) {
  OPLIST *current = root;
#if DEBUG
  printf("\n op is %s, \nMembers in index:", op);
#endif
  while (current != NULL) {
#if DEBUG
    printf("\n%s", current->node.Name);
    printf("\nstrcmp result: %d", strcmp(current->node.Name, op));
    printf("\n%s", current->node.Name);
    printf("\n%s", op);
    int i = 0;
    while (op[i] != '\0') {
      i++;
    }
    printf("\nop length is: %d", i);
#endif
    if (!(strcmp(current->node.Name, op))) {
      return current;
    }
    current = current->next;
  }
  // printf("\nERROR: \"%s\" IS NOT A VALID OPCODE", op);
  return NULL;
}

int PrintOPList(OPLIST *HEAD) {
  OPLIST *current = HEAD;
  // printf("\nAvailable OpCodes:");
  while (current != NULL) {
    printf("\n%s\t%02x", current->node.Name,
           current->node.OpCode); // print node data to terminal
    current = current->next;      // go to next list
  }
  return 1;
} // end PrintLink
