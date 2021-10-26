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

void PrintList(RECLIST *HEAD){
  RECLIST *current = HEAD;
  while(current != NULL){
    printf("\n%s", current->record);
    current = current->next;
  }
}

void Relative(RECLIST **rHEAD, RECLIST **TAIL, int locCount, int *recSize){
  printf("\ncreating new T Record");
  char beginning[10];
  sprintf(beginning, "T%06X", locCount);
  *rHEAD = *TAIL;
  *TAIL = PushLinkREC(*TAIL, beginning);
  *recSize = 0;
}
