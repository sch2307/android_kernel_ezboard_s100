#include	"DT_queue.h"	/* For DynamicTracer-TestPoint */
/**********************************************************************
 *
 * Copyright(c) 2008 Imagination Technologies Ltd. All rights reserved.
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope it will be useful but, except 
 * as otherwise stated in writing, without any warranty; without even the 
 * implied warranty of merchantability or fitness for a particular purpose. 
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 * 
 * The full GNU General Public License is included in this distribution in
 * the file called "COPYING".
 *
 * Contact Information:
 * Imagination Technologies Ltd. <gpl-support@imgtec.com>
 * Home Park Estate, Kings Langley, Herts, WD4 8LZ, UK 
 *
 ******************************************************************************/

#include "services_headers.h"

#include "lists.h"


#define DC_NUM_COMMANDS_PER_TYPE		1

typedef struct _DEVICE_COMMAND_DATA_
{
	PFN_CMD_PROC			pfnCmdProc;
	PCOMMAND_COMPLETE_DATA	apsCmdCompleteData[DC_NUM_COMMANDS_PER_TYPE];
	IMG_UINT32				ui32CCBOffset;
} DEVICE_COMMAND_DATA;


#if defined(__linux__) && defined(__KERNEL__)

#include "proc.h"

void ProcSeqShowQueue(struct seq_file *sfile,void* el)
{
	PVRSRV_QUEUE_INFO *psQueue = (PVRSRV_QUEUE_INFO*)el;
	IMG_INT cmds = 0;
	IMG_SIZE_T ui32ReadOffset;
	IMG_SIZE_T ui32WriteOffset;
	PVRSRV_COMMAND *psCmd;

	__DtTestPoint(__DtFunc_ProcSeqShowQueue, __DtStep_0)
	if(el == PVR_PROC_SEQ_START_TOKEN)
	{
		__DtTestPoint(__DtFunc_ProcSeqShowQueue, __DtStep_1)
		seq_printf( sfile,
					"Command Queues\n"
					"Queue    CmdPtr      Pid Command Size DevInd  DSC  SSC  #Data ...\n");
		__DtTestPoint(__DtFunc_ProcSeqShowQueue, __DtStep_2)
		return;
	}

	ui32ReadOffset = psQueue->ui32ReadOffset;
	ui32WriteOffset = psQueue->ui32WriteOffset;

	while (ui32ReadOffset != ui32WriteOffset)
	{
		__DtTestPoint(__DtFunc_ProcSeqShowQueue, __DtStep_3)
		psCmd= (PVRSRV_COMMAND *)((IMG_UINTPTR_T)psQueue->pvLinQueueKM + ui32ReadOffset);

		seq_printf(sfile, "%x %x  %5u  %6u  %3u  %5u   %2u   %2u    %3u  \n",
							(IMG_UINTPTR_T)psQueue,
							(IMG_UINTPTR_T)psCmd,
					 		psCmd->ui32ProcessID,
							psCmd->CommandType,
							psCmd->ui32CmdSize,
							psCmd->ui32DevIndex,
							psCmd->ui32DstSyncCount,
							psCmd->ui32SrcSyncCount,
							psCmd->ui32DataSize);
		
		ui32ReadOffset += psCmd->ui32CmdSize;
		ui32ReadOffset &= psQueue->ui32QueueSize - 1;
		cmds++;
	}

	if (cmds == 0)
	{
		__DtTestPoint(__DtFunc_ProcSeqShowQueue, __DtStep_4)
		seq_printf(sfile, "%x <empty>\n", (IMG_UINTPTR_T)psQueue);
	}
	__DtTestPoint(__DtFunc_ProcSeqShowQueue, __DtStep_5)
}

void* ProcSeqOff2ElementQueue(struct seq_file * sfile, loff_t off)
{
	PVRSRV_QUEUE_INFO *psQueue = IMG_NULL;
	SYS_DATA *psSysData;

	__DtTestPoint(__DtFunc_ProcSeqOff2ElementQueue, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(sfile);

	if(!off)
	{
		__DtTestPoint(__DtFunc_ProcSeqOff2ElementQueue, __DtStep_1)
		return PVR_PROC_SEQ_START_TOKEN;
	}


	psSysData = SysAcquireDataNoCheck();
	if (psSysData != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_ProcSeqOff2ElementQueue, __DtStep_2)
		for (psQueue = psSysData->psQueueList; (((--off) > 0) && (psQueue != IMG_NULL)); psQueue = psQueue->psNextKM);
	}

	__DtTestPoint(__DtFunc_ProcSeqOff2ElementQueue, __DtStep_3)
	return psQueue;
}
#endif 

#define GET_SPACE_IN_CMDQ(psQueue)										\
	((((psQueue)->ui32ReadOffset - (psQueue)->ui32WriteOffset)				\
	+ ((psQueue)->ui32QueueSize - 1)) & ((psQueue)->ui32QueueSize - 1))

#define UPDATE_QUEUE_WOFF(psQueue, ui32Size)							\
	(psQueue)->ui32WriteOffset = ((psQueue)->ui32WriteOffset + (ui32Size))	\
	& ((psQueue)->ui32QueueSize - 1);

#define SYNCOPS_STALE(ui32OpsComplete, ui32OpsPending)					\
	((ui32OpsComplete) >= (ui32OpsPending))


static IMG_VOID QueueDumpCmdComplete(COMMAND_COMPLETE_DATA *psCmdCompleteData,
									 IMG_UINT32				i,
									 IMG_BOOL				bIsSrc)
{
	PVRSRV_SYNC_OBJECT	*psSyncObject;

	__DtTestPoint(__DtFunc_QueueDumpCmdComplete, __DtStep_0)
	psSyncObject = bIsSrc ? psCmdCompleteData->psSrcSync : psCmdCompleteData->psDstSync;

	if (psCmdCompleteData->bInUse)
	{
		__DtTestPoint(__DtFunc_QueueDumpCmdComplete, __DtStep_1)
		PVR_LOG(("\t%s %u: ROC DevVAddr:0x%X ROP:0x%x ROC:0x%x, WOC DevVAddr:0x%X WOP:0x%x WOC:0x%x",
				bIsSrc ? "SRC" : "DEST", i,
				psSyncObject[i].psKernelSyncInfoKM->sReadOpsCompleteDevVAddr.uiAddr,
				psSyncObject[i].psKernelSyncInfoKM->psSyncData->ui32ReadOpsPending,
				psSyncObject[i].psKernelSyncInfoKM->psSyncData->ui32ReadOpsComplete,
				psSyncObject[i].psKernelSyncInfoKM->sWriteOpsCompleteDevVAddr.uiAddr,
				psSyncObject[i].psKernelSyncInfoKM->psSyncData->ui32WriteOpsPending,
				psSyncObject[i].psKernelSyncInfoKM->psSyncData->ui32WriteOpsComplete))
	}
	else
	{
		__DtTestPoint(__DtFunc_QueueDumpCmdComplete, __DtStep_2)
		PVR_LOG(("\t%s %u: (Not in use)", bIsSrc ? "SRC" : "DEST", i))
	}
	__DtTestPoint(__DtFunc_QueueDumpCmdComplete, __DtStep_3)
}


