#include "headers.h"
#define DEBUG 0

RECLIST *PushLinkREC(RECLIST *TAIL, char *storage) {
#if DEBUG
  printf("PushLinkREC Called");
#endif
  RECLIST *newlist = (RECLIST *)malloc(sizeof(RECLIST));
  memset(newlist, 0, sizeof(RECLIST));
  int size = strlen(storage) + 1;
#if DEBUG
  printf("\nsize = %d", size);
#endif
  // allocate space to copy string to record
  newlist->record = malloc(size * sizeof(char));
  memset(newlist->record, '\0', size * sizeof(char));
  // copy contents of storage to record
  strncpy(newlist->record, storage, size);
#if DEBUG
  printf("\nPushed String is: %s", newlist->record);
#endif
  TAIL->next = newlist;
  newlist->next = NULL;
  return newlist;
}

void PrintList(RECLIST *HEAD) {
  printf("\nPrinting List\n");
  RECLIST *current = HEAD;
  while (current != NULL) {
    printf("%s", current->record);
    current = current->next;
  }
}

void outputList(RECLIST *HEAD, FILE *stream) {
  // printf("\nOutputing List to file\n");
  RECLIST *current = HEAD;
  while (current != NULL) {
    fprintf(stream, "%s", current->record);
    current = current->next;
  }
}

void Relative(RECLIST **rHEAD, RECLIST **TAIL, int locCount, int *recSize) {
#if DEBUG
  printf("\ncreating new T Record");
#endif
  char beginning[10];
  sprintf(beginning, "T%06X", locCount);
  *TAIL = PushLinkREC(*TAIL, beginning);
  *rHEAD = *TAIL;
#if DEBUG
  printf("\nrHEAD in function: %s", (*rHEAD)->record);
  printf("\nTAIL in function: %s", (*TAIL)->record);
#endif
  *recSize = 0;
}

void InsertLength(RECLIST **rHEAD, RECLIST **TAIL, int recSize) {
  RECLIST *size = (RECLIST *)malloc(sizeof(RECLIST));
  memset(size, 0, sizeof(RECLIST));
  size->record = malloc(3 * sizeof(char));
  memset(size->record, '\0', 3 * sizeof(char));
#if DEBUG
  printf("\nrHEAD in InsertLength: %s", (*rHEAD)->record);
  printf("\nTAIL in InsertLength: %s", (*TAIL)->record);
  printf("\nrHEAD->next is: %s", (*rHEAD)->next->record);
#endif
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
