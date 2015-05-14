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
//
	struct	ExecIFace			*IExec;
	struct  PolymorphIFace		*IPolymorph;
	struct	DOSIFace			*IDOS;
	struct	CommoditiesIFace	*ICX;
	struct	UtilityIFace		*IUtility;
	struct	LocaleIFace			*ILocale;
	struct	RexxSysIFace		*IREXX;
	struct	ApplicationIFace	*IApplication;
//
	struct	MsgPort				*cxPort;
	ULONG						cxSignal;
	APTR						CommodityKey;
	UWORD						CommodityFlags;
//
	ULONG						hApplication;
	struct	ApplicationIconInfo *hApplicationInfo;
//
	APTR						hLocale;
	APTR						hLocaleDB;
//
	struct	MsgPort				*rxPort;
	ULONG						rxSignal;
//
};

void  InitCommodity(struct DaemonApplication *Self, LONG active);
void  ExitCommodity(struct DaemonApplication *Self);
ULONG PolymorphCommodityEvent(struct DaemonApplication *Self, APTR message);
void  InitApplication(struct DaemonApplication *Self);
void  ExitApplication(struct DaemonApplication *Self);
void  InitLocalization(struct DaemonApplication *Self);
void  ExitLocalization(struct DaemonApplication *Self);
void  InitRexxHost(struct DaemonApplication *Self);
void  ExitRexxHost(struct DaemonApplication *Self);
ULONG PolymorphRexxHostEvent(struct DaemonApplication *Self, APTR message);

/*
 *	Process Information
*/
STATIC CONST BYTE	DaemonName[]			= "Polymorph-VMM\0";
STATIC CONST ULONG	DaemonStackSize			= 131072UL;
STATIC CONST ULONG	DaemonPriority			= 20UL;
STATIC CONST BYTE	DaemonDescription[]		= "Virtual Environments\0";
STATIC CONST BYTE	DaemonReleaseString[]	= "Open Source Edition";
STATIC CONST BYTE	DaemonConfiguration[]	= "Polymorph-VMM\0";
STATIC CONST BYTE	DaemonEnvironment[]		= "Polymorph\0\0";
STATIC CONST BYTE   DaemonLocale[]			= "english";

/*	Commodity Flag markers
*/
#define POLYMORPH_STATE_ACTIVE     0x1000

/*
		InitPolymorphDaemon() is called to launch the PolymorphDaemon Process
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

/**/
void ExitPolymorphDaemon(struct LIBRARY_CLASS *SelfBase)
{
	return;
};

