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
	NULL,
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

struct library *LCALL_Init(struct LIBRARY_CLASS *Self,
				 BPTR SegmentList,
				 struct Interface *Executive)
{
	APTR rc=Self;
	struct ExecIFace *IExec = (struct ExecIFace *)Executive;
	struct Library *Base = NULL;

	Self->IExec = (struct ExecIFace *)Executive;
	Self->Library.lib_Node.ln_Type	= NT_LIBRARY;
	Self->Library.lib_Node.ln_Pri	= 0;
	Self->Library.lib_Node.ln_Name = (APTR)LibName;
	Self->Library.lib_Flags = LIBF_SUMUSED | LIBF_CHANGED;
	Self->Library.lib_Version = LIBRARY_VERSION;
	Self->Library.lib_Revision = LIBRARY_REVISION;
	Self->Library.lib_IdString = (APTR)LibIdentity;
	Self->SegmentList = SegmentList;

	if((Base = IExec->OpenLibrary("graphics.library",50L)))
		Self->IGfx = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

	if((Base = IExec->OpenLibrary("intuition.library",50L)))
		Self->IIntuition = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

	if((Base = IExec->OpenLibrary("dos.library",53L)))
		Self->IDOS = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

	if((Base = IExec->OpenLibrary("utility.library",50L)))
		Self->IUtility = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

	if((Base = IExec->OpenLibrary("locale.library",50L)))
		Self->ILocale = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

	return(rc);
}

struct Library *LCALL_Open(struct LibraryManagerInterface *Iface, ULONG version)
{
	APTR rc = (APTR) Iface->Data.LibBase;
	struct LIBRARY_CLASS *Self = (APTR) Iface->Data.LibBase;
	Self->Library.lib_Flags &= ~LIBF_DELEXP;
	Self->Library.lib_OpenCnt++;
	return(rc);
}

APTR LCALL_Close(struct LibraryManagerInterface *Iface)
{
	APTR rc=NULL;
	struct LIBRARY_CLASS *Self = (APTR) Iface->Data.LibBase;
	Self->Library.lib_OpenCnt--;
	return(rc);
}

APTR LCALL_Expunge(struct LibraryManagerInterface *Iface)
{
    struct ExecIFace *IExec = (struct ExecIFace *)(*(struct ExecBase **)4)->MainInterface;
	APTR rc=NULL;
	struct LIBRARY_CLASS *Self = (APTR) Iface->Data.LibBase;
	struct Library *Base = NULL;
	if(Self->Library.lib_OpenCnt)
	{
		Self->Library.lib_Flags |= LIBF_DELEXP;
	}else{

		if(Self->ILocale)
		{
			Base = Self->ILocale->Data.LibBase;
			IExec->DropInterface((APTR)Self->ILocale);
            IExec->CloseLibrary((APTR)Base);
		}

		if(Self->IUtility)
		{
			Base = Self->IUtility->Data.LibBase;
			IExec->DropInterface((APTR)Self->IUtility);
            IExec->CloseLibrary((APTR)Base);
		}

		if(Self->IDOS)
		{
			Base = Self->IDOS->Data.LibBase;
			IExec->DropInterface((APTR)Self->IDOS);
            IExec->CloseLibrary((APTR)Base);
		}

		if(Self->IIntuition)
		{
			Base = Self->IIntuition->Data.LibBase;
			IExec->DropInterface((APTR)Self->IIntuition);
            IExec->CloseLibrary((APTR)Base);
		}

		if(Self->IGfx)
		{
			Base = Self->IGfx->Data.LibBase;
			IExec->DropInterface((APTR)Self->IGfx);
            IExec->CloseLibrary((APTR)Base);
		}

		IExec->Remove((APTR)Self);
		IExec->DeleteLibrary((APTR)Self);
	};
	return(rc);
}

APTR LCALL_Reserved(void)
{
	return(NULL);
}

ULONG MCALL_Obtain(struct Interface *Iface)
{
	return(Iface->Data.RefCount++);
};

ULONG MCALL_Release(struct Interface *Iface)
{
	return(Iface->Data.RefCount--);
};
