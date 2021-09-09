#include "headers.h"

int IsAValidSymbol (char *TestSymbol){
    int Result = 1;


    //Does Symbol start w/ alpha character
    if ((int)TestSymbol[0] < 65 || (int)TestSymbol[0] > 90){
        Result = 0;
        return Result;
    }

    //test Symbol Length
    int i = 0;
    while(TestSymbol[i] != NULL || i > 6){
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
    //printf("String length is: %i", i);
    if (i > 6) {Result = 0; return Result;}
    return Result;
}

int CmprDir(char *Symbol){
    //using binary search algorithm
    char *Dir[7] = {"BYTE", "END", "RESB", "RESR", "RESW", "START", "WORD"};
    char *mid = Dir[4];
    int m = 4;
    int l = 0;
//    while(l <=){

//    }
}
