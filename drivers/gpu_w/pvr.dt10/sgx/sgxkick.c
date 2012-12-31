#include	"DT_sgxkick.h"	/* For DynamicTracer-TestPoint */
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

#include <stddef.h> 
#include "services_headers.h"
#include "sgxinfo.h"
#include "sgxinfokm.h"
#if defined (PDUMP)
#include "sgxapi_km.h"
#include "pdump_km.h"
#endif
#include "sgx_bridge_km.h"
#include "osfunc.h"
#include "pvr_debug.h"
#include "sgxutils.h"

IMG_EXPORT
PVRSRV_ERROR SGXDoKickKM(IMG_HANDLE hDevHandle, SGX_CCB_KICK *psCCBKick)
{
	PVRSRV_ERROR eError;
	PVRSRV_KERNEL_SYNC_INFO	*psSyncInfo;
	PVRSRV_KERNEL_MEM_INFO	*psCCBMemInfo = (PVRSRV_KERNEL_MEM_INFO *) psCCBKick->hCCBKernelMemInfo;
	SGXMKIF_CMDTA_SHARED *psTACmd;
	IMG_UINT32 i;

	__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_0)
	if (!CCB_OFFSET_IS_VALID(SGXMKIF_CMDTA_SHARED, psCCBMemInfo, psCCBKick, ui32CCBOffset))
	{
		__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "SGXDoKickKM: Invalid CCB offset"));
		__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_2)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}
	
	
	psTACmd = CCB_DATA_FROM_OFFSET(SGXMKIF_CMDTA_SHARED, psCCBMemInfo, psCCBKick, ui32CCBOffset);

	
	if (psCCBKick->hTA3DSyncInfo)
	{
		__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_3)
		psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *)psCCBKick->hTA3DSyncInfo;
		psTACmd->sTA3DDependency.sWriteOpsCompleteDevVAddr = psSyncInfo->sWriteOpsCompleteDevVAddr;

		psTACmd->sTA3DDependency.ui32WriteOpsPendingVal   = psSyncInfo->psSyncData->ui32WriteOpsPending;

		if (psCCBKick->bTADependency)
		{
			__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_4)
			psSyncInfo->psSyncData->ui32WriteOpsPending++;
		}
	}

	if (psCCBKick->hTASyncInfo != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_5)
		psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *)psCCBKick->hTASyncInfo;

		psTACmd->sTATQSyncReadOpsCompleteDevVAddr  = psSyncInfo->sReadOpsCompleteDevVAddr;
		psTACmd->sTATQSyncWriteOpsCompleteDevVAddr = psSyncInfo->sWriteOpsCompleteDevVAddr;

		psTACmd->ui32TATQSyncReadOpsPendingVal = psSyncInfo->psSyncData->ui32ReadOpsPending++;
		psTACmd->ui32TATQSyncWriteOpsPendingVal = psSyncInfo->psSyncData->ui32WriteOpsPending;
	}

	if (psCCBKick->h3DSyncInfo != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_6)
		psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *)psCCBKick->h3DSyncInfo;

		psTACmd->s3DTQSyncReadOpsCompleteDevVAddr  = psSyncInfo->sReadOpsCompleteDevVAddr;
		psTACmd->s3DTQSyncWriteOpsCompleteDevVAddr = psSyncInfo->sWriteOpsCompleteDevVAddr;

		psTACmd->ui323DTQSyncReadOpsPendingVal = psSyncInfo->psSyncData->ui32ReadOpsPending++;
		psTACmd->ui323DTQSyncWriteOpsPendingVal  = psSyncInfo->psSyncData->ui32WriteOpsPending;
	}

	psTACmd->ui32NumTAStatusVals = psCCBKick->ui32NumTAStatusVals;
	if (psCCBKick->ui32NumTAStatusVals != 0)
	{
		
		__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_7)
		for (i = 0; i < psCCBKick->ui32NumTAStatusVals; i++)
		{
#if defined(SUPPORT_SGX_NEW_STATUS_VALS)
			psTACmd->sCtlTAStatusInfo[i] = psCCBKick->asTAStatusUpdate[i].sCtlStatus;
#else
			psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *)psCCBKick->ahTAStatusSyncInfo[i];
			psTACmd->sCtlTAStatusInfo[i].sStatusDevAddr = psSyncInfo->sReadOpsCompleteDevVAddr;
			psTACmd->sCtlTAStatusInfo[i].ui32StatusValue = psSyncInfo->psSyncData->ui32ReadOpsPending;
