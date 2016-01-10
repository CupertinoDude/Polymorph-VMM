/****************************************************************************/ 
/*  _FSUnLock.c
**       
**  Created by FileSystem VectorPort tool. (FSVPTool 53.23) 
**       
**  :ts=4 
**       
*/       
/****************************************************************************/ 


#include "all_includes.h" 


/****************************************************************************/ 



static int32 freelock(APTR dummy UNUSED,...) 
{
	return 0; /* Write me ! */ 
}



/****************************************************************************/ 


int32 FSUnLock(struct FSVP *vp, int32 *res2, struct Lock *lock_in)
{
	struct GlobalData *gd = vp->FSV.FSPrivate;
	struct ObjLock *lock  = (struct ObjLock *)lock_in; 
	int32 result;

	IEXEC->ObtainSemaphore(gd->Sem);

	if( lock->updated ) 
	{ 
		Change_Update(gd, lock->node); 
	} 

	result = freelock(gd, res2, lock);

	IEXEC->ReleaseSemaphore(gd->Sem);

	return(result);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_FREE_LOCK - FSUnLock()

   SYNOPSIS
	int32 RESULT1 = FSUnLock(struct FileSystemVectorPort *fsvp,
	                        int32 *result2, struct Lock *lock);

   FUNCTION
	This action frees the lock passed to it.  
	If passed a ZERO lock, the handler must also return success (DOSTRUE).


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	lock    - (struct Lock *) Pointer to the lock to unlock.
	RESULT1 - (int32) Boolean Success/Failure code, FALSE on error.
	RESULT2 - (int32) Failure code if RESULT1 == FALSE.


   INPUTS    (DosPacket method)
	dp_Type - (int32) ACTION_FREE_LOCK
	dp_Arg1 - (BPTR)  lock.
	RESULT1 - (int32) Success/Failure code, FALSE on error.
	RESULT2 - (int32) Failure code if RESULT1 == FALSE


   SEE ALSO
	IDOS->UnLock()



**/ 


/****************************************************************************/ 
/* EOF */ 



