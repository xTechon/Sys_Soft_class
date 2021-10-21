#include "headers.h"
#define DEBUG 0

int CmpHash(int size, char *key) {
  // printf("\nStarting key gen");
  // h(x) = r1x1 + r2x2 + ... + rkxk mod m
  int i = 0;
  int index = 0; // return value
  int temp = 0;  // hold the value between sums
  int sum = 0;   // hold the summnation
  int mod = 0;   // hold xk % m
  // took a lot of matlab to get these numbers
  int R[4] = {22, 22, 11, 17};

#if DEBUG
  printf("\nkey String is: %s", key);
#endif
  while (key[i] != '\0' && i < 4) {
#if DEBUG
    printf("\n index %d has char: %c with value %d", i, key[i], key[i]);
#endif
    mod = key[i] % size; // xk % m
#if DEBUG
    printf(" %% %d = %2d", size, mod);
#endif
    if ((key[i] >= 65 && key[i] <= 90)        // A-F
        || (key[i] >= 97 && key[i] <= 122)) { // a-f
      temp = R[i] * mod;                      // rkxk sum
      sum += temp;
#if DEBUG
      printf(" * %d += %d", R[i], temp);
#endif
    } else {
      sum += 0;
    }
    i++;
  }
  index = sum % size;
#if DEBUG
  printf("\n%d %% %d = %d", sum, size, index);
#endif
  return index;
}

int PushHash(OPLIST *arr[], int size, OPCODES *op) {
  int key = CmpHash(size, op->Name);
#if DEBUG
  printf("\nKey generated: %d", key);
#endif
  arr[key] = PushLinkOP(arr[key], *op);
  return 1;
}

OPCODES *FindHash(OPLIST *arr[], int size, char *op) {
  int key = CmpHash(size, op);
#if DEBUG
  printf("\nKey is: %d", key);
#endif
  OPLIST *temp = FindOp(arr[key], op);
  if (temp != NULL)
    return &(temp->node); // get the address of the OPCODE
  else
#if DEBUG
    printf("\n %d is Empty", key);
#endif
  return NULL;
}

int PrintHash(OPLIST *arr[], int size) {
  printf("\nAvailable OpCodes:");
  for (int i = 0; i < size; i++) {
    printf("\nindex is: %d", i);
    if (arr[i] != NULL) {
#if DEBUG
      printf("\nsomething in index");
#endif
      PrintOPList(arr[i]);
    }
  }
  return 1;
}
