/*
\\		"Polymorph"
*/
#include "polymorph.h"

/*  The Daemon is the Central Manager for the entire of Polymorph.
 *
 *  It is the equivalent of a Deity to those Applications contained by Polymorph
 *    that are not native Applications for the Host OS (3.x or 4.x)
*/
struct DaemonApplication
{
	struct	LIBRARY_CLASS		*PolymorphBase;
/**/
	struct	ExecIFace			*IExec;
	struct  PolymorphIFace		*IPolymorph;
	struct	DOSIFace			*IDOS;
	struct	CommoditiesIFace	*ICX;
	struct	UtilityIFace		*IUtility;
	struct	LocaleIFace			*ILocale;
	struct	RexxSysIFace		*IREXX;
	struct	ApplicationIFace	*IApplication;
/**/
	uint32	ApplicationID;
/**/
	struct	MsgPort	*cxPort;
	ULONG	cxSignal;
	APTR	CommodityKey;
	APTR	CommodityFlags;
/**/
	struct	MsgPort				*rxPort;
	ULONG						rxSignal;
/**/
};

void  InitCommodity(struct DaemonApplication *Self, LONG active);
void  ExitCommodity(struct DaemonApplication *Self);
void  PolymorphCommodityEvent(struct DaemonApplication *Self, APTR message);
void  PolymorphRexxHostEvent(struct DaemonApplication *Self, APTR message);
void  InitRegistration(struct DaemonApplication *Self);
void  ExitRegistration(struct DaemonApplication *Self);

/*
 *	Process Information
*/
STATIC CONST BYTE	DaemonName[]			= "Polymorph-VMM\0";
STATIC CONST ULONG	DaemonStackSize			= 131072UL;
STATIC CONST ULONG	DaemonPriority			= 20UL;
STATIC CONST BYTE	DaemonDescription[]		= "Virtual Environments\0";
STATIC CONST BYTE	DaemonReleaseString[]	= "Open Source Edition";

STATIC CONST BYTE	EnvironmentSettings[]	= "Env:Polymorph/Environment\0";
STATIC CONST BYTE	EnvironmentStorage[]	= "Env:Polymorph/\0";

/*
		InitPerceptionDaemon() is called to launch the PolymorphDaemon Process
*/
void InitPolymorphDaemon(struct LIBRARY_CLASS *SelfBase)
{
	if(SelfBase->IDOS)
		if(SelfBase->DaemonProcess==NULL)
			SelfBase->DaemonProcess=(APTR)SelfBase->IDOS->CreateNewProcTags(
				NP_Entry,		(APTR)&ExecPolymorphDaemon,
				NP_Input,		NULL,			NP_CloseInput,	NULL,
				NP_Output,		NULL,			NP_CloseOutput,	NULL,
				NP_Error,		NULL,			NP_CloseError,	NULL,
				NP_CurrentDir,	NULL,           NP_Name,		DaemonName,
				NP_CommandName,	DaemonName,		NP_Priority,	DaemonPriority,
				NP_StackSize,	DaemonStackSize,NP_ConsoleTask,	NULL,
				NP_WindowPtr,	NULL,			NP_CopyVars,	NULL,
				NP_Cli,			NULL,			NP_Path,		NULL,
				TAG_END,		NULL);
	return;
};

void ExitPolymorphDaemon(struct LIBRARY_CLASS *SelfBase)
{
	return;
};

