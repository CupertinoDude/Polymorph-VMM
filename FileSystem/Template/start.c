/****************************************************************************/ 
/*  start.c
**       
**  Created by FileSystem VectorPort tool. (FSVPTool 53.23) 
**       
**  :ts=4 
**       
*/       
/****************************************************************************/ 


#include "all_includes.h" 


/****************************************************************************/ 


#include "vectors.h" 


/****************************************************************************/ 
/* FWD Ref */ 
static void handle_action_shutdown(struct GlobalData *gd, struct DosPacket *dp);

/****************************************************************************/ 
/* 
**  This is the handler process entry with a standard startup packet. 
**  The format is: 
** 
**      dp->dp_Action = ACTION_STARTUP;
**      dp->dp_Arg1   = MKBADDR(bstartup_name); // can be an extended BSTR 
**      dp->dp_Arg2   = devnode->dn_Startup;
**      dp->dp_Arg3   = MKBADDR(devnode);
**      dp->dp_Arg4   = 0;
**      dp->dp_Arg5   = dp->dp_Arg1;  //tells us this has nul-termed BSTR's 
**      dp->dp_Arg6   = 0;
**      dp->dp_Arg7   = 0;
** 
**  The only thing we use here is Arg3,  the BPTR to the device node.
*/ 
/*****************************************************************************/ 


