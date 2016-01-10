/****************************************************************************/ 
/*  _FSParentDir.c
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


struct Lock * FSParentDir(struct FSVP *vp, int32 *res2, struct Lock *dirlock)
{
	struct GlobalData *gd = vp->FSV.FSPrivate;
	struct ObjLock * lock = (struct ObjLock *)dirlock;
	struct Lock *  result = NULL;
	struct ObjNode *node;

	IEXEC->ObtainSemaphore(gd->Sem);

	if( lock )
	{
		if(( node = lock->node )) 
		{
			if(( node = node->parent_dir )) 
			{
				//result  = new_lock(gd, res2, node, SHARED_LOCK);
				(*res2) = 0;
			}
		}
	}

	IEXEC->ReleaseSemaphore(gd->Sem);

	return(result);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_PARENT - FSParentDir()

   SYNOPSIS
	struct Lock *RESULT1 = FSParentDir(struct FileSystemVectorPort *fsvp,
	                                   int32 *result2, struct Lock *lock);

   FUNCTION
	This function uses a lock on an object and creates a shared lock 
	on that object's parent directory.

	If the lock is for a directory, the RESULT1 will be a shared directory
	lock one directory level above.

	If lock is on a file, the RESULT1 will returns a shared directory lock
	to the directory level where the file is located.

	If the lock has no parent, then a lock of ZERO is returned.
	No secondary error code shall be returned for ZERO root locks.

	A lock of ZERO will return ZERO and a secondary error code of
	ERROR_REQUIRED_ARG_MISSING in RESULT2.
	Note that this case is currently handled in IDOS->ParentDir() but the
	filesystem should still be prepared for ZERO locks being supplied.

	Note that this operation is typically used in the process of building
	an absolute path name for a given object.


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	lock    - (struct Lock *) Pointer to a struct lock.
	RESULT1 - (struct Lock *) Parent dir lock, ZERO on error.
	RESULT2 - (int32) Failure code if RESULT1 == ZERO.


   INPUTS    (DosPacket method)
	dp_Type - (int32) ACTION_PARENT
	dp_Arg1 - (BPTR)  Lock on the object to get the parent of.
	RESULT1 - (BPTR)  Parent lock, ZERO on error.
	RESULT2 - (int32) Failure code if RESULT1 == ZERO


   SEE ALSO
	IDOS->ParentDir()



**/ 


/****************************************************************************/ 
/* EOF */ 



