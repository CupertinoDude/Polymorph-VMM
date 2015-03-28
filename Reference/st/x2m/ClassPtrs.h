/*
	Class Pointers

$Header: ClassPtrs.h,v 2.1 85/11/19 17:38:15 miw Exp $
$Log:	ClassPtrs.h,v $
 * Revision 2.1  85/11/19  17:38:15  miw
 * Stabilised with all other modules at Rev 2.1
 * 
 * Revision 1.1  85/01/10  12:52:07  miw
 * Initial revision
 * 

*/

					/*ST!Interpreter!initializeGuaranteedPointers!*/

#define CLASS_SMALL_INTEGER     12      /* p. 687 */
#define CLASS_STRING            14
#define CLASS_ARRAY             16
#define CLASS_FLOAT             20      /* not defined in G/R */
#define CLASS_METH_CTX          22
#define CLASS_BLOCK_CTX         24
#define CLASS_POINT             26
#define CLASS_LG_POS_INT        28
#define CLASS_DISPLAY_BITMAP    30
#define CLASS_MSG               32
#define CLASS_COMPILED_METHOD   34      /* p. 686 */
#define CLASS_SEMA              38      /* not defined in G/R */
#define CLASS_CHARACTER         40
#define CLASS_FORM              0xC52   /* required for BitBlt */


					/*ST!Interpreter!initializePointIndices!*/
/* Point info */
#define PT_SIZE         2       /* the size of an instance of class Point */
#define XINDEX          0
#define YINDEX          1
