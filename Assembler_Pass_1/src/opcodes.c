#include "headers.h"

//OPLISTHEAD* OpcodeTable;

int ReadOpCodeFile(){
    printf("\nLooking for opcode file...");
    FILE *fd = NULL;
    printf("\right before fopen");
    fd = fopen("SIC_Instructions.csv", "r");
    printf("\nWent through fopen");

    if (fd == NULL){
        printf("ERROR: OPCODE CSV COULD NOT BE FOUND");
        return 0;
    }

    printf("\nImporting OpCodes...");
    OPLISTHEAD* OpcodeTable = malloc(29*sizeof(OPLISTHEAD));
    char line [1024];
    fgets(line, 1024, fd); //skip headers
    char* opName;
    opName = malloc(5*sizeof(char));
    memset(opName, '\0', 5*sizeof(char));
    char* opChar;
    opChar = malloc(3*sizeof(char));
    memset(opChar, '\0', 5*sizeof(char));
    int opCode;
    OPCODES* op;
    op = (OPCODES*) malloc(sizeof(OPCODES));
    while(fgets(line, 1024, fd) != NULL){
        opName = strtok(line, ",");
        strtok(NULL, ",");
        strtok(NULL, ",");
        opChar = strtok(NULL, ",");
        printf("\nopChar is: %s", opChar);
        opCode = (int) strtol(opChar, NULL, 16);
        //store the data from the file into an opcode link
        strcpy(op->Name,opName);
        op->OpCode = opCode;
        printf("\nOPCODE is: %x", opCode);
        //push the data into the hash
        PushHash(OpcodeTable, 29, op);
    }
    //confirm opcodes
    PrintHash(OpcodeTable, 29);
    fclose(fd);
    return 1;
}
