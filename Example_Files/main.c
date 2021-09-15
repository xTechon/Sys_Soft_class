#include "headers.h"


int main( int argc, char* argv[]){

	FILE *fp;
	char line[1024];
	char* newsym;
	char* nextoken;

	char fullline[1024];

	if ( argc != 2 ) {
	printf("ERROR: Usage: %s filename\n", argv[0]);
	return 0;
	}


	fp = fopen( argv[1], "r");

	if ( fp == NULL ) {
	printf("ERROR: %s could not be opened for reading,\n", argv[1] );
	return 0;
	}

	newsym = malloc(  1024 * sizeof(char)             );	
	memset( newsym, '\0', 1024 * sizeof(char) );
	nextoken = malloc(  1024 * sizeof(char)             );	
	memset( nextoken, '\0', 1024 * sizeof(char) );
	while(  fgets( line , 1024 , fp ) != NULL   ) {

		strcpy( fullline, line );
		if ( line[0] == 35) {  
			printf("COMMENT:%s", line );

			continue;
		}	

		if (  (line[0] >= 65 ) && ( line[0] <= 90 )   )  {
			newsym = strtok( line, " \t\n");
			printf("FULL LINE:%s\n", fullline );
			printf("NEW SYMBOL : %s\n",newsym);
			printf("Is a valid symbol is %d\n", IsAValidSymbol( newsym ) );


			nextoken = strtok( NULL, " \t\n"  );
			printf("NEXT TOKEN ON LINE IS %s\n", nextoken );

			if ( IsAValidSymbol(newsym) == 0 ) {

				printf("ERROR. INVALID SYMBOL\n");
				fclose(fp);
				return 0;
			}
			continue;
		}
		


		printf("%s", line );


	}




	fclose( fp );


}
