/****************************************************************************/ 
/*  _FSRename.c
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


static int32 do_rename(APTR dummy UNUSED,...) 
{
	return 0; /* Write me ! */ 
}



/****************************************************************************/ 


int32 FSRename(struct FSVP *vp, int32 *res2, 
                     struct Lock *src_rel, CONST_STRPTR from_str, 
                     struct Lock *dst_rel, CONST_STRPTR to_str ) 
{
	struct GlobalData *   gd = vp->FSV.FSPrivate;
	struct ObjLock *from_dir = (struct ObjLock *)src_rel;
	struct ObjLock *  to_dir = (struct ObjLock *)dst_rel;
	int32 result;

	IEXEC->ObtainSemaphore(gd->Sem);

	result = do_rename(gd, res2, from_dir, from_str, to_dir, to_str);

	IEXEC->ReleaseSemaphore(gd->Sem);

	return(result);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_RENAME_OBJECT  -  FSRename()

   SYNOPSIS
	int32 RESULT1 = FSRename(struct FileSystemVectorPort *fsvp,
	                         int32 *result2,
	                         struct Lock *src_rel, CONST_STRPTR src,
	                         struct Lock *dst_rel, CONST_STRPTR dst);

   FUNCTION
	This function renames the file, directory or symbolic link specified.
	An error is returned if the operation fails.
	There is a responsibility of the file system to ensure that the
	operation is not going to cause adverse effects.

	If the volume itself is write protected, (check this first)
	return failure and set the error code to; ERROR_DISK_WRITE_PROTECTED.

	The renaming action allows moving files across directory bounds and
	as such must ensure that it doesn't create hidden directory loops by
	renaming a directory into a child of itself. To accomplish this,
	simply iterate backwards (towards the root directory) starting at the
	'dst_rel' relative directory, if the 'src_rel' directory is one of the
	'dst_rel' parents, abort and return failure with the error code set
	to; ERROR_OBJECT_IN_USE.

	This function must also fail if a destination object already exists
	providing it is a different object from the one being renamed,
	return failure with the error code set to; ERROR_OBJECT_EXISTS.

	It is perfectly legal to specify the same object for the source and
	destination strings, but with different case, by default, AmigaDOS
	is case insensitive, so an operation to change case must succeed.
	EG:  IDOS->Rename("sys:foo","sys:FoO");

	Renaming softlinks may not be supported by some dospacket based
	filesystems, some may simply fail a softlink renaming request by
	returning FALSE with the secondary error code ERROR_IS_SOFT_LINK.
	It is mandatory that vector-port based filesystems support both
	changing the name of a softlink and moving it outside its original
	directory if requested, regardless of whether it adversely affects
	the softlink target path validity.

	To handle the relative dir and name string parameters correctly,
	use the example code shown in ACTION_LOCATE_OBJECT / FSLock() autodoc.


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	src_rel - (struct Lock *) Pointer to the source dir lock.
	src     - (CONST_STRPTR)  Pointer to the name of the source object.
	dst_rel - (struct Lock *) Pointer to the destination dir lock.
	dst     - (CONST_STRPTR)  Pointer to the name of the destination object.
   RESULT1 - (int32) Boolean Success/Failure indicator, (FALSE on failure.)
   RESULT2 - (int32) Failure code if RESULT1 == FALSE


   INPUTS    (DosPacket method)
	dp_Type - (int32) ACTION_RENAME_OBJECT
	dp_Arg1 - (BPTR)  relative lock for dp_Arg2. (source)
	dp_Arg2 - (BSTR)  source object name. (may also include path)
	dp_Arg3 - (BPTR)  relative lock for dp_Arg4. (destination)
	dp_Arg4 - (BSTR)  destination object name. (may also include path)
	dp_Arg5 - (BSTR)  nameformat - name format indicator value,
	           only if(dp_Arg2==dp_Arg5) then BSTR's are guaranteed to be
	           a nul-terminated string. (53.23)
   RESULT1 - (int32) Boolean Success/Failure indicator, (FALSE on failure.)
   RESULT2 - (int32) Failure code if RESULT1 == FALSE


   NOTES
	The fast file system currently checks the write protection bit of
	the respective directories and fails the operation if they have the
	write protection bit enabled.  However, only the (D)elete protection
	bit has any meaning for directories on the old ROM filesystem and
	other third party filesystems.


   SEE ALSO
	IDOS->Rename(), IDOS->SetProtection()



**/ 


/****************************************************************************/ 
/* EOF */ 



