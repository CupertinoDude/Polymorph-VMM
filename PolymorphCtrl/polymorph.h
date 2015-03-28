#ifndef POLYMORPH_H
#define POLYMORPH_H
/*
\\		"Polymorph"
*/
#include <exec/types.h>
#include <exec/resident.h>
#include <exec/libraries.h>
#include <exec/semaphores.h>
#include <exec/execbase.h>
#include <exec/interfaces.h>
#include <graphics/gfxbase.h>
#include <intuition/intuition.h>
#include <intuition/screens.h>
#include <intuition/intuitionbase.h>
#include <dos/dos.h>
#include <dos/dosextens.h>

#define __USE_SYSBASE
#define __NOLIBBASE__
#define __NOGLOBALIFACE__

#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/dos.h>
#include <proto/utility.h>

extern APTR SysBase;
extern APTR GfxBase;
extern APTR IntuitionBase;
extern APTR DOSBase;
extern APTR UtilityBase;

extern struct ExecIFace *IExec;
extern struct GraphicsIFace *IGfx;
extern struct IntuitionIFace *IIntuition;
extern struct DOSIFace *IDOS;
extern struct UtilityIFace *IUtility;

/*main.c*/
int main(void);
/**/

#endif
/*POLYMORPH_H*/
