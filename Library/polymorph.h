#ifndef POLYMORPH_LIBRARY_H
#define POLYMORPH_LIBRARY_H 1
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
#include <rexx/rxslib.h>
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

#define LIBRARY_NAME		"polymorph.library"
#define LIBRARY_IDENTITY	"polymorph.library v1.1"

#define LIBRARY_CLASS		PolymorphBase
#define LIBRARY_VERSION		1
#define LIBRARY_REVISION	2

/*	The Default State of any System patching is to self-bypass and let the user override!
*/
#define	POLYMORPH_ENABLED   TRUE

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
	struct CommoditiesIFace *ICX;
	struct DataTypesIFace *IDataTypes;
	struct RexxSysIFace *IRexxSys;

	APTR	DaemonProcess;
	APTR	DaemonDHandle;

	struct SignalSemaphore	ObjectCacheLock;
	struct List 			ObjectCacheList;
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
extern void  LCALL_Reservation (struct PolymorphIFace *IFace);	// DUMMY FUNCTION
extern APTR  LCALL_ScriptExec(struct PolymorphIFace *IFace, struct RexxMsg * rmsg, STRPTR * result);
extern ULONG LCALL_OptionTagList(struct PolymorphIFace *IFace, struct TagItem *options);
extern APTR  LCALL_NewMemoryBlock(struct PolymorphIFace *IFace, APTR container, ULONG size, ULONG attrs);
extern void  LCALL_EndMemoryBlock(struct PolymorphIFace *IFace, APTR block );
extern ULONG LCALL_GetMemoryBlockA(struct PolymorphIFace *IFace, APTR block, ULONG attr );
extern void  LCALL_SetMemoryBlockA(struct PolymorphIFace *IFace, APTR block, ULONG attr, ULONG option);
extern APTR  LCALL_ObtainApplication(struct PolymorphIFace *IFace, ULONG magic, ULONG options);
extern void  LCALL_ReleaseApplication(struct PolymorphIFace *IFace, APTR application );
extern ULONG LCALL_GetApplicationA(struct PolymorphIFace *IFace, APTR application, ULONG attr );
extern void  LCALL_SetApplicationA(struct PolymorphIFace *IFace, APTR application, ULONG attr, ULONG option);
extern ULONG LCALL_InitApplicationEntryPoint(struct PolymorphIFace *IFace, APTR entrypoint, APTR application);
extern ULONG LCALL_ExecApplicationEntryPoint(struct PolymorphIFace *IFace, APTR entrypoint);
extern ULONG LCALL_LoadApplicationSegments(struct PolymorphIFace *IFace, APTR application, APTR fname );
extern ULONG LCALL_SaveApplicationSegments(struct PolymorphIFace *IFace, APTR application, APTR fname );
extern APTR  LCALL_ObtainEnvironment(struct PolymorphIFace *IFace, ULONG magic, ULONG options );
extern void  LCALL_ReleaseEnvironment(struct PolymorphIFace *IFace, APTR Environment );
extern ULONG LCALL_GetEnvironmentA(struct PolymorphIFace *IFace, APTR env, ULONG attr);
extern void  LCALL_SetEnvironmentA(struct PolymorphIFace *IFace, APTR env, ULONG attr, ULONG option);
extern APTR  LCALL_FindEnvironmentA(struct PolymorphIFace *IFace, ULONG options );
extern APTR  LCALL_FindEnvironmentM(struct PolymorphIFace *IFace, ULONG magic );
extern ULONG LCALL_LoadEnvironment(struct PolymorphIFace *IFace, APTR env );
extern ULONG LCALL_SaveEnvironment(struct PolymorphIFace *IFace, APTR env );
/*emucalls.c*/
extern UBYTE ECALL_ReadOctet(struct ProcessorIFace *IFace, APTR addr);
extern void  ECALL_WriteOctet(struct ProcessorIFace *IFace, APTR addr, UBYTE value);
extern UWORD ECALL_ReadShortBE(struct ProcessorIFace *IFace, APTR addr);
extern void  ECALL_WriteShortBE(struct ProcessorIFace *IFace, APTR addr, UWORD value);
extern UWORD ECALL_ReadShortLE(struct ProcessorIFace *IFace, APTR addr);
extern void  ECALL_WriteShortLE(struct ProcessorIFace *IFace, APTR addr, UWORD value);
extern ULONG ECALL_ReadLongBE(struct ProcessorIFace *IFace, APTR addr);
extern void  ECALL_WriteLongBE(struct ProcessorIFace *IFace, APTR addr, ULONG value);
extern ULONG ECALL_ReadLongLE(struct ProcessorIFace *IFace, APTR addr);
extern void  ECALL_WriteLongLE(struct ProcessorIFace *IFace, APTR addr, ULONG value);
extern APTR  ECALL_ExecShort(struct ProcessorIFace *IFace, USHORT offset);
extern APTR  ECALL_ExecOctet(struct ProcessorIFace *IFace, USHORT offset);
extern APTR  ECALL_ResolveVector(struct ProcessorIFace *IFace);		//	ToDo:Address Map Resolution
extern APTR  ECALL_ExternVector(struct ProcessorIFace *IFace);		//	ToDo:FFI External Calling Convention
/*Daemon.c*/
extern void  InitPolymorphDaemon(struct LIBRARY_CLASS *Self);
extern void  ExitPolymorphDaemon(struct LIBRARY_CLASS *Self);
extern int32 ExecPolymorphDaemon(STRPTR argv, ULONG argc);
/*filter.c*/
extern void  InitPolymorphFilter(struct LIBRARY_CLASS *Self);
extern void  ExitPolymorphFilter(struct LIBRARY_CLASS *Self);
extern void  EnableFilter(void);
extern void  DisableFilter(void);
/*Segments.c*/
extern BPTR	 InitPolymorphSegment(struct LIBRARY_CLASS *Self,APTR name,struct DataTypeHeader *dtype);
extern void  ExitPolymorphSegment(struct LIBRARY_CLASS *Self,BPTR sc);
extern BPTR	 ObtainPolymorphSegment(struct LIBRARY_CLASS *Self,APTR name,struct DataTypeHeader *dtype);
extern void	 ReleasePolymorphSegment(struct LIBRARY_CLASS *Self,BPTR sc);
extern BPTR  LoadPolymorphSegments(struct LIBRARY_CLASS *Self,BPTR sc,BPTR lock);
/*Plugins.c*/
extern APTR  InitPolymorphPlugin(struct LIBRARY_CLASS *Self);
extern APTR  ExitPolymorphPlugin(struct LIBRARY_CLASS *Self);
/*Threads.c*/
extern APTR  FindPolymorphApplication(APTR name);
extern int32 ExecPolymorphApplication(void);
/*messages.c*/

#endif
/*POLYMORPH_LIBRARY_H*/
