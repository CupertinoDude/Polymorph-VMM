/*
	Convert Manchester 1 format to Manchester 2 format
	
*/
#ifdef SUN
#include <pixrect/pixrect_hs.h>
#endif
#define rectangle malloc
#include <stdio.h>
#include "StdHdr.h"
#include "ST.h"
#include "OMMacros.h"
#include "ClassPtrs.h"
#include "StdPtrs.h"
#include "Selectors.h"

#define MANCHESTER      42
#define MANCHESTER2	43

  /* #define HEAP_SPACE_STOP         (OM_SIZE - 1)     /* G/R p.658 */

WORD om[OM_SIZE];
NEWWORD new_om[OM_SIZE];

typedef union {
	ADDR	addr;
	unsigned index;	/* used for the free chain */
} LOC;

LOC otLoc[OT_SIZE];
NEWWORD newloc[OT_SIZE];

BYTE otRest[OT_SIZE], otCount[OT_SIZE];
   /* bytes 0, 2, 4, etc for refce counts,
      bytes 1, 3, 5 etc for free, ptr and odd bits */

WORD Sizes[OT_SIZE];    /* the new arrays */
NEWWORD newSizes[OT_SIZE];    /* the new arrays */
OOP Classes[OT_SIZE];

extern long ftell();
extern char *malloc();

main(argc, argv)
int argc;
char *argv[];
{
	unsigned n_words;

	if (argc != 3)
		err("usage: x2m xerox-image new-image");
		
	/* initialise free pointer bits */
	for_every_oop(oop) {
		freePut(oop, TRUE);
	}}
	
	load_snapshot(argv[1]);

	n_words = copy_old_to_new();

	write_snapshot(argv[2], n_words);
}

copy_old_to_new()		/* copy old OM to new OM */
{
	union {
		WORD words[2];
		float val;
		NEWWORD all;
	} f;
	unsigned di = 0;

	for_every_oop(oop) {
		unsigned loc = loctni(oop);
		int this_size = size(oop);

		if (isFree(oop)) {
			continue;
		}

 		newloc[oop] = di;
		if (ptrs(oop)) {
/* 			fprintf(stderr, "%04x p%d\n", oop, this_size); */
			if (class(oop) == CLASS_METH_CTX) {
				/* fix up instr ptr */
				OOP meth = om[loc+3];
				WORD hdr = om[loctni(meth)];
				WORD litCount = hdr&0x3F;
				WORD ip = intVal(om[loc+1]);
				WORD newip = ip + 2 /* for the header */ + 2*litCount;
				om[loc+1] = intObj(newip);
			} else if (class(oop) == CLASS_BLOCK_CTX) {
				/* patch initialIP too */
				OOP home = om[loc+5];
				OOP meth = om[loctni(home)+3];
				WORD hdr = om[loctni(meth)];
				WORD litCount = hdr&0x3F;
				WORD ip = intVal(om[loc+1]);
				WORD newip = ip + 2 + 2*litCount;
				WORD initip = intVal(om[loc+4]);
				WORD newinitip = initip + 2 + 2*litCount;
				om[loc+1] = intObj(newip);
				om[loc+4] = intObj(newinitip);
			}
			while (this_size-- > 0) {
				WORD this_field = om[loc++];
				if (isInt(this_field)) {
					new_om[di++] = 0x80000000|intVal(this_field);
				} else {
					new_om[di++] = this_field;
				}
			}
			newSizes[oop] = size(oop) * sizeof(NEWWORD);
		} else if (class(oop) == CLASS_FLOAT) {
			f.words[0] = om[loc++];  f.words[1] = om[loc];
#ifdef VAX
  			f.val *= 4;
#endif
/*  			fprintf(stderr, "%04x f%f\n", oop, f.val); */
			new_om[di++] = f.all;
			newSizes[oop] = sizeof(float);
		} else if (om[loctni(class(oop))+2] & 0x2000) {
/* 			fprintf(stderr, "%04x w%d\n", oop, this_size); */
 			/* pack 16-bit words into 32-bit words */
		        for ( ; this_size > 0 ; this_size -= 2) {
 				f.words[0] = om[loc++];
				f.words[1] = this_size > 1 ? om[loc++] : 0;
				new_om[di++] = f.all;
			}
			newSizes[oop] = size(oop)*sizeof(WORD);
		} else {
			int i;
/* 			fprintf(stderr, "%04x m%d\n", oop, this_size); */
			/* byte object (or CompiledMethod) */
			for (i = 0; i < lastPointer(oop); ++i, --this_size) {
				/* deal with literal field */
				WORD this_field = om[loc++];
				if (isInt(this_field)) {
					new_om[di++] = 0x80000000|intVal(this_field);
				} else {
					new_om[di++] = this_field;
				}
			}
			for ( ; i < size(oop); i += 2, this_size -= 2) {
				f.words[0] = om[loc++];
				f.words[1] = this_size > 1 ? om[loc++] : 0;
				new_om[di++] = f.all;
			}
			newSizes[oop] = lastPointer(oop) * sizeof(NEWWORD)
			  + (size(oop) - lastPointer(oop)) * sizeof(WORD)
			    - (odd(oop) ? 1 : 0);
		}
	}}
  	return di;
}

