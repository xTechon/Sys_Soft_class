#include "headers.h"


int main(int argc, char *argv[]){
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
    printf("File Opened successfully.");
    int j = IsAValidSymbol("RESWR");
    printf("\ninput is: %s\nj is: %i", "RESWR",j);
    fclose(fp);
    return 0;
}