#endif
		}
	}

	psTACmd->ui32Num3DStatusVals = psCCBKick->ui32Num3DStatusVals;
	if (psCCBKick->ui32Num3DStatusVals != 0)
	{
		
		__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_8)
		for (i = 0; i < psCCBKick->ui32Num3DStatusVals; i++)
		{
#if defined(SUPPORT_SGX_NEW_STATUS_VALS)
			psTACmd->sCtl3DStatusInfo[i] = psCCBKick->as3DStatusUpdate[i].sCtlStatus;
#else
			psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *)psCCBKick->ah3DStatusSyncInfo[i];
			psTACmd->sCtl3DStatusInfo[i].sStatusDevAddr = psSyncInfo->sReadOpsCompleteDevVAddr;
			psTACmd->sCtl3DStatusInfo[i].ui32StatusValue = psSyncInfo->psSyncData->ui32ReadOpsPending;
#endif
		}
	}


#if defined(SUPPORT_SGX_GENERALISED_SYNCOBJECTS)
	
	psTACmd->ui32NumTASrcSyncs = psCCBKick->ui32NumTASrcSyncs;
	for (i=0; i<psCCBKick->ui32NumTASrcSyncs; i++)
	{
		__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_9)
		psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *) psCCBKick->ahTASrcKernelSyncInfo[i];

		psTACmd->asTASrcSyncs[i].sWriteOpsCompleteDevVAddr = psSyncInfo->sWriteOpsCompleteDevVAddr;
		psTACmd->asTASrcSyncs[i].sReadOpsCompleteDevVAddr = psSyncInfo->sReadOpsCompleteDevVAddr;

		
		psTACmd->asTASrcSyncs[i].ui32ReadOpsPendingVal = psSyncInfo->psSyncData->ui32ReadOpsPending++;
		
		psTACmd->asTASrcSyncs[i].ui32WriteOpsPendingVal = psSyncInfo->psSyncData->ui32WriteOpsPending;
	}

	psTACmd->ui32NumTADstSyncs = psCCBKick->ui32NumTADstSyncs;
	for (i=0; i<psCCBKick->ui32NumTADstSyncs; i++)
	{
		__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_10)
		psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *) psCCBKick->ahTADstKernelSyncInfo[i];

		psTACmd->asTADstSyncs[i].sWriteOpsCompleteDevVAddr = psSyncInfo->sWriteOpsCompleteDevVAddr;
		psTACmd->asTADstSyncs[i].sReadOpsCompleteDevVAddr = psSyncInfo->sReadOpsCompleteDevVAddr;

		
		psTACmd->asTADstSyncs[i].ui32ReadOpsPendingVal = psSyncInfo->psSyncData->ui32ReadOpsPending;
		
		psTACmd->asTADstSyncs[i].ui32WriteOpsPendingVal = psSyncInfo->psSyncData->ui32WriteOpsPending++;
	}

	psTACmd->ui32Num3DSrcSyncs = psCCBKick->ui32Num3DSrcSyncs;
	for (i=0; i<psCCBKick->ui32Num3DSrcSyncs; i++)
	{
		__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_11)
		psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *) psCCBKick->ah3DSrcKernelSyncInfo[i];

		psTACmd->as3DSrcSyncs[i].sWriteOpsCompleteDevVAddr = psSyncInfo->sWriteOpsCompleteDevVAddr;
		psTACmd->as3DSrcSyncs[i].sReadOpsCompleteDevVAddr = psSyncInfo->sReadOpsCompleteDevVAddr;

		
		psTACmd->as3DSrcSyncs[i].ui32ReadOpsPendingVal = psSyncInfo->psSyncData->ui32ReadOpsPending++;
		
		psTACmd->as3DSrcSyncs[i].ui32WriteOpsPendingVal = psSyncInfo->psSyncData->ui32WriteOpsPending;
	}
#else 
	
	psTACmd->ui32NumSrcSyncs = psCCBKick->ui32NumSrcSyncs;
	for (i=0; i<psCCBKick->ui32NumSrcSyncs; i++)
	{
		__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_12)
		psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *) psCCBKick->ahSrcKernelSyncInfo[i];

		psTACmd->asSrcSyncs[i].sWriteOpsCompleteDevVAddr = psSyncInfo->sWriteOpsCompleteDevVAddr;
		psTACmd->asSrcSyncs[i].sReadOpsCompleteDevVAddr = psSyncInfo->sReadOpsCompleteDevVAddr;

		
		psTACmd->asSrcSyncs[i].ui32ReadOpsPendingVal = psSyncInfo->psSyncData->ui32ReadOpsPending++;
		
		psTACmd->asSrcSyncs[i].ui32WriteOpsPendingVal = psSyncInfo->psSyncData->ui32WriteOpsPending;
	}
