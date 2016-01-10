/****************************************************************************/ 
/*  _FSSetComment.c
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

/* 
** ==================================================================
**  The FSetXXXX() functions must also cause a change update, however,
**  this must begin at the objects parent dir and not on the object,
**  as this will interfere with multiple successive attribute changes 
**  for operations such as clone file copying. 
** ==================================================================
*/ 


static int32 do_setcomment(APTR dummy UNUSED,...) 
{
	return 0; /* Write me ! */ 
}



/****************************************************************************/ 


int32 FSSetComment(struct FSVP *vp, int32 *res2, struct Lock *dirlock, 
                              CONST_STRPTR objname, CONST_STRPTR comment) 
{
	struct GlobalData *gd = vp->FSV.FSPrivate;
	struct ObjLock *lock  = (struct ObjLock *)dirlock;
	int32  result;

	IEXEC->ObtainSemaphore(gd->Sem);

	result = do_setcomment(gd,res2,lock,objname,comment);

	IEXEC->ReleaseSemaphore(gd->Sem);

	return(result);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_SET_COMMENT - FSSetComment()

   SYNOPSIS
	int32 RESULT1 = FSSetComment(struct FileSystemVectorPort *fsvp, 
	                             int32 *result2, struct Lock *rel,
	                             CONST_STRPTR name, CONST_STRPTR comment);

   FUNCTION
	This action sets the comment string for a filesystem object.

	If the object does not exist or any other error occured then FALSE
	will be returned in RESULT1 with the failure code in RESULT2.

	The comment string was limited to 79 characters in earlier filesystems,
	this is no longer the case,  note that not all filesystems will support
	comments, for example, NFS usually will not, or the size of comment 
	supported may vary, but they can not exceed 255 bytes, so be prepared
	to handle this size.

	To handle the 'rel' and 'name' parameters correctly, use the
	example code shown in ACTION_LOCATE_OBJECT / FSLock() autodoc.


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	rel     - (struct Lock *) Pointer to the 'name' reference lock.
	name    - (CONST_STRPTR) Pointer to the object name.(no path component)
	comment - (CONST_STRPTR) Pointer to the comment string.
	RESULT1 - (int32)  Success/failure (DOSTRUE/FALSE) (FALSE on error).
	RESULT2 - (int32)  Failure code if RESULT1 == FALSE


   INPUTS    (DosPacket method)
	dp_Type - (int32)  ACTION_SET_COMMENT
	dp_Arg1 - <unused> 0
	dp_Arg2 - (BPTR)   Lock to which dp_Arg3 is relative.
	dp_Arg3 - (BSTR)   Name of object relative to dp_Arg2
	dp_Arg4 - (BSTR)   New comment string.
	dp_Arg5 - (BSTR)   Nametype - name format indicator value.
	                   if(dp_Arg3==dp_Arg5) then all BSTR's are
	                   guaranteed to be nul-terminated. (53.23)
	RESULT1 - (int32)  Success/failure (DOSTRUE/FALSE) (FALSE on error).
	RESULT2 - (int32)  Failure code if RESULT1 == FALSE


   SEE ALSO
	IDOS->SetComment(),  ACTION_INHIBIT_DOS_LONGPATH_HANDLING



**/ 


/****************************************************************************/ 
/* EOF */ 