static int32 filesystem( struct ExecBase *sysbase )
{
	struct ExecIFace *iexec = (struct ExecIFace *)sysbase->MainInterface;
	struct DosPacket *startup_packet =0;
	int32  shut_down = FALSE; 
	struct GlobalData *gd;
	struct Process *thisproc;
	struct DosPacket *dp;
	struct Message *msg;
	struct MsgPort *msgport;
	uint32 dosverrev;
//	int32  r2 =0;
	TEXT   devName[256];

	do
	{
		/* 
		** We ABSOLUTELY must fetch the handler startup dospacket FIRST.
		** It is illegal to perform any I/O before this, or even call
		** anything that may indirectly cause I/O via this process.
		*/ 
		thisproc = (struct Process *)iexec->FindTask(0);
		msgport  = &thisproc->pr_MsgPort;

		/* 
		** DOS started up this handler process, now we wait for it to send the 
		** startup packet, WaitPort() can never return until there is a message 
		** at the port, please avoid doing anything more convoluted here than 
		** simply calling WaitPort() and then calling GetMsg().
		*/ 
		iexec->WaitPort(msgport);
		msg = iexec->GetMsg(msgport);
		startup_packet = (struct DosPacket *) msg->mn_Node.ln_Name;


		/* 
		** Here we check if it is actually a dospacket we received, 
		** workbench startup messages have a NULL for ln_Name,
		** Dospackets have ln_Name pointing to the struct DosPacket, 
		** someone may have stuck an icon onto the binary file and 
		** tried to run it from Workbench.
		*/ 
		if( NO startup_packet )
		{
			iexec->ReplyMsg(msg);
			iexec->DebugPrintF("[FS] Ran from WB, Eeek!\n",0);
			return(RETURN_FAIL);
		}


	 	/* 
		** The global data must be shared clear unswappable memory.
		*/ 
		gd = iexec->AllocVecTags(sizeof(*gd), AVT_Type,MEMF_SHARED, 
		                                      AVT_Lock,TRUE,        
		                                      AVT_ClearWithValue,0, TAG_END);

		if( NO gd )
		{
			startup_packet->dp_Res1 = FALSE; 
			startup_packet->dp_Res2 = ERROR_NO_FREE_STORE;
			iexec->ReplyMsg(msg);
			iexec->DebugPrintF("[FS] No memory for GD\n",0);
			return(RETURN_FAIL);
		}


	 	/* 
		** Start setting up the globaldata lib bases and interfaces 
		** for the upper case; IEXEC,IDOS,IUTILITY macros to work. 
		*/ 
		gd->SysBase = (APTR)sysbase; 
		gd->IExec   = iexec; 


		/* 
		**  Open the minimum dos.library and v50 utility lib,
		**  an exact dos version check follows below.
		*/ 
		if( NOT (gd->DOSBase = IEXEC->OpenLibrary ("dos.library", 50) ))
		{
			IEXEC->DebugPrintF("[FS] Can't open V50 dos.library\n",0);
			break;
		}
		if( NOT (gd->UtilityBase = IEXEC->OpenLibrary ("utility.library", 50) ))
		{
			IEXEC->DebugPrintF("[FS] Can't open V50 utility.library\n",0);
			break;
		}

		if( NOT (IDOS = (struct DOSIFace *)IEXEC->GetInterface(gd->DOSBase, "main", 1, NULL)))
		{
			break;
		}
		if( NOT (IUTILITY =(struct UtilityIFace *)IEXEC->GetInterface(gd->UtilityBase,"main",1,NULL)))
		{
			break; 
		}


		/* 
		** Get the exec debug level value.
		*/ 
		if( gd->SysBase->lib_Version > 51)
		{
			struct DebugIFace *iDebug = (APTR) IEXEC->GetInterface(gd->SysBase, "debug", 1,0);

			if( iDebug )
			{
				if( iDebug->GetDebugLevel )
				{
					gd->debug_level = iDebug->GetDebugLevel();
				}
				IEXEC->DropInterface((struct Interface *)iDebug);
			}
		}


		/* 
		** Extract the device name from the DOS device node. 
		** We will need this below for the version check and AllocDosObject().
		** Also, store the device node pointer in the global data for later. 
		*/ 
		gd->device_node = BADDR(startup_packet->dp_Arg3);
		IDOS->CopyStringBSTRToC(gd->device_node->dn_Name, devName, sizeof(devName));


		/* 
		** Must check compatibility with the doslib revision, 
		** it will fail if no vector functions existed.
		** 
		** There was no vector port in AllocDosObject() before doslib 53.90 
		** Also softlinks didn't work properly before 53.95 
		*/ 
		dosverrev = (gd->DOSBase->lib_Version <<16) | gd->DOSBase->lib_Revision;

		if( dosverrev < ((53<<16)|95) )
		{
			CONST TEXT youneed[] = "[%s] You need dos.library 53.95+\nYou only have V%ld.%ld\n\n";

			IEXEC->DebugPrintF(youneed, devName, gd->DOSBase->lib_Version, gd->DOSBase->lib_Revision);

			IDOS->TimedDosRequesterTags(TDR_Timeout, 30,
			                            TDR_NonBlocking,TRUE,
			                            TDR_TitleString,VERS,
			                            TDR_FormatString, youneed,
			                            TDR_GadgetString, "OK",
			                            TDR_ImageType, TDRIMAGE_ERROR,
			                            TDR_Arg1, devName, 
			                            TDR_Arg2, (int32)gd->DOSBase->lib_Version,
			                            TDR_Arg3, (int32)gd->DOSBase->lib_Revision,
			                            TAG_END );

			break;
		}


		/* 
		** Create the vector port. 
		*/ 
		gd->FSVectorPort = IDOS->AllocDosObjectTags(DOS_FSVECTORPORT,
		                                            ADO_Name,    devName,
		                                            ADO_Vectors, &FSVectors,
		                                            TAG_END);
		if( NO gd->FSVectorPort)
		{
			IEXEC->DebugPrintF("[%s] Cannot allocate vector port.\n",devName);
			break;
		}


		/* 
		** Here is where we store the global data pointer
		** so it can be accessed from every vector-port function.
		*/ 
		gd->FSVectorPort->FSV.FSPrivate = gd;

		/* 
		** Here is where we allocate our global data access semaphore.
		*/ 
		gd->Sem = IEXEC->AllocSysObjectTags(ASOT_SEMAPHORE, ASOSEM_Public,FALSE, TAG_END);

		if( NO gd->Sem )
		{
			IEXEC->DebugPrintF("[%s] Cannot allocate semaphore.\n",devName);
			break;
		}


		/* 
		** Setup the memory pools.
		*/ 
	//	if( NOT create_memory_pool(gd))
		{
			IEXEC->DebugPrintF("[%s] Cannot allocate pool.\n",devName);
			break; 
		}


		/* 
		** Create the rootdir node and initialise it.
		*/ 
	//	gd->root_node = create_dir_node(gd, &r2, DISK_VOLUME_NAME);

		if( NO gd->root_node )
		{
			IEXEC->DebugPrintF("[%s] Cannot create root dir.\n",devName);
			break;
		}


		/* 
		** Create a volume node, initialise it, and add it to the doslist.
		*/ 
		gd->volume_node = (APTR) IDOS->MakeDosEntry((STRPTR) DISK_VOLUME_NAME, DLT_VOLUME);

		if( NO gd->volume_node )
		{
			IEXEC->DebugPrintF("[%s] Cannot allocate volume node.\n",devName);
			break;
		}


		gd->volume_node->vn_DiskType = ID_VP255_DOS_DISK;
		gd->volume_node->vn_Port     = &gd->FSVectorPort->MP;
		IDOS->DateStamp(&gd->volume_node->vn_VolumeDate);

		if( NOT IDOS->NonBlockingModifyDosEntry((APTR)gd->volume_node, NBM_ADDDOSENTRY, 0,0))
		{
			IEXEC->DebugPrintF("[%s] Cannot add volume node to doslist.\n",devName);
			IDOS->FreeDosEntry((struct DosList *)gd->volume_node);
			gd->volume_node = NULL;
			break;
		}


		/* 
		** We will be 'on the air' the instant we reply to the ACTION_STARTUP packet,
		** but before doing that we must initialise the device node port so we can be found by DOS.
		*/ 
		gd->device_node->dn_Port = &gd->FSVectorPort->MP;
		IDOS->ReplyPkt(startup_packet, DOSTRUE, 0);
		startup_packet = NULL;

		if( gd->debug_level > 5 )
		{
			IEXEC->DebugPrintF("[%s] Handler has started successfully.\n",devName);
		}

		/************************************************************************/ 

		while( NOT shut_down )
		{
			IEXEC->WaitPort(&gd->FSVectorPort->MP);

			while((msg = IEXEC->GetMsg(&gd->FSVectorPort->MP)))  /* Must loop until port empty */
			{
				if(( dp = (struct DosPacket *) msg->mn_Node.ln_Name ))
				{
					if( dp->dp_Link == msg )  /* to be sure we received a dospacket */ 
					{
						if( ACTION_SHUTDOWN == dp->dp_Action )
						{
							handle_action_shutdown(gd, dp); 
							shut_down = TRUE;  /* Don't break out, we must empty the msgport */ 
						}
						else 
						{ 
							/* Pass all other packets to the dos packet emulator vector. */ 
							gd->FSVectorPort->FSV.DOSEmulatePacket(gd->FSVectorPort,dp);
						}
					}
				}

				IEXEC->ReplyMsg(msg);
			}
		}

		/************************************************************************/ 
 	}
	while(0);
/*  CLEANUP: */ 

	if( gd->debug_level > 5 ) 
	{
		IEXEC->DebugPrintF("[FS] Handler cleanup in progress...\n",0);
	}


	/* 
	** We also need to do this here incase the shutdown function was not called.
	** This also implies that the startup packet was never replied to. 
	** 
	** Clearing the device node port will make the next access to this device
	** from DOS restart a new handler process.
	** 
	** Just for safety, we also invalidate the vector-port first to stop any
	** possibility of new calls occuring. 
	*/ 

	if( NOT shut_down )  /* the propper shutdown function was not called. */ 
	{
		if( gd->FSVectorPort )   /* must kill the VP. */ 
		{
			gd->FSVectorPort->FSV.Version = 0; 
		}

		if( gd->device_node )   /* must also kill the device node port. */ 
		{
			gd->device_node->dn_Port = NULL;
			gd->device_node = NULL; 
		}
	}


	/* 
	** Now we begin deallocating everything...
	*/ 
	if( gd->root_node )
	{
		//freevec(gd, (APTR)gd->root_node->name);
		//freevec(gd, gd->root_node);
	}

	if( gd->memory_pool )
	{
		IEXEC->FreeSysObject(ASOT_MEMPOOL, gd->memory_pool);
	}

	if( gd->Sem )
	{
		IEXEC->FreeSysObject(ASOT_SEMAPHORE, gd->Sem);
	}

	if( gd->IUtility )
	{
		IEXEC->DropInterface((struct Interface *)gd->IUtility);
	}

	if( gd->UtilityBase )
	{
		IEXEC->CloseLibrary(gd->UtilityBase);
	}

	if( gd->IDos )
	{
		IDOS->NonBlockingModifyDosEntry((APTR)gd->volume_node, NBM_REMFREEDOSENTRY, 0,0); /* NULL safe */ 
		IDOS->FreeDosObject(DOS_FSVECTORPORT, gd->FSVectorPort); /* NULL safe */ 

		IDOS->ReplyPkt(startup_packet, FALSE, IDOS->IoErr());  /* NULL safe */ 
		IEXEC->DropInterface((struct Interface *)gd->IDos); 
	}

	if( gd->DOSBase )
	{
		IEXEC->CloseLibrary(gd->DOSBase);
	}

	if( gd->debug_level > 5 )
	{
		IEXEC->DebugPrintF("[FS] Handler cleanup completed, process exiting...\n",0);
	}

	IEXEC->UnlockMem(gd,sizeof(*gd)); 
	IEXEC->FreeVec(gd);

	return(RETURN_OK);
}





