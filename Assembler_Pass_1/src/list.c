#include "headers.h"

//keep track of the beginning and end of the list
//SYMLIST* END;
//SYMLIST* HEAD;
//int lSize;

int PrintSymList(SYMLIST* HEAD){
    SYMLIST* current = HEAD;
    while (current != NULL){
        printf("\n%s\t%d", current->node.Name, current->node.Address); //print node data to terminal
        current = current->next; //go to next list
    }
    return 1;
}//end PrintLink

int PushLinkSym(SYMLIST *HEAD, SYMLIST *END, int lSize, SYMBOL addition){
    SYMLIST* newLink = NULL; //make a new pointer for the new Link
    newLink = (SYMLIST*)malloc(sizeof(SYMLIST)); //create a new link in memory and assign the pointer to it
    //(I didn't know -> applied in C also)
    newLink->node = addition; //access the node symbol from newLink pointer and copy the data from symbol
    newLink->next = NULL;     //make the pointer of the newLink empty
    if (lSize > 1) {
        END->next = newLink;      //set the next of the end of linked list to the current pointer
        newLink->prev = END;      //set end as the previous pointer
        END =  newLink;           //set the new END pointer
    }else if (lSize == 1){
        END = newLink;
        HEAD->next = END;
        END->prev = HEAD;
    }else if (lSize == 0){
        HEAD = newLink;
    }else {
        printf("ERROR: Attepmt to add link to non-existent list");
        return 0;
    }
    lSize++;
    return 1;
}//end PushLink

//creates a new list
SYMLIST NewSymList(int *size){
    SYMLIST* HEAD = (SYMLIST*)malloc(sizeof(SYMLIST));
    HEAD->prev = NULL;
    HEAD->next = NULL;
    //END = HEAD;
    *size = 0;
    printf("\ncreated list");
    return *HEAD;
}//end NewList()

OPLIST NewOpList(int *size){
    OPLIST* HEAD = (OPLIST*)malloc(sizeof(OPLIST));
    HEAD->prev = NULL;
    HEAD->next = NULL;
    //END = HEAD;
    *size = 0;
    printf("\ncreated list");
    return *HEAD;
}//end NewList()

int PushLinkOP(OPLIST *HEAD, OPLIST *END, int lSize, OPLIST addition){
    OPLIST* newLink = NULL; //make a new pointer for the new Link
    newLink = (OPLIST*)malloc(sizeof(OPLIST)); //create a new link in memory and assign the pointer to it
    //(I didn't know -> applied in C also)
    newLink->node = addition; //access the node symbol from newLink pointer and copy the data from symbol
    newLink->next = NULL;     //make the pointer of the newLink empty
    if (lSize > 1) {
        END->next = newLink;      //set the next of the end of linked list to the current pointer
        newLink->prev = END;      //set end as the previous pointer
        END =  newLink;           //set the new END pointer
    }else if (lSize == 1){
        END = newLink;
        HEAD->next = END;
        END->prev = HEAD;
    }else if (lSize == 0){
        HEAD = newLink;
    }else {
        printf("ERROR: Attepmt to add link to non-existent list");
        return 0;
    }
    lSize++;
    return 1;
}//end PushLink
