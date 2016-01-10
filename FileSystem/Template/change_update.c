/****************************************************************************/ 
/*  change_update.c
**       
**  Created by FileSystem VectorPort tool. (FSVPTool 53.23) 
**       
**  :ts=4 
**       
*/       
/****************************************************************************/ 


#include "all_includes.h" 


/****************************************************************************/ 



void Change_Update( struct GlobalData *gd, struct ObjNode *obj ) 
{ 
	struct DateStamp DS; 
 
	if( obj ) 
	{ 
		int32 r2 = IDOS->IoErr(); //NB: DataStamp() will affect pr_Result2. 
		IDOS->DateStamp(&DS); 
		IDOS->SetIoErr(r2);  
 
		/* 
		** We update all the way to the root dir. 
		*/ 
		while( obj ) 
		{ 
			obj->protection &= (~EXDF_ARCHIVE);  //clear -A- bit  
 
			obj->date.ds_Tick   = DS.ds_Tick; 
			obj->date.ds_Minute = DS.ds_Minute;  //update object datestamp 
			obj->date.ds_Days   = DS.ds_Days; 
 
			obj = obj->parent_dir; 
		} 
	} 
} 



/****************************************************************************/ 
/* EOF */ 



