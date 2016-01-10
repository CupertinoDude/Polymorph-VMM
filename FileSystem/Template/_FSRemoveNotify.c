/****************************************************************************/ 
/*  _FSRemoveNotify.c
**       
**  Created by FileSystem VectorPort tool. (FSVPTool 53.23) 
**       
**  :ts=4 
**       
*/       
/****************************************************************************/ 


#include "all_includes.h" 


/****************************************************************************/ 


/* 
** ==================================================================
**  This functions is optional, dos.library will emulate the full 
**  functionality of this when the vector-port function responds  
**  with FALSE, and with the secondary code of ERROR_ACTION_NOT_KNOWN.
**  This function only need be implemented by remote filesystems. 
** ==================================================================
*/ 


int32 FSRemoveNotify(struct FSVP *vp UNUSED, int32 *res2, struct NotifyRequest *nr UNUSED)
{
	(*res2) = ERROR_ACTION_NOT_KNOWN; /* makes dos emulate this */ 
	return(FALSE);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_REMOVE_NOTIFY - FSRemoveNotify()
	                       (OPTIONAL - Emulated by DOS V52.28+)

   SYNOPSIS
	int32 RESULT1 = FSRemoveNotify(struct FileSystemVectorPort *fsvp,
	                            int32 *result2, 
	                            struct NotifyRequest *nreq);

   FUNCTION
	This optional action cancels a notification (see ACTION_ADD_NOTIFY).
	the notifyrequest argument is the NotifyRequest structure used to start
	the notification, it does not delete the actual NotifyRequest structure,
	See FreeDosObject() for this.

	The handler should abandon any pending notification messages.
	Note that it is possible for a file system to receive a reply from
	a previously sent notification message even after the notification
	has been terminated.
	It should accept these messages silently and throw them away.


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	nreq    - (struct NotifyRequest *) Pointer to the notify request.
	RESULT1 - (int32) Success/failure (DOSTRUE/FALSE), FALSE on error.
	RESULT2 - (int32) Failure code if RESULT1 == FALSE


   INPUTS    (DosPacket method)
	dp_Type - (int32) ACTION_REMOVE_NOTIFY
	dp_Arg1 - (struct NotifyRequest *) notifyrequest.
	RESULT1 - (int32) Success/Failure (DOSTRUE/FALSE), FALSE on error.
	RESULT2 - (int32) Failure code if RESULT1 == FALSE


   SEE ALSO
	IDOS->EndNotify()



**/ 


/****************************************************************************/ 
/* EOF */ 



