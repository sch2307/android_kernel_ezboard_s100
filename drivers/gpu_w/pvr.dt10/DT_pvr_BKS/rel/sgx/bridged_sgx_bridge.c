#include	"DT_bridged_sgx_bridge.h"	/* For DynamicTracer-TestPoint */
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

#include "img_defs.h"

#if defined(SUPPORT_SGX)

#include "services.h"
#include "pvr_debug.h"
#include "pvr_bridge.h"
#include "sgx_bridge.h"
#include "perproc.h"
#include "power.h"
#include "pvr_bridge_km.h"
#include "sgx_bridge_km.h"

#if defined(SUPPORT_MSVDX)
	#include "msvdx_bridge.h"
#endif

#include "bridged_pvr_bridge.h"
#include "bridged_sgx_bridge.h"
#include "sgxutils.h"
#include "pdump_km.h"

static IMG_INT
SGXGetClientInfoBW(IMG_UINT32 ui32BridgeID,
				   PVRSRV_BRIDGE_IN_GETCLIENTINFO *psGetClientInfoIN,
				   PVRSRV_BRIDGE_OUT_GETCLIENTINFO *psGetClientInfoOUT,
				   PVRSRV_PER_PROCESS_DATA *psPerProc)
{
	IMG_HANDLE hDevCookieInt;

	__DtTestPoint(__DtFunc_SGXGetClientInfoBW, __DtStep_0)
	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGX_GETCLIENTINFO);

	psGetClientInfoOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase, &hDevCookieInt,
						   psGetClientInfoIN->hDevCookie,
						   PVRSRV_HANDLE_TYPE_DEV_NODE);
	if(psGetClientInfoOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXGetClientInfoBW, __DtStep_1)
		return 0;
	}

	psGetClientInfoOUT->eError =
		SGXGetClientInfoKM(hDevCookieInt,
						   &psGetClientInfoOUT->sClientInfo);
	__DtTestPoint(__DtFunc_SGXGetClientInfoBW, __DtStep_2)
	return 0;
}

static IMG_INT
SGXReleaseClientInfoBW(IMG_UINT32 ui32BridgeID,
					   PVRSRV_BRIDGE_IN_RELEASECLIENTINFO *psReleaseClientInfoIN,
					   PVRSRV_BRIDGE_RETURN *psRetOUT,
					   PVRSRV_PER_PROCESS_DATA *psPerProc)
{
	PVRSRV_SGXDEV_INFO *psDevInfo;
	IMG_HANDLE hDevCookieInt;

	__DtTestPoint(__DtFunc_SGXReleaseClientInfoBW, __DtStep_0)
	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGX_RELEASECLIENTINFO);

	psRetOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase, &hDevCookieInt,
						   psReleaseClientInfoIN->hDevCookie,
						   PVRSRV_HANDLE_TYPE_DEV_NODE);
	if(psRetOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXReleaseClientInfoBW, __DtStep_1)
		return 0;
	}

	psDevInfo = (PVRSRV_SGXDEV_INFO *)((PVRSRV_DEVICE_NODE *)hDevCookieInt)->pvDevice;

	PVR_ASSERT(psDevInfo->ui32ClientRefCount > 0);

	psDevInfo->ui32ClientRefCount--;

	psRetOUT->eError = PVRSRV_OK;

	__DtTestPoint(__DtFunc_SGXReleaseClientInfoBW, __DtStep_2)
	return 0;
}


static IMG_INT
SGXGetInternalDevInfoBW(IMG_UINT32 ui32BridgeID,
						PVRSRV_BRIDGE_IN_GETINTERNALDEVINFO *psSGXGetInternalDevInfoIN,
						PVRSRV_BRIDGE_OUT_GETINTERNALDEVINFO *psSGXGetInternalDevInfoOUT,
						PVRSRV_PER_PROCESS_DATA *psPerProc)
{
	IMG_HANDLE hDevCookieInt;

	__DtTestPoint(__DtFunc_SGXGetInternalDevInfoBW, __DtStep_0)
	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGX_GETINTERNALDEVINFO);

	psSGXGetInternalDevInfoOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase, &hDevCookieInt,
						   psSGXGetInternalDevInfoIN->hDevCookie,
						   PVRSRV_HANDLE_TYPE_DEV_NODE);
	if(psSGXGetInternalDevInfoOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXGetInternalDevInfoBW, __DtStep_1)
		return 0;
	}

	psSGXGetInternalDevInfoOUT->eError =
		SGXGetInternalDevInfoKM(hDevCookieInt,
								&psSGXGetInternalDevInfoOUT->sSGXInternalDevInfo);

	
	psSGXGetInternalDevInfoOUT->eError =
		PVRSRVAllocHandle(psPerProc->psHandleBase,
						  &psSGXGetInternalDevInfoOUT->sSGXInternalDevInfo.hHostCtlKernelMemInfoHandle,
						  psSGXGetInternalDevInfoOUT->sSGXInternalDevInfo.hHostCtlKernelMemInfoHandle,
						  PVRSRV_HANDLE_TYPE_MEM_INFO,
						  PVRSRV_HANDLE_ALLOC_FLAG_SHARED);

	__DtTestPoint(__DtFunc_SGXGetInternalDevInfoBW, __DtStep_2)
	return 0;
}


static IMG_INT
SGXDoKickBW(IMG_UINT32 ui32BridgeID,
			PVRSRV_BRIDGE_IN_DOKICK *psDoKickIN,
			PVRSRV_BRIDGE_RETURN *psRetOUT,
			PVRSRV_PER_PROCESS_DATA *psPerProc)
{
	IMG_HANDLE hDevCookieInt;
	IMG_UINT32 i;
	IMG_INT ret = 0;
	IMG_UINT32 ui32NumDstSyncs;
	IMG_HANDLE *phKernelSyncInfoHandles = IMG_NULL;

	__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_0)
	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGX_DOKICK);

	psRetOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDevCookieInt,
						   psDoKickIN->hDevCookie,
						   PVRSRV_HANDLE_TYPE_DEV_NODE);

	if(psRetOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_1)
		return 0;
	}

	psRetOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &psDoKickIN->sCCBKick.hCCBKernelMemInfo,
						   psDoKickIN->sCCBKick.hCCBKernelMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);

	if(psRetOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_2)
		return 0;
	}

	if(psDoKickIN->sCCBKick.hTA3DSyncInfo != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_3)
		psRetOUT->eError =
			PVRSRVLookupHandle(psPerProc->psHandleBase,
							   &psDoKickIN->sCCBKick.hTA3DSyncInfo,
							   psDoKickIN->sCCBKick.hTA3DSyncInfo,
							   PVRSRV_HANDLE_TYPE_SYNC_INFO);

		if(psRetOUT->eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_4)
			return 0;
		}
	}

	if(psDoKickIN->sCCBKick.hTASyncInfo != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_5)
		psRetOUT->eError =
			PVRSRVLookupHandle(psPerProc->psHandleBase,
							   &psDoKickIN->sCCBKick.hTASyncInfo,
							   psDoKickIN->sCCBKick.hTASyncInfo,
							   PVRSRV_HANDLE_TYPE_SYNC_INFO);

		if(psRetOUT->eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_6)
			return 0;
		}
	}

	if(psDoKickIN->sCCBKick.h3DSyncInfo != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_7)
		psRetOUT->eError =
			PVRSRVLookupHandle(psPerProc->psHandleBase,
							   &psDoKickIN->sCCBKick.h3DSyncInfo,
							   psDoKickIN->sCCBKick.h3DSyncInfo,
							   PVRSRV_HANDLE_TYPE_SYNC_INFO);

		if(psRetOUT->eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_8)
			return 0;
		}
	}


#if defined(SUPPORT_SGX_GENERALISED_SYNCOBJECTS)
	
	if (psDoKickIN->sCCBKick.ui32NumTASrcSyncs > SGX_MAX_TA_SRC_SYNCS)
	{
		__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_9)
		psRetOUT->eError = PVRSRV_ERROR_INVALID_PARAMS;
		__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_10)
		return 0;
	}

	for(i=0; i<psDoKickIN->sCCBKick.ui32NumTASrcSyncs; i++)
	{
		__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_11)
		psRetOUT->eError =
			PVRSRVLookupHandle(psPerProc->psHandleBase,
							   &psDoKickIN->sCCBKick.ahTASrcKernelSyncInfo[i],
							   psDoKickIN->sCCBKick.ahTASrcKernelSyncInfo[i],
							   PVRSRV_HANDLE_TYPE_SYNC_INFO);

		if(psRetOUT->eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_12)
			return 0;
		}
	}

	if (psDoKickIN->sCCBKick.ui32NumTADstSyncs > SGX_MAX_TA_DST_SYNCS)
	{
		__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_13)
		psRetOUT->eError = PVRSRV_ERROR_INVALID_PARAMS;
		__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_14)
		return 0;
	}

	for(i=0; i<psDoKickIN->sCCBKick.ui32NumTADstSyncs; i++)
	{
		__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_15)
		psRetOUT->eError =
			PVRSRVLookupHandle(psPerProc->psHandleBase,
							   &psDoKickIN->sCCBKick.ahTADstKernelSyncInfo[i],
							   psDoKickIN->sCCBKick.ahTADstKernelSyncInfo[i],
							   PVRSRV_HANDLE_TYPE_SYNC_INFO);

		if(psRetOUT->eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_16)
			return 0;
		}
	}

	if (psDoKickIN->sCCBKick.ui32Num3DSrcSyncs > SGX_MAX_3D_SRC_SYNCS)
	{
		__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_17)
		psRetOUT->eError = PVRSRV_ERROR_INVALID_PARAMS;
		__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_18)
		return 0;
	}

	for(i=0; i<psDoKickIN->sCCBKick.ui32Num3DSrcSyncs; i++)
	{
		__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_19)
		psRetOUT->eError =
			PVRSRVLookupHandle(psPerProc->psHandleBase,
							   &psDoKickIN->sCCBKick.ah3DSrcKernelSyncInfo[i],
							   psDoKickIN->sCCBKick.ah3DSrcKernelSyncInfo[i],
							   PVRSRV_HANDLE_TYPE_SYNC_INFO);

		if(psRetOUT->eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_20)
			return 0;
		}
	}
#else
	
	if (psDoKickIN->sCCBKick.ui32NumSrcSyncs > SGX_MAX_SRC_SYNCS)
	{
		__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_21)
		psRetOUT->eError = PVRSRV_ERROR_INVALID_PARAMS;
		__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_22)
		return 0;
	}
	for(i=0; i<psDoKickIN->sCCBKick.ui32NumSrcSyncs; i++)
	{
		__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_23)
		psRetOUT->eError =
			PVRSRVLookupHandle(psPerProc->psHandleBase,
							   &psDoKickIN->sCCBKick.ahSrcKernelSyncInfo[i],
							   psDoKickIN->sCCBKick.ahSrcKernelSyncInfo[i],
							   PVRSRV_HANDLE_TYPE_SYNC_INFO);

		if(psRetOUT->eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_24)
			return 0;
		}
	}
#endif

	if (psDoKickIN->sCCBKick.ui32NumTAStatusVals > SGX_MAX_TA_STATUS_VALS)
	{
		__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_25)
		psRetOUT->eError = PVRSRV_ERROR_INVALID_PARAMS;
		__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_26)
		return 0;
	}
	for (i = 0; i < psDoKickIN->sCCBKick.ui32NumTAStatusVals; i++)
	{
		__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_27)
		psRetOUT->eError =
#if defined(SUPPORT_SGX_NEW_STATUS_VALS)
			PVRSRVLookupHandle(psPerProc->psHandleBase,
							   &psDoKickIN->sCCBKick.asTAStatusUpdate[i].hKernelMemInfo,
							   psDoKickIN->sCCBKick.asTAStatusUpdate[i].hKernelMemInfo,
							   PVRSRV_HANDLE_TYPE_MEM_INFO);
#else
			PVRSRVLookupHandle(psPerProc->psHandleBase,
							   &psDoKickIN->sCCBKick.ahTAStatusSyncInfo[i],
							   psDoKickIN->sCCBKick.ahTAStatusSyncInfo[i],
							   PVRSRV_HANDLE_TYPE_SYNC_INFO);
#endif
		if(psRetOUT->eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_28)
			return 0;
		}
	}

	if (psDoKickIN->sCCBKick.ui32Num3DStatusVals > SGX_MAX_3D_STATUS_VALS)
	{
		__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_29)
		psRetOUT->eError = PVRSRV_ERROR_INVALID_PARAMS;
		__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_30)
		return 0;
	}
	for(i = 0; i < psDoKickIN->sCCBKick.ui32Num3DStatusVals; i++)
	{
		__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_31)
		psRetOUT->eError =
#if defined(SUPPORT_SGX_NEW_STATUS_VALS)
			PVRSRVLookupHandle(psPerProc->psHandleBase,
							   &psDoKickIN->sCCBKick.as3DStatusUpdate[i].hKernelMemInfo,
							   psDoKickIN->sCCBKick.as3DStatusUpdate[i].hKernelMemInfo,
							   PVRSRV_HANDLE_TYPE_MEM_INFO);
#else
			PVRSRVLookupHandle(psPerProc->psHandleBase,
							   &psDoKickIN->sCCBKick.ah3DStatusSyncInfo[i],
							   psDoKickIN->sCCBKick.ah3DStatusSyncInfo[i],
							   PVRSRV_HANDLE_TYPE_SYNC_INFO);
#endif

		if(psRetOUT->eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_32)
			return 0;
		}
	}

	ui32NumDstSyncs = psDoKickIN->sCCBKick.ui32NumDstSyncObjects;

	if(ui32NumDstSyncs > 0)
	{
		__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_33)
		if(!OSAccessOK(PVR_VERIFY_READ,
						psDoKickIN->sCCBKick.pahDstSyncHandles,
						ui32NumDstSyncs * sizeof(IMG_HANDLE)))
		{
			__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_34)
			PVR_DPF((PVR_DBG_ERROR, "%s: SGXDoKickBW:"
					" Invalid pasDstSyncHandles pointer", __FUNCTION__));
			__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_35)
			return -EFAULT;
		}

		psRetOUT->eError = OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP,
										ui32NumDstSyncs * sizeof(IMG_HANDLE),
										(IMG_VOID **)&phKernelSyncInfoHandles,
										0,
										"Array of Synchronization Info Handles");
		if (psRetOUT->eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_36)
			return 0;
		}

		if(CopyFromUserWrapper(psPerProc,
							ui32BridgeID,
							phKernelSyncInfoHandles,
							psDoKickIN->sCCBKick.pahDstSyncHandles,
							ui32NumDstSyncs * sizeof(IMG_HANDLE)) != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_37)
			ret = -EFAULT;
			goto PVRSRV_BRIDGE_SGX_DOKICK_RETURN_RESULT;
		}

		
		psDoKickIN->sCCBKick.pahDstSyncHandles = phKernelSyncInfoHandles;

		for( i = 0; i < ui32NumDstSyncs; i++)
		{
			__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_38)
			psRetOUT->eError =
				PVRSRVLookupHandle(psPerProc->psHandleBase,
									&psDoKickIN->sCCBKick.pahDstSyncHandles[i],
									psDoKickIN->sCCBKick.pahDstSyncHandles[i],
									PVRSRV_HANDLE_TYPE_SYNC_INFO);

			if(psRetOUT->eError != PVRSRV_OK)
			{
				__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_39)
				goto PVRSRV_BRIDGE_SGX_DOKICK_RETURN_RESULT;
			}

		}

		psRetOUT->eError =
					PVRSRVLookupHandle(psPerProc->psHandleBase,
									   &psDoKickIN->sCCBKick.hKernelHWSyncListMemInfo,
									   psDoKickIN->sCCBKick.hKernelHWSyncListMemInfo,
									   PVRSRV_HANDLE_TYPE_MEM_INFO);

		if(psRetOUT->eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_40)
			goto PVRSRV_BRIDGE_SGX_DOKICK_RETURN_RESULT;
		}
	}

	psRetOUT->eError =
		SGXDoKickKM(hDevCookieInt,
					&psDoKickIN->sCCBKick);

