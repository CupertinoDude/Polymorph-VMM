/****************************************************************************/ 
/*  _FSDupLockFromFH.c
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
**  New objects must inherit owner and group attributes from the     
**  parent directory, they must have a current creation timestamp,   
**  the protection mask must be set to EXDF_DEFAULT (0), and the     
**  comment strings shall default to a nul-string.                   
** ==================================================================
*/ 


struct Lock * FSDupLockFromFH(struct FSVP *vp, int32 *res2, struct FileHandle *file)
{
	struct Lock *result;

	if( file )
	{
		result = FSDupLock(vp, res2, file->fh_Arg2);
	}
	else
	{
		(*res2) = ERROR_REQUIRED_ARG_MISSING;
		result  = NULL;
	}

	return(result);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_COPY_LOCK_FH  -  FSDupLockFromFH()

   SYNOPSIS
	struct Lock *RESULT1 = FSDupLockFromFH(struct FileSystemVectorPort *vp,
	                          int32 *result2, struct FileHandle *file);

   FUNCTION
	This actions original name (ACTION_COPY_DIR_FH) was misleading as it
	does not actually manipulate directories, instead, it creates a shared
	lock on an open file. ACTION_COPY_LOCK_FH should only be used now.

	This function requests that the handler return a lock associated with
	the currently opened file handle.  The request may fail for any
	restriction imposed by the file system (for example when the file
	handle is not opened in a shared mode).  

	The file handle is still usable after this call, unlike the lock
	that is consumed with ACTION_FH_FROM_LOCK / FSOpenFromLock().

	The RESULT1 lock remains valid even after the file is closed.


   INPUTS    (FileSystemVectorPort method)
	vp      - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	file    - (struct FileHandle *) Pointer to the open file handle.
	RESULT1 - (struct Lock *) Lock associated with file handle, 0 on error.
	RESULT2 - (int32) Failure code if RESULT1 == NULL.


   INPUTS    (DosPacket method)
	dp_Type - (int32) ACTION_COPY_LOCK_FH.
	dp_Arg1 - (APTR)  filehandle->fh_Arg1.
	dp_Arg2 - (struct FileHandle *)filehandle. (added 53.49, previously 0)
	RESULT1 - (BPTR)  Lock associated with file handle or ZERO on error.
	RESULT2 - (int32) Failure code if RESULT1 == ZERO


   SEE ALSO
	IDOS->DupLockFromFH()



**/ 


/****************************************************************************/ 
/* EOF */ 