static IMG_VOID QueueDumpDebugInfo_ForEachCb(PVRSRV_DEVICE_NODE *psDeviceNode)
{
	__DtTestPoint(__DtFunc_QueueDumpDebugInfo_ForEachCb, __DtStep_0)
	if (psDeviceNode->sDevId.eDeviceClass == PVRSRV_DEVICE_CLASS_DISPLAY)
	{
		IMG_UINT32				ui32CmdCounter, ui32SyncCounter;
		SYS_DATA				*psSysData;
		DEVICE_COMMAND_DATA		*psDeviceCommandData;
		PCOMMAND_COMPLETE_DATA	psCmdCompleteData;

		__DtTestPoint(__DtFunc_QueueDumpDebugInfo_ForEachCb, __DtStep_1)
		SysAcquireData(&psSysData);

		psDeviceCommandData = psSysData->apsDeviceCommandData[psDeviceNode->sDevId.ui32DeviceIndex];

		if (psDeviceCommandData != IMG_NULL)
		{
			__DtTestPoint(__DtFunc_QueueDumpDebugInfo_ForEachCb, __DtStep_2)
			for (ui32CmdCounter = 0; ui32CmdCounter < DC_NUM_COMMANDS_PER_TYPE; ui32CmdCounter++)
			{
				__DtTestPoint(__DtFunc_QueueDumpDebugInfo_ForEachCb, __DtStep_3)
				psCmdCompleteData = psDeviceCommandData[DC_FLIP_COMMAND].apsCmdCompleteData[ui32CmdCounter];

				PVR_LOG(("Flip Command Complete Data %u for display device %u:",
						ui32CmdCounter, psDeviceNode->sDevId.ui32DeviceIndex))

				for (ui32SyncCounter = 0;
					 ui32SyncCounter < psCmdCompleteData->ui32SrcSyncCount;
					 ui32SyncCounter++)
				{
					__DtTestPoint(__DtFunc_QueueDumpDebugInfo_ForEachCb, __DtStep_4)
					QueueDumpCmdComplete(psCmdCompleteData, ui32SyncCounter, IMG_TRUE);
				}

				for (ui32SyncCounter = 0;
					 ui32SyncCounter < psCmdCompleteData->ui32DstSyncCount;
					 ui32SyncCounter++)
				{
					__DtTestPoint(__DtFunc_QueueDumpDebugInfo_ForEachCb, __DtStep_5)
					QueueDumpCmdComplete(psCmdCompleteData, ui32SyncCounter, IMG_FALSE);
				}
			}
		}
		else
		{
			__DtTestPoint(__DtFunc_QueueDumpDebugInfo_ForEachCb, __DtStep_6)
			PVR_LOG(("There is no Command Complete Data for display device %u", psDeviceNode->sDevId.ui32DeviceIndex))
		}
	}
	__DtTestPoint(__DtFunc_QueueDumpDebugInfo_ForEachCb, __DtStep_7)
}


IMG_VOID QueueDumpDebugInfo(IMG_VOID)
{
	SYS_DATA	*psSysData;
	__DtTestPoint(__DtFunc_QueueDumpDebugInfo, __DtStep_0)
	SysAcquireData(&psSysData);
	List_PVRSRV_DEVICE_NODE_ForEach(psSysData->psDeviceNodeList, &QueueDumpDebugInfo_ForEachCb);
	__DtTestPoint(__DtFunc_QueueDumpDebugInfo, __DtStep_1)
}


static IMG_SIZE_T NearestPower2(IMG_SIZE_T ui32Value)
{
	IMG_SIZE_T ui32Temp, ui32Result = 1;

	__DtTestPoint(__DtFunc_NearestPower2, __DtStep_0)
	if(!ui32Value)
		return 0;

	ui32Temp = ui32Value - 1;
	while(ui32Temp)
	{
		__DtTestPoint(__DtFunc_NearestPower2, __DtStep_1)
		ui32Result <<= 1;
		ui32Temp >>= 1;
	}

	__DtTestPoint(__DtFunc_NearestPower2, __DtStep_2)
	return ui32Result;
}


IMG_EXPORT
PVRSRV_ERROR IMG_CALLCONV PVRSRVCreateCommandQueueKM(IMG_SIZE_T ui32QueueSize,
													 PVRSRV_QUEUE_INFO **ppsQueueInfo)
{
	PVRSRV_QUEUE_INFO	*psQueueInfo;
	IMG_SIZE_T			ui32Power2QueueSize = NearestPower2(ui32QueueSize);
	SYS_DATA			*psSysData;
	PVRSRV_ERROR		eError;
	IMG_HANDLE			hMemBlock;

	__DtTestPoint(__DtFunc_PVRSRVCreateCommandQueueKM, __DtStep_0)
	SysAcquireData(&psSysData);

	
	eError = OSAllocMem(PVRSRV_OS_NON_PAGEABLE_HEAP,
					 sizeof(PVRSRV_QUEUE_INFO),
					 (IMG_VOID **)&psQueueInfo, &hMemBlock,
					 "Queue Info");
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVCreateCommandQueueKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVCreateCommandQueueKM: Failed to alloc queue struct"));
		goto ErrorExit;
	}
	OSMemSet(psQueueInfo, 0, sizeof(PVRSRV_QUEUE_INFO));

	psQueueInfo->hMemBlock[0] = hMemBlock;
	psQueueInfo->ui32ProcessID = OSGetCurrentProcessIDKM();

	
	eError = OSAllocMem(PVRSRV_OS_NON_PAGEABLE_HEAP,
					 ui32Power2QueueSize + PVRSRV_MAX_CMD_SIZE,
					 &psQueueInfo->pvLinQueueKM, &hMemBlock,
					 "Command Queue");
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVCreateCommandQueueKM, __DtStep_2)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVCreateCommandQueueKM: Failed to alloc queue buffer"));
		goto ErrorExit;
	}

	psQueueInfo->hMemBlock[1] = hMemBlock;
	psQueueInfo->pvLinQueueUM = psQueueInfo->pvLinQueueKM;

	
	PVR_ASSERT(psQueueInfo->ui32ReadOffset == 0);
	PVR_ASSERT(psQueueInfo->ui32WriteOffset == 0);

	psQueueInfo->ui32QueueSize = ui32Power2QueueSize;

	
	if (psSysData->psQueueList == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_PVRSRVCreateCommandQueueKM, __DtStep_3)
		eError = OSCreateResource(&psSysData->sQProcessResource);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PVRSRVCreateCommandQueueKM, __DtStep_4)
			goto ErrorExit;
		}
	}

	
	eError = OSLockResource(&psSysData->sQProcessResource,
							KERNEL_ID);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVCreateCommandQueueKM, __DtStep_5)
		goto ErrorExit;
	}

	psQueueInfo->psNextKM = psSysData->psQueueList;
	psSysData->psQueueList = psQueueInfo;

	eError = OSUnlockResource(&psSysData->sQProcessResource, KERNEL_ID);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVCreateCommandQueueKM, __DtStep_6)
		goto ErrorExit;
	}

	*ppsQueueInfo = psQueueInfo;

	__DtTestPoint(__DtFunc_PVRSRVCreateCommandQueueKM, __DtStep_7)
	return PVRSRV_OK;

