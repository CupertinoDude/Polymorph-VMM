/****************************************************************************/ 
/*  _FSFlush.c
**       
**  Created by FileSystem VectorPort tool. (FSVPTool 53.23) 
**       
**  :ts=4 
**       
*/       
/****************************************************************************/ 


#include "all_includes.h" 


/****************************************************************************/ 



static int32 do_flush(APTR dummy UNUSED,...) 
{
	return 0; /* Write me ! */ 
}



/****************************************************************************/ 


int32 FSFlush(struct FSVP *vp, int32 *res2)
{
	struct GlobalData *gd = vp->FSV.FSPrivate;
	int32 result;

	/* 
	**  Request exclusive access for FSFlush(). 
	**  This makes us wait for all callers to 
	**  finish their current access before we flush. 
	*/ 
	IEXEC->ObtainSemaphore(gd->Sem);

	result = do_flush(gd, res2);

	IEXEC->ReleaseSemaphore(gd->Sem);

	return(result);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_FLUSH - FSFlush()

   SYNOPSIS
	int32 RESULT1 = FSFlush(struct FileSystemVectorPort *fsvp, 
	                        int32 *result2);

   FUNCTION
	This action causes the file system to flush out all buffers to disk.
	If any writes are pending, this must be completed before responding
	to this request.  

	This action allows an application to make sure that the data that is
	supposed to be on the disk is actually written to the disk instead
	of waiting in a buffer.


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	RESULT1 - (int32) DOSTRUE.
	RESULT2 - (int32) 0


   INPUTS    (DosPacket method)
	dp_Type - (int32)  ACTION_FLUSH
	dp_Arg1 - <unused> 0.
	RESULT1 - (int32) DOSTRUE.
	RESULT2 - (int32) 0


   SEE ALSO
	IDOS->FlushVolume(), IDOS->FlushVolumePort(), InhibitPort()



**/ 


/****************************************************************************/ 
/* EOF */ 



