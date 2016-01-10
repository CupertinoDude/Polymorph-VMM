/****************************************************************************/ 
/*  _FSOpen.c
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

/* 
** ==================================================================
**  New objects must inherit owner and group attributes from the     
**  parent directory, they must have a current creation timestamp,   
**  the protection mask must be set to EXDF_DEFAULT (0), and the     
**  comment strings shall default to a nul-string.                   
** ==================================================================
*/ 


static struct ObjLock *do_openfile(APTR dummy UNUSED,...) 
{
	/* 
	** When creating a new object set the lock change update to TRUE 
	** so that when the FSClose()/FSUnLock() function is called, 
	** the update will occur. 
	*/ 
//	if( new_object ) 
//	{ 
//		lock->updated = TRUE; 
//	} 
	return 0; /* Write me ! */ 
}



/****************************************************************************/ 


int32 FSOpen(struct FSVP *vp, int32 *res2, struct FileHandle *file, 
                   struct Lock *rel_dir, CONST_STRPTR obj, int32 mode)
{
	struct GlobalData    *gd = vp->FSV.FSPrivate;
	struct ObjLock *rel_lock = (struct ObjLock *)rel_dir;
	struct ObjLock   *result = NULL;

	IEXEC->ObtainSemaphore(gd->Sem);

	result = do_openfile(gd, res2, file, rel_lock, obj, mode);

	if( result ) 
	{ 
		/* 
		** Make sure the filehandle structure is filled out correctly. 
		*/ 
		file->fh_Arg1 = MKBADDR(result); /*-- Setup the handle for legacy packet emulation.*/ 
		file->fh_Arg2 = result;          /*-- Setup the handle for internal access. */ 
		result->fh    = file;            /*-- Store the handle ptr in the lock struct, */ 
	}	                                 /*   this is required when dismounting. */ 
	

	IEXEC->ReleaseSemaphore(gd->Sem);

	return (result) ? DOSTRUE : FALSE;
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_FINDxxxx - FSOpen()

	ACTION_FINDINPUT   aka   MODE_OLDFILE
	ACTION_FINDOUTPUT  aka   MODE_NEWFILE
	ACTION_FINDUPDATE  aka   MODE_READWRITE

   SYNOPSIS
	int32 RESULT1 = FSOpen(struct FileSystemVectorPort *fsvp, 
	                       int32 *result2, struct FileHandle *fh,
	                       struct Lock *rel, CONST_STRPTR name, 
	                       int32 mode);
   
   FUNCTION
	This action opens a stream to a file or a stream handler, and will use
	the supplied 'fh' (filehandle) to do so.

	For filesystems, the 'rel' lock and a 'name' arguments identify the
	object to open, this action must correctly support all argument methods
	currently in use, by using the relative arguments example code that can
	be found in the autodoc for ACTION_LOCATE_OBJECT / FSLock(), maximum
	compatibility for all string input functions will be guaranteed.
	
	The input arguments may appear in any of the following ways to identify
	a filesystem object, all three methods must be supported:

	(1) If the 'rel' lock is NULL, then the file 'name' is to be taken as
	    being relative to the root directory of this filesystem.

	(2) If the 'rel' lock is NON-NULL, this usually identifies the directory
	    where the file 'name' is to be located, providing the 'rel' lock is
	    really a directory lock, never assume this lock will always be so.

	(3) If the 'name' is an empty string (""), and the 'rel' lock is a
	    NON-NULL lock on a file, then that file shall be the object opened.
	    The 'rel' lock is not consumed, it remains a valid reference lock
	    and therefore this mode will fail if used with the exclusive open
	    mode. (MODE_NEWFILE).

	For all Vector-Port function calls, 'name' never has path components,
	the dos.library parses it completely to the single final component name
	before calling the vector-port function.

	For DosPacket callers (only), file names are not limited to a single 
	file name but instead can include a volume name (followed by a colon)
	and multiple slashes requiring the file system to resolve the object name.
	This eliminated the need for AmigaDOS or the application to parse names
	before sending them to the file system, unfortunately this also means
	that every filesystem must parse every dospacket name string to resolve
	it to a single object name.


	The calling program owns the file handle.  
	In the case of a call to the DOS Open() function, AmigaDOS allocates
	the file handle automatically and then frees it in Close().

	The file handle must be initialized before trying to open anything,
	the Open() function will initialise the fh_MsgPort field with a pointer
	to the MsgPort (Vector-Port) of the handler process, don't clear it.!!

