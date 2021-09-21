#include "headers.h"

int TestMode();
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
    int lCount = 0;

    while (fgets(line, 1024, fp) !=NULL){
        strcpy (fline, line);
        lCount++; //Keep track of lines
        if (line[0] == 35){
            //printf("Comment: %s", line); //remove or comment out before submission
            continue;
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
            nextToken = strtok(NULL, " \t\n");
            continue;
        }

        //max word size is 2^23, check programmer's ref

    }
    fclose(fp);
    return 0;
}

//testing mode to skip normal program runtime behavior
int TestMode(){
    /*
     int j = IsAValidSymbol("BYTE");
     printf("\ninput is: %s\nj is: %i", "BYTE",j);
     printf("\nTesting Binary Tree");
    */

     OPCODES opTest;
     strcpy(opTest.Name, "ADD");
     opTest.OpCode = 0x18;
     ReadOpCodeFile();
     int result;
     result = FindHash(OpcodeTable, 29, &opTest);
     printf("\nFindHash Result: %d", result);
     return 0;
}
