/****************************************************************************/ 
/*  _FSGetFilePosition.c
**       
**  Created by FileSystem VectorPort tool. (FSVPTool 53.23) 
**       
**  :ts=4 
**       
*/       
/****************************************************************************/ 


#include "all_includes.h" 


/****************************************************************************/ 



int64 FSGetFilePosition(struct FSVP *vp, int32 *res2, struct FileHandle *file)
{
	struct GlobalData *gd = vp->FSV.FSPrivate;
	struct ObjLock *lock;
	int64  result = -1LL;

	if( file )
	{
		IEXEC->ObtainSemaphoreShared(gd->Sem);

		if(( lock = file->fh_Arg2 ))
		{
			result = lock->current_position;
		}

		if( result < 0 )
		{
			(*res2) = ERROR_SEEK_ERROR;
		}

		IEXEC->ReleaseSemaphore(gd->Sem);
	}
	else
	{
		(*res2) = ERROR_REQUIRED_ARG_MISSING;
	}

	return(result);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_GET_FILE_POSITION64 (NB: DosPacket64) - FSGetFilePosition()   

   SYNOPSIS
	int64 RESULT1 = FSGetFilePosition(struct FileSystemVectorPort *fsvp,
	                         int32 *result2, struct FileHandle *file);
	 
   FUNCTION
	ACTION_GET_FILE_POSITION64 returns the current absolute file cursor
	position of an open file, or the value of -1LL if an error occured.
	For packet callers, this call will use a struct DosPacket64.


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	file    - (struct FileHandle *) Pointer to the open file handle.
	RESULT1 - (int64) Current File Position, -1LL on failure.
	RESULT2 - (int32) Failure code if RESULT1 == -1LL.


   INPUTS    (DosPacket64 method)
	dp_Type - (int32) ACTION_GET_FILE_POSITION64
	dp_Arg1 - (APTR)  filehandle->fh_Arg1
	dp_Arg2 - (int64) <unused> 0.
	dp_Arg3 - (struct FileHandle *) filehandle (added 53.49, previously 0)
	RESULT1 - (int64) Current file position (-1LL on failure).
	RESULT2 - (int32) Failure code if RESULT1 == -1LL


   SEE ALSO
	IDOS->GetFilePosition()



**/ 


/****************************************************************************/ 
/* EOF */ 



