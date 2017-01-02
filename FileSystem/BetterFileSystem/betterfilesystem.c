/*
**	BetterFileSystem - AmigaOS VectorPort FileSystem
*/
#include	"AmigaOS.h"
#include	"FileSystem.h"
#include	"FileSystemVectorPort.h"

#define	ID_BTRFS_DISK	0x42545200

unsigned int32 WorkbenchLauncher(struct ExecIFace *IExec, void *wbmsg)
{
	unsigned int32 rc = RETURN_OK;
	struct  *wbMessage = wbmsg;
	struct wbApplication *wbApplication=NULL;

	return(rc);
}

unsigned int32 BetterFileSystem( struct ExecBase *ExecBase)
{
	unsigned int32 rc=0L;
	struct ExecIFace *IExec = (struct ExecIFace *)ExecBase->MainInterface;
	struct DosPacket *StartPacket=NULL, *DoPacket=NULL;
	struct Process *ThisProc=NULL;
	struct Message *message;
	struct MsgPort *mport;
	struct fsApplication *fsApplication;
	unsigned long isActive=TRUE;
	TEXT	fsDeviceName[256];

	do{
		ThisProc = (struct Process *)IExec->FindTask(0);
		mport  = &ThisProc->pr_MsgPort;
		IExec->WaitPort(mport);
		message = IExec->GetMsg(mport);
		StartPacket = (struct DosPacket *)message->mn_Node.ln_Name;

		if( !StartPacket )
		{
			IExec->ReplyMsg(message)
			IExec->DebugPrintF("BetterFileSystem :: Workbench Launch applied\n",0);
			rc=WorkbenchLauncher(IExec,message);
			rc=RETURN_FAIL;
			break;
		}else{
			fsApplication = IExec->AllocVecTags(sizeof(*fsApplication),
				AVT_Type,MEMF_SHARED,
				AVT_Lock,TRUE,
				AVT_ClearWithValue,0,
				TAG_END);
		};

		if(!fsApplication)
		{
			StartPacket->dp_Res1 = FALSE; 
			StartPacket->dp_Res2 = ERROR_NO_FREE_STORE;
			IExec->ReplyMsg(message);
			rc=RETURN_FAIL;
			break;
		}else{
			fsApplication->SysBase=(APTR)ExecBase;
			fsApplication->IExec=IExec;

			if(fsApplication->SysBase->lib_Version > 51)
			{
				struct DebugIFace *IDBG=(APTR)IExec->GetInterface(ExecBase,"debug",1,0);
				if(IDBG)
				{
					if( IDBG->GetDebugLevel )
					{
						fsApplication->debug_level = IDBG->GetDebugLevel();
					}
					IExec->DropInterface((struct Interface *)IDBG);
				}
			}

			if(fsApplication->DOSBase=IExec->OpenLibrary("dos.library", 50))
			{
				IExec->DebugPrintF("[BetterFileSystem] Can't open V50 dos.library\n",0);
				break;
			}else{
				if(!(fsApplication->IDOS=(struct DOSIFace *)IExec->GetInterface(fsApplication->DOSBase, "main", 1, NULL)))
				if(fsApplication->IDOS)
					fsApplication->fsDeviceNode=BADDR(StartPacket->dp_Arg3);
					fsApplication->IDOS->CopyStringBSTRToC(
						fsApplication->fsDeviceNode->dn_Name,
						fsDeviceName,
						sizeof(fsDeviceName));
					break;
			};

			if(((fsApplication->DOSBase->lib_Version <<16) | fsApplication->DOSBase->lib_Revision) < (53<<16)|96))
			{
				CONST TEXT dosNeededVersionString[] = "[%s] You need dos.library 53.95+\nYou only have V%ld.%ld\n\n";
				IExec->DebugPrintF(NeededVersionString,
					fsDeviceName,
					fsApplication->DOSBase->lib_Version,
					fsApplication->DOSBase->lib_Revision);

					fsApplication->IDOS->TimedDosRequesterTags(
						TDR_Timeout, 30,
						TDR_NonBlocking,TRUE,
						TDR_TitleString,VERS,
						TDR_FormatString, dosNeededVersionString,
						TDR_GadgetString, "OK",
						TDR_ImageType, TDRIMAGE_ERROR,
						TDR_Arg1, fsDeviceName, 
						TDR_Arg2, (int32)fsApplication->DOSBase->lib_Version,
						TDR_Arg3, (int32)fsApplication->DOSBase->lib_Revision,
						TAG_END );

			break;
			}

			if(fsApplication->UtilityBase=IExec->OpenLibrary("utility.library", 50))
			{
				IExec->DebugPrintF("[BetterFileSystem] Can't open V50 utility.library\n",0);
				break;
			}else{
				if(!(fsApplication->IUtility=(struct UtilityIFace *)IExec->GetInterface(fsApplication->UtilityBase, "main", 1, NULL)))
					break;
			};
			
			if(fsApplication->DeviceIOBase=IExec->OpenLibrary("deviceio.library", 0))
			{
				IExec->DebugPrintF("[BetterFileSystem] Can't open DeviceIO.library\n",0);
				break;
			}else{
				if(!(fsApplication->IDeviceIO=(struct DeviceIOIFace *)IExec->GetInterface(fsApplication->DeviceIOBase, "main", 1, NULL)))
					break;
			};

			isActive=fsObtainResources(fsApplication, StartPacket);
		};

		while(isActive)
		{
			IExec->WaitPort(fsApplication->fsVectorPort->MP);
			while((message = IExec->GetMsg(&fsApplication->fsVectorPort->MP)))
			{
				if((DoPacket=(struct DosPacket *)message>man_Node.ln_Name))
				{
					if(DoPacket->dp_Link==message)
					{
						if(DoPacket->dp_Action == ACTION_SHUTDOWN)
						{
							fsReleaseResources(fsApplication,DoPacket);
							isActive=FALSE;
						}else{
							fsApplication->fsVectorPort->FSV.DOSEmulatePacket(fsApplication->fsVectorPort,DoPacket);
						};
					}
				}
				IExec->ReplyMsg(message);
			}	
		}
		
	}while(0);

	return(rc);

	fsReleaseResources(fsApplication);
}