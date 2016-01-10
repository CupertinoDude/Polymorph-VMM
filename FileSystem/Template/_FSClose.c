/****************************************************************************/ 
/*  _FSClose.c
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
**  Closing may cause the last block written to be flushed, 
**  we allow files to be flushed on close even if the file 
**  becomes write inhibited or the volume becomes write protected 
**  since the last write occured. 
** ==================================================================
*/ 


int32 FSClose (struct FSVP *vp, int32 *res2, struct FileHandle *fh) 
{ 
//	struct GlobalData *gd = vp->FSV.FSPrivate; 
	int32  result; 


	if( fh ) 
	{ 
		/* 
		** Do any custom stream close handling here. 
		** 
		** The change update is performed in FSUnLock(). 
		*/ 


		/* 
		** Use FSUnLock() to release the lock. 
		*/ 
		result  = FSUnLock(vp, res2, fh->fh_Arg2); 
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
	ACTION_END - FSClose()

   SYNOPSIS
	int32 RESULT1 = FSClose(struct FileSystemVectorPort *fsvp,
	                        int32 *result2, struct FileHandle *file);

   FUNCTION
	This action closes an open file handle.  This function generally
	returns a DOSTRUE as there is little the application can do to
	recover from a file closing failure.  

	However, if something does actually go wrong while releasing a file,
	it is recommended that FALSE be returned to indicate this, even if 
	it is rather unlikely anybody is actually listening. 
	
	This function is generally used to initiate the clearing of the Archive
	bit for changed objects, the archive bit of the parent directory shall
	always be cleared as well.  This should propagate all the way back to
	the root directory so directories with changed objects can be identified
	by backup software. 


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	file    - (struct FileHandle *) Pointer to an open file.
	RESULT1 - (int32) BOOLEAN success.  Returns FALSE on error.
	RESULT2 - (int32) Failure code if RESULT1 == ZERO.


   INPUTS    (DosPacket method)
	dp_Type - (int32) ACTION_END
	dp_Arg1 - (APTR)  fh_Arg1 field of the opened struct FileHandle.
	dp_Arg2 - (APTR)  struct FileHandle pointer (added 53.24, previously 0)
	RESULT1 - (int32) BOOLEAN success.  Returns FALSE if Close() failed.  
	RESULT2 - (int32) Failure code if RESULT1 == FALSE


   NOTES
	In the event a volume becomes write protected before a file is closed
	and the file has cached writes pending, the current methodology is
	to allow the cache flush to occur before the file is actually closed.

   BUGS
	If an error is returned under 2.0, DOS will not deallocate the
	file handle.  Under 1.3, it does not check the result at all.

   SEE ALSO
	IDOS->Close()



**/ 


/****************************************************************************/ 
/* EOF */ 



