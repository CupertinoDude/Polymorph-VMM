/****************************************************************************/ 
/*  _FSRelabel.c
**       
**  Created by FileSystem VectorPort tool. (FSVPTool 53.23) 
**       
**  :ts=4 
**       
*/       
/****************************************************************************/ 


#include "all_includes.h" 


/****************************************************************************/ 



int32 FSRelabel(struct FSVP *vp, int32 *res2, CONST_STRPTR vol_name)
{
	struct GlobalData *gd = vp->FSV.FSPrivate;
	STRPTR          vname = (STRPTR)vol_name;
	STRPTR  new_root_name = NULL;
	int32          result = FALSE;
	uint32           len;


	IEXEC->ObtainSemaphore(gd->Sem);

	do
	{
		if( gd->write_protected )
		{
			(*res2) = ERROR_DISK_WRITE_PROTECTED;
			break;
		}

		len = strlen(vname);

		if( len > MAX_FILENAME )
		{
			(*res2) = ERROR_INVALID_COMPONENT_NAME;
			break;
		}


		/* 
		**  Don't do anything if nothing needs to be done. 
		*/ 
		if( len == strlen(gd->root_node->name) ) 
		{ 
			if( SAME == memcmp(vname,gd->root_node->name,len)) 
			{
				break;
			}
		}


		/* 
		**  Allocation size - add one for nul-terminator.
		**  Do the allocation before we are commited to the change. 
		*/ 
		len ++; 

	//	new_root_name = getvec(gd, res2, len);

		if( NO new_root_name )
		{
			break;
		}


		/* 
		**  Change the doslist volume node name first,
		**  this will test for a name collision. 
		** 
		**  If this succeeds, change the rootnode name.
		*/ 
		if( IDOS->NonBlockingModifyDosEntry((APTR)gd->volume_node, NBM_RENAMEDOSENTRY, vname, 0))
		{
			IUTILITY->Strlcpy(new_root_name, vname, len);

			/* 
			**  Free the old name memory and add the new one.
			**  Needs to update the datestamp too.
			*/ 
		//	freevec(gd, gd->root_node->name);

			gd->root_node->name = new_root_name;
			IDOS->DateStamp(&gd->root_node->date);

			/* 
			** Prevent the new name being freed on success. 
			*/ 
			new_root_name = NULL;

			/* 
			**  For a volume renaming, send a disk inserted event so that anyone 
			**  waiting for an IDCMP_DISKINSERTED message sees one for the new name.
			*/ 
			IDOS->NonBlockingModifyDosEntry(0, NBM_DISKINSERTED, 0,0);

			result = DOSTRUE;
			(*res2) = 0;
		}
	}
	while(0);

	if( new_root_name )
	{
		//freevec(gd, new_root_name);
	}

	IEXEC->ReleaseSemaphore(gd->Sem);

	return(result);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_RENAME_DISK - FSRelabel()

   SYNOPSIS
	int32 RESULT1 = FSRelabel(struct FileSystemVectorPort *fsvp,
	                          int32 *result2
	                          CONST_STRPTR newname);

   FUNCTION
	This action allows an application to change the name of the current
	volume.  A file system implementing this function must also change
	the name stored in the volume node of the DOS device list and the
	name of the root directory.
	Be prepared for failure from RenameDosEntry() if a name conflicts.

	After the relabel succeeds, issue a DISKINSERTED event for anyone
	waiting for IDCMP_DISKINSERTED, this can be done by simply calling;
	IDOS->NonBlockingModifyDosEntry(0, NBM_DISKINSERTED, 0, 0);

   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	newname - (CONST_STRPTR) Pointer to the new volume name string.
	RESULT1 - (int32) Success/failure (DOSTRUE/FALSE), FALSE on error.
	RESULT2 - (int32) Failure code if RESULT1 == FALSE


   INPUTS    (DosPacket method)
	dp_Type - (int32)  ACTION_RENAME_DISK
	dp_Arg1 - (BSTR)   newname    (without colon)
	dp_Arg2 - <unused> 0
	dp_Arg3 - <unused> 0
	dp_Arg4 - <unused> 0
	dp_Arg5 - (BSTR)   nametype - name format indicator value.
	                   if(dp_Arg1==dp_Arg5) then BSTR's are guaranteed
	                   to be nul-terminated. (53.23)
	RESULT1 - (int32)  Success/Failure (DOSTRUE/FALSE), FALSE on error.
	RESULT2 - (int32)  Failure code if RESULT1 == FALSE


   SEE ALSO
	IDOS->Relabel(), IDOS->NonBlockingModifyDosEntry().



**/ 


/****************************************************************************/ 
/* EOF */ 



