/****************************************************************************/ 
/*  all_includes.h
**       
**  Created by FileSystem VectorPort tool. (FSVPTool 53.23) 
**       
**  :ts=4 
**       
*/       
/****************************************************************************/ 

#ifndef _ALLINCLUDES_H 
#define _ALLINCLUDES_H 

/*****************************************************************************/ 

#define __NOLIBBASE__
#define __NOGLOBALIFACE__
#define   NO_OBSOLETE_DOS_DEFINITIONS

/*****************************************************************************/ 

#include <proto/utility.h> 
#include <proto/exec.h> 
#include <proto/dos.h> 
#include <dos/startup.h> 

#include "FS_VECTORPORT_rev.h" 

/*****************************************************************************/ 

#include <string.h> 
#include <stddef.h> 

/*****************************************************************************/ 

#ifndef NOT   
#define NOT ! 
#endif        

#ifndef NO    
#define NO  ! 
#endif        

#ifndef SAME 
#define SAME 0 
#endif 

/*****************************************************************************/ 
/* These macros are used to access the interfaces from the global data. */ 

#define IEXEC     (gd->IExec)
#define IDOS      (gd->IDos)
#define IUTILITY  (gd->IUtility)

/*****************************************************************************/ 
/* These are defined in dos.h  - currently both are 255 characters long. */ 

#define MAX_FILENAME   MAX_VP_FILENAME 
#define MAX_COMMENT    MAX_VP_COMMENT 

/*****************************************************************************/ 

#define DISK_VOLUME_NAME  "FS_VECTORPORT"  

/*****************************************************************************/ 
/* Example object node structure */ 


struct ObjNode 
{
		/* Add your own type of node structure here */ 
	struct Node             node; 

		/* The node name would normally be pointed to by this. */ 
	CONST_STRPTR            name; 

		/* For a directory; this would be a list of files/dirs. */ 
		/* For a file this would be a block list of data. */ 
	struct MinList          content_list;	 

		/* This would be a pointer to the parent node. */ 
	struct ObjNode *        parent_dir;	

		/* If this node represents a hardlink, this points to the */ 
		/* target object node the hardlink refers to.  Otherwise 0. */ 
	struct ObjNode *        hard_link_target;  

		/* If this is non-zero, this object has hard links bound to it. */ 
		/* This points to the first or next one in a chain, NULL ends the list. */ 
	struct ObjNode *        next_hard_link;  

		/* The EXF_xxx flags to describe the object. */ 
	uint32                  type;

		/* File size must be in a signed 64 bit integer. */ 
	int64                   file_size;

	uint32                  protection;
	uint32                  owner;
	uint32                  group;
	struct DateStamp        date;
	STRPTR                  comment;
};


/*****************************************************************************/ 
/* Example of our extended lock structure, some members of the Lock struct   */ 
/* need to be initialised for maximum application compatibility,             */ 
/* please note the following requirements.... */ 

struct ObjLock 
{
		/* The fl_Link must still link locks together with the BCPL linkage method. */ 
		/* The fl_Key must point to something unique (ie; the object node address) */ 
		/* The fl_Access must be only the values SHARED_LOCK or EXCLUSIVE_LOCK. */ 
		/* The fl_Port must point to the vector-ports message port. */ 
		/* The fl_Volume must be BPTR to the DLT_VOLUME DosList node. */ 
		/* The fl_DOSType should be the 32 bit encoded dostype (via ADO_DOSType) */ 
	struct Lock        lock; 

		/* Corresponding object node. We don't want to rely on fl_Key. */ 
	struct ObjNode    *node;

		/* Current block in the nodes block_list */ 
	int64              current_block;

		/* 64 bit byte offset within the block */ 
	int64              current_block_offset;

		/* 64 bit byte position within file */ 
	int64              current_position;

		/* This is so we can find the filehandle via a lock during shutdown. */ 
	struct FileHandle *fh;

	int32              updated; 
		/* This is to be set TRUE when an object if modified and 
		** requires a change update be done in FSClose()/FSUnLock(), 
		** this refreshes the last changed datestamp and    
		** clears the [A]rchive bit of the object and path. 
		*/ 
};


/*****************************************************************************/ 
/* Example of a typical global data structure */ 

struct GlobalData 
{
	struct Library *             SysBase;
	struct Library *             DOSBase;
	struct Library *             UtilityBase;

	struct ExecIFace *           IExec;
	struct DOSIFace *            IDos;
	struct UtilityIFace *        IUtility;

	int32                        debug_level;

	struct SignalSemaphore      *Sem;

	BPTR                         lock_list;

	struct ObjNode *             root_node;

	struct VolumeNode *          volume_node;
	struct DeviceNode *          device_node;

	struct FileSystemVectorPort *FSVectorPort;

	uint32                       max_num_blocks;

	APTR                         memory_pool;

	int32                        write_protected;
	uint32                       write_protect_key;
};


#endif /* _ALLINCLUDES_H */ 


/*****************************************************************************/ 
/* Fwd ref's */ 

void Change_Update( struct GlobalData *gd, struct ObjNode *obj ); 
int32 read_softlink_target(APTR dummy UNUSED,...); 



/****************************************************************************/ 
/* EOF */ 