#endif

	if (psCCBKick->bFirstKickOrResume && psCCBKick->ui32NumDstSyncObjects > 0)
	{
		PVRSRV_KERNEL_MEM_INFO	*psHWDstSyncListMemInfo =
								(PVRSRV_KERNEL_MEM_INFO *)psCCBKick->hKernelHWSyncListMemInfo;
		SGXMKIF_HWDEVICE_SYNC_LIST *psHWDeviceSyncList = psHWDstSyncListMemInfo->pvLinAddrKM;
		IMG_UINT32	ui32NumDstSyncs = psCCBKick->ui32NumDstSyncObjects;

		__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_13)
		PVR_ASSERT(((PVRSRV_KERNEL_MEM_INFO *)psCCBKick->hKernelHWSyncListMemInfo)->ui32AllocSize >= (sizeof(SGXMKIF_HWDEVICE_SYNC_LIST) +
								(sizeof(PVRSRV_DEVICE_SYNC_OBJECT) * ui32NumDstSyncs)));

		psHWDeviceSyncList->ui32NumSyncObjects = ui32NumDstSyncs;
#if defined(PDUMP)
		if (PDumpIsCaptureFrameKM())
		{
			__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_14)
			PDUMPCOMMENT("HWDeviceSyncList for TACmd\r\n");
			PDUMPMEM(IMG_NULL,
					 psHWDstSyncListMemInfo,
					 0,
					 sizeof(SGXMKIF_HWDEVICE_SYNC_LIST),
					 0,
					 MAKEUNIQUETAG(psHWDstSyncListMemInfo));
		}
#endif

		for (i=0; i<ui32NumDstSyncs; i++)
		{
			__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_15)
			psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *)psCCBKick->pahDstSyncHandles[i];

			if (psSyncInfo)
			{
				__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_16)
				psHWDeviceSyncList->asSyncData[i].sWriteOpsCompleteDevVAddr = psSyncInfo->sWriteOpsCompleteDevVAddr;
				psHWDeviceSyncList->asSyncData[i].sReadOpsCompleteDevVAddr = psSyncInfo->sReadOpsCompleteDevVAddr;

				psHWDeviceSyncList->asSyncData[i].ui32ReadOpsPendingVal = psSyncInfo->psSyncData->ui32ReadOpsPending;
				psHWDeviceSyncList->asSyncData[i].ui32WriteOpsPendingVal = psSyncInfo->psSyncData->ui32WriteOpsPending++;

	#if defined(PDUMP)
				if (PDumpIsCaptureFrameKM())
				{
					IMG_UINT32 ui32ModifiedValue;
					IMG_UINT32 ui32SyncOffset = offsetof(SGXMKIF_HWDEVICE_SYNC_LIST, asSyncData)
												+ (i * sizeof(PVRSRV_DEVICE_SYNC_OBJECT));
					IMG_UINT32 ui32WOpsOffset = ui32SyncOffset
												+ offsetof(PVRSRV_DEVICE_SYNC_OBJECT, ui32WriteOpsPendingVal);
					IMG_UINT32 ui32ROpsOffset = ui32SyncOffset
												+ offsetof(PVRSRV_DEVICE_SYNC_OBJECT, ui32ReadOpsPendingVal);

					__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_17)
					PDUMPCOMMENT("HWDeviceSyncObject for RT: %i\r\n", i);

					PDUMPMEM(IMG_NULL,
							 psHWDstSyncListMemInfo,
							 ui32SyncOffset,
							 sizeof(PVRSRV_DEVICE_SYNC_OBJECT),
							 0,
							 MAKEUNIQUETAG(psHWDstSyncListMemInfo));

					if ((psSyncInfo->psSyncData->ui32LastOpDumpVal == 0) &&
						(psSyncInfo->psSyncData->ui32LastReadOpDumpVal == 0))
					{
						
						__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_18)
						PDUMPCOMMENT("Init RT ROpsComplete\r\n");
						PDUMPMEM(&psSyncInfo->psSyncData->ui32LastReadOpDumpVal,
							psSyncInfo->psSyncDataMemInfoKM,
							offsetof(PVRSRV_SYNC_DATA, ui32ReadOpsComplete),
							sizeof(psSyncInfo->psSyncData->ui32ReadOpsComplete),
							0,
							MAKEUNIQUETAG(psSyncInfo->psSyncDataMemInfoKM));
						
						PDUMPCOMMENT("Init RT WOpsComplete\r\n");
							PDUMPMEM(&psSyncInfo->psSyncData->ui32LastOpDumpVal,
								psSyncInfo->psSyncDataMemInfoKM,
								offsetof(PVRSRV_SYNC_DATA, ui32WriteOpsComplete),
								sizeof(psSyncInfo->psSyncData->ui32WriteOpsComplete),
								0,
								MAKEUNIQUETAG(psSyncInfo->psSyncDataMemInfoKM));
					}

					psSyncInfo->psSyncData->ui32LastOpDumpVal++;

					ui32ModifiedValue = psSyncInfo->psSyncData->ui32LastOpDumpVal - 1;

					PDUMPCOMMENT("Modify RT %d WOpPendingVal in HWDevSyncList\r\n", i);

					PDUMPMEM(&ui32ModifiedValue,
						psHWDstSyncListMemInfo,
						ui32WOpsOffset,
						sizeof(IMG_UINT32),
						0,
						MAKEUNIQUETAG(psHWDstSyncListMemInfo));

					ui32ModifiedValue = 0;
					PDUMPCOMMENT("Modify RT %d ROpsPendingVal in HWDevSyncList\r\n", i);

					PDUMPMEM(&psSyncInfo->psSyncData->ui32LastReadOpDumpVal,
						 psHWDstSyncListMemInfo,
						 ui32ROpsOffset,
						 sizeof(IMG_UINT32),
						 0,
						MAKEUNIQUETAG(psHWDstSyncListMemInfo));
				}
	#endif	
			}
			else
			{
				__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_19)
				psHWDeviceSyncList->asSyncData[i].sWriteOpsCompleteDevVAddr.uiAddr = 0;
				psHWDeviceSyncList->asSyncData[i].sReadOpsCompleteDevVAddr.uiAddr = 0;

				psHWDeviceSyncList->asSyncData[i].ui32ReadOpsPendingVal = 0;
				psHWDeviceSyncList->asSyncData[i].ui32WriteOpsPendingVal = 0;
			}
		}
	}
	
	


	psTACmd->ui32CtrlFlags |= SGXMKIF_CMDTA_CTRLFLAGS_READY;