ErrorExit:

	if(psQueueInfo)
	{
		__DtTestPoint(__DtFunc_PVRSRVCreateCommandQueueKM, __DtStep_8)
		if(psQueueInfo->pvLinQueueKM)
		{
			__DtTestPoint(__DtFunc_PVRSRVCreateCommandQueueKM, __DtStep_9)
			OSFreeMem(PVRSRV_OS_NON_PAGEABLE_HEAP,
						psQueueInfo->ui32QueueSize,
						psQueueInfo->pvLinQueueKM,
						psQueueInfo->hMemBlock[1]);
			psQueueInfo->pvLinQueueKM = IMG_NULL;
		}

		OSFreeMem(PVRSRV_OS_NON_PAGEABLE_HEAP,
					sizeof(PVRSRV_QUEUE_INFO),
					psQueueInfo,
					psQueueInfo->hMemBlock[0]);
		
	}

	__DtTestPoint(__DtFunc_PVRSRVCreateCommandQueueKM, __DtStep_10)
	return eError;
}


IMG_EXPORT
PVRSRV_ERROR IMG_CALLCONV PVRSRVDestroyCommandQueueKM(PVRSRV_QUEUE_INFO *psQueueInfo)
{
	PVRSRV_QUEUE_INFO	*psQueue;
	SYS_DATA			*psSysData;
	PVRSRV_ERROR		eError;
	IMG_BOOL			bTimeout = IMG_TRUE;

	__DtTestPoint(__DtFunc_PVRSRVDestroyCommandQueueKM, __DtStep_0)
	SysAcquireData(&psSysData);

	psQueue = psSysData->psQueueList;

	 
	LOOP_UNTIL_TIMEOUT(MAX_HW_TIME_US)
	{
		if(psQueueInfo->ui32ReadOffset == psQueueInfo->ui32WriteOffset)
		{
			__DtTestPoint(__DtFunc_PVRSRVDestroyCommandQueueKM, __DtStep_1)
			bTimeout = IMG_FALSE;
			break;
		}
		OSSleepms(1);
	} END_LOOP_UNTIL_TIMEOUT();

	if (bTimeout)
	{
		
		__DtTestPoint(__DtFunc_PVRSRVDestroyCommandQueueKM, __DtStep_2)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVDestroyCommandQueueKM : Failed to empty queue"));
		eError = PVRSRV_ERROR_CANNOT_FLUSH_QUEUE;
		goto ErrorExit;
	}

	
	eError = OSLockResource(&psSysData->sQProcessResource,
								KERNEL_ID);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVDestroyCommandQueueKM, __DtStep_3)
		goto ErrorExit;
	}

	if(psQueue == psQueueInfo)
	{
		__DtTestPoint(__DtFunc_PVRSRVDestroyCommandQueueKM, __DtStep_4)
		psSysData->psQueueList = psQueueInfo->psNextKM;

		OSFreeMem(PVRSRV_OS_NON_PAGEABLE_HEAP,
					NearestPower2(psQueueInfo->ui32QueueSize) + PVRSRV_MAX_CMD_SIZE,
					psQueueInfo->pvLinQueueKM,
					psQueueInfo->hMemBlock[1]);
		psQueueInfo->pvLinQueueKM = IMG_NULL;
		OSFreeMem(PVRSRV_OS_NON_PAGEABLE_HEAP,
					sizeof(PVRSRV_QUEUE_INFO),
					psQueueInfo,
					psQueueInfo->hMemBlock[0]);
		 
		psQueueInfo = IMG_NULL; 
	}
	else
	{
		__DtTestPoint(__DtFunc_PVRSRVDestroyCommandQueueKM, __DtStep_5)
		while(psQueue)
		{
			__DtTestPoint(__DtFunc_PVRSRVDestroyCommandQueueKM, __DtStep_6)
			if(psQueue->psNextKM == psQueueInfo)
			{
				__DtTestPoint(__DtFunc_PVRSRVDestroyCommandQueueKM, __DtStep_7)
				psQueue->psNextKM = psQueueInfo->psNextKM;

				OSFreeMem(PVRSRV_OS_NON_PAGEABLE_HEAP,
							psQueueInfo->ui32QueueSize,
							psQueueInfo->pvLinQueueKM,
							psQueueInfo->hMemBlock[1]);
				psQueueInfo->pvLinQueueKM = IMG_NULL;
				OSFreeMem(PVRSRV_OS_NON_PAGEABLE_HEAP,
							sizeof(PVRSRV_QUEUE_INFO),
							psQueueInfo,
							psQueueInfo->hMemBlock[0]);
				 
				psQueueInfo = IMG_NULL; 
				break;
			}
			psQueue = psQueue->psNextKM;
		}

		if(!psQueue)
		{
			__DtTestPoint(__DtFunc_PVRSRVDestroyCommandQueueKM, __DtStep_8)
			eError = OSUnlockResource(&psSysData->sQProcessResource, KERNEL_ID);
			if (eError != PVRSRV_OK)
			{
				__DtTestPoint(__DtFunc_PVRSRVDestroyCommandQueueKM, __DtStep_9)
				goto ErrorExit;
			}
			eError = PVRSRV_ERROR_INVALID_PARAMS;
			goto ErrorExit;
		}
	}

	
	eError = OSUnlockResource(&psSysData->sQProcessResource, KERNEL_ID);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVDestroyCommandQueueKM, __DtStep_10)
		goto ErrorExit;
	}

	
	if (psSysData->psQueueList == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_PVRSRVDestroyCommandQueueKM, __DtStep_11)
		eError = OSDestroyResource(&psSysData->sQProcessResource);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PVRSRVDestroyCommandQueueKM, __DtStep_12)
			goto ErrorExit;
		}
	}

ErrorExit:

	__DtTestPoint(__DtFunc_PVRSRVDestroyCommandQueueKM, __DtStep_13)
	return eError;
}


