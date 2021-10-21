#include "headers.h"
#define DEBUG 0

RECLIST *PushLinkREC(RECLIST *TAIL, char* storage){
    RECLIST *newlist = (RECLIST *)malloc(sizeof(RECLIST));
    memset(newlist, 0, sizeof(RECLIST));
    strcpy(newlist->record,storage);
    TAIL->next = newlist;
    newlist->next = NULL;
    return newlist;
}

void ClearList(RECLIST *HEAD){
    if (HEAD != NULL){
        ClearList(HEAD->next);
        free(HEAD->next);
    }
    return;
}

char* RetrieveREC(RECLIST *HEAD){
    RECLIST *current = HEAD;
    char *line = malloc(70 * sizeof(char));
    memset(line, '\0', 70 * sizeof(char));
    while (current != NULL){
        strcat(line, current->record);
        current = current->next;
    }
    ClearList(HEAD);
    return line;
}
