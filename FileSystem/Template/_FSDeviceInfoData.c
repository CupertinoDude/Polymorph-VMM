/****************************************************************************/ 
/*  _FSDeviceInfoData.c
**       
**  Created by FileSystem VectorPort tool. (FSVPTool 53.23) 
**       
**  :ts=4 
**       
*/       
/****************************************************************************/ 


#include "all_includes.h" 


/****************************************************************************/ 



static int32 num_locks(struct GlobalData *gd) 
{ 
	struct ObjLock *p; 
	int32  result = 0; 

	for(p = BADDR(gd->lock_list); p; p = BADDR(p->lock.fl_Link) ) 
	{ 
		result ++; 
	} 
	return(result); 
} 



int32 FSDeviceInfoData(struct FSVP *vp, int32 *res2, struct InfoData *id)
{
	struct GlobalData *gd = vp->FSV.FSPrivate;
	uint32 num_blocks_used =0;


	IEXEC->ObtainSemaphoreShared(gd->Sem);

	id->id_NumSoftErrors = 0;
	id->id_UnitNumber    = 0;
	id->id_DiskState     = gd->write_protected ? ID_DISKSTATE_WRITE_PROTECTED : ID_DISKSTATE_VALIDATED;

	id->id_BytesPerBlock = 512;
	id->id_DiskType      = gd->volume_node->vn_DiskType;
	id->id_VolumeNode    = MKBADDR(gd->volume_node);
	id->id_InUse         = num_locks(gd);

	/* 
	** Round up the amount of data stored. If there is nothing stored,
	** make sure that we don't return 0 since some software is completely
	** unprepared to handle it. 
	*/ 

	//num_blocks_used = (num_bytes_used + (id->id_BytesPerBlock-1)) / id->id_BytesPerBlock;

	if( NO num_blocks_used )
	{
		num_blocks_used ++; 
	}

	id->id_NumBlocks     = gd->max_num_blocks; 
	id->id_NumBlocksUsed = num_blocks_used; 

	(*res2)=0;
	IEXEC->ReleaseSemaphore(gd->Sem);
	return(DOSTRUE);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_DISK_INFO - FSDeviceInfoData()

   SYNOPSIS
	int32 RESULT1 = FSDeviceInfoData(struct FileSystemVectorPort *fsvp,
	                                 int32 *result2, 
	                                 struct InfoData *infodat);

   FUNCTION
	The ACTION_DISK_INFO is used to get information about the device and
	status of the handler. It fills in an InfoData structure about the
	device the file system currently controls.  This packet has a special
	meaning for console style handlers.  When presented with this packet,
	a console style handler will return an InfoData structure where some
	fields have special meaning and are documented here.

	The InfoData structure will always be longword aligned for dospacket
	calls due to the BPTR conversions required.  For a vector-port calls,
	this is not mandated, so default alignment is satisfactory.

	Unlike FSVolumeInfo()/ACTION_INFO, this function does not depend on
	the state of a volume or depend on whether removable media is inserted,
	the only thing required is that the handler process is running.
 
	The contents of the InfoData structure are arranged as follows:

	        id_NumSoftErrors    ( -- Deprecated field )
	            Typically was for the number of media read/write errors
	            encountered on this volume. This number increased for every
	            error, regardless of which disk may be currently inserted,
	            which means it's of little use for anything at all.
	            Few file systems ever provided this information, it is
	            generally always set to zero. This field will likely be
	            reused in future for some other purpose, until then,
	            please leave it set to zero.

	        id_UnitNumber       ( -- Deprecated field )
	            This can be the low level device driver unit number
	            responsible for this disk drive, it also may not be.
	            Few file systems ever provided this information, 
	            and the operating system itself no longer uses it
	            as of AmigaOS 2.04 and beyond.  This field is generally
	            set to zero and will most likely be reused in future for
	            some other purpose, until then, please leave it set to zero.

	        id_DiskState
	            This is one of the following:

	                ID_DISKSTATE_WRITE_PROTECTED
	                    Medium is readable, but cannot be
	                    written to.

	                ID_DISKSTATE_VALIDATED
	                    Medium is readable and writable.

	                ID_DISKSTATE_VALIDATING
	                    Medium may be readable and is currently being
	                    prepared to make it writable again.
	                    This is also the default "parking" state used when
	                    the filesystem is waiting for media to be inserted,
	                    or the file system is in an unknown state and unable
	                    return meaningfull information about the medium,
	                    Always refer to id_DiskType for device state info.

	        id_NumBlocks
	            The number of storage units ("blocks") which this
	            file system consists of.

	                NOTE: This value can be zero!

	        id_NumBlocksUsed
	            The number of storage units currently in use by
	            the file system.

	                NOTE: This value can be zero or identical to
	                      the id_NumBlocks value!

	        id_BytesPerBlock
	            The number of bytes that make up a single storage
                   unit ("block"). Note that this number may not match
	            the physical size of the storage block, as used by
	            the underlying storage device. The file system might
	            return the size of a single data block instead, which
	            can be shorter.

	        id_DiskType
	            This field indicates both the state of the file system
	            and what kind of file system is involved. The
	            following values indicate file system state:

	                ID_NO_DISK_PRESENT
	                    No medium is currently present.

	                ID_UNREADABLE_DISK
	                    A medium is present, but low level read
	                    errors have prevented the file system from
	                    figuring out what is stored on it. This
	                    medium may not be formatted or simply
	                    defective.

	                ID_NOT_REALLY_DOS
	                    A medium is present, its contents are
	                    readable, but they are of no use for this
	                    file system.

	                ID_BUSY
	                    The file system is currently "offline" and
	                    cannot access the medium. This may be because
	                    commands such as "Format" or "DiskCopy" are
	                    accessing the low level disk data and do not
	                    want the file system to interfere.

	            The following values indicate what type of file
	            system is involved:

	                ID_DOS_DISK
	                ID_FFS_DISK
	                ID_INTER_DOS_DISK
	                ID_INTER_FFS_DISK
	                ID_FASTDIR_DOS_DISK
	                ID_FASTDIR_FFS_DISK
	                ID_LONGNAME_DOS_DISK
	                ID_LONGNAME_FFS_DISK
	                ID_MSDOS_DISK
	                ID_VP255_DOS_DISK

	            Each of these values stands for a particular type
	            of file system and disk data format.

	            The con-handler may return one of the following
	            types, which indicate the state it currently
	            operates in:

	                ID_CON
	                    Input is line-buffered, i.e. you have to
	                    hit the [Return] key to transfer your input
	                    to the client reading data from this console
	                    stream.

	                ID_RAWCON
	                    Input is unbuffered, i.e. each key you hit
	                    that produces a character immediately transfers
	                    input to the client software. This is the
	                    so-called 'single character' mode which can
	                    be enabled via IDOS->SetMode(..,DOSTRUE).

	            Last, but not least, you might very rarely encounter
	            a so-called "Kickstart" disk which contains the
	            operating system for the Amiga 1000 and Amiga 3000
	            computers. It is identified by the following value:

	                ID_KICKSTART_DISK

	            Other disk type values may exist, which represent
	            the file system signature stored on the disk (often
	            in the first sector) or might just be random data
	            found on an unformatted storage medium. Be prepared
	            to handle this: always check the id_DiskState field
	            to verify if the file system considers itself
	            responsible for this disk. Do not make decisions
	            based solely on the file system type found in the
	            id_DiskType field! A file system should never report
	            an id_DiskState value of ID_DISKSTATE_VALIDATED for
	            a medium it cannot use.

	        id_VolumeNode
	            This field is supposed to be a BCPL pointer to the volume
	            node of the currently active filesystem volume.
	            Not all file systems will supply this, or exactly this
	            particular information.  Non filesystems may use this field
	            for other completely incompatible purposes, for example,
	            the con-handler may return a pointer to the console window.
	            This field should be treated with caution.
	            Note that it can also be ZERO!

	        id_InUse
	            This field is supposed to be set to FALSE/ZERO if there
	            are currently no locks or files bound to the handler.
	            Otherwise, the non-zero value returned from some old file
	            systems may infact be any arbitrary non-zero boolean value.
	            The now prefered V50+ use for filesystems is that they
	            should actually return an integer for the number of locks
	            currently in force. Stream handlers should indicate the
	            number of streams currently open.
	            The con-handler also uses this field for a completely
	            different and incompatible purpose, such as storing a
	            pointer to the console I/O request. As with id_VolumeNode
	            this field should be treated with caution.


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	infodat - (struct InfoData *) Pointer to the InfoData structure.
	RESULT1 - (int32) Success/Failure (DOSTRUE/FALSE), FALSE on error.
	RESULT2 - (int32) Failure code if RESULT1 == FALSE


   INPUTS    (DosPacket method)
	dp_Type - (int32) ACTION_DISK_INFO
	dp_Arg1 - (BPTR)  InfoData structure.
	RESULT1 - (int32) Success/Failure (DOSTRUE/FALSE), FALSE on error.
	RESULT2 - (int32) Failure code if RESULT1 == FALSE


   SEE ALSO
	IDOS->GetDiskInfo()



**/ 


/****************************************************************************/ 
/* EOF */ 



