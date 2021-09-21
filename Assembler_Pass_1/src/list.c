#include "headers.h"

OPLIST* PushLinkOP(OPLIST* HEAD, OPCODES addition){
   OPLIST* newlist = (OPLIST*) malloc(sizeof(OPLIST));
   memset(newlist, '\0', sizeof(OPLIST));
   newlist->node = addition;
   if (HEAD == NULL){
       HEAD = newlist;
       HEAD->next = NULL;
   }else {
       newlist->next = HEAD;
   }
   return newlist;
}//end PushLink

OPLIST* FindOp(OPLIST* root, char *op){
    OPLIST* current = root;
    while(current != NULL){
        if(!(strcmp(current->node.Name, op))){
            return current;
        }
        current = current->next;
    }
    printf("\nERROR: %s IS NOT A VALID OPCODE", op);
    return NULL;
}

int PrintOPList(OPLIST* HEAD){
    OPLIST* current = HEAD;
    //printf("\nAvailable OpCodes:");
    while (current != NULL){
        printf("\n%s\t%x", current->node.Name, current->node.OpCode); //print node data to terminal
        current = current->next; //go to next list
    }
    return 1;
}//end PrintLink
