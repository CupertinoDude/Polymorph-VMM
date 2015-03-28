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
/*Polymorph Instance*/
  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,
  (APTR)&ECALL_Reserved,
/**/
  (APTR)-1
};