load_snapshot(f)
     char *f;
{
	unsigned obj_sp_len;            /* size of the object space */
	unsigned obj_tab_len;           /* size of the object table */
	FILE *image;
	extern long ftell();
 	extern char *rectangle(); 
	long ot_offset;
	WORD image_type;

	if ((image = fopen(f, "r")) == NULL)
		err("Cannot access image in st80.image");

	obj_sp_len = get32(image);

	fprintf(stderr, "%d words in the object space\n", obj_sp_len);

	if (obj_sp_len > OM_SIZE)
		err("Image is too big for memory");

	obj_tab_len = get32(image);

	fread((char *)&image_type, sizeof(WORD), 1, image);
	if (image_type != MANCHESTER)
		err("Image not in Manchester format");

	if (obj_tab_len != OT_SIZE)
		err("Object table is the wrong size");

	fseek(image, 512L, 0);

	/* read object space */
	{
		printf("Loading object space\n"); 

		fread((char *)om, sizeof(WORD), obj_sp_len, image);
	}

	ot_offset = (ftell(image) + 511) & ~511;
	fprintf(stderr, "Object table begins at offset %d\n", ot_offset);
	if (fseek(image, ot_offset, 0) == -1)
		err("Can't seek to object table");
	
	{       /* read object table */
		printf("Loading object table\n"); 
		fread((char *)otLoc, sizeof(unsigned), OT_SIZE, image);
		fread((char *)otCount, sizeof(BYTE), OT_SIZE, image);
		fread((char *)otRest, sizeof(BYTE), OT_SIZE, image);
		/* BEWARE if you change the oop representation */
		fread((char *)Sizes, sizeof(WORD), OT_SIZE, image);
		fread((char *)Classes,sizeof(OOP), OT_SIZE, image);
	}

	{
		WORD w, h;
		WORD *bm_area;

		printf("Loading bitmaps\n");
		
#ifdef SUN
		for_every_oop(oop) {
			if (!isFree(oop) && bitmap(oop) && loctni(oop) != 0) {
				fseek(image, (long)(loctni(oop)), 0);
				if ((loctn(oop) = (WORD *)pr_load(image, NULL)) == NULL) {
					fprintf(stderr, "Error loading bitmap\n");
					exit(1);
				}
				putchar('.'); fflush(stdout);
			}
		}}
#else
		for_every_oop(oop) {
			if (!isFree(oop) && bitmap(oop) && loctni(oop) != 0) {
				fseek(image, (long)(loctni(oop)), 0);
				loctn(oop) = (WORD *)rectangle(size(oop)*2);
				fread((char *)loctn(oop), sizeof(WORD), size(oop), image);
				putchar('.'); fflush(stdout);
			}
		}}
#endif
		putchar('\n');
	}

	fclose(image);

	printf("Snapshot loaded.\n"); 
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
		WORD imageType = MANCHESTER2;

		fwrite((char *)&imageType, sizeof(WORD), 1, image);
	}
	fseek(image, 512L, 0);

	{       /* write out object space */
		fwrite((char *)new_om, sizeof(NEWWORD), obj_sp_len, image);
	}

	{       /* write out object table */

		long ot_offset = (ftell(image)+511) & ~511;

		fseek(image, ot_offset, 0);
		fprintf(stderr, "object table begins at offset %ld\n", ot_offset);

		fwrite((char *)newloc, sizeof(ADDR), OT_SIZE, image);
		/* write out counts */
		fwrite((char *)otCount, 1, OT_SIZE, image);
		/* write out rest */
		fwrite((char *)otRest, 1, OT_SIZE, image);
		fwrite((char *)newSizes, sizeof(NEWWORD), OT_SIZE, image);
		for_every_oop(p) {
			NEWWORD c = class(p);
			fwrite((char *)&c, sizeof(c), 1, image);
		}}
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

#define litCount(m)  (om[loctni(m)] & 0x3F)

lastPointer(oop)
OOP oop;
{
	if (ptrs(oop))
		return size(oop);
	else if (class(oop) == CLASS_COMPILED_METHOD)
		return litCount(oop) + 1;
	else
		return 0;
}

