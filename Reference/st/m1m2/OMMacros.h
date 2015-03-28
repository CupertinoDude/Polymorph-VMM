/*

	Macros for access to the Object Memory

Abbreviations used:
	HC      heap chunk
	FCL     free chunk list
	oop     object pointer(?)
	ptr     pointer
	hdr,HDR header
	W       word
	B       byte

$Header: OMMacros.h,v 2.9 87/02/07 17:41:03 miw Exp $
$Log:	OMMacros.h,v $
 * Revision 2.9  87/02/07  17:41:03  miw
 * Changed call to free to free_bitmap (for SUN).
 * 
 * Revision 2.8  86/01/17  21:18:27  miw
 * Parameterised the display bitmap for all machines (added SCREEN).
 * 
 * Revision 2.7  86/01/17  18:47:35  miw
 * Added support for the D-B garbage collector:
 * - macros to access the ZCT
 * - more non-ref. counting versions of storePtr.
 * - moved hasObject() into OM.c
 * - made c_down() faster.
 * Also added collective access to all the attribute bits in the object table.
 * Split transfer into two specialised cases: NRtransfer, and transferAndPop.
 * 
 * Revision 2.6  85/12/12  10:29:29  miw
 * Fixed ref. counting bug (was ref.counting exactly those objects that weren't
 * supposed to be!)  Added the addrOfOop() macro.
 * 
 * Revision 2.5  85/12/07  15:35:08  miw
 * Made the object pointer SmallInteger tag bit the most sig. bit.
 * Consequently altered the macros which depended on the position of this
 * bit, and improved reference counting by not counting changes to objects
 * 0-3 (unused, nil, false, true).
 * 
 * Revision 2.4  85/12/06  18:25:21  miw
 * Removed the alternative HCbyte() for machines with the bytes the
 * ``wrong way round.''  Now assumes that this is fixed when the image is
 * converted to Manchester format.
 * 
 * Revision 2.3  85/12/04  20:23:56  miw
 * Removed the HDR_SIZE constant, and all references to the display object.
 * Added the bitmap bit to the object table, and macros to access and change
 * it.  Extended the deallocate() macro to cope with cached bitmaps.
 * Tidied up some of the declarations (OOPs declared as WORDs).
 * 
 * Revision 2.2  85/11/23  17:02:05  miw
 * Altered size and class macros to access separate arrays, and made
 * HDR_SIZE equal to 0.
 * 
 * Revision 2.1  85/11/19  17:41:27  miw
 * Stabilised with all other modules at Rev 2.1
 * 
 * Revision 1.9  85/11/18  20:01:12  miw
 * Fixed type error in c_up (altered from WORD to BYTE).
 * 
 * Revision 1.8  85/08/07  08:29:53  miw
 * Added loctni() macro.
 * 
 * Revision 1.7  85/07/23  21:01:05  miw
 * Changed c_down so that only the zero trap requires a function call.
 * Made the loctn field of the object table a direct pointer into om[].
 * 
 * Revision 1.6  85/07/05  13:06:36  miw
 * Altered the ref.counting scheme to place the overflow bit in the l.s.b.
 * Added the NRstorePtr() macro.
 * 
 * Revision 1.5  85/06/27  19:05:16  miw
 * Split the object table into two arrays, and altered the relevant
 * macros.
 * 
 * Revision 1.4  85/04/02  11:29:10  miw
 * Added register declaration to for_every_oop.
 * Removed all references to the ``extra word'' in large objects.
 * 
 * Revision 1.3  85/03/14  19:36:56  miw
 * fixed sign extension in isIntVal.  Expanded object memory size.
 * 
 * Revision 1.2  85/03/06  15:25:42  miw
 * Changed HCbyte macro for Apollo (has the correct byte-sex).
 * 
 * Revision 1.1  85/01/10  12:55:09  miw
 * Initial revision
 * 
*/

#define BIG_SIZE                40              /* largest index into free chunk list heads */

#define NON_PTR         ONES            /* something that can't be an index into the
					   object table */

#ifdef PERQ
#	define SCREEN		0		/* ptr to screen window */
#else
#	define SCREEN theScreen
	extern char *theScreen;
#endif

#define OT_SIZE         (32*1024)          /* size of object table */

#define OM_SIZE                 (400*1024)        /* size of object memory, in WORDs!! */

