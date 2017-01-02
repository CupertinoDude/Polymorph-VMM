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
	    { MIT_Version,     31                    	},
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
  (APTR)&LCALL_Reservation,
  (APTR)&LCALL_Reservation,
  (APTR)&LCALL_NewMemoryBlock,
  (APTR)&LCALL_EndMemoryBlock,
  (APTR)&LCALL_Reservation,
  (APTR)&LCALL_Reservation,
  (APTR)&LCALL_GetMemoryBlockA,
  (APTR)&LCALL_SetMemoryBlockA,
  (APTR)&LCALL_Reservation,
  (APTR)&LCALL_Reservation,
  (APTR)&LCALL_Reservation,
  (APTR)&LCALL_Reservation,
  (APTR)&LCALL_ObtainApplication,
  (APTR)&LCALL_ReleaseApplication,
  (APTR)&LCALL_GetApplicationA,
  (APTR)&LCALL_SetApplicationA,
  (APTR)&LCALL_InitApplicationEntryPoint,
  (APTR)&LCALL_ExecApplicationEntryPoint,
  (APTR)&LCALL_LoadApplicationSegments,
  (APTR)&LCALL_SaveApplicationSegments,
  (APTR)&LCALL_Reservation,
  (APTR)&LCALL_Reservation,
  (APTR)&LCALL_Reservation,
  (APTR)&LCALL_Reservation,
  (APTR)&LCALL_ObtainEnvironment,
  (APTR)&LCALL_ReleaseEnvironment,
  (APTR)&LCALL_GetEnvironmentA,
  (APTR)&LCALL_SetEnvironmentA,
  (APTR)&LCALL_FindEnvironmentA,
  (APTR)&LCALL_FindEnvironmentM,
  (APTR)&LCALL_LoadEnvironment,
  (APTR)&LCALL_SaveEnvironment,
  (APTR)&LCALL_Reservation,
  (APTR)&LCALL_Reservation,
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
*	SAFETY WARNING,  HOST AND CONTAINER SEMANTICS ARE EXCLUSIVE.
*  USE OF EXTERNAL(HOST) AND INTERNAL(CONTAINER) MIXED MEMORY ACCESS IS LETHAL
*  AND SUBJECT TO MIXED-ENDIAN DATA ISSUES.
*
*   HOSTed AND CONTAINed Memory Semantics must be assumed as ACTIVELY HOSTILE!!
*
*    LE containment on BE hosts (and the reverse) IS a practical hostile example.
*
*****************************************************************************
*
*/

void LCALL_Reservation (struct PolymorphIFace *IFace)
{
/*	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;*/
	return;
}

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

/****** polymorph.library/NewMemoryBlock() **********************************
*---------------------------------------------------------------------------*
*
*	NAME
*		NewMemoryBlock()
*
*	SYNOPSIS
*		LCALL_NewMemoryBlock(container, size, attrs)
*
*	DESCRIPTION
*		Polymorph Internal Memory Management,
*	Allocator for Container and Application Memory Usage with Resource-Tracking
*
*	Container and Contained Applications only please
*
*****************************************************************************
*
*/
APTR LCALL_NewMemoryBlock(struct PolymorphIFace *IFace, APTR container, u_long size, u_long attrs)
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
	ULONG 	rc=0L;

	return(rc);
}


/****** polymorph.library/EndMemoryBlock() **********************************
*---------------------------------------------------------------------------*
*
*	NAME
*		EndMemoryBlock()
*
*	SYNOPSIS
*
*	DESCRIPTION
*	  Resource-Tracking cleanup and clean removal of memory from a container
*
*	DANGEROUS FOR NON-CONTAINED MEMORY REGIONS.
*****************************************************************************
*
*/
void LCALL_EndMemoryBlock(struct PolymorphIFace *IFace, APTR block )
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
}

/****** polymorph.library/GetMemoryBlock() **********************************
*---------------------------------------------------------------------------*
*
*	NAME
*		GetMemoryBlock()
*
*	SYNOPSIS
*
*	DESCRIPTION
*		Read tracking or security details about a MemoryBlock
*
*****************************************************************************
*
*/
u_long LCALL_GetMemoryBlockA(struct PolymorphIFace *IFace, APTR block )
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
}

