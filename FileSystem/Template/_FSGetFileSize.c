/****************************************************************************/ 
/*  _FSGetFileSize.c
**       
**  Created by FileSystem VectorPort tool. (FSVPTool 53.23) 
**       
**  :ts=4 
**       
*/       
/****************************************************************************/ 


#include "all_includes.h" 


/****************************************************************************/ 



int64 FSGetFileSize(struct FSVP *vp, int32 *res2, struct FileHandle *file)
{
	struct GlobalData *gd = vp->FSV.FSPrivate;
	struct ObjLock *lock;
	int64  result = -1LL;

	if( file )
	{
		IEXEC->ObtainSemaphoreShared(gd->Sem);

		if((lock = file->fh_Arg2 ))
		{
			result = lock->node->file_size;
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
	ACTION_GET_FILE_SIZE64 (NB: DosPacket64) -  FSGetFileSize()  

   SYNOPSIS
	int64 RESULT1 = FSGetFileSize(struct FileSystemVectorPort *fsvp,
	                        int32 *result2, struct FileHandle *file);

   FUNCTION
	ACTION_GET_FILE_SIZE64 returns the current size of an open file,
	or the value of -1LL if an error occured.

	This call may also cause the filesystem to flush any pending write
	buffers first, to obtain accurate and up to date information.


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	file    - (struct FileHandle *) Pointer to the open file handle.
	RESULT1 - (int64) Current file size, -1LL on failure.
	RESULT2 - (int32) Failure code if RESULT1 == -1LL.


   INPUTS    (DosPacket64 method)
	dp_Type - (int32) ACTION_GET_FILE_SIZE64
	dp_Arg1 - (APTR)  file->fh_Arg1
	dp_Arg2 - (int64) <unused> 0.
	dp_Arg3 - (struct FileHandle *) file (added 53.49, previously 0)
	RESULT1 - (int64) Current file size, (-1LL on failure).
	RESULT2 - (int32) Failure code if RESULT1 == -1LL


   SEE ALSO
	IDOS->GetFileSize()



**/ 


/****************************************************************************/ 
/* EOF */ 



