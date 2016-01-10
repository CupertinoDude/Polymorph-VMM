/****************************************************************************/ 
/*  _FSFileSystemAttr.c
**       
**  Created by FileSystem VectorPort tool. (FSVPTool 53.23) 
**       
**  :ts=4 
**       
*/       
/****************************************************************************/ 


#include "all_includes.h" 


/****************************************************************************/ 




int32 FSFileSystemAttr(struct FSVP *vp, int32 *res2, struct TagItem *const_tags)
{
	struct GlobalData *gd = vp->FSV.FSPrivate;
	struct TagItem *taglist, *tag;
	uint32 *data;
	STRPTR cp;
	int32  bufsize, len;
	int32  rc1 = DOSTRUE; /* default success */ 
	int32  rc2 = 0; 


	IEXEC->ObtainSemaphoreShared(gd->Sem);

	taglist = (APTR) const_tags; 

	while( (tag = IUTILITY->NextTagItem(&taglist)) ) 
	{
		data = (uint32 *)tag->ti_Data; 

		switch(tag->ti_Tag)
		{
			case	FSA_MaxFileNameLengthR: /* (uint32 *) -- Obtain the maximum filename length. (including /0) */ 
					(*data) = MAX_FILENAME; 
					break; 


			case	FSA_VersionNumberR:     /* (uint32 *) -- Obtain the version/rev number for the filesystem. */ 
					(*data) = (VERSION << 16) | REVISION; 
					break; 


			case	FSA_DOSTypeR:           /* (uint32 *) -- Obtain the dostype identifier for the filesystem. */ 
					(*data) = gd->volume_node->vn_DiskType; 
					break; 


			case	FSA_VersionStringR:     /* (STRPTR) -- Obtain a copy of the version string for the filesystem. */ 
					bufsize = IUTILITY->GetTagData(FSA_VersionStringR_BufSize,0,const_tags); 
					len = sizeof(VSTRING); /* sizeof includes the 0 byte. */ 

					if( len > bufsize )
					{
						rc2 = ERROR_LINE_TOO_LONG;
						rc1 = FALSE;
						break;
					}
					IUTILITY->Strlcpy((STRPTR)data, VSTRING, bufsize);
					/* trim off newlines etc... */ 
					for(cp=(STRPTR)data; *cp; cp++)
					{
						if(( '\n' == *cp ) || ('\r' == *cp )) 
						{
							(*cp) =0;
							break;
						}
					}
					break;


		/* these return 0 / FALSE */ 
			case	FSA_HasRecycledEntriesR:     /* (int32 *) -- Get boolean value if filesystem supports recycled entries. */ 
			case	FSA_ActivityFlushTimeoutR:   /* (uint32 *) -- Get the mS (1000th second) before a flush while active occurs. */ 
			case	FSA_InactivityFlushTimeoutR: /* (uint32 *) -- Get the mS (1000th second) before a flush when inactive occurs. */ 
			case	FSA_MaxRecycledEntriesR:     /* (uint32 *) -- Obtain the number of recycled entries supported. */ 
					(*data) = 0; 
					break; 


		/* these cause unimplemented failure here */ 
			case	FSA_MaxRecycledEntriesW:     /* (uint32) -- Set the number of recycled entries supported. */ 
			case	FSA_InactivityFlushTimeoutW: /* (uint32) -- Set the mS (1000th second) before a flush when inactive occurs. */ 
			case	FSA_ActivityFlushTimeoutW:   /* (uint32) -- Set the mS (1000th second) before a flush while active occurs. */ 
			case	FSA_MaxFileNameLengthW:      /* (uint32) -- Set the maximum filename length. (including /0) */ 
					rc2 = ERROR_NOT_IMPLEMENTED; 
					rc1 = FALSE; 
   					break; 


		/* these are ignored here - most were for DOS library */ 
			case	FSA_VersionStringR_BufSize:  /* gotten previously */ 
			case	FSA_StringNameInput:
    		case	FSA_FileHandleInput:
    		case	FSA_LockInput:
    		case	FSA_MsgPortInput:
	    			/* fall through */ 

			default:
				break;

		}


		if( NO rc1 )
		{
			break;	/* an error occured, abort processing the rest */ 
		}

	}
	/* while */ 


	(*res2) = rc2; 
	IEXEC->ReleaseSemaphore(gd->Sem);

	return(rc1);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_FILESYSTEM_ATTR - FSFileSystemAttr()

   SYNOPSIS
	int32 RESULT1 = FSFileSystemAttr(struct FileSystemVectorPort *fsvp, 
	                        int32 *result2, struct TagItem *taglist);

   FUNCTION
	This packet allows access to various attributes of a given filesystem
	that is mounted on top of a disk driver, such as a hard or floppy disk.  

	The ACTION_FILESYSTEM_ATTR function provides a simple method for reading 
	parameters or setting values for filesystems supporting this feature.
	This function will only succeed if the underlying filesystem expressly
	supports a particular option, otherwise, it will return a failure code,
	(unless dos.library can supply the required information.)

	This function works by using a taglist to supply or obtain information,
	the taglist items are passed through to the underlying filesystem, 
	the options are defined in such a way, as to provide specific tags that
	are used in either read or write mode.

   WARNING
	The filesystem shall reject values that are not within tollerance for
	that particular filesystem, it will simply abort further processing of
	any remaining tags when any value supplied is not within the allowable
	range.

	NB:
	The dos.library may also be able to extrapolate some required data if
	this function is not supported by the filesystem, in such cases, 
	dos.library will only attempt to do so, where indicated, and only after
	it has determined that the filesystem cannot supply this information.
	There is also no guarantee that dos.library will succeed in doing this.


   INPUTS
	tags -- Pointer to a tagitem array for the options listed below.

   SOURCE TAGS ; Mandatory tag item to enable filesystem identification.
	              The following four tags are mutually exclusive and
	              are not passed through to the filesystem itself.

	FSA_StringNameInput (STRPTR) -- Pointer to a string identifier.
	    (See GetDeviceProcFlags() for details about this.)

	FSA_FileHandleInput (BPTR) -- BCPL pointer to a FileHandle.
	    Identify the filesystem by the stream pointed to by a FileHandle.
	    The opening mode does not affect identification.

	FSA_LockInput   (BPTR) -- BCPL pointer to a Lock.
	    Identify the filesystem by this associated Lock.
	    The lock mode does not affect the identification.

	FSA_MsgPortInput   (struct MsgPort *) -- Pointer to a message port.
	    Identify the filesystem by this associated message port.
	    ( NOTE: This tag was added in 51.96 )

                                   ===

   READ TAGS ; The taglist options to Read filesystem information.

	FSA_MaxFileNameLengthR (uint32 *) -- Pointer to a uint32 storage area.
	    Obtain the maximum filename length permitted. (not including \0) 
	      ( DOS library will attempt to extrapolate this value if the 
	        filesystem does not support this option. )

	FSA_DOSTypeR (uint32 *) -- Pointer to a uint32 storage area.
	    Obtain the dostype identifier for the filesystem. 
	    EG: 0x444F5303 = 'DOS\3'

	FSA_VersionNumberR (uint32 *) -- Pointer to a uint32 storage area.
	    Obtain the filesystem version number as; (MAJOR<<16) | MINOR)

	FSA_VersionStringR (STRPTR) -- Pointer to a string buffer area.
	    Obtain a copy of the version string for the filesystem.
	    The version string will be the full version string in the same
	    format as the 'bumprev' tools VSTRING, this being; 
	        "filesystem_name VER.REV (dd.mm.yyyy)"

	    ( DOS library will attempt to extrapolate this value if the 
	      filesystem does not support this option, by searching for a
	      $VER: cookie in the device nodes seglist. Note that the result
	      buffer will have $VER: part removed, if one is found. )

	    You are required to specify the size of your string buffer area
	    with the following sub-tag; FSA_VersionStringR_BufSize
	    Failing to supply this tag will cause ERROR_REQUIRED_ARG_MISSING
	    to be returned by IoErr(). 
	    (Dos.library tests for this before calling the filesystem)

	    FSA_VersionStringR_BufSize (uint32) -- Size of the buffer pointed
	       to by the FSA_VersionStringR tag. Recommended minimum; 60 bytes.
	       Note: You are guaranteed that no more than this number of bytes
	             will be written to your buffer area, if the storage area
	             is smaller than the version string, it will be truncated.
	             Passing a value of zero will fail with ERROR_BAD_NUMBER.
	             (Dos.library test for zero before calling the filesystem)

	FSA_ActivityFlushTimeoutR (uint32 *) --Pointer to a uint32 storage area.
	    Obtain the time in milliseconds before a flush while active occurs.
	    ( 1000mS = 1 second)

	FSA_InactivityFlushTimeoutR	(uint32 *) --Pointer to uint32 storage area.
	    Obtain the time in milliseconds before a flush when inactive occurs.
	    ( 1000mS = 1 second)

	FSA_HasRecycledEntriesR	(int32 *) -- Pointer to a int32 storage area.
	    Boolean test to see if the filesystem supports recycled entries.
	    Note: This tag exists because the following FSA_MaxRecycledEntriesR
	          tag allows for zero to indicate a valid number.

	FSA_MaxRecycledEntriesR (uint32 *) -- Pointer to a uint32 storage area.
	    Obtain the number of recycled entries supported by the filesystem.
	    Note: This may be zero, even if recycled entries are supported.

                                   ===

   TAGS ; The taglist options to Set features for the filesystem.

	FSA_MaxFileNameLengthW (uint32) -- New value to set in filesystem.
	    Set the maximum filename length. (not including \0)
	    Note; this option may have limited support, if any.

	FSA_ActivityFlushTimeoutW (uint32) -- New value to set in filesystem.
	    Set the time in milliseconds before a flush while active occurs.
	    ( 1000mS = 1 second)

	FSA_InactivityFlushTimeoutW	(uint32) -- New value to set in filesystem.
	    Set the time in milliseconds before a flush when inactive occurs.
	    ( 1000mS = 1 second)

	FSA_MaxRecycledEntriesW (uint32) -- New value to set in filesystem.
	    Set the number of recycled entries supported by the filesystem.


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	taglist - (struct TagItem *) Pointer to the taglist.
	RESULT1 - (int32) Success/Failure (DOSTRUE/FALSE), FALSE on error.
	RESULT2 - (int32) Failure code if RESULT1 == FALSE


   INPUTS    (DosPacket method)
	dp_Type - (int32) ACTION_FILESYSTEM_ATTR
	dp_Arg1 - (struct TagItem *) taglist
	RESULT1 - (int32) Success/Failure (DOSTRUE/FALSE), FALSE on error.
	RESULT2 - (int32) Failure code if RESULT1 == FALSE


   NOTES
	If the filesystem does not support a particular option, it will return
	FALSE in RESULT1 and ERROR_NOT_IMPLEMENTED in RESULT2.

	This function will return FALSE if a file system option requested
	could not be obtained or an option is not relevant for the specified
	filesystem, or the value specified in a (W)rite operation was illegal.

	From DOS 51.50 the input source tags are filtered out before calling
	the filesystem.

	The dos.library will pre-test common errors such as a NULL buffer
	pointer and zero length parameter for FSA_VersionStringR, which will
	result in the function being aborted before calling the filesystem.

	If the filesystem does not support a particular option, it should 
	immediately return FALSE, with ERROR_NOT_IMPLEMENTED in RESULT2.
	Other errors that may abort processing, have different error codes that
	should be specific for the type of option used. 
	For example, if a (W)rite operation supplied an illegal value, 
	a value of ERROR_BAD_NUMBER should be returned. 

	If the packet is returned with RESULT1 == FALSE, and the RESULT2 code
	is set to ERROR_ACTION_NOT_KNOWN, dos.library will attempt to 
	extrapolate only the nominated items of information mentioned above.

   SEE ALSO
	IDOS->FileSystemAttr()



**/ 


/****************************************************************************/ 
/* EOF */ 



