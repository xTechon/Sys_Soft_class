#include "headers.h"
#define DEBUG 0

RECLIST *PushLinkREC(RECLIST *TAIL, char *storage) {
#if DEBUG
  printf("PushLinkREC Called");
#endif
  RECLIST *newlist = (RECLIST *)malloc(sizeof(RECLIST));
  memset(newlist, 0, sizeof(RECLIST));
  int size = strlen(storage) + 1;
  printf("\nsize = %d", size);
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
  // memset(line, '\0', 70 * sizeof(char));
  while (current != NULL) {
    strcat(line, current->record);
    printf("\nCurrent line value: %s", line);
    current = current->next;
  }
  // ClearList(HEAD);
  return line;
}

void PrintList(RECLIST *HEAD) {
  printf("\nPrinting List\n");
  RECLIST *current = HEAD;
  while (current != NULL) {
    printf("%s", current->record);
    current = current->next;
  }
}

void Relative(RECLIST **rHEAD, RECLIST **TAIL, int locCount, int *recSize) {
  printf("\ncreating new T Record");
  char beginning[10];
  sprintf(beginning, "T%06X", locCount);
  *TAIL = PushLinkREC(*TAIL, beginning);
  *rHEAD = *TAIL;
  printf("\nrHEAD in function: %s", (*rHEAD)->record);
  printf("\nTAIL in function: %s", (*TAIL)->record);
  *recSize = 0;
}

void InsertLength(RECLIST **rHEAD, RECLIST **TAIL, int recSize) {
  RECLIST *size = (RECLIST *)malloc(sizeof(RECLIST));
  memset(size, 0, sizeof(RECLIST));
  size->record = malloc(3 * sizeof(char));
  memset(size->record, '\0', 3 * sizeof(char));
  printf("\nrHEAD in InsertLength: %s", (*rHEAD)->record);
  printf("\nTAIL in InsertLength: %s", (*TAIL)->record);
  printf("\nrHEAD->next is: %s", (*rHEAD)->next->record);
  sprintf(size->record, "%02X", recSize);
  size->next = (*rHEAD)->next;
  (*rHEAD)->next = size;
  *TAIL = PushLinkREC(*TAIL, "\n");
  *rHEAD = NULL;
}

void CreateModHEAD(RECLIST **MHEAD, char *mod, int size) {
  (*MHEAD)->record = malloc(size * sizeof(char));
  memset((*MHEAD)->record, '\0', size * sizeof(char));
  strcpy((*MHEAD)->record, mod);
}
