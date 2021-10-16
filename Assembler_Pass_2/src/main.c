#include "headers.h"
#define TEST 0
#define DEBUG 0
#define ELMSG(L, V) printf("\nERROR %2d: LOCATION %x SURPASSES SIC MEMORY\n", L, V);

int TestMode();
//int checkOverflow(int count);
int main(int argc, char *argv[]){
#if TEST
    TestMode();return 0; //uncomment for testing mode
#endif
    //File Checking Section of code
    if (argc != 2) { //check to see if correct amnt of arguments entered
        printf("ERROR: Usage: %s filename\n", argv[0]);
        return 0;
    }
    FILE *fp;
    fp = fopen(argv[1], "r");
    if (fp == NULL){
        printf ("ERROR: %s could not be opened for reading.\n", argv[1]);
        return 0;
    }

#if DEBUG
    printf("\nFile Opened successfully.");
#endif

    memset(OpcodeTable, 0, 29*sizeof(OPLIST*));
    ReadOpCodeFile(); //import opcodes

    char line[1024];
    char fline[1024];
    char* newsym;
    int errC = 0;
    newsym = malloc(1024*sizeof(char));
    memset(newsym, '\0', 1024*sizeof(char));

    char* nextToken;
    nextToken = malloc(1024*sizeof(char));
    memset(nextToken, '\0', 1024*sizeof(char));

    char* operand;
    operand = malloc(1024*sizeof(char));
    memset(operand, '\0', 1024*sizeof(char));


    char* argument;
    argument = malloc(1024*sizeof(char));
    memset(argument, '\0', 1024*sizeof(char));

    int lCount = 0;   //keeps track of the line number
    int locCount = 0; //location in BYTES not words
    SYMBOL sym;
    int dirTrack = 0;
    //int hexCounter = 0;
    while (fgets(line, 1024, fp) !=NULL){
        strcpy (fline, line);
        lCount++; //Keep track of lines
        if (line[0] == 35){
#if DEBUG
            printf("Comment: %s", line); //remove or comment out before submission
#endif
            continue;
        }
        if (line[0] == '\n'){ //quit if it finds a blank line
            printf("\nERROR: FILE HAS BLANK LINES\n");
            return 0;
        }
        if ((line[0] >= 65) && (line[0] <= 90)){ //check for symbols that start with capitals
            newsym = strtok(line, " \t\n");
#if DEBUG
            printf("\nNEW SYMBOL ON LINE: %d", lCount);
            printf("\nNEW SYMBOL IS: %s", newsym);
#endif
            errC = IsAValidSymbol(newsym);
            if (errC != 1){
                printf("\nERROR %2d: INVALID SYMBOL \"%s\" WITH CODE: %d\n", lCount, newsym, errC);
                fclose(fp);
                return 0;
            }
            //set some of the values of the new Symbol
            strcpy(sym.Name, newsym);
            sym.SourceLine = lCount;
            if (locCount != 0){
                sym.Address = locCount;
                if (PushLeaf(sym)) {
                    fclose(fp);
                    printf("\nERROR %2d: DUPLICATE SYMBOL \"%s\"\n", lCount, sym.Name);
                    return 0;
                }
#if DEBUG
                printf("\nPUSHED LEAF");
#endif
            }
            nextToken = strtok(NULL, " \t\n");
        //get the new value for newsym
        }else {nextToken = strtok(line, " \t\n");}

#if DEBUG
        printf("\nnext token is: %s", nextToken);
#endif
        dirTrack = CmprDir(nextToken); //LEAK AT THIS LINE
        //case if newsym is a directive
        if (dirTrack < 0){ //directive behavior
#if DEBUG
            printf ("\n\"%s\" is a DIRECTIVE", nextToken);
#endif
            switch (dirTrack){
            case -1: //BYTE
                operand = strtok(NULL, "#\n");
                if (operand[0] == 'X'){
                    strtok(operand, "'");
                    argument = strtok(NULL, "'");
#if DEBUG
                    printf("\nHEXADECIMAL CONSTANT: %s", argument);
#endif
                    if (ValHEX(argument)){
                        int j = 0;
                        j += (int) strtol(argument, NULL, 16); //convert char in hex to int
                        if (j > 8388608){
                            printf("\nERROR %2d: HEXADECIMAL CONSTANT OVER INTEGER LIMIT ON LINE\n", lCount);
                            {fclose(fp); return 0;}
                        }
                        int i = 0;
                        while (argument[i] != '\0'){
                            i++;
                        }
                        if (i % 2){
                            i++;
                        }
                        i /= 2;
                        locCount+= i; //increment by the number of bytes required to store constant
                    }else {
                        printf("\nERROR %2d: \"%s\" IS NOT A VALID HEXADECIMAL CONSTANT\n", lCount, argument);
                        fclose(fp);
                        return 0;
                    }
                }else if(operand[0] == 'C'){
                    strtok(operand, "'");
                    argument = strtok(NULL, "'");
                    int i = 0;
#if DEBUG
                        printf("\nCHARACTER CONSTANT: %s", argument);
#endif
                        while (argument[i] != '\0'){
                            i++;
                        }
                        locCount += i; //characters are stored in one byte
                }
                    if (checkOverflow(locCount)) {fclose(fp); ELMSG(lCount, locCount) return 0;}
                    continue;
            case -2: //END
                operand = strtok(NULL, "#\n");
                if (checkOverflow(locCount)) {fclose(fp); ELMSG(lCount, locCount) return 0;}
                continue;
            case -3: //EXPORT
                operand = strtok(NULL, "#\n");
                locCount += 3;
                if (checkOverflow(locCount)) {fclose(fp); ELMSG(lCount, locCount) return 0;}
                continue;
            case -4: //RESB
                operand = strtok(NULL, "#\n");
                locCount += atoi(operand);
                if (checkOverflow(locCount)) {fclose(fp); ELMSG(lCount, locCount) return 0;}
                continue;
            case -5: //RESR
                operand = strtok(NULL, "#\n");
                locCount += 3;
                if (checkOverflow(locCount)) {fclose(fp); ELMSG(lCount, locCount) return 0;}
                continue;
            case -6: //RESW
                operand = strtok(NULL, "#\n");
                locCount += atoi(operand) * 3;
                if (checkOverflow(locCount)) {fclose(fp); ELMSG(lCount, locCount) return 0;}
                continue;
            case -7: //START
                operand = strtok(NULL, "#\n");
                locCount = (int) strtol(operand, NULL, 16); //convert char in hex to int
#if DEBUG
                printf("\n%x\n", locCount);
#endif
                if (checkOverflow(locCount)) {fclose(fp); ELMSG(lCount, locCount) return 0;}
                sym.Address = locCount;
                PushLeaf(sym);
                break;
            case -8: //WORD
                operand = strtok(NULL, "#\n");
                int i = atoi(operand);
                if (i > 8388608 || i < -8388608 ){
                    printf("\nERROR %2d: INTEGER CONSTANT %d EXCEEDS LIMIT\n", lCount, i);
                    fclose(fp);
                    return 0;
                }
                locCount+= 3;
                if (checkOverflow(locCount)){ ELMSG(lCount, locCount) return 0;}
                continue;
            }
        }
        //case if newsym is an opcode
        else if(FindHash(OpcodeTable, 29, nextToken) != NULL){
#if DEBUG
            printf("\n\"%s\" is an OPCODE", nextToken);
#endif
            operand = strtok(NULL, "#\n");
            locCount += 3;
        }
        else {
            printf("\nERROR %2d: \"%s\" IS NOT A VALID OPCODE\n", lCount, nextToken);
            fclose(fp);
            return 0;
        }
        //max word size is 2^23, check programmer's ref
        //printf("\nLocation is: %x\n", locCount);
    }
    PrintTree();
    printf("\n");
    fclose(fp);
    return 0;
}

//testing mode to skip normal program runtime behavior
int TestMode(){
    /*
     int j = IsAValidSymbol("BYTE");
     printf("\ninput is: %s\nj is: %i", "BYTE",j);
     printf("\nTesting Binary Tree");


     OPCODES opTest;
     strcpy(opTest.Name, "ADD");
     opTest.OpCode = 0x18;
     ReadOpCodeFile();
     int result;
     result = FindHash(OpcodeTable, 29, &opTest);
     printf("\nFindHash Result: %d", result);
     */
     return 0;
}