PVRSRV_BRIDGE_SGX_DOKICK_RETURN_RESULT:

	if(phKernelSyncInfoHandles)
	{
		__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_41)
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP,
				  ui32NumDstSyncs * sizeof(IMG_HANDLE),
				  (IMG_VOID *)phKernelSyncInfoHandles,
				  0);
		
	}
	__DtTestPoint(__DtFunc_SGXDoKickBW, __DtStep_42)
	return ret;
}


static IMG_INT
SGXScheduleProcessQueuesBW(IMG_UINT32 ui32BridgeID,
			PVRSRV_BRIDGE_IN_SGX_SCHEDULE_PROCESS_QUEUES *psScheduleProcQIN,
			PVRSRV_BRIDGE_RETURN *psRetOUT,
			PVRSRV_PER_PROCESS_DATA *psPerProc)
{
	IMG_HANDLE hDevCookieInt;

	__DtTestPoint(__DtFunc_SGXScheduleProcessQueuesBW, __DtStep_0)
	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGX_SCHEDULE_PROCESS_QUEUES);

	psRetOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDevCookieInt,
						   psScheduleProcQIN->hDevCookie,
						   PVRSRV_HANDLE_TYPE_DEV_NODE);

	if(psRetOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXScheduleProcessQueuesBW, __DtStep_1)
		return 0;
	}

	psRetOUT->eError = SGXScheduleProcessQueuesKM(hDevCookieInt);

	__DtTestPoint(__DtFunc_SGXScheduleProcessQueuesBW, __DtStep_2)
	return 0;
}


#if defined(TRANSFER_QUEUE)
static IMG_INT
SGXSubmitTransferBW(IMG_UINT32 ui32BridgeID,
			PVRSRV_BRIDGE_IN_SUBMITTRANSFER *psSubmitTransferIN,
			PVRSRV_BRIDGE_RETURN *psRetOUT,
			PVRSRV_PER_PROCESS_DATA *psPerProc)
{
	IMG_HANDLE hDevCookieInt;
	PVRSRV_TRANSFER_SGX_KICK *psKick;
	IMG_UINT32 i;

	__DtTestPoint(__DtFunc_SGXSubmitTransferBW, __DtStep_0)
	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGX_SUBMITTRANSFER);
	PVR_UNREFERENCED_PARAMETER(ui32BridgeID);

	psKick = &psSubmitTransferIN->sKick;

	psRetOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDevCookieInt,
						   psSubmitTransferIN->hDevCookie,
						   PVRSRV_HANDLE_TYPE_DEV_NODE);
	if(psRetOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXSubmitTransferBW, __DtStep_1)
		return 0;
	}

	psRetOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &psKick->hCCBMemInfo,
						   psKick->hCCBMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if(psRetOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXSubmitTransferBW, __DtStep_2)
		return 0;
	}

	if (psKick->hTASyncInfo != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_SGXSubmitTransferBW, __DtStep_3)
		psRetOUT->eError =
			PVRSRVLookupHandle(psPerProc->psHandleBase,
							   &psKick->hTASyncInfo,
							   psKick->hTASyncInfo,
							   PVRSRV_HANDLE_TYPE_SYNC_INFO);
		if(psRetOUT->eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXSubmitTransferBW, __DtStep_4)
			return 0;
		}
	}

	if (psKick->h3DSyncInfo != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_SGXSubmitTransferBW, __DtStep_5)
		psRetOUT->eError =
			PVRSRVLookupHandle(psPerProc->psHandleBase,
							   &psKick->h3DSyncInfo,
							   psKick->h3DSyncInfo,
							   PVRSRV_HANDLE_TYPE_SYNC_INFO);
		if(psRetOUT->eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXSubmitTransferBW, __DtStep_6)
			return 0;
		}
	}

	if (psKick->ui32NumSrcSync > SGX_MAX_TRANSFER_SYNC_OPS)
	{
		__DtTestPoint(__DtFunc_SGXSubmitTransferBW, __DtStep_7)
		psRetOUT->eError = PVRSRV_ERROR_INVALID_PARAMS;
		__DtTestPoint(__DtFunc_SGXSubmitTransferBW, __DtStep_8)
		return 0;
	}
	for (i = 0; i < psKick->ui32NumSrcSync; i++)
	{
		__DtTestPoint(__DtFunc_SGXSubmitTransferBW, __DtStep_9)
		psRetOUT->eError =
			PVRSRVLookupHandle(psPerProc->psHandleBase,
							   &psKick->ahSrcSyncInfo[i],
							   psKick->ahSrcSyncInfo[i],
							   PVRSRV_HANDLE_TYPE_SYNC_INFO);
		if(psRetOUT->eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXSubmitTransferBW, __DtStep_10)
			return 0;
		}
	}

	if (psKick->ui32NumDstSync > SGX_MAX_TRANSFER_SYNC_OPS)
	{
		__DtTestPoint(__DtFunc_SGXSubmitTransferBW, __DtStep_11)
		psRetOUT->eError = PVRSRV_ERROR_INVALID_PARAMS;
		__DtTestPoint(__DtFunc_SGXSubmitTransferBW, __DtStep_12)
		return 0;
	}
	for (i = 0; i < psKick->ui32NumDstSync; i++)
	{
		__DtTestPoint(__DtFunc_SGXSubmitTransferBW, __DtStep_13)
		psRetOUT->eError =
			PVRSRVLookupHandle(psPerProc->psHandleBase,
							   &psKick->ahDstSyncInfo[i],
							   psKick->ahDstSyncInfo[i],
							   PVRSRV_HANDLE_TYPE_SYNC_INFO);
		if(psRetOUT->eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXSubmitTransferBW, __DtStep_14)
			return 0;
		}
	}

	psRetOUT->eError = SGXSubmitTransferKM(hDevCookieInt, psKick);

	__DtTestPoint(__DtFunc_SGXSubmitTransferBW, __DtStep_15)
	return 0;
}


#if defined(SGX_FEATURE_2D_HARDWARE)
static IMG_INT
SGXSubmit2DBW(IMG_UINT32 ui32BridgeID,
			PVRSRV_BRIDGE_IN_SUBMIT2D *psSubmit2DIN,
			PVRSRV_BRIDGE_RETURN *psRetOUT,
			PVRSRV_PER_PROCESS_DATA *psPerProc)
{
	IMG_HANDLE hDevCookieInt;
	PVRSRV_2D_SGX_KICK *psKick;
	IMG_UINT32 i;

	__DtTestPoint(__DtFunc_SGXSubmit2DBW, __DtStep_0)
	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGX_SUBMIT2D);
	PVR_UNREFERENCED_PARAMETER(ui32BridgeID);

	psRetOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDevCookieInt,
						   psSubmit2DIN->hDevCookie,
						   PVRSRV_HANDLE_TYPE_DEV_NODE);

	if(psRetOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXSubmit2DBW, __DtStep_1)
		return 0;
	}

	psKick = &psSubmit2DIN->sKick;

	psRetOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &psKick->hCCBMemInfo,
						   psKick->hCCBMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if(psRetOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXSubmit2DBW, __DtStep_2)
		return 0;
	}

	if (psKick->hTASyncInfo != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_SGXSubmit2DBW, __DtStep_3)
		psRetOUT->eError =
			PVRSRVLookupHandle(psPerProc->psHandleBase,
							   &psKick->hTASyncInfo,
							   psKick->hTASyncInfo,
							   PVRSRV_HANDLE_TYPE_SYNC_INFO);
		if(psRetOUT->eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXSubmit2DBW, __DtStep_4)
			return 0;
		}
	}

	if (psKick->h3DSyncInfo != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_SGXSubmit2DBW, __DtStep_5)
		psRetOUT->eError =
			PVRSRVLookupHandle(psPerProc->psHandleBase,
							   &psKick->h3DSyncInfo,
							   psKick->h3DSyncInfo,
							   PVRSRV_HANDLE_TYPE_SYNC_INFO);
		if(psRetOUT->eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXSubmit2DBW, __DtStep_6)
			return 0;
		}
	}

	if (psKick->ui32NumSrcSync > SGX_MAX_2D_SRC_SYNC_OPS)
	{
		__DtTestPoint(__DtFunc_SGXSubmit2DBW, __DtStep_7)
		psRetOUT->eError = PVRSRV_ERROR_INVALID_PARAMS;
		__DtTestPoint(__DtFunc_SGXSubmit2DBW, __DtStep_8)
		return 0;
	}
	for (i = 0; i < psKick->ui32NumSrcSync; i++)
	{
		__DtTestPoint(__DtFunc_SGXSubmit2DBW, __DtStep_9)
		psRetOUT->eError =
			PVRSRVLookupHandle(psPerProc->psHandleBase,
							   &psKick->ahSrcSyncInfo[i],
							   psKick->ahSrcSyncInfo[i],
							   PVRSRV_HANDLE_TYPE_SYNC_INFO);
		if(psRetOUT->eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXSubmit2DBW, __DtStep_10)
			return 0;
		}
	}

	if (psKick->hDstSyncInfo != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_SGXSubmit2DBW, __DtStep_11)
		psRetOUT->eError =
			PVRSRVLookupHandle(psPerProc->psHandleBase,
							   &psKick->hDstSyncInfo,
							   psKick->hDstSyncInfo,
							   PVRSRV_HANDLE_TYPE_SYNC_INFO);
		if(psRetOUT->eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXSubmit2DBW, __DtStep_12)
			return 0;
		}
	}

	psRetOUT->eError =
		SGXSubmit2DKM(hDevCookieInt, psKick);

	__DtTestPoint(__DtFunc_SGXSubmit2DBW, __DtStep_13)
	return 0;
}
#endif 
#endif 


static IMG_INT
SGXGetMiscInfoBW(IMG_UINT32 ui32BridgeID,
				 PVRSRV_BRIDGE_IN_SGXGETMISCINFO *psSGXGetMiscInfoIN,
				 PVRSRV_BRIDGE_RETURN *psRetOUT,
				 PVRSRV_PER_PROCESS_DATA *psPerProc)
{
	IMG_HANDLE hDevCookieInt;
	IMG_HANDLE hDevMemContextInt = 0;
	PVRSRV_SGXDEV_INFO *psDevInfo;
	SGX_MISC_INFO        sMiscInfo;
 	PVRSRV_DEVICE_NODE *psDeviceNode;

	__DtTestPoint(__DtFunc_SGXGetMiscInfoBW, __DtStep_0)
	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID,
							PVRSRV_BRIDGE_SGX_GETMISCINFO);

	psRetOUT->eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
							&hDevCookieInt,
							psSGXGetMiscInfoIN->hDevCookie,
							PVRSRV_HANDLE_TYPE_DEV_NODE);

	if(psRetOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXGetMiscInfoBW, __DtStep_1)
		return 0;
	}

#if defined(SUPPORT_SGX_EDM_MEMORY_DEBUG)
	
	if (psSGXGetMiscInfoIN->psMiscInfo->eRequest == SGX_MISC_INFO_REQUEST_MEMREAD)
	{
		__DtTestPoint(__DtFunc_SGXGetMiscInfoBW, __DtStep_2)
		psRetOUT->eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
								&hDevMemContextInt,
								psSGXGetMiscInfoIN->psMiscInfo->hDevMemContext,
								PVRSRV_HANDLE_TYPE_DEV_MEM_CONTEXT);

		if(psRetOUT->eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXGetMiscInfoBW, __DtStep_3)
			return 0;
		}
	}
#endif
	
	psDeviceNode = hDevCookieInt;
	PVR_ASSERT(psDeviceNode != IMG_NULL);
	if (psDeviceNode == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_SGXGetMiscInfoBW, __DtStep_4)
		return -EFAULT;
	}

	psDevInfo = psDeviceNode->pvDevice;

	
	psRetOUT->eError = CopyFromUserWrapper(psPerProc,
			                               ui32BridgeID,
			                               &sMiscInfo,
			                               psSGXGetMiscInfoIN->psMiscInfo,
			                               sizeof(SGX_MISC_INFO));
	if (psRetOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXGetMiscInfoBW, __DtStep_5)
		return -EFAULT;
	}

	{
		psRetOUT->eError = SGXGetMiscInfoKM(psDevInfo, &sMiscInfo, psDeviceNode, hDevMemContextInt);

		if (psRetOUT->eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXGetMiscInfoBW, __DtStep_6)
			return 0;
		}
	}

	
	psRetOUT->eError = CopyToUserWrapper(psPerProc,
		                             ui32BridgeID,
		                             psSGXGetMiscInfoIN->psMiscInfo,
		                             &sMiscInfo,
		                             sizeof(SGX_MISC_INFO));
	if (psRetOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXGetMiscInfoBW, __DtStep_7)
		return -EFAULT;
	}
	__DtTestPoint(__DtFunc_SGXGetMiscInfoBW, __DtStep_8)
	return 0;
}


