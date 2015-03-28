/*
	Useful constants

$Header: StdHdr.h,v 2.5 86/09/16 11:48:07 miw Exp $
$Log:	StdHdr.h,v $
 * Revision 2.5  86/09/16  11:48:07  miw
 * Added support for SUN.
 * 
 * Revision 2.4  86/01/23  22:01:07  miw
 * Forced SINGLE_STEP for VAX.
 * 
 * Revision 2.3  86/01/17  21:22:38  miw
 * Customised ALIGN for VAX and ORION.  Added void to PERQ version.
 * 
 * Revision 2.2  86/01/17  18:51:24  miw
 * Removed support for APOLLO, added special cases for PERQ1 and 2.
 * 
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


#ifdef PERQ
#define void int
#define ALIGN   4       /* PNX alignment (words) for bitmaps (must be a power of 2) */
#endif
#ifdef ORION
#define rectangle malloc
#define ALIGN 2
#endif
#ifdef VAX
#define rectangle malloc
#define ALIGN 2
#define SINGLE_STEP
#endif
#ifdef SUN
#define rectangle malloc
#define ALIGN 1
#endif

#define BITS    (sizeof(WORD)*ALIGN*8)