#if defined(PDUMP)
	if (PDumpIsCaptureFrameKM())
	{
		__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_20)
		PDUMPCOMMENT("Shared part of TA command\r\n");

		PDUMPMEM(psTACmd,
				 psCCBMemInfo,
				 psCCBKick->ui32CCBDumpWOff,
				 sizeof(SGXMKIF_CMDTA_SHARED),
				 0,
				 MAKEUNIQUETAG(psCCBMemInfo));

#if defined(SUPPORT_SGX_GENERALISED_SYNCOBJECTS)
		for (i=0; i<psCCBKick->ui32NumTASrcSyncs; i++)
		{
			IMG_UINT32 	ui32ModifiedValue;
			__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_21)
			psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *) psCCBKick->ahTASrcKernelSyncInfo[i];

			if ((psSyncInfo->psSyncData->ui32LastOpDumpVal == 0) &&
				(psSyncInfo->psSyncData->ui32LastReadOpDumpVal == 0))
			{
				
				__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_22)
				PDUMPCOMMENT("Init RT TA-SRC ROpsComplete\r\n", i);
				PDUMPMEM(&psSyncInfo->psSyncData->ui32LastReadOpDumpVal,
					psSyncInfo->psSyncDataMemInfoKM,
					offsetof(PVRSRV_SYNC_DATA, ui32ReadOpsComplete),
					sizeof(psSyncInfo->psSyncData->ui32ReadOpsComplete),
					0,
					MAKEUNIQUETAG(psSyncInfo->psSyncDataMemInfoKM));
				
				PDUMPCOMMENT("Init RT TA-SRC WOpsComplete\r\n");
					PDUMPMEM(&psSyncInfo->psSyncData->ui32LastOpDumpVal,
						psSyncInfo->psSyncDataMemInfoKM,
						offsetof(PVRSRV_SYNC_DATA, ui32WriteOpsComplete),
						sizeof(psSyncInfo->psSyncData->ui32WriteOpsComplete),
						0,
						MAKEUNIQUETAG(psSyncInfo->psSyncDataMemInfoKM));
			}

			psSyncInfo->psSyncData->ui32LastReadOpDumpVal++;

			ui32ModifiedValue = psSyncInfo->psSyncData->ui32LastReadOpDumpVal - 1;

			PDUMPCOMMENT("Modify TA SrcSync %d ROpsPendingVal\r\n", i);

			PDUMPMEM(&ui32ModifiedValue,
				 psCCBMemInfo,
				 psCCBKick->ui32CCBDumpWOff + offsetof(SGXMKIF_CMDTA_SHARED, asTASrcSyncs) +
					(i * sizeof(PVRSRV_DEVICE_SYNC_OBJECT)) + offsetof(PVRSRV_DEVICE_SYNC_OBJECT, ui32ReadOpsPendingVal),
				 sizeof(IMG_UINT32),
				 0,
				MAKEUNIQUETAG(psCCBMemInfo));

			PDUMPCOMMENT("Modify TA SrcSync %d WOpPendingVal\r\n", i);

			PDUMPMEM(&psSyncInfo->psSyncData->ui32LastOpDumpVal,
				psCCBMemInfo,
				psCCBKick->ui32CCBDumpWOff + offsetof(SGXMKIF_CMDTA_SHARED, asTASrcSyncs) +
					(i * sizeof(PVRSRV_DEVICE_SYNC_OBJECT)) + offsetof(PVRSRV_DEVICE_SYNC_OBJECT, ui32WriteOpsPendingVal),
				sizeof(IMG_UINT32),
				0,
				MAKEUNIQUETAG(psCCBMemInfo));
		}

		for (i=0; i<psCCBKick->ui32NumTADstSyncs; i++)
		{
			IMG_UINT32 	ui32ModifiedValue;
			__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_23)
			psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *) psCCBKick->ahTADstKernelSyncInfo[i];

			if ((psSyncInfo->psSyncData->ui32LastOpDumpVal == 0) &&
				(psSyncInfo->psSyncData->ui32LastReadOpDumpVal == 0))
			{
				
				__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_24)
				PDUMPCOMMENT("Init RT TA-DST ROpsComplete\r\n", i);
				PDUMPMEM(&psSyncInfo->psSyncData->ui32LastReadOpDumpVal,
					psSyncInfo->psSyncDataMemInfoKM,
					offsetof(PVRSRV_SYNC_DATA, ui32ReadOpsComplete),
					sizeof(psSyncInfo->psSyncData->ui32ReadOpsComplete),
					0,
					MAKEUNIQUETAG(psSyncInfo->psSyncDataMemInfoKM));
				
				PDUMPCOMMENT("Init RT TA-DST WOpsComplete\r\n");
					PDUMPMEM(&psSyncInfo->psSyncData->ui32LastOpDumpVal,
						psSyncInfo->psSyncDataMemInfoKM,
						offsetof(PVRSRV_SYNC_DATA, ui32WriteOpsComplete),
						sizeof(psSyncInfo->psSyncData->ui32WriteOpsComplete),
						0,
						MAKEUNIQUETAG(psSyncInfo->psSyncDataMemInfoKM));
			}

			psSyncInfo->psSyncData->ui32LastOpDumpVal++;

			ui32ModifiedValue = psSyncInfo->psSyncData->ui32LastOpDumpVal - 1;

			PDUMPCOMMENT("Modify TA DstSync %d WOpPendingVal\r\n", i);

			PDUMPMEM(&ui32ModifiedValue,
				 psCCBMemInfo,
				 psCCBKick->ui32CCBDumpWOff + offsetof(SGXMKIF_CMDTA_SHARED, asTADstSyncs) +
					(i * sizeof(PVRSRV_DEVICE_SYNC_OBJECT)) + offsetof(PVRSRV_DEVICE_SYNC_OBJECT, ui32WriteOpsPendingVal),
				 sizeof(IMG_UINT32),
				 0,
				MAKEUNIQUETAG(psCCBMemInfo));

			PDUMPCOMMENT("Modify TA DstSync %d ROpsPendingVal\r\n", i);

			PDUMPMEM(&psSyncInfo->psSyncData->ui32LastReadOpDumpVal,
				psCCBMemInfo,
				psCCBKick->ui32CCBDumpWOff + offsetof(SGXMKIF_CMDTA_SHARED, asTADstSyncs) +
					(i * sizeof(PVRSRV_DEVICE_SYNC_OBJECT)) + offsetof(PVRSRV_DEVICE_SYNC_OBJECT, ui32ReadOpsPendingVal),
				sizeof(IMG_UINT32),
				0,
				MAKEUNIQUETAG(psCCBMemInfo));
		}

		for (i=0; i<psCCBKick->ui32Num3DSrcSyncs; i++)
		{
			IMG_UINT32 	ui32ModifiedValue;
			__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_25)
			psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *) psCCBKick->ah3DSrcKernelSyncInfo[i];

			if ((psSyncInfo->psSyncData->ui32LastOpDumpVal == 0) &&
				(psSyncInfo->psSyncData->ui32LastReadOpDumpVal == 0))
			{
				
				__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_26)
				PDUMPCOMMENT("Init RT 3D-SRC ROpsComplete\r\n", i);
				PDUMPMEM(&psSyncInfo->psSyncData->ui32LastReadOpDumpVal,
					psSyncInfo->psSyncDataMemInfoKM,
					offsetof(PVRSRV_SYNC_DATA, ui32ReadOpsComplete),
					sizeof(psSyncInfo->psSyncData->ui32ReadOpsComplete),
					0,
					MAKEUNIQUETAG(psSyncInfo->psSyncDataMemInfoKM));
				
				PDUMPCOMMENT("Init RT 3D-SRC WOpsComplete\r\n");
					PDUMPMEM(&psSyncInfo->psSyncData->ui32LastOpDumpVal,
						psSyncInfo->psSyncDataMemInfoKM,
						offsetof(PVRSRV_SYNC_DATA, ui32WriteOpsComplete),
						sizeof(psSyncInfo->psSyncData->ui32WriteOpsComplete),
						0,
						MAKEUNIQUETAG(psSyncInfo->psSyncDataMemInfoKM));
			}

			psSyncInfo->psSyncData->ui32LastReadOpDumpVal++;

			ui32ModifiedValue = psSyncInfo->psSyncData->ui32LastReadOpDumpVal - 1;

			PDUMPCOMMENT("Modify 3D SrcSync %d ROpsPendingVal\r\n", i);

			PDUMPMEM(&ui32ModifiedValue,
				 psCCBMemInfo,
				 psCCBKick->ui32CCBDumpWOff + offsetof(SGXMKIF_CMDTA_SHARED, as3DSrcSyncs) +
					(i * sizeof(PVRSRV_DEVICE_SYNC_OBJECT)) + offsetof(PVRSRV_DEVICE_SYNC_OBJECT, ui32ReadOpsPendingVal),
				 sizeof(IMG_UINT32),
				 0,
				MAKEUNIQUETAG(psCCBMemInfo));

			PDUMPCOMMENT("Modify 3D SrcSync %d WOpPendingVal\r\n", i);

			PDUMPMEM(&psSyncInfo->psSyncData->ui32LastOpDumpVal,
				psCCBMemInfo,
				psCCBKick->ui32CCBDumpWOff + offsetof(SGXMKIF_CMDTA_SHARED, as3DSrcSyncs) +
					(i * sizeof(PVRSRV_DEVICE_SYNC_OBJECT)) + offsetof(PVRSRV_DEVICE_SYNC_OBJECT, ui32WriteOpsPendingVal),
				sizeof(IMG_UINT32),
				0,
				MAKEUNIQUETAG(psCCBMemInfo));
		}