static IMG_INT
SGXReadHWPerfCBBW(IMG_UINT32							ui32BridgeID,
				  PVRSRV_BRIDGE_IN_SGX_READ_HWPERF_CB	*psSGXReadHWPerfCBIN,
				  PVRSRV_BRIDGE_OUT_SGX_READ_HWPERF_CB	*psSGXReadHWPerfCBOUT,
				  PVRSRV_PER_PROCESS_DATA				*psPerProc)
{
	IMG_HANDLE					hDevCookieInt;
	PVRSRV_SGX_HWPERF_CB_ENTRY	*psAllocated;
	IMG_HANDLE					hAllocatedHandle;
	IMG_UINT32					ui32AllocatedSize;

	__DtTestPoint(__DtFunc_SGXReadHWPerfCBBW, __DtStep_0)
	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGX_READ_HWPERF_CB);

	psSGXReadHWPerfCBOUT->eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
							&hDevCookieInt,
							psSGXReadHWPerfCBIN->hDevCookie,
							PVRSRV_HANDLE_TYPE_DEV_NODE);

	if(psSGXReadHWPerfCBOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXReadHWPerfCBBW, __DtStep_1)
		return 0;
	}

	ui32AllocatedSize = psSGXReadHWPerfCBIN->ui32ArraySize *
							sizeof(psSGXReadHWPerfCBIN->psHWPerfCBData[0]);
	ASSIGN_AND_EXIT_ON_ERROR(psSGXReadHWPerfCBOUT->eError,
	                    OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP,
	                    ui32AllocatedSize,
	                    (IMG_VOID **)&psAllocated,
	                    &hAllocatedHandle,
						"Array of Hardware Performance Circular Buffer Data"));

	psSGXReadHWPerfCBOUT->eError = SGXReadHWPerfCBKM(hDevCookieInt,
													 psSGXReadHWPerfCBIN->ui32ArraySize,
													 psAllocated,
													 &psSGXReadHWPerfCBOUT->ui32DataCount,
													 &psSGXReadHWPerfCBOUT->ui32ClockSpeed,
													 &psSGXReadHWPerfCBOUT->ui32HostTimeStamp);
	if (psSGXReadHWPerfCBOUT->eError == PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXReadHWPerfCBBW, __DtStep_2)
		psSGXReadHWPerfCBOUT->eError = CopyToUserWrapper(psPerProc,
		                                                 ui32BridgeID,
		                                                 psSGXReadHWPerfCBIN->psHWPerfCBData,
		                                                 psAllocated,
		                                                 ui32AllocatedSize);
	}

	OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP,
			  ui32AllocatedSize,
			  psAllocated,
			  hAllocatedHandle);
	

	__DtTestPoint(__DtFunc_SGXReadHWPerfCBBW, __DtStep_3)
	return 0;
}


static IMG_INT
SGXDevInitPart2BW(IMG_UINT32 ui32BridgeID,
				  PVRSRV_BRIDGE_IN_SGXDEVINITPART2 *psSGXDevInitPart2IN,
				  PVRSRV_BRIDGE_RETURN *psRetOUT,
				  PVRSRV_PER_PROCESS_DATA *psPerProc)
{
	IMG_HANDLE hDevCookieInt;
	PVRSRV_ERROR eError;
	IMG_BOOL bDissociateFailed = IMG_FALSE;
	IMG_BOOL bLookupFailed = IMG_FALSE;
	IMG_BOOL bReleaseFailed = IMG_FALSE;
	IMG_HANDLE hDummy;
	IMG_UINT32 i;

	__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_0)
	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGX_DEVINITPART2);

	if(!psPerProc->bInitProcess)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_1)
		psRetOUT->eError = PVRSRV_ERROR_PROCESS_NOT_INITIALISED;
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_2)
		return 0;
	}

	psRetOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDevCookieInt,
						   psSGXDevInitPart2IN->hDevCookie,
						   PVRSRV_HANDLE_TYPE_DEV_NODE);
	if(psRetOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_3)
		return 0;
	}

	
	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDummy,
						   psSGXDevInitPart2IN->sInitInfo.hKernelCCBMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_4)
		bLookupFailed = IMG_TRUE;
	}

	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDummy,
						   psSGXDevInitPart2IN->sInitInfo.hKernelCCBCtlMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_5)
		bLookupFailed = IMG_TRUE;
	}

	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDummy,
						   psSGXDevInitPart2IN->sInitInfo.hKernelCCBEventKickerMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_6)
		bLookupFailed = IMG_TRUE;
	}

	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDummy,
						   psSGXDevInitPart2IN->sInitInfo.hKernelSGXHostCtlMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_7)
		bLookupFailed = IMG_TRUE;
	}

	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDummy,
						   psSGXDevInitPart2IN->sInitInfo.hKernelSGXTA3DCtlMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_8)
		bLookupFailed = IMG_TRUE;
	}


	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDummy,
						   psSGXDevInitPart2IN->sInitInfo.hKernelSGXMiscMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_9)
		bLookupFailed = IMG_TRUE;
	}

#if defined(SGX_SUPPORT_HWPROFILING)
	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDummy,
						   psSGXDevInitPart2IN->sInitInfo.hKernelHWProfilingMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_10)
		bLookupFailed = IMG_TRUE;
	}
#endif

#if defined(SUPPORT_SGX_HWPERF)
	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDummy,
						   psSGXDevInitPart2IN->sInitInfo.hKernelHWPerfCBMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_11)
		bLookupFailed = IMG_TRUE;
	}
#endif

	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
								&hDummy,
								psSGXDevInitPart2IN->sInitInfo.hKernelTASigBufferMemInfo,
								PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_12)
		bLookupFailed = IMG_TRUE;
	}

	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
								&hDummy,
								psSGXDevInitPart2IN->sInitInfo.hKernel3DSigBufferMemInfo,
								PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_13)
		bLookupFailed = IMG_TRUE;
	}

#if defined(FIX_HW_BRN_29702)
	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDummy,
						   psSGXDevInitPart2IN->sInitInfo.hKernelCFIMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_14)
		bLookupFailed = IMG_TRUE;
	}
#endif

#if defined(FIX_HW_BRN_29823)
	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDummy,
						   psSGXDevInitPart2IN->sInitInfo.hKernelDummyTermStreamMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_15)
		bLookupFailed = IMG_TRUE;
	}
#endif

#if defined(FIX_HW_BRN_31542)
	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDummy,
						   psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAVDMStreamMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_16)
		bLookupFailed = IMG_TRUE;
	}
	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDummy,
						   psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAIndexStreamMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_17)
		bLookupFailed = IMG_TRUE;
	}
	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDummy,
						   psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAPDSMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_18)
		bLookupFailed = IMG_TRUE;
	}
	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDummy,
						   psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAUSEMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_19)
		bLookupFailed = IMG_TRUE;
	}
	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDummy,
						   psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAParamMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_20)
		bLookupFailed = IMG_TRUE;
	}
	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDummy,
						   psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAPMPTMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_21)
		bLookupFailed = IMG_TRUE;
	}
	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDummy,
						   psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWADPMLSSMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_22)
		bLookupFailed = IMG_TRUE;
	}
	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDummy,
						   psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWATPCMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_23)
		bLookupFailed = IMG_TRUE;
	}
	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDummy,
						   psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAPSGRgnHdrMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_24)
		bLookupFailed = IMG_TRUE;
	}
	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDummy,
						   psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAPartiPIMMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_25)
		bLookupFailed = IMG_TRUE;
	}
#endif

#if defined(PVRSRV_USSE_EDM_STATUS_DEBUG)
	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDummy,
						   psSGXDevInitPart2IN->sInitInfo.hKernelEDMStatusBufferMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_26)
		bLookupFailed = IMG_TRUE;
	}
#endif

#if defined(SGX_FEATURE_SPM_MODE_0)
	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDummy,
						   psSGXDevInitPart2IN->sInitInfo.hKernelTmpDPMStateMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_27)
		bLookupFailed = IMG_TRUE;
	}
#endif

	for (i = 0; i < SGX_MAX_INIT_MEM_HANDLES; i++)
	{
		IMG_HANDLE hHandle = psSGXDevInitPart2IN->sInitInfo.asInitMemHandles[i];

		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_28)
		if (hHandle == IMG_NULL)
		{
			__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_29)
			continue;
		}

		eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
							   &hDummy,
							   hHandle,
							   PVRSRV_HANDLE_TYPE_MEM_INFO);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_30)
			bLookupFailed = IMG_TRUE;
		}
	}

	if (bLookupFailed)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_31)
		PVR_DPF((PVR_DBG_ERROR, "DevInitSGXPart2BW: A handle lookup failed"));
		psRetOUT->eError = PVRSRV_ERROR_INIT2_PHASE_FAILED;
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_32)
		return 0;
	}

	
	eError = PVRSRVLookupAndReleaseHandle(psPerProc->psHandleBase,
						   &psSGXDevInitPart2IN->sInitInfo.hKernelCCBMemInfo,
						   psSGXDevInitPart2IN->sInitInfo.hKernelCCBMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_33)
		bReleaseFailed = IMG_TRUE;
	}

	eError = PVRSRVLookupAndReleaseHandle(psPerProc->psHandleBase,
						   &psSGXDevInitPart2IN->sInitInfo.hKernelCCBCtlMemInfo,
						   psSGXDevInitPart2IN->sInitInfo.hKernelCCBCtlMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_34)
		bReleaseFailed = IMG_TRUE;
	}

	eError = PVRSRVLookupAndReleaseHandle(psPerProc->psHandleBase,
						   &psSGXDevInitPart2IN->sInitInfo.hKernelCCBEventKickerMemInfo,
						   psSGXDevInitPart2IN->sInitInfo.hKernelCCBEventKickerMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_35)
		bReleaseFailed = IMG_TRUE;
	}


	eError = PVRSRVLookupAndReleaseHandle(psPerProc->psHandleBase,
						   &psSGXDevInitPart2IN->sInitInfo.hKernelSGXHostCtlMemInfo,
						   psSGXDevInitPart2IN->sInitInfo.hKernelSGXHostCtlMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_36)
		bReleaseFailed = IMG_TRUE;
	}

	eError = PVRSRVLookupAndReleaseHandle(psPerProc->psHandleBase,
						   &psSGXDevInitPart2IN->sInitInfo.hKernelSGXTA3DCtlMemInfo,
						   psSGXDevInitPart2IN->sInitInfo.hKernelSGXTA3DCtlMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_37)
		bReleaseFailed = IMG_TRUE;
	}

	eError = PVRSRVLookupAndReleaseHandle(psPerProc->psHandleBase,
						   &psSGXDevInitPart2IN->sInitInfo.hKernelSGXMiscMemInfo,
						   psSGXDevInitPart2IN->sInitInfo.hKernelSGXMiscMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_38)
		bReleaseFailed = IMG_TRUE;
	}


	#if defined(SGX_SUPPORT_HWPROFILING)
	eError = PVRSRVLookupAndReleaseHandle(psPerProc->psHandleBase,
						   &psSGXDevInitPart2IN->sInitInfo.hKernelHWProfilingMemInfo,
						   psSGXDevInitPart2IN->sInitInfo.hKernelHWProfilingMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_39)
		bReleaseFailed = IMG_TRUE;
	}
#endif

#if defined(SUPPORT_SGX_HWPERF)
	eError = PVRSRVLookupAndReleaseHandle(psPerProc->psHandleBase,
						   &psSGXDevInitPart2IN->sInitInfo.hKernelHWPerfCBMemInfo,
						   psSGXDevInitPart2IN->sInitInfo.hKernelHWPerfCBMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_40)
		bReleaseFailed = IMG_TRUE;
	}
#endif

	eError = PVRSRVLookupAndReleaseHandle(psPerProc->psHandleBase,
										  &psSGXDevInitPart2IN->sInitInfo.hKernelTASigBufferMemInfo,
										  psSGXDevInitPart2IN->sInitInfo.hKernelTASigBufferMemInfo,
										  PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_41)
		bReleaseFailed = IMG_TRUE;
	}

	eError = PVRSRVLookupAndReleaseHandle(psPerProc->psHandleBase,
										  &psSGXDevInitPart2IN->sInitInfo.hKernel3DSigBufferMemInfo,
										  psSGXDevInitPart2IN->sInitInfo.hKernel3DSigBufferMemInfo,
										  PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_42)
		bReleaseFailed = IMG_TRUE;
	}

#if defined(FIX_HW_BRN_29702)
	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &psSGXDevInitPart2IN->sInitInfo.hKernelCFIMemInfo,
						   psSGXDevInitPart2IN->sInitInfo.hKernelCFIMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_43)
		bLookupFailed = IMG_TRUE;
	}
#endif

#if defined(FIX_HW_BRN_29823)
	eError = PVRSRVLookupAndReleaseHandle(psPerProc->psHandleBase,
						   &psSGXDevInitPart2IN->sInitInfo.hKernelDummyTermStreamMemInfo,
						   psSGXDevInitPart2IN->sInitInfo.hKernelDummyTermStreamMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_44)
		bReleaseFailed = IMG_TRUE;
	}
#endif


#if defined(FIX_HW_BRN_31542)
	eError = PVRSRVLookupAndReleaseHandle(psPerProc->psHandleBase,
						   &psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAVDMStreamMemInfo,
						   psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAVDMStreamMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_45)
		bReleaseFailed = IMG_TRUE;
	}
	eError = PVRSRVLookupAndReleaseHandle(psPerProc->psHandleBase,
						   &psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAIndexStreamMemInfo,
						   psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAIndexStreamMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_46)
		bReleaseFailed = IMG_TRUE;
	}
	eError = PVRSRVLookupAndReleaseHandle(psPerProc->psHandleBase,
						   &psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAPDSMemInfo,
						   psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAPDSMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_47)
		bReleaseFailed = IMG_TRUE;
	}
	eError = PVRSRVLookupAndReleaseHandle(psPerProc->psHandleBase,
						   &psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAUSEMemInfo,
						   psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAUSEMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_48)
		bReleaseFailed = IMG_TRUE;
	}
	eError = PVRSRVLookupAndReleaseHandle(psPerProc->psHandleBase,
						   &psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAParamMemInfo,
						   psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAParamMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_49)
		bReleaseFailed = IMG_TRUE;
	}
	eError = PVRSRVLookupAndReleaseHandle(psPerProc->psHandleBase,
						   &psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAPMPTMemInfo,
						   psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAPMPTMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_50)
		bReleaseFailed = IMG_TRUE;
	}
	eError = PVRSRVLookupAndReleaseHandle(psPerProc->psHandleBase,
						   &psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWADPMLSSMemInfo,
						   psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWADPMLSSMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_51)
		bReleaseFailed = IMG_TRUE;
	}
	eError = PVRSRVLookupAndReleaseHandle(psPerProc->psHandleBase,
						   &psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWATPCMemInfo,
						   psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWATPCMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_52)
		bReleaseFailed = IMG_TRUE;
	}
	eError = PVRSRVLookupAndReleaseHandle(psPerProc->psHandleBase,
						   &psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAPSGRgnHdrMemInfo,
						   psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAPSGRgnHdrMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_53)
		bReleaseFailed = IMG_TRUE;
	}
	eError = PVRSRVLookupAndReleaseHandle(psPerProc->psHandleBase,
						   &psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAPartiPIMMemInfo,
						   psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAPartiPIMMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_54)
		bReleaseFailed = IMG_TRUE;
	}
#endif

#if defined(PVRSRV_USSE_EDM_STATUS_DEBUG)
	eError = PVRSRVLookupAndReleaseHandle(psPerProc->psHandleBase,
						   &psSGXDevInitPart2IN->sInitInfo.hKernelEDMStatusBufferMemInfo,
						   psSGXDevInitPart2IN->sInitInfo.hKernelEDMStatusBufferMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_55)
		bReleaseFailed = IMG_TRUE;
	}
#endif

