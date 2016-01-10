/****************************************************************************/ 
/*  _FSChangeFilePosition.c
**       
**  Created by FileSystem VectorPort tool. (FSVPTool 53.23) 
**       
**  :ts=4 
**       
*/       
/****************************************************************************/ 


#include "all_includes.h" 


/****************************************************************************/ 



static int32 do_changefilepos(APTR dummy UNUSED,...) 
{
	return 0; /* Write me ! */ 
}



/****************************************************************************/ 


int32 FSChangeFilePosition(struct FSVP *vp, int32 *res2, struct FileHandle *file, int32 mode, int64 pos)
{
	struct GlobalData *gd = vp->FSV.FSPrivate;
	int32  result;

	if( file )
	{
		IEXEC->ObtainSemaphore(gd->Sem);

		result = do_changefilepos(gd, res2, file->fh_Arg2, pos, mode);

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
	ACTION_CHANGE_FILE_POSITION64 (DosPacket64) - FSChangeFilePosition()

   SYNOPSIS
	int32 RESULT1 = FSChangeFilePosition(struct FileSystemVectorPort *fsvp,
	                         int32 *result2, struct FileHandle *file,
	                         int32 mode, int64 pos);

   FUNCTION
	ChangeFilePosition() sets the read/write cursor for the file to the
	position 'pos'.   This position is used by both Read() and Write()
	as a place to start reading or writing. 

	'mode' can be OFFSET_BEGINNING, OFFSET_CURRENT or OFFSET_END. 
	It is used to specify the relative start position. 
	For example, 20 from current is a position 20 bytes forward 
	from current, -20 is 20 bytes back from current.

	Attempting to change the file position beyond the ends of a file
	must cause failure with a secondary result of ERROR_SEEK_ERROR.

	On any failure, the original file position shall remain valid.


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	file    - (struct FileHandle *) Pointer to the open file handle.
	mode    - (int32) OFFSET_BEGINNING, OFFSET_CURRENT or OFFSET_END.
	pos     - (int64) Position relative to mode.
	RESULT1 - (int32) Boolean, success/failure, FALSE on failure.
	RESULT2 - (int32) Failure code if RESULT1 == FALSE.


   INPUTS    (DosPacket64 method)
	dp_Type - (int32) ACTION_CHANGE_FILE_POSITION64
	dp_Arg1 - (APTR)  filehandle->fh_Arg1
	dp_Arg2 - (int64) position.
	dp_Arg3 - (int32) mode
	dp_Arg4 - (struct FileHandle *) filehandle
	RESULT1 - (int64) 64 bit success/failure (-1LL or 0LL) 0LL on failure.
	RESULT2 - (int32) Failure code if RESULT1 == 0LL


   SEE ALSO
	IDOS->ChangeFilePosition()



**/ 


/****************************************************************************/ 
/* EOF */ 



