/****************************************************************************/ 
/*  _FSCreateHardLink.c
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
**  New objects must inherit owner and group attributes from the     
**  parent directory, they must have a current creation timestamp,   
**  the protection mask must be set to EXDF_DEFAULT (0), and the     
**  comment strings shall default to a nul-string.                   
** ==================================================================
*/ 


static int32 do_create_hardlink(APTR dummy UNUSED,...) 
{
	return 0; /* Write me ! */ 
}



/****************************************************************************/ 


int32 FSCreateHardLink(struct FSVP *vp, int32 *res2, struct Lock *rel_dirlock, 
                       CONST_STRPTR link_name, struct Lock *target_obj) 
{
	struct GlobalData *      gd = vp->FSV.FSPrivate;
	struct ObjLock *   dir_lock = (struct ObjLock *)rel_dirlock;
	struct ObjLock *target_lock = (struct ObjLock *)target_obj;
	int32  result;

	IEXEC->ObtainSemaphore(gd->Sem);

	result = do_create_hardlink(gd, res2, dir_lock, link_name, target_lock);

	IEXEC->ReleaseSemaphore(gd->Sem);

	return(result);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_MAKE_LINK - FSCreateSoftLink() - FSCreateHardLink()

   SYNOPSIS
	int32 RESULT1 = FSCreateSoftLink(struct FileSystemVectorPort *fsvp,
	                                 int32 *result2, struct Lock *rel,
	                                 CONST_STRPTR link, CONST_STRPTR target);

	int32 RESULT1 = FSCreateHardLink(struct FileSystemVectorPort *fsvp,
	                                 int32 *result2, struct Lock *rel,
	                                 CONST_STRPTR link, struct Lock *target);

   FUNCTION
	Causes the file system to create a link to a file or directory.
	There are two kinds of links, hard links and soft links.
	The basic difference between them is that a file system resolves a
	hard link itself, while the file system passes a string back to DOS
	telling it where to find a soft linked file or directory.
	To the DOS level programmer, there is essentially no difference
	between referencing a file by its original name or by its link name.
	In the case of a hard link, `target' is a lock on the file or directory
	target that the link is `linked' to, while in a soft link, 
	`target' is a pointer (CPTR) to a C-style path string.

	In an over-simplified model of the file system, when asked to
	locate a file, the system scans a disk looking for a file header with
	a specific (file) name.  That file header points to the actual file
	data somewhere on the disk.  With hard links, more than one file
	header can point to the same file data, so data can be referenced by
	more than one name.  When the user tries to delete a hard link to a
	file, the system first checks to see if there are any other hard
	links to the file.  If there are, only the hard link is deleted, 
	the actual file data the hard link used to reference remains, 
	so the existing hard links can still use it.  
	In the case where the 'real' object (not a hard link) is deleted,
	the file system will make one of its hard links the new `real' object.
	Hard links can exist on directories as well.  Because hard links
	`link' directly to the underlying media, hard links in one file
	system cannot reference objects in another file system.
	Also for hard directory links, the same precautions against creating
	directory loops (like FSRename()) should be observed, in that it should
	be impossible to create a hard directory link that points to a target
	directory above the level of the hard directory link itself,
	return ERROR_OBJECT_EXISTS if this is attempted.

	Soft links are resolved through DOS calls.  When the file system scans
	a disk for a file or directory name and finds that the name is a soft
	link, it returns failure and an error code ERROR_IS_SOFT_LINK.  
	If this happens, the dos.library asks the file system to tell it
	what the link refers to by calling ACTION_READ_SOFT_LINK or the new
	Vector-port function FSReadSoftLink() to obtain the target string.
	Soft Links are stored on the media just like a file, but instead of
	pointing directly to data on the disk, a soft link simply contains a
	string descriptor that is the path and name of the target object which
	may be a link directory relative path or an absolute path and may also
	contain the name of another volume, in which case, softlink can be used
	to reference objects on other disks.

	For vector-port callers, the maximum length of a softlink target string
	is set at 4000 bytes, the target string supplied to the filesystem is
	only stored and nothing more.



   INPUTS    (FileSystemVectorPort method;  FSCreateSoftLink() )
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	rel     - (struct Lock *) Pointer to the 'link' reference lock.
	link    - (CONST_STRPTR) Pointer to the link name.(no path component)
	target  - (CONST_STRPTR) Pointer to the target string.
	RESULT1 - (int32) Success/Failure (DOSTRUE/FALSE) FALSE on error.
	RESULT2 - (int32) Failure code if RESULT1 == FALSE.

	
   INPUTS    (FileSystemVectorPort method;  FSCreateHardLink() )
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	rel     - (struct Lock *) Pointer to the 'link' reference lock.
	link    - (CONST_STRPTR) Pointer to the link name.(no path component)
	target  - (struct Lock *) Pointer to the target reference lock.
	RESULT1 - (int32) Success/Failure (DOSTRUE/FALSE) FALSE on error.
	RESULT2 - (int32) Failure code if RESULT1 == FALSE.



   INPUTS    (DosPacket method)
	dp_Type - (int32) ACTION_MAKE_LINK
	dp_Arg1 - (BPTR)  relative lock (relative to dp_Arg2)
	dp_Arg2 - (BSTR)  name of link to be created.
	dp_Arg3 - (int32) target object ( STRPTR or BPTR ) [*] 
	dp_Arg4 - (int32) Boolean; -1=SOFTLINK or 0=HARDLINK.
	dp_Arg5 - (BSTR)  nametype; name format indicator value.
	                  if(dp_Arg2==dp_Arg5) then BSTR's are
	                  guaranteed to be nul-terminated. (53.23)
	RESULT1 - (int32) Success/Failure (DOSTRUE/FALSE) FALSE on error.
	RESULT2 - (int32) Failure code if RESULT1 == FALSE.

	[*] For soft-link packets, dp_Arg3 = (STRPTR) Target object string.
	    For hard-link packets, dp_Arg3 = (BPTR) A lock to the target object.


	NOTES
	Links are never permitted to destroy any object including other links.
	RESULT1=FALSE, RESULT2=ERROR_OBJECT_EXISTS shall be returned if this
	situation is encountered.

	For Hardlinks, the supplied object reference lock is not harmed in the
	making of this hardlink. Just like the examinations functions, it is
	only used to identify the object, it is never consumed. 


   SEE ALSO
	IDOS->MakeLink(), ACTION_INHIBIT_DOS_LONGPATH_HANDLING



**/ 


/****************************************************************************/ 
/* EOF */ 