#if defined(SGX_FEATURE_SPM_MODE_0)
	eError = PVRSRVLookupAndReleaseHandle(psPerProc->psHandleBase,
						   &psSGXDevInitPart2IN->sInitInfo.hKernelTmpDPMStateMemInfo,
						   psSGXDevInitPart2IN->sInitInfo.hKernelTmpDPMStateMemInfo,
						   PVRSRV_HANDLE_TYPE_MEM_INFO);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_56)
		bReleaseFailed = IMG_TRUE;
	}
#endif


	for (i = 0; i < SGX_MAX_INIT_MEM_HANDLES; i++)
	{
		IMG_HANDLE *phHandle = &psSGXDevInitPart2IN->sInitInfo.asInitMemHandles[i];

		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_57)
		if (*phHandle == IMG_NULL)
			continue;

		eError = PVRSRVLookupAndReleaseHandle(psPerProc->psHandleBase,
							   phHandle,
							   *phHandle,
							   PVRSRV_HANDLE_TYPE_MEM_INFO);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_58)
			bReleaseFailed = IMG_TRUE;
		}
	}

	if (bReleaseFailed)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_59)
		PVR_DPF((PVR_DBG_ERROR, "DevInitSGXPart2BW: A handle release failed"));
		psRetOUT->eError = PVRSRV_ERROR_INIT2_PHASE_FAILED;
		
		PVR_DBG_BREAK;
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_60)
		return 0;
	}

	
	eError = PVRSRVDissociateDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelCCBMemInfo);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_61)
		bDissociateFailed = IMG_TRUE;
	}

	eError = PVRSRVDissociateDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelCCBCtlMemInfo);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_62)
		bDissociateFailed = IMG_TRUE;
	}

	eError = PVRSRVDissociateDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelCCBEventKickerMemInfo);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_63)
		bDissociateFailed = IMG_TRUE;
	}

	eError = PVRSRVDissociateDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelSGXHostCtlMemInfo);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_64)
		bDissociateFailed = IMG_TRUE;
	}

	eError = PVRSRVDissociateDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelSGXTA3DCtlMemInfo);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_65)
		bDissociateFailed = IMG_TRUE;
	}

	
	eError = PVRSRVDissociateDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelSGXMiscMemInfo);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_66)
		bDissociateFailed = IMG_TRUE;
	}


#if defined(SGX_SUPPORT_HWPROFILING)
	eError = PVRSRVDissociateDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelHWProfilingMemInfo);
	bDissociateFailed |= (IMG_BOOL)(eError != PVRSRV_OK);
#endif

#if defined(SUPPORT_SGX_HWPERF)
	eError = PVRSRVDissociateDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelHWPerfCBMemInfo);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_67)
		bDissociateFailed = IMG_TRUE;
	}
#endif

	eError = PVRSRVDissociateDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelTASigBufferMemInfo);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_68)
		bDissociateFailed = IMG_TRUE;
	}

	eError = PVRSRVDissociateDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernel3DSigBufferMemInfo);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_69)
		bDissociateFailed = IMG_TRUE;
	}

#if defined(FIX_HW_BRN_29702)
	eError = PVRSRVDissociateDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelCFIMemInfo);
	bDissociateFailed |= (IMG_BOOL)(eError != PVRSRV_OK);
#endif

#if defined(FIX_HW_BRN_29823)
	eError = PVRSRVDissociateDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelDummyTermStreamMemInfo);
	bDissociateFailed |= (IMG_BOOL)(eError != PVRSRV_OK);
#endif

#if defined(FIX_HW_BRN_31542)
	eError = PVRSRVDissociateDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAVDMStreamMemInfo);
	bDissociateFailed |= (IMG_BOOL)(eError != PVRSRV_OK);
	eError = PVRSRVDissociateDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAIndexStreamMemInfo);
	bDissociateFailed |= (IMG_BOOL)(eError != PVRSRV_OK);
	eError = PVRSRVDissociateDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAPDSMemInfo);
	bDissociateFailed |= (IMG_BOOL)(eError != PVRSRV_OK);
	eError = PVRSRVDissociateDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAUSEMemInfo);
	bDissociateFailed |= (IMG_BOOL)(eError != PVRSRV_OK);
	eError = PVRSRVDissociateDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAParamMemInfo);
	bDissociateFailed |= (IMG_BOOL)(eError != PVRSRV_OK);
	eError = PVRSRVDissociateDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAPMPTMemInfo);
	bDissociateFailed |= (IMG_BOOL)(eError != PVRSRV_OK);
	eError = PVRSRVDissociateDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWADPMLSSMemInfo);
	bDissociateFailed |= (IMG_BOOL)(eError != PVRSRV_OK);
	eError = PVRSRVDissociateDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWATPCMemInfo);
	bDissociateFailed |= (IMG_BOOL)(eError != PVRSRV_OK);
	eError = PVRSRVDissociateDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAPSGRgnHdrMemInfo);
	bDissociateFailed |= (IMG_BOOL)(eError != PVRSRV_OK);
	eError = PVRSRVDissociateDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelClearClipWAPartiPIMMemInfo);
	bDissociateFailed |= (IMG_BOOL)(eError != PVRSRV_OK);
#endif

#if defined(PVRSRV_USSE_EDM_STATUS_DEBUG)
	eError = PVRSRVDissociateDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelEDMStatusBufferMemInfo);
	bDissociateFailed |= (IMG_BOOL)(eError != PVRSRV_OK);
#endif

#if defined(SGX_FEATURE_SPM_MODE_0)
	eError = PVRSRVDissociateDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelTmpDPMStateMemInfo);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_70)
		bDissociateFailed = IMG_TRUE;
	}
#endif

	for (i = 0; i < SGX_MAX_INIT_MEM_HANDLES; i++)
	{
		IMG_HANDLE hHandle = psSGXDevInitPart2IN->sInitInfo.asInitMemHandles[i];

		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_71)
		if (hHandle == IMG_NULL)
			continue;

		eError = PVRSRVDissociateDeviceMemKM(hDevCookieInt, hHandle);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_72)
			bDissociateFailed = IMG_TRUE;
		}
	}

	
	if(bDissociateFailed)
	{
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_73)
		PVRSRVFreeDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelCCBMemInfo);
		PVRSRVFreeDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelCCBCtlMemInfo);
		PVRSRVFreeDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelSGXHostCtlMemInfo);
		PVRSRVFreeDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelSGXTA3DCtlMemInfo);
		PVRSRVFreeDeviceMemKM(hDevCookieInt, psSGXDevInitPart2IN->sInitInfo.hKernelSGXMiscMemInfo);

		for (i = 0; i < SGX_MAX_INIT_MEM_HANDLES; i++)
		{
			IMG_HANDLE hHandle = psSGXDevInitPart2IN->sInitInfo.asInitMemHandles[i];

			__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_74)
			if (hHandle == IMG_NULL)
				continue;

			PVRSRVFreeDeviceMemKM(hDevCookieInt, (PVRSRV_KERNEL_MEM_INFO *)hHandle);

		}

		PVR_DPF((PVR_DBG_ERROR, "DevInitSGXPart2BW: A dissociate failed"));

		psRetOUT->eError = PVRSRV_ERROR_INIT2_PHASE_FAILED;

		
		PVR_DBG_BREAK;
		__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_75)
		return 0;
	}

	psRetOUT->eError =
		DevInitSGXPart2KM(psPerProc,
						  hDevCookieInt,
						  &psSGXDevInitPart2IN->sInitInfo);

	__DtTestPoint(__DtFunc_SGXDevInitPart2BW, __DtStep_76)
	return 0;
}


static IMG_INT
SGXRegisterHWRenderContextBW(IMG_UINT32 ui32BridgeID,
							 PVRSRV_BRIDGE_IN_SGX_REGISTER_HW_RENDER_CONTEXT *psSGXRegHWRenderContextIN,
							 PVRSRV_BRIDGE_OUT_SGX_REGISTER_HW_RENDER_CONTEXT *psSGXRegHWRenderContextOUT,
							 PVRSRV_PER_PROCESS_DATA *psPerProc)
{
	IMG_HANDLE hDevCookieInt;
	IMG_HANDLE hHWRenderContextInt;

	__DtTestPoint(__DtFunc_SGXRegisterHWRenderContextBW, __DtStep_0)
	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGX_REGISTER_HW_RENDER_CONTEXT);

	NEW_HANDLE_BATCH_OR_ERROR(psSGXRegHWRenderContextOUT->eError, psPerProc, 1);

	psSGXRegHWRenderContextOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDevCookieInt,
						   psSGXRegHWRenderContextIN->hDevCookie,
						   PVRSRV_HANDLE_TYPE_DEV_NODE);
	if(psSGXRegHWRenderContextOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXRegisterHWRenderContextBW, __DtStep_1)
		return 0;
	}

	hHWRenderContextInt =
		SGXRegisterHWRenderContextKM(hDevCookieInt,
									 &psSGXRegHWRenderContextIN->sHWRenderContextDevVAddr,
									 psPerProc);

	if (hHWRenderContextInt == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_SGXRegisterHWRenderContextBW, __DtStep_2)
		psSGXRegHWRenderContextOUT->eError = PVRSRV_ERROR_UNABLE_TO_REGISTER_CONTEXT;
		__DtTestPoint(__DtFunc_SGXRegisterHWRenderContextBW, __DtStep_3)
		return 0;
	}

	PVRSRVAllocHandleNR(psPerProc->psHandleBase,
					  &psSGXRegHWRenderContextOUT->hHWRenderContext,
					  hHWRenderContextInt,
					  PVRSRV_HANDLE_TYPE_SGX_HW_RENDER_CONTEXT,
					  PVRSRV_HANDLE_ALLOC_FLAG_NONE);

	COMMIT_HANDLE_BATCH_OR_ERROR(psSGXRegHWRenderContextOUT->eError, psPerProc);

	__DtTestPoint(__DtFunc_SGXRegisterHWRenderContextBW, __DtStep_4)
	return 0;
}


static IMG_INT
SGXUnregisterHWRenderContextBW(IMG_UINT32 ui32BridgeID,
							   PVRSRV_BRIDGE_IN_SGX_UNREGISTER_HW_RENDER_CONTEXT *psSGXUnregHWRenderContextIN,
							   PVRSRV_BRIDGE_RETURN *psRetOUT,
							   PVRSRV_PER_PROCESS_DATA *psPerProc)
{
	IMG_HANDLE hHWRenderContextInt;

	__DtTestPoint(__DtFunc_SGXUnregisterHWRenderContextBW, __DtStep_0)
	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGX_UNREGISTER_HW_RENDER_CONTEXT);

	psRetOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hHWRenderContextInt,
						   psSGXUnregHWRenderContextIN->hHWRenderContext,
						   PVRSRV_HANDLE_TYPE_SGX_HW_RENDER_CONTEXT);
	if(psRetOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXUnregisterHWRenderContextBW, __DtStep_1)
		return 0;
	}

	psRetOUT->eError = SGXUnregisterHWRenderContextKM(hHWRenderContextInt);
	if(psRetOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXUnregisterHWRenderContextBW, __DtStep_2)
		return 0;
	}

	psRetOUT->eError =
		PVRSRVReleaseHandle(psPerProc->psHandleBase,
							psSGXUnregHWRenderContextIN->hHWRenderContext,
							PVRSRV_HANDLE_TYPE_SGX_HW_RENDER_CONTEXT);

	__DtTestPoint(__DtFunc_SGXUnregisterHWRenderContextBW, __DtStep_3)
	return 0;
}


static IMG_INT
SGXRegisterHWTransferContextBW(IMG_UINT32 ui32BridgeID,
							 PVRSRV_BRIDGE_IN_SGX_REGISTER_HW_TRANSFER_CONTEXT *psSGXRegHWTransferContextIN,
							 PVRSRV_BRIDGE_OUT_SGX_REGISTER_HW_TRANSFER_CONTEXT *psSGXRegHWTransferContextOUT,
							 PVRSRV_PER_PROCESS_DATA *psPerProc)
{
	IMG_HANDLE hDevCookieInt;
	IMG_HANDLE hHWTransferContextInt;

	__DtTestPoint(__DtFunc_SGXRegisterHWTransferContextBW, __DtStep_0)
	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGX_REGISTER_HW_TRANSFER_CONTEXT);

	NEW_HANDLE_BATCH_OR_ERROR(psSGXRegHWTransferContextOUT->eError, psPerProc, 1);

	psSGXRegHWTransferContextOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDevCookieInt,
						   psSGXRegHWTransferContextIN->hDevCookie,
						   PVRSRV_HANDLE_TYPE_DEV_NODE);
	if(psSGXRegHWTransferContextOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXRegisterHWTransferContextBW, __DtStep_1)
		return 0;
	}

	hHWTransferContextInt =
		SGXRegisterHWTransferContextKM(hDevCookieInt,
									   &psSGXRegHWTransferContextIN->sHWTransferContextDevVAddr,
									   psPerProc);

	if (hHWTransferContextInt == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_SGXRegisterHWTransferContextBW, __DtStep_2)
		psSGXRegHWTransferContextOUT->eError = PVRSRV_ERROR_UNABLE_TO_REGISTER_CONTEXT;
		__DtTestPoint(__DtFunc_SGXRegisterHWTransferContextBW, __DtStep_3)
		return 0;
	}

	PVRSRVAllocHandleNR(psPerProc->psHandleBase,
					  &psSGXRegHWTransferContextOUT->hHWTransferContext,
					  hHWTransferContextInt,
					  PVRSRV_HANDLE_TYPE_SGX_HW_TRANSFER_CONTEXT,
					  PVRSRV_HANDLE_ALLOC_FLAG_NONE);

	COMMIT_HANDLE_BATCH_OR_ERROR(psSGXRegHWTransferContextOUT->eError, psPerProc);

	__DtTestPoint(__DtFunc_SGXRegisterHWTransferContextBW, __DtStep_4)
	return 0;
}


static IMG_INT
SGXUnregisterHWTransferContextBW(IMG_UINT32 ui32BridgeID,
							   PVRSRV_BRIDGE_IN_SGX_UNREGISTER_HW_TRANSFER_CONTEXT *psSGXUnregHWTransferContextIN,
							   PVRSRV_BRIDGE_RETURN *psRetOUT,
							   PVRSRV_PER_PROCESS_DATA *psPerProc)
{
	IMG_HANDLE hHWTransferContextInt;

	__DtTestPoint(__DtFunc_SGXUnregisterHWTransferContextBW, __DtStep_0)
	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGX_UNREGISTER_HW_TRANSFER_CONTEXT);

	psRetOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hHWTransferContextInt,
						   psSGXUnregHWTransferContextIN->hHWTransferContext,
						   PVRSRV_HANDLE_TYPE_SGX_HW_TRANSFER_CONTEXT);
	if(psRetOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXUnregisterHWTransferContextBW, __DtStep_1)
		return 0;
	}

	psRetOUT->eError = SGXUnregisterHWTransferContextKM(hHWTransferContextInt);
	if(psRetOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXUnregisterHWTransferContextBW, __DtStep_2)
		return 0;
	}

	psRetOUT->eError =
		PVRSRVReleaseHandle(psPerProc->psHandleBase,
							psSGXUnregHWTransferContextIN->hHWTransferContext,
							PVRSRV_HANDLE_TYPE_SGX_HW_TRANSFER_CONTEXT);

	__DtTestPoint(__DtFunc_SGXUnregisterHWTransferContextBW, __DtStep_3)
	return 0;
}


