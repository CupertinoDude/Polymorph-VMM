/****************************************************************************/ 
/*  _FSDupLock.c
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
**  New objects must inherit owner and group attributes from the     
**  parent directory, they must have a current creation timestamp,   
**  the protection mask must be set to EXDF_DEFAULT (0), and the     
**  comment strings shall default to a nul-string.                   
** ==================================================================
*/ 


static struct Lock *do_duplock(APTR dummy UNUSED,...) 
{
	return 0; /* Write me ! */ 
}



/****************************************************************************/ 


struct Lock * FSDupLock(struct FSVP *vp, int32 *res2, struct Lock *lockin) 
{
	struct GlobalData *gd = vp->FSV.FSPrivate;
	struct ObjLock * lock = (struct ObjLock *)lockin;
	struct Lock *result;


	IEXEC->ObtainSemaphore(gd->Sem);

	result = do_duplock(gd, res2, lock);

	IEXEC->ReleaseSemaphore(gd->Sem);

	return(result);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_COPY_LOCK  - FSDupLock()

   SYNOPSIS
	struct Lock *RESULT1 = FSDupLock(struct FileSystemVectorPort *fsvp,
	                            int32 *result2, struct Lock *lock);

   FUNCTION
	This actions original name (ACTION_COPY_DIR) was misleading as it
	does not manipulate directories, instead, it creates a copy of a
	shared lock. ACTION_COPY_LOCK should always be used now.
	The copy is subsequently freed with an ACTION_FREE_LOCK.

	Only valid (non zero) locks are passed on to the filesystem.

	This function will always fail with RESULT2 = ERROR_OBJECT_IN_USE
	when attempting to create a duplicate lock on an already exclusively
	locked object.


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	lock    - (struct Lock *) Pointer to the lock to duplicate.
	RESULT1 - (struct Lock *) Duplicate lock, NULL on error.
	RESULT2 - (int32) Failure code if RESULT1 == ZERO.


   INPUTS    (DosPacket method)
	dp_Type - (int32) ACTION_COPY_LOCK
	dp_Arg1 - (BPTR)  lock.
	RESULT1 - (BPTR)  Duplicated lock, ZERO on error.
	RESULT2 - (int32) Failure code if RESULT1 == ZERO


   SEE ALSO
	IDOS->DupLock()



**/ 


/****************************************************************************/ 
/* EOF */ 



