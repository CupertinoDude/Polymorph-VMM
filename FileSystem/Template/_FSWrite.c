/****************************************************************************/ 
/*  _FSWrite.c
**       
**  Created by FileSystem VectorPort tool. (FSVPTool 53.23) 
**       
**  :ts=4 
**       
*/       
/****************************************************************************/ 


#include "all_includes.h" 


/****************************************************************************/ 



static int32 do_write(APTR dummy UNUSED,...) 
{
	return 0; /* Write me ! */ 
}



/****************************************************************************/ 


int32 FSWrite (struct FSVP *vp, int32 *res2, struct FileHandle *fh, 
                                       STRPTR buffer, int32 numbytes) 
{
	struct GlobalData *gd = vp->FSV.FSPrivate;
	int32 result;

	IEXEC->ObtainSemaphore(gd->Sem);

	if( fh )
	{
		struct ObjLock *lock = fh->fh_Arg2; 
		
		result = do_write(gd, res2, lock, buffer, numbytes);
		/* 
		** If any data is written, cause a change update to occur 
		** when the FSClose()/FSUnLock() function is called. 
		*/ 
		if( result > 0 ) 
		{ 
			lock->updated = TRUE; 
		} 
	}
	else
	{
		(*res2) = ERROR_REQUIRED_ARG_MISSING;
		result  = -1;
	}

	IEXEC->ReleaseSemaphore(gd->Sem);

	return(result);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_WRITE - FSWrite()

   SYNOPSIS
	int32 RESULT1 = FSWrite(struct FileSystemVectorPort *fsvp, 
	                         int32 *result2, struct FileHandle *file,
	                          STRPTR buffer, int32 numchar);

   FUNCTION
	This function copies data into the file at the current position.
	The file is automatically extended when the write passes the
	end of the file.

	The handler returns the number of bytes written in RESULT1 which should
	always be the same as the "numchar' value, with zero in RESULT2.

	When any failure occurs, the handler returns -1 in RESULT1 along with
	the appropriate error code in RESULT2. (See dos/ErrorReport() for info)

	The handler is also responsible for checking the usual issues, such as
	checking for attempts to write to a write protected file, or attempting
	to write on a write protected volume, or writing to a full disk.
  
	In the case of a failure, the handler does not update the current file
	position (although the file may have been extended and some data
	overwritten) so that an application can safely retry the operation.


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	file    - (struct FileHandle *) Pointer to the open file handle.
	buffer  - (STRPTR) Pointer to the buffer to be written.
	numchar - (int32) Number of bytes to write.
	RESULT1 - (int32) Number of bytes actually written or -1 on error.
	RESULT2 - (int32) Failure code if RESULT1 != numchar.


   INPUTS    (DosPacket method)
	dp_Type - (int32) ACTION_WRITE
	dp_Arg1 - (APTR)  fh_Arg1 field of the opened struct FileHandle.
	dp_Arg2 - (APTR)  Buffer to write to the file handle.
	dp_Arg3 - (int32) Number of bytes to write.
	dp_Arg4 - (struct FileHandle *) FileHandle. (added 53.49, previously 0)
	RESULT1 - (int32) Number of bytes actually written or -1 on error.
	RESULT2 - (int32) Failure code if RESULT1 != dp_Arg3


   SEE ALSO
	IDOS->Write();



**/ 


/****************************************************************************/ 
/* EOF */ 