IMG_EXPORT
PVRSRV_ERROR IMG_CALLCONV PVRSRVGetQueueSpaceKM(PVRSRV_QUEUE_INFO *psQueue,
												IMG_SIZE_T ui32ParamSize,
												IMG_VOID **ppvSpace)
{
	IMG_BOOL bTimeout = IMG_TRUE;

	
	__DtTestPoint(__DtFunc_PVRSRVGetQueueSpaceKM, __DtStep_0)
	ui32ParamSize =  (ui32ParamSize+3) & 0xFFFFFFFC;

	if (ui32ParamSize > PVRSRV_MAX_CMD_SIZE)
	{
		__DtTestPoint(__DtFunc_PVRSRVGetQueueSpaceKM, __DtStep_1)
		PVR_DPF((PVR_DBG_WARNING,"PVRSRVGetQueueSpace: max command size is %d bytes", PVRSRV_MAX_CMD_SIZE));
		__DtTestPoint(__DtFunc_PVRSRVGetQueueSpaceKM, __DtStep_2)
		return PVRSRV_ERROR_CMD_TOO_BIG;
	}

	 
	LOOP_UNTIL_TIMEOUT(MAX_HW_TIME_US)
	{
		if (GET_SPACE_IN_CMDQ(psQueue) > ui32ParamSize)
		{
			__DtTestPoint(__DtFunc_PVRSRVGetQueueSpaceKM, __DtStep_3)
			bTimeout = IMG_FALSE;
			break;
		}
		OSSleepms(1);
	} END_LOOP_UNTIL_TIMEOUT();

	if (bTimeout == IMG_TRUE)
	{
		__DtTestPoint(__DtFunc_PVRSRVGetQueueSpaceKM, __DtStep_4)
		*ppvSpace = IMG_NULL;

		__DtTestPoint(__DtFunc_PVRSRVGetQueueSpaceKM, __DtStep_5)
		return PVRSRV_ERROR_CANNOT_GET_QUEUE_SPACE;
	}
	else
	{
		__DtTestPoint(__DtFunc_PVRSRVGetQueueSpaceKM, __DtStep_6)
		*ppvSpace = (IMG_VOID *)((IMG_UINTPTR_T)psQueue->pvLinQueueUM + psQueue->ui32WriteOffset);
	}

	__DtTestPoint(__DtFunc_PVRSRVGetQueueSpaceKM, __DtStep_7)
	return PVRSRV_OK;
}


IMG_EXPORT
PVRSRV_ERROR IMG_CALLCONV PVRSRVInsertCommandKM(PVRSRV_QUEUE_INFO	*psQueue,
												PVRSRV_COMMAND		**ppsCommand,
												IMG_UINT32			ui32DevIndex,
												IMG_UINT16			CommandType,
												IMG_UINT32			ui32DstSyncCount,
												PVRSRV_KERNEL_SYNC_INFO	*apsDstSync[],
												IMG_UINT32			ui32SrcSyncCount,
												PVRSRV_KERNEL_SYNC_INFO	*apsSrcSync[],
												IMG_SIZE_T			ui32DataByteSize )
{
	PVRSRV_ERROR 	eError;
	PVRSRV_COMMAND	*psCommand;
	IMG_SIZE_T		ui32CommandSize;
	IMG_UINT32		i;

	
	__DtTestPoint(__DtFunc_PVRSRVInsertCommandKM, __DtStep_0)
	ui32DataByteSize = (ui32DataByteSize + 3UL) & ~3UL;

	
	ui32CommandSize = sizeof(PVRSRV_COMMAND)
					+ ((ui32DstSyncCount + ui32SrcSyncCount) * sizeof(PVRSRV_SYNC_OBJECT))
					+ ui32DataByteSize;

	
	eError = PVRSRVGetQueueSpaceKM (psQueue, ui32CommandSize, (IMG_VOID**)&psCommand);
	if(eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVInsertCommandKM, __DtStep_1)
		return eError;
	}

	psCommand->ui32ProcessID	= OSGetCurrentProcessIDKM();

	
	psCommand->ui32CmdSize		= ui32CommandSize; 
	psCommand->ui32DevIndex 	= ui32DevIndex;
	psCommand->CommandType 		= CommandType;
	psCommand->ui32DstSyncCount	= ui32DstSyncCount;
	psCommand->ui32SrcSyncCount	= ui32SrcSyncCount;
	
	
	psCommand->psDstSync		= (PVRSRV_SYNC_OBJECT*)(((IMG_UINTPTR_T)psCommand) + sizeof(PVRSRV_COMMAND));


	psCommand->psSrcSync		= (PVRSRV_SYNC_OBJECT*)(((IMG_UINTPTR_T)psCommand->psDstSync)
								+ (ui32DstSyncCount * sizeof(PVRSRV_SYNC_OBJECT)));

	psCommand->pvData			= (PVRSRV_SYNC_OBJECT*)(((IMG_UINTPTR_T)psCommand->psSrcSync)
								+ (ui32SrcSyncCount * sizeof(PVRSRV_SYNC_OBJECT)));
	psCommand->ui32DataSize		= ui32DataByteSize;

	
	for (i=0; i<ui32DstSyncCount; i++)
	{
		__DtTestPoint(__DtFunc_PVRSRVInsertCommandKM, __DtStep_2)
		psCommand->psDstSync[i].psKernelSyncInfoKM = apsDstSync[i];
		psCommand->psDstSync[i].ui32WriteOpsPending = PVRSRVGetWriteOpsPending(apsDstSync[i], IMG_FALSE);
		psCommand->psDstSync[i].ui32ReadOpsPending = PVRSRVGetReadOpsPending(apsDstSync[i], IMG_FALSE);

		PVR_DPF((PVR_DBG_MESSAGE, "PVRSRVInsertCommandKM: Dst %u RO-VA:0x%x WO-VA:0x%x ROP:0x%x WOP:0x%x",
				i, psCommand->psDstSync[i].psKernelSyncInfoKM->sReadOpsCompleteDevVAddr.uiAddr,
				psCommand->psDstSync[i].psKernelSyncInfoKM->sWriteOpsCompleteDevVAddr.uiAddr,
				psCommand->psDstSync[i].ui32ReadOpsPending,
				psCommand->psDstSync[i].ui32WriteOpsPending));
	}

	
	for (i=0; i<ui32SrcSyncCount; i++)
	{
		__DtTestPoint(__DtFunc_PVRSRVInsertCommandKM, __DtStep_3)
		psCommand->psSrcSync[i].psKernelSyncInfoKM = apsSrcSync[i];
		psCommand->psSrcSync[i].ui32WriteOpsPending = PVRSRVGetWriteOpsPending(apsSrcSync[i], IMG_TRUE);
		psCommand->psSrcSync[i].ui32ReadOpsPending = PVRSRVGetReadOpsPending(apsSrcSync[i], IMG_TRUE);

		PVR_DPF((PVR_DBG_MESSAGE, "PVRSRVInsertCommandKM: Src %u RO-VA:0x%x WO-VA:0x%x ROP:0x%x WOP:0x%x",
				i, psCommand->psSrcSync[i].psKernelSyncInfoKM->sReadOpsCompleteDevVAddr.uiAddr,
				psCommand->psSrcSync[i].psKernelSyncInfoKM->sWriteOpsCompleteDevVAddr.uiAddr,
				psCommand->psSrcSync[i].ui32ReadOpsPending,
				psCommand->psSrcSync[i].ui32WriteOpsPending));
	}

	
	*ppsCommand = psCommand;

	__DtTestPoint(__DtFunc_PVRSRVInsertCommandKM, __DtStep_4)
	return PVRSRV_OK;
}


