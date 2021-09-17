#include "headers.h"


int cmpHash(int size, char* key){
    //h(x) = r1x1 + r2x2 + ... + rkxk mod m
    int i = 0;
    int index = 0; //return value
    int sum = 0; //hold the summnation
    int mod = 0; //hold xk % m
    //took a lot of matlab to get these numbers
    int R[4] = {22, 22, 11, 17};
    while(key[i] != '\0'){
        mod = key[i] % size; //xk % m
        sum += R[i]*mod; //rkxk sum
    }
    index = sum % size;
    return index;
}

int pushHash(OPCODES *arr[], int size, OPCODES *op){
    int key = cmpHash(size, op->Name);
    if (arr[key] == NULL){

    }

    return 1;
}
