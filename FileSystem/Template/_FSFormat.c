/****************************************************************************/ 
/*  _FSFormat.c
**       
**  Created by FileSystem VectorPort tool. (FSVPTool 53.23) 
**       
**  :ts=4 
**       
*/       
/****************************************************************************/ 


#include "all_includes.h" 


/****************************************************************************/ 



static int32 do_format(APTR dummy UNUSED,...) 
{
	return 0; /* Write me ! */ 
}



/****************************************************************************/ 


int32 FSFormat(struct FSVP *vp, int32 *res2, CONST_STRPTR newname, uint32 dostype)
{
	struct GlobalData *gd = vp->FSV.FSPrivate;
	int32 result;

	IEXEC->ObtainSemaphore(gd->Sem);

	result = do_format(gd,res2,newname,dostype);

	IEXEC->ReleaseSemaphore(gd->Sem);

	return(result);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_FORMAT - FSFormat()

   SYNOPSIS
	int32 RESULT1 = FSFormat(struct FileSystemVectorPort *fsvp,
	                         int32 *result2,
	                         CONST_STRPTR volname,
	                         uint32 dostype);

   FUNCTION
	This function tells a file system to perform any device or file system
	specific formatting on any newly initialized media.  Upon receiving
	this action, a file system can assume that the media has already been
	low level formatted and should proceed to write out any high level
	disk structure necessary to create an empty volume.

	The filesystem absolutely MUST be in an inhibited state before this
	action is called, otherwise a failure code of ERROR_OBJECT_IN_USE must
	be returned by the filesystem, this is also used as a safety interlock,
	so the filesystem must never do anything destructive to the volume
	if this action is called when uninhibited.

	This function shall also freshly serialize the new empty volume to
	perform the functionality of FSSerialize() / ACTION_SERIALIZE_DISK,
	this prevents DOS volume name conflicts with pre-existing volumes.

	Volumes of the same name are permitted, but the datestamps of the
	volume nodes must be different.


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	volname - (CONST_STRPTR) Pointer to the new volume name.
	dostype - (uint32) DosType signature longword.
	RESULT1 - (int32) Success/failure (DOSTRUE/FALSE), FALSE on error.
	RESULT2 - (int32) Failure code if RESULT1 == FALSE


   INPUTS    (DosPacket method)
	dp_Type - (int32)  ACTION_FORMAT
	dp_Arg1 - (BSTR)   Name for volume (if supported).
	dp_Arg2 - (uint32) DosType of format (file system specific)
	dp_Arg3 - <unused> 0
	dp_Arg4 - <unused> 0
	dp_Arg5 - (BSTR)   nametype - name format indicator.
	                   if(dp_Arg1==dp_Arg5) then BSTR's are guaranteed
	                   to be nul-terminated. (53.23)
	RESULT1 - (int32)  Success/Failure (DOSTRUE/FALSE), FALSE on error.
	RESULT2 - (int32)  Failure code if RESULT1 == FALSE




**/ 


/****************************************************************************/ 
/* EOF */ 



