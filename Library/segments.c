/*
\\		"Polymorph"
*/
#include "polymorph.h"

/**/
/*
        PolymorphSegmentHandle	"mc" is MetaClassification Information
*/
BPTR  InitPolymorphSegment(struct LIBRARY_CLASS *Self,APTR name,struct DataTypeHeader *dtype)
{
	BPTR	rc=0L;
/*
	dos_segment=Self->IDOS->AllocSegList((APTR)&ExecPolymorphApplication,NULL,131072L,PSIKEY_INIT);
*/
	return(rc);
};

void  ExitPolymorphSegment(struct LIBRARY_CLASS *Self,BPTR sc)
{
	return;
};

BPTR  ObtainPolymorphSegment(struct LIBRARY_CLASS *Self,APTR name,struct DataTypeHeader *dtype)
{
	BPTR rc=0L;
	return(rc);
};

void  ReleasePolymorphSegment(struct LIBRARY_CLASS *Self,BPTR sc)
{
	return;
};

BPTR  LoadPolymorphSegments(struct LIBRARY_CLASS *Self,BPTR sc,BPTR lock)
{
	BPTR	rc=0L;
	return(rc);
};

/**/
