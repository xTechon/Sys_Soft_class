#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct symbols {
	int	DefinedOnSourceLine;
	int	Address; 
	char	Name[7]; };

typedef struct symbols	SYMBOL;

struct opcodes {
	char	OpCode;	
	char	Name[8];

};

typedef struct opcodes OPCODES;


OPCODES OpcodeTable[ 32 ];

int IsAValidSymbol( char *TestSymbol );
int IsADirective( char *Test );