int32 ExecPolymorphDaemon(STRPTR argv, ULONG argc)
{
	uint32	rc=0L;
    struct ExecIFace *IExec = (struct ExecIFace *)(*(struct ExecBase **)4)->MainInterface;
	struct Library *Base = NULL;

	struct DaemonApplication *hPolymorph=NULL, dApplication;

	ULONG	signals = 0L, sigmask = 0L;
	APTR	message = NULL;

	dApplication.IExec			= (struct ExecIFace *)(*(struct ExecBase **)4)->MainInterface;
	if((Base = (APTR)IExec->OpenLibrary(LIBRARY_NAME, 0L)))
		dApplication.IPolymorph = (APTR)IExec->GetInterface(Base,"main",1L,NULL);
	dApplication.PolymorphBase = (APTR)Base;

	if((Base = (APTR)IExec->OpenLibrary("dos.library", 50L)))
		dApplication.IDOS		 = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

	if((Base = (APTR)IExec->OpenLibrary("commodities.library", 50L)))
		dApplication.ICX		 = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

	if((Base = (APTR)IExec->OpenLibrary("utility.library", 50L)))
		dApplication.IUtility	 = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

	if((Base = (APTR)IExec->OpenLibrary("locale.library", 50L)))
		dApplication.ILocale	 = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

	if((Base = (APTR)IExec->OpenLibrary("rexxsyslib.library", 50L)))
		dApplication.IREXX	 = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

	if((Base = (APTR)IExec->OpenLibrary("application.library", 50L)))
		dApplication.IApplication = (APTR)IExec->GetInterface(Base,"main",2L,NULL);

	if(dApplication.ICX)
		InitCommodity(hPolymorph, POLYMORPH_ENABLED);
	if(dApplication.IApplication)
		InitRegistration(hPolymorph);

	hPolymorph = &dApplication;

	do{
		sigmask = dApplication.cxSignal ;
		signals = IExec->Wait(sigmask);

		if(signals && dApplication.cxSignal)
			while((message=(APTR)IExec->GetMsg(dApplication.cxPort)))
				PolymorphCommodityEvent(hPolymorph,message);

	}while(NULL != hPolymorph);

	if(dApplication.ApplicationID)
		ExitRegistration(&dApplication);
	if(dApplication.cxPort)
		ExitCommodity(&dApplication);

	if(dApplication.IApplication)
	{
		Base = dApplication.IApplication->Data.LibBase;
		IExec->DropInterface((APTR)dApplication.IApplication);
		IExec->CloseLibrary((APTR)Base);
	}

	if(dApplication.IREXX)
	{
		Base = dApplication.IREXX->Data.LibBase;
		IExec->DropInterface((APTR)dApplication.IREXX);
		IExec->CloseLibrary((APTR)Base);
	}

	if(dApplication.ILocale)
	{
		Base = dApplication.ILocale->Data.LibBase;
		IExec->DropInterface((APTR)dApplication.ILocale);
		IExec->CloseLibrary((APTR)Base);
	}

	if(dApplication.IUtility)
	{
		Base = dApplication.IUtility->Data.LibBase;
		IExec->DropInterface((APTR)dApplication.IUtility);
		IExec->CloseLibrary((APTR)Base);
	}

	if(dApplication.ICX)
	{
		Base = dApplication.ICX->Data.LibBase;
		IExec->DropInterface((APTR)dApplication.ICX);
		IExec->CloseLibrary((APTR)Base);
	}

	if(dApplication.IDOS)
	{
		Base = dApplication.IDOS->Data.LibBase;
		IExec->DropInterface((APTR)dApplication.IDOS);
		IExec->CloseLibrary((APTR)Base);
	}

	if(dApplication.IPolymorph)
		IExec->DropInterface((APTR)dApplication.IPolymorph);
	if(dApplication.PolymorphBase)
		IExec->CloseLibrary((APTR)dApplication.PolymorphBase);

	return(rc);
};

