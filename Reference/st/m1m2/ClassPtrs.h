/*
	Class Pointers

$Header: ClassPtrs.h,v 2.2 85/12/07 15:37:00 miw Exp $
$Log:	ClassPtrs.h,v $
 * Revision 2.2  85/12/07  15:37:00  miw
 * Halved the values of all the oops, because the tag bit has been moved
 * from lsb to msb.
 * 
 * Revision 2.1  85/11/19  17:38:15  miw
 * Stabilised with all other modules at Rev 2.1
 * 
 * Revision 1.1  85/01/10  12:52:07  miw
 * Initial revision
 * 

*/

					/*ST!Interpreter!initializeGuaranteedPointers!*/

#define CLASS_SMALL_INTEGER     6      /* p. 687 */
#define CLASS_STRING            7
#define CLASS_ARRAY             8
#define CLASS_FLOAT             10      /* not defined in G/R */
#define CLASS_METH_CTX          11
#define CLASS_BLOCK_CTX         12
#define CLASS_POINT             13
#define CLASS_LG_POS_INT        14
#define CLASS_DISPLAY_BITMAP    15
#define CLASS_MSG               16
#define CLASS_COMPILED_METHOD   17      /* p. 686 */
#define CLASS_SEMA              19      /* not defined in G/R */
#define CLASS_CHARACTER         20
#define CLASS_FORM              0x629   /* required for BitBlt */


					/*ST!Interpreter!initializePointIndices!*/
/* Point info */
#define PT_SIZE         2       /* the size of an instance of class Point */
#define XINDEX          0
#define YINDEX          1
