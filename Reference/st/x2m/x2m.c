/*
	Convert Xerox Format to Manchester 1 format
	
*/
/* uses v1.5 of OMMacros.h, 2.1 of everything else */

/* #define REVERSE_BYTES	/* undefine this for machines with bytes the right way round (Apollo) */

#include <stdio.h>
#include "StdHdr.h"
#include "StdPtrs.h"
#include "ST.h"
#include "OMMacros.h"
#include "ClassPtrs.h"
#include "Selectors.h"

#define HEAP_SPACE_STOP         (OM_SIZE - 1)     /* G/R p.658 */

WORD om[OM_SIZE];

ADDR otLoc[OT_SIZE];
BYTE otRest[OT_SIZE+OT_SIZE];
   /* bytes 0, 2, 4, etc for refce counts,
      bytes 1, 3, 5 etc for free, ptr and odd bits */

WORD sizes[OT_SIZE];    /* the new arrays */
OOP classes[OT_SIZE];

extern long ftell();

main(argc, argv)
int argc;
char *argv[];
{
	unsigned n_words, obj_sp_len;
	extern unsigned load_snapshot(), sweep_OM();
	OOP i;

	if (argc != 3)
		err("usage: x2m xerox-image new-image");
		
	/* initialise free pointer bits */
	for_every_oop(oop) {
		freePut(oop, TRUE);
	}}
	
	obj_sp_len = load_snapshot(argv[1]);

#ifdef REVERSE_BYTES
	byte_swap();
#endif

	reverse_pointers();
	
	n_words = sweep_OM(obj_sp_len);

	/* saturate nil, true, false, etc */
	for (i = NIL_PTR; i <= MUST_BE_BOOLEAN;i+=2)
		count(i) = 1;
	
	/* make oop 0 unusable */
	freePut(0, FALSE);
	
	oop_swap();
	
	write_snapshot(argv[2], n_words);
}

reverse_pointers()
{
	/* reverse heap ptrs above low water mark */
	fprintf(stderr, "reversing pointers\n");
	for_every_oop(p) {
		if (!free(p)) {
			WORD sz = size(p);
			size(p) = p;
			loctn(p) = sz;
		}
	}}
}

unsigned sweep_OM(obj_sp_len)
unsigned obj_sp_len;
		/* sweep object memory, copying class and size fields into
		   separate arrays and compacting */
{
	unsigned si, di;
	register OOP oop;

	fprintf(stderr, "sweeping object memory\n");

	for (si = di = 0; si < obj_sp_len; ) {
		WORD ASize;
		register unsigned i;

		oop = om[si++];
		sizes[oop>>1] = (ASize = loctn(oop))-HDR_SIZE;
		classes[oop>>1] = om[si++];
		loctn(oop) = di;

		for (i = HDR_SIZE;  i < ASize; ++i) {
			om[di++] = om[si++];
		}
	}
	return di;
}


unsigned load_snapshot(filename)
char *filename;
{
	unsigned obj_sp_len;            /* size of the object space */
	unsigned obj_tab_len;           /* size of the object table */
	FILE *image;
	long ot_offset;

	if ((image = fopen(filename, "r")) == NULL)
		err("Cannot access image in st80.image");

	obj_sp_len = get32(image);

	fprintf(stderr, "%d words in the object space\n", obj_sp_len);

	if (obj_sp_len > OM_SIZE)
		err("Image is too big for memory");

	obj_tab_len = get32(image);

	if (obj_tab_len > (OT_SIZE-1)*2)
		err("The last oop is reserved for the display");

	if (getc(image) | getc(image))
		err("Image not in standard interchange format");

	fseek(image, 512L, 0);

	/* read object space */
	{
		unsigned wc;    /* word count */

		fprintf(stderr, "Loading object space\n");

		for (wc = 0; wc < obj_sp_len; )
			om[wc++] = get16(image);
	}

	ot_offset = (ftell(image) + 511) & ~511;
	fprintf(stderr, "Object table begins at offset %d\n", ot_offset);
	if (fseek(image, ot_offset, 0) == -1)
		err("Can't seek to object table");
	
	{       /* read object table */
		unsigned oop;

		fprintf(stderr, "Loading object table\n"); 

		/* BEWARE if you change the oop representation */
		for (oop = 0;  oop < obj_tab_len; oop += 2) {
			int c1, c2, c3, c4;

			c1 = getc(image)&0xFF;
			count(oop) = c1 > 127 ? 1 : (c1 << 1);  /* lsb==overflow bit */
			c2 = getc(image)&0xFF;
			c3 = getc(image)&0xFF;
			c4 = getc(image)&0xFF;
			oddPut(oop, (c2 & 0x80) != 0);
			ptrsPut(oop, (c2 & 0x40) != 0);
			freePut(oop, (c2 & 0x20) != 0);
			loctn(oop) = ((c2 & 0xF) << 16) + (c3 << 8) + c4;

/*
			if (oop < 20) {
				fprintf(stderr, "Object table entry %04x:\n", oop);
				fprintf(stderr, "\tcount = %d %c%c%c loctn = %08x\n",
					count(oop),
					odd(oop) ? 'O' : '.',
					ptrs(oop) ? 'P' : 'W',
					free(oop) ? 'F' : 'U',
					loctn(oop));
			}
*/
		}
	}

	fclose(image);

	fprintf(stderr, "Snapshot loaded.\n");
	return obj_sp_len;
}


