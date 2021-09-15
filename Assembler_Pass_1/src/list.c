#include "headers.h"

//keep track of the beginning and end of the list
LIST* END;
LIST* HEAD;
int lSize;

int PrintList(){
    LIST* current = HEAD;
    while (current->next != NULL){
        printf("%s\t%d\n", current->node.Name, current->node.Address); //print node data to terminal
        current = current->next; //go to next list
    }
    return 1;
}//end PrintLink

int PushLink(SYMBOL addition){
    LIST* newLink = NULL; //make a new pointer for the new Link
    newLink = (LIST*)malloc(sizeof(LIST)); //create a new link in memory and assign the pointer to it

    newLink->node = addition; //access the node symbol from newLink pointer (I didn't know -> applied in C also)
    newLink->next = NULL;     //make the pointer of the newLink empty
    if (lSize > 1) {
        END->next = newLink;      //set the next of the end of linked list to the current pointer
        newLink->prev = END;      //set end as the previous pointer
        END =  newLink;           //set the new END pointer
    }else if (lSize < 1){
        printf("ERROR: Attepmt to add link to empty list");
        return 0;
    }else{
        END = newLink;
        HEAD->next = END;
        END->prev = HEAD;
    }
    return 1;
}//end PushLink

//creates a new list
LIST NewList(){
    HEAD = (LIST*)malloc(sizeof(LIST));
    HEAD->prev = NULL;
    HEAD->next = NULL;
    END = HEAD;
    lSize = 1;
    return *HEAD;
}//end NewList()
