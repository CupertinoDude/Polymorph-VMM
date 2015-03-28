#ifndef TEMPLATE_DEVICE_H
#define TEMPLATE_DEVICE_H 1
/*
\\		"Template Device Driver"
*/
#include <exec/types.h>
#include <exec/resident.h>
#include <exec/ports.h>
#include <exec/libraries.h>
#include <exec/semaphores.h>
#include <exec/execbase.h>
#include <exec/interfaces.h>

#define __USE_SYSBASE
#define __NOLIBBASE__
#define __NOGLOBALIFACE__

#include <proto/exec.h>

#include <interfaces/exec.h>

/*
\\	KDEBUG() macro

	Usage = KDEBUG("At line [%lld] address [%lld]\n", __LINE__, address)
*/
#ifndef KDEBUG
#define KDEBUG(format, argv...) \
	((struct ExecIFace *)((*(struct ExecBase **)4)->MainInterface))->DebugPrintF(format, ## argv)
#endif

/*
\\	Developer Materials
*/
#define DEVICE_NAME			"template.device"
#define DEVICE_IDENTITY		"template.device v1.1"

#define DEVICE_CLASS		TemplateBase
#define DEVICE_VERSION		1
#define DEVICE_REVISION		1

struct DEVICE_CLASS
{
	struct	Library	Library;
	APTR	SegmentList;
	struct	SignalSemaphore	Lock;
	struct	ExecIFace *IExec;
};

/*	Published Internal Global Data
*/
STATIC CONST struct Resident RomTag;
STATIC CONST BYTE DevName[];
STATIC CONST BYTE DevIdentity[];
extern CONST struct TagItem InitTable[];
extern CONST APTR DevInterfaces[];
extern CONST APTR DevManagerTable[];
extern CONST struct TagItem DevManagerTags[];
extern CONST APTR DevInterfaceTable[];
extern CONST struct TagItem DevInterfaceTags[];

/*main.c*/
extern int32 _start(void);
extern struct library *DCALL_Init(struct DEVICE_CLASS *SelfBase,
				 APTR SegmentList,
				 struct Interface *Executive);
extern struct Library *DCALL_Open(struct LibraryManagerInterface *Self, ULONG version);
extern APTR  DCALL_Close(struct LibraryManagerInterface *Self);
extern APTR  DCALL_Expunge(struct LibraryManagerInterface *Self);
extern APTR  DCALL_Reserved(void);
extern ULONG MCALL_Obtain(struct Interface *Self);
extern ULONG MCALL_Release(struct Interface *Self);
/*apicalls.c*/
extern APTR  DCALL_BeginIO(struct Interface *Self,struct IORequest *iov);
extern APTR  DCALL_AbortIO(struct Interface *Self,struct IORequest *iov);
/**/
/*TEMPLATE_DEVICE_H*/
#endif
/**/
