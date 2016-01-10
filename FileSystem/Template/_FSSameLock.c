/****************************************************************************/ 
/*  _FSSameLock.c
**       
**  Created by FileSystem VectorPort tool. (FSVPTool 53.23) 
**       
**  :ts=4 
**       
*/       
/****************************************************************************/ 


#include "all_includes.h" 


/****************************************************************************/ 



int32 FSSameLock(struct FSVP *vp, int32 *res2, struct Lock *lk1, struct Lock *lk2)
{
	struct GlobalData *gd = vp->FSV.FSPrivate;
	struct ObjLock *lock1 = (struct ObjLock *)lk1;
	struct ObjLock *lock2 = (struct ObjLock *)lk2;
	int32          result = FALSE;


	IEXEC->ObtainSemaphoreShared(gd->Sem);

	if( lock1 && lock2 )
	{
		if( lock1->node == lock2->node )
		{
			result  = DOSTRUE;
			(*res2) = 0;
		}
	}

	IEXEC->ReleaseSemaphore(gd->Sem);

	return(result);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_SAME_LOCK - FSSameLock()

   SYNOPSIS
	int32 RESULT1 = FSSameLock(struct FileSystemVectorPort *fsvp,
	                           int32 *result2, 
	                           struct Lock *lock1, struct Lock *lock2);

   FUNCTION
	This action compares the targets of two locks, if they are on the 
	same object, ACTION_SAME_LOCK will return DOSTRUE.
	If the two locks are on different objects, return FALSE.
	Both locks are guaranteed to never be NULL. 
	
   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	lock1   - (struct Lock *) Pointer to the first object lock.
	lock2   - (struct Lock *) Pointer to the second object lock.
	RESULT1 - (int32) DOSTRUE / FALSE  (DOSTRUE for same object)
	RESULT2 - (int32) Failure code if RESULT1==FALSE and an error occured.


   INPUTS    (DosPacket method)
	dp_Type - (int32) ACTION_SAME_LOCK
	dp_Arg1 - (BPTR)  lock 1.
	dp_Arg2 - (BPTR)  lock 2.
	RESULT1 - (int32) DOSTRUE / FALSE   (DOSTRUE for same object)
	RESULT2 - (int32) Failure code if RESULT1==FALSE and an error occured.


   SEE ALSO
	IDOS->SameLock()



**/ 


/****************************************************************************/ 
/* EOF */ 



