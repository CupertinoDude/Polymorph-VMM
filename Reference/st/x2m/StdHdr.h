/*
	Useful constants

$Header: StdHdr.h,v 2.1 85/11/19 17:42:41 miw Exp $
$Log:	StdHdr.h,v $
 * Revision 2.1  85/11/19  17:42:41  miw
 * Stabilised with all other modules at Rev 2.1
 * 
 * Revision 1.3  85/07/05  13:23:52  miw
 * Removed the BC_TRACE and MSG_TRACE options.
 * 
 * Revision 1.2  85/06/27  19:09:33  miw
 * Added conditional definition of NULL.
 * 
 * Revision 1.1  85/01/10  12:57:28  miw
 * Initial revision
 * 

*/

#define FALSE   (0)
#define TRUE    (!FALSE)

#ifndef NULL
#define NULL    0
#endif

typedef unsigned BOOL;


#define ALIGN   4       /* PNX alignment (words) for bitmaps (must be a power of 2) */
#define BITS    (sizeof(WORD)*ALIGN*8)

/* make the following VAX or PERQ or APOLLO */
#define VAX