/****************************************************************************/ 
/* Here we perform a dismount for an ACTION_SHUTDOWN packet. */ 

static void handle_action_shutdown(struct GlobalData *gd, struct DosPacket *dp)
{
	struct ObjLock *lock, *next;
	struct MsgPort *port;

	if( gd->debug_level > 5 )
	{
		IEXEC->DebugPrintF("[FS] ACTION_SHUTDOWN packet received.\n",0);
	}

	/* 
	** Trouble!, looks like someone tried to shut us down twice.
	*/ 
	if( NO gd->FSVectorPort->FSV.Version )
	{
		dp->dp_Res1 = FALSE;
		dp->dp_Res2 = ERROR_ACTION_NOT_KNOWN;
		return;
	}

	IEXEC->ObtainSemaphore(gd->Sem);  /* get exclusive access */ 

	/* 
	** Wait for any current callers to release before we invalidate 
	** the vector-port to stop any further dos.library callers. 
	*/ 
	gd->FSVectorPort->FSV.Version = 0; 

	/* 
	** If we have a volume node up, pull it out of service.
	** Send a diskchange event so that anyone looking will notice
	** that the volume has been changed, (or removed).
	** For this example, the FSDeviceInfoData() gets the vn_DiskType value, 
	** (as the 32 bit encoded DOSType) and places it directly into id_DiskType, 
	** so setting this to ID_NO_DISK_PRESENT will cause it to immediately appear 
	** in the next infodata structure, you may choose to do something more elaborate. 
	*/ 
	if( gd->volume_node )
	{
		gd->volume_node->vn_DiskType = ID_NO_DISK_PRESENT;
		IDOS->NonBlockingModifyDosEntry(0, NBM_DISKREMOVED, 0,0);
	}

	/* 
	** Clear the device nodes port, this will make the next access 
	** to this device from DOS, restart a new handler process.
	** Also clear the global data pointer to indicate we have done so. 
	*/ 
	if( gd->device_node )
	{
		gd->device_node->dn_Port = NULL;
		gd->device_node = NULL;
	}

	/* 
	** Dump all the public locks/files over to the DOS lock_handler.
	** It is nearly impossible for it to fail, as long as we
	** get the message port for the lock_handler and the 
	** objects were allocated by IDOS->AllocDosObject().
	** Remember, only pointers to objects that are public 
	** may be passed to the DOS lockhandler. 
	*/ 
	if(( port = IEXEC->FindPort("dos_lock_handler_port") ))
	{
		for(lock = BADDR(gd->lock_list);  lock;  lock = next)
		{
			next = BADDR(lock->lock.fl_Link);

			if( lock->fh ) /* This is for a public open file, pass the filehandle to the lockhandler. */ 
			{ 
				IDOS->DoPkt(port,ACTION_COLLECT,ID_COLLECT_FILEHANDLE,(int32)lock->fh, 0,0,0);

				IDOS->FreeDosObject(DOS_LOCK,lock);  /* free private filesystem lock memory now. */ 
			}
			else  /* This is for a public object lock, pass it to the lockhandler. */ 
			{ 
				IDOS->DoPkt(port,ACTION_COLLECT,ID_COLLECT_LOCK,(int32)lock, 0,0,0);
			}
		}

		/* 
		** Clear the locklist, all locks are now gone or owned by the DOS lock_handler, 
		** it will service those until they are unlocked or closed.
		*/ 
		gd->lock_list = ZERO;
	}

	IEXEC->ReleaseSemaphore(gd->Sem);

	if( gd->debug_level > 5 )
	{
		IEXEC->DebugPrintF("[FS] Dismounting completed.\n",0);
	}

	dp->dp_Res1 = DOSTRUE;
	dp->dp_Res2 = 0;

	return;
}




