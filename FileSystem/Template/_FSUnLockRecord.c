/****************************************************************************/ 
/*  _FSUnLockRecord.c
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


int32 FSUnLockRecord(struct FSVP *vp UNUSED, int32 *res2, struct FileHandle *file UNUSED,
                      int64 offset UNUSED, int64 length UNUSED)
{
	(*res2) = ERROR_ACTION_NOT_KNOWN; /* makes dos emulate this */ 
	return(FALSE);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_FREE_RECORD64 - FSUnLockRecord()

   SYNOPSIS
	int32 RESULT1 = FSUnLockRecord(struct FileSystemVectorPort *fsvp,
	                               int32 *result2, struct FileHandle *file,
	                               int64 offset, int64 length);

   FUNCTION
	For filesystems, this function is optional, it would generally be
	implemented only for networked filesystems.

	From dos.library 53.102, this function will be emulated if you supply
	a NULL pointer for FSUnLockRecord(), or reject the action with FALSE
	and result2=ERROR_ACTION_NOT_KNOWN. (The prefered method.)

	This function unlocks any previous record lock.  
	If the given range does not represent one that is currently locked in
	the file, this action returns FALSE along with ERROR_RECORD_NOT_LOCKED.
	In the event of multiple locks on a given area, all locks made by the
	same filehandle specified are freed.

	'offset' is the start position (in bytes) of the record in the file.
	'length' is the size (in bytes) of the record to be unlocked.


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	file    - (struct FileHandle *) Pointer to the open filehandle.
	offset  - (int64) Byte offset into file of record start.
	length  - (int64) Byte length of the record.
	RESULT1 - (int32) Boolean Success/Failure code, FALSE on error.
	RESULT2 - (int32) Failure code if RESULT1 == FALSE.

   INPUTS    (DosPacket64 method)
	dp_Type - (int32) ACTION_FREE_RECORD64
	dp_Arg1 - (struct FileHandle *) file
	dp_Arg2 - (int64) offset
	dp_Arg3 - (int32) 0
	dp_Arg4 - (int32) 0
	dp_Arg5 - (int64) length
	RESULT1 - (int64) Success/Failure (DOSTRUE/FALSE)
	RESULT2 - (int32) Failure code if RESULT1 == FALSE


   NOTES
	For old filesystems updating to the new 64 bit DosPacket method,
	be warned that besides the argument size difference, the argument 
	order has also been changed.

	DOS callers to OBSOLETEUnLockRecord32() will be redirected to the new
	function slot if the old packet is rejected.

   SEE ALSO
	IDOS->UnLockRecord()



**/ 


/****************************************************************************/ 
/* EOF */ 