/****** polymorph.library/SetMemoryBlock() **********************************
*---------------------------------------------------------------------------*
*
*	NAME
*		SetMemoryBlock()
*
*	SYNOPSIS
*
*	DESCRIPTION
*		modify MemoryBlock tracking or security settings
*	  
*
*****************************************************************************
*
*/
u_long LCALL_SetMemoryBlockA(struct PolymorphIFace *IFace, APTR block, u_long attrs)
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
	u_long rec=0L;
	return(rc);
}


/****** polymorph.library/() **********************************
*---------------------------------------------------------------------------*
*
*	NAME
*		()
*
*	SYNOPSIS
*
*	DESCRIPTION
*		Create or Find a valid runtime image for an application
*	When an existing Application Image exists CopyOnWrite cloning may occur
*
*****************************************************************************
*
*/
void LCALL_ObtainApplication(struct PolymorphIFace *IFace, , )
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
}

/****** polymorph.library/() **********************************
*---------------------------------------------------------------------------*
*
*	NAME
*		()
*
*	SYNOPSIS
*
*	DESCRIPTION
*	  
*
*****************************************************************************
*
*/
void LCALL_ReleaseApplication(struct PolymorphIFace *IFace, , )
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
}

/****** polymorph.library/() **********************************
*---------------------------------------------------------------------------*
*
*	NAME
*		()
*
*	SYNOPSIS
*
*	DESCRIPTION
*		Read attributes from a contained Application,
*	This may incur special operations penalties
*
*****************************************************************************
*
*/
void LCALL_GetApplicationA(struct PolymorphIFace *IFace, , )
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
}

/****** polymorph.library/() **********************************
*---------------------------------------------------------------------------*
*
*	NAME
*		()
*
*	SYNOPSIS
*
*	DESCRIPTION
*		Write attribute changes to an Application being contained,
*	For some special attributes,  additional operations occur.
*
*****************************************************************************
*
*/
void LCALL_SetApplicationA(struct PolymorphIFace *IFace, , )
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
}

/****** polymorph.library/() **********************************
*---------------------------------------------------------------------------*
*
*	NAME
*		()
*
*	SYNOPSIS
*
*	DESCRIPTION
*	  Setup an "EntryPoint" Hook and provide an external addressable handle
*	Security concerns mean code provided is of limited viability,
*		and will require re-Init() of "EntryPoint"s for complex containers
*
*****************************************************************************
*
*/
void LCALL_InitApplicationEntryPoint(struct PolymorphIFace *IFace, , )
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
}
 
/****** polymorph.library/() **********************************
*---------------------------------------------------------------------------*
*
*	NAME
*		()
*
*	SYNOPSIS
*
*	DESCRIPTION
*		Execute code using an "EntryPoint" handle
*	Security is a consideration here along with relocation demands
*
*****************************************************************************
*
*/
void LCALL_ExecApplicationEntryPoint(struct PolymorphIFace *IFace, , )
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
}
 
/****** polymorph.library/() ************************************
*---------------------------------------------------------------------------*
*
*	NAME
*		LoadSegments()
*
*	SYNOPSIS
*
*	DESCRIPTION
*		Identify and Transfer an Application into a Container(creating if required)
*
*****************************************************************************
*
*/
BPTR LCALL_LoadApplicationSegments(struct PolymorphIFace *IFace, APTR fname )
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
	ULONG 	rc=0L;
	BPTR	fh=0L,sc=0L;
	struct DataType *dtype=NULL;

	fh=Self->IDOS->Lock(fname,EXCLUSIVE_LOCK);
	if(fh)
	{
		if(Self->IDataTypes)
			dtype=(APTR)Self->IDataTypes->ObtainDataTypeA(DTST_FILE,(APTR)fh,NULL);
		if(dtype)
		{
			sc=ObtainPolymorphSegment(Self,fname,dtype->dtn_Header);
			if(!sc)
				sc=InitPolymorphSegment(Self,fname,dtype->dtn_Header);
			if(sc)
				rc=LoadPolymorphSegments(Self,sc,fh);
			Self->IDataTypes->ReleaseDataType(dtype);
		}
		Self->IDOS->UnLock(fh);
	}
	if(sc)
		rc=sc;

	return(rc);
}

