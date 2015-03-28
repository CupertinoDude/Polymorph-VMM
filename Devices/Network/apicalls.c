/*
\\		"AmigaOSv4 Device Driver"
*/
#include "device.h"

/*
 * DO NOT MODIFY THIS ANY FURTHER!!!! WE DON'T NEED ANY HIDDEN SPECIAL FUNCTIONS
*/
CONST struct TagItem DevInterfaceTags[] =
{
	    { MIT_Name,        (ULONG)"main"            },
	    { MIT_VectorTable, (ULONG)DevInterfaceTable	},
	    { MIT_Version,     1                        },
	    { TAG_END,         0                        }
};

CONST APTR DevInterfaces[] =
{
		(APTR)&DevManagerTags,
		(APTR)&DevInterfaceTags,
		NULL
};

CONST APTR DevInterfaceTable[] =
{
  (APTR)&MCALL_Obtain,
  (APTR)&MCALL_Release,
  NULL,
  NULL,
/**/
  (APTR)&DCALL_BeginIO,
  (APTR)&DCALL_AbortIO,
/**/
  (APTR)-1
};

/****** template.device/ ****************************************************
*---------------------------------------------------------------------------*
*
*****************************************************************************
*
*/
APTR  DCALL_BeginIO(struct Interface *Self,struct IORequest *iov)
{
	APTR rc=NULL;
	return(rc);
}

/****** template.device/ ****************************************************
*---------------------------------------------------------------------------*
*
*****************************************************************************
*
*/
APTR DCALL_AbortIO(struct Interface *Self,struct IORequest *iov)
{
	APTR rc=NULL;
	return(rc);
}

/**/
