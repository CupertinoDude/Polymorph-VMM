/****************************************************************************/ 
/*  _FSSetProtection.c
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
**  The FSetXXXX() functions must also cause a change update, however,
**  this must begin at the objects parent dir and not on the object,
**  as this will interfere with multiple successive attribute changes 
**  for operations such as clone file copying. 
** ==================================================================
*/ 


static int32 do_setprotection(APTR dummy UNUSED,...) 
{
	return 0; /* Write me ! */ 
}



/****************************************************************************/ 


int32 FSSetProtection(struct FSVP *vp, int32 *res2, struct Lock *dirlock, 
                                 CONST_STRPTR objname, uint32 mask) 
{
	struct GlobalData *gd = vp->FSV.FSPrivate;
	struct ObjLock * lock = (struct ObjLock *)dirlock;
	int32 result;

	IEXEC->ObtainSemaphore(gd->Sem);

	result = do_setprotection(gd,res2,lock,objname,mask);

	IEXEC->ReleaseSemaphore(gd->Sem);

	return(result);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_SET_PROTECT  -  FSSetProtection()

   SYNOPSIS
	int32 RESULT1 = FSSetProtection(struct FileSystemVectorPort *fsvp, 
	                                int32 *result2, struct Lock *rel,
	                                CONST_STRPTR name, uint32 mask);

   FUNCTION
	This action allows the caller to modify the protection bits of a
	file or directory.  The 4 lowest order bits (RWED) are a bit peculiar
	due to ancient compatibility issues.  
	If their respective bit is set, that operation is not allowed
	(i.e. if a file's delete bit is set the file is not deleteable).

	By default, objects are created with a mask value of EXDF_DEFAULT,
	this is currently a value of zero for a mask that defines an object
	as; (R)eadable, (W)riteable, (E)xecutable, (D)eletable.

	Any operation which modifies an object is required to clear the
	(A)rchive bit, this includes at least the immediate parent directory
	for the modified object.

	For files, the (R)ead, (W)rite and (D)elete bits control filesystem
	functionality for their respective operations.

	For directories, the (D)elete bit is the ONLY bit that has any defined
	function for filesystems, the other bits shall not have any affect on
	any filesystem operations until new functionality is formally ratified.
	Do NOT cook-up your own (however well intensioned) functionality for the
	undocumented bits, it is guaranteed to break application compatibility.

	To handle the 'rel' and 'name' parameters correctly, use the
	example code shown in ACTION_LOCATE_OBJECT / FSLock() autodoc.


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	rel     - (struct Lock *) Pointer to the 'name' reference dir lock.
	name    - (CONST_STRPTR) Pointer to the object name.(no path component)
	mask    - (uint32) Protection bitmask.
	RESULT1 - (int32)  Success/failure (DOSTRUE/FALSE) (FALSE on error).
	RESULT2 - (int32)  Failure code if RESULT1 == FALSE


   INPUTS    (DosPacket method)
	dp_Type - (int32)  ACTION_SET_PROTECT
	dp_Arg1 - <unused> 0
	dp_Arg2 - (BPTR)   Lock to which dp_Arg3 is relative.
	dp_Arg3 - (BSTR)   Name of object (relative to dp_Arg2)
	dp_Arg4 - (uint32) Mask of new protection bits.
	dp_Arg5 - (BSTR)   Nametype - name format indicator value.
	                   if(dp_Arg3==dp_Arg5) then BSTR's are
	                   guaranteed to be nul-terminated. (53.23)
	RESULT1 - (int32)  Success/failure (DOSTRUE/FALSE) (FALSE on error).
	RESULT2 - (int32)  Failure code if RESULT1 == FALSE


   SEE ALSO
	IDOS->SetProtection()



**/ 


/****************************************************************************/ 
/* EOF */ 