get32(f)
FILE *f;
{
	unsigned n;

	n = getc(f)&0xFF;
	n = (n<<8) + (getc(f)&0xFF);
	n = (n<<8) + (getc(f)&0xFF);
	return (n<<8) + (getc(f)&0xFF);
}

get16(f)
FILE *f;
{
	unsigned n;

	n = getc(f)&0xFF;
	return (n<<8) + (getc(f)&0xFF);
}


write_snapshot(filename, obj_sp_len)            /* write out a snapshot in new format */
char *filename;
unsigned obj_sp_len;
{
	FILE *image;

	fprintf(stderr, "Writing snapshot.\n");

	if ((image = fopen(filename, "w")) == NULL)
		err("Can't open output file\n");

	fprintf(stderr, "%d words in the object space\n", obj_sp_len);
	
	put32(obj_sp_len, image);
	put32(OT_SIZE, image);

	{       /* write out format type */
		WORD imageType = 42;

		fwrite((char *)&imageType, sizeof(WORD), 1, image);
	}
	fseek(image, 512L, 0);

	{       /* write out object space */
		fwrite((char *)om, sizeof(WORD), obj_sp_len, image);
	}

	{       /* write out object table */

		long ot_offset = (ftell(image)+511) & ~511;

		fseek(image, ot_offset, 0);
		fprintf(stderr, "object table begins at offset %ld\n", ot_offset);

		fwrite((char *)otLoc, sizeof(ADDR), OT_SIZE, image);
		/* write out counts */
		for_every_oop(p) {
			putc(otRest[p], image);
		}}
		/* write out rest */
		for_every_oop(p) {
			putc(otRest[p|1], image);
		}}
		fwrite((char *)sizes, sizeof(WORD), OT_SIZE, image);
		fwrite((char *)classes,sizeof(OOP), OT_SIZE, image);
	}

	fclose(image);
	fprintf(stderr, "Snapshot written\n");
}



put16(w, f)
WORD w;
FILE *f;
{
	putc(w>>8, f);
	putc(w&255, f);
}

put32(w, f)
unsigned w;
FILE *f;
{
	putc(w>>24, f);
	putc(w>>16, f);
	putc(w>>8, f);
	putc(w&255, f);
}

err(s)
char *s;
{
	fprintf(stderr, "%s\n", s);
	exit(1);
}

byte_swap()	/* swap order of bytes in byte objects and compiled methods */
{
	union {
		struct {
			BYTE a,b;
		} two;
		WORD one;
	} w;
	BYTE tmp;
	extern WORD lastPointer();
	
	fprintf(stderr, "Swapping bytes\n");
	
	for_every_oop(p) {
		int i;
		WORD inspec;
		if (ptrs(p))
			continue;
		else {
			inspec = fetchPtr(2, class(p));
			if (inspec&0x4000)	/* words */
				continue;
		}
		for (i = lastPointer(p); i < size(p) ; ++i) {
			w.one = HCword(p, i);
			tmp = w.two.a;
			w.two.a = w.two.b;
			w.two.b = tmp;
			HCwordPut(p, i, w.one);
		}
	}}
}

#define litCount(m)  ((fetchPtr(0,m) & 0x7e) >> 1)

WORD lastPointer(oop)
OOP oop;
{
	if (ptrs(oop))
		return size(oop);
	else if (class(oop) == CLASS_COMPILED_METHOD)
		return litCount(oop) + HDR_SIZE + 1;
	else
		return HDR_SIZE;
}

#define newLitCount(m)  ((om[otLoc[(m)>>1]] & 0x7e) >> 1)

WORD newLastPointer(oop)	/* after the size and class have been stripped off */
OOP oop;
{
	if (ptrs(oop))
		return sizes[oop>>1];
	else if (classes[oop>>1] == CLASS_COMPILED_METHOD)
		return newLitCount(oop) + 1;
	else
		return 0;
}

oop_swap()	/* move the SmallInteger tag bit from lsb to msb */
{
	int i, last;
	OOP oop;

	fprintf(stderr, "Moving SmallInteger tag bit\n");

	for_every_oop(p) {
		if (free(p))
			continue;
		for (i = 0, last = newLastPointer(p);  i < last;  ++i) {
			oop = om[otLoc[p>>1]+i];
			if (isInt(oop))
				oop = (oop >> 1)|0x8000;
			else
				oop >>= 1;
			om[otLoc[p>>1]+i] = oop;
		}
		classes[p>>1] >>= 1;
	}}
}
