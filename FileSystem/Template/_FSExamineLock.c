/****************************************************************************/ 
/*  _FSExamineLock.c
**       
**  Created by FileSystem VectorPort tool. (FSVPTool 53.23) 
**       
**  :ts=4 
**       
*/       
/****************************************************************************/ 


#include "all_includes.h" 


/****************************************************************************/ 



struct ExamineData * FSExamineLock( struct FSVP *vp, int32 *res2, struct Lock *objLock )
{
	struct GlobalData       *gd = vp->FSV.FSPrivate;
	struct ExamineData     *exd = NULL;
	struct ObjLock        *lock = (struct ObjLock *) objLock;
	struct ObjNode *n, *objnode = NULL;

#define safe_strlen(str)  ((str) ? (strlen(str)) : 0)

	IEXEC->ObtainSemaphoreShared(gd->Sem);

	do
	{
		if( NO lock )
		{
			(*res2) = ERROR_REQUIRED_ARG_MISSING;
			break;
		}

		if( NO (objnode = lock->node ))
		{
			break;
		}


		/* 
		**  This is usually not required here, because all locks are already resolved to 
		**  the target object.  If so, this may be removed.
		**  However, if the node can be for a hardlink, show info for the target instead. 
		*/ 
		if( (FSOF_LINK|FSO_TYPE_FILE) == objnode->type )
		{
			objnode = objnode->hard_link_target;
		}
		else if( (FSOF_LINK|FSO_TYPE_DIRECTORY) == objnode->type )
		{
			objnode = objnode->hard_link_target;
		}


		if( NO objnode ) 
		{
			break;
		}


		exd = IDOS->AllocDosObjectTags (DOS_EXAMINEDATA,
							ADO_ExamineData_NameSize,    1+ safe_strlen(objnode->name),
							ADO_ExamineData_CommentSize, 1+ safe_strlen(objnode->comment),
							TAG_END);
		if( NO exd )
		{
			(*res2) = ERROR_NO_FREE_STORE;
			break;
		}


		IUTILITY->Strlcpy(exd->Name, objnode->name, exd->NameSize);

		if( objnode->comment ) /* May be NULL for no comment */ 
		{
			IUTILITY->Strlcpy(exd->Comment, objnode->comment, exd->CommentSize);
		}
		else 
		{
			exd->Comment[0]=0; 
		}


		exd->Date.ds_Days   = objnode->date.ds_Days;
		exd->Date.ds_Minute = objnode->date.ds_Minute;
		exd->Date.ds_Tick   = objnode->date.ds_Tick;

		exd->Type           = objnode->type;
		exd->FileSize       = objnode->file_size;       /* file size if a file, or -1 */ 
		exd->ObjectID       = (uint64)(uint32)objnode;  /* We just use inode address as object ID here. */ 
		exd->Protection     = objnode->protection;
		exd->OwnerUID       = objnode->owner;
		exd->OwnerGID       = objnode->group;


		/* 
		**  Count the hardlink references for this object. 
		**  1 indicates no ADDITIONAL references to this object. 
		**  0 is reserved to indicate this field is unsupported. 
		*/ 
		exd->UseCount = 1; 
		for(n = objnode->next_hard_link; n; n = n->next_hard_link) 
		{
			exd->UseCount ++; 
		}


	}
	while(0);

	IEXEC->ReleaseSemaphore(gd->Sem);

	return(exd);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_EXAMINEDATA - FSExamineLock()

   SYNOPSIS
	struct ExamineData *RESULT1 = FSExamineLock(
	                                struct FileSystemVectorPort *fsvp,
	                                int32 *result2, 
	                                struct Lock *lock);

   FUNCTION
	Examines a single filesystem object identified by the lock.
	Returns current data in the form of a struct ExamineData.
	The ExamineData block will be allocated by the filesystem by calling
	the AllocDosObject() function using the appropriate buffer sizes (###)
	as follows;  data = IDOS->AllocDosObjectTags(DOS_EXAMINEDATA,
	                          ADO_ExamineData_NameSize,    ###,
	                          ADO_ExamineData_CommentSize, ###, TAG_END);

	The filesystem will fill in the data structure members with current
	information for the object.  This call may also cause the filesystem
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
	  STRPTR           Link;         // (RO) -- Unused by FSExamineLock().   
	  uint32           LinkSize;     // (RO) -- Unused by FSExamineLock().   
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
	lock    - (struct Lock *) Pointer to a valid object lock.
	RESULT1 - (struct ExamineData *) data or NULL on Failure.
	RESULT2 - (int32) Failure code if RESULT1 == NULL


   INPUTS    (DosPacket method)
	dp_Type - (int32) ACTION_EXAMINEDATA
	dp_Arg1 - (struct Lock *) lock.   
	RESULT1 - (struct ExamineData *) data or NULL on failure.
	RESULT2 - (int32) Failure code if RESULT1 == NULL


   NOTES
	This function shall return failure if 'lock' is NULL. 

   SEE ALSO
	IDOS->ExamineObject()



**/ 


/****************************************************************************/ 
/* EOF */ 



