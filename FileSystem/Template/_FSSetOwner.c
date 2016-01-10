/****************************************************************************/ 
/*  _FSSetOwner.c
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


static int32 do_setowner(APTR dummy UNUSED,...) 
{
	return 0; /* Write me ! */ 
}



/****************************************************************************/ 


int32 FSSetOwner(struct FSVP *vp, int32 *res2, struct Lock *dirlock, 
                                 CONST_STRPTR objname, uint32 owner) 
{
	struct GlobalData *gd = vp->FSV.FSPrivate;
	struct ObjLock * lock = (struct ObjLock *)dirlock;
	int32 result;

	IEXEC->ObtainSemaphore(gd->Sem);

	result = do_setowner(gd,res2,lock,objname,owner);

	IEXEC->ReleaseSemaphore(gd->Sem);

	return(result);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_SET_OWNER_INFO - FSSetGroup() - FSSetOwner()

	int32 RESULT1 = FSSetGroup(struct FileSystemVectorPort *fsvp, 
	                           int32 *result2, struct Lock *rel,
	                           CONST_STRPTR name, uint32 mask);

	int32 RESULT1 = FSSetOwner(struct FileSystemVectorPort *fsvp, 
	                           int32 *result2, struct Lock *rel,
	                           CONST_STRPTR name, uint32 mask);

   FUNCTION
	These functions replace the deprecated V39 ACTION_SET_OWNER.
	It allows the caller to set the owner or group information for
	the file or directory.

	A filesystem takes no interest in the values other than storing 
	(and returning) them, for the respective filesystem objects,
	these are primarily for use by networking software (clients and hosts),
	in conjunction with the ExamineData EXDF_OTR_xxx and EXDF_GRP_xxx 
	protection bits.


	Some filesystems previously stored the mask for both group and owner in
	the same 32 bit longword as; owner uid (31:16) and group id (15:0)
	if the filesystem has room, these two masks should be stored in their
	own separate 32 bit wide fields.

	For dospacket calls, there are two tags of interest to the filesystem,
	these are;
	OI_OwnerUID (uint32) -- Owner UID value.
	    Set the value for the objects UID field. 
	    By not specifying this tag, the UID field will remain unchanged.

	OI_OwnerGID (uint32) -- Group GID value.
	    Set the value for the objects GID field. 
	    By not specifying this tag, the GID field will remain unchanged.

	There are no default values for these tags, the caller must specify
	at least one of OI_OwnerUID or OI_OwnerGID tags, or it shall fail.
	The dos.library will check for this before calling the filesystem
	as well as testing for a NULL 'tags' arg.

	The dos.library source tags are NOT filtered from the taglist, so the
	filesystem must ignore all tags other than the two listed above.

	For vector-port callers, FSSetGroup() and/or FSSetOwner() will be 
	called depending on the arguments supplied by the caller.

	To handle the 'rel' and 'name' parameters correctly, use the
	example code shown in ACTION_LOCATE_OBJECT / FSLock() autodoc.


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	rel     - (struct Lock *) Pointer to the 'name' reference dir lock.
	name    - (CONST_STRPTR) Pointer to the object name.(no path component)
	mask    - (uint32) The group or owner bitmask, each are 32 bits wide.
	RESULT1 - (int32) Success/failure (DOSTRUE/FALSE) (FALSE on error).
	RESULT2 - (int32) Failure code if RESULT1 == FALSE


   INPUTS    (DosPacket method)
	dp_Type - (int32) ACTION_SET_OWNER_INFO
	dp_Arg1 - (BPTR)  Relative Lock
	dp_Arg2 - (BSTR)  name
	dp_Arg3 - (struct TagItem *) taglist
	dp_Arg4 - <unused> 0
	dp_Arg5 - (BSTR)  nametype - name format indicator value.
	                  if(dp_Arg2==dp_Arg5) then BSTR's are guaranteed to
	                  be nul-terminated. (53.23)
	RESULT1 - (int32) Success/failure (DOSTRUE/FALSE) (FALSE on error).
	RESULT2 - (int32) Failure code if RESULT1 == FALSE


   SEE ALSO
	IDOS->SetOwnerInfo(), ACTION_INHIBIT_DOS_LONGPATH_HANDLING



**/ 


/****************************************************************************/ 
/* EOF */ 