IMG_EXPORT
PVRSRV_ERROR IMG_CALLCONV PVRSRVSubmitCommandKM(PVRSRV_QUEUE_INFO *psQueue,
												PVRSRV_COMMAND *psCommand)
{
	
	
	
	__DtTestPoint(__DtFunc_PVRSRVSubmitCommandKM, __DtStep_0)
	if (psCommand->ui32DstSyncCount > 0)
	{
		__DtTestPoint(__DtFunc_PVRSRVSubmitCommandKM, __DtStep_1)
		psCommand->psDstSync = (PVRSRV_SYNC_OBJECT*)(((IMG_UINTPTR_T)psQueue->pvLinQueueKM)
									+ psQueue->ui32WriteOffset + sizeof(PVRSRV_COMMAND));
	}

	if (psCommand->ui32SrcSyncCount > 0)
	{
		__DtTestPoint(__DtFunc_PVRSRVSubmitCommandKM, __DtStep_2)
		psCommand->psSrcSync = (PVRSRV_SYNC_OBJECT*)(((IMG_UINTPTR_T)psQueue->pvLinQueueKM)
									+ psQueue->ui32WriteOffset + sizeof(PVRSRV_COMMAND)
									+ (psCommand->ui32DstSyncCount * sizeof(PVRSRV_SYNC_OBJECT)));
	}

	psCommand->pvData = (PVRSRV_SYNC_OBJECT*)(((IMG_UINTPTR_T)psQueue->pvLinQueueKM)
									+ psQueue->ui32WriteOffset + sizeof(PVRSRV_COMMAND)
									+ (psCommand->ui32DstSyncCount * sizeof(PVRSRV_SYNC_OBJECT))
									+ (psCommand->ui32SrcSyncCount * sizeof(PVRSRV_SYNC_OBJECT)));

	
	UPDATE_QUEUE_WOFF(psQueue, psCommand->ui32CmdSize);

	__DtTestPoint(__DtFunc_PVRSRVSubmitCommandKM, __DtStep_3)
	return PVRSRV_OK;
}



static
PVRSRV_ERROR PVRSRVProcessCommand(SYS_DATA			*psSysData,
								  PVRSRV_COMMAND	*psCommand,
								  IMG_BOOL			bFlush)
{
	PVRSRV_SYNC_OBJECT		*psWalkerObj;
	PVRSRV_SYNC_OBJECT		*psEndObj;
	IMG_UINT32				i;
	COMMAND_COMPLETE_DATA	*psCmdCompleteData;
	PVRSRV_ERROR			eError = PVRSRV_OK;
	IMG_UINT32				ui32WriteOpsComplete;
	IMG_UINT32				ui32ReadOpsComplete;
	DEVICE_COMMAND_DATA		*psDeviceCommandData;
	IMG_UINT32				ui32CCBOffset;

	
	__DtTestPoint(__DtFunc_PVRSRVProcessCommand, __DtStep_0)
	psWalkerObj = psCommand->psDstSync;
	psEndObj = psWalkerObj + psCommand->ui32DstSyncCount;
	while (psWalkerObj < psEndObj)
	{
		PVRSRV_SYNC_DATA *psSyncData = psWalkerObj->psKernelSyncInfoKM->psSyncData;

		__DtTestPoint(__DtFunc_PVRSRVProcessCommand, __DtStep_1)
		ui32WriteOpsComplete = psSyncData->ui32WriteOpsComplete;
		ui32ReadOpsComplete = psSyncData->ui32ReadOpsComplete;
		
		if ((ui32WriteOpsComplete != psWalkerObj->ui32WriteOpsPending)
		||	(ui32ReadOpsComplete != psWalkerObj->ui32ReadOpsPending))
		{
			__DtTestPoint(__DtFunc_PVRSRVProcessCommand, __DtStep_2)
			if (!bFlush ||
				!SYNCOPS_STALE(ui32WriteOpsComplete, psWalkerObj->ui32WriteOpsPending) ||
				!SYNCOPS_STALE(ui32ReadOpsComplete, psWalkerObj->ui32ReadOpsPending))
			{
				__DtTestPoint(__DtFunc_PVRSRVProcessCommand, __DtStep_3)
				return PVRSRV_ERROR_FAILED_DEPENDENCIES;
			}
		}

		psWalkerObj++;
	}

	
	psWalkerObj = psCommand->psSrcSync;
	psEndObj = psWalkerObj + psCommand->ui32SrcSyncCount;
	while (psWalkerObj < psEndObj)
	{
		PVRSRV_SYNC_DATA *psSyncData = psWalkerObj->psKernelSyncInfoKM->psSyncData;

		__DtTestPoint(__DtFunc_PVRSRVProcessCommand, __DtStep_4)
		ui32ReadOpsComplete = psSyncData->ui32ReadOpsComplete;
		ui32WriteOpsComplete = psSyncData->ui32WriteOpsComplete;
		
		if ((ui32WriteOpsComplete != psWalkerObj->ui32WriteOpsPending)
		|| (ui32ReadOpsComplete != psWalkerObj->ui32ReadOpsPending))
		{
			__DtTestPoint(__DtFunc_PVRSRVProcessCommand, __DtStep_5)
			if (!bFlush &&
				SYNCOPS_STALE(ui32WriteOpsComplete, psWalkerObj->ui32WriteOpsPending) &&
				SYNCOPS_STALE(ui32ReadOpsComplete, psWalkerObj->ui32ReadOpsPending))
			{
				__DtTestPoint(__DtFunc_PVRSRVProcessCommand, __DtStep_6)
				PVR_DPF((PVR_DBG_WARNING,
						"PVRSRVProcessCommand: Stale syncops psSyncData:0x%x ui32WriteOpsComplete:0x%x ui32WriteOpsPending:0x%x",
						(IMG_UINTPTR_T)psSyncData, ui32WriteOpsComplete, psWalkerObj->ui32WriteOpsPending));
			}

			if (!bFlush ||
				!SYNCOPS_STALE(ui32WriteOpsComplete, psWalkerObj->ui32WriteOpsPending) ||
				!SYNCOPS_STALE(ui32ReadOpsComplete, psWalkerObj->ui32ReadOpsPending))
			{
				__DtTestPoint(__DtFunc_PVRSRVProcessCommand, __DtStep_7)
				return PVRSRV_ERROR_FAILED_DEPENDENCIES;
			}
		}
		psWalkerObj++;
	}

	
	if (psCommand->ui32DevIndex >= SYS_DEVICE_COUNT)
	{
		__DtTestPoint(__DtFunc_PVRSRVProcessCommand, __DtStep_8)
		PVR_DPF((PVR_DBG_ERROR,
					"PVRSRVProcessCommand: invalid DeviceType 0x%x",
					psCommand->ui32DevIndex));
		__DtTestPoint(__DtFunc_PVRSRVProcessCommand, __DtStep_9)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	
	psDeviceCommandData = psSysData->apsDeviceCommandData[psCommand->ui32DevIndex];
	ui32CCBOffset = psDeviceCommandData[psCommand->CommandType].ui32CCBOffset;
	psCmdCompleteData = psDeviceCommandData[psCommand->CommandType].apsCmdCompleteData[ui32CCBOffset];
	if (psCmdCompleteData->bInUse)
	{
		
		__DtTestPoint(__DtFunc_PVRSRVProcessCommand, __DtStep_10)
		return PVRSRV_ERROR_FAILED_DEPENDENCIES;
	}

	
	psCmdCompleteData->bInUse = IMG_TRUE;

	
	psCmdCompleteData->ui32DstSyncCount = psCommand->ui32DstSyncCount;
	for (i=0; i<psCommand->ui32DstSyncCount; i++)
	{
		__DtTestPoint(__DtFunc_PVRSRVProcessCommand, __DtStep_11)
		psCmdCompleteData->psDstSync[i] = psCommand->psDstSync[i];

		PVR_DPF((PVR_DBG_MESSAGE, "PVRSRVProcessCommand: Dst %u RO-VA:0x%x WO-VA:0x%x ROP:0x%x WOP:0x%x (CCB:%u)",
				i, psCmdCompleteData->psDstSync[i].psKernelSyncInfoKM->sReadOpsCompleteDevVAddr.uiAddr,
				psCmdCompleteData->psDstSync[i].psKernelSyncInfoKM->sWriteOpsCompleteDevVAddr.uiAddr,
				psCmdCompleteData->psDstSync[i].ui32ReadOpsPending,
				psCmdCompleteData->psDstSync[i].ui32WriteOpsPending,
				ui32CCBOffset));
	}


	
	psCmdCompleteData->ui32SrcSyncCount = psCommand->ui32SrcSyncCount;
	for (i=0; i<psCommand->ui32SrcSyncCount; i++)
	{
		__DtTestPoint(__DtFunc_PVRSRVProcessCommand, __DtStep_12)
		psCmdCompleteData->psSrcSync[i] = psCommand->psSrcSync[i];

		PVR_DPF((PVR_DBG_MESSAGE, "PVRSRVProcessCommand: Src %u RO-VA:0x%x WO-VA:0x%x ROP:0x%x WOP:0x%x (CCB:%u)",
				i, psCmdCompleteData->psSrcSync[i].psKernelSyncInfoKM->sReadOpsCompleteDevVAddr.uiAddr,
				psCmdCompleteData->psSrcSync[i].psKernelSyncInfoKM->sWriteOpsCompleteDevVAddr.uiAddr,
				psCmdCompleteData->psSrcSync[i].ui32ReadOpsPending,
				psCmdCompleteData->psSrcSync[i].ui32WriteOpsPending,
				ui32CCBOffset));
	}

	









	if (psDeviceCommandData[psCommand->CommandType].pfnCmdProc((IMG_HANDLE)psCmdCompleteData,
															   psCommand->ui32DataSize,
															   psCommand->pvData) == IMG_FALSE)
	{
		


		__DtTestPoint(__DtFunc_PVRSRVProcessCommand, __DtStep_13)
		psCmdCompleteData->bInUse = IMG_FALSE;
		eError = PVRSRV_ERROR_CMD_NOT_PROCESSED;
	}
	
	
	psDeviceCommandData[psCommand->CommandType].ui32CCBOffset = (ui32CCBOffset + 1) % DC_NUM_COMMANDS_PER_TYPE;

	__DtTestPoint(__DtFunc_PVRSRVProcessCommand, __DtStep_14)
	return eError;
}


static IMG_VOID PVRSRVProcessQueues_ForEachCb(PVRSRV_DEVICE_NODE *psDeviceNode)
{
	__DtTestPoint(__DtFunc_PVRSRVProcessQueues_ForEachCb, __DtStep_0)
	if (psDeviceNode->bReProcessDeviceCommandComplete &&
		psDeviceNode->pfnDeviceCommandComplete != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_PVRSRVProcessQueues_ForEachCb, __DtStep_1)
		(*psDeviceNode->pfnDeviceCommandComplete)(psDeviceNode);
	}
	__DtTestPoint(__DtFunc_PVRSRVProcessQueues_ForEachCb, __DtStep_2)
}

