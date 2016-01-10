/****************************************************************************/ 
/*  _FSParentOfFH.c
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


struct Lock * FSParentOfFH(struct FSVP *vp, int32 *res2 UNUSED, struct FileHandle *file)
{
	struct GlobalData *gd = vp->FSV.FSPrivate;
	struct Lock   *result = NULL;
	struct ObjNode *node;
	struct ObjLock *lock;

	IEXEC->ObtainSemaphore(gd->Sem);

	if( file ) 
	{ 
		if(( lock = file->fh_Arg2 )) 
		{
			if(( node = lock->node )) 
			{
				if(( node = node->parent_dir )) /* parent dir of file */ 
				{
					//result = new_lock(gd, res2, node, SHARED_LOCK);
				}
			}
		}
	}

	IEXEC->ReleaseSemaphore(gd->Sem);

	return(result);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_PARENT_FH - FSParentOfFH()

   SYNOPSIS
	struct Lock *RESULT1 = FSParentOfFH(struct FileSystemVectorPort *fsvp,
	                              int32 *result2, struct FileHandle *file);

   FUNCTION
	This function returns a lock on the parent directory for a currently
	opened file handle.  The lock mode returned is always a shared lock. 
	Note that unlike ACTION_COPY_LOCK_FH, the mode of the file handle
	is unimportant.   For an open file, ACTION_PARENT_FH should return 
	a lock under all circumstances.


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	file    - (struct FileHandle *) Pointer to an open file.
	RESULT1 - (struct Lock *)Lock on the parent dir of file, ZERO on error.
	RESULT2 - (int32) Failure code if RESULT1 == ZERO.


   INPUTS    (DosPacket method)
	dp_Type - (int32) ACTION_PARENT_FH
	dp_Arg1 - (APTR)  file->fh_Arg1.
	dp_Arg2 - (struct FileHandle *)file. (added 53.49, previously 0)
	RESULT1 - (BPTR)  Lock on parent dir of file or ZERO on error.
	RESULT2 - (int32) Failure code if RESULT1 == ZERO


   SEE ALSO
	IDOS->ParentOfFH()



**/ 


/****************************************************************************/ 
/* EOF */ 



