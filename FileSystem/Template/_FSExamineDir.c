/****************************************************************************/ 
/*  _FSExamineDir.c
**       
**  Created by FileSystem VectorPort tool. (FSVPTool 53.23) 
**       
**  :ts=4 
**       
*/       
/****************************************************************************/ 


#include "all_includes.h" 


/****************************************************************************/ 



/* This should be in _FSReadSoftLink.c */ 
extern int32 read_softlink_target(APTR dummy UNUSED,...); 



/****************************************************************************/ 


int32 FSExamineDir(struct FSVP *vp, int32 *res2, struct PRIVATE_ExamineDirContext *ctx)
{
	struct GlobalData   *gd = vp->FSV.FSPrivate;
	struct ObjNode *dirnode = NULL;
	struct ObjLock *lock;
	uint32 mask;
	struct ExamineData *exd;
	CONST_STRPTR linktargetname;
	struct ObjNode *entry, *next;
	TEXT   slbuf[4096];


#define safe_strlen(str)  ((str) ? (strlen(str)) : 0)


	IEXEC->ObtainSemaphoreShared(gd->Sem);

	do
	{
		if( NO ctx )
		{
			(*res2) = ERROR_REQUIRED_ARG_MISSING;
			break; 
		}

		lock = (struct ObjLock *) ctx->ReferenceLock;
		mask = ctx->DataRequestFlags;


		/* 
		**  We generally do not bother trying to reuse nodes anymore, 
		**  due to the high availability of RAM on modern machines,   
		**  so just dump any stale nodes that are still on the stale list.  
		*/ 
		while( ( exd = (APTR)IEXEC->RemHead((APTR)&ctx->StaleNodeList) ) )
		{
			IDOS->FreeDosObject(DOS_EXAMINEDATA, exd);
		}


		if( lock )  /* somewhat redundant test */ 
		{
			dirnode = lock->node; /* get the directory node from the lock. */ 
		}
		if( NO dirnode )
		{
			break;
		}


		/* 
		** We can't scan with anything but a dir.
		*/ 
		if( dirnode->type != FSO_TYPE_DIRECTORY)  
		{ 
			(*res2) = ERROR_OBJECT_WRONG_TYPE;
			break;
		} 



		for( entry = (APTR)IEXEC->GetHead((APTR)&dirnode->content_list); entry; entry = next )
		{
			next = (APTR)IEXEC->GetSucc((APTR)entry);

			linktargetname = NULL;

			/*  
			**  Don't get link names unless asked for them.  
			*/  
			if( mask & EXF_LINK ) 
			{ 
				if( (FSOF_LINK|FSO_TYPE_FILE) == entry->type ) 
				{ 
					linktargetname = entry->hard_link_target->name; 
				} 
				else if( (FSOF_LINK|FSO_TYPE_DIRECTORY) == entry->type ) 
				{ 
					linktargetname = entry->hard_link_target->name; 
				} 
				else if( (FSOF_LINK|FSO_TYPE_SOFTLINK) == entry->type ) 
				{ 
					if( read_softlink_target(gd, res2, entry, slbuf, sizeof(slbuf)) >= 0 ) 
					{ 
						linktargetname = slbuf; 
					} 
					else 
					{ 
						break;   /* result 2 set by read_softlink_target() */ 
					} 
				} 
			} 


			exd = IDOS->AllocDosObjectTags (DOS_EXAMINEDATA,
							ADO_ExamineData_LinkSize,    1+ safe_strlen(linktargetname),
							ADO_ExamineData_NameSize,    1+ safe_strlen(entry->name),
							ADO_ExamineData_CommentSize, 1+ safe_strlen(entry->comment),
							ADO_ExamineDir_Context,      ctx,
							TAG_END);

			if( NO exd )
			{
				(*res2) = ERROR_NO_FREE_STORE;
				break;
			}



			if( mask & EXF_TYPE )
			{
				exd->Type = entry->type;
			}

			if( mask & EXF_SIZE )
			{
				switch( entry->type )
				{
					case FSO_TYPE_FILE:  /* real files */ 
							exd->FileSize = entry->file_size;
							break;

					case (FSO_TYPE_FILE|FSOF_LINK):  /* for hardlink files, we show only the size in the link */ 
							exd->FileSize = entry->hard_link_target->file_size;
							break;
 
					default:  /* directories, hardlinked dirs and soft links are this size */ 
							exd->FileSize = -1LL;
							break;
				}
			}

			if( mask & EXF_DATE )
			{
				exd->Date.ds_Days   = entry->date.ds_Days;
				exd->Date.ds_Minute = entry->date.ds_Minute;
				exd->Date.ds_Tick   = entry->date.ds_Tick;
			}

			/* 
			**  Count the hardlink references for this object. 
			**  1 indicates no ADDITIONAL references to this object. 
			**  0 is reserved to indicate this field is unsupported. 
			*/ 
			if( mask & EXF_USECOUNT )
			{
				exd->UseCount = 1; 

				if( NOT (entry->type & FSOF_LINK ))  /* if not a link object */ 
				{
					struct ObjNode *n; 

					for(n = entry->next_hard_link; n; n = n->next_hard_link) 
					{
						exd->UseCount ++; 
					}
				}
			}

			if( mask & EXF_OBJECTID )
			{
				exd->ObjectID = (uint64)(uint32)entry;   /* use inode address */ 
			}

			if( mask & EXF_NAME )
			{
				if( entry->name )
					IUTILITY->Strlcpy(exd->Name, entry->name, exd->NameSize);
				else
					exd->Name[0] =0;
			}

			if( mask & EXF_COMMENT )
			{
				if( entry->comment ) 
					IUTILITY->Strlcpy(exd->Comment, entry->comment, exd->CommentSize);
				else
					exd->Comment[0] =0;
			}

			if( mask & EXF_LINK )
			{
				if( linktargetname ) 
					IUTILITY->Strlcpy(exd->Link, linktargetname, exd->LinkSize);
				else
					exd->Link[0] =0;
			}

			if( mask & EXF_PROTECTION )
			{
				exd->Protection = entry->protection;
			}

			if( mask & EXF_OWNER )
			{
				exd->OwnerUID = entry->owner;
				exd->OwnerGID = entry->group;
			}

			IEXEC->AddTail( (struct List *)&ctx->FreshNodeList, (struct Node *)&exd->EXDnode);
		}

		if( NO entry )
		{
			(*res2) = ERROR_NO_MORE_ENTRIES; /* actually this means success for this case. */ 
		}

	}
	while(0);

	IEXEC->ReleaseSemaphore(gd->Sem);

	return(0);   
	/*  
	** We either get the whole dir in one go, 
	** or we fail with a res2 code set. 
	** DOS only saves the res2 code when 
	** a primary FALSE returncode is set, 
	** this is what will be used when there is 
	** nothing left on the freshnodelist. 
	*/ 
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_EXAMINEDATA_DIR - FSExamineDir()

   SYNOPSIS
	int32 RESULT1 = FSExamineDir(struct FileSystemVectorPort *fsvp,
	                             int32 *result2, 
	                             struct PRIVATE_ExamineDirContext *ctx);

   FUNCTION
	This action initiates a list refill of struct ExamineData nodes that
	represent the entries within a directory specified by the parameters
	in the struct PRIVATE_ExamineDirContex which is initialised by the
	IDOS->ObtainDirContext() function.

	The returned data structure looks like this;

	struct ExamineData
	{
	  struct MinNode   EXDnode;      // List node.                           
	  uint32           EXDinfo;      // General purpose user data.           
	                                 //                                      
	  // These are private members exclusively for the FS & DOS.             
	  uint32           FSPrivate;    // FILESYSTEM ONLY - Private usage.     
	  uint32           Reserved0;    // RESERVED - Future private usage.     
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
	  STRPTR           Link;         // (RO) Nul-terminated target name.     
	  uint32           LinkSize;     // (RO) Allocated target buffer size.   
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
	

	                                 ===== 


	The private context control structure that is passed to the filesystem
	is as follows:

	    struct PRIVATE_ExamineDirContext  (see; dos/dosextens.h)
	    {
	          uint32              StructSize;
	          struct MinList      FreshNodeList;
	          struct MinList      StaleNodeList;
	          uint32              DataRequestFlags;
	          struct Lock *       ReferenceLock;
	          struct MsgPort *    MessagePort;
	          APTR                MemoryPool;

	          BPTR                DOSPrivateCD;
	          BPTR                DOSPrivateLock;
	          STRPTR              DOSPrivateName;
	          int32               DOSPrivateResult2;
	          APTR                DOSPrivateFallback;
	          struct Hook *       DOSPrivateMatchFunc;
	          STRPTR              DOSPrivateMatchString;

	          APTR                FSPrivateMemPtr;
	          uint32              FSPrivate[8];
	    };    // 108 bytes - v51.104   
  
	    StructSize - (uint32) The size of this structure, it may expand in
	          future for new features.

	    FreshNodeList - (struct MinList) A list containing all the nodes
	          that were added by the filesystem from the last action of
	          type ACTION_EXAMINEDATA_DIR or a VP call to FSExamineDir().

	          Each time the caller invokes the IDOS->ExamineDir() function, 
	          DOS pulls the head node from this list and appends it to the
	          StaleNodeList, then returns a pointer to that node.

	          If there are no more nodes, another ACTION_EXAMINEDATA_DIR
	          dospacket request is sent or a FSVP call to FSExamineDir(),  
	          DOS then tries to remove the head node again.
	          If there are still no more, ExamineDir() returns NULL and the
	          scan terminates.

	          If this examine context is reused by using the tag; 
	          EX_ResetContext, DOS will move all nodes still left on this
	          list to the "StaleNodeList" and reset the scan, before the
	          next dospacket/fsvp call to the filesystem.

	    StaleNodeList - (struct MinList) A list containing the ExamineData
	          nodes which have already been returned to the caller by DOS,
	          from invoking the ExamineDir() function.

	    |     On each call to this function, (and if the filesystem does
	    |     not wish to attempt node reuse), the filesystem should simply
	    |     call IExec->RemHead() for all nodes on this list and release
	    |     them back to the memory pool by calling;
	    |        while((n=IExec->RemHead(&ctx->StaleNodeList)))
	    |        {
	    |             IDOS->FreeDosObject(DOS_EXAMINEDATA,n);
	    |        }
	    |
	    |     The filesystem should then obtain a new node by calling;
	    |        data = AllocDosObjectTags(DOS_EXAMINEDATA,
	    |                        ADO_ExamineData_NameSize, ###,
	    |                        ADO_ExamineData_CommentSize, ###,
	    |                        ADO_ExamineData_LinkSize, ###,
	    |                        ADO_ExamineDir_Context, context, TAG_END);
	    |
	    |
	    |     The filesystem shall then fill out the requested data fields
	    |     as specified by the 'DataRequestFlags' bitmask,
	    |     then it places the node on the tail of the 'FreshNodeList'.
	    |     This continues until there are no more directory entries.

	          If the resources run out (memory) before directory entries,
	          the filesystem can (at its discretion) do any of these;
	              1) Release all remaining nodes left on this 'StaleList'
	                 back to the pool using RemHead() + FreeDosObject(),
	                 then try to allocate a new one again.
	              2) Use the static FSPrivate[] array area to hold FS 
	                 private context data so it can resume when the caller
	                 uses up what has just been partially provided by 
	                 continuing to call ExamineDir() in the application.
	              3) Use the FSPrivateMemPtr buffer which the filesystem may
	                 have previously allocated to hold context data,
	                 or release it to free up "emergency memory" if it was
	                 used in that way.

	    |     Scan termination occurs ONLY when there are no more nodes
	    |     found on the 'FreshNodeList' after a filesystem call to refill.
	    |
	    |     If the last refill request resulted in completion of the scan,
	    |     the filesystem must return FALSE with ERROR_NO_MORE_ENTRIES
	    |     in Result2.
	    | 
	    |     The filesystems primary FALSE returncode is only used by DOS
	    |     to evaluate if the call is supported or not, and to tell
	    |     when to save the secondary code that will eventually be
	    |     returned to the caller when there are no more entries found on
	    |     the FreshNodeList, TRUTH values are ignored by ExamineDir().

	    DataRequestFlags - (uint32) The bitmask specified with the tag; 
	          EX_DataFields in the ObtainDirContext() call to specify what
	          data items are to be returned by the filesystem.

	          NB: Unspecified fields shall be left uninitialised, if they
	              are read they may contain data from a previous use.

	    ReferenceLock - (struct Lock *) The reference lock place holder.
	          Using EX_LockInput;
	              DOS does not modify the original lock, only convert
	              it to an APTR and places it here.

	          Using EX_StringNameInput;
	              The lock is obtained when DOS performs a Lock() on the
	              string argument supplied, it is placed here and also
	              in the DOSPrivateLock field which will be unlocked in
	              ReleaseDirContext().

	          Using EX_FileHandleInput;
	              The lock is obtained when DOS performs a ParentOfFH()
	              on the filehandle supplied, it is placed here and also
	              in the DOSPrivateLock field which will be unlocked in
	              ReleaseDirContext().

	    MessagePort - (struct MsgPort *) The filesystems packet/vector port.

	    MemoryPool  - (APTR) Exec Memory pool for this context.


	    DOSPrivateCD - (BPTR) DOS private lock for the original current
	          directory lock when EX_DoCurrentDir,TRUE tag was specified.
	          The old current dir will be restored in ReleaseDirContext()
	          or on subsequent failure of this function if context reused.
	          This value will be (BPTR)-1 if the tag EX_DoCurrentDir,TRUE
	          has not been specified.
	          ( Filesystems ignore this. )

	    DOSPrivateLock - (BPTR) DOS private lock.
	          DOS lock for EX_StringNameInput or EX_FileHandleInput, 
	          this lock will be unlocked in ReleaseDirContext().
	          ( Filesystems ignore this. )

	    DOSPrivateName - (STRPTR) DOS private name of context allocator.
	          ( Filesystems ignore this. ) ( Added 53.4 for resource track )

	    DOSPrivateResult2 - (int32) DOS private field. (internal Result2)
	          ( Filesystems ignore this. )

	    DOSPrivateFallback - (APTR) DOS private memory for fallback code.
	          This is used by DOS for the ExAllControl data only when the
	          action is not supported by the respective filesystem.
	          ( Filesystems ignore this. )

	    DOSPrivateMatchFunc - (struct Hook *) Pointer to a Hook structure
	          which has the h_Entry pointer set to a filter function. 
	          (Default = NULL), for no filtering to occur.
	          Value initialised by EX_MatchFunc tag. 
	          ( Filesystems ignore this. )

	    DOSPrivateMatchString - (STRPTR) If this field is NULL then all
	          filenames will be returned, the MatchFunc will not be invoked.
	          Value initialised by EX_MatchString tag. 
	          ( Filesystems ignore this. )

	    FSPrivate[8] - (uint32[8]) An -OPTIONAL- area for the filesystem to
	          hold any sort of context information between successive
	          action calls, such as may be required to restart a scan,
	          if interrupted by a memory shortage, for example.  
	          All fields of this array will be 0 before the first call to
	          ExamineDir() and all fields will also be set to 0 by DOS
	          when an old context is reused with the EX_ResetContext tag.
	          ( Filesystem use only. )

	    FSPrivateMemPtr - (APTR) An -OPTIONAL- pointer to an additional
	          memory area that is reserved for the filesystem to use, 
	          to hold any sort of data or to reserve "emergency memory".

	          DOS also uses this pointer for the fallback code read buffer, 
	          but ONLY if the filesystem does not support this action,
	          otherwise, if the action is supported by the filesystem, 
	          this field can be considered as "filesystem use only". 

	          The required memory must be obtained ONLY from the function
	          AllocVecPooled(), using the context memory pool pointer.
	          This field will be NULL before the first call to ExamineDir(),
	          and will be FreeVecPooled() and set to NULL by DOS, when an
	          old context is released or reused with EX_ResetContext tag.
	          ( Filesystem use only, when action is supported. )


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	ctx     - (struct PRIVATE_ExamineDirContext *) Pointer to the context.
	RESULT1 - (int32) DOSTRUE/FALSE (Success/Failure).
	RESULT2 - (int32) Failure code if RESULT1 == FALSE


   INPUTS    (DosPacket method)
	dp_Type - (int32) ACTION_EXAMINEDATA_DIR
	dp_Arg1 - (struct PRIVATE_ExamineDirContext *) ctx.
	RESULT1 - (int32) DOSTRUE/FALSE (Success/Failure).
	RESULT2 - (int32) Failure code if RESULT1 == FALSE


   NOTES
	The 'Link' target field for softlinks and hardlinks shall be initialised
	as follows:

	For softlinks, the filesystem will follow the proceedure as per the
	FSReadSoftLink() / ACTION_READ_SOFT_LINK function autodocs, 
	which means the only thing required is to copy the target string into
	the examinedata->Link buffer verbatim, nothing more.

	For hardlinks, the filesystem shall copy the target object name only
	into the examinedata->Link field buffer, nothing more.


   SEE ALSO
	IDOS->ExamineDir()



**/ 


/****************************************************************************/ 
/* EOF */ 



