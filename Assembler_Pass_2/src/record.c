#include "headers.h"
#define DEBUG 0

RECLIST *PushLinkREC(RECLIST *TAIL, char *storage) {
#if DEBUG
  printf("PushLinkREC Called");
#endif
  RECLIST *newlist = (RECLIST *)malloc(sizeof(RECLIST));
  memset(newlist, 0, sizeof(RECLIST));
  int size = strlen(storage);
  // allocate space to copy string to record
  newlist->record = malloc(size * sizeof(char));
  memset(newlist->record, '\0', size * sizeof(char));
  // copy contents of storage to record
  strncpy(newlist->record, storage, size);
  printf("\nPushed String is: %s", newlist->record);
  TAIL->next = newlist;
  newlist->next = NULL;
  return newlist;
}

void ClearList(RECLIST *HEAD) {
  if (HEAD != NULL) {
    ClearList(HEAD->next);
    free(HEAD->record);
    free(HEAD->next);
  }
  return;
}

char *RetrieveREC(RECLIST *HEAD) {
  RECLIST *current = HEAD;
  char *line = malloc(70 * sizeof(char));
  memset(line, '\0', 70 * sizeof(char));
  while (current != NULL) {
    strcat(line, current->record);
    printf("\nCurrent line value: %s", line);
    current = current->next;
  }
  ClearList(HEAD);
  return line;
}

RECLIST *Trec(RECLIST *TAIL, int startAdd, int opcode, int add) {
  static int length = 0;
  if (TAIL != NULL) {
    static RECLIST *tempH = NULL;
    // initalize temp if first time
    if (!tempH) {
      tempH = malloc(sizeof(RECLIST));
      memset(tempH, 0, sizeof(RECLIST));
    }
    // start adding opcodes and operands here
    char instruction[7];
    sprintf(instruction, "%2X%04X", opcode, add);
    TAIL = PushLinkREC(TAIL, instruction);

    // Case for when RSUB is reached or length is maxed out
    if (opcode == 0x4C || length > 70) {
      return TAIL;
      // terminate case
    }
  } else {
    char beginning[8];
    sprintf(beginning, "T%06X", startAdd);
    TAIL = PushLinkREC(TAIL, beginning);
    length = 0;
    return TAIL;
  }
  return NULL;
}
