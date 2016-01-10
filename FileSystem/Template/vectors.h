/****************************************************************************/ 
/*  vectors.h
**       
**  Created by FileSystem VectorPort tool. (FSVPTool 53.23) 
**       
**  :ts=4 
**       
*/       
/****************************************************************************/ 


#include "all_includes.h" 


/****************************************************************************/ 



CONST struct FileSystemVectors FSVectors = 
{
	sizeof (struct FileSystemVectors), /* StructSize */ 
	FS_VECTORPORT_VERSION,             /* Version  */ 
	0,                /* FSPrivate - We use this for globaldata */ 
	{ 0, 0, 0 },      /* Reserved[3] */ 
	0,                /* DOSPrivate  */ 
	NULL,             /* DOS will set this to DOSEmulatePacket() */ 

	FSLock,
	FSUnLock,
	FSDupLock,
	FSCreateDir,
	FSParentDir,
	FSDupLockFromFH,
	FSOpenFromLock,
	FSParentOfFH,
	FSOpen,
	FSClose,
	FSDelete,
	FSRead,
	FSWrite,
	FSFlush,

	FSChangeFilePosition,
	FSChangeFileSize,

	FSGetFilePosition,
	FSGetFileSize,

	FSChangeLockMode,
	FSChangeFileMode,

	FSSetDate,
	FSSetProtection,
	FSSetComment,
	FSSetGroup,
	FSSetOwner,
	FSRename,

	FSCreateSoftLink,
	FSCreateHardLink,
	FSReadSoftLink,

	FSSameLock,
	FSSameFile,
	FSFileSystemAttr,
	FSVolumeInfoData,
	FSDeviceInfoData,

	NULL, /* reserved */ 
	NULL, /* reserved */ 

	FSExamineLock,
	FSExamineFile,
	FSExamineDir,
	FSInhibit,
	FSWriteProtect,
	FSFormat,
	FSSerialize,
	FSRelabel,

	NULL, /* reserved */ 

	FSAddNotify,
	FSRemoveNotify,

	FSLockRecord,
	FSUnLockRecord,

	-1	/* V53_End_Marker */ 
};


/****************************************************************************/ 
/* EOF */ 



