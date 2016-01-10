/****************************************************************************/ 
/*  _FSLock.c
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


static struct Lock *new_lock(APTR dummy UNUSED,...) 
{
	return 0; /* Write me !  */ 
}



/****************************************************************************/ 


struct Lock * FSLock(struct FSVP *vp, int32 *res2, struct Lock *rel_lock, 
                           CONST_STRPTR obj, int32 mode)
{
	struct GlobalData * gd = vp->FSV.FSPrivate;
	struct ObjLock *reldir = (struct ObjLock *)rel_lock; 
	struct Lock * result;


	IEXEC->ObtainSemaphore(gd->Sem);

	result = new_lock(gd, res2, reldir, obj, mode);

	IEXEC->ReleaseSemaphore(gd->Sem);

	return(result);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_LOCATE_OBJECT - FSLock()

   SYNOPSIS
	struct Lock * RESULT1 = FSLock(struct FileSystemVectorPort *fsvp, 
	                               int32 *result2, struct Lock *rel,
	                               CONST_STRPTR name, int32 mode);

   FUNCTION
	The AmigaDOS functions Lock() and LockTags() use this action.
	Given a name for the object, (which can include a path for dospacket
	calls only), and a relative lock from which to look for the name,
	which can also be NULL for root directory references.
	This action will locate the object within the filesystem and create
	a Lock structure associated with the object.

	For vector-port calls, the 'name' will always be a single object name
	without any path component whatsoever, the reference lock 'rel'
	generally points to the directory where the object 'name' resides.
  
	When the 'rel' reference lock is NULL, then the reference shall always
	be taken as being the root directory of the volume.
	If the 'name' is a empty string (""), the returned lock shall be a copy
	of the reference lock.
	Therefore, if both the reference lock 'rel' is NULL, and the 'name' is
	an empty string, a copy of the root directory lock shall be returned.
	Never assume 'rel' is a directory lock, it may actually be on a file.

	See EXAMPLE section below for example code to handle this correctly.


	The memory for the Lock structure returned in RESULT1 shall be allocated
	by the IDOS->AllocDosObject(DOS_LOCK,...) function and freed by the 
	IDOS->FreeDosObject(DOS_LOCK,...) function upon an ACTION_FREE_LOCK or
	a vector call to FSVP->FSUnLock(). 

	A handler can create an exclusive lock only if there are no other
	outstanding locks on the given file. Once created, an exclusive lock
	prevents any other locks from being created for that file.

	As of V53, exclusive locks on directories have been deprecated, 
	all requests for an exclusive directory lock MUST return a shared lock
	without causing an error, in effect, the exclusive lock mode request
	is simply ignored for directories.
	This is NOT optional, if you fail to implement only shared directory
	lock mode, then the dos.library vector-port and long path handling will
	simply not work in all circumstances.

	The fl_Volume field of the returned Lock structure shall point to the
	DOS device lists volume entry for the volume of the lock, as a BPTR.


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	rel     - (struct Lock *) Pointer to the 'name' reference lock.
	name    - (CONST_STRPTR) Pointer to the object name.(no path component)
	mode    - (int32) SHARED_LOCK, EXCLUSIVE_LOCK.
	RESULT1 - (struct Lock *) Pointer to the lock, NULL on failure.
	RESULT2 - (int32) Failure code if RESULT1 == NULL


   INPUTS    (DosPacket method)
	dp_Type - (int32) ACTION_LOCATE_OBJECT
	dp_Arg1 - (BPTR)  lock.
	dp_Arg2 - (BSTR)  object_name. (may also include path)
	dp_Arg3 - (int32) Mode. SHARED_LOCK or EXCLUSIVE_LOCK.
	dp_Arg4 - <unused> 0
	dp_Arg5 - (BSTR)  nameformat - name format indicator value.
	           only if(dp_Arg2==dp_Arg5) then BSTR's are guaranteed to be
	           nul-terminated. (53.23)
	RESULT1 - (BPTR)  Lock - (ZERO on failure.)
	RESULT2 - (int32) Failure code if RESULT1 == ZERO


   EXAMPLE
	This pseudocode demonstrates how to handle the supplied parameters
	inside the filesystem to cope with all possible argument variations.
	This method should be used in all filesystem functions that accept
	the standard paired relative lock with a string name descriptor.

	It is important to make no assumptions about the supplied arguments,
	failure to follow this resolution method will likely result in failure
	with some combinations, such as attempting to Open() an assignment
	to a file, or root relative specifications and using Lock() to obtain
	a copy of the relative reference lock, or locks on the root directory.

	struct ObjNode *handle_rel_args(struct ObjLock *rel, CONST_STRPTR name)
	{
	    struct ObjNode *reference;
	    struct ObjNode *object = NULL;

	    //
	    //  Handle the 'rel' argument first, to get the reference.
	    //  Make no assumptions about what object type it references. 
	      
	    if( rel )                   // handle valid locks   
	    {
	        reference = rel->node;  // the reference object from lock   
	    }
	    else
	    {
	        reference = ROOT->node; // use root directory when 'rel'=0   
	    }
	    

	    //
	    //  Next, handle the string name argument.
	      
	    if( name[0] )               // scan only with valid obj name here.   
	    {                           // do the directory test in find()   
	        object = find(reference, name); 
	    }
	    else
	    {
	        object = reference;     // "" name means use reference object    
	    }                           // regardless of what it may be.         
	    
	    return(object);
	}


   SEE ALSO
	IDOS->Lock(), IDOS->LockTags(),  ACTION_INHIBIT_DOS_LONGPATH_HANDLING



**/ 


/****************************************************************************/ 
/* EOF */ 



