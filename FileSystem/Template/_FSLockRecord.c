/****************************************************************************/ 
/*  _FSLockRecord.c
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


int32 FSLockRecord(struct FSVP *vp UNUSED, int32 *res2, struct FileHandle *f UNUSED, 
                    int64 oset UNUSED, int64 len UNUSED, uint32 m UNUSED, uint32 tot UNUSED) 
{
	(*res2) = ERROR_ACTION_NOT_KNOWN; /* makes dos emulate this */ 
	return(FALSE);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_LOCK_RECORD64 -  FSLockRecord()

   SYNOPSIS
	int32 RESULT1 = FSLockRecord(struct FileSystemVectorPort *fsvp,
	                             int32 *result2, struct FileHandle *file,
	                             int64 offset, int64 length, 
	                             uint32 mode,  uint32 timeout);

   FUNCTION
	For filesystems, this function is optional, it would generally be
	implemented only for networked filesystems.
	From dos.library 53.102, this function will be emulated if you supply
	a NULL pointer for FSLockRecord(), or preferably reject the action by
	returning FALSE and result2=ERROR_ACTION_NOT_KNOWN to avoid the DOS
	library dospacket fallback being tried.

	This function locks an area of a file in either a sharable
	(indicating read-only) or exclusive (indicating read/write) mode.
	Several sharable record locks from different file handles can exist
	simultaneously on a particular file area but only one file handle can
	have exclusive record locks on a particular area at a time. 

	The exclusivity of an exclusive file lock only applies to record locks
	from other file handles, not to record locks with the same file handle.
	One file handle can have any number of overlapping exclusive or shared
	record locks.

	'offset' is the start position (in bytes) of record in the file.

	'length' is the size (in bytes) of the record to be locked.

	'mode'   is one of the following:
	    REC_EXCLUSIVE       (0) = Exclusive
	    REC_EXCLUSIVE_IMMED (1) = Immediate Exclusive (timeout is ignored)
	    REC_SHARED          (2) = Shared
	    REC_SHARED_IMMED    (3) = Immediate Shared (timeout is ignored)
	         
	'timeout' is the period (in the number of AmigaDOS ticks @ 1/50 second)
	to wait for success, before failing the operation.


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	file    - (struct   FileHandle *) Pointer to the open filehandle.
	offset  - (int64)   Byte offset into file of record start.
	length  - (int64)   Byte length of the record.
	mode    - (uint32)  Mode for locking record.
	timeout - (uint32)  Number of ticks to wait before failing.
	RESULT1 - (int32)   Boolean Success/Failure code, FALSE on error.
	RESULT2 - (int32)   Failure code if RESULT1 == FALSE.

   INPUTS    (DosPacket64 method)
	dp_Type - (int32)  ACTION_LOCK_RECORD64
	dp_Arg1 - (struct  FileHandle *) file
	dp_Arg2 - (int64)  offset
	dp_Arg3 - (int32)  mode
	dp_Arg4 - (int32)  timeout
	dp_Arg5 - (int64)  length
	RESULT1 - (int64)  Success/Failure (DOSTRUE/FALSE)
	RESULT2 - (int32)  Failure code if RESULT1 == FALSE


   NOTES
	For old filesystems updating to the new 64 bit DosPacket method,
	be warned that besides the argument size difference, the argument
	order has also been changed.

	DOS callers to OBSOLETELockRecord32() will be redirected to the new
	function slot if the old dospacket is rejected by returning FALSE 
	and result2=ERROR_ACTION_NOT_KNOWN.
 
   SEE ALSO
	IDOS->LockRecord()



**/ 


/****************************************************************************/ 
/* EOF */ 