#else
		for (i=0; i<psCCBKick->ui32NumSrcSyncs; i++)
		{
			IMG_UINT32 	ui32ModifiedValue;
			__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_27)
			psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *) psCCBKick->ahSrcKernelSyncInfo[i];

			if ((psSyncInfo->psSyncData->ui32LastOpDumpVal == 0) &&
				(psSyncInfo->psSyncData->ui32LastReadOpDumpVal == 0))
			{
				
				__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_28)
				PDUMPCOMMENT("Init RT ROpsComplete\r\n");
				PDUMPMEM(&psSyncInfo->psSyncData->ui32LastReadOpDumpVal,
					psSyncInfo->psSyncDataMemInfoKM,
					offsetof(PVRSRV_SYNC_DATA, ui32ReadOpsComplete),
					sizeof(psSyncInfo->psSyncData->ui32ReadOpsComplete),
					0,
					MAKEUNIQUETAG(psSyncInfo->psSyncDataMemInfoKM));
				
				PDUMPCOMMENT("Init RT WOpsComplete\r\n");
					PDUMPMEM(&psSyncInfo->psSyncData->ui32LastOpDumpVal,
						psSyncInfo->psSyncDataMemInfoKM,
						offsetof(PVRSRV_SYNC_DATA, ui32WriteOpsComplete),
						sizeof(psSyncInfo->psSyncData->ui32WriteOpsComplete),
						0,
						MAKEUNIQUETAG(psSyncInfo->psSyncDataMemInfoKM));
			}

			psSyncInfo->psSyncData->ui32LastReadOpDumpVal++;

			ui32ModifiedValue = psSyncInfo->psSyncData->ui32LastReadOpDumpVal - 1;

			PDUMPCOMMENT("Modify SrcSync %d ROpsPendingVal\r\n", i);

			PDUMPMEM(&ui32ModifiedValue,
				 psCCBMemInfo,
				 psCCBKick->ui32CCBDumpWOff + offsetof(SGXMKIF_CMDTA_SHARED, asSrcSyncs) +
					(i * sizeof(PVRSRV_DEVICE_SYNC_OBJECT)) + offsetof(PVRSRV_DEVICE_SYNC_OBJECT, ui32ReadOpsPendingVal),
				 sizeof(IMG_UINT32),
				 0,
				MAKEUNIQUETAG(psCCBMemInfo));

			PDUMPCOMMENT("Modify SrcSync %d WOpPendingVal\r\n", i);

			PDUMPMEM(&psSyncInfo->psSyncData->ui32LastOpDumpVal,
				psCCBMemInfo,
				psCCBKick->ui32CCBDumpWOff + offsetof(SGXMKIF_CMDTA_SHARED, asSrcSyncs) +
					(i * sizeof(PVRSRV_DEVICE_SYNC_OBJECT)) + offsetof(PVRSRV_DEVICE_SYNC_OBJECT, ui32WriteOpsPendingVal),
				sizeof(IMG_UINT32),
				0,
				MAKEUNIQUETAG(psCCBMemInfo));
		}