/****************************************************************************/ 
/* Print something if it was run from a cli, version info would be usefull. */ 

STATIC int32 cli_startup( struct ExecBase *sysbase ) 
{
	int32  result = RETURN_ERROR;
	struct ExecIFace *iexec  = (struct ExecIFace *)sysbase->MainInterface;
	struct Library *dosbase;
	struct DOSIFace *idos;


	if((dosbase = iexec->OpenLibrary("dos.library", 50)))
	{
		if((idos = (APTR) iexec->GetInterface(dosbase, "main", 1, NULL)))
		{
			idos->Printf("[Example Vectorport Filesystem Interface]\n",0);

			iexec->DropInterface((struct Interface *)idos);
		}
		iexec->CloseLibrary(dosbase);
	}

	return(result);
}


/****************************************************************************/ 
/* This is the primary DOS start() vector when started via wb/cli/handler.  */ 

int32  _start( STRPTR argstring UNUSED, int32 arglen UNUSED, struct ExecBase *sysbase)
{
	struct ExecIFace *iexec  = (struct ExecIFace *)sysbase->MainInterface;
	struct Process *thisproc = (struct Process *)iexec->FindTask(0);
	int32  result;

	iexec->Obtain(); 

	if( thisproc->pr_CLI )
	{
		result = cli_startup(sysbase); /* started via a shell commandline. */ 
	}
	else
	{
		result = filesystem(sysbase);  /* started via WB or as a FS handler. */ 
	}

	iexec->Release(); 

	return(result);
}







