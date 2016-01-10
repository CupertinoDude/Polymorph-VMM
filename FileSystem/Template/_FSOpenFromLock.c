/****************************************************************************/ 
/*  _FSOpenFromLock.c
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
**  When filling out a FileHandle structure, fill in both Arg fields.
**  1) fh_Arg1 should be maintained and stored as a BPTR for backward
**     dospacket compatibility.
**  2) For vector-port filesystems we use fh_Arg2 as an APTR to the 
**     internal lock structure representation. 
**  3) To simplify dismounting, a filehandle structure pointer should 
**     also be stored in a field of the internal lock structure,
**     this allows it to be easily found from the lock list. 
** ==================================================================
*/ 


int32 FSOpenFromLock(struct FSVP *vp, int32 *res2, struct FileHandle *file, struct Lock *lockin)
{
	struct GlobalData *gd = vp->FSV.FSPrivate;
	struct ObjLock  *lock = (struct ObjLock *)lockin;
	int32          result = FALSE;
	struct ObjNode *node;

	IEXEC->ObtainSemaphore(gd->Sem);

	if( file && lock )
	{
		node = lock->node; 

		/* 
		** Here we simply consume their supplied file lock 
		** and convert it to a file handle. 
		** Make absolutely sure the lock is on a file. 
		** Also, make sure the handle is stored back in the lock 
		** struct so it can be accessed when dismounting. 
		*/ 
		if( FSO_TYPE_FILE == node->type )
		{
			file->fh_Arg1 = MKBADDR(lock); /* Setup the handle for legacy packet emulation */ 
			file->fh_Arg2 = lock;          /* Setup the handle for internal access */ 
			lock->fh = file;               /* Store the handle in the lock struct. */ 

			result = DOSTRUE; 
		}
		else
		{
			(*res2) = ERROR_OBJECT_WRONG_TYPE;  /* can't 'open' a dir. */ 
		}
	}
	else
	{
		(*res2) = ERROR_REQUIRED_ARG_MISSING;
	}

	IEXEC->ReleaseSemaphore(gd->Sem);

	return (result);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_FH_FROM_LOCK - FSOpenFromLock()

   SYNOPSIS
	int32 RESULT1 = FSOpenFromLock(struct FileSystemVectorPort *fsvp,
	                               int32 *result2, 
	                               struct FileHandle *file,
	                               struct Lock *lock);
   FUNCTION
	This function opens a file from a given file lock.
	If this action is successful, the file system will essentially consume
	the lock so a program cannot use it anymore.

	If this function fails, the lock is still usable.
	Attempting to open a file using a directory lock MUST fail with the
	RESULT2 code set to ERROR_OBJECT_WRONG_TYPE.
 
	The lock mode determins the effective filehandle open mode,
	SHARED_LOCK returns a filehandle mode equivalent to MODE_OLDFILE,
	an EXCLUSIVE_LOCK returns a filehandle mode equivalent to MODE_NEWFILE
	but without modifying the file contents or meta-data, this operation is
	as would occur if opened in MODE_OLDFILE and promoted to MODE_NEWFILE
	with ACTION_CHANGE_MODE or FSChangeFileMode().


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	file    - (struct FileHandle *) Pointer to the file handle structure.
	lock    - (struct Lock *) Pointer to the struct lock on the file.
	RESULT1 - (int32) Success / Failure indicator.
	RESULT2 - (int32) Failure code if RESULT1 == FALSE.


   INPUTS
	dp_Type - (int32) ACTION_FH_FROM_LOCK
	dp_Arg1 - (BPTR)  filehandle structure to fill in.
	dp_Arg2 - (BPTR)  lock on the file to open.
	RESULT1 - (int32) Boolean Success/Failure indicator.
	RESULT2 - (int32) Failure code if RESULT1 == FALSE


   SEE ALSO
	IDOS->OpenFromLock()



**/ 


/****************************************************************************/ 
/* EOF */ 