#endif

		for (i = 0; i < psCCBKick->ui32NumTAStatusVals; i++)
		{
#if !defined(SUPPORT_SGX_NEW_STATUS_VALS)
			psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *)psCCBKick->ahTAStatusSyncInfo[i];
			PDUMPCOMMENT("Modify TA status value in TA cmd\r\n");
			PDUMPMEM(&psSyncInfo->psSyncData->ui32LastOpDumpVal,
				 psCCBMemInfo,
				 psCCBKick->ui32CCBDumpWOff + (IMG_UINT32)offsetof(SGXMKIF_CMDTA_SHARED, sCtlTAStatusInfo[i].ui32StatusValue),
				 sizeof(IMG_UINT32),
				 0,
				MAKEUNIQUETAG(psCCBMemInfo));
#endif
		}

		for (i = 0; i < psCCBKick->ui32Num3DStatusVals; i++)
		{
#if !defined(SUPPORT_SGX_NEW_STATUS_VALS)
			psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *)psCCBKick->ah3DStatusSyncInfo[i];
			PDUMPCOMMENT("Modify 3D status value in TA cmd\r\n");
			PDUMPMEM(&psSyncInfo->psSyncData->ui32LastOpDumpVal,
				 psCCBMemInfo,
				 psCCBKick->ui32CCBDumpWOff + (IMG_UINT32)offsetof(SGXMKIF_CMDTA_SHARED, sCtl3DStatusInfo[i].ui32StatusValue),
				 sizeof(IMG_UINT32),
				 0,
				MAKEUNIQUETAG(psCCBMemInfo));