/****** polymorph.library/SaveApplicationSegments() ************************************
*---------------------------------------------------------------------------*
*
*	NAME
*		SaveApplicationSegments()
*
*	SYNOPSIS
*
*	DESCRIPTION
*		Streams out a single "Application" Image to any valid disk/network target
*		post-write changes
*
*****************************************************************************
*
*/
ULONG LCALL_SaveApplicationSegments(struct PolymorphIFace *IFace, APTR fname )
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
	ULONG rc=0L;
	return(rc);
}

/****** polymorph.library/ObtainEnvironment() **********************************
*---------------------------------------------------------------------------*
*
*	NAME
*		ObtainEnvironment()
*
*	SYNOPSIS
*
*	DESCRIPTION
*	  Creates or Finds container resources
*
*	Special case (NULL, NULL) forces creation without search,
*		this provides for container to container runtime reconstruction,
*		garbage-collection or security flags may trigger this special-case
*
*	SECURITY-WARNING:  MULTIPLE-APPLICATION-CONTAINER-SHARING IS SUBJECT
*		TO ACTIVELY FORKING RUNTIME STATE WITH ONLY THE NEWEST SURVIVING
*	ALL DYNAMIC ALLOCATIONS MAY BE RELOCATED ON DEMAND.
*	(THIS MAY TRIGGER UNDEFINED INCOHERENCE WITHIN SOME SYSTEMS)
*
*****************************************************************************
*
*/
APTR LCALL_ObtainEnvironment(struct PolymorphIFace *IFace, u_long magic, u_long options )
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
	APTR rc=NULL;

	return(rc);
}

/****** polymorph.library/() **********************************
*---------------------------------------------------------------------------*
*
*	NAME
*		()
*
*	SYNOPSIS
*
*	DESCRIPTION
*	  Unlock Container resources (triggers garbage collection)
*
*****************************************************************************
*
*/
void LCALL_ReleaseEnvironment(struct PolymorphIFace *IFace, APTR Environment )
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;

	return;
}

/****** polymorph.library/() **********************************
*---------------------------------------------------------------------------*
*
*	NAME
*		()
*
*	SYNOPSIS
*
*	DESCRIPTION
*	  Read current container attributes
*
*****************************************************************************
*
*/
void LCALL_GetEnvironmentA(struct PolymorphIFace *IFace, , )
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
}

/****** polymorph.library/() **********************************
*---------------------------------------------------------------------------*
*
*	NAME
*		()
*
*	SYNOPSIS
*
*	DESCRIPTION
*	  Write container attribute changes
*
*****************************************************************************
*
*/
void LCALL_SetEnvironmentA(struct PolymorphIFace *IFace, , )
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
}

/****** polymorph.library/() **********************************
*---------------------------------------------------------------------------*
*
*	NAME
*		()
*
*	SYNOPSIS
*
*	DESCRIPTION
*	  Search for a Container by Attributes
*
*	This still requires additional compatability checking.
*		(it is possible to hard-load x86 code into an ARM/LE container)
*
*****************************************************************************
*
*/
void LCALL_FindEnvironmentA(struct PolymorphIFace *IFace, , )
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
}

/****** polymorph.library/() **********************************
*---------------------------------------------------------------------------*
*
*	NAME
*		()
*
*	SYNOPSIS
*
*	DESCRIPTION
*	  Search for a Container by MagicID (Value provided by plugins)
*
*	MagicID values may be arbitrary so are of limited reliability.
*
*****************************************************************************
*
*/
void LCALL_FindEnvironmentM(struct PolymorphIFace *IFace, , )
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
}

/****** polymorph.library/() **********************************
*---------------------------------------------------------------------------*
*
*	NAME
*		()
*
*	SYNOPSIS
*
*	DESCRIPTION
*	  Create a Memory Image from a streamed container.
*
*	Disk/Network or other sources are possible
*
*****************************************************************************
*
*/
void LCALL_LoadEnvironment(struct PolymorphIFace *IFace, , )
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
}

/****** polymorph.library/() **********************************
*---------------------------------------------------------------------------*
*
*	NAME
*		()
*
*	SYNOPSIS
*
*	DESCRIPTION
*	  Creates and Stores partial or complete container streams.
*
*	Storage Destinations may be Disk or Network
*
*****************************************************************************
*
*/
void LCALL_SaveEnvironment(struct PolymorphIFace *IFace, , )
{
	struct LIBRARY_CLASS *Self = (APTR) IFace->Data.LibBase;
}

/**/
