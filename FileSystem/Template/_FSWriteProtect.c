/****************************************************************************/ 
/*  _FSWriteProtect.c
**       
**  Created by FileSystem VectorPort tool. (FSVPTool 53.23) 
**       
**  :ts=4 
**       
*/       
/****************************************************************************/ 


#include "all_includes.h" 


/****************************************************************************/ 



int32 FSWriteProtect(struct FSVP *vp, int32 *res2, int32 protect, uint32 passkey)
{
	struct GlobalData *gd = vp->FSV.FSPrivate;
	int32  trigger_change = FALSE;
	int32          result = FALSE;


	IEXEC->ObtainSemaphore(gd->Sem);

	do
	{
		if( protect )
		{
			/* 
			** If it's already protected, fail.
			*/ 
			if( gd->write_protected ) 
			{
				(*res2) = ERROR_BAD_NUMBER;
				break;
			}

			gd->write_protected   = TRUE;
			gd->write_protect_key = passkey;
			trigger_change        = TRUE;

			result  = DOSTRUE;
			(*res2) = 0;
			break;
		}


		/* 
		** We are being asked to unprotect when 
		** it's already unprotected. 
		*/ 
		if( NOT gd->write_protected )
		{
			(*res2) = ERROR_BAD_NUMBER;
			break;
		}


		/* 
		** If the passkey is not the same, fail.
		*/ 
		if( passkey != gd->write_protect_key ) 
		{
			(*res2) = ERROR_BAD_NUMBER;
			break;
		}

		gd->write_protected = FALSE;
		trigger_change      = TRUE;

		result = DOSTRUE;
		(*res2) = 0;

	}
	while(0);

	IEXEC->ReleaseSemaphore(gd->Sem);

	if( trigger_change )
	{
		IDOS->NonBlockingModifyDosEntry(0, NBM_DISKINSERTED, 0,0);
	}

	return(result);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_WRITE_PROTECT - FSWriteProtect()

   SYNOPSIS
	int32 RESULT1 = FSWriteProtect(struct FileSystemVectorPort *fsvp,
	                               int32 *result2, 
	                               int32  wp_state, 
	                               uint32 passkey);

   FUNCTION
	This action allows an application to change the write protect flag of
	a disk (if possible - applications cannot write to floppies that have
	their write-protect tabs set).  This action is primarily intended to
	allow write-protecting non-removable media such as hard disks.
	The value in wp_state toggles the write status.

	The 32-bit passkey allows a program to prevent other programs from
	unwrite-protecting a disk.  To unlock a disk, 'passkey' must match the
	passkey of the action that locked the disk, unless the disk was locked
	with a passkey value of 0.  
	In this case, no passkey is necessary to unlock the disk.

	On failure with a passkey mismatch, return RESULT1=FALSE along with
	RESULT2 = ERROR_BAD_NUMBER.


   INPUTS     (FileSystemVectorPort method)
	fsvp     - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2  - (int32 *) Pointer to the storage area for RESULT2.
	wp_state - (int32)   Inhibit state, (DOSTRUE/FALSE).
	passkey  - (uint32)  Passkey value, (use zero for no passkey).
	RESULT1  - (int32)   Success/failure (DOSTRUE/FALSE), FALSE on error.
	RESULT2  - (int32)   Failure code if RESULT1 == FALSE


   INPUTS     (DosPacket method)
	dp_Type  - (int32) ACTION_WRITE_PROTECT
	dp_Arg1  - (int32) DOSTRUE/FALSE (write protect/un-write protect).
	dp_Arg2  - (int32) 32 Bit pass key. 
	RESULT1  - (int32) Success/failure (DOSTRUE/FALSE), FALSE on error.
	RESULT2  - (int32) Failure code if RESULT1 == FALSE


   SEE ALSO
	IDOS->WriteProtectVolume()



**/ 


/****************************************************************************/ 
/* EOF */ 



