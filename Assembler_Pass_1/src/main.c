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
//    printf("File Opened successfully.");


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
            printf("NEW SYMBOL ON LINE: %d\n", lCount);
            printf("NEW SYMBOL IS: %s\n", newsym);
            errC = IsAValidSymbol(newsym);
            if (errC != 1){
                printf("ERROR. INVALID SYMBOL \"%s\" WITH CODE: %d\n", newsym, errC);
                fclose(fp);
                return 0;
            }
            nextToken = strtok(NULL, " \t\n");
            continue;
        }

    }
    fclose(fp);
    return 0;
}

//testing mode to skip normal program runtime behavior
int TestMode(){
     int j = IsAValidSymbol("BYTE");
     printf("\ninput is: %s\nj is: %i", "BYTE",j);
     return 0;
}
