/****************************************************************************/ 
/*  _FSChangeFileSize.c
**       
**  Created by FileSystem VectorPort tool. (FSVPTool 53.23) 
**       
**  :ts=4 
**       
*/       
/****************************************************************************/ 


#include "all_includes.h" 


/****************************************************************************/ 



static int32 do_changefilesize(APTR dummy UNUSED,...) 
{
	return 0; /* Write me ! */ 
}



/****************************************************************************/ 


int32 FSChangeFileSize(struct FSVP *vp, int32 *res2, struct FileHandle *file, int32 mode, int64 size)
{
	struct GlobalData *gd = vp->FSV.FSPrivate;
	int32  result = FALSE;

	if( file )
	{
		struct ObjLock *lock = file->fh_Arg2; 

		IEXEC->ObtainSemaphore(gd->Sem);

		result = do_changefilesize(gd, res2, lock, size, mode);

		/* 
		** On success, cause a change update to occur 
		** when the FSClose()/FSUnLock() function is called. 
		*/ 
		if( result ) 
		{ 
			lock->updated = TRUE; 
		} 

		IEXEC->ReleaseSemaphore(gd->Sem);
	}
	else
	{
		(*res2) = ERROR_REQUIRED_ARG_MISSING;
		result  = FALSE;
	}

	return(result);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_CHANGE_FILE_SIZE64 (NB: DosPacket64) - FSChangeFileSize()   

   SYNOPSIS
	int32 RESULT1 = FSChangeFileSize(struct FileSystemVectorPort *fsvp,
	                         int32 *result2, struct FileHandle *file,
	                         int32 mode, int64 size);

   FUNCTION
	Changes the file size, truncating or extending as needed. 
	If the file is extended, the values for the new bytes should always
	be either zeroed or scrambled for security reasons, this area should
	never contain residual readable block data from some previous usage.

	This function shall return a failure code if the required new size
	cannot be achieved.    If the file in question could not be changed
	successfully, the file must be left exactly as it was before.

	If the new size would be before the filehandle's current cursor position,
	the filehandle's cursor position shall be adjusted to point at the new
	end-of-file, otherwise the file cursor position shall not be changed.

   WARNING
	Filesystems shall not allow the size of a file to be changed if there
	is more than the one filehandle open on the same file and the size
	change would result in any of the other file position pointers being left
	pointing past the new end-of-file, the error code; ERROR_OBJECT_IN_USE
	shall be used to indicate the failure type for this situation.


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	file    - (struct FileHandle *) Pointer to the open file handle.
	mode    - (int32) OFFSET_BEGINNING, OFFSET_CURRENT or OFFSET_END.
	size    - (int64) Size relative to mode.
	RESULT1 - (int32) Boolean, success/failure, FALSE on failure.
	RESULT2 - (int32) Failure code if RESULT1 == FALSE.


   INPUTS    (DosPacket64 method)
	dp_Type - (int32) ACTION_CHANGE_FILE_SIZE64
	dp_Arg1 - (APTR)  filehandle->fh_Arg1
	dp_Arg2 - (int64) size
	dp_Arg3 - (int32) mode
	dp_Arg4 - (struct FileHandle *) filehandle
	RESULT1 - (int64) 64 bit success/failure (-1LL or 0LL)  0LL on failure.
	RESULT2 - (int32) Failure code if RESULT1 == 0LL


   SEE ALSO
	IDOS->ChangeFileSize()



**/ 


/****************************************************************************/ 
/* EOF */ 