IMG_EXPORT
PVRSRV_ERROR PVRSRVProcessQueues(IMG_UINT32	ui32CallerID,
								 IMG_BOOL	bFlush)
{
	PVRSRV_QUEUE_INFO 	*psQueue;
	SYS_DATA			*psSysData;
	PVRSRV_COMMAND 		*psCommand;
	PVRSRV_ERROR		eError;

	__DtTestPoint(__DtFunc_PVRSRVProcessQueues, __DtStep_0)
	SysAcquireData(&psSysData);

	
	psSysData->bReProcessQueues = IMG_FALSE;

	
	eError = OSLockResource(&psSysData->sQProcessResource,
							ui32CallerID);
	if(eError != PVRSRV_OK)
	{
		
		__DtTestPoint(__DtFunc_PVRSRVProcessQueues, __DtStep_1)
		psSysData->bReProcessQueues = IMG_TRUE;

		
		if(ui32CallerID == ISR_ID)
		{
			__DtTestPoint(__DtFunc_PVRSRVProcessQueues, __DtStep_2)
			if (bFlush)
			{
				__DtTestPoint(__DtFunc_PVRSRVProcessQueues, __DtStep_3)
				PVR_DPF((PVR_DBG_ERROR,"PVRSRVProcessQueues: Couldn't acquire queue processing lock for FLUSH"));
			}
			else
			{
				__DtTestPoint(__DtFunc_PVRSRVProcessQueues, __DtStep_4)
				PVR_DPF((PVR_DBG_MESSAGE,"PVRSRVProcessQueues: Couldn't acquire queue processing lock"));
			}
		}
		else
		{
			__DtTestPoint(__DtFunc_PVRSRVProcessQueues, __DtStep_5)
			PVR_DPF((PVR_DBG_MESSAGE,"PVRSRVProcessQueues: Queue processing lock-acquire failed when called from the Services driver."));
			PVR_DPF((PVR_DBG_MESSAGE,"                     This is due to MISR queue processing being interrupted by the Services driver."));
		}

		__DtTestPoint(__DtFunc_PVRSRVProcessQueues, __DtStep_6)
		return PVRSRV_OK;
	}

	psQueue = psSysData->psQueueList;

	if(!psQueue)
	{
		__DtTestPoint(__DtFunc_PVRSRVProcessQueues, __DtStep_7)
		PVR_DPF((PVR_DBG_MESSAGE,"No Queues installed - cannot process commands"));
	}

	if (bFlush)
	{
		__DtTestPoint(__DtFunc_PVRSRVProcessQueues, __DtStep_8)
		PVRSRVSetDCState(DC_STATE_FLUSH_COMMANDS);
	}

	while (psQueue)
	{
		__DtTestPoint(__DtFunc_PVRSRVProcessQueues, __DtStep_9)
		while (psQueue->ui32ReadOffset != psQueue->ui32WriteOffset)
		{
			__DtTestPoint(__DtFunc_PVRSRVProcessQueues, __DtStep_10)
			psCommand = (PVRSRV_COMMAND*)((IMG_UINTPTR_T)psQueue->pvLinQueueKM + psQueue->ui32ReadOffset);

			if (PVRSRVProcessCommand(psSysData, psCommand, bFlush) == PVRSRV_OK)
			{
				
				__DtTestPoint(__DtFunc_PVRSRVProcessQueues, __DtStep_11)
				UPDATE_QUEUE_ROFF(psQueue, psCommand->ui32CmdSize)

				continue;
			}

			break;
		}
		psQueue = psQueue->psNextKM;
	}

	if (bFlush)
	{
		__DtTestPoint(__DtFunc_PVRSRVProcessQueues, __DtStep_12)
		PVRSRVSetDCState(DC_STATE_NO_FLUSH_COMMANDS);
	}

	
	List_PVRSRV_DEVICE_NODE_ForEach(psSysData->psDeviceNodeList,
									&PVRSRVProcessQueues_ForEachCb);


	
	OSUnlockResource(&psSysData->sQProcessResource, ui32CallerID);

	
	if(psSysData->bReProcessQueues)
	{
		__DtTestPoint(__DtFunc_PVRSRVProcessQueues, __DtStep_13)
		return PVRSRV_ERROR_PROCESSING_BLOCKED;
	}

	__DtTestPoint(__DtFunc_PVRSRVProcessQueues, __DtStep_14)
	return PVRSRV_OK;
}

