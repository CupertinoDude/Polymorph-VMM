/****************************************************************************/ 
/*  _FSChangeLockMode.c
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


int32 FSChangeLockMode(struct FSVP *vp, int32 *res2, struct Lock *lock_in, int32 newmode)
{
	struct GlobalData *gd = vp->FSV.FSPrivate;
	struct ObjLock  *lock = (struct ObjLock *)lock_in;
	int32          result = FALSE;

	IEXEC->ObtainSemaphore(gd->Sem);

	do
	{
		if( NO lock )
		{
			(*res2) = ERROR_REQUIRED_ARG_MISSING;
			break;
		}


		if( EXCLUSIVE_LOCK == newmode ) 
		{
			/*
			**  We never allow dirs to become exclusive, 
			**  hard directory links included.
			*/
			if( FSO_TYPE_DIRECTORY == (lock->node->type & FSO_TYPE_MASK) )
			{
				(*res2) = ERROR_OBJECT_WRONG_TYPE;
				break;
			}


			/*
			**  If caller already has exclusive access, give caller anything
			**  but do this only if the existing object is a shared lock.
			*/
			if( SHARED_LOCK == lock->lock.fl_Access )
			{
				struct ObjLock *ol;

				/* 
				**  Caller has a shared lock and trying for an exclusive lock.
				**  To do this we can not allow any other locks on it.
				**  Scan the locklist for a different lock but with the same node.
				*/ 
				for( ol = BADDR(gd->lock_list); ol; ol = BADDR(ol->lock.fl_Link) )
				{
					if( ol != lock )  
					{
						if( ol->node == lock->node )
						{
							break;
						}
					}
				}

				if( ol )   /* we found another one */ 
				{
					(*res2) = ERROR_OBJECT_IN_USE;
					break;
				}
			}
		}


		lock->lock.fl_Access = newmode;

		result = DOSTRUE;
		(*res2) = 0;

	}
	while(0);

 	IEXEC->ReleaseSemaphore(gd->Sem);
	return(result);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_CHANGE_MODE - FSChangeLockMode() - FSChangeFileMode()

   SYNOPSIS
	int32 RESULT1 = FSChangeLockMode(struct FileSystemVectorPort *fsvp,
                      int32 *result2, struct Lock *lock, int32 newmode);

	int32 RESULT1 = FSChangeFileMode(struct FileSystemVectorPort *fsvp,
                int32 *result2, struct FileHandle *file, int32 newmode);
	
   FUNCTION
	This action requests that the handler change the mode of the given
	file handle or file lock to the new mode specified.  

	This request should fail if the handler can't change the mode as
	requested, for example an exclusive request for a file that has
	multiple users already.

	Attempting to change a directory lock to an exclusive lock MUST cause
	failure from V53 onwards.  This is NOT optional, if you fail to 
	implement only the shared directory lock mode, then the dos.library
	vector-port and path handling code will not work properly.

	If an objects mode is already the same as being requested, a success
	code should be returned even if no actual operation was performed.


   INPUTS    (FileSystemVectorPort method) - FSChangeLockMode()
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	lock    - (struct Lock *) Pointer to the struct lock.
	newmode - (int32) New mode for object; SHARED_LOCK or EXCLUSIVE_LOCK
	RESULT1 - (int32) Success / Failure indicator.
	RESULT2 - (int32) Failure code if RESULT1 == FALSE.


   INPUTS    (FileSystemVectorPort method) - FSChangeFileMode()
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	file    - (struct FileHandle *) Pointer to the file handle.
	newmode - (int32) New mode for file; SHARED_LOCK or EXCLUSIVE_LOCK
	RESULT1 - (int32) Success / Failure indicator.
	RESULT2 - (int32) Failure code if RESULT1 == FALSE.


   INPUTS    (DosPacket method)
	dp_Type - (int32) ACTION_CHANGE_MODE
	dp_Arg1 - (int32) object_type_id; CHANGE_FH or CHANGE_LOCK.
	dp_Arg2 - (BPTR)  filehandle or lock as identified by dp_Arg1.
	dp_Arg3 - (int32) Newmode of object; SHARED_LOCK or EXCLUSIVE_LOCK.
	RESULT1 - (int32) Success/Failure (DOSTRUE/FALSE), FALSE on error.
	RESULT2 - (int32) Failure code if RESULT1 == FALSE


   SEE ALSO
	IDOS->ChangeMode()



**/ 


/****************************************************************************/ 
/* EOF */ 