#define HUGE_SIZE       256             /* G/R p.661 */

	/* the SmallInteger tag bit is now the most significant */
#define FIRST_OOP       0
#define LAST_OOP        (OT_SIZE-1)

			/* don't forget the extra } */
#define for_every_oop(x)        { register WORD x;                          	\
				  for (x = FIRST_OOP; x <= LAST_OOP; ++x)

/* assume that ObjectTableStart = 0 (G/R p.661) */
/* note that we don't divide the object memory into segments */
					/*ST!ObjectMemory!segmentBitsOf:!*/
					/*ST!ObjectMemory!segmentBitsOf:put:!*/

					/*ST!ObjectMemory!locationBitsOf:!*/
					/*ST!ObjectMemory!locationBitsOf:put:!*/
#define loctn(oop)      (otLoc[(oop)].addr)

	/* get the absolute address of the first word/oop in an object */
#define addrOfOop(oop)	loctn(oop)

/* the following is used when accessing the free chain within otLoc[] */
#define loctni(oop)     (otLoc[(oop)].index)


					/*ST!ObjectMemory!countBitsOf:!*/
					/*ST!ObjectMemory!countBitsOf:put:!*/
#define count(oop)      (otCount[(oop)])

	/* access all the object table bits at once */
#define attribs(oop)			(otRest[(oop)])
#define otAttribsPut(oop, att)		{ attribs(oop) = (att); }

					/* odd() returns 0 or ODD */
#define ODD     		1
					/*ST!ObjectMemory!oddBitOf:!*/
#define odd(oop)		(otRest[(oop)]&ODD)
					/*ST!ObjectMemory!oddBitOf:put:!*/
#define oddPut(oop, isOdd)      { if (isOdd) otRest[(oop)] |= ODD; else otRest[(oop)] &= ~ODD; }

					/* ptrs() returns 0 or PTRS */
#define PTRS    2
					/*ST!ObjectMemory!pointerBitOf:!*/
#define ptrs(oop)               (otRest[(oop)]&PTRS)
					/*ST!ObjectMemory!pointerBitOf:put:!*/
#define ptrsPut(oop, isPtrs)     { if (isPtrs) otRest[(oop)] |= PTRS; else otRest[(oop)] &= ~PTRS; }

				/* isFree() returns 0 or FREE */
#define FREE    4
					/*ST!ObjectMemory!freeBitOf:!*/
#define isFree(oop)               (otRest[(oop)]&FREE)
					/*ST!ObjectMemory!freeBitOf:put:!*/
#define freePut(oop, isFree)    { if (isFree) otRest[(oop)] |= FREE; else otRest[(oop)] &= ~FREE; }


#define BITMAP	8
#define bitmap(oop)			(otRest[(oop)]&BITMAP)
#define bitmapPut(oop, isBitmap)	{ if (isBitmap) otRest[(oop)] |= BITMAP; 	\
					  else otRest[(oop)] &= ~BITMAP; }

#define INZCT	16
#define inZCT(oop)			(otRest[(oop)]&INZCT)
#define inZCTput(oop, isInZCT)		{ if (isInZCT) otRest[(oop)] |= INZCT; 		\
					  else otRest[(oop)] &= ~INZCT; }

extern WORD *ConvBitMap();
	/* is oop cached as a bitmap? If not, cache it.  Then return the address of the bitmap */
#define BitMapOf(oop, w, h, is_ht)	(bitmap(oop) ? loctn(oop) : ConvBitMap(oop, w, h, is_ht))


						/*ST!ObjectMemory!heapChunkOf:word:!*/
#define HCword(oop, offset)             (loctn(oop)[offset])
						/*ST!ObjectMemory!heapChunkOf:word:put:!*/
#define HCwordPut(oop, offset, val)     { HCword(oop, offset) = (val); }

/* the order of bytes within words is fixed when an image is converted (if necessary) */
						/*ST!ObjectMemory!heapChunkOf:byte!*/
#define HCbyte(oop, boffset)            (((BYTE *)loctn(oop))[boffset])

						/*ST!ObjectMemory!heapChunkOf:byte:put:!*/
#define HCbytePut(oop, boffset, bval)   { HCbyte(oop, boffset) = (bval); }

						/*ST!ObjectMemory!sizeBitsOf:!*/
						/*ST!ObjectMemory!sizeBitsOf:put:!*/
