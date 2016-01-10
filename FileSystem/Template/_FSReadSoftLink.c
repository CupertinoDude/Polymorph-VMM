/****************************************************************************/ 
/*  _FSReadSoftLink.c
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
**  This string input function can have a NULL reference lock to 
**  indicate a root directory relative object name string, 
**  The string name may also be a nul-string to indicate the relative  
**  lock object should be used, and both conditions may also occur to  
**  indicate a root directory reference, all these situations must be  
**  handled here appropriately. 
**  See the FSLock() autodoc for example code to handle this.   
** ==================================================================  
*/ 


int32 read_softlink_target(APTR dummy UNUSED,...) 
{
	return 0; /* Write me ! */ 
}



/****************************************************************************/ 


int32 FSReadSoftLink(struct FSVP *vp, int32 *res2, struct Lock *rel_dir, 
                        CONST_STRPTR linkname, STRPTR buffer, int32 bufsize) 
{
	struct GlobalData *gd = vp->FSV.FSPrivate;
	struct ObjLock *lock  = (struct ObjLock *)rel_dir;
	int32  result;

	IEXEC->ObtainSemaphoreShared(gd->Sem);

	result = read_softlink_target(gd, res2, lock, linkname, buffer, bufsize);

	IEXEC->ReleaseSemaphore(gd->Sem);

	return(result);
}




/****************************************************************************/ 
/** 

   NAME
	ACTION_READ_SOFT_LINK - FSReadSoftLink()

   SYNOPSIS
	int32 RESULT1 = FSReadSoftLink(struct FileSystemVectorPort *fsvp,
	                           int32 *result2, 
	                           struct Lock *rel,
	                           CONST_STRPTR linkname,
	                           STRPTR buffer, int32 bufsize);

   FUNCTION
	Returns the destination target object descriptor for a soft link.

	The softlink is identified by the relative lock 'rel' and 'linkname'.
	To handle the 'rel' and 'linkname' parameters correctly within the
	filesystem, use the example code in the ACTION_LOCATE_OBJECT / FSLock()
	autodoc.

	This operation requires the filesystem to do nothing else but copy the
	target string of the softlink into the destination buffer and return
	the string length of it in RESULT1, or a negative value upon error, as:
	    -2 if there isn't enough space in 'buffer'.
	    -1 for other errors, specified by the error code in RESULT2.
	    (See NOTES for error codes)


   INPUTS     (FileSystemVectorPort method)
	fsvp     - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2  - (int32 *) Pointer to the storage area for RESULT2.
	rel      - (struct Lock *) Pointer to the soft linkname relative lock.
	linkname - (CONST_STRPTR)  Pointer to the soft linkname string.
	buffer   - (STRPTR) Buffer to hold the destination target string.
	bufsize  - (int32)  Size of the destination target string buffer.
	RESULT1  - (int32)  Actual length of returned string, -2 if there isn't
	                    enough space in buffer, or -1 for other errors.
	RESULT2  - (int32)  Failure code if RESULT1 is a negative value.


   INPUTS     (DosPacket method)
	dp_Type  - (int32)  ACTION_READ_SOFT_LINK
	dp_Arg1  - (BPTR)   lock on directory that dp_Arg2 is relative to.
	dp_Arg2  - (STRPTR) Linkname and path string. (relative to dp_Arg1).
	                    NOTE: This is a C string not a BSTR.
	dp_Arg3  - (STRPTR) Buffer for the target object destination string.
	                    NOTE: This is a C string not a BSTR.
	dp_Arg4  - (int32)  Size of the destination buffer. (dp_Arg3).
	RESULT1  - (int32)  Actual length of returned string, -2 if there isn't
	                    enough space in buffer, or -1 for other errors.
	RESULT2  - (int32)  Failure code if RESULT1 is a negative value.


   NOTES
	For -2 error codes, the FS shall set RESULT2 = ERROR_LINE_TOO_LONG.
	For -1 error codes, the FS shall use a code appropriate for the error.


   SEE ALSO
	IDOS->ReadSoftLink()



**/ 


/****************************************************************************/ 
/* EOF */ 