void InitCommodity(struct DaemonApplication *Self, LONG active)
{
	LONG	error=0L;
	struct NewBroker NewCommodityDaemon;

	NewCommodityDaemon.nb_Version			= NB_VERSION;
	NewCommodityDaemon.nb_Name				= (APTR)DaemonName;
	NewCommodityDaemon.nb_Title				= (APTR)DaemonName;
	NewCommodityDaemon.nb_Descr				= (APTR)DaemonDescription;
	NewCommodityDaemon.nb_Unique			= 0;
	NewCommodityDaemon.nb_Flags				= 0;
	NewCommodityDaemon.nb_Pri				= 0;
	NewCommodityDaemon.nb_ReservedChannel	= 0;

	if(Self->IExec)
		Self->cxPort =	(APTR)Self->IExec->AllocSysObjectTags( ASOT_PORT,
			ASOPORT_Size,		sizeof(struct MsgPort),
/*			ASOPORT_AllocSig,	TRUE,	*/
/*			ASOPORT_Signal,		NULL,	*/
/*			ASOPORT_Action,		0L,		*/
			ASOPORT_Pri,		0L,
/*			ASOPORT_Name,		NULL,	*/
/*			ASOPORT_CopyName,	NULL,	*/
/*			ASOPORT_Target,		NULL,	*/
/*			ASOPORT_Public,		NULL,	*/
			NULL,				NULL);
	if(Self->cxPort)
		Self->cxSignal=1L << Self->cxPort->mp_SigBit;
	if(Self->ICX)
	{
		NewCommodityDaemon.nb_Port = Self->cxPort;
		Self->CommodityKey=Self->ICX->CxBroker(&NewCommodityDaemon,&error);
	}
	if(Self->CommodityKey)
	{
		Self->ICX->ActivateCxObj(Self->CommodityKey, active);
	}
	return;
}

void ExitCommodity(struct DaemonApplication *Self)
{
	if(Self->CommodityKey)
	{
		Self->ICX->ActivateCxObj(Self->CommodityKey,FALSE);
		Self->ICX->DeleteCxObjAll(Self->CommodityKey);
		Self->CommodityKey=NULL;
	}
	return;
}

/*
	Commodities Event Handling

	Disable	-> Block Launching and Suspend existing PolymorphApplications
	Enable	-> Allow Launching and Resume suspended PolymorphApplications
	Kill	-> Run the Garbage Collector, push suspended PolymorphApplications to disk
	Unique	-> System Breach!, Force GrimRepeaping
*/
void PolymorphCommodityEvent(struct DaemonApplication *Self, APTR message)
{
	if((Self->ICX->CxMsgType(message))==CXM_COMMAND)
		switch(Self->ICX->CxMsgID(message))
		{
			case	CXCMD_DISABLE:		DisableFilter();
				KDEBUG("Polymorph[Commodity::Disable()]\n");	break;
			case	CXCMD_ENABLE:		EnableFilter();
				KDEBUG("Polymorph[Commodity::Enable()]\n");		break;
			case	CXCMD_APPEAR:   	/*ShowConfig();*/
				KDEBUG("Polymorph[Commodity::Appear()]\n");		break;
			case	CXCMD_DISAPPEAR:	/*HideConfig();*/
				KDEBUG("Polymorph[Commodity::Disappear()]\n");	break;
			case	CXCMD_KILL:			/*GarbageCollector();*/
				KDEBUG("Polymorph[Commodity::Kill()]\n");		break;
			case	CXCMD_UNIQUE:		/*GarbageCollector();*/
				KDEBUG("Polymorph[Commodity::Unique()]\n");		break;
			default:
				break;
		}
	return;
}

/**/
void PolymorphRexxHostEvent(struct DaemonApplication *Self, APTR message)
{
	return;
}

/**/
void InitRegistration(struct DaemonApplication *Self)
{
	if(Self->IApplication)
		Self->ApplicationID=Self->IApplication->RegisterApplication((APTR)DaemonName,
			REGAPP_UniqueApplication,		TRUE,
			REGAPP_Hidden,					TRUE,
			REGAPP_CustomPrefsFileName,		EnvironmentSettings,
			REGAPP_ENVDir,					EnvironmentStorage,
			REGAPP_AppIconInfo,				NULL,
			REGAPP_Description,				DaemonDescription,
			NULL,							NULL);
}

void ExitRegistration(struct DaemonApplication *Self)
{
	if(Self->IApplication)
		Self->IApplication->UnregisterApplication(Self->ApplicationID, NULL);
}

/**/