#if defined(SGX_FEATURE_2D_HARDWARE)
static IMG_INT
SGXRegisterHW2DContextBW(IMG_UINT32 ui32BridgeID,
							 PVRSRV_BRIDGE_IN_SGX_REGISTER_HW_2D_CONTEXT *psSGXRegHW2DContextIN,
							 PVRSRV_BRIDGE_OUT_SGX_REGISTER_HW_2D_CONTEXT *psSGXRegHW2DContextOUT,
							 PVRSRV_PER_PROCESS_DATA *psPerProc)
{
	IMG_HANDLE hDevCookieInt;
	IMG_HANDLE hHW2DContextInt;

	__DtTestPoint(__DtFunc_SGXRegisterHW2DContextBW, __DtStep_0)
	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGX_REGISTER_HW_2D_CONTEXT);

	NEW_HANDLE_BATCH_OR_ERROR(psSGXRegHW2DContextOUT->eError, psPerProc, 1);

	psSGXRegHW2DContextOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDevCookieInt,
						   psSGXRegHW2DContextIN->hDevCookie,
						   PVRSRV_HANDLE_TYPE_DEV_NODE);
	if(psSGXRegHW2DContextOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXRegisterHW2DContextBW, __DtStep_1)
		return 0;
	}

	hHW2DContextInt =
		SGXRegisterHW2DContextKM(hDevCookieInt,
								 &psSGXRegHW2DContextIN->sHW2DContextDevVAddr,
								 psPerProc);

	if (hHW2DContextInt == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_SGXRegisterHW2DContextBW, __DtStep_2)
		psSGXRegHW2DContextOUT->eError = PVRSRV_ERROR_UNABLE_TO_REGISTER_CONTEXT;
		__DtTestPoint(__DtFunc_SGXRegisterHW2DContextBW, __DtStep_3)
		return 0;
	}

	PVRSRVAllocHandleNR(psPerProc->psHandleBase,
					  &psSGXRegHW2DContextOUT->hHW2DContext,
					  hHW2DContextInt,
					  PVRSRV_HANDLE_TYPE_SGX_HW_2D_CONTEXT,
					  PVRSRV_HANDLE_ALLOC_FLAG_NONE);

	COMMIT_HANDLE_BATCH_OR_ERROR(psSGXRegHW2DContextOUT->eError, psPerProc);

	__DtTestPoint(__DtFunc_SGXRegisterHW2DContextBW, __DtStep_4)
	return 0;
}


static IMG_INT
SGXUnregisterHW2DContextBW(IMG_UINT32 ui32BridgeID,
							   PVRSRV_BRIDGE_IN_SGX_UNREGISTER_HW_2D_CONTEXT *psSGXUnregHW2DContextIN,
							   PVRSRV_BRIDGE_RETURN *psRetOUT,
							   PVRSRV_PER_PROCESS_DATA *psPerProc)
{
	IMG_HANDLE hHW2DContextInt;

	__DtTestPoint(__DtFunc_SGXUnregisterHW2DContextBW, __DtStep_0)
	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGX_UNREGISTER_HW_2D_CONTEXT);

	psRetOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hHW2DContextInt,
						   psSGXUnregHW2DContextIN->hHW2DContext,
						   PVRSRV_HANDLE_TYPE_SGX_HW_2D_CONTEXT);
	if(psRetOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXUnregisterHW2DContextBW, __DtStep_1)
		return 0;
	}

	psRetOUT->eError = SGXUnregisterHW2DContextKM(hHW2DContextInt);
	if(psRetOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXUnregisterHW2DContextBW, __DtStep_2)
		return 0;
	}

	psRetOUT->eError =
		PVRSRVReleaseHandle(psPerProc->psHandleBase,
							psSGXUnregHW2DContextIN->hHW2DContext,
							PVRSRV_HANDLE_TYPE_SGX_HW_2D_CONTEXT);

	__DtTestPoint(__DtFunc_SGXUnregisterHW2DContextBW, __DtStep_3)
	return 0;
}
#endif 

static IMG_INT
SGXFlushHWRenderTargetBW(IMG_UINT32 ui32BridgeID,
						  PVRSRV_BRIDGE_IN_SGX_FLUSH_HW_RENDER_TARGET *psSGXFlushHWRenderTargetIN,
						  PVRSRV_BRIDGE_RETURN *psRetOUT,
						  PVRSRV_PER_PROCESS_DATA *psPerProc)
{
	IMG_HANDLE hDevCookieInt;
	__DtTestPoint(__DtFunc_SGXFlushHWRenderTargetBW, __DtStep_0)
	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGX_FLUSH_HW_RENDER_TARGET);

	psRetOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDevCookieInt,
						   psSGXFlushHWRenderTargetIN->hDevCookie,
						   PVRSRV_HANDLE_TYPE_DEV_NODE);
	if(psRetOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXFlushHWRenderTargetBW, __DtStep_1)
		return 0;
	}

	SGXFlushHWRenderTargetKM(hDevCookieInt, psSGXFlushHWRenderTargetIN->sHWRTDataSetDevVAddr);

	__DtTestPoint(__DtFunc_SGXFlushHWRenderTargetBW, __DtStep_2)
	return 0;
}


static IMG_INT
SGX2DQueryBlitsCompleteBW(IMG_UINT32 ui32BridgeID,
						  PVRSRV_BRIDGE_IN_2DQUERYBLTSCOMPLETE *ps2DQueryBltsCompleteIN,
						  PVRSRV_BRIDGE_RETURN *psRetOUT,
						  PVRSRV_PER_PROCESS_DATA *psPerProc)
{
	IMG_HANDLE hDevCookieInt;
	IMG_VOID *pvSyncInfo;
	PVRSRV_SGXDEV_INFO *psDevInfo;

	__DtTestPoint(__DtFunc_SGX2DQueryBlitsCompleteBW, __DtStep_0)
	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGX_2DQUERYBLTSCOMPLETE);

	psRetOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase, &hDevCookieInt,
						   ps2DQueryBltsCompleteIN->hDevCookie,
						   PVRSRV_HANDLE_TYPE_DEV_NODE);
	if(psRetOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGX2DQueryBlitsCompleteBW, __DtStep_1)
		return 0;
	}

	psRetOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase, &pvSyncInfo,
						   ps2DQueryBltsCompleteIN->hKernSyncInfo,
						   PVRSRV_HANDLE_TYPE_SYNC_INFO);
	if(psRetOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGX2DQueryBlitsCompleteBW, __DtStep_2)
		return 0;
	}

	psDevInfo = (PVRSRV_SGXDEV_INFO *)((PVRSRV_DEVICE_NODE *)hDevCookieInt)->pvDevice;

	psRetOUT->eError =
		SGX2DQueryBlitsCompleteKM(psDevInfo,
								  (PVRSRV_KERNEL_SYNC_INFO *)pvSyncInfo,
								  ps2DQueryBltsCompleteIN->bWaitForComplete);

	__DtTestPoint(__DtFunc_SGX2DQueryBlitsCompleteBW, __DtStep_3)
	return 0;
}


static IMG_INT
SGXFindSharedPBDescBW(IMG_UINT32 ui32BridgeID,
					  PVRSRV_BRIDGE_IN_SGXFINDSHAREDPBDESC *psSGXFindSharedPBDescIN,
					  PVRSRV_BRIDGE_OUT_SGXFINDSHAREDPBDESC *psSGXFindSharedPBDescOUT,
					  PVRSRV_PER_PROCESS_DATA *psPerProc)
{
	IMG_HANDLE hDevCookieInt;
	PVRSRV_KERNEL_MEM_INFO *psSharedPBDescKernelMemInfo;
	PVRSRV_KERNEL_MEM_INFO *psHWPBDescKernelMemInfo;
	PVRSRV_KERNEL_MEM_INFO *psBlockKernelMemInfo;
	PVRSRV_KERNEL_MEM_INFO *psHWBlockKernelMemInfo;
	PVRSRV_KERNEL_MEM_INFO **ppsSharedPBDescSubKernelMemInfos = IMG_NULL;
	IMG_UINT32 ui32SharedPBDescSubKernelMemInfosCount = 0;
	IMG_UINT32 i;
	IMG_HANDLE hSharedPBDesc = IMG_NULL;

	__DtTestPoint(__DtFunc_SGXFindSharedPBDescBW, __DtStep_0)
	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGX_FINDSHAREDPBDESC);

	NEW_HANDLE_BATCH_OR_ERROR(psSGXFindSharedPBDescOUT->eError, psPerProc, PVRSRV_BRIDGE_SGX_SHAREDPBDESC_MAX_SUBMEMINFOS + 4);

	psSGXFindSharedPBDescOUT->hSharedPBDesc = IMG_NULL;

	psSGXFindSharedPBDescOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hDevCookieInt,
						   psSGXFindSharedPBDescIN->hDevCookie,
						   PVRSRV_HANDLE_TYPE_DEV_NODE);
	if(psSGXFindSharedPBDescOUT->eError != PVRSRV_OK)
		goto PVRSRV_BRIDGE_SGX_FINDSHAREDPBDESC_EXIT;

	psSGXFindSharedPBDescOUT->eError =
		SGXFindSharedPBDescKM(psPerProc, hDevCookieInt,
							  psSGXFindSharedPBDescIN->bLockOnFailure,
							  psSGXFindSharedPBDescIN->ui32TotalPBSize,
							  &hSharedPBDesc,
							  &psSharedPBDescKernelMemInfo,
							  &psHWPBDescKernelMemInfo,
							  &psBlockKernelMemInfo,
							  &psHWBlockKernelMemInfo,
							  &ppsSharedPBDescSubKernelMemInfos,
							  &ui32SharedPBDescSubKernelMemInfosCount);
	if(psSGXFindSharedPBDescOUT->eError != PVRSRV_OK)
		goto PVRSRV_BRIDGE_SGX_FINDSHAREDPBDESC_EXIT;

	PVR_ASSERT(ui32SharedPBDescSubKernelMemInfosCount
			   <= PVRSRV_BRIDGE_SGX_SHAREDPBDESC_MAX_SUBMEMINFOS);

	psSGXFindSharedPBDescOUT->ui32SharedPBDescSubKernelMemInfoHandlesCount =
		ui32SharedPBDescSubKernelMemInfosCount;

	if(hSharedPBDesc == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_SGXFindSharedPBDescBW, __DtStep_1)
		psSGXFindSharedPBDescOUT->hSharedPBDescKernelMemInfoHandle = 0;
		
		goto PVRSRV_BRIDGE_SGX_FINDSHAREDPBDESC_EXIT;
	}

	PVRSRVAllocHandleNR(psPerProc->psHandleBase,
					  &psSGXFindSharedPBDescOUT->hSharedPBDesc,
					  hSharedPBDesc,
					  PVRSRV_HANDLE_TYPE_SHARED_PB_DESC,
					  PVRSRV_HANDLE_ALLOC_FLAG_NONE);

	
	PVRSRVAllocSubHandleNR(psPerProc->psHandleBase,
					  &psSGXFindSharedPBDescOUT->hSharedPBDescKernelMemInfoHandle,
					  psSharedPBDescKernelMemInfo,
					  PVRSRV_HANDLE_TYPE_MEM_INFO_REF,
					  PVRSRV_HANDLE_ALLOC_FLAG_MULTI,
					  psSGXFindSharedPBDescOUT->hSharedPBDesc);

	PVRSRVAllocSubHandleNR(psPerProc->psHandleBase,
					  &psSGXFindSharedPBDescOUT->hHWPBDescKernelMemInfoHandle,
					  psHWPBDescKernelMemInfo,
					  PVRSRV_HANDLE_TYPE_MEM_INFO_REF,
					  PVRSRV_HANDLE_ALLOC_FLAG_MULTI,
					  psSGXFindSharedPBDescOUT->hSharedPBDesc);

	PVRSRVAllocSubHandleNR(psPerProc->psHandleBase,
				  &psSGXFindSharedPBDescOUT->hBlockKernelMemInfoHandle,
				  psBlockKernelMemInfo,
				  PVRSRV_HANDLE_TYPE_MEM_INFO_REF,
				  PVRSRV_HANDLE_ALLOC_FLAG_MULTI,
				  psSGXFindSharedPBDescOUT->hSharedPBDesc);

	PVRSRVAllocSubHandleNR(psPerProc->psHandleBase,
				  &psSGXFindSharedPBDescOUT->hHWBlockKernelMemInfoHandle,
				  psHWBlockKernelMemInfo,
				  PVRSRV_HANDLE_TYPE_MEM_INFO_REF,
				  PVRSRV_HANDLE_ALLOC_FLAG_MULTI,
				  psSGXFindSharedPBDescOUT->hSharedPBDesc);


	for(i=0; i<ui32SharedPBDescSubKernelMemInfosCount; i++)
	{
		PVRSRV_BRIDGE_OUT_SGXFINDSHAREDPBDESC *psSGXFindSharedPBDescOut =
			psSGXFindSharedPBDescOUT;

			__DtTestPoint(__DtFunc_SGXFindSharedPBDescBW, __DtStep_2)
			PVRSRVAllocSubHandleNR(psPerProc->psHandleBase,
							  &psSGXFindSharedPBDescOut->ahSharedPBDescSubKernelMemInfoHandles[i],
							  ppsSharedPBDescSubKernelMemInfos[i],
							  PVRSRV_HANDLE_TYPE_MEM_INFO_REF,
							  PVRSRV_HANDLE_ALLOC_FLAG_MULTI,
							  psSGXFindSharedPBDescOUT->hSharedPBDescKernelMemInfoHandle);
	}

PVRSRV_BRIDGE_SGX_FINDSHAREDPBDESC_EXIT:
	if (ppsSharedPBDescSubKernelMemInfos != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_SGXFindSharedPBDescBW, __DtStep_3)
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP,
				  sizeof(PVRSRV_KERNEL_MEM_INFO *) * ui32SharedPBDescSubKernelMemInfosCount,
				  ppsSharedPBDescSubKernelMemInfos,
				  IMG_NULL);
	}

	if(psSGXFindSharedPBDescOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXFindSharedPBDescBW, __DtStep_4)
		if(hSharedPBDesc != IMG_NULL)
		{
			__DtTestPoint(__DtFunc_SGXFindSharedPBDescBW, __DtStep_5)
			SGXUnrefSharedPBDescKM(hSharedPBDesc);
		}
	}
	else
	{
		__DtTestPoint(__DtFunc_SGXFindSharedPBDescBW, __DtStep_6)
		COMMIT_HANDLE_BATCH_OR_ERROR(psSGXFindSharedPBDescOUT->eError, psPerProc);
	}

	__DtTestPoint(__DtFunc_SGXFindSharedPBDescBW, __DtStep_7)
	return 0;
}


