/****************************************************************************/ 
/*  _FSSerialize.c
**       
**  Created by FileSystem VectorPort tool. (FSVPTool 53.23) 
**       
**  :ts=4 
**       
*/       
/****************************************************************************/ 


#include "all_includes.h" 


/****************************************************************************/ 



int32 FSSerialize(struct FSVP *vp, int32 *res2)
{
	struct GlobalData *gd = vp->FSV.FSPrivate;

	IEXEC->ObtainSemaphore(gd->Sem);

	IDOS->DateStamp(&gd->root_node->date);
	IEXEC->CopyMem(&gd->root_node->date, &gd->volume_node->vn_VolumeDate, sizeof(struct DateStamp));

	IEXEC->ReleaseSemaphore(gd->Sem);
	(*res2) = 0;
	return(DOSTRUE); 
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_SERIALIZE_DISK - FSSerialize()

   SYNOPSIS
	int32 RESULT1 = FSSerialize(struct FileSystemVectorPort *fsvp,
	                            int32 *result2);

   FUNCTION
	This function will tell a file system to serialize the current volume.
	This is typically done by updating the creation date in the disk data
	and by also updating the current datestamp for the volume node.

	At the discretion of the filesystem programmer, other serializing
	features may be employed internally, but the minimum action required
	is setting the current datestamp in the volume node.

	DOS uses the volume node datestamp as a second tier check against
	volume name collisions in the doslist.


   INPUTS     (FileSystemVectorPort method)
	fsvp     - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2  - (int32 *) Pointer to the storage area for RESULT2.
	RESULT1  - (int32) Success/failure (DOSTRUE/FALSE), FALSE on error.
	RESULT2  - (int32) Failure code if RESULT1 == FALSE


   INPUTS     (DosPacket method)
	dp_Type  - (int32)  ACTION_SERIALIZE_DISK
	dp_Arg1  - <unused> 0
	RESULT1  - (int32) Success/Failure (DOSTRUE/FALSE), FALSE on error.
	RESULT2  - (int32) Failure code if RESULT1 == FALSE


   SEE ALSO
	IDOS->Serialize()



**/ 


/****************************************************************************/ 
/* EOF */ 