/****************************************************************************/ 
/** 

   NAME
	FILESYSTEM VECTOR-PORT() CALLING:

	NB. The minimum DOS version for vector-port support is 53.95.
	    Check your developer SDK:c for these tools; FSVPTool and FSTest.
	    FSVPTool will generate a skeleton vector-port filesystem for you.

	The new V53 struct FileSystemVectorPort information follows.
	See also; include/dos/filehandler.h for structure definitions. 

	The new filesystem vector-port calling method permits optional backward
	compatibility to the old style message port based dospacket interface,
	while allowing DOS to call the new filesystem vectors in the context of
	the dos.library function caller without the old dospacket overhead.

	The new filesystem vector-port calling method is to be implemented only
	by filesystems that, by convention, responds to ACTION_IS_FILESYSTEM
	with a TRUTH result.

	The implementation in a filesystems is relatively simple, a filesystem 
	historically allocates its own dospacket message port upon filesystem 
	process startup, the only requirement now is that the filesystem call 
	the IDOS->AllocDosObject(DOS_FSVECTORPORT,...) function to create the
	new style vector-port which is basically a traditional message port 
	with a function vector table appended.

	Supporting filesystems create an initialised 'struct FileSystemVector'
	table of function pointers which absolutely must include setting three
	additional fields in this structure, these are the 'Version' member to
	FS_VECTORPORT_VERSION and the 'StructSize' member shall be set to the
	sizeof(struct FileSystemVectors) and the 'End_Marker' set to -1.   

	A pointer to this structure is then passed to the IDOS->AllocDosObject()
	function using the ADO_Vectors tag, it will then copy the vector table
	to the allocated FileSystemVectorPort structure, the ADO_Name tag should
	also be specified to give the port a name, this shall be the same as the
	device node name as used for the devicenode->dn_Name.
	This can be fetched from the devicenode via the ACTION_STARTUP dospacket
	(via dp_Arg3) that is still sent when the filesystem process is started.
	The IDOS->AllocDosObject() function will copy this name and also perform
	other validations such as setting the mgsport struct mp_Node.ln_Type to
	NT_FILESYSTEM. The returned struct FileSystemVectorPort pointer is then
	deployed the same way as a normal dospacket message port was. 
	(See the ACTION_STARTUP autodoc for more info.)

	To avoid having to make large changes in the existing filesystems, the
	vector functions are as similar as practical to the parameters for the
	old dospackets, without any of the old BCPL pointer or BSTR conversion
	mayhem of course, the exceptions are where single dospackets were used
	in a somewhat convoluted manner in performing two or more functions
	based on some sort of flag/s, these have generally been separated into
	discrete functions.  An example of this is the vector-port functions
	for ACTION_CHANGE_MODE, this is now two dedicated functions;
	FSChangeLockMode() and FSChangeFileMode().

	It is also very important to avoid concurrency issues, because the
	vector-port functions will be invoked from the calling process context,
	all vector-port functions that modify global data (such as the
	filesystems internal locklist)  MUST now be surrounded by at least an
	exclusive access semaphore, mutex or such like, while functions that
	only read data would use a shared access semaphore or mutex.

	To simplify the implementation, NULL vector-port function pointers have
	special meaning where the dos.library will fall back to sending the
	equivalent dospacket to the filesystem message port to try and obtain
	a response.  This allows an existing dospacket orientated filesystem to
	implement the vector-port functions in a limited number for speed
	sensitive functions only, or implement functions incrementally for
	ease of debugging.

	To stop DOS from sending the fall back dospacket for truly unimplemented
	functions, simply provide a valid vector-port function stub that returns
	the values;  Result1=0, Result2=ERROR_ACTION_NOT_KNOWN.


	VECTOR PORT FUNCTION SEMANTICS:

	(1) The dos.library startup methodology for filesystems remains the same
	    as handlers, dos.library will start up a filesystem process and send
	    an ACTION_STARTUP dospacket to the process message port initially,
	    then wait for the reply, after this, supporting filesystems will
	    have their vector-port functions called from this point onward,
	    providing they are not NULL and the vector-port validates.
	    An ACTION_SHUTDOWN dospacket may also be sent by the dos.library to
	    shutdown the filesystem, this must be also handled by the filesystem
	    in a method appropriate for the particular filesystem.

	(2) The 'result2' (secondary result) pointers will always be specified
	    by dos.library, the filesystem may assume that these will never be
	    NULL pointers, also, dos.library will always clear result2 to 0
	    just before calling the vector-port function.

	(3) For Vector-port calls only, all STRPTR object descriptors shall no
	    longer contain any DOS path components whatsoever. The paths will be
	    pre-resolved by dos.library to a single object name and a relative
	    parent directory lock. Only the stored string returned from the 
	    FSReadSoftLink() function can contain DOS paths descriptors and the
	    filesystem must never do anything with this itself, except store it.

	(4) Exclusive Directory locks have been deprecated, regardless of the
	    requested mode.  Only locks for files may be exclusive, all requests
	    for exclusive directory locks shall be quietly changed to shared.
	    This shall be employed for both dospacket and vector-port functions.



	LEGACY APPLICATIONS THAT SEND DOSPACKETS DIRECTLY TO FILESYSTEMS:

	Legacy applications that still send dospackets directly to the message
	port of the filesystem would normally break when a pure vector-port
	filesystem is created, to handle these situations, all filesystems are
	provided with a special vector-port function that the dos.library
	initialises when any vector-port is created by AllocDosObject().
	The function vector prototype is;  
	    void DOSEmulatePacket(struct FSVP *vp, struct DosPacket *dp);

	A filesystem may simply pass all dospackets received from their port
	directly to this vector, ( except for the ACTION_SHUTDOWN dospacket
	which, like ACTION_STARTUP, is used for filesystem start and ending.)
	The dos.library will use the DosPacket information to perform an
	equivalent vector-port call through the supplied vector-port and
	dospacket arguments, then places the result back into the dospacket
	and return.  The filesystem should reply to the dospacket as it
	normally would in the dospacket handling loop, which should look
	quite similar to this;

	EXAMPLE:

	while( ! shutdown )
	{
	   IExec->WaitPort(&VectorPort.MP);
	   
	   while((msg = IExec->GetMsg(&VectorPort.MP)))
	   {
	      if(( dp = (struct DosPacket *) msg->mn_Node.ln_Name ))
	      {
	         if( dp->dp_Link == msg )  // validate for safety   
	         {
	            if( ACTION_SHUTDOWN == dp->dp_Action )
	            {
	               handle_action_shutdown(VectorPort,dp);
	               shutdown = TRUE; // don't break, must dequeue port   
	            }
	            else  // Pass all other packets to the dos emulator.   
	            { 
	               VectorPort->FSV.DOSEmulatePacket(VectorPort,dp);
	            }
	         }
	      }

	      IExec->ReplyMsg(msg);
	   }
	}


	The DOSEmulatePacket() vector will cause any dospackets sent by the
	dos.library to be responded to with 0 and ERROR_ACTION_NOT_KNOWN.
	Dospackets from all other senders will be converted to an equivalent
	vector-port function call, if the required functions are supported
	in the current dos.library version.

	If the vector-port has been invalidated, eg. from a shutdown, 
	the dospacket return codes; res1=0 and res2=ERROR_ACTION_NOT_KNOWN,
	will always be returned from the DOSEmulatePacket() vector.

	For implementors of the handle_action_shutdown() function,
	writers must invalidate the vector-port to stop dos.library
	callers vectoring, before clearing the devicenode->dn_Port ONCE ONLY.
	To do this, set the version number to zero;
	    VectorPort->FSV.Version = 0;

	A typical handle_action_shutdown() function should look something
	very much like this;

	EXAMPLE:

	static void handle_action_shutdown(struct FSVP * FSVectorPort,
	                                   struct DosPacket *dp)
	{
	    struct GlobalData *gd = FSVectorPort->FSV.FSPrivate;
	    struct ObjLock *lock, *next;
	    struct MsgPort *port;


	    //
	    // Trouble!, looks like someone tried to shut us down twice.
	      
	    if( 0 == FSVectorPort->FSV.Version )
	    {
	        dp->dp_Res1 = FALSE;
	        dp->dp_Res2 = ERROR_ACTION_NOT_KNOWN;
	        return;
	    }

	    IEXEC->ObtainSemaphore(gd->Sem); // gain exclusive access   

	    //
	    //  Invalidate the vector-port to stop any further dos.library 
	    //  callers vectoring from this point onward.
	      
	    FSVectorPort->FSV.Version = 0;

	    //
	    //  Set InfoData->id_DiskType = ID_NO_DISK_PRESENT; at this point.
	    //  If we have a volume node up, pull the disk out of service.
	    //  The volume node will be deleted later.
	      
	    if( gd->Volume_Node )
	    {
	        // Set InfoData->id_DiskType = ID_NO_DISK_PRESENT;  here   
	        IDOS->NonBlockingModifyDosEntry(gd->Volume_Node,NBM_REMDOSENTRY,0,0);
	    }

	    //
	    //  Clear the device nodes port, this will make the next access
	    //  to this device from DOS, restart a new handler process.
	    //  Clear the pointer from our global data to indicate it was 
	    //  done just the ONCE to avoid startup race conditions.
	      
	    if( gd->Device_Node )
	    {
	        gd->Device_Node->dn_Port = NULL;
	        gd->Device_Node = NULL;
	    }

	    //
	    // Pass all the public locks/files over to the DOS lock_handler.
	    // It is nearly impossible for it to fail, as long as we
	    // get the message port for the lock_handler and the 
	    // objects were allocated by IDOS->AllocDosObject().
	    // Remember, only pointers to objects that are public 
	    // may be passed to the DOS lockhandler. 
	      
	    if(( port = IEXEC->FindPort("dos_lock_handler_port") ))
	    {
	        for(lock = BADDR(gd->Lock_List); lock; lock = next)
	        {
	            next = BADDR(lock->lock.fl_Link);
	            
	            if( lock->fh )  // A public open file.   
	            {
	                IDOS->DoPkt(port, ACTION_COLLECT, ID_COLLECT_FILEHANDLE,
	                             (int32)lock->fh, 0,0,0);

	                            // free internal FS lock struct memory now   
	                IDOS->FreeDosObject(DOS_LOCK,lock);
	            }
	            else            // A public object lock   
	            {
	                IDOS->DoPkt(port, ACTION_COLLECT, ID_COLLECT_LOCK,
	                             (int32)lock, 0,0,0);
	            }
	        }
	        
	        //
	        //  Clear the locklist, all locks are now owned
	        //  by the DOS lock_handler, it will service them
	        //  until they are unlocked or closed.
	          
	        gd->Lock_List = ZERO;
	    }
	
	    dp->dp_Res1 = DOSTRUE;
	    dp->dp_Res2 = 0;
	
	    //
	    //  Send a diskchange event so that anyone looking will notice
	    //  that the volume has been removed.
	      
	    IDOS->NonBlockingModifyDosEntry(0, NBM_DISKREMOVED, 0,0);
	    IEXEC->ReleaseSemaphore(gd->Sem);
	    return;
	}


============================================================================




**/ 


/****************************************************************************/ 
/* EOF */ 