static IMG_INT
SGXUnrefSharedPBDescBW(IMG_UINT32 ui32BridgeID,
					   PVRSRV_BRIDGE_IN_SGXUNREFSHAREDPBDESC *psSGXUnrefSharedPBDescIN,
					   PVRSRV_BRIDGE_OUT_SGXUNREFSHAREDPBDESC *psSGXUnrefSharedPBDescOUT,
					   PVRSRV_PER_PROCESS_DATA *psPerProc)
{
	IMG_HANDLE hSharedPBDesc;

	__DtTestPoint(__DtFunc_SGXUnrefSharedPBDescBW, __DtStep_0)
	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGX_UNREFSHAREDPBDESC);

	psSGXUnrefSharedPBDescOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase,
						   &hSharedPBDesc,
						   psSGXUnrefSharedPBDescIN->hSharedPBDesc,
						   PVRSRV_HANDLE_TYPE_SHARED_PB_DESC);
	if(psSGXUnrefSharedPBDescOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXUnrefSharedPBDescBW, __DtStep_1)
		return 0;
	}

	psSGXUnrefSharedPBDescOUT->eError =
		SGXUnrefSharedPBDescKM(hSharedPBDesc);

	if(psSGXUnrefSharedPBDescOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXUnrefSharedPBDescBW, __DtStep_2)
		return 0;
	}

	psSGXUnrefSharedPBDescOUT->eError =
		PVRSRVReleaseHandle(psPerProc->psHandleBase,
						   psSGXUnrefSharedPBDescIN->hSharedPBDesc,
						   PVRSRV_HANDLE_TYPE_SHARED_PB_DESC);

	__DtTestPoint(__DtFunc_SGXUnrefSharedPBDescBW, __DtStep_3)
	return 0;
}


static IMG_INT
SGXAddSharedPBDescBW(IMG_UINT32 ui32BridgeID,
					 PVRSRV_BRIDGE_IN_SGXADDSHAREDPBDESC *psSGXAddSharedPBDescIN,
					 PVRSRV_BRIDGE_OUT_SGXADDSHAREDPBDESC *psSGXAddSharedPBDescOUT,
					 PVRSRV_PER_PROCESS_DATA *psPerProc)
{
	IMG_HANDLE hDevCookieInt;
	PVRSRV_KERNEL_MEM_INFO *psSharedPBDescKernelMemInfo;
	PVRSRV_KERNEL_MEM_INFO *psHWPBDescKernelMemInfo;
	PVRSRV_KERNEL_MEM_INFO *psBlockKernelMemInfo;
	PVRSRV_KERNEL_MEM_INFO *psHWBlockKernelMemInfo;
	IMG_UINT32 ui32KernelMemInfoHandlesCount =
		psSGXAddSharedPBDescIN->ui32KernelMemInfoHandlesCount;
	IMG_INT ret = 0;
	IMG_HANDLE *phKernelMemInfoHandles = IMG_NULL;
	PVRSRV_KERNEL_MEM_INFO **ppsKernelMemInfos = IMG_NULL;
	IMG_UINT32 i;
	PVRSRV_ERROR eError;
	IMG_HANDLE hSharedPBDesc = IMG_NULL;

	__DtTestPoint(__DtFunc_SGXAddSharedPBDescBW, __DtStep_0)
	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGX_ADDSHAREDPBDESC);

	NEW_HANDLE_BATCH_OR_ERROR(psSGXAddSharedPBDescOUT->eError, psPerProc, 1);

	psSGXAddSharedPBDescOUT->hSharedPBDesc = IMG_NULL;

	PVR_ASSERT(ui32KernelMemInfoHandlesCount
			   <= PVRSRV_BRIDGE_SGX_SHAREDPBDESC_MAX_SUBMEMINFOS);

	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
								&hDevCookieInt,
								psSGXAddSharedPBDescIN->hDevCookie,
								PVRSRV_HANDLE_TYPE_DEV_NODE);
	if(eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXAddSharedPBDescBW, __DtStep_1)
		goto PVRSRV_BRIDGE_SGX_ADDSHAREDPBDESC_RETURN_RESULT;
	}

	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
								(IMG_VOID **)&psSharedPBDescKernelMemInfo,
								psSGXAddSharedPBDescIN->hSharedPBDescKernelMemInfo,
								PVRSRV_HANDLE_TYPE_SHARED_SYS_MEM_INFO);
	if(eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXAddSharedPBDescBW, __DtStep_2)
		goto PVRSRV_BRIDGE_SGX_ADDSHAREDPBDESC_RETURN_RESULT;
	}

	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
								(IMG_VOID **)&psHWPBDescKernelMemInfo,
								psSGXAddSharedPBDescIN->hHWPBDescKernelMemInfo,
								PVRSRV_HANDLE_TYPE_MEM_INFO);
	if(eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXAddSharedPBDescBW, __DtStep_3)
		goto PVRSRV_BRIDGE_SGX_ADDSHAREDPBDESC_RETURN_RESULT;
	}

	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
								(IMG_VOID **)&psBlockKernelMemInfo,
								psSGXAddSharedPBDescIN->hBlockKernelMemInfo,
								PVRSRV_HANDLE_TYPE_SHARED_SYS_MEM_INFO);
	if(eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXAddSharedPBDescBW, __DtStep_4)
		goto PVRSRV_BRIDGE_SGX_ADDSHAREDPBDESC_RETURN_RESULT;
	}

	eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
								(IMG_VOID **)&psHWBlockKernelMemInfo,
								psSGXAddSharedPBDescIN->hHWBlockKernelMemInfo,
								PVRSRV_HANDLE_TYPE_MEM_INFO);
	if(eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXAddSharedPBDescBW, __DtStep_5)
		goto PVRSRV_BRIDGE_SGX_ADDSHAREDPBDESC_RETURN_RESULT;
	}


	if(!OSAccessOK(PVR_VERIFY_READ,
				   psSGXAddSharedPBDescIN->phKernelMemInfoHandles,
				   ui32KernelMemInfoHandlesCount * sizeof(IMG_HANDLE)))
	{
		__DtTestPoint(__DtFunc_SGXAddSharedPBDescBW, __DtStep_6)
		PVR_DPF((PVR_DBG_ERROR, "%s: PVRSRV_BRIDGE_SGX_ADDSHAREDPBDESC:"
				 " Invalid phKernelMemInfos pointer", __FUNCTION__));
		ret = -EFAULT;
		goto PVRSRV_BRIDGE_SGX_ADDSHAREDPBDESC_RETURN_RESULT;
	}

	eError = OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP,
				  ui32KernelMemInfoHandlesCount * sizeof(IMG_HANDLE),
				  (IMG_VOID **)&phKernelMemInfoHandles,
				  0,
				  "Array of Handles");
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXAddSharedPBDescBW, __DtStep_7)
		goto PVRSRV_BRIDGE_SGX_ADDSHAREDPBDESC_RETURN_RESULT;
	}

	if(CopyFromUserWrapper(psPerProc,
			               ui32BridgeID,
			               phKernelMemInfoHandles,
						   psSGXAddSharedPBDescIN->phKernelMemInfoHandles,
						   ui32KernelMemInfoHandlesCount * sizeof(IMG_HANDLE))
	   != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXAddSharedPBDescBW, __DtStep_8)
		ret = -EFAULT;
		goto PVRSRV_BRIDGE_SGX_ADDSHAREDPBDESC_RETURN_RESULT;
	}

	eError = OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP,
				  ui32KernelMemInfoHandlesCount * sizeof(PVRSRV_KERNEL_MEM_INFO *),
				  (IMG_VOID **)&ppsKernelMemInfos,
				  0,
				  "Array of pointers to Kernel Memory Info");
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXAddSharedPBDescBW, __DtStep_9)
		goto PVRSRV_BRIDGE_SGX_ADDSHAREDPBDESC_RETURN_RESULT;
	}

	for(i=0; i<ui32KernelMemInfoHandlesCount; i++)
	{
		__DtTestPoint(__DtFunc_SGXAddSharedPBDescBW, __DtStep_10)
		eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
									(IMG_VOID **)&ppsKernelMemInfos[i],
									phKernelMemInfoHandles[i],
									PVRSRV_HANDLE_TYPE_MEM_INFO);
		if(eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXAddSharedPBDescBW, __DtStep_11)
			goto PVRSRV_BRIDGE_SGX_ADDSHAREDPBDESC_RETURN_RESULT;
		}
	}

	
	 
	eError = PVRSRVReleaseHandle(psPerProc->psHandleBase,
								psSGXAddSharedPBDescIN->hSharedPBDescKernelMemInfo,
								PVRSRV_HANDLE_TYPE_SHARED_SYS_MEM_INFO);
	PVR_ASSERT(eError == PVRSRV_OK);

	 
	eError = PVRSRVReleaseHandle(psPerProc->psHandleBase,
								psSGXAddSharedPBDescIN->hHWPBDescKernelMemInfo,
								PVRSRV_HANDLE_TYPE_MEM_INFO);
	PVR_ASSERT(eError == PVRSRV_OK);

	 
	eError = PVRSRVReleaseHandle(psPerProc->psHandleBase,
								psSGXAddSharedPBDescIN->hBlockKernelMemInfo,
								PVRSRV_HANDLE_TYPE_SHARED_SYS_MEM_INFO);
	PVR_ASSERT(eError == PVRSRV_OK);

	 
	eError = PVRSRVReleaseHandle(psPerProc->psHandleBase,
								psSGXAddSharedPBDescIN->hHWBlockKernelMemInfo,
								PVRSRV_HANDLE_TYPE_MEM_INFO);
	PVR_ASSERT(eError == PVRSRV_OK);

	for(i=0; i<ui32KernelMemInfoHandlesCount; i++)
	{
		 
		__DtTestPoint(__DtFunc_SGXAddSharedPBDescBW, __DtStep_12)
		eError = PVRSRVReleaseHandle(psPerProc->psHandleBase,
									phKernelMemInfoHandles[i],
									PVRSRV_HANDLE_TYPE_MEM_INFO);
		PVR_ASSERT(eError == PVRSRV_OK);
	}

	eError = SGXAddSharedPBDescKM(psPerProc, hDevCookieInt,
								  psSharedPBDescKernelMemInfo,
								  psHWPBDescKernelMemInfo,
								  psBlockKernelMemInfo,
								  psHWBlockKernelMemInfo,
								  psSGXAddSharedPBDescIN->ui32TotalPBSize,
								  &hSharedPBDesc,
								  ppsKernelMemInfos,
								  ui32KernelMemInfoHandlesCount);


	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXAddSharedPBDescBW, __DtStep_13)
		goto PVRSRV_BRIDGE_SGX_ADDSHAREDPBDESC_RETURN_RESULT;
	}

	PVRSRVAllocHandleNR(psPerProc->psHandleBase,
				  &psSGXAddSharedPBDescOUT->hSharedPBDesc,
				  hSharedPBDesc,
				  PVRSRV_HANDLE_TYPE_SHARED_PB_DESC,
				  PVRSRV_HANDLE_ALLOC_FLAG_NONE);

PVRSRV_BRIDGE_SGX_ADDSHAREDPBDESC_RETURN_RESULT:

	if(phKernelMemInfoHandles)
	{
		__DtTestPoint(__DtFunc_SGXAddSharedPBDescBW, __DtStep_14)
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP,
				  psSGXAddSharedPBDescIN->ui32KernelMemInfoHandlesCount * sizeof(IMG_HANDLE),
				  (IMG_VOID *)phKernelMemInfoHandles,
				  0);
	}
	if(ppsKernelMemInfos)
	{
		__DtTestPoint(__DtFunc_SGXAddSharedPBDescBW, __DtStep_15)
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP,
				  psSGXAddSharedPBDescIN->ui32KernelMemInfoHandlesCount * sizeof(PVRSRV_KERNEL_MEM_INFO *),
				  (IMG_VOID *)ppsKernelMemInfos,
				  0);
	}

	if(ret == 0 && eError == PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXAddSharedPBDescBW, __DtStep_16)
		COMMIT_HANDLE_BATCH_OR_ERROR(psSGXAddSharedPBDescOUT->eError, psPerProc);
	}

	psSGXAddSharedPBDescOUT->eError = eError;

	__DtTestPoint(__DtFunc_SGXAddSharedPBDescBW, __DtStep_17)
	return ret;
}

static IMG_INT
SGXGetInfoForSrvinitBW(IMG_UINT32 ui32BridgeID,
					   PVRSRV_BRIDGE_IN_SGXINFO_FOR_SRVINIT *psSGXInfoForSrvinitIN,
					   PVRSRV_BRIDGE_OUT_SGXINFO_FOR_SRVINIT *psSGXInfoForSrvinitOUT,
					   PVRSRV_PER_PROCESS_DATA *psPerProc)
{
	IMG_HANDLE hDevCookieInt;
	IMG_UINT32 i;
	__DtTestPoint(__DtFunc_SGXGetInfoForSrvinitBW, __DtStep_0)
	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGXINFO_FOR_SRVINIT);

	NEW_HANDLE_BATCH_OR_ERROR(psSGXInfoForSrvinitOUT->eError, psPerProc, PVRSRV_MAX_CLIENT_HEAPS);

	if(!psPerProc->bInitProcess)
	{
		__DtTestPoint(__DtFunc_SGXGetInfoForSrvinitBW, __DtStep_1)
		psSGXInfoForSrvinitOUT->eError = PVRSRV_ERROR_PROCESS_NOT_INITIALISED;
		__DtTestPoint(__DtFunc_SGXGetInfoForSrvinitBW, __DtStep_2)
		return 0;
	}

	psSGXInfoForSrvinitOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase, &hDevCookieInt,
						   psSGXInfoForSrvinitIN->hDevCookie,
						   PVRSRV_HANDLE_TYPE_DEV_NODE);

	if(psSGXInfoForSrvinitOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXGetInfoForSrvinitBW, __DtStep_3)
		return 0;
	}

	psSGXInfoForSrvinitOUT->eError =
		SGXGetInfoForSrvinitKM(hDevCookieInt,
							   &psSGXInfoForSrvinitOUT->sInitInfo);

	if(psSGXInfoForSrvinitOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXGetInfoForSrvinitBW, __DtStep_4)
		return 0;
	}

	for(i = 0; i < PVRSRV_MAX_CLIENT_HEAPS; i++)
	{
		PVRSRV_HEAP_INFO *psHeapInfo;

		__DtTestPoint(__DtFunc_SGXGetInfoForSrvinitBW, __DtStep_5)
		psHeapInfo = &psSGXInfoForSrvinitOUT->sInitInfo.asHeapInfo[i];

		if (psHeapInfo->ui32HeapID != (IMG_UINT32)SGX_UNDEFINED_HEAP_ID)
		{
			IMG_HANDLE hDevMemHeapExt;

			__DtTestPoint(__DtFunc_SGXGetInfoForSrvinitBW, __DtStep_6)
			if (psHeapInfo->hDevMemHeap != IMG_NULL)
			{
				
				__DtTestPoint(__DtFunc_SGXGetInfoForSrvinitBW, __DtStep_7)
				PVRSRVAllocHandleNR(psPerProc->psHandleBase,
								  &hDevMemHeapExt,
								  psHeapInfo->hDevMemHeap,
								  PVRSRV_HANDLE_TYPE_DEV_MEM_HEAP,
								  PVRSRV_HANDLE_ALLOC_FLAG_SHARED);
				psHeapInfo->hDevMemHeap = hDevMemHeapExt;
			}
		}
	}

	COMMIT_HANDLE_BATCH_OR_ERROR(psSGXInfoForSrvinitOUT->eError, psPerProc);

	__DtTestPoint(__DtFunc_SGXGetInfoForSrvinitBW, __DtStep_8)
	return 0;
}