#endif
		}
	}
#endif	

	eError = SGXScheduleCCBCommandKM(hDevHandle, SGXMKIF_CMD_TA, &psCCBKick->sCommand, KERNEL_ID, 0, psCCBKick->bLastInScene);
	if (eError == PVRSRV_ERROR_RETRY)
	{
		__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_29)
		if (psCCBKick->bFirstKickOrResume && psCCBKick->ui32NumDstSyncObjects > 0)
		{
			__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_30)
			for (i=0; i < psCCBKick->ui32NumDstSyncObjects; i++)
			{
				
				__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_31)
				psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *)psCCBKick->pahDstSyncHandles[i];

				if (psSyncInfo)
				{
					__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_32)
					psSyncInfo->psSyncData->ui32WriteOpsPending--;
#if defined(PDUMP)
					if (PDumpIsCaptureFrameKM())
					{
						__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_33)
						psSyncInfo->psSyncData->ui32LastOpDumpVal--;
					}
#endif
				}
			}
		}

#if defined(SUPPORT_SGX_GENERALISED_SYNCOBJECTS)
		for (i=0; i<psCCBKick->ui32NumTASrcSyncs; i++)
		{
			__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_34)
			psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *) psCCBKick->ahTASrcKernelSyncInfo[i];
			psSyncInfo->psSyncData->ui32ReadOpsPending--;
		}
		for (i=0; i<psCCBKick->ui32NumTADstSyncs; i++)
		{
			__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_35)
			psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *) psCCBKick->ahTADstKernelSyncInfo[i];
			psSyncInfo->psSyncData->ui32WriteOpsPending--;
		}
		for (i=0; i<psCCBKick->ui32Num3DSrcSyncs; i++)
		{
			__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_36)
			psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *) psCCBKick->ah3DSrcKernelSyncInfo[i];
			psSyncInfo->psSyncData->ui32ReadOpsPending--;
		}
#else
		for (i=0; i<psCCBKick->ui32NumSrcSyncs; i++)
		{
			__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_37)
			psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *) psCCBKick->ahSrcKernelSyncInfo[i];
			psSyncInfo->psSyncData->ui32ReadOpsPending--;
		}
#endif

		return eError;
	}
	else if (PVRSRV_OK != eError)
	{
		__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_38)
		PVR_DPF((PVR_DBG_ERROR, "SGXDoKickKM: SGXScheduleCCBCommandKM failed."));
		__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_39)
		return eError;
	}


