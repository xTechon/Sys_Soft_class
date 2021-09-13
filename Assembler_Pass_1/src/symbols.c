#include "headers.h"

int CmprDir(char *Symbol);
int IsAValidSymbol (char *TestSymbol){
    int Result = 1;


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
            if (Result == -1) return Result;
            else break;
        default:
            break;
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
    char r[4] = "RES";
    //int testR;
    switch (Symbol[0]){
        case'B':
            return -(!strcmp(Symbol, "BYTE"));
        case'E':
            //testR = -(!strcmp(Symbol, "END"));
            //printf("Possible Dir: %s, test Result: %d\n", Symbol, testR);
            return -(!strcmp(Symbol, "END"));
        case 'R':
            for (int i = 0; i < 3; i++){
                if(Symbol[i]!=r[i]) return -1;
            }
            switch (Symbol[3]){
                case 'B':
                case 'R':
                case 'W':
                    if (Symbol[4] == NULL)return -1;
                    else return 1;
                default:
                    break;
            }
            return 1;
        case 'S':
            return -(!strcmp(Symbol, "START"));
        case 'W':
            return -(!strcmp(Symbol, "WORD"));
    }
    return 1;
}
