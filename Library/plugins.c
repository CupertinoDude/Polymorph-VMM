/*
\\		"Polymorph"
*/
#include "polymorph.h"

/**/
APTR  InitPolymorphPlugin(struct LIBRARY_CLASS *SelfBase)
{
	APTR rc=NULL;



	return(rc);
};

APTR  ExitPolymorphPlugin(struct LIBRARY_CLASS *SelfBase)
{
	APTR rc=NULL;
	return(rc);
};

/**/

struct pmxHandle *_XPLUG_Reserved(struct pmxHandle *handle)
{
	struct pmxHandle *rc=NULL;
	return(rc);
};

/*
	Reserved		Scripting Support ?
	LoadSegments    Generate an In-Memory Executable MemoryMap
	ExecSegments    Execute an In-Memory Executable MemoryMap content
*/
/**/

