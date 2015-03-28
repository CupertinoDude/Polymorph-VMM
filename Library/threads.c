/*
\\		"Polymorph"
*/
#include "polymorph.h"

/**/
APTR FindPolymorphApplication(APTR name)
{
	APTR rc=NULL;
	return(rc);
}

/*
	This is the *raw* EntryPoint whenever any "Polymorph Wrapped" Data Object begins Execution

	AmigaDOS hands execution to this function *directly* and it is mandatory to perform special
		magic handling for pulling data from the first segment of the chain and processing further

	Pure Re-Entrant Code is an absolute PRIORITY!!! ...

	Segments behaviour and caching is currently UNDEFINED BEHAVIOUR SUBJECT TO CHANGE WITHOUT NOTICE!
*/
int32 ExecPolymorphApplication(void)
{
	uint32	rc=0L;
    struct ExecIFace *IExec = (struct ExecIFace *)(*(struct ExecBase **)4)->MainInterface;
	struct Library *Base = NULL;

	struct PolymorphApplication *hApplication=NULL, pApplication;

	pApplication.IExec			= (struct ExecIFace *)(*(struct ExecBase **)4)->MainInterface;
	if((Base = (APTR)IExec->OpenLibrary(LIBRARY_NAME, 0L)))
		pApplication.IPolymorph = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

	if((Base = (APTR)IExec->OpenLibrary("dos.library", 50L)))
		pApplication.IDOS		 = (APTR)IExec->GetInterface(Base,"main",1L,NULL);

	hApplication = &pApplication;

	do{
	}while(hApplication!=NULL);

	if(pApplication.IProcessor)
		IExec->DropInterface((APTR)pApplication.IProcessor);

	if(pApplication.IPolymorph)
	{
		Base = pApplication.IPolymorph->Data.LibBase;
		IExec->DropInterface((APTR)pApplication.IPolymorph);
		IExec->CloseLibrary((APTR)Base);
	}

	if(pApplication.IDOS)
	{
		Base = pApplication.IDOS->Data.LibBase;
		IExec->DropInterface((APTR)pApplication.IDOS);
		IExec->CloseLibrary((APTR)Base);
	}

	if(pApplication.IIntuition)
	{
		Base = pApplication.IIntuition->Data.LibBase;
		IExec->DropInterface((APTR)pApplication.IIntuition);
		IExec->CloseLibrary((APTR)Base);
	}

	if(pApplication.IGfx)
	{
		Base = pApplication.IGfx->Data.LibBase;
		IExec->DropInterface((APTR)pApplication.IGfx);
		IExec->CloseLibrary((APTR)Base);
	}

	return(rc);
}

/**/