/**/
int32 ExecPolymorphDaemon(STRPTR argv, ULONG argc)
{
	uint32	rc=0L;
    struct ExecIFace *IExec = (struct ExecIFace *)(*(struct ExecBase **)4)->MainInterface;
	struct Library *Base = NULL;
	struct DaemonApplication *dApplication=NULL;

	ULONG	signals = 0L, sigmask = 0L;
	APTR	message = NULL;

	dApplication=IExec->AllocVecTags(sizeof(struct DaemonApplication),
		AVT_Type,MEMF_SHARED,AVT_ClearWithValue,0L,TAG_DONE);
	if(dApplication)
	{
		if((Base = (APTR)IExec->OpenLibrary(LIBRARY_NAME, 0L)))
			dApplication->PolymorphBase = (APTR)Base;
		dApplication->IExec			= (struct ExecIFace *)(*(struct ExecBase **)4)->MainInterface;
		if(dApplication->PolymorphBase)
			dApplication->IPolymorph = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

		if((Base = (APTR)IExec->OpenLibrary("dos.library", 50L)))
			dApplication->IDOS		 = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

		if((Base = (APTR)IExec->OpenLibrary("commodities.library", 50L)))
			dApplication->ICX		 = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

		if((Base = (APTR)IExec->OpenLibrary("utility.library", 50L)))
			dApplication->IUtility	 = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

		if((Base = (APTR)IExec->OpenLibrary("application.library", 50L)))
			dApplication->IApplication = (APTR)IExec->GetInterface(Base,"main",2L,NULL);

		if((Base = (APTR)IExec->OpenLibrary("locale.library", 50L)))
			dApplication->ILocale	 = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

		if((Base = (APTR)IExec->OpenLibrary("rexxsyslib.library", 0L)))
			dApplication->IREXX		 = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

		if(dApplication->ICX)
			InitCommodity(dApplication,TRUE);

		if(dApplication->IApplication)
			InitApplication(dApplication);

		if(dApplication->ILocale)
			InitLocalization(dApplication);

		if(dApplication->IREXX)
			InitRexxHost(dApplication);

		do{
			sigmask = dApplication->ioSignal | dApplication->cxSignal | dApplication->rxSignal;
			signals = IExec->Wait(sigmask);

			if(signals && dApplication->cxSignal)
				while((message=(APTR)IExec->GetMsg(dApplication->cxPort)))
					exit=PolymorphCommodityEvent(dApplication,message);
			if(signals && dApplication->rxSignal)
				while((message=(APTR)IExec->GetMsg(dApplication->rxPort)))
					exit=PolymorphRexxHostEvent(dApplication,message);
		}while(!exit);

		ExitInputHandler(dApplication);

		ExitRexxHost(dApplication);
		ExitLocalization(dApplication);
		ExitApplication(dApplication);
		ExitCommodity(dApplication);

		if(dApplication->IREXX)
		{
			Base = dApplication->IREXX->Data.LibBase;
			IExec->DropInterface((APTR)dApplication->IREXX);
			IExec->CloseLibrary((APTR)Base);
		}

		if(dApplication->ILocale)
		{
			Base = dApplication->ILocale->Data.LibBase;
			IExec->DropInterface((APTR)dApplication->ILocale);
			IExec->CloseLibrary((APTR)Base);
		}

		if(dApplication->IApplication)
		{
			Base = dApplication->IApplication->Data.LibBase;
			IExec->DropInterface((APTR)dApplication->IApplication);
			IExec->CloseLibrary((APTR)Base);
		}

		if(dApplication->IUtility)
		{
			Base = dApplication->IUtility->Data.LibBase;
			IExec->DropInterface((APTR)dApplication->IUtility);
			IExec->CloseLibrary((APTR)Base);
		}

		if(dApplication->ICX)
		{
			Base = dApplication->ICX->Data.LibBase;
			IExec->DropInterface((APTR)dApplication->ICX);
			IExec->CloseLibrary((APTR)Base);
		}

		if(dApplication->IDOS)
		{
			Base = dApplication->IDOS->Data.LibBase;
			IExec->DropInterface((APTR)dApplication->IDOS);
			IExec->CloseLibrary((APTR)Base);
		}

		if(dApplication->IPolymorph)
			IExec->DropInterface((APTR)dApplication->IPolymorph);
		if(dApplication->PolymorphBase)
			IExec->CloseLibrary((APTR)dApplication->PolymorphBase);

        IExec->FreeVec(dApplication);
	}
	return(rc);
};

/**/
void InitCommodity(struct DaemonApplication *Self,LONG active)
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
			ASOPORT_Pri,		0L,
			ASOPORT_Name,		DaemonName,
			ASOPORT_Public,		FALSE,
			TAG_END,			NULL);
	if(Self->cxPort)
		Self->cxSignal=1L << Self->cxPort->mp_SigBit;
	if(Self->cxSignal)
		NewCommodityDaemon.nb_Port = Self->cxPort;
	Self->CommodityKey=Self->ICX->CxBroker(&NewCommodityDaemon,&error);
	if(Self->CommodityKey)
		Self->ICX->ActivateCxObj(Self->CommodityKey, active);
	if(active)
		Self->CommodityFlags = Self->CommodityFlags | POLYMORPH_STATE_ACTIVE;
	return;
}

/**/
void ExitCommodity(struct DaemonApplication *Self)
{
	if(Self->CommodityKey)
	{
		Self->ICX->ActivateCxObj(Self->CommodityKey,TRUE);
		Self->ICX->DeleteCxObjAll(Self->CommodityKey);
		Self->CommodityKey=NULL;
	}
	return;
}