#define size(oop)                       (Sizes[(oop)])

						/*ST!ObjectMemory!classBitsOf:!*/
						/*ST!ObjectMemory!classBitsOf:put:!*/
#define class(oop)                      (Classes[(oop)])

						/*ST!ObjectMemory!integerValueOf:!*/
/* the following is NON-PORTABLE - assumes -1>>1 == -1 */
#define intVal(oop)                     (((SIGNED)((oop)<<1))>>1)

						/*ST!ObjectMemory!integerObjectOf:!*/
#define intObj(val)                     ((val)|0x8000)

						/*ST!ObjectMemory!isIntegerObject:!*/
#define isInt(oop)                      ((oop)&0x8000)

						/*ST!ObjectMemory!isIntegerValue:!*/
/* BEWARE of side effects when using this macro */
/* Note that for large systems we have to coerce val to int */
#define isIntVal(val)                   (((int)val) <= MAX_INT && ((int)val) >= MIN_INT)

#define intToObj(i)                     ((i) & 0x7FFF)
#define objToInt(o)                     ((o) | 0x8000)


	/* is oop reference counted ?
	   object 0 is reserved, 1 is NIL, etc...see the Virtual Image book */
#define LAST_REF_COUNTED_OOP	26	/* MUST_BE_BOOLEAN */
#define counted(oop)		((SIGNED)(oop) > LAST_REF_COUNTED_OOP)

						/*ST!ObjectMemory!countUp:!*/
						/*ST!ObjectMemory!increaseReferencesTo:!*/
/* this is the refce. count decrementer without checks for an Integer Oop */
#define SATURATED			1
#define c_up(oop)                       { BYTE cnt = count(oop) + 2;                    \
					  count(oop) =  cnt==0 ? SATURATED : cnt; }

#define countUp(oop)                    { OOP oop1 = (oop); if (counted(oop1)) c_up(oop1); }

#define MAXZCT				256

#define addToZCT(oop)			{ if (!inZCT(oop)) {				\
						ZCT[ZCTindex++] = (oop);		\
						inZCTput(oop, TRUE);			\
						if (ZCTindex == MAXZCT)			\
							endTransaction();		\
					}}

						/*ST!ObjectMemory!decreaseReferencesTo:!*/
						/*ST!ObjectMemory!countDown:!*/
#define countDown(oop)                  { OOP oop2 = (oop); if (counted(oop2)) c_down(oop2); }

#define c_down(oop)                     { if ((count(oop) -= 2) == 0)			\
						addToZCT(oop);               		\
					}

						/*ST!ObjectMemory!fetchPointer:ofObject:!*/
#define fetchPtr(i, oop)                HCword(oop, i)

						/*ST!ObjectMemory!storePointer:ofObject:withValue:!*/
/* DON'T alter the order of assignments in the initialiser */
#define storePtr(i, oop, val)           { OOP  oop3 = (oop),  val1 = (val);             \
					  WORD chunkInd = (i);                          \
					  countUp(val1);                                \
					  countDown(HCword(oop3, chunkInd));            \
					  HCwordPut(oop3, chunkInd, val1);              \
					}

	/* special versions for objects that have direct pointers */
#define ABSfetchPtr(i,oop)		(oop[i])
	/* BEWARE of side effects on arg 2 */
#define ABStorePtr(i, oop, val)         { OOP  val1 = (val);             		\
					  WORD chunkInd = (i);                          \
					  countUp(val1);                                \
					  countDown(oop[chunkInd]);            		\
					  oop[chunkInd] = val1;              		\
					}

	/* non-ref.counting versions */
#define NRstorePtr(i, oop, val)         { OOP  oop3 = (oop),  val1 = (val);             \
					  WORD chunkInd = (i);                	        \
					  countDown(HCword(oop3, chunkInd));            \
					  HCwordPut(oop3, chunkInd, val1);              \
					}
#define NR2storePtr(i, oop, val)	HCwordPut(oop, i, val);

						/*ST!ObjectMemory!fetchWord:ofObject:!*/
#define fetchWord(i, oop)               HCword(oop, i)

						/*ST!ObjectMemory!storeWord:ofObject:withValue:!*/
