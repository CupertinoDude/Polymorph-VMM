/****************************************************************************/ 
/*  _FSSameFile.c
**       
**  Created by FileSystem VectorPort tool. (FSVPTool 53.23) 
**       
**  :ts=4 
**       
*/       
/****************************************************************************/ 


#include "all_includes.h" 


/****************************************************************************/ 



int32 FSSameFile(struct FSVP *vp, int32 *res2, struct FileHandle *fh1, struct FileHandle *fh2)
{
	int32 result;

	if( fh1 && fh2 )
	{
		result = FSSameLock(vp, res2, fh1->fh_Arg2, fh2->fh_Arg2);
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
	ACTION_SAME_FH - FSSameFile()

   SYNOPSIS
	int32 RESULT1 = FSSameFile(struct FileSystemVectorPort *fsvp,
	                           int32 *result2, struct FileHandle *fh1,
	                           struct FileHandle *fh2);

   FUNCTION
	This action compares the targets of two filehandles, if they are on the
	same file, ACTION_SAME_FH will return DOSTRUE.
	If the two filehandles are on different objects, it returns FALSE.
	

   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	fh1     - (struct FileHandle *) Pointer to the first filehandle.
	fh2     - (struct FileHandle *) Pointer to the second filehandle.
	RESULT1 - (int32) DOSTRUE / FALSE  (DOSTRUE for same file)
	RESULT2 - (int32) Failure code if RESULT1==FALSE and an error occured.


   INPUTS    (DosPacket method)
	dp_Type - (int32) ACTION_SAME_FH
	dp_Arg1 - (APTR)  fh1->fh_Arg1.
	dp_Arg2 - (APTR)  fh2->fh_Arg1.
	dp_Arg3 - (struct FileHandle *)fh1.
	dp_Arg4 - (struct FileHandle *)fh2.
	RESULT1 - (int32) DOSTRUE / FALSE   (DOSTRUE for same file)
	RESULT2 - (int32) Failure code if RESULT1 == FALSE, otherwise 0.


   NOTES
	This packet was introduced in dos.library 53.71
	If the filesystem does not support this packet yet, dos.library will
	provide emulation using SameLock() and DupLockFromFH().
 
   SEE ALSO
	IDOS->SameFH()



**/ 


/****************************************************************************/ 
/* EOF */ 