/*
//	Commodities Event Handling

	Disable	-> Block Launching and Suspend existing PolymorphApplications
	Enable	-> Allow Launching and Resume suspended PolymorphApplications
	Kill	-> Run the Garbage Collector, push suspended PolymorphApplications to disk
	Unique	-> System Breach!, Force GrimRepeaping
*/
void PolymorphCommodityEvent(struct DaemonApplication *Self, APTR message)
{
	ULONG rc=0L;
	if((Self->ICX->CxMsgType(message))==CXM_COMMAND)
		switch(Self->ICX->CxMsgID(message))
		{
			case	CXCMD_DISABLE:
				Self->CommodityFlags = Self->CommodityFlags & (!POLYMORPH_STATE_ACTIVE);
				break;
			case	CXCMD_ENABLE:
				Self->CommodityFlags = Self->CommodityFlags | POLYMORPH_STATE_ACTIVE;
				break;
			case	CXCMD_APPEAR:   	/*ShowConfig();*/
				KDEBUG("Polymorph[Commodity::Appear()]\n");
				break;
			case	CXCMD_DISAPPEAR:	/*HideConfig();*/
				KDEBUG("Polymorph[Commodity::Disappear()]\n");
				break;
			case	CXCMD_KILL:			/*GarbageCollector();*/
				KDEBUG("Polymorph[Commodity::Kill()]\n");
				rc=TRUE;
				break;
			case	CXCMD_UNIQUE:		/*GarbageCollector();*/
				KDEBUG("Polymorph[Commodity::Unique()]\n");
				break;
			case	CXCMD_KILL:			/* External then Internal */
				break;
			case	CXCMD_UNIQUE:		/* Internal Special */
				break;
			default:
				break;
		}
	return(rc);
}

/**/
void  InitApplication(struct DaemonApplication *Self)
{
	struct ApplicationIconInfo *info=NULL;
    info=Self->IExec->AllocVecTags(sizeof(struct ApplicationIconInfo),
		AVT_Type,			MEMF_SHARED,
		AVT_ClearWithValue,	0L,
		TAG_DONE,			0L);
	if(info)
	{
		info->iconType=APPICONT_Docky;
		Self->hApplicationInfo=info;
	}

	Self->hApplication=Self->IApplication->RegisterApplication((APTR)DaemonName,
		REGAPP_UniqueApplication,	TRUE,
		REGAPP_LoadPrefs,			TRUE,
		REGAPP_SavePrefs,			TRUE,
		REGAPP_CustomPrefsFileName,	DaemonConfiguration,
		REGAPP_ENVDir,				DaemonEnvironment,
		REGAPP_Hidden,				FALSE,
		REGAPP_AppIconInfo,			info,
		REGAPP_Description,			DaemonDescription,
		TAG_END,					NULL);
}

/**/
void  ExitApplication(struct DaemonApplication *Self)
{
	Self->IApplication->UnregisterApplication(Self->hApplication,0L);
}

/**/
void  InitLocalization(struct DaemonApplication *Self)
{
	Self->hLocale	= Self->ILocale->OpenLocale(NULL);
	if(Self->hLocale)
		Self->hLocaleDB	= Self->ILocale->OpenCatalog(Self->hLocale, "Polymorph",
			OC_BuiltInLanguage, (LONG)DaemonLocale,
			OC_BuiltInCodeSet,	106L,
			OC_PreferExternal,	TRUE,
			TAG_END,			NULL);
	return;
}

/**/
void  ExitLocalization(struct DaemonApplication *Self)
{
	if(Self->hLocaleDB)
		Self->ILocale->CloseCatalog(Self->hLocaleDB);
	if(Self->hLocale)
		Self->ILocale->CloseLocale(Self->hLocale);
	return;
}

/**/
void  InitRexxHost(struct DaemonApplication *Self)
{
	if(Self->IExec)
		Self->rxPort =	(APTR)Self->IExec->AllocSysObjectTags( ASOT_PORT,
			ASOPORT_Size,		sizeof(struct MsgPort),
			ASOPORT_Pri,		0L,
			ASOPORT_Name,		DaemonName,
			ASOPORT_Public,		TRUE,
			TAG_END,			NULL);
	if(Self->rxPort)
		Self->rxSignal=1L << Self->rxPort->mp_SigBit;
	return;
}

/**/
void  ExitRexxHost(struct DaemonApplication *Self)
{
	if(Self->rxPort)
    	Self->IExec->FreeSysObject( ASOT_PORT, Self->rxPort );
	return;
}

/**/
ULONG PolymorphRexxHostEvent(struct DaemonApplication *Self, APTR message)
{
	ULONG rc=0L;
	return(rc);
}

/**/