#if defined(SUPPORT_CUSTOM_SWAP_OPERATIONS)
IMG_INTERNAL
IMG_VOID PVRSRVFreeCommandCompletePacketKM(IMG_HANDLE	hCmdCookie,
										   IMG_BOOL		bScheduleMISR)
{
	COMMAND_COMPLETE_DATA	*psCmdCompleteData = (COMMAND_COMPLETE_DATA *)hCmdCookie;
	SYS_DATA				*psSysData;

	__DtTestPoint(__DtFunc_PVRSRVFreeCommandCompletePacketKM, __DtStep_0)
	SysAcquireData(&psSysData);

	
	psCmdCompleteData->bInUse = IMG_FALSE;

	
	PVRSRVScheduleDeviceCallbacks();

	if(bScheduleMISR)
	{
		__DtTestPoint(__DtFunc_PVRSRVFreeCommandCompletePacketKM, __DtStep_1)
		OSScheduleMISR(psSysData);
	}
	__DtTestPoint(__DtFunc_PVRSRVFreeCommandCompletePacketKM, __DtStep_2)
}

#endif 


IMG_EXPORT
IMG_VOID PVRSRVCommandCompleteKM(IMG_HANDLE	hCmdCookie,
								 IMG_BOOL	bScheduleMISR)
{
	IMG_UINT32				i;
	COMMAND_COMPLETE_DATA	*psCmdCompleteData = (COMMAND_COMPLETE_DATA *)hCmdCookie;
	SYS_DATA				*psSysData;

	__DtTestPoint(__DtFunc_PVRSRVCommandCompleteKM, __DtStep_0)
	SysAcquireData(&psSysData);

	
	for (i=0; i<psCmdCompleteData->ui32DstSyncCount; i++)
	{
		__DtTestPoint(__DtFunc_PVRSRVCommandCompleteKM, __DtStep_1)
		psCmdCompleteData->psDstSync[i].psKernelSyncInfoKM->psSyncData->ui32WriteOpsComplete++;

		PVR_DPF((PVR_DBG_MESSAGE, "PVRSRVCommandCompleteKM: Dst %u RO-VA:0x%x WO-VA:0x%x ROP:0x%x WOP:0x%x",
				i, psCmdCompleteData->psDstSync[i].psKernelSyncInfoKM->sReadOpsCompleteDevVAddr.uiAddr,
				psCmdCompleteData->psDstSync[i].psKernelSyncInfoKM->sWriteOpsCompleteDevVAddr.uiAddr,
				psCmdCompleteData->psDstSync[i].ui32ReadOpsPending,
				psCmdCompleteData->psDstSync[i].ui32WriteOpsPending));
	}

	
	for (i=0; i<psCmdCompleteData->ui32SrcSyncCount; i++)
	{
		__DtTestPoint(__DtFunc_PVRSRVCommandCompleteKM, __DtStep_2)
		psCmdCompleteData->psSrcSync[i].psKernelSyncInfoKM->psSyncData->ui32ReadOpsComplete++;

		PVR_DPF((PVR_DBG_MESSAGE, "PVRSRVCommandCompleteKM: Src %u RO-VA:0x%x WO-VA:0x%x ROP:0x%x WOP:0x%x",
				i, psCmdCompleteData->psSrcSync[i].psKernelSyncInfoKM->sReadOpsCompleteDevVAddr.uiAddr,
				psCmdCompleteData->psSrcSync[i].psKernelSyncInfoKM->sWriteOpsCompleteDevVAddr.uiAddr,
				psCmdCompleteData->psSrcSync[i].ui32ReadOpsPending,
				psCmdCompleteData->psSrcSync[i].ui32WriteOpsPending));
	}

	
	psCmdCompleteData->bInUse = IMG_FALSE;

	
	PVRSRVScheduleDeviceCallbacks();

	if(bScheduleMISR)
	{
		__DtTestPoint(__DtFunc_PVRSRVCommandCompleteKM, __DtStep_3)
		OSScheduleMISR(psSysData);
	}
	__DtTestPoint(__DtFunc_PVRSRVCommandCompleteKM, __DtStep_4)
}




