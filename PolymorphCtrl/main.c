/*
\\		"Polymorph"
//
\\		Virtual Machine and Emulator Stack
*/
#include "polymorph.h"

APTR	PolymorphBase=NULL;

int main(void)
{
	int rc=0L;

	PolymorphBase=(APTR)IExec->OpenLibrary("polymorph.library",0L);
	IDOS->Delay(30L);
	IExec->CloseLibrary(PolymorphBase);

	return(rc);
}
