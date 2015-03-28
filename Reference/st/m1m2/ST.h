/* 
	types and constants of use in the Smalltalk Virtual Machine

$Header: ST.h,v 2.1 85/11/19 17:42:17 miw Exp $
$Log:	ST.h,v $
 * Revision 2.1  85/11/19  17:42:17  miw
 * Stabilised with all other modules at Rev 2.1
 * 
 * Revision 1.4  85/07/23  21:00:09  miw
 * Changed ADDR to be a direct pointer into object memory.
 * 
 * Revision 1.3  85/06/27  19:07:07  miw
 * Added definition of ADDR (previously in OMMacros.h).
 * 
 * Revision 1.2  85/01/14  17:58:27  miw
 * Added max and MaxBec macros
 * 
 * Revision 1.1  85/01/10  12:56:23  miw
 * Initial revision
 * 
*/

typedef unsigned short WORD;
typedef unsigned NEWWORD;

typedef WORD OOP;
typedef NEWWORD NEWOOP;

typedef short SIGNED;
typedef int NEWSIGNED;

#define MAX_INT         (16*1024 - 1)
#define MIN_INT         (-16*1024)
#define NEW_MAX_INT         (0x3FFFFFFF)
#define NEW_MIN_INT         (0xC0000000)

typedef unsigned char BYTE;

typedef unsigned TIME;

typedef WORD *ADDR;                  /* index into object memory */

/* Parameterisation for 16-bit (object memory) words. */

#define BPW     (sizeof(WORD)*8)        /* bits per word */
#define L2BPW   4                       /* log (base 2) BPW */
#define ONES    0xFFFF
#define NEWBPW     (sizeof(NEWWORD)*8)        /* bits per word */
#define NEWL2BPW   5                       /* log (base 2) BPW */
#define NEWONES    0xFFFFFFFF

/* this is a useful macro for assigning a minimum value.
   MinBec(x,y) is the same as if(x>y)x=y but more efficient
   as y is only evaluated once.
*/

#define MinBec(x, y)    { int t = (y); if (x > t) x = t; }

#define min(a, b)       (a < b ? a : b)

#define MaxBec(x, y)    { int t = (y); if (x < t) x = t; }

#define max(a, b)       (a > b ? a : b)
