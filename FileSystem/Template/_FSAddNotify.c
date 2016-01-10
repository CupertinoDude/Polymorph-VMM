/****************************************************************************/ 
/*  _FSAddNotify.c
**       
**  Created by FileSystem VectorPort tool. (FSVPTool 53.23) 
**       
**  :ts=4 
**       
*/       
/****************************************************************************/ 


#include "all_includes.h" 


/****************************************************************************/ 


/* 
** ==================================================================
**  This functions is optional, dos.library will emulate the full 
**  functionality of this when the vector-port function responds  
**  with FALSE, and with the secondary code of ERROR_ACTION_NOT_KNOWN.
**  This function only need be implemented by remote filesystems. 
** ==================================================================
*/ 


int32 FSAddNotify(struct FSVP *vp UNUSED, int32 *res2, struct NotifyRequest *nr UNUSED)
{
	(*res2) = ERROR_ACTION_NOT_KNOWN; /* makes dos emulate this */ 
	return(FALSE);
}





/****************************************************************************/ 
/** 

   NAME
	ACTION_ADD_NOTIFY - FSAddNotify()
	                    (OPTIONAL - Emulated by DOS V52.28+)

   SYNOPSIS
	int32 RESULT1 = FSAddNotify(struct FileSystemVectorPort *fsvp,
	                            int32 *result2, 
	                            struct NotifyRequest *nreq);

   FUNCTION
	Do not implement this action unless you are a remote filesystem or 
	have a good reason to do so, that cannot be handled by the dos.library
	internal notification handler.


	This action asks a file system to notify the calling program if a
	particular object is altered.  The handler will notify a program 
	either by sending a message, calling a Hook or by signaling a task.

	To use this action, an application needs to allocate and initialize a
	NotifyRequest structure, this is done with IDOS->AllocDosObject(),
	the handler gets the watched object's name from the nr_FullName field.

	The string in nr_FullName will be an absolute path, including the
	name of the root volume or device (no assigns).  
	The absolute path is necessary because the file or its parent
	directories do not have to exist when the notification is set up, this
	allows notification on files and directories that do not yet exist.
	Notification will not occur until the directory or file is created.

	An application that uses the StartNotify() DOS call does not fill in
	the NotifyRequest's nr_FullName field, but instead fills in the nr_Name
	field.  StartNotify() takes the name from the nr_Name field and uses
	GetDeviceProc() and DevNameFromLock() to expand any assigns and storing
	the result in nr_FullName.

	The notification type depends on which bit is set in the 
	NotifyRequest.nr_Flags field.  If the NRF_SEND_MESSAGE bit is set, 
	an application receives notification of changes to the object through
	a message (see NotifyMessage from dos/notify.h).  In this case, the
	nr_Port field must point to the message port that will receive the
	notifying message .  If the nr_Flags NRF_SEND_SIGNAL bit is set, the
	file system will signal a task instead of sending a message.  
	In this case, nr_Task points to the task and nr_SignalNum is the 
	signal number.  

	The third method is NRF_CALL_HOOK, invoke the Hook pointed to by
	NotifyRequest->nr_stuff.nr_CallHook.nr_Hook;
	Note that this feature may not be supported by all file systems
	so be prepared for failure.
	This notification method was intended to be used with directories
	and does not work with files when serviced by a filesystem, however,
	both are supported only if dos.library V52.28+ is used to handle the
	notification.
	                
	NB: Your hook structure MUST remain persistent for the life of the
	    notify request.

	The Hook function is invoked with the following parameters:

	result = func(hook,reserved,msg);
	  D0           A0     A2     A1

	int32 func(struct Hook *hook, APTR reserved, struct NotifyHookMsg *msg);

	The 'reserved' parameter is currently used for special features within
	the operating system when employing dos.library handled notifications,
	filesystems must set this to 0.

	Your hook function should return 0 if successful.

	The NotifyHookMsg looks like this:

	struct NotifyHookMsg
	{
	    int32  nhm_Size;
	    int32  nhm_Action;
	    STRPTR nhm_Name;
	};

	The individual fields have the following meanings:

	nhm_Size
	    The size of this data structure (in bytes);
	    this must be >= 12; more fields may be added in the future and by
	    looking at nhm_Size you can tell if the fields you are expecting
	    are actually present.

	nhm_Name
	    The name of the object.

	nhm_Action
	    This can be one of the following values:

	    NHM_ACTION_INITIAL
	        This is the initial notification produced, as requested by
	        setting the NRF_NOTIFY_INITIAL flag in NotifyRequest->nr_Flags.

	    NHM_ACTION_ADD
	        An entry by the name given in the nhm_Name field was added to
	        the directory being monitored.

	    NHM_ACTION_CHANGE
	        The directory entry, whose name is given in the nhm_Name field,
	        has changed. For example, its size or modification time may
	        have changed.

	    NHM_ACTION_DELETE
	        An entry by the name given in the nhm_Name field was removed
	        from the directory being monitoried.

	        Note that further actions may be supported in the future.
	        As a rule, if your hook cannot support or 'understand' the
	        action given, then it should ignore it.

	        Only three different actions represent changes to directory
	        entries. For example, creating a new file will produce a
	        message of type NHM_ACTION_ADD. Deleting a file will produce a
	        message of type NHM_ACTION_DELETE.
	        Changes to entries involving modification time, comments, size,
	        protection bits and owner info will trigger NHM_ACTION_CHANGE
	        messages.
	        You will need to find out for yourself what exactly has changed.

	Only one of these three bits should be set!

	When an application wants to limit the number of NotifyMessages an
	handler can send per NotifyRequest, the application sets the
	NRF_WAIT_REPLY bit in the nr_Flags field.  This bit tells the handler
	not to send new NotifyMessages to a NotifyRequest's message port if
	the application has not returned a previous NotifyMessage.  
	This pertains only to a specific NotifyRequest, if other NotifyRequests
	exist on the same file (or directory) the handler will still send
	NotifyMessages to the other NotifyRequest's message ports.  
	The NRF_WAIT_REPLY bit only applies to message notification.

	Handlers should only perform a notification when the actual contents
	of the file have changed or its associated meta-data.  
	This includes ACTION_WRITE, ACTION_SET_xxxx, ACTION_DELETE, 
	ACTION_RENAME_OBJECT, ACTION_FINDUPDATE (MODE_READWRITE - create),
	ACTION_FINDOUTPUT (MODE_NEWFILE).

	For file write (change) and create notifications, these are sent in the
	ACTION_END function. ( ie; Close() )

	It should also include a change notification for all other meta-data
	modification actions such as the ACTION_SET_COMMENT/PROTECT/DATE/OWNER
	function, but this was not generally supported by filesystems.
	The built-in dos.library notification handler does support this.


   INPUTS    (FileSystemVectorPort method)
	fsvp    - (struct FileSystemVectorPort *) Pointer to the vector port.
	result2 - (int32 *) Pointer to the storage area for RESULT2.
	nreq    - (struct NotifyRequest *) Pointer to the notify request.
	RESULT1 - (int32) Success/failure (DOSTRUE/FALSE), FALSE on error.
	RESULT2 - (int32) Failure code if RESULT1 == FALSE


   INPUTS  - (DosPacket method)
	dp_Type - (int32) ACTION_ADD_NOTIFY
	dp_Arg1 - (struct NotifyRequest *) NotifyRequest structure pointer.
	RESULT1 - (int32) Success/Failure (DOSTRUE/FALSE), FALSE on error.
	RESULT2 - (int32) Failure code if RESULT1 == FALSE.


   SEE ALSO
	IDOS->StartNotify()



**/ 


/****************************************************************************/ 
/* EOF */ 



