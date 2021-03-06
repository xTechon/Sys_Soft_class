#include "headers.h"

int CmpHash(int size, char* key){
    //printf("\nStarting key gen");
    //h(x) = r1x1 + r2x2 + ... + rkxk mod m
    int i = 0;
    int index = 0; //return value
    int sum = 0; //hold the summnation
    int mod = 0; //hold xk % m
    //took a lot of matlab to get these numbers
    int R[4] = {22, 22, 11, 17};
    //printf("\nkey String is: %s", key);
    while(key[i] != '\0'){
        //printf("\niterating through next index");
        mod = key[i] % size; //xk % m
        sum += R[i]*mod; //rkxk sum
        i++;
    }
    index = sum % size;
    return index;
}

int PushHash(OPLIST* arr[], int size, OPCODES* op){
    int key = CmpHash(size, op->Name);
    //printf("\nKey generated: %d", key);
    arr[key] = PushLinkOP(arr[key], *op);
    return 1;
}

OPCODES* FindHash(OPLIST* arr[], int size, char* op){
    int key = CmpHash(size, op);
    //printf("Key is: %d", key);
    OPLIST* temp = FindOp(arr[key], op);
    if(temp != NULL) return &(temp->node); //get the address of the OPCODE
    else return NULL;
}

int PrintHash(OPLIST* arr[], int size){
    printf("\nAvailable OpCodes:");
    for (int i = 0; i < size; i++){
        //printf("\ni is: %d", i);
        if (arr[i]!=NULL){
            //printf("\nsomething in index");
            PrintOPList(arr[i]);
        }
    }
    return 1;
}