#define storeWord(i, oop, val)          HCwordPut(oop, i, val)

						/*ST!ObjectMemory!fetchByte:ofObject:!*/
#define fetchByte(i, oop)               HCbyte(oop, i)
						/*ST!ObjectMemory!storeByte:ofObject:withValue:!*/
#define storeByte(i, oop, bval)         HCbytePut(oop, i, bval)

						/*ST!ObjectMemory!fetchClassOf:!*/
/* BEWARE of side effects when using this macro */
#define fetchClass(oop)                 (isInt(oop) ? CLASS_SMALL_INTEGER : class(oop))

						/*ST!ObjectMemory!fetchWordLengthOf:!*/
#define Wlen(oop)                       size(oop)

						/*ST!ObjectMemory!fetchByteLengthOf:!*/
/* BEWARE of side effects when using this macro */
#define Blen(oop)                       ((Wlen(oop)<<1) - (odd(oop) ? 1 : 0))


						/*ST!ObjectMemory!headOfFreeChunkList:inSegment:!*/
						/*ST!ObjectMemory!headOfFreeChunkList:inSegment:put:!*/
#define FCLhead(s)                      (FCLhd[s])

					/*ST!ObjectMemory!toFreePointerListAdd:!*/
#define toFreeAdd(oop)			{ loctni(oop) = free_ptr;  free_ptr = (WORD)(oop); }

	/* deallocation has a two-way branch: bitmaps are freed using
	   free_bitmap(), other objects by making them into free chunks */
						/*ST!ObjectMemory!deallocate:!*/
						/*ST!ObjectMemory!toFreeChunkList:add:!*/
#ifdef SUN
#define free_bitmap mem_destroy
#else
#define free_bitmap free
#endif

#define deallocate(oop)                 { OOP oop4 = (oop);                             \
					  if (bitmap(oop4)) {				\
						if (loctn(oop4) != (WORD *)SCREEN)	\
							free_bitmap((char *)loctn(oop4));\
						freePut(oop4, TRUE);			\
						toFreeAdd(oop4);			\
					  } else {					\
						WORD Size = size(oop4);                 \
						words_left += Size;                     \
						MinBec(Size, BIG_SIZE);                 \
						class(oop4) = FCLhead(Size);            \
						FCLhead(Size) = oop4;                   \
					  }						\
					  ++oops_left;                                  \
					  if (lowOnSpace && oops_left >= ptr_limit 	\
					      &&  words_left > word_limit) 		\
						lowOnSpace = FALSE;			\
					}


						/*ST!ObjectMemory!instantiateClass:withPointers:!*/
/* BEWARE of side-effects when using this macro */
#define instPtrs(class, len)            allocate(len, FALSE, TRUE, (class))

						/*ST!ObjectMemory!instantiateClass:withWords:!*/
#define instWords(class, len)           allocate(len, FALSE, FALSE, (class))

						/*ST!ObjectMemory!instantiateClass:withBytes:!*/
/* BEWARE of side effects in the following macro: */
#define instBytes(class, len)           allocate(((len)+1)>>1, (len)&1, FALSE, (class))


						/*ST!Interpreter!fetchInteger:ofObject:!*/
/* fast versions of fetchInt() and storeInt() which do not check that the arg. is an int. */
#define ffetchInt(i, oop)               intVal(fetchPtr(i, oop))

						/*ST!Interpreter!storeInteger:ofObject:withValue:!*/
#define fstoreInt(i, oop, val)          NRstorePtr(i, oop, intObj(val))

						/*ST!Interpreter!transfer:fromIndex:ofObject:toIndex:ofObject:!*/
	/* special version used to copy from active context */
#define transferAndPop(c, from, to)                                   	\
	{       WORD    *fp = from, *tp = to, Count = c;                \
		pop(Count);						\
		while (Count-- > 0) {                                   \
			countUp(*fp);					\
			*tp++ = *fp++;                                  \
		}                                                       \
	}

	/* non-ref-counting version */
#define NRtransfer(c, from, to)                                   	\
	{       WORD    *fp = from, *tp = to, Count = c;                \
		while (Count-- > 0) {                                   \
			*tp++ = *fp;                                    \
			*fp++ = NIL_PTR;                                \
		}                                                       \
	}


				/*ST!Interpreter!initializeAssociationIndex!*/
#define VALUE           1

