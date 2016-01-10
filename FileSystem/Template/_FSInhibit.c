/****************************************************************************/ 
/*  _FSInhibit.c
**       
**  Created by FileSystem VectorPort tool. (FSVPTool 53.23) 
**       
**  :ts=4 
**       
*/       
/****************************************************************************/ 


#include "all_includes.h" 


/****************************************************************************/ 



static int32 do_inhibit(APTR dummy UNUSED,...) 
{
	return 0; /* Write me ! */ 
}



/****************************************************************************/ 


int32 FSInhibit(struct FSVP *vp, int32 *res2, int32 state)
{
	struct GlobalData *gd = vp->FSV.FSPrivate;
	int32 result;

	/* 
	**  Request exclusive access for FSInhibit(). 
	**  This makes us wait for all callers to 
	**  finish their current access before we do it. 
	*/ 
	IEXEC->ObtainSemaphore(gd->Sem);

	result = do_inhibit(gd, res2, state);

	IEXEC->ReleaseSemaphore(gd->Sem);

	return(result);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_INHIBIT - FSInhibit()

   SYNOPSIS
	int32 RESULT1 = FSInhibit(struct FileSystemVectorPort *fsvp,
	                          int32 *result2, 
	                          int32  state);

   FUNCTION
	This action is probably one of the most dangerous that a file system
	has to handle.

	When inhibiting succeeds, (state = TRUTH), the file system must not
	access any underlying media and must return the error code;
	ERROR_OBJECT_IN_USE on all actions that attempt to access the device.

	The file system must flush all buffers before the inhibit is enacted,
	popping up a requester demanding that the user put back the current
	disk, if necessary, and attempt to complete a pending write request.
	Alternatively, a file system may simply reject an inhibit request if
	any files are currently open.
	The way this function is implemented is filesystem design dependant,
	but in all cases, access to the underlying media must be prevented
	after an inhibit request has been enacted.

	If an inhibit request cannot be enacted, this function shall return 
	FALSE instead of DOSTRUE.

	Once uninhibited (state = FALSE), the file system MUST assume that the
	medium has been changed and generate the appropriate diskchange events,
	along with validating the medium again.

	When uninhibiting, the primary RESULT1 value shall always be DOSTRUE,
	there shall be no impediment or failure possible when uninhibiting.

	A handler must also nest inhibits.
	Prior to 2.0, the system handlers did not keep a nesting count and
	were subject to some obscure race conditions.
	The 2.0 ROM filing system introduced a nesting count.


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	state   - (int32) Inhibit state, (DOSTRUE/FALSE).
	RESULT1 - (int32) Success/failure (DOSTRUE/FALSE), FALSE on error.
	RESULT2 - (int32) Failure code if RESULT1 == FALSE


   INPUTS    (DosPacket method)
	dp_Type - (int32) ACTION_INHIBIT
	dp_Arg1 - (int32) DOSTRUE = inhibit,  FALSE = uninhibit.
	RESULT1 - (int32) Success/failure (DOSTRUE/FALSE), FALSE on error.
	RESULT2 - (int32) Failure code if RESULT1 == FALSE


   SEE ALSO
	IDOS->Inhibit(), NonBlockingModifyDosEntry().



**/ 


/****************************************************************************/ 
/* EOF */ 



