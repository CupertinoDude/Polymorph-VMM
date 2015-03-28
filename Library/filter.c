/*
\\		"Polymorph"
*/
#include "polymorph.h"

/*	The Offset of "LoadSeg()" in dos.library to apply the correct patch location
*/
#define	OFFSET_LOADSEG	(168)
/*
	This Libraries own Base Pointer
*/
APTR PolymorphBase;
/*
	The Enable Switch for Polymorph to Extend the OS with Sandbox and Plugin support
*/
BOOL EnablePolymorphX;
/*
	Original dos.library/LoadSeg() Interface Method
*/
BPTR (*XCALL_LoadSegDOS)(struct DOSIFace *IFace,CONST_STRPTR name);
/*
	Polymorph / LoadSeg() Method Applied to dos.library
*/
BPTR LCALL_LoadSegVMM(struct DOSIFace *IFace,CONST_STRPTR name);

/**/
void InitPolymorphFilter(struct LIBRARY_CLASS *SelfBase)
{
	SelfBase->IExec->Forbid();
	if(SelfBase->IDOS)
	{
		if(!XCALL_LoadSegDOS)
		{
			XCALL_LoadSegDOS=(APTR)SelfBase->IExec->SetMethod((APTR)SelfBase->IDOS,OFFSET_LOADSEG,(APTR)LCALL_LoadSegVMM);
		}
	}
	if(XCALL_LoadSegDOS)
	{
		PolymorphBase=SelfBase;
	}
	if(PolymorphBase)
	{
		EnablePolymorphX=POLYMORPH_ENABLED;
    }
	SelfBase->IExec->Permit();
	return;
};

void ExitPolymorphFilter(struct LIBRARY_CLASS *SelfBase)
{
	SelfBase->IExec->Forbid();
	if(EnablePolymorphX)
	{
		EnablePolymorphX=FALSE;
    }
	if(XCALL_LoadSegDOS)
	{
		SelfBase->IExec->SetMethod((APTR)SelfBase->IDOS,OFFSET_LOADSEG,(APTR)XCALL_LoadSegDOS);
		PolymorphBase=NULL;
	}
	SelfBase->IExec->Permit();
	return;
};

/*
	I am deliberately using the following overloaded construct to make these as atomic as possible
		Also this forces that by whatever means is used for accessing Exec...
		THESE calls will ALWAYS work because there is no local anything to potentially damage them

	((struct ExecIFace *)((*(struct ExecBase **)4)->MainInterface))->METHOD_CALL_HERE();
*/
void  EnableFilter(void)
{
    ((struct ExecIFace *)((*(struct ExecBase **)4)->MainInterface))->Forbid();
	EnablePolymorphX = TRUE;
    ((struct ExecIFace *)((*(struct ExecBase **)4)->MainInterface))->Permit();
};

void  DisableFilter(void)
{
    ((struct ExecIFace *)((*(struct ExecBase **)4)->MainInterface))->Forbid();
	EnablePolymorphX = FALSE;
    ((struct ExecIFace *)((*(struct ExecBase **)4)->MainInterface))->Permit();
};

/*
	WARNING!!! RUNNING FROM dos.library/LoadSeg() CONTEXT!!! WARNING!!!

	1=Validly Call the native loader (retained pointer "XCALL_LoadSegDOS")
	2=Verify Native Failure
	3=Verify Polymorph is Enabled
	4=Identify the Target Object using DataTypes
	5=Verify a PolymorphPlugin matching the DataTypeID
	6=Generate a PolymorphApplication Object with Plugin Information bindings

	I need to complete the "Loading" of any arbitrary "object" from disk into memory without
		the object becoming unlinked by copying and breaking the Exec Linked List concerned

*/
BPTR LCALL_LoadSegVMM(struct DOSIFace *XDOS,CONST_STRPTR Name)
{
	BPTR 					rc=0L,fh=0L,sc=0L;
	struct LIBRARY_CLASS 	*Self=PolymorphBase;
	struct DataType			*dtype=NULL;

	if(XCALL_LoadSegDOS)
	{
		rc=XCALL_LoadSegDOS(XDOS,Name);
	}else{
	    ((struct ExecIFace *)((*(struct ExecBase **)4)->MainInterface))->DebugPrintF("LoadSegVMM(%s)\n", Name);
	};
	if(!rc && EnablePolymorphX)
	{
		fh=Self->IDOS->Lock(Name,EXCLUSIVE_LOCK);
		if(fh)
		{
			if(Self->IDataTypes)
			{
				dtype=Self->IDataTypes->ObtainDataTypeA(DTST_FILE,(APTR)fh,NULL);
				if(dtype)
				{
					sc=ObtainPolymorphSegment(Self,(APTR)Name,dtype->dtn_Header);
					if(!sc)
						sc=InitPolymorphSegment(Self,(APTR)Name,dtype->dtn_Header);
					if(sc)
						sc=LoadPolymorphSegments(Self,sc,fh);
					Self->IDataTypes->ReleaseDataType(dtype);
				};
			};
			Self->IDOS->UnLock(fh);
		};
		if(sc)
			rc=sc;
	};
	return(rc);
};

/**/
