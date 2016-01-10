/****************************************************************************/ 
/*  _FSRead.c
**       
**  Created by FileSystem VectorPort tool. (FSVPTool 53.23) 
**       
**  :ts=4 
**       
*/       
/****************************************************************************/ 


#include "all_includes.h" 


/****************************************************************************/ 



static int32 do_read(APTR dummy UNUSED,...) 
{
	return 0; /* Write me ! */ 
}



/****************************************************************************/ 


int32 FSRead(struct FSVP *vp, int32 *res2, struct FileHandle *fh, STRPTR buffer, int32 numbytes)
{
	struct GlobalData *gd = vp->FSV.FSPrivate;
	int32 result;

	IEXEC->ObtainSemaphoreShared(gd->Sem);

	if( fh )
	{
		result = do_read(gd, res2, fh->fh_Arg2, buffer, numbytes);
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
	ACTION_READ - FSRead()

   SYNOPSIS
	int32 RESULT1 = FSRead(struct FileSystemVectorPort *fsvp, 
	                       int32 *result2, struct FileHandle *file,
	                       STRPTR buffer, int32 numchar);

   FUNCTION
	This function extracts data from the file (or input channel) at the
	current position.  

	If fewer bytes remain in the file than requested, only those bytes
	remaining will be returned with the number of bytes actually read
	will be returned in RESULT1, this may also be zero if end-of-file
	has already been reached.

	The handler indicates errors with -1 in RESULT1 and the error code 
	in RESULT2.

	An error would occur if attempting to read from a read protected file,
	in this particular case the RESULT2 error code = ERROR_READ_PROTECTED.

	If the read fails, the current file position remains unchanged. 
	A handler may return a smaller number of bytes than requested,
	even if not at the end of a file.  This can happen with interactive
	type file handles which may return one line at a time as the user
	hits return, for example the console handler CON:.


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	file    - (struct FileHandle *) Pointer to the open file handle.
	buffer  - (STRPTR) Pointer to the buffer to read data into.
	numchar - (int32) Number of bytes to read.
	RESULT1 - (int32) Number of bytes actually read. 0 for EOF, -1 on error.
	RESULT2 - (int32) Failure code if RESULT1 == -1.


   INPUTS    (DosPacket method)
	dp_Type - (int32) ACTION_READ
	dp_Arg1 - (APTR)  fh_Arg1 field of the open struct FileHandle.
	dp_Arg2 - (APTR)  Buffer to put data into.
	dp_Arg3 - (int32) Number of bytes to read.
	dp_Arg4 - (APTR)  struct FileHandle pointer (added 53.49, previously 0)
	RESULT1 - (int32) Number of bytes read. 0 indicates EOF, -1 on error.
	RESULT2 - (int32) Failure code if RESULT1 == -1


   SEE ALSO
	IDOS->Read()



**/ 


/****************************************************************************/ 
/* EOF */ 



