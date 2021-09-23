#include "headers.h"

int CmprDir(char *Symbol);
int IsAValidSymbol (char *TestSymbol){
    int Result = 1;
    int maxlength = 6;

    //Does Symbol start w/ alpha character
    if ((int)TestSymbol[0] < 65 || (int)TestSymbol[0] > 90){
        Result = 0;
        return Result;
    }
    //check to see if it's a directive only if the first letter starts the same as one
    switch(TestSymbol[0]){
        case 'B':
        case 'E':
        case 'R':
        case 'S':
        case 'W':
            Result = CmprDir(TestSymbol);
            if (Result < 0) return Result;
            else {Result = 1; break;}
        default:
            break;
    }
    //test Symbol Length
    int i = 0;
    while((int) TestSymbol[i] != '\0' || i > 7){
        switch((int)TestSymbol[i]){
            case 32:  //Space
                Result = 0;
                return Result;
            case 36:  //$
                Result = 0;
                return Result;
            case 33:  //!
                Result = 0;
                return Result;
            case 40:  //(
                Result = 0;
                return Result;
            case 41:  //)
                Result = 0;
                return Result;
            case 43:  //+
                Result = 0;
                return Result;
            case 45:  //-
                Result = 0;
                return Result;
            case 61:  //=
                Result = 0;
                return Result;
            case 64:  //@
                Result = 0;
                return Result;
        }
        i++;
    }
    //printf("String length is: %i, Result is: %d", i, Result);
    if (i > maxlength) {Result = 0; return Result;}
    return Result;
}

int CmprDir(char *Symbol){
    char r[4] = "RES";
    int testR;
    switch (Symbol[0]){
        case'B': //-1
            if (!strcmp(Symbol, "BYTE")) return -1;
            else return 1;
            //operand is character or hexadecimal constant
        case'E': //-2 and -3
            testR = strcmp(Symbol, "END");
            //printf("Possible Dir: %s, test Result: %d\n", Symbol, testR);
            if (testR == 0){
                return -2;
            }
            testR = strcmp(Symbol, "EXPORTS");
            if (testR == 0){
                return -3;
                //3 bytes reserved
            }
            return 1;
        case 'R': //-4, -5, -6
            for (int i = 0; i < 3; i++){
                if(Symbol[i]!=r[i]) return 1;
            }
            switch (Symbol[3]){
                case 'B':
                    if (Symbol[4] == '\0')return -4;
                    else return 1;
                   //reserve num bytes
                case 'R':
                    if (Symbol[4] == '\0')return -5;
                    else return 1;
                    //reserve 3 bytes of mem
                case 'W':
                    if (Symbol[4] == '\0')return -6;
                    else return 1;
                    //reserve num words
                default:
                    break;
            }
            return 1;
        case 'S': //-7
            testR = strcmp(Symbol, "START");
            if (testR == 0){
                return -7;
            }else return 1;
            //next operand is start address
        case 'W': //-8
            testR = strcmp(Symbol, "WORD");
            if (testR == 0){
                return -8;
            }else return 1;
            //3 bytes

    }
    return 1;
}