#if defined(PDUMP)
static IMG_VOID
DumpBufferArray(PVRSRV_PER_PROCESS_DATA *psPerProc,
				PSGX_KICKTA_DUMP_BUFFER	psBufferArray,
				IMG_UINT32						ui32BufferArrayLength,
				IMG_BOOL						bDumpPolls)
{
	IMG_UINT32	i;

	__DtTestPoint(__DtFunc_DumpBufferArray, __DtStep_0)
	for (i=0; i<ui32BufferArrayLength; i++)
	{
		PSGX_KICKTA_DUMP_BUFFER	psBuffer;
		PVRSRV_KERNEL_MEM_INFO 	*psCtrlMemInfoKM;
		IMG_CHAR * pszName;
		IMG_HANDLE hUniqueTag;
		IMG_UINT32	ui32Offset;

		__DtTestPoint(__DtFunc_DumpBufferArray, __DtStep_1)
		psBuffer = &psBufferArray[i];
		pszName = psBuffer->pszName;
		if (!pszName)
		{
			__DtTestPoint(__DtFunc_DumpBufferArray, __DtStep_2)
			pszName = "Nameless buffer";
		}

		hUniqueTag = MAKEUNIQUETAG((PVRSRV_KERNEL_MEM_INFO *)psBuffer->hKernelMemInfo);

	#if defined(SUPPORT_SGX_NEW_STATUS_VALS)
		psCtrlMemInfoKM	= ((PVRSRV_KERNEL_MEM_INFO *)psBuffer->hCtrlKernelMemInfo);
		ui32Offset =  psBuffer->sCtrlDevVAddr.uiAddr - psCtrlMemInfoKM->sDevVAddr.uiAddr;
	#else
		psCtrlMemInfoKM = ((PVRSRV_KERNEL_MEM_INFO *)psBuffer->hKernelMemInfo)->psKernelSyncInfo->psSyncDataMemInfoKM;
		ui32Offset = offsetof(PVRSRV_SYNC_DATA, ui32ReadOpsComplete);
	#endif

		if (psBuffer->ui32Start <= psBuffer->ui32End)
		{
			__DtTestPoint(__DtFunc_DumpBufferArray, __DtStep_3)
			if (bDumpPolls)
			{
				__DtTestPoint(__DtFunc_DumpBufferArray, __DtStep_4)
				PDUMPCOMMENTWITHFLAGS(0, "Wait for %s space\r\n", pszName);
				PDUMPCBP(psCtrlMemInfoKM,
						 ui32Offset,
						 psBuffer->ui32Start,
						 psBuffer->ui32SpaceUsed,
						 psBuffer->ui32BufferSize,
						 0,
						 MAKEUNIQUETAG(psCtrlMemInfoKM));
			}

			PDUMPCOMMENTWITHFLAGS(0, "%s\r\n", pszName);
			PDUMPMEMUM(psPerProc,
					 IMG_NULL,
					 psBuffer->pvLinAddr,
					 (PVRSRV_KERNEL_MEM_INFO*)psBuffer->hKernelMemInfo,
					 psBuffer->ui32Start,
					 psBuffer->ui32End - psBuffer->ui32Start,
					 0,
					 hUniqueTag);
		}
		else
		{
			

			__DtTestPoint(__DtFunc_DumpBufferArray, __DtStep_5)
			if (bDumpPolls)
			{
				__DtTestPoint(__DtFunc_DumpBufferArray, __DtStep_6)
				PDUMPCOMMENTWITHFLAGS(0, "Wait for %s space\r\n", pszName);
				PDUMPCBP(psCtrlMemInfoKM,
						 ui32Offset,
						 psBuffer->ui32Start,
						 psBuffer->ui32BackEndLength,
						 psBuffer->ui32BufferSize,
						 0,
						 MAKEUNIQUETAG(psCtrlMemInfoKM));
			}
			PDUMPCOMMENTWITHFLAGS(0, "%s (part 1)\r\n", pszName);
			PDUMPMEMUM(psPerProc,
					 IMG_NULL,
					 psBuffer->pvLinAddr,
					 (PVRSRV_KERNEL_MEM_INFO*)psBuffer->hKernelMemInfo,
					 psBuffer->ui32Start,
					 psBuffer->ui32BackEndLength,
					 0,
					 hUniqueTag);

			if (bDumpPolls)
			{
				__DtTestPoint(__DtFunc_DumpBufferArray, __DtStep_7)
				PDUMPMEMPOL(psCtrlMemInfoKM,
							ui32Offset,
							0,
							0xFFFFFFFF,
							PDUMP_POLL_OPERATOR_NOTEQUAL,
							0,
							MAKEUNIQUETAG(psCtrlMemInfoKM));

				PDUMPCOMMENTWITHFLAGS(0, "Wait for %s space\r\n", pszName);
				PDUMPCBP(psCtrlMemInfoKM,
						 ui32Offset,
						 0,
						 psBuffer->ui32End,
						 psBuffer->ui32BufferSize,
						 0,
						 MAKEUNIQUETAG(psCtrlMemInfoKM));
			}
			PDUMPCOMMENTWITHFLAGS(0, "%s (part 2)\r\n", pszName);
			PDUMPMEMUM(psPerProc,
					 IMG_NULL,
					 psBuffer->pvLinAddr,
					 (PVRSRV_KERNEL_MEM_INFO*)psBuffer->hKernelMemInfo,
					 0,
					 psBuffer->ui32End,
					 0,
					 hUniqueTag);
		}
	}
	__DtTestPoint(__DtFunc_DumpBufferArray, __DtStep_8)
}
static IMG_INT
SGXPDumpBufferArrayBW(IMG_UINT32 ui32BridgeID,
				   PVRSRV_BRIDGE_IN_PDUMP_BUFFER_ARRAY *psPDumpBufferArrayIN,
				   IMG_VOID *psBridgeOut,
				   PVRSRV_PER_PROCESS_DATA *psPerProc)
{
	IMG_UINT32 i;
	SGX_KICKTA_DUMP_BUFFER *psKickTADumpBuffer;
	IMG_UINT32 ui32BufferArrayLength =
		psPDumpBufferArrayIN->ui32BufferArrayLength;
	IMG_UINT32 ui32BufferArraySize =
		ui32BufferArrayLength * sizeof(SGX_KICKTA_DUMP_BUFFER);
	PVRSRV_ERROR eError = PVRSRV_ERROR_TOO_FEW_BUFFERS;

	__DtTestPoint(__DtFunc_SGXPDumpBufferArrayBW, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(psBridgeOut);

	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGX_PDUMP_BUFFER_ARRAY);

	if(OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP,
				  ui32BufferArraySize,
				  (IMG_PVOID *)&psKickTADumpBuffer, 0,
				  "Array of Kick Tile Accelerator Dump Buffer") != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXPDumpBufferArrayBW, __DtStep_1)
		return -ENOMEM;
	}

	if(CopyFromUserWrapper(psPerProc,
			               ui32BridgeID,
						   psKickTADumpBuffer,
						   psPDumpBufferArrayIN->psBufferArray,
						   ui32BufferArraySize) != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXPDumpBufferArrayBW, __DtStep_2)
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, ui32BufferArraySize, psKickTADumpBuffer, 0);
		
		__DtTestPoint(__DtFunc_SGXPDumpBufferArrayBW, __DtStep_3)
		return -EFAULT;
	}

	for(i = 0; i < ui32BufferArrayLength; i++)
	{
		IMG_VOID *pvMemInfo;

		__DtTestPoint(__DtFunc_SGXPDumpBufferArrayBW, __DtStep_4)
		eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
									&pvMemInfo,
									psKickTADumpBuffer[i].hKernelMemInfo,
									PVRSRV_HANDLE_TYPE_MEM_INFO);

		if(eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXPDumpBufferArrayBW, __DtStep_5)
			PVR_DPF((PVR_DBG_ERROR, "PVRSRV_BRIDGE_SGX_PDUMP_BUFFER_ARRAY: "
					 "PVRSRVLookupHandle failed (%d)", eError));
			break;
		}
		psKickTADumpBuffer[i].hKernelMemInfo = pvMemInfo;

#if defined(SUPPORT_SGX_NEW_STATUS_VALS)
		eError = PVRSRVLookupHandle(psPerProc->psHandleBase,
									&pvMemInfo,
									psKickTADumpBuffer[i].hCtrlKernelMemInfo,
									PVRSRV_HANDLE_TYPE_MEM_INFO);

		if(eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SGXPDumpBufferArrayBW, __DtStep_6)
			PVR_DPF((PVR_DBG_ERROR, "PVRSRV_BRIDGE_SGX_PDUMP_BUFFER_ARRAY: "
					 "PVRSRVLookupHandle failed (%d)", eError));
			break;
		}
		psKickTADumpBuffer[i].hCtrlKernelMemInfo = pvMemInfo;
#endif
	}

	if(eError == PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXPDumpBufferArrayBW, __DtStep_7)
		DumpBufferArray(psPerProc,
						psKickTADumpBuffer,
						ui32BufferArrayLength,
						psPDumpBufferArrayIN->bDumpPolls);
	}

	OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, ui32BufferArraySize, psKickTADumpBuffer, 0);
	

	__DtTestPoint(__DtFunc_SGXPDumpBufferArrayBW, __DtStep_8)
	return 0;
}

static IMG_INT
SGXPDump3DSignatureRegistersBW(IMG_UINT32 ui32BridgeID,
				   PVRSRV_BRIDGE_IN_PDUMP_3D_SIGNATURE_REGISTERS *psPDump3DSignatureRegistersIN,
				   PVRSRV_BRIDGE_RETURN *psRetOUT,
				   PVRSRV_PER_PROCESS_DATA *psPerProc)
{
	IMG_UINT32 ui32RegisterArraySize =  psPDump3DSignatureRegistersIN->ui32NumRegisters * sizeof(IMG_UINT32);
	IMG_UINT32 *pui32Registers = IMG_NULL;
	PVRSRV_SGXDEV_INFO	*psDevInfo = IMG_NULL;
#if defined(SGX_FEATURE_MP)	&& defined(FIX_HW_BRN_27270)
	IMG_UINT32	ui32RegVal = 0;
#endif
	PVRSRV_DEVICE_NODE *psDeviceNode;
	IMG_INT ret = -EFAULT;

	PVR_UNREFERENCED_PARAMETER(psRetOUT);

	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGX_PDUMP_3D_SIGNATURE_REGISTERS);

	if (ui32RegisterArraySize == 0)
	{
		__DtTestPoint(__DtFunc_SGXPDump3DSignatureRegistersBW, __DtStep_0)
		goto ExitNoError;
	}

	psRetOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase, (IMG_VOID**)&psDeviceNode,
						   psPDump3DSignatureRegistersIN->hDevCookie,
						   PVRSRV_HANDLE_TYPE_DEV_NODE);
	if(psRetOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXPDump3DSignatureRegistersBW, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "PDumpTASignatureRegistersBW: hDevCookie lookup failed"));
		goto Exit;
	}

	psDevInfo = (PVRSRV_SGXDEV_INFO*)psDeviceNode->pvDevice;

#if defined(SGX_FEATURE_MP)	&& defined(FIX_HW_BRN_27270)
	
	ui32RegVal = OSReadHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_MASTER_CORE);
	OSWriteHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_MASTER_CORE, (SGX_FEATURE_MP_CORE_COUNT - 1) << EUR_CR_MASTER_CORE_ENABLE_SHIFT);
#if defined(PDUMP)
	PDUMPREGWITHFLAGS(SGX_PDUMPREG_NAME, EUR_CR_MASTER_CORE, (SGX_FEATURE_MP_CORE_COUNT - 1) << EUR_CR_MASTER_CORE_ENABLE_SHIFT,
						psPDump3DSignatureRegistersIN->bLastFrame ? PDUMP_FLAGS_LASTFRAME : 0);
#endif
#endif

	if(OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP,
				  ui32RegisterArraySize,
				  (IMG_PVOID *)&pui32Registers, 0,
				  "Array of Registers") != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXPDump3DSignatureRegistersBW, __DtStep_2)
		PVR_DPF((PVR_DBG_ERROR, "PDump3DSignatureRegistersBW: OSAllocMem failed"));
		goto Exit;
	}

	if(CopyFromUserWrapper(psPerProc,
			        	ui32BridgeID,
					pui32Registers,
					psPDump3DSignatureRegistersIN->pui32Registers,
					ui32RegisterArraySize) != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXPDump3DSignatureRegistersBW, __DtStep_3)
		PVR_DPF((PVR_DBG_ERROR, "PDump3DSignatureRegistersBW: CopyFromUserWrapper failed"));
		goto Exit;
	}

	PDump3DSignatureRegisters(&psDeviceNode->sDevId,
					psPDump3DSignatureRegistersIN->ui32DumpFrameNum,
					psPDump3DSignatureRegistersIN->bLastFrame,
					pui32Registers,
					psPDump3DSignatureRegistersIN->ui32NumRegisters);

	PDumpSignatureBuffer(&psDeviceNode->sDevId,
						 "out.tasig", "TA", 0, 
						 psDevInfo->psKernelTASigBufferMemInfo->sDevVAddr,
						 psDevInfo->psKernelTASigBufferMemInfo->ui32AllocSize,
						 0 );
	PDumpSignatureBuffer(&psDeviceNode->sDevId,
						 "out.3dsig", "3D", 0, 
						 psDevInfo->psKernel3DSigBufferMemInfo->sDevVAddr,
						 psDevInfo->psKernel3DSigBufferMemInfo->ui32AllocSize,
						 0 );
						 
ExitNoError:
	psRetOUT->eError = PVRSRV_OK;
	ret = 0;
Exit:
	if (pui32Registers != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_SGXPDump3DSignatureRegistersBW, __DtStep_4)
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, ui32RegisterArraySize, pui32Registers, 0);
	}

#if defined(SGX_FEATURE_MP)	&& defined(FIX_HW_BRN_27270)
	if (psDevInfo != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_SGXPDump3DSignatureRegistersBW, __DtStep_5)
		OSWriteHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_MASTER_CORE, ui32RegVal);
#if defined(PDUMP)
		PDUMPREGWITHFLAGS(SGX_PDUMPREG_NAME, EUR_CR_MASTER_CORE, ui32RegVal,
							psPDump3DSignatureRegistersIN->bLastFrame ? PDUMP_FLAGS_LASTFRAME : 0);
#endif
	}
#endif

	return ret;
}

