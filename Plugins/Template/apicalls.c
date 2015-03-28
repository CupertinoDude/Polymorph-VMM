/*
\\		"Polymorph"
*/
#include "plugin.h"

CONST struct TagItem LibInterfaceTags[] =
{
	    { MIT_Name,        (ULONG)"main"   	        },
	    { MIT_VectorTable, (ULONG)LibInterfaceTable	},
	    { MIT_Version,     1                    	},
	    { TAG_END,         0                    	}
};

CONST APTR LibInterfaces[] =
{
		(APTR)&LibManagerTags,
        (APTR)&LibInterfaceTags,
		(APTR)&LibEmulationTags,
		NULL
};

CONST APTR LibInterfaceTable[] =
{
  (APTR)&MCALL_Obtain,
  (APTR)&MCALL_Release,
  NULL,
  NULL,
  (APTR)&LCALL_Open,
  (APTR)&LCALL_Close,
  (APTR)&LCALL_Expunge,
  (APTR)&LCALL_Reserved,
/*Polymorph ESYS Interface* /
  (APTR)&LCALL_Reserved,    /*	"GetDataTypeClass()"	*/
  (APTR)&LCALL_Reserved,    /*	"ExecProcTags()"		*/
  (APTR)&LCALL_Reserved,	/*	"LoadSegments()"		*/
  (APTR)&LCALL_Reserved,	/*	"SaveSegments()"		*/
  (APTR)&LCALL_Reserved,	/*	""	*/
  (APTR)&LCALL_Reserved,	/*	""	*/
  (APTR)&LCALL_Reserved,	/*	""	*/
  (APTR)&LCALL_Reserved,	/*	""	*/
  (APTR)&LCALL_Reserved,	/*	""	*/
  (APTR)&LCALL_Reserved,	/*	""	*/
  (APTR)&LCALL_Reserved,	/*	""	*/
  (APTR)&LCALL_Reserved,	/*	""	*/
  (APTR)&LCALL_Reserved,	/*	""	*/
  (APTR)&LCALL_Reserved,	/*	""	*/
  (APTR)&LCALL_Reserved,	/*	""	*/
  (APTR)&LCALL_Reserved,	/*	""	*/
  (APTR)&LCALL_Reserved,	/*	""	*/
/**/
  (APTR)-1
};

/*
	Polymorph Specific Functionality

	The "Systems" Interface is the Host and Target "OS Call" mechanism support,

	The Initial 16 Vectors on the Interface are Polymorph Reserved for ESYS usage,
		additional LibCall tables are mandatory for the ECPU and EXCEPT handling mechanism.

	Whatever the Host OS uses for a Dynamic Binding Function is to be used for the mechanism
        of table management.	(Some OS such as Windows are excessively lacking in this)
*/

/**/
