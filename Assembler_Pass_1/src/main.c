#include "headers.h"

int TestMode();
int checkOverflow(int count);
int main(int argc, char *argv[]){
    //TestMode();return 0; //uncomment for testing mode
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
    printf("\nFile Opened successfully.");

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

    int lCount = 0;
    int locCount = 0; //location in BYTES not words
    SYMBOL sym;
    int dirTrack = 0;
    //int hexCounter = 0;
    while (fgets(line, 1024, fp) !=NULL){
        strcpy (fline, line);
        lCount++; //Keep track of lines
        if (line[0] == 35){
            //printf("Comment: %s", line); //remove or comment out before submission
            continue;
        }
        if (line[0] == '\n'){ //quit if it finds a blank line
            printf("ERROR: FILE HAS BLANK LINES");
            return 0;
        }
        if ((line[0] >= 65) && (line[0] <= 90)){ //check for symbols that start with capitals
            newsym = strtok(line, " \t\n");
            printf("\nNEW SYMBOL ON LINE: %d", lCount);
            printf("\nNEW SYMBOL IS: %s", newsym);
            errC = IsAValidSymbol(newsym);
            if (errC != 1){
                printf("\nERROR. INVALID SYMBOL \"%s\" WITH CODE: %d", newsym, errC);
                fclose(fp);
                return 0;
            }
            //set some of the values of the new Symbol
            strcpy(sym.Name, newsym);
            sym.SourceLine = lCount;
            if (locCount != 0){
                sym.Address = locCount;
                if (!PushLeaf(sym)) return 0;
            }

        }

        //get the new value for newsym
        nextToken = strtok(NULL, " \t\n");
        printf("\nnext token is: %s", nextToken);
        dirTrack = CmprDir(nextToken);
        //case if newsym is a directive
        if (dirTrack < 0){ //directive behavior
            printf ("\n\"%s\" is a DIRECTIVE", nextToken);
            switch (dirTrack){
                case -1: //BYTE
                    operand = strtok(NULL, "#\n");
                    if (operand[0] == 'X'){
                        argument = strtok(operand, "'");
                        int j = 0;
                        j += (int) strtol(argument, NULL, 16); //convert char in hex to int
                        if (j > 8388608){
                            printf("\nERROR: HEXADECIMAL CONSTANT OVER INTEGER LIMIT ON LINE %d", lCount);
                            return 0;
                        }
                        locCount+= 3; //increment by the word size
                    }else if(operand[0] == 'C'){
                        argument = strtok(operand, "'");
                        int i = 0;
                        while (argument[i] != '\0'){
                            i++;
                        }
                        locCount += i; //characters are stored in one byte
                    }
                    if (checkOverflow(locCount)) return 0;
                    continue;
                case -2: //END
                    operand = strtok(NULL, "#\n");
                    if (checkOverflow(locCount)) return 0;
                    continue;
                case -3: //EXPORT
                    operand = strtok(NULL, "#\n");
                    locCount += 3;
                    if (checkOverflow(locCount)) return 0;
                    continue;
                case -4: //RESB
                    operand = strtok(NULL, "#\n");
                    locCount += atoi(operand);
                    if (checkOverflow(locCount)) return 0;
                    continue;
                case -5: //RESR
                    operand = strtok(NULL, "#\n");
                    locCount += 3;
                    if (checkOverflow(locCount)) return 0;
                    continue;
                case -6: //RESW
                    operand = strtok(NULL, "#\n");
                    locCount += atoi(operand) * 3;
                    if (checkOverflow(locCount)) return 0;
                    continue;
                case -7: //START
                    operand = strtok(NULL, "#\n");
                    locCount = (int) strtol(operand, NULL, 16); //convert char in hex to int
                    if (checkOverflow(locCount)) return 0;
                    sym.Address = locCount;
                    PushLeaf(sym);
                    break;
                case -8: //WORD
                    operand = strtok(NULL, "#\n");
                    locCount+= 3;
                    if (checkOverflow(locCount)) return 0;
                    continue;
            }
        }
        //case if newsym is an opcode
        else if(FindHash(OpcodeTable, 29, nextToken) != NULL){
            printf("\n\"%s\" is an OPCODE", nextToken);
            operand = strtok(NULL, "#\n");
            locCount += 3;
        }
        else {
            printf("\nERROR: \"%s\" ON LINE %d IS NOT A VALID DIRECTIVE OR OPCODE", nextToken, lCount);
        }
        nextToken = strtok(NULL, " \t\n");
        printf("\nnext token is: %s", nextToken);
        //max word size is 2^23, check programmer's ref

    }
    PrintTree();
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
int checkOverflow(int count){
    if (count > 32768){
        printf("\nERROR: PROGRAM STARTS AT LOCATION %x NO ROOM LEFT IN SIC MEMORY", count);
        return 1;
    }
    return 0;
}
