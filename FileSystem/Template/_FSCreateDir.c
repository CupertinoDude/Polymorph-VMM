/****************************************************************************/ 
/*  _FSCreateDir.c
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
**  This function must always create only shared directory locks.
**  Exclusive locks on directories are absolutely not permitted.
** ==================================================================
*/ 

/* 
** ==================================================================
**  New objects must inherit owner and group attributes from the     
**  parent directory, they must have a current creation timestamp,   
**  the protection mask must be set to EXDF_DEFAULT (0), and the     
**  comment strings shall default to a nul-string.                   
** ==================================================================
*/ 


static struct Lock *do_createdir(APTR dummy UNUSED,...) 
{
	return 0; /* Write me ! */ 
}



/****************************************************************************/ 


struct Lock * FSCreateDir(struct FSVP *vp, int32 *res2, struct Lock *rel_lock, 
                            CONST_STRPTR dirname)
{
	struct GlobalData *gd = vp->FSV.FSPrivate;
	struct ObjLock * lock = (struct ObjLock *)rel_lock;
	struct Lock *result;

	IEXEC->ObtainSemaphore(gd->Sem);

	result = do_createdir(gd, res2, lock, dirname);

	IEXEC->ReleaseSemaphore(gd->Sem);

	return(result);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_CREATE_DIR - FSCreateDir()

   SYNOPSIS
	struct Lock *RESULT1 = FSCreateDir(struct FileSystemVectorPort *fsvp,
	                                int32 *result2, struct Lock *rel, 
	                                CONST_STRPTR name);

   FUNCTION
	ACTION_CREATE_DIR creates a new directory with the specified name.
	An error is returned if it fails.  Directories can only be created
	on devices which support them, e.g. disks.  

	Before V53, ACTION_CREATE_DIR returned an exclusive lock on the new
	directory if it succeeded, from V53+ it is now required that this be
	a shared lock, this is NOT optional, if you fail to implement only
	a shared directory lock mode, then the dos.library vector-port and
	long path handling will simply not work in all circumstances.

	The 'name' refers to the object, which may include a path component
	for DosPacket callers only, vector-port calls will have only the 
	final directory name and never any path component.

	Creating a directory must fail with the error code; ERROR_OBJECT_EXISTS
	if the 'name' was already in use by any filesystem object. 
	If the disk itself is write protected, return failure along with the
	error code; ERROR_DISK_WRITE_PROTECTED.

	The new directory shall also inherit the parents protection mask,
	as well as the group and owner values, and comments shall always 
	default to a nul-string.
	

   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	rel     - (struct Lock *) Pointer to the 'name' relative lock.
	name    - (CONST_STRPTR) Pointer to the dir name.(no path component)
	RESULT1 - (struct Lock *)  Lock - (NULL on failure.)
	RESULT2 - (int32) Failure code if RESULT1 == NULL


   INPUTS    (DosPacket method)
	dp_Type - (int32) ACTION_CREATE_DIR
	dp_Arg1 - (BPTR)  relative directory lock.
	dp_Arg2 - (BSTR)  Name of new directory. (may also include path)
	dp_Arg3 - <unused>   0
	dp_Arg4 - <unused>   0
	dp_Arg5 - (BSTR)  nametype - name format indicator value.
	                  if(dp_Arg2==dp_Arg5) then BSTR's are guaranteed to be
	                  nul-terminated. (53.23)
	RESULT1 - (BPTR)  Lock - (ZERO on failure.)
	RESULT2 - (int32) Failure code if RESULT1 == ZERO


	NOTES
	From V53 onwards, all DosPacket callers (using ACTION_CREATE_DIR) will
	have the result lock mode changed by dos.library to a shared lock upon
	success, vector port callers will be unaffected. 

   SEE ALSO
	IDOS->CreateDir(), IDOS->CreateDirTree(), 



**/ 


/****************************************************************************/ 
/* EOF */ 