static IMG_INT
SGXPDumpCounterRegistersBW(IMG_UINT32 ui32BridgeID,
				   PVRSRV_BRIDGE_IN_PDUMP_COUNTER_REGISTERS *psPDumpCounterRegistersIN,
				   IMG_VOID *psBridgeOut,
				   PVRSRV_PER_PROCESS_DATA *psPerProc)
{
	IMG_UINT32 ui32RegisterArraySize =  psPDumpCounterRegistersIN->ui32NumRegisters * sizeof(IMG_UINT32);
	IMG_UINT32 *pui32Registers = IMG_NULL;
	PVRSRV_DEVICE_NODE *psDeviceNode;
	IMG_INT ret = -EFAULT;

	__DtTestPoint(__DtFunc_SGXPDumpCounterRegistersBW, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(psBridgeOut);

	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGX_PDUMP_COUNTER_REGISTERS);

	if (ui32RegisterArraySize == 0)
	{
		__DtTestPoint(__DtFunc_SGXPDumpCounterRegistersBW, __DtStep_1)
		goto ExitNoError;
	}

	if(PVRSRVLookupHandle(psPerProc->psHandleBase, (IMG_VOID**)&psDeviceNode,
						   psPDumpCounterRegistersIN->hDevCookie,
						   PVRSRV_HANDLE_TYPE_DEV_NODE) != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXPDumpCounterRegistersBW, __DtStep_2)
		PVR_DPF((PVR_DBG_ERROR, "SGXPDumpCounterRegistersBW: hDevCookie lookup failed"));
		ret = -ENOMEM;
		goto Exit;
	}

	if(OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP,
				  ui32RegisterArraySize,
				  (IMG_PVOID *)&pui32Registers, 0,
				  "Array of Registers") != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXPDumpCounterRegistersBW, __DtStep_3)
		PVR_DPF((PVR_DBG_ERROR, "PDumpCounterRegistersBW: OSAllocMem failed"));
		ret = -ENOMEM;
		goto Exit;
	}

	if(CopyFromUserWrapper(psPerProc,
			        	ui32BridgeID,
					pui32Registers,
					psPDumpCounterRegistersIN->pui32Registers,
					ui32RegisterArraySize) != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXPDumpCounterRegistersBW, __DtStep_4)
		PVR_DPF((PVR_DBG_ERROR, "PDumpCounterRegistersBW: CopyFromUserWrapper failed"));
		goto Exit;
	}

	PDumpCounterRegisters(&psDeviceNode->sDevId,
					psPDumpCounterRegistersIN->ui32DumpFrameNum,
					psPDumpCounterRegistersIN->bLastFrame,
					pui32Registers,
					psPDumpCounterRegistersIN->ui32NumRegisters);

ExitNoError:
	ret = 0;
Exit:
	if (pui32Registers != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_SGXPDumpCounterRegistersBW, __DtStep_5)
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, ui32RegisterArraySize, pui32Registers, 0);
	}

	__DtTestPoint(__DtFunc_SGXPDumpCounterRegistersBW, __DtStep_6)
	return ret;
}

static IMG_INT
SGXPDumpTASignatureRegistersBW(IMG_UINT32 ui32BridgeID,
				   PVRSRV_BRIDGE_IN_PDUMP_TA_SIGNATURE_REGISTERS *psPDumpTASignatureRegistersIN,
				   PVRSRV_BRIDGE_RETURN *psRetOUT,
				   PVRSRV_PER_PROCESS_DATA *psPerProc)
{
	IMG_UINT32 ui32RegisterArraySize =  psPDumpTASignatureRegistersIN->ui32NumRegisters * sizeof(IMG_UINT32);
	IMG_UINT32 *pui32Registers = IMG_NULL;
#if defined(SGX_FEATURE_MP)	&& defined(FIX_HW_BRN_27270)
	PVRSRV_SGXDEV_INFO	*psDevInfo = IMG_NULL;
	IMG_UINT32	ui32RegVal = 0;
#endif
	PVRSRV_DEVICE_NODE *psDeviceNode;
	IMG_INT ret = -EFAULT;

	PVR_UNREFERENCED_PARAMETER(psRetOUT);

	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGX_PDUMP_TA_SIGNATURE_REGISTERS);

	if (ui32RegisterArraySize == 0)
	{
		__DtTestPoint(__DtFunc_SGXPDumpTASignatureRegistersBW, __DtStep_0)
		goto ExitNoError;
	}

	psRetOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase, (IMG_VOID**)&psDeviceNode,
						   psPDumpTASignatureRegistersIN->hDevCookie,
						   PVRSRV_HANDLE_TYPE_DEV_NODE);
	if(psRetOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXPDumpTASignatureRegistersBW, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "PDumpTASignatureRegistersBW: hDevCookie lookup failed"));
		goto Exit;
	}

#if defined(SGX_FEATURE_MP)	&& defined(FIX_HW_BRN_27270)

	psDevInfo = (PVRSRV_SGXDEV_INFO*)psDeviceNode->pvDevice;

	
	ui32RegVal = OSReadHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_MASTER_CORE);
	OSWriteHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_MASTER_CORE, (SGX_FEATURE_MP_CORE_COUNT - 1) << EUR_CR_MASTER_CORE_ENABLE_SHIFT);
#if defined(PDUMP)
	PDUMPREGWITHFLAGS(SGX_PDUMPREG_NAME, EUR_CR_MASTER_CORE, (SGX_FEATURE_MP_CORE_COUNT - 1) << EUR_CR_MASTER_CORE_ENABLE_SHIFT,
						psPDumpTASignatureRegistersIN->bLastFrame ? PDUMP_FLAGS_LASTFRAME : 0);
#endif
#endif

	if(OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP,
				  ui32RegisterArraySize,
				  (IMG_PVOID *)&pui32Registers, 0,
				  "Array of Registers") != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXPDumpTASignatureRegistersBW, __DtStep_2)
		PVR_DPF((PVR_DBG_ERROR, "PDumpTASignatureRegistersBW: OSAllocMem failed"));
		ret = -ENOMEM;
		goto Exit;
	}

	if(CopyFromUserWrapper(psPerProc,
			        	ui32BridgeID,
					pui32Registers,
					psPDumpTASignatureRegistersIN->pui32Registers,
					ui32RegisterArraySize) != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXPDumpTASignatureRegistersBW, __DtStep_3)
		PVR_DPF((PVR_DBG_ERROR, "PDumpTASignatureRegistersBW: CopyFromUserWrapper failed"));
		goto Exit;
	}

	PDumpTASignatureRegisters(&psDeviceNode->sDevId,
					psPDumpTASignatureRegistersIN->ui32DumpFrameNum,
					psPDumpTASignatureRegistersIN->ui32TAKickCount,
					psPDumpTASignatureRegistersIN->bLastFrame,
					pui32Registers,
					psPDumpTASignatureRegistersIN->ui32NumRegisters);

ExitNoError:
	psRetOUT->eError = PVRSRV_OK;
	ret = 0;
Exit:
	if (pui32Registers != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_SGXPDumpTASignatureRegistersBW, __DtStep_4)
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, ui32RegisterArraySize, pui32Registers, 0);
	}

#if defined(SGX_FEATURE_MP)	&& defined(FIX_HW_BRN_27270)
	if (psDevInfo != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_SGXPDumpTASignatureRegistersBW, __DtStep_5)
		OSWriteHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_MASTER_CORE, ui32RegVal);
#if defined(PDUMP)
		PDUMPREGWITHFLAGS(SGX_PDUMPREG_NAME, EUR_CR_MASTER_CORE, ui32RegVal,
							psPDumpTASignatureRegistersIN->bLastFrame ? PDUMP_FLAGS_LASTFRAME : 0);
#endif
	}
#endif

	return ret;
}
static IMG_INT
SGXPDumpHWPerfCBBW(IMG_UINT32						ui32BridgeID,
				   PVRSRV_BRIDGE_IN_PDUMP_HWPERFCB	*psPDumpHWPerfCBIN,
				   PVRSRV_BRIDGE_RETURN 			*psRetOUT,
				   PVRSRV_PER_PROCESS_DATA 			*psPerProc)
{
#if defined(SUPPORT_SGX_HWPERF)
#if defined(__linux__)
	PVRSRV_SGXDEV_INFO	*psDevInfo;
	PVRSRV_DEVICE_NODE *psDeviceNode;

	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGX_PDUMP_HWPERFCB);

	psRetOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase, (IMG_VOID**)&psDeviceNode,
						   psPDumpHWPerfCBIN->hDevCookie,
						   PVRSRV_HANDLE_TYPE_DEV_NODE);
	if(psRetOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXPDumpHWPerfCBBW, __DtStep_0)
		return 0;
	}

	psDevInfo = psDeviceNode->pvDevice;

	PDumpHWPerfCBKM(&psDeviceNode->sDevId,
					&psPDumpHWPerfCBIN->szFileName[0],
					psPDumpHWPerfCBIN->ui32FileOffset,
					psDevInfo->psKernelHWPerfCBMemInfo->sDevVAddr,
					psDevInfo->psKernelHWPerfCBMemInfo->ui32AllocSize,
					psPDumpHWPerfCBIN->ui32PDumpFlags);

	__DtTestPoint(__DtFunc_SGXPDumpHWPerfCBBW, __DtStep_1)
	return 0;
#else
	PVR_UNREFERENCED_PARAMETER(ui32BridgeID);
	PVR_UNREFERENCED_PARAMETER(psPDumpHWPerfCBIN);
	PVR_UNREFERENCED_PARAMETER(psRetOUT);
	PVR_UNREFERENCED_PARAMETER(psPerProc);
	__DtTestPoint(__DtFunc_SGXPDumpHWPerfCBBW, __DtStep_2)
	return 0;
#endif 
#else
	PVR_UNREFERENCED_PARAMETER(ui32BridgeID);
	PVR_UNREFERENCED_PARAMETER(psPDumpHWPerfCBIN);
	PVR_UNREFERENCED_PARAMETER(psRetOUT);
	PVR_UNREFERENCED_PARAMETER(psPerProc);
	__DtTestPoint(__DtFunc_SGXPDumpHWPerfCBBW, __DtStep_3)
	return -EFAULT;
#endif 
}


static IMG_INT
SGXPDumpSaveMemBW(IMG_UINT32						ui32BridgeID,
				  PVRSRV_BRIDGE_IN_PDUMP_SAVEMEM	*psPDumpSaveMem,
				  PVRSRV_BRIDGE_RETURN 				*psRetOUT,
				  PVRSRV_PER_PROCESS_DATA 			*psPerProc)
{
	PVRSRV_DEVICE_NODE *psDeviceNode;

	__DtTestPoint(__DtFunc_SGXPDumpSaveMemBW, __DtStep_0)
	PVRSRV_BRIDGE_ASSERT_CMD(ui32BridgeID, PVRSRV_BRIDGE_SGX_PDUMP_SAVEMEM);

	psRetOUT->eError =
		PVRSRVLookupHandle(psPerProc->psHandleBase, (IMG_VOID**)&psDeviceNode,
						   psPDumpSaveMem->hDevCookie,
						   PVRSRV_HANDLE_TYPE_DEV_NODE);
	if(psRetOUT->eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXPDumpSaveMemBW, __DtStep_1)
		return 0;
	}

	PDumpSaveMemKM(&psDeviceNode->sDevId,
				   &psPDumpSaveMem->szFileName[0],
				   psPDumpSaveMem->ui32FileOffset,
				   psPDumpSaveMem->sDevVAddr,
				   psPDumpSaveMem->ui32Size,
				   psPDumpSaveMem->ui32DataMaster,
				   psPDumpSaveMem->ui32PDumpFlags);
	__DtTestPoint(__DtFunc_SGXPDumpSaveMemBW, __DtStep_2)
	return 0;
}

#endif 


   
IMG_VOID SetSGXDispatchTableEntry(IMG_VOID)
{

	__DtTestPoint(__DtFunc_SetSGXDispatchTableEntry, __DtStep_0)
	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_GETCLIENTINFO, SGXGetClientInfoBW);
	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_RELEASECLIENTINFO, SGXReleaseClientInfoBW);
	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_GETINTERNALDEVINFO, SGXGetInternalDevInfoBW);
	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_DOKICK, SGXDoKickBW);
	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_GETPHYSPAGEADDR, DummyBW);
	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_READREGISTRYDWORD, DummyBW);

	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_2DQUERYBLTSCOMPLETE, SGX2DQueryBlitsCompleteBW);

	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_GETMMUPDADDR, DummyBW);

#if defined(TRANSFER_QUEUE)
	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_SUBMITTRANSFER, SGXSubmitTransferBW);
#endif
	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_GETMISCINFO, SGXGetMiscInfoBW);
	SetDispatchTableEntry(PVRSRV_BRIDGE_SGXINFO_FOR_SRVINIT	, SGXGetInfoForSrvinitBW);
	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_DEVINITPART2, SGXDevInitPart2BW);

	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_FINDSHAREDPBDESC, SGXFindSharedPBDescBW);
	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_UNREFSHAREDPBDESC, SGXUnrefSharedPBDescBW);
	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_ADDSHAREDPBDESC, SGXAddSharedPBDescBW);
	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_REGISTER_HW_RENDER_CONTEXT, SGXRegisterHWRenderContextBW);
	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_FLUSH_HW_RENDER_TARGET, SGXFlushHWRenderTargetBW);
	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_UNREGISTER_HW_RENDER_CONTEXT, SGXUnregisterHWRenderContextBW);
#if defined(SGX_FEATURE_2D_HARDWARE)
	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_SUBMIT2D, SGXSubmit2DBW);
	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_REGISTER_HW_2D_CONTEXT, SGXRegisterHW2DContextBW);
	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_UNREGISTER_HW_2D_CONTEXT, SGXUnregisterHW2DContextBW);
#endif
	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_REGISTER_HW_TRANSFER_CONTEXT, SGXRegisterHWTransferContextBW);
	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_UNREGISTER_HW_TRANSFER_CONTEXT, SGXUnregisterHWTransferContextBW);

	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_SCHEDULE_PROCESS_QUEUES, SGXScheduleProcessQueuesBW);

	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_READ_HWPERF_CB, SGXReadHWPerfCBBW);

#if defined(PDUMP)
	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_PDUMP_BUFFER_ARRAY, SGXPDumpBufferArrayBW);
	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_PDUMP_3D_SIGNATURE_REGISTERS, SGXPDump3DSignatureRegistersBW);
	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_PDUMP_COUNTER_REGISTERS, SGXPDumpCounterRegistersBW);
	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_PDUMP_TA_SIGNATURE_REGISTERS, SGXPDumpTASignatureRegistersBW);
	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_PDUMP_HWPERFCB, SGXPDumpHWPerfCBBW);
	SetDispatchTableEntry(PVRSRV_BRIDGE_SGX_PDUMP_SAVEMEM, SGXPDumpSaveMemBW);
#endif
	__DtTestPoint(__DtFunc_SetSGXDispatchTableEntry, __DtStep_1)
}
 

#endif 
