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

$Header: OMMacros.h,v 1.5 85/06/27 19:05:16 miw Exp $
$Log:	OMMacros.h,v $
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

#define HDR_SIZE        2               /* size (in words) of an object header */

#define BIG_SIZE                40              /* largest index into free chunk list heads */

#define NON_PTR         ONES            /* something that can't be an index into the
					   object table */
#define OT_SIZE         (32*1024)          /* size of object table */

#define OM_SIZE                 (500*1024)        /* size of object memory, in WORDs!! */

#define HUGE_SIZE       256             /* G/R p.661 */


#define FIRST_OOP       0
#define LAST_OOP        0xFFFE

			/* don't forget the extra } */
#define for_every_oop(x)        { register unsigned x;				\
				  for (x = FIRST_OOP; x <= LAST_OOP; x += 2)	\
				
#define DISPLAY         LAST_OOP        /* oop of the display bitmap */

				
/* assume that ObjectTableStart = 0 (G/R p.661) */
/* note that we don't divide the object memory into segments */
					/*ST!ObjectMemory!segmentBitsOf:!*/
					/*ST!ObjectMemory!segmentBitsOf:put:!*/

					/*ST!ObjectMemory!locationBitsOf:!*/
					/*ST!ObjectMemory!locationBitsOf:put:!*/
#define loctn(oop)	(otLoc[(oop)>>1])

/* The following macros depend on the fact that the tag bit of a nonSmallInteger
    is zero */
					/*ST!ObjectMemory!countBitsOf:!*/
					/*ST!ObjectMemory!countBitsOf:put:!*/
#define count(oop)	(otRest[oop])

					/* odd() returns 0 or ODD */
#define ODD 	1
					/*ST!ObjectMemory!oddBitOf:!*/
#define odd(oop)	(otRest[(oop)|1]&ODD)
					/*ST!ObjectMemory!oddBitOf:put:!*/
#define oddPut(oop, isOdd)	{ if (isOdd) otRest[(oop)|1] |= ODD; else otRest[(oop)|1] &= ~ODD; }

					/* ptrs() returns 0 or PTRS */
#define PTRS	2
					/*ST!ObjectMemory!pointerBitOf:!*/
#define ptrs(oop)		(otRest[(oop)|1]&PTRS)
					/*ST!ObjectMemory!pointerBitOf:put:!*/
#define ptrsPut(oop isPtrs)	{ if (isPtrs) otRest[(oop)|1] |= PTRS; else otRest[(oop)|1] &= ~PTRS; }

				/* free() returns 0 or FREE */
#define FREE	4
					/*ST!ObjectMemory!freeBitOf:!*/
#define free(oop)		(otRest[(oop)|1]&FREE)
					/*ST!ObjectMemory!freeBitOf:put:!*/
#define freePut(oop, isFree)	{ if (isFree) otRest[(oop)|1] |= FREE; else otRest[(oop)|1] &= ~FREE; }



						/*ST!ObjectMemory!heapChunkOf:word:!*/
#define HCword(oop, offset)             (om[loctn(oop) + (offset)])
						/*ST!ObjectMemory!heapChunkOf:word:put:!*/
#define HCwordPut(oop, offset, val)     { HCword(oop, offset) = (val); }

						/*ST!ObjectMemory!heapChunkOf:byte!*/
/* note that bytes within words are the "wrong way round", except on the Apollo  */
#ifdef APOLLO
#define HCbyte(oop, boffset)            ((BYTE)((BYTE *)om)[(loctn(oop)<<1) + (boffset)])
#else
#define HCbyte(oop, boffset)            (((BYTE *)om)[(loctn(oop)<<1) + ((boffset)^1)])
#endif

						/*ST!ObjectMemory!heapChunkOf:byte:put:!*/
#define HCbytePut(oop, boffset, bval)   { HCbyte(oop, boffset) = (bval); }

						/*ST!ObjectMemory!sizeBitsOf:!*/
						/*ST!ObjectMemory!sizeBitsOf:put:!*/
#define size(oop)			HCword(oop, 0)

						/*ST!ObjectMemory!classBitsOf:!*/
						/*ST!ObjectMemory!classBitsOf:put:!*/
#define class(oop)                      HCword(oop, 1)

						/*ST!ObjectMemory!hasObject:!*/
/* hasObject(oop): is there a valid object with the given oop ?  */
#define hasObject(oop)                  (count(oop) != 0)


						/*ST!ObjectMemory!integerValueOf:!*/
/* the following is NON-PORTABLE - assumes -1>>1 == -1 */
#define intVal(oop)                     (((SIGNED)oop)>>1)

						/*ST!ObjectMemory!integerObjectOf:!*/
#define intObj(val)                     (((val)<<1)+1)

						/*ST!ObjectMemory!isIntegerObject:!*/
