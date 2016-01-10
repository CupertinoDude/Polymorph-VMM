/****************************************************************************/ 
/*  _FSExamineFile.c
**       
**  Created by FileSystem VectorPort tool. (FSVPTool 53.23) 
**       
**  :ts=4 
**       
*/       
/****************************************************************************/ 


#include "all_includes.h" 


/****************************************************************************/ 



struct	ExamineData * FSExamineFile(struct FSVP *vp, int32 *res2, struct FileHandle *fh)
{
	struct ExamineData *result;

	if( fh )
	{
 		result = FSExamineLock(vp,res2,fh->fh_Arg2);
	}
	else
	{
		(*res2) = ERROR_REQUIRED_ARG_MISSING;
		result  = NULL;
	}

	return(result);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_EXAMINEDATA_FH - FSExamineFile()

   SYNOPSIS
	struct ExamineData *RESULT1 = FSExamineFile(
	                                struct FileSystemVectorPort *fsvp,
	                                int32 *result2, 
	                                struct FileHandle *file);

   FUNCTION
	Examines a single file identified by the filehandle reference.
	Returns data in the form of a struct ExamineData.
	The ExamineData block will be allocated by the filesystem by calling
	the AllocDosObject() function using the appropriate buffer sizes (###)
	as follows;  data = IDOS->AllocDosObjectTags(DOS_EXAMINEDATA,
	                          ADO_ExamineData_NameSize,    ###,
	                          ADO_ExamineData_CommentSize, ###, TAG_END);

	The filesystem will fill in the data structure members with current
	information for the file.  This call may also cause the filesystem
	to flush any pending write buffers first, to get accurate information.

	If successfull, the returned data is used by the caller as needed,
	the caller must eventually release the ExamineData block by using the
	FreeDosObject() function;  IDOS->FreeDosObject(DOS_EXAMINEDATA,data).


	The returned data structure looks like this;

	struct ExamineData
	{
	  struct MinNode   EXDnode;      // List node.                           
	  uint32           EXDinfo;      // General purpose user data.           
	                                 //                                      
	  // These are private members exclusively for the FS & DOS.             
	  uint32           FSPrivate;    // FILESYSTEM ONLY - Private usage.     
	  uint32           Reserved0;    // DOS RESERVED - Private usage.        
	  APTR             DOSPrivate;   // DOSLIB ONLY - Private usage.         
	                                 //                                      
	  // The following are public fields.                                    
	  uint32           StructSize;   // (RO) Full size of the structure.     
	  uint32           Type;         // The entry descriptor value.          
	  int64            FileSize;     // Size if a file or -1LL if not.       
	  struct DateStamp Date;         // Days, Mins, Ticks.                   
	  uint32           RefCount;     // Object hardlink count.    [Note 1]   
	  uint64           ObjectID;     // Unique object identifier. [Note 2]   
	  STRPTR           Name;         // (RO) Nul-terminated object name.     
	  uint32           NameSize;     // (RO) Allocated name buffer size.     
	  STRPTR           Comment;      // (RO) Nul-terminated comment.         
	  uint32           CommentSize;  // (RO) Allocated comment buffer size.  
	  STRPTR           Link;         // (RO) -- Unused by FSExamineFile().   
	  uint32           LinkSize;     // (RO) -- Unused by FSExamineFile().   
	  uint32           Protection;   // Protection status bitmask.           
	  uint32           OwnerUID;     // Owner info.  [Note 3]                
	  uint32           OwnerGID;     // Group info.  [Note 3]                
	  uint32           Reserved2;    // DOS reserved field.                  
	  uint32           Reserved3;    // DOS reserved field.                  
	  uint32           Reserved4;    // DOS reserved field.                  
	};
	(RO) = Read Only, these fields must not be modified by the application.
	See structure definition in include/dos/dos.h for detailed information.

	[Note 1] If possible, this field should be supported by filesystems that
	         support hardlinks, it is used to indicate the hardlink reference
	         count for objects.  A value of one indicates that the object has
	         no additional references other than itself, a value of two will
	         indicate that it has one additional hardlink reference,
	         a value of three means it has two hardlink references, etc...
	         All hardlinks themselves always show a value of 1 in this field
	         when supported, otherwise this field MUST be set to zero for
	         all objects, if the filesystem does not support it.
	         It will be undefined if the caller fails to specify the
	         EXF_REFCOUNT bit in the EX_DataFields mask.
	
	[Note 2] This field is the mandatory unique object identfier for all
	         objects on this volume, the value used here may be any arbitrary
	         value as long as it is unique from all other objects on this
	         volume, it may use the object node address, the disk header block
	         number or other such values. (Note: this field is 64 bits wide.)
	         This field will be undefined if the caller fails to specify the
	         EXF_OBJECTID bit in the EX_DataFields mask.
	
	[Note 3] Historically, these fields only used the low 16 bits.
	         For extensibility, the size of these are now 32 bit fields.
	         However, for interoperability, it is recommended that values
	         greater than 65535 be avoided where possible so reading only the
	         low 16 bits results in the same value for legacy applications.
	         These fields will be undefined if the caller fails to specify
	         the EXF_OWNER bit in the EX_DataFields mask.
	

   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	file    - (struct FileHandle *) Pointer to a valid file handle.
	RESULT1 - (struct ExamineData *) ExamineData or NULL on Failure.
	RESULT2 - (int32) Failure code if RESULT1 == NULL


   INPUTS    (DosPacket method)
	dp_Type - (int32) ACTION_EXAMINEDATA_FH
	dp_Arg1 - (APTR)  file->fh_Arg1
	dp_Arg2 - (struct FileHandle *) file
	RESULT1 - (struct ExamineData *)data, NULL on failure.
	RESULT2 - (int32) Failure code if RESULT1 == NULL


   NOTES
	The function shall return failure if 'file' is NULL. 

   SEE ALSO
	IDOS->ExamineObject()



**/ 


/****************************************************************************/ 
/* EOF */ 