#if defined(NO_HARDWARE)


	
	if (psCCBKick->hTA3DSyncInfo)
	{
		__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_40)
		psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *)psCCBKick->hTA3DSyncInfo;

		if (psCCBKick->bTADependency)
		{
			__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_41)
			psSyncInfo->psSyncData->ui32WriteOpsComplete = psSyncInfo->psSyncData->ui32WriteOpsPending;
		}
	}

	if (psCCBKick->hTASyncInfo != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_42)
		psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *)psCCBKick->hTASyncInfo;

		psSyncInfo->psSyncData->ui32ReadOpsComplete =  psSyncInfo->psSyncData->ui32ReadOpsPending;
	}

	if (psCCBKick->h3DSyncInfo != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_43)
		psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *)psCCBKick->h3DSyncInfo;

		psSyncInfo->psSyncData->ui32ReadOpsComplete =  psSyncInfo->psSyncData->ui32ReadOpsPending;
	}

	
	for (i = 0; i < psCCBKick->ui32NumTAStatusVals; i++)
	{
#if defined(SUPPORT_SGX_NEW_STATUS_VALS)
		PVRSRV_KERNEL_MEM_INFO *psKernelMemInfo = (PVRSRV_KERNEL_MEM_INFO*)psCCBKick->asTAStatusUpdate[i].hKernelMemInfo;
		
		*(IMG_UINT32*)((IMG_UINTPTR_T)psKernelMemInfo->pvLinAddrKM
						+ (psTACmd->sCtlTAStatusInfo[i].sStatusDevAddr.uiAddr
						- psKernelMemInfo->sDevVAddr.uiAddr)) = psTACmd->sCtlTAStatusInfo[i].ui32StatusValue;
#else
		psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *)psCCBKick->ahTAStatusSyncInfo[i];
		psSyncInfo->psSyncData->ui32ReadOpsComplete = psTACmd->sCtlTAStatusInfo[i].ui32StatusValue;
#endif
	}

#if defined(SUPPORT_SGX_GENERALISED_SYNCOBJECTS)
	
	for (i=0; i<psCCBKick->ui32NumTASrcSyncs; i++)
	{
		__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_44)
		psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *) psCCBKick->ahTASrcKernelSyncInfo[i];
		psSyncInfo->psSyncData->ui32ReadOpsComplete =  psSyncInfo->psSyncData->ui32ReadOpsPending;
	}
	for (i=0; i<psCCBKick->ui32NumTADstSyncs; i++)
	{
		__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_45)
		psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *) psCCBKick->ahTADstKernelSyncInfo[i];
		psSyncInfo->psSyncData->ui32WriteOpsComplete =  psSyncInfo->psSyncData->ui32WriteOpsPending;
	}
	for (i=0; i<psCCBKick->ui32Num3DSrcSyncs; i++)
	{
		__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_46)
		psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *) psCCBKick->ah3DSrcKernelSyncInfo[i];
		psSyncInfo->psSyncData->ui32ReadOpsComplete =  psSyncInfo->psSyncData->ui32ReadOpsPending;
	}
#else
	
	for (i=0; i<psCCBKick->ui32NumSrcSyncs; i++)
	{
		__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_47)
		psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *) psCCBKick->ahSrcKernelSyncInfo[i];
		psSyncInfo->psSyncData->ui32ReadOpsComplete =  psSyncInfo->psSyncData->ui32ReadOpsPending;
	}
#endif

	if (psCCBKick->bTerminateOrAbort)
	{
		__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_48)
		if (psCCBKick->ui32NumDstSyncObjects > 0)
		{
			PVRSRV_KERNEL_MEM_INFO	*psHWDstSyncListMemInfo =
								(PVRSRV_KERNEL_MEM_INFO *)psCCBKick->hKernelHWSyncListMemInfo;
			SGXMKIF_HWDEVICE_SYNC_LIST *psHWDeviceSyncList = psHWDstSyncListMemInfo->pvLinAddrKM;

			__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_49)
			for (i=0; i<psCCBKick->ui32NumDstSyncObjects; i++)
			{
				__DtTestPoint(__DtFunc_SGXDoKickKM, __DtStep_50)
				psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *)psCCBKick->pahDstSyncHandles[i];
				if (psSyncInfo)
					psSyncInfo->psSyncData->ui32WriteOpsComplete = psHWDeviceSyncList->asSyncData[i].ui32WriteOpsPendingVal+1;
			}
		}

		
		for (i = 0; i < psCCBKick->ui32Num3DStatusVals; i++)
		{
#if defined(SUPPORT_SGX_NEW_STATUS_VALS)
			PVRSRV_KERNEL_MEM_INFO *psKernelMemInfo = (PVRSRV_KERNEL_MEM_INFO*)psCCBKick->as3DStatusUpdate[i].hKernelMemInfo;
			
			*(IMG_UINT32*)((IMG_UINTPTR_T)psKernelMemInfo->pvLinAddrKM
							+ (psTACmd->sCtl3DStatusInfo[i].sStatusDevAddr.uiAddr
							- psKernelMemInfo->sDevVAddr.uiAddr)) = psTACmd->sCtl3DStatusInfo[i].ui32StatusValue;
#else
			psSyncInfo = (PVRSRV_KERNEL_SYNC_INFO *)psCCBKick->ah3DStatusSyncInfo[i];
			psSyncInfo->psSyncData->ui32ReadOpsComplete = psTACmd->sCtl3DStatusInfo[i].ui32StatusValue;
#endif
		}
	}
#endif

	return eError;
}

