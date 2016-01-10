/****************************************************************************/ 
/*  _FSDelete.c
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


static int32 do_delete(APTR dummy UNUSED,...) 
{
	return 0; /* Write me ! */ 
}



/****************************************************************************/ 


int32 FSDelete(struct FSVP *vp, int32 *res2, struct Lock *dirlock, CONST_STRPTR objstr)
{
	struct GlobalData *gd = vp->FSV.FSPrivate;
	struct ObjLock  *lock = (struct ObjLock *)dirlock;
	int32 result;

	IEXEC->ObtainSemaphore(gd->Sem);

	result = do_delete(gd, res2, lock, objstr);

	IEXEC->ReleaseSemaphore(gd->Sem);

	return(result);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_DELETE_OBJECT - FSDelete()

   SYNOPSIS
	int32 RESULT1 = FSDelete(struct FileSystemVectorPort *fsvp, 
	                         int32 *result2, struct Lock *rel,
	                         CONST_STRPTR name);

   FUNCTION
	This function deletes the file, directory or symbolic link specified
	if there are currently no active locks in force and the protection
	status bits allow it.

	If there are existing locks being held on the object to be deleted,
	return failure along with the error code; ERROR_OBJECT_IN_USE.

	Note that all the objects within a directory must be deleted before
	the directory itself can be deleted, if the directory is not empty,
	return failure along with the error code; ERROR_DIRECTORY_NOT_EMPTY.

	If any object has the protection bit EXDF_NO_DELETE set,
	return failure along with the error code; ERROR_DELETE_PROTECTED.

	If the object is a file and the protection bit EXDF_NO_WRITE is set,
	return failure along with the error code; ERROR_WRITE_PROTECTED.

	If the disk itself is write protected, return failure along with the
	error code; ERROR_DISK_WRITE_PROTECTED.

	The 'name' refers to the object, which may include a path component
	for DosPacket callers only, vector-port callers will have only the 
	object name and never any path component.
	The 'rel' is a relative lock from which to look for the 'name' object.  

	Use the example code shown in ACTION_LOCATE_OBJECT / FSLock() autodoc
	to handle the relative argument parameters in all situations.

	Failure (FALSE) is returned if the deletion fails, DOSTRUE upon success.


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	rel     - (struct Lock *) Pointer to the 'name' reference lock.
	name    - (CONST_STRPTR) Pointer to the object name.(no path component)
	RESULT1 - (int32) Success/Failure indicator, (FALSE on failure.)
	RESULT2 - (int32) Failure code if RESULT1 == FALSE


   INPUTS    (DosPacket method)
	dp_Type - (int32) ACTION_DELETE_OBJECT
	dp_Arg1 - (BPTR)  relative lock.
	dp_Arg2 - (BSTR)  name. (may also include a path component)
	dp_Arg3 - <unused> 0
	dp_Arg4 - <unused> 0
	dp_Arg5 - (BSTR)  nameformat - name format indicator value,
	                  only if(dp_Arg2==dp_Arg5) then BSTR's are guaranteed
	                  to be a nul-terminated string. (53.23)
	RESULT1 - (int32) Success/Failure indicator, (FALSE on failure.)
	RESULT2 - (int32) Failure code if RESULT1 == FALSE


   SEE ALSO
	IDOS->Delete(), ACTION_INHIBIT_DOS_LONGPATH_HANDLING



**/ 


/****************************************************************************/ 
/* EOF */ 



