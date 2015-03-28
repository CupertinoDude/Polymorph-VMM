#ifndef PLUGIN_H
#define PLUGIN_H
/*
\\		"Polymorph"
*/
#include <exec/types.h>
#include <exec/resident.h>
#include <exec/ports.h>
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
#include <utility/tagitem.h>
#include <utility/hooks.h>
#include <rexx/storage.h>

#include <libraries/application.h>
#include <libraries/polymorph.h>

#define __USE_SYSBASE
#define __NOLIBBASE__
#define __NOGLOBALIFACE__

#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/dos.h>
#include <proto/commodities.h>
#include <proto/locale.h>
#include <proto/utility.h>
#include <proto/datatypes.h>
#include <proto/rexxsyslib.h>
#include <proto/application.h>

#include <interfaces/exec.h>
#include <interfaces/graphics.h>
#include <interfaces/intuition.h>
#include <interfaces/dos.h>
#include <interfaces/commodities.h>
#include <interfaces/locale.h>
#include <interfaces/utility.h>
#include <interfaces/datatypes.h>
#include <interfaces/rexxsyslib.h>
#include <interfaces/application.h>

#include <interfaces/polymorph.h>
/*
\\	KDEBUG() macro

	Usage = KDEBUG("At line [%lld] address [%lld]\n", __LINE__, address)
*/
#ifndef KDEBUG
#define KDEBUG(format, argv...) \
	((struct ExecIFace *)((*(struct ExecBase **)4)->MainInterface))->DebugPrintF(format, ## argv)
#endif

/*
\\	"Polymorph"		Developer Materials
*/
#define LIBRARY_NAME		"javavm.polymorph"
#define LIBRARY_IDENTITY	"Polymorph::JavaVM v1.1"

#define LIBRARY_CLASS		PluginBase
#define LIBRARY_VERSION		1
#define LIBRARY_REVISION	1

struct LIBRARY_CLASS
{
	struct Library Library;
	BPTR SegmentList;
	struct SignalSemaphore Lock;

	struct ExecIFace *IExec;
	struct GraphicsIFace *IGfx;
	struct IntuitionIFace *IIntuition;
	struct DOSIFace *IDOS;
	struct UtilityIFace *IUtility;
	struct LocaleIFace *ILocale;
};

/*main.c*/
STATIC CONST struct Resident RomTag;
STATIC CONST BYTE LibName[];
STATIC CONST BYTE LibIdentity[];
extern CONST struct TagItem InitTable[];
extern CONST APTR LibManagerTable[];
extern CONST struct TagItem LibManagerTags[];
/*apicalls.c*/
extern CONST APTR LibInterfaces[];
extern CONST APTR LibInterfaceTable[];
extern CONST struct TagItem LibInterfaceTags[];
/*emucalls.c*/
extern CONST APTR EmulationTable[];
extern CONST struct TagItem LibEmulationTags[];

/*main.c*/
extern int32 _start(void);
extern struct library *LCALL_Init(struct LIBRARY_CLASS *Self,
				 BPTR SegmentList,
				 struct Interface *Executive);
extern struct Library *LCALL_Open(struct LibraryManagerInterface *Iface, ULONG version);
extern APTR  LCALL_Close(struct LibraryManagerInterface *Iface);
extern APTR  LCALL_Expunge(struct LibraryManagerInterface *Iface);
extern APTR  LCALL_Reserved(void);
extern ULONG MCALL_Obtain(struct Interface *Iface);
extern ULONG MCALL_Release(struct Interface *Iface);
/*apicalls.c*/
extern APTR DCALL_GetDataTypeClass(void);
/*emucalls.c*/
extern void ECALL_Reserved(void);
/**/

#endif
/*PLUGIN_H*/
