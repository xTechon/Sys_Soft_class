#include "headers.h"


int IsAnInstruction( char *Test ){

return		( 	! ( 
		 strcmp( Test, "LDA" )  &&
		 strcmp( Test, "STCH" )   

		 ) ) ;
}