#define isInt(oop)                      ((oop)&1)

						/*ST!ObjectMemory!isIntegerValue:!*/
/* BEWARE of side effects when using this macro */
/* Note that for large systems we have to coerce val to int */
#define isIntVal(val)                   (((int)val) <= MAX_INT && ((int)val) >= MIN_INT)

#define intToObj(i)                     ((i) & 0xFFFE)
#define objToInt(o)                     ((o) | 1)


						/*ST!ObjectMemory!fetchPointer:ofObject:!*/
#define fetchPtr(i, oop)                HCword(oop, HDR_SIZE + (i))

						/*ST!ObjectMemory!storePointer:ofObject:withValue:!*/
/* DON'T alter the order of assignments in the initialiser */
#define storePtr(i, oop, val)           { WORD oop3 = (oop),  val1 = (val);             \
					  WORD chunkInd = HDR_SIZE + (i);               \
					  countUp(val1);                                \
					  countDown(HCword(oop3, chunkInd));            \
					  HCwordPut(oop3, chunkInd, val1);              \
					}


						/*ST!ObjectMemory!fetchWord:ofObject:!*/
#define fetchWord(i, oop)               HCword(oop, HDR_SIZE+(i))

						/*ST!ObjectMemory!storeWord:ofObject:withValue:!*/
#define storeWord(i, oop, val)          HCwordPut(oop, HDR_SIZE+(i), val)

						/*ST!ObjectMemory!fetchByte:ofObject:!*/
#define fetchByte(i, oop)               HCbyte(oop, (HDR_SIZE*2) + (i))
						/*ST!ObjectMemory!storeByte:ofObject:withValue:!*/
#define storeByte(i, oop, bval)         HCbytePut(oop, (HDR_SIZE*2) + (i), bval)

						/*ST!ObjectMemory!fetchClassOf:!*/
/* BEWARE of side effects when using this macro */
#define fetchClass(oop)                 (isInt(oop) ? CLASS_SMALL_INTEGER : class(oop))

						/*ST!ObjectMemory!fetchWordLengthOf:!*/
#define Wlen(oop)                       (size(oop) - HDR_SIZE)

						/*ST!ObjectMemory!fetchByteLengthOf:!*/
/* BEWARE of side effects when using this macro */
#define Blen(oop)                       ((Wlen(oop)<<1) - (odd(oop) ? 1 : 0))


						/*ST!ObjectMemory!headOfFreeChunkList:inSegment:!*/
						/*ST!ObjectMemory!headOfFreeChunkList:inSegment:put:!*/
#define FCLhead(s)                      (FCLhd[s])

						/*ST!ObjectMemory!deallocate:!*/
						/*ST!ObjectMemory!toFreeChunkList:add:!*/
#define deallocate(oop)                 { WORD oop4 = (oop);                            \
					  WORD Size = size(oop4);                       \
					  words_left += Size;                           \
					  ++oops_left;                                  \
					  MinBec(Size, BIG_SIZE);                       \
					  class(oop4) = FCLhead(Size);                 \
					  FCLhead(Size) = oop4;                        \
					}


						/*ST!ObjectMemory!instantiateClass:withPointers:!*/
/* BEWARE of side-effects when using this macro */
#define instPtrs(class, len)            allocate(HDR_SIZE+(len), FALSE, TRUE, (class))

						/*ST!ObjectMemory!instantiateClass:withWords:!*/
#define instWords(class, len)           allocate(HDR_SIZE+(len), FALSE, FALSE, (class))

						/*ST!ObjectMemory!instantiateClass:withBytes:!*/
/* BEWARE of side effects in the following macro: */
#define instBytes(class, len)           allocate(HDR_SIZE+(((len)+1)>>1), (len)&1, FALSE, (class))


						/*ST!Interpreter!fetchInteger:ofObject:!*/
/* fast versions of fetchInt() and storeInt() which do not check that the arg. is an int. */
#define ffetchInt(i, oop)               intVal(fetchPtr(i, oop))

						/*ST!Interpreter!storeInteger:ofObject:withValue:!*/
#define fstoreInt(i, oop, val)          storePtr(i, oop, intObj(val))

						/*ST!Interpreter!transfer:fromIndex:ofObject:toIndex:ofObject:!*/
/* NB: this version of transfer takes advantage of the fact that refce. counts stay the same */
#define transfer(c, fi, from, ti, to)                                   \
	{       WORD    *fp = &fetchPtr(fi, from),                      \
			*tp = &fetchPtr(ti, to),                        \
			Count = c;                                      \
		while (Count-- > 0) {                                   \
			*tp++ = *fp;                                    \
			*fp++ = NIL_PTR;                                \
		}                                                       \
	}


				/*ST!Interpreter!initializeAssociationIndex!*/
#define VALUE           1

