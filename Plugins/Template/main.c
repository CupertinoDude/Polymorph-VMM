/*
\\		"Polymorph"
//
\\		Virtual Machine and Emulator Stack
*/
#include "plugin.h"

STATIC CONST USED struct Resident RomTag =
{
  RTC_MATCHWORD,
  ( struct Resident* ) &RomTag,
  ( struct Resident* ) &RomTag + 1,
  RTF_AUTOINIT|RTF_NATIVE,
  LIBRARY_VERSION,
  NT_LIBRARY,
  0,
  (APTR) LibName,
  (APTR) LibIdentity,
  (APTR) InitTable
};

CONST APTR LibManagerTable[] =
{
	(APTR)&MCALL_Obtain,
	(APTR)&MCALL_Release,
	NULL,
	NULL,
	(APTR)&LCALL_Open,
	(APTR)&LCALL_Close,
	(APTR)&LCALL_Expunge,
	(APTR)&LCALL_Reserved,
	(APTR)-1
};

CONST struct TagItem LibManagerTags[] =
{
	    { MIT_Name,        (ULONG)"__library"   	},
	    { MIT_VectorTable, (ULONG)LibManagerTable	},
	    { MIT_Version,     1                    	},
	    { TAG_END,         0}
};

CONST struct TagItem InitTable[] =
{
	    { CLT_DataSize,      sizeof( struct LIBRARY_CLASS ) },
	    { CLT_InitFunc,      ( ULONG ) LCALL_Init			},
	    { CLT_Interfaces,    ( ULONG ) LibInterfaces		},
	    { CLT_NoLegacyIFace, TRUE							},
		{TAG_END,	0}
};

STATIC CONST BYTE LibName[] = LIBRARY_NAME;
STATIC CONST BYTE LibIdentity[] = LIBRARY_IDENTITY;

int32 _start(void)
{
  return -1;
}

struct library *LCALL_Init(struct LIBRARY_CLASS *SelfBase,
				 BPTR SegmentList,
				 struct Interface *Executive)
{
	APTR rc=SelfBase;
	struct ExecIFace *IExec = (struct ExecIFace *)Executive;

	SelfBase->Library.lib_Node.ln_Type	= NT_LIBRARY;
	SelfBase->Library.lib_Node.ln_Pri	= 0;
	SelfBase->Library.lib_Node.ln_Name = (APTR)LibName;
	SelfBase->Library.lib_Flags = LIBF_SUMUSED | LIBF_CHANGED;
	SelfBase->Library.lib_Version = LIBRARY_VERSION;
	SelfBase->Library.lib_Revision = LIBRARY_REVISION;
	SelfBase->Library.lib_IdString = (APTR)LibIdentity;
	SelfBase->SegmentList = SegmentList;
	SelfBase->IExec = (struct ExecIFace *)Executive;

	SelfBase->GfxBase = IExec->OpenLibrary("graphics.library",0L);
	if(SelfBase->GfxBase)
		SelfBase->IGfx = (APTR)IExec->GetInterface(SelfBase->GfxBase,"main",1L,NULL);

	SelfBase->IntuitionBase = IExec->OpenLibrary("intuition.library",0L);
	if(SelfBase->IntuitionBase)
		SelfBase->IIntuition = (APTR)IExec->GetInterface(SelfBase->IntuitionBase,"main",1L,NULL);

	SelfBase->DOSBase = IExec->OpenLibrary("dos.library",0L);
	if(SelfBase->DOSBase)
		SelfBase->IDOS = (APTR)IExec->GetInterface(SelfBase->DOSBase,"main",1L,NULL);

	SelfBase->UtilityBase = IExec->OpenLibrary("utility.library",0L);
	if(SelfBase->UtilityBase)
		SelfBase->IUtility = (APTR)IExec->GetInterface(SelfBase->UtilityBase,"main",1L,NULL);

	IExec->InitSemaphore(&SelfBase->Lock);
	IExec->ObtainSemaphore(&SelfBase->Lock);
	IExec->ReleaseSemaphore(&SelfBase->Lock);

	return(rc);
}

struct Library *LCALL_Open(struct LibraryManagerInterface *Self, ULONG version)
{
	APTR rc = (APTR) Self->Data.LibBase;
	struct LIBRARY_CLASS *SelfBase = (APTR) Self->Data.LibBase;
	SelfBase->Library.lib_Flags &= ~LIBF_DELEXP;
	SelfBase->Library.lib_OpenCnt++;
	return(rc);
}

APTR LCALL_Close(struct LibraryManagerInterface *Self)
{
	APTR rc=NULL;
	struct LIBRARY_CLASS *SelfBase = (APTR) Self->Data.LibBase;
	SelfBase->Library.lib_OpenCnt--;
	return(rc);
}

APTR LCALL_Expunge(struct LibraryManagerInterface *Self)
{
    struct ExecIFace *IExec = (struct ExecIFace *)(*(struct ExecBase **)4)->MainInterface;
	APTR rc=NULL;
	struct LIBRARY_CLASS *SelfBase = (APTR) Self->Data.LibBase;
	if(SelfBase->Library.lib_OpenCnt)
	{
		SelfBase->Library.lib_Flags |= LIBF_DELEXP;
	}else{
		IExec->ObtainSemaphore(&SelfBase->Lock);
		IExec->ReleaseSemaphore(&SelfBase->Lock);
		/*
			Insert Additional Expulsion code here...
		*/
		IExec->Remove((APTR)SelfBase);
		IExec->DeleteLibrary((APTR)SelfBase);
	};
	return(rc);
}

APTR LCALL_Reserved(void)
{
	return(NULL);
}

ULONG MCALL_Obtain(struct Interface *Self)
{
	return(Self->Data.RefCount++);
};

ULONG MCALL_Release(struct Interface *Self)
{
	return(Self->Data.RefCount--);
};
