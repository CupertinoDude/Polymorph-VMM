/*
\\		"Polymorph"
*/
#include "polymorph.h"

/*
\\	Primary "main" Interface
*/
CONST struct TagItem LibInterfaceTags[] =
{
	    { MIT_Name,        (ULONG)"main"   	        },
	    { MIT_VectorTable, (ULONG)LibInterfaceTable	},
	    { MIT_Version,     1                    	},
	    { TAG_END,         0                    	}
};

/*apicalls.c*/
CONST APTR LibInterfaceTable[] =
{
  (APTR)&MCALL_Obtain,
  (APTR)&MCALL_Release,
  NULL,
  NULL,
  (APTR)&LCALL_ScriptExec,
  (APTR)&LCALL_OptionTagList,
  (APTR)&LCALL_LoadSegments,
  (APTR)&LCALL_StoreSegment,
  (APTR)&LCALL_ObtainObject,
  (APTR)&LCALL_ReleaseObject,
  (APTR)&LCALL_GetObjectAttr,
  (APTR)&LCALL_SetObjectAttr,
/**/
  (APTR)-1
};

/*
\\	Existing Interface Definitions provision to the OS here
*/
CONST APTR LibInterfaces[] =
{
		(APTR)&LibManagerTags,
        (APTR)&LibInterfaceTags,
		(APTR)&LibEmulationTags,
		NULL
};

/****** polymorph.library/-Background- *************************************
*---------------------------------------------------------------------------*
*
*   BACKGROUND
*
*	This is a Virtual Machine / Emulation / JIT / Translation Extension for
*	AmigaOS to support arbitrary Application Container types
*
*	Each PolymorphApplication plugin added to the system will have its own
*	limitations for what it can and can not support
*
*****************************************************************************
*
*/

/****** polymorph.library/-RexxHost- ***************************************
*---------------------------------------------------------------------------*
*
*	ARexx will be supported through this ScriptExec vector...
*
*		However that is a "FUTURE SUPPORT" item, not to be written until
*       primary code is functional
*
*****************************************************************************
*
*/
APTR  LCALL_ScriptExec(struct PolymorphIFace *IFace, struct RexxMsg * rmsg, STRPTR * result)
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
	APTR rc=Self;	rc=NULL;	/* DEBUG Placeholder */
	return(rc);
}

/****** polymorph.library/OptionTagList() **********************************
*---------------------------------------------------------------------------*
*
*	NAME
*		PolymorphOptionTags()
*		PolymorphOptionTagList()
*
*	SYNOPSIS
*
*	DESCRIPTION
*
*****************************************************************************
*
*/
ULONG LCALL_OptionTagList(struct PolymorphIFace *IFace, struct TagItem *options)
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
	ULONG rc=0L;

	if(options)
		rc=0xFFFFFFFF;

	return(rc);
}

/****** polymorph.library/LoadSegments() ************************************
*---------------------------------------------------------------------------*
*
*	NAME
*		LoadSegments()
*
*	SYNOPSIS
*
*	DESCRIPTION
*
*****************************************************************************
*
*/
BPTR LCALL_LoadSegments(struct PolymorphIFace *IFace, APTR fname )
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
	ULONG 	rc=0L;
	BPTR	fh=0L,sc=0L;
	struct DataType *dtype=NULL;

	fh=Self->IDOS->Lock(fname,EXCLUSIVE_LOCK);
	if(fh)
	{
		if(Self->IDataTypes)
		{
			Self->IDataTypes->ObtainDataTypeA(DTST_FILE,(APTR)fh,NULL);
			if(dtype)
			{
				sc=ObtainPolymorphSegment(Self,fname,dtype->dtn_Header);
				if(!sc)
					sc=InitPolymorphSegment(Self,fname,dtype->dtn_Header);
				if(sc)
					rc=LoadPolymorphSegments(Self,sc,fh);
				Self->IDataTypes->ReleaseDataType(dtype);
			}
		}
		Self->IDOS->UnLock(fh);
	}
	if(sc)
		rc=sc;

	return(rc);
}

/****** polymorph.library/StoreSegment() ************************************
*---------------------------------------------------------------------------*
*
*	NAME
*		StoreSegment()
*
*	SYNOPSIS
*
*	DESCRIPTION
*
*****************************************************************************
*
*/
ULONG LCALL_StoreSegment(struct PolymorphIFace *IFace, BPTR fh )
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
	ULONG rc=0L;
	return(rc);
}

/****** polymorph.library/ObtainPolymorphObject() *************************
*---------------------------------------------------------------------------*
*
*	NAME
*		ObtainPolymorphObject()
*
*	SYNOPSIS
*
*	DESCRIPTION
*
*****************************************************************************
*
*/
APTR LCALL_ObtainObject(struct PolymorphIFace *IFace, struct TagItem *setup)
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
	APTR rc=NULL;
	return(rc);
}

/****** polymorph.library/ReleasePolymorphObject() ************************
*---------------------------------------------------------------------------*
*
*	NAME
*		ReleasePolymorphObject()
*
*	SYNOPSIS
*
*	DESCRIPTION
*
*****************************************************************************
*
*/
APTR LCALL_ReleaseObject(struct PolymorphIFace *IFace, APTR object)
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
	APTR rc=NULL;
	return(rc);
}

/****** polymorph.library/GetPolymorphObjectAttr() **************************
*---------------------------------------------------------------------------*
*
*	NAME
*		GetPolymorphObjectAttr()
*
*	SYNOPSIS
*
*	DESCRIPTION
*
*****************************************************************************
*
*/
ULONG LCALL_GetObjectAttr(struct PolymorphIFace *IFace, APTR object, ULONG attr)
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
	ULONG rc=0L;
	return(rc);
}

/****** polymorph.library/SetPolymorphObjectAttr() **************************
*---------------------------------------------------------------------------*
*
*	NAME
*		SetPolymorphObjectAttr()
*
*	SYNOPSIS
*
*	DESCRIPTION
*
*****************************************************************************
*
*/
void LCALL_SetObjectAttr(struct PolymorphIFace *IFace, APTR object, ULONG attr, ULONG value)
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
	return;
}

/**/
