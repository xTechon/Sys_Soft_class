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
//    printf("File Opened successfully.");
//    int j = IsAValidSymbol("RESWR");
//    printf("\ninput is: %s\nj is: %i", "RESWR",j);

    char line[1024];
    char* newsym;
    newsym = malloc(1024*sizeof(char));
    memset(newsym, '\0', 1024*sizeof(char));

    while (fgets(line, 1024, fp) !=NULL){
        if (line[0] == 35){
            printf("Comment: %s", line); //remove or comment out before submission

            continue;
        }
        if ( (line[0] >= 65) && (line[0] <= 90)){
            newsym = strtok(line, " \t\n");
            printf("NEW SYMBOL ON LINE: %s\n", line);
            printf("NEW SYMBOL IS: %s\n", newsym);
            if (IsAValidSymbol == 0){
                printf("ERROR. INVALID SYMBOL\n");
                fclose(fp);
                return 0;
            }
            continue;
        }
    }
    fclose(fp);
    return 0;
}
