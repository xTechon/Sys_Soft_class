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
    printf("\nkey String is: %s", key);
    while(key[i] != '\0'){
        //printf("\niterating through next index");
        mod = key[i] % size; //xk % m
        sum += R[i]*mod; //rkxk sum
        i++;
    }
    index = sum % size;
    return index;
}

int PushHash(OPLISTHEAD *arr[], int size, OPCODES* op){
    int key = CmpHash(size, op->Name);
    printf("\nKey generated: %d", key);
    if (arr[key] == NULL){
        printf("\nArray index is NULL");
        arr[key] = (OPLISTHEAD*)malloc(sizeof(OPLISTHEAD)); //create space for a new Head pointer
        arr[key] -> HEAD = NewOpList(&arr[key]->lSize);     //create the new Head pointer
        arr[key] -> lSize = PushLinkOP(arr[key]->HEAD, arr[key]->END, arr[key]->lSize, *op); //Push the OPCODE on the new linked list and increment lSize
        printf("\nlSize is: %d", arr[key] -> lSize);
    } else{
        printf("\nArray index contains some value");
        arr[key] -> lSize = PushLinkOP(arr[key]->HEAD, arr[key]->END, arr[key]->lSize, *op); //Push the OPCODE on the new linked list and increment lSize
    }
    return 1;
}

int FindHash(OPLISTHEAD *arr[], int size, OPCODES* op){
    int key = CmpHash(size, op->Name);
    if(FindOp(arr[key], op->Name) != NULL) return 1;
    else return 0;
}

int PrintHash(OPLISTHEAD *arr[], int size){
    printf("\nAvailable OpCodes:");
    for (int i = 0; i < size; i++){
        if (arr[i]!=NULL){
            PrintOPList(arr[i]->HEAD);
        }
    }
    return 1;
}
