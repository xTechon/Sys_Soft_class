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
            //printf("\nNEW SYMBOL ON LINE: %d", lCount);
            //printf("\nNEW SYMBOL IS: %s", newsym);
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
                if (!PushLeaf(sym)) {fclose(fp); return 0;}
                //printf("\nPUSHED LEAF");
            }
            nextToken = strtok(NULL, " \t\n");
        //get the new value for newsym
        }else {nextToken = strtok(line, " \t\n");}

        //printf("\nnext token is: %s", nextToken);
        dirTrack = CmprDir(nextToken); //LEAK AT THIS LINE
        //case if newsym is a directive
        if (dirTrack < 0){ //directive behavior
            //printf ("\n\"%s\" is a DIRECTIVE", nextToken);
            switch (dirTrack){
                case -1: //BYTE
                    operand = strtok(NULL, "#\n");
                    if (operand[0] == 'X'){
                        strtok(operand, "'");
                        argument = strtok(NULL, "'");
                        //printf("\nHEXADECIMAL CONSTANT: %s", argument);
                        if (ValHEX(argument)){
                            int j = 0;
                            j += (int) strtol(argument, NULL, 16); //convert char in hex to int
                            if (j > 8388608){
                                printf("\nERROR: HEXADECIMAL CONSTANT OVER INTEGER LIMIT ON LINE %d", lCount);
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
                            printf("\nERROR: \"%s\" ON LINE %d IS NOT A VALID HEXADECIMAL CONSTANT", argument, lCount);
                            fclose(fp);
                            return 0;
                        }
                    }else if(operand[0] == 'C'){
                        strtok(operand, "'");
                        argument = strtok(NULL, "'");
                        int i = 0;
                        //printf("\nCHARACTER CONSTANT: %s", argument);
                        while (argument[i] != '\0'){
                            i++;
                        }
                        locCount += i; //characters are stored in one byte
                    }
                    if (checkOverflow(locCount)) {fclose(fp); return 0;}
                    continue;
                case -2: //END
                    operand = strtok(NULL, "#\n");
                    if (checkOverflow(locCount)) {fclose(fp); return 0;}
                    continue;
                case -3: //EXPORT
                    operand = strtok(NULL, "#\n");
                    locCount += 3;
                    if (checkOverflow(locCount)) {fclose(fp); return 0;}
                    continue;
                case -4: //RESB
                    operand = strtok(NULL, "#\n");
                    locCount += atoi(operand);
                    if (checkOverflow(locCount)) {fclose(fp); return 0;}
                    continue;
                case -5: //RESR
                    operand = strtok(NULL, "#\n");
                    locCount += 3;
                    if (checkOverflow(locCount)) {fclose(fp); return 0;}
                    continue;
                case -6: //RESW
                    operand = strtok(NULL, "#\n");
                    locCount += atoi(operand) * 3;
                    if (checkOverflow(locCount)) {fclose(fp); return 0;}
                    continue;
                case -7: //START
                    operand = strtok(NULL, "#\n");
                    locCount = (int) strtol(operand, NULL, 16); //convert char in hex to int
                    //printf("\n%x\n", locCount);
                    if (checkOverflow(locCount)) {fclose(fp); return 0;}
                    sym.Address = locCount;
                    PushLeaf(sym);
                    break;
                case -8: //WORD
                    operand = strtok(NULL, "#\n");
                    int i = atoi(operand);
                    if (i > 8388608 || i < -8388608 ){
                       printf("\nERROR: INTEGER CONSTANT %d EXCEEDS LIMIT", i);
                       fclose(fp);
                       return 0;
                    }
                    locCount+= 3;
                    if (checkOverflow(locCount)) return 0;
                    continue;
            }
        }
        //case if newsym is an opcode
        else if(FindHash(OpcodeTable, 29, nextToken) != NULL){
            //printf("\n\"%s\" is an OPCODE", nextToken);
            operand = strtok(NULL, "#\n");
            locCount += 3;
        }
        else {
            printf("\nERROR: \"%s\" ON LINE %d IS NOT A VALID DIRECTIVE OR OPCODE", nextToken, lCount);
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
