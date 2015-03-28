/* 
	standard pointers of the system 

$Header: StdPtrs.h,v 2.1 85/11/19 17:42:46 miw Exp $
$Log:	StdPtrs.h,v $
 * Revision 2.1  85/11/19  17:42:46  miw
 * Stabilised with all other modules at Rev 2.1
 * 
 * Revision 1.1  85/01/10  12:57:40  miw
 * Initial revision
 * 

*/
					/*ST!Interpreter!initializeSmallIntegers!*/
#define MINUS_ONE_PTR   65535
#define ZERO_PTR        1
#define ONE_PTR         3
#define TWO_PTR         5

					/*ST!Interpreter!initializeGuaranteedPointers!*/
#define NIL_PTR         2
#define FALSE_PTR       4
#define TRUE_PTR        6

#define SCHED_ASS_PTR           8

#define SPECIAL_SELECTORS       48
#define CHAR_TABLE              50
