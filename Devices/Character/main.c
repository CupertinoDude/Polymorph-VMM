/*
\\		"Template Device"
*/
#include "device.h"

STATIC CONST USED struct Resident RomTag =
{
  RTC_MATCHWORD,
  ( struct Resident* ) &RomTag,
  ( struct Resident* ) &RomTag + 1,
  RTF_AUTOINIT|RTF_NATIVE,
  DEVICE_VERSION,
  NT_DEVICE,
  0,
  (APTR) DevName,
  (APTR) DevIdentity,
  (APTR) InitTable
};

CONST APTR DevManagerTable[] =
{
	(APTR)&MCALL_Obtain,
	(APTR)&MCALL_Release,
	NULL,
	NULL,
	(APTR)&DCALL_Open,
	(APTR)&DCALL_Close,
	(APTR)&DCALL_Expunge,
	(APTR)&DCALL_Reserved,
	(APTR)-1
};

CONST struct TagItem DevManagerTags[] =
{
	    { MIT_Name,        (ULONG)"__library"   	},
	    { MIT_VectorTable, (ULONG)DevManagerTable	},
	    { MIT_Version,     1                    	},
	    { TAG_END,         0}
};

CONST struct TagItem InitTable[] =
{
	    { CLT_DataSize,      sizeof( struct DEVICE_CLASS )	},
	    { CLT_InitFunc,      ( ULONG ) DCALL_Init			},
	    { CLT_Interfaces,    ( ULONG ) DevInterfaces		},
	    { CLT_NoLegacyIFace, TRUE							},
		{TAG_END,	0}
};

STATIC CONST BYTE DevName[] = DEVICE_NAME;
STATIC CONST BYTE DevIdentity[] = DEVICE_IDENTITY;

int32 _start(void)
{
  return -1;
}

struct library *DCALL_Init(struct DEVICE_CLASS *SelfBase,
				 APTR SegmentList,
				 struct Interface *Executive)
{
	APTR rc=SelfBase;
	struct ExecIFace *IExec = (struct ExecIFace *)Executive;

	SelfBase->Library.lib_Node.ln_Type	= NT_DEVICE;
	SelfBase->Library.lib_Node.ln_Pri	= 0;
	SelfBase->Library.lib_Node.ln_Name = (APTR)DevName;
	SelfBase->Library.lib_Flags = LIBF_SUMUSED | LIBF_CHANGED;
	SelfBase->Library.lib_Version = DEVICE_VERSION;
	SelfBase->Library.lib_Revision = DEVICE_REVISION;
	SelfBase->Library.lib_IdString = (APTR)DevIdentity;
	SelfBase->SegmentList = SegmentList;
	SelfBase->IExec = (struct ExecIFace *)Executive;

	IExec->InitSemaphore(&SelfBase->Lock);
	IExec->ObtainSemaphore(&SelfBase->Lock);

	IExec->ReleaseSemaphore(&SelfBase->Lock);

	return(rc);
}

struct Library *DCALL_Open(struct LibraryManagerInterface *Self, ULONG version)
{
	APTR rc = (APTR) Self->Data.LibBase;
	struct DEVICE_CLASS *SelfBase = (APTR) Self->Data.LibBase;
	SelfBase->Library.lib_Flags &= ~LIBF_DELEXP;
	SelfBase->Library.lib_OpenCnt++;
	return(rc);
}

APTR DCALL_Close(struct LibraryManagerInterface *Self)
{
	APTR rc=NULL;
	struct DEVICE_CLASS *SelfBase = (APTR) Self->Data.LibBase;
	SelfBase->Library.lib_OpenCnt--;
	return(rc);
}

APTR DCALL_Expunge(struct LibraryManagerInterface *Self)
{
    struct ExecIFace *IExec = (struct ExecIFace *)(*(struct ExecBase **)4)->MainInterface;
	APTR rc=NULL;
	struct DEVICE_CLASS *SelfBase = (APTR) Self->Data.LibBase;

	if(SelfBase->Library.lib_OpenCnt)
	{
		SelfBase->Library.lib_Flags |= LIBF_DELEXP;
	}else{
		IExec->ObtainSemaphore(&SelfBase->Lock);

		IExec->ReleaseSemaphore(&SelfBase->Lock);

		IExec->ObtainSemaphore(&SelfBase->Lock);
		IExec->ReleaseSemaphore(&SelfBase->Lock);

		IExec->Remove((APTR)SelfBase);
		IExec->DeleteLibrary((APTR)SelfBase);
	};
	return(rc);
}

APTR DCALL_Reserved(void)
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

/**/