IMG_EXPORT
PVRSRV_ERROR PVRSRVRegisterCmdProcListKM(IMG_UINT32		ui32DevIndex,
										 PFN_CMD_PROC	*ppfnCmdProcList,
										 IMG_UINT32		ui32MaxSyncsPerCmd[][2],
										 IMG_UINT32		ui32CmdCount)
{
	SYS_DATA				*psSysData;
	PVRSRV_ERROR			eError;
	IMG_UINT32				ui32CmdCounter, ui32CmdTypeCounter;
	IMG_SIZE_T				ui32AllocSize;
	DEVICE_COMMAND_DATA		*psDeviceCommandData;
	COMMAND_COMPLETE_DATA	*psCmdCompleteData;

	
	__DtTestPoint(__DtFunc_PVRSRVRegisterCmdProcListKM, __DtStep_0)
	if(ui32DevIndex >= SYS_DEVICE_COUNT)
	{
		__DtTestPoint(__DtFunc_PVRSRVRegisterCmdProcListKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,
					"PVRSRVRegisterCmdProcListKM: invalid DeviceType 0x%x",
					ui32DevIndex));
		__DtTestPoint(__DtFunc_PVRSRVRegisterCmdProcListKM, __DtStep_2)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	
	SysAcquireData(&psSysData);

	
	ui32AllocSize = ui32CmdCount * sizeof(*psDeviceCommandData);
	eError = OSAllocMem(PVRSRV_OS_NON_PAGEABLE_HEAP,
						ui32AllocSize,
						(IMG_VOID **)&psDeviceCommandData, IMG_NULL,
						"Array of Pointers for Command Store");
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVRegisterCmdProcListKM, __DtStep_3)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVRegisterCmdProcListKM: Failed to alloc CC data"));
		goto ErrorExit;
	}

	psSysData->apsDeviceCommandData[ui32DevIndex] = psDeviceCommandData;

	for (ui32CmdTypeCounter = 0; ui32CmdTypeCounter < ui32CmdCount; ui32CmdTypeCounter++)
	{
		__DtTestPoint(__DtFunc_PVRSRVRegisterCmdProcListKM, __DtStep_4)
		psDeviceCommandData[ui32CmdTypeCounter].pfnCmdProc = ppfnCmdProcList[ui32CmdTypeCounter];
		psDeviceCommandData[ui32CmdTypeCounter].ui32CCBOffset = 0;
		
		for (ui32CmdCounter = 0; ui32CmdCounter < DC_NUM_COMMANDS_PER_TYPE; ui32CmdCounter++)
		{
			

			__DtTestPoint(__DtFunc_PVRSRVRegisterCmdProcListKM, __DtStep_5)
			ui32AllocSize = sizeof(COMMAND_COMPLETE_DATA) 
						  + ((ui32MaxSyncsPerCmd[ui32CmdTypeCounter][0]
						  +	ui32MaxSyncsPerCmd[ui32CmdTypeCounter][1])
						  * sizeof(PVRSRV_SYNC_OBJECT));	 

			eError = OSAllocMem(PVRSRV_OS_NON_PAGEABLE_HEAP,
								ui32AllocSize,
								(IMG_VOID **)&psCmdCompleteData,
								IMG_NULL,
								"Command Complete Data");
			if (eError != PVRSRV_OK)
			{
				__DtTestPoint(__DtFunc_PVRSRVRegisterCmdProcListKM, __DtStep_6)
				PVR_DPF((PVR_DBG_ERROR,"PVRSRVRegisterCmdProcListKM: Failed to alloc cmd %d", ui32CmdTypeCounter));
				goto ErrorExit;
			}
			
			psDeviceCommandData[ui32CmdTypeCounter].apsCmdCompleteData[ui32CmdCounter] = psCmdCompleteData;
			
			
			OSMemSet(psCmdCompleteData, 0x00, ui32AllocSize);

			
			psCmdCompleteData->psDstSync = (PVRSRV_SYNC_OBJECT*)
											(((IMG_UINTPTR_T)psCmdCompleteData)
											+ sizeof(COMMAND_COMPLETE_DATA));
			psCmdCompleteData->psSrcSync = (PVRSRV_SYNC_OBJECT*)
											(((IMG_UINTPTR_T)psCmdCompleteData->psDstSync)
											+ (sizeof(PVRSRV_SYNC_OBJECT) * ui32MaxSyncsPerCmd[ui32CmdTypeCounter][0]));

			psCmdCompleteData->ui32AllocSize = ui32AllocSize;
		}
	}

	__DtTestPoint(__DtFunc_PVRSRVRegisterCmdProcListKM, __DtStep_7)
	return PVRSRV_OK;

ErrorExit:

	

 	if (PVRSRVRemoveCmdProcListKM(ui32DevIndex, ui32CmdCount) != PVRSRV_OK)
  	{
 		__DtTestPoint(__DtFunc_PVRSRVRegisterCmdProcListKM, __DtStep_8)
 		PVR_DPF((PVR_DBG_ERROR,
 				"PVRSRVRegisterCmdProcListKM: Failed to clean up after error, device 0x%x",
 				ui32DevIndex));
  	}

	__DtTestPoint(__DtFunc_PVRSRVRegisterCmdProcListKM, __DtStep_9)
	return eError;
}


IMG_EXPORT
PVRSRV_ERROR PVRSRVRemoveCmdProcListKM(IMG_UINT32 ui32DevIndex,
									   IMG_UINT32 ui32CmdCount)
{
	SYS_DATA				*psSysData;
	IMG_UINT32				ui32CmdTypeCounter, ui32CmdCounter;
	DEVICE_COMMAND_DATA		*psDeviceCommandData;
	COMMAND_COMPLETE_DATA	*psCmdCompleteData;
	IMG_SIZE_T				ui32AllocSize;

	
	__DtTestPoint(__DtFunc_PVRSRVRemoveCmdProcListKM, __DtStep_0)
	if(ui32DevIndex >= SYS_DEVICE_COUNT)
	{
		__DtTestPoint(__DtFunc_PVRSRVRemoveCmdProcListKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,
				"PVRSRVRemoveCmdProcListKM: invalid DeviceType 0x%x",
				ui32DevIndex));
		__DtTestPoint(__DtFunc_PVRSRVRemoveCmdProcListKM, __DtStep_2)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	
	SysAcquireData(&psSysData);

	psDeviceCommandData = psSysData->apsDeviceCommandData[ui32DevIndex];
	if(psDeviceCommandData != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_PVRSRVRemoveCmdProcListKM, __DtStep_3)
		for (ui32CmdTypeCounter = 0; ui32CmdTypeCounter < ui32CmdCount; ui32CmdTypeCounter++)
		{
			__DtTestPoint(__DtFunc_PVRSRVRemoveCmdProcListKM, __DtStep_4)
			for (ui32CmdCounter = 0; ui32CmdCounter < DC_NUM_COMMANDS_PER_TYPE; ui32CmdCounter++)
			{
				__DtTestPoint(__DtFunc_PVRSRVRemoveCmdProcListKM, __DtStep_5)
				psCmdCompleteData = psDeviceCommandData[ui32CmdTypeCounter].apsCmdCompleteData[ui32CmdCounter];
				
				
				if (psCmdCompleteData != IMG_NULL)
				{
					__DtTestPoint(__DtFunc_PVRSRVRemoveCmdProcListKM, __DtStep_6)
					OSFreeMem(PVRSRV_OS_NON_PAGEABLE_HEAP, psCmdCompleteData->ui32AllocSize,
							  psCmdCompleteData, IMG_NULL);
					psDeviceCommandData[ui32CmdTypeCounter].apsCmdCompleteData[ui32CmdCounter] = IMG_NULL;
				}
			}
		}

		
		ui32AllocSize = ui32CmdCount * sizeof(*psDeviceCommandData);
		OSFreeMem(PVRSRV_OS_NON_PAGEABLE_HEAP, ui32AllocSize, psDeviceCommandData, IMG_NULL);
		psSysData->apsDeviceCommandData[ui32DevIndex] = IMG_NULL;
	}

	__DtTestPoint(__DtFunc_PVRSRVRemoveCmdProcListKM, __DtStep_7)
	return PVRSRV_OK;
}

