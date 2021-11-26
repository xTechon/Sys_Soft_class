#include "headers.h"
#define DEBUG 0

int CmprDir(char *Symbol);
int IsAValidSymbol(char *TestSymbol) {
  int Result = 1;
  int maxlength = 6;

  // Does Symbol start w/ alpha character
  if ((int)TestSymbol[0] < 65 || (int)TestSymbol[0] > 90) {
    Result = 0;
    return Result;
  }
  // check to see if it's a directive only if the first letter starts the same
  // as one
  switch (TestSymbol[0]) {
  case 'B':
  case 'E':
  case 'R':
  case 'S':
  case 'W':
    Result = CmprDir(TestSymbol);
    if (Result < 0)
      return Result;
    else {
      Result = 1;
      break;
    }
  default:
    break;
  }
  // test Symbol Length
  int i = 0;
  while ((int)TestSymbol[i] != '\0' || i > 7) {
    switch ((int)TestSymbol[i]) {
    case 32: // Space
      Result = 0;
      return Result;
    case 36: //$
      Result = 0;
      return Result;
    case 33: //!
      Result = 0;
      return Result;
    case 40: //(
      Result = 0;
      return Result;
    case 41: //)
      Result = 0;
      return Result;
    case 43: //+
      Result = 0;
      return Result;
    case 45: //-
      Result = 0;
      return Result;
    case 61: //=
      Result = 0;
      return Result;
    case 64: //@
      Result = 0;
      return Result;
    }
    i++;
  }
  // printf("String length is: %i, Result is: %d", i, Result);
  if (i > maxlength) {
    Result = 0;
    return Result;
  }
  return Result;
}

int CmprDir(char *Symbol) {
  char r[4] = "RES";
  int testR;
  switch (Symbol[0]) {
  case 'B': //-1
    if (!strcmp(Symbol, "BYTE"))
      return -1;
    else
      return 1;
    // operand is character or hexadecimal constant
  case 'E': //-2 and -3
    testR = strcmp(Symbol, "END");
    // printf("Possible Dir: %s, test Result: %d\n", Symbol, testR);
    if (testR == 0) {
      return -2;
    }
    testR = strcmp(Symbol, "EXPORTS");
    if (testR == 0) {
      return -3;
      // 3 bytes reserved
    }
    return 1;
  case 'R': //-4, -5, -6
    for (int i = 0; i < 3; i++) {
      if (Symbol[i] != r[i])
        return 1;
    }
    switch (Symbol[3]) {
    case 'B':
      if (Symbol[4] == '\0')
        return -4;
      else
        return 1;
      // reserve num bytes
    case 'R':
      if (Symbol[4] == '\0')
        return -5;
      else
        return 1;
      // reserve 3 bytes of mem
    case 'W':
      if (Symbol[4] == '\0')
        return -6;
      else
        return 1;
      // reserve num words
    default:
      break;
    }
    return 1;
  case 'S': //-7
    testR = strcmp(Symbol, "START");
    if (testR == 0) {
      return -7;
    } else
      return 1;
    // next operand is start address
  case 'W': //-8
    testR = strcmp(Symbol, "WORD");
    if (testR == 0) {
      return -8;
    } else
      return 1;
    // 3 bytes
  }
  return 1;
}

// rip windows, but I don't want to develop server-side
int ValHEX(char *eval) {
  int i = 0;
  while (eval[i] != '\0') {
    if ((eval[i] >= 48 && eval[i] <= 57)      // 0-9
        || (eval[i] >= 65 && eval[i] <= 70)   // A-F
        || (eval[i] >= 97 && eval[i] <= 102)) // a-f
    {
#if DEBUG
      printf("\neval[%d] is: %c", i, eval[i]);
#endif
      i++;
      continue;
    } else {
      return 0; // return 1 if it breaks hex convention
    }
  }
  return 1;
}

int checkOverflow(int count) {
  if (count >= 0x8000) {
    // printf("\nERROR: LOCATION %x SURPASSES SIC MEMORY\n", count);
    return 1;
  }
  return 0;
}
void KillWhiteChar(char *eval) {
  int i = 0;
  while (eval[i] != '\0') {
#if DEBUG
    printf("\nChar %d: %c", i, eval[i]);
#endif
    if (!((eval[i] >= 48 && eval[i] <= 57)         // 0-9
          || (eval[i] >= 65 && eval[i] <= 90)      // A-F
          || (eval[i] >= 97 && eval[i] <= 122))) { // a-f
      eval[i] = '\0';
      // printf("\nreplaced");
      break;
    }
    i++;
  }
  // printf("\nfree from while loop");
  return;
}
// found out regex is actually slower than original implementaiton
/*
int ValHEXreg(char* eval){
    static regex_t* regex;
    regex = malloc(sizeof(regex_t));
    memset(regex, 0, sizeof(regex_t));
    regoff_t off, len;
    static char *const HexPattern = "(\\d*[a-f]*)+|(\\d|[a-f])";
    if (regex == NULL){
        if(regcomp(regex, HexPattern, REG_ICASE)) {
            printf("ERROR: HEX PATTER COMPILATION FAILED");
            return 1;
        }
    }
    regmatch_t pmatch[1];
    if(regexec(regex, eval, 1, pmatch, 0)){
        return 0;
    }
    //off = pmatch[0].rm_so + (eval - );
    regfree(regex); //free the compiled memory buffer
    return 0;
}*/
