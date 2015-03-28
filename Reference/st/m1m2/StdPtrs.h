/* 
	standard pointers of the system 

$Header: StdPtrs.h,v 2.3 85/12/07 15:38:36 miw Exp $
$Log:	StdPtrs.h,v $
 * Revision 2.3  85/12/07  15:38:36  miw
 * Halved the values of the oops to take into account the change in position of
 * the tag bit, and altered the values of the SmallInteger pointers as well.
 * Moved SMALLTALK from OM.c to here.
 * 
 * Revision 2.2  85/12/04  20:26:58  miw
 * Added UNUSED_PTR.
 * 
 * Revision 2.1  85/11/19  17:42:46  miw
 * Stabilised with all other modules at Rev 2.1
 * 
 * Revision 1.1  85/01/10  12:57:40  miw
 * Initial revision
 * 

*/
					/*ST!Interpreter!initializeSmallIntegers!*/
#define MINUS_ONE_PTR   0xFFFF
#define ZERO_PTR        0x8000
#define ONE_PTR         0x8001
#define TWO_PTR         0x8002
#define NEW_MINUS_ONE_PTR   0xFFFFFFFF
#define NEW_ZERO_PTR        0x80000000
#define NEW_ONE_PTR         0x80000001
#define NEW_TWO_PTR         0x80000002

					/*ST!Interpreter!initializeGuaranteedPointers!*/
#define UNUSED_PTR	0
#define NIL_PTR         1
#define FALSE_PTR       2
#define TRUE_PTR        3

#define SCHED_ASS_PTR           4

#define SPECIAL_SELECTORS       24
#define CHAR_TABLE              25
#define SMALLTALK       	9      /* the system dictionary, not defd in G&R */