	Lastly, the handler itself must initialize ONLY fh_Arg1/fh_Arg2 with
	something that allows the handler to identify the object being opened,
	this value is an implementation specific kind of "lock", but for various
	compatibility and interoperability purposes, fh_Arg1 should always be
	used as a BPTR to this custom "lock", while fh_Arg2 should always be
	an APTR to the same "lock".
	The reason for this is because for DosPacket based operations, only
	fh_Arg1 is passed as the primary argument to locate the object for 
	the operations that perform various READ/WRITE/CHANGE/END/TRUNCATE
	functions, only this value was available and not the file handle itself.

	However, for Vector-Port calls, the original file handle pointer is
	supplied, therefore to avoid the BPTR conversions for non-dospacket
	based filesystems, the internal "lock" should be stored in fh_Arg2.  
	The filesystem should also store the file handle pointer itself in the
	internal "lock" structure too, so it can be found from the filesystems
	"lock" list, for dismounting purposes.(if supported).


	FINDINPUT (MODE_OLDFILE) will only succeed if the file already exists,
	the file will be opened with a shared lock.  Failure to locate the 
	file must return failure plus the error code ERROR_OBJECT_NOT_FOUND.
	This opening mode is not affected by the protection bits or the disk
	write protection status.

	FINDUPDATE (MODE_READWRITE) will also open it with a shared lock
	but if the file doesn't exist, FINDUPDATE will create a new file.
	The disk write protection status must be observed with this mode 
	when creating new files, the protection bits and the disk write
	protection status have no effect on opening an existing file.

	FINDOUTPUT (MODE_NEWFILE) will open a new file with an exclusive lock.
	It will effectively delete an existing file before creating a new file.
	(See; NOTES regarding semantics for hardlinks)
	With this mode, the disk write protection status must first be observed,
	along with the existing files (D)elete protection status.
	If the existing file is delete protected, failure shall be returned
	with a error code of ERROR_DELETE_PROTECTED.  If the disk is write
	protected the error code shall be ERROR_DISK_WRITE_PROTECTED.


	All modes attempting to open on a directory object shall always return
	failure, the error code must be ERROR_OBJECT_WRONG_TYPE.

	All modes attempting to open a softlink shall always return failure,
	the error code must be ERROR_IS_SOFT_LINK.

	On success, all modes above will place the internal file position
	pointer at the beginning of the file.


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	fh      - (struct FileHandle *) Pointer to the filehandle struct.
	rel     - (struct Lock *) Pointer to the reference lock for 'name'.
	name    - (CONST_STRPTR) Pointer to the object name.(no path component)
	mode    - (int32) MODE_OLDFILE, MODE_NEWFILE, MODE_READWRITE.
	RESULT1 - (int32) Boolean Success/Failure - (DOSTRUE or FALSE)
	RESULT2 - (int32) Failure code if RESULT1 == FALSE


   INPUTS    (DosPacket method)
	dp_Type - (int32) ACTION_FINDxxxx
	dp_Arg1 - (BPTR)  fh    (filehandle). 
	dp_Arg2 - (BPTR)  rel   (lock)
	dp_Arg3 - (BSTR)  name. (string)
	dp_Arg4 - <unused> 0
	dp_Arg5 - (BSTR)  nameformat - name format indicator value.
	           if(dp_Arg3==dp_Arg5) then BSTR's are guaranteed to be
	           a nul-terminated extended BSTR. (53.23)
	RESULT1 - (int32) Boolean Success/Failure - (DOSTRUE or FALSE)
	RESULT2 - (int32) Failure code if RESULT1 == FALSE


   NOTES
	When using MODE_NEWFILE on filesystems that support hard file links,
	be aware that doing so with an existing hard file link name is
	equivalent to opening the target file itself in MODE_NEWFILE.
	However, because that target file has hardlinks bound to it, 
	the implied "delete first" behaviour of MODE_NEWFILE means that the
	target file itself must not be physically "deleted" in the conventional
	sense, as that would disconnect the hard file linkage.
	Therefore only the content and associated data of the target file will
	be reset to that of a newly created file, where it has the default
	protection attributes set to EXDF_DEFAULT, a size of zero bytes, 
	no comment string and a fresh creation datestamp, without affecting
	the existing hard file linkage.
	The default owner and user values for new files shall be inherited
	from the parent directory.


   SEE ALSO
	IDOS->Open(), ACTION_INHIBIT_DOS_LONGPATH_HANDLING



**/ 


/****************************************************************************/ 
/* EOF */ 



