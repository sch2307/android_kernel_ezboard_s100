#include	"DT_deviceclass.h"	/* For DynamicTracer-TestPoint */
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
#include "buffer_manager.h"
#include "kernelbuffer.h"
#include "kerneldisplay.h"
#include "pvr_bridge_km.h"
#include "pdump_km.h"
#include "deviceid.h"

#include "lists.h"

PVRSRV_ERROR AllocateDeviceID(SYS_DATA *psSysData, IMG_UINT32 *pui32DevID);
PVRSRV_ERROR FreeDeviceID(SYS_DATA *psSysData, IMG_UINT32 ui32DevID);

#if defined(SUPPORT_MISR_IN_THREAD)
void OSVSyncMISR(IMG_HANDLE, IMG_BOOL);
#endif

#if defined(SUPPORT_CUSTOM_SWAP_OPERATIONS)
IMG_VOID PVRSRVFreeCommandCompletePacketKM(IMG_HANDLE	hCmdCookie,
										   IMG_BOOL		bScheduleMISR);
#endif
typedef struct PVRSRV_DC_SRV2DISP_KMJTABLE_TAG *PPVRSRV_DC_SRV2DISP_KMJTABLE;

typedef struct PVRSRV_DC_BUFFER_TAG
{
	
	PVRSRV_DEVICECLASS_BUFFER sDeviceClassBuffer;

	struct PVRSRV_DISPLAYCLASS_INFO_TAG *psDCInfo;
	struct PVRSRV_DC_SWAPCHAIN_TAG *psSwapChain;
} PVRSRV_DC_BUFFER;

typedef struct PVRSRV_DC_SWAPCHAIN_TAG
{
	IMG_HANDLE							hExtSwapChain;
	IMG_UINT32							ui32SwapChainID;
	IMG_UINT32							ui32RefCount;
	IMG_UINT32							ui32Flags;
	PVRSRV_QUEUE_INFO					*psQueue;
	PVRSRV_DC_BUFFER					asBuffer[PVRSRV_MAX_DC_SWAPCHAIN_BUFFERS];
	IMG_UINT32							ui32BufferCount;
	PVRSRV_DC_BUFFER					*psLastFlipBuffer;
	IMG_UINT32							ui32MinSwapInterval;
	IMG_UINT32							ui32MaxSwapInterval;
	struct PVRSRV_DISPLAYCLASS_INFO_TAG *psDCInfo;
	struct PVRSRV_DC_SWAPCHAIN_TAG		*psNext;
} PVRSRV_DC_SWAPCHAIN;


typedef struct PVRSRV_DC_SWAPCHAIN_REF_TAG
{
	struct PVRSRV_DC_SWAPCHAIN_TAG		*psSwapChain;
	IMG_HANDLE							hResItem;	
} PVRSRV_DC_SWAPCHAIN_REF;


typedef struct PVRSRV_DISPLAYCLASS_INFO_TAG
{
	IMG_UINT32 							ui32RefCount;
	IMG_UINT32							ui32DeviceID;
	IMG_HANDLE							hExtDevice;
	PPVRSRV_DC_SRV2DISP_KMJTABLE		psFuncTable;
	IMG_HANDLE							hDevMemContext;
	PVRSRV_DC_BUFFER 					sSystemBuffer;
	struct PVRSRV_DC_SWAPCHAIN_TAG		*psDCSwapChainShared;
} PVRSRV_DISPLAYCLASS_INFO;


typedef struct PVRSRV_DISPLAYCLASS_PERCONTEXT_INFO_TAG
{
	PVRSRV_DISPLAYCLASS_INFO			*psDCInfo;
	PRESMAN_ITEM						hResItem;
} PVRSRV_DISPLAYCLASS_PERCONTEXT_INFO;


typedef struct PVRSRV_BC_SRV2BUFFER_KMJTABLE_TAG *PPVRSRV_BC_SRV2BUFFER_KMJTABLE;

typedef struct PVRSRV_BC_BUFFER_TAG
{
	
	PVRSRV_DEVICECLASS_BUFFER sDeviceClassBuffer;

	struct PVRSRV_BUFFERCLASS_INFO_TAG *psBCInfo;
} PVRSRV_BC_BUFFER;


typedef struct PVRSRV_BUFFERCLASS_INFO_TAG
{
	IMG_UINT32 							ui32RefCount;
	IMG_UINT32							ui32DeviceID;
	IMG_HANDLE							hExtDevice;
	PPVRSRV_BC_SRV2BUFFER_KMJTABLE		psFuncTable;
	IMG_HANDLE							hDevMemContext;
	
	IMG_UINT32							ui32BufferCount;
	PVRSRV_BC_BUFFER 					*psBuffer;

} PVRSRV_BUFFERCLASS_INFO;


typedef struct PVRSRV_BUFFERCLASS_PERCONTEXT_INFO_TAG
{
	PVRSRV_BUFFERCLASS_INFO				*psBCInfo;
	IMG_HANDLE							hResItem;
} PVRSRV_BUFFERCLASS_PERCONTEXT_INFO;


static PVRSRV_DISPLAYCLASS_INFO* DCDeviceHandleToDCInfo (IMG_HANDLE hDeviceKM)
{
	PVRSRV_DISPLAYCLASS_PERCONTEXT_INFO *psDCPerContextInfo;

	__DtTestPoint(__DtFunc_DCDeviceHandleToDCInfo, __DtStep_0)
	psDCPerContextInfo = (PVRSRV_DISPLAYCLASS_PERCONTEXT_INFO *)hDeviceKM;

	__DtTestPoint(__DtFunc_DCDeviceHandleToDCInfo, __DtStep_1)
	return psDCPerContextInfo->psDCInfo;
}


static PVRSRV_BUFFERCLASS_INFO* BCDeviceHandleToBCInfo (IMG_HANDLE hDeviceKM)
{
	PVRSRV_BUFFERCLASS_PERCONTEXT_INFO *psBCPerContextInfo;

	__DtTestPoint(__DtFunc_BCDeviceHandleToBCInfo, __DtStep_0)
	psBCPerContextInfo = (PVRSRV_BUFFERCLASS_PERCONTEXT_INFO *)hDeviceKM;

	__DtTestPoint(__DtFunc_BCDeviceHandleToBCInfo, __DtStep_1)
	return psBCPerContextInfo->psBCInfo;
}

static IMG_VOID PVRSRVEnumerateDCKM_ForEachVaCb(PVRSRV_DEVICE_NODE *psDeviceNode, va_list va)
{
	IMG_UINT *pui32DevCount;
	IMG_UINT32 **ppui32DevID;
	PVRSRV_DEVICE_CLASS peDeviceClass;

	__DtTestPoint(__DtFunc_PVRSRVEnumerateDCKM_ForEachVaCb, __DtStep_0)
	pui32DevCount = va_arg(va, IMG_UINT*);
	ppui32DevID = va_arg(va, IMG_UINT32**);
	peDeviceClass = va_arg(va, PVRSRV_DEVICE_CLASS);

	if	((psDeviceNode->sDevId.eDeviceClass == peDeviceClass)
	&&	(psDeviceNode->sDevId.eDeviceType == PVRSRV_DEVICE_TYPE_EXT))
	{
		__DtTestPoint(__DtFunc_PVRSRVEnumerateDCKM_ForEachVaCb, __DtStep_1)
		(*pui32DevCount)++;
		if(*ppui32DevID)
		{
			__DtTestPoint(__DtFunc_PVRSRVEnumerateDCKM_ForEachVaCb, __DtStep_2)
			*(*ppui32DevID)++ = psDeviceNode->sDevId.ui32DeviceIndex;
		}
	}
	__DtTestPoint(__DtFunc_PVRSRVEnumerateDCKM_ForEachVaCb, __DtStep_3)
}


IMG_EXPORT
PVRSRV_ERROR PVRSRVEnumerateDCKM (PVRSRV_DEVICE_CLASS DeviceClass,
								  IMG_UINT32 *pui32DevCount,
								  IMG_UINT32 *pui32DevID )
{
	
	IMG_UINT			ui32DevCount = 0;
	SYS_DATA 			*psSysData;

	__DtTestPoint(__DtFunc_PVRSRVEnumerateDCKM, __DtStep_0)
	SysAcquireData(&psSysData);

	
	List_PVRSRV_DEVICE_NODE_ForEach_va(psSysData->psDeviceNodeList,
										&PVRSRVEnumerateDCKM_ForEachVaCb,
										&ui32DevCount,
										&pui32DevID,
										DeviceClass);

	if(pui32DevCount)
	{
		__DtTestPoint(__DtFunc_PVRSRVEnumerateDCKM, __DtStep_1)
		*pui32DevCount = ui32DevCount;
	}
	else if(pui32DevID == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_PVRSRVEnumerateDCKM, __DtStep_2)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVEnumerateDCKM: Invalid parameters"));
		__DtTestPoint(__DtFunc_PVRSRVEnumerateDCKM, __DtStep_3)
		return (PVRSRV_ERROR_INVALID_PARAMS);
	}

	__DtTestPoint(__DtFunc_PVRSRVEnumerateDCKM, __DtStep_4)
	return PVRSRV_OK;
}


static
PVRSRV_ERROR PVRSRVRegisterDCDeviceKM (PVRSRV_DC_SRV2DISP_KMJTABLE *psFuncTable,
									   IMG_UINT32 *pui32DeviceID)
{
	PVRSRV_DISPLAYCLASS_INFO 	*psDCInfo = IMG_NULL;
	PVRSRV_DEVICE_NODE			*psDeviceNode;
	SYS_DATA					*psSysData;

	__DtTestPoint(__DtFunc_PVRSRVRegisterDCDeviceKM, __DtStep_0)
	printk("=================> [%s:%d]\n",__FILE__,__LINE__);













	SysAcquireData(&psSysData);

	
	printk("=================> [%s:%d]\n",__FILE__,__LINE__);


	
	if(OSAllocMem( PVRSRV_OS_PAGEABLE_HEAP,
					 sizeof(*psDCInfo),
					 (IMG_VOID **)&psDCInfo, IMG_NULL,
					 "Display Class Info") != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVRegisterDCDeviceKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVRegisterDCDeviceKM: Failed psDCInfo alloc"));
		__DtTestPoint(__DtFunc_PVRSRVRegisterDCDeviceKM, __DtStep_2)
		return PVRSRV_ERROR_OUT_OF_MEMORY;
	}
	printk("=================> [%s:%d]\n",__FILE__,__LINE__);
	OSMemSet (psDCInfo, 0, sizeof(*psDCInfo));

	
	printk("=================> [%s:%d]\n",__FILE__,__LINE__);
	if(OSAllocMem( PVRSRV_OS_PAGEABLE_HEAP,
					 sizeof(PVRSRV_DC_SRV2DISP_KMJTABLE),
					 (IMG_VOID **)&psDCInfo->psFuncTable, IMG_NULL,
					 "Function table for SRVKM->DISPLAY") != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVRegisterDCDeviceKM, __DtStep_3)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVRegisterDCDeviceKM: Failed psFuncTable alloc"));
		goto ErrorExit;
	}
	printk("=================> [%s:%d]\n",__FILE__,__LINE__);
	OSMemSet (psDCInfo->psFuncTable, 0, sizeof(PVRSRV_DC_SRV2DISP_KMJTABLE));

	
	printk("=================> [%s:%d]\n",__FILE__,__LINE__);
	*psDCInfo->psFuncTable = *psFuncTable;

	
	printk("=================> [%s:%d]\n",__FILE__,__LINE__);
	if(OSAllocMem( PVRSRV_OS_PAGEABLE_HEAP,
					 sizeof(PVRSRV_DEVICE_NODE),
					 (IMG_VOID **)&psDeviceNode, IMG_NULL,
					 "Device Node") != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVRegisterDCDeviceKM, __DtStep_4)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVRegisterDCDeviceKM: Failed psDeviceNode alloc"));
		goto ErrorExit;
	}
	printk("=================> [%s:%d]\n",__FILE__,__LINE__);
	OSMemSet (psDeviceNode, 0, sizeof(PVRSRV_DEVICE_NODE));

	printk("=================> [%s:%d]\n",__FILE__,__LINE__);
	psDeviceNode->pvDevice = (IMG_VOID*)psDCInfo;
	psDeviceNode->ui32pvDeviceSize = sizeof(*psDCInfo);
	psDeviceNode->ui32RefCount = 1;
	psDeviceNode->sDevId.eDeviceType = PVRSRV_DEVICE_TYPE_EXT;
	psDeviceNode->sDevId.eDeviceClass = PVRSRV_DEVICE_CLASS_DISPLAY;
	psDeviceNode->psSysData = psSysData;

	
	printk("=================> [%s:%d]\n",__FILE__,__LINE__);
	if (AllocateDeviceID(psSysData, &psDeviceNode->sDevId.ui32DeviceIndex) != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVRegisterDCDeviceKM, __DtStep_5)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVRegisterBCDeviceKM: Failed to allocate Device ID"));
		goto ErrorExit;
	}
	printk("=================> [%s:%d]\n",__FILE__,__LINE__);
	psDCInfo->ui32DeviceID = psDeviceNode->sDevId.ui32DeviceIndex;
	printk("=================> [%s:%d]\n",__FILE__,__LINE__);
	if (pui32DeviceID)
	{
		__DtTestPoint(__DtFunc_PVRSRVRegisterDCDeviceKM, __DtStep_6)
		*pui32DeviceID = psDeviceNode->sDevId.ui32DeviceIndex;
	}

	
	printk("=================> [%s:%d]\n",__FILE__,__LINE__);
	SysRegisterExternalDevice(psDeviceNode);

	printk("=================> [%s:%d]\n",__FILE__,__LINE__);
	
	List_PVRSRV_DEVICE_NODE_Insert(&psSysData->psDeviceNodeList, psDeviceNode);

	printk("=================> [%s:%d]\n",__FILE__,__LINE__);
	__DtTestPoint(__DtFunc_PVRSRVRegisterDCDeviceKM, __DtStep_7)
	return PVRSRV_OK;

ErrorExit:

	if(psDCInfo->psFuncTable)
	{
		__DtTestPoint(__DtFunc_PVRSRVRegisterDCDeviceKM, __DtStep_8)
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(PVRSRV_DC_SRV2DISP_KMJTABLE), psDCInfo->psFuncTable, IMG_NULL);
		psDCInfo->psFuncTable = IMG_NULL;
	}

	OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(PVRSRV_DISPLAYCLASS_INFO), psDCInfo, IMG_NULL);
	

	__DtTestPoint(__DtFunc_PVRSRVRegisterDCDeviceKM, __DtStep_9)
	return PVRSRV_ERROR_OUT_OF_MEMORY;
}

static PVRSRV_ERROR PVRSRVRemoveDCDeviceKM(IMG_UINT32 ui32DevIndex)
{
	SYS_DATA					*psSysData;
	PVRSRV_DEVICE_NODE			*psDeviceNode;
	PVRSRV_DISPLAYCLASS_INFO	*psDCInfo;

	__DtTestPoint(__DtFunc_PVRSRVRemoveDCDeviceKM, __DtStep_0)
	SysAcquireData(&psSysData);

	
	psDeviceNode = (PVRSRV_DEVICE_NODE*)
		List_PVRSRV_DEVICE_NODE_Any_va(psSysData->psDeviceNodeList,
									   &MatchDeviceKM_AnyVaCb,
									   ui32DevIndex,
									   IMG_FALSE,
									   PVRSRV_DEVICE_CLASS_DISPLAY);
	if (!psDeviceNode)
	{
		
		__DtTestPoint(__DtFunc_PVRSRVRemoveDCDeviceKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVRemoveDCDeviceKM: requested device %d not present", ui32DevIndex));
		__DtTestPoint(__DtFunc_PVRSRVRemoveDCDeviceKM, __DtStep_2)
		return PVRSRV_ERROR_NO_DEVICENODE_FOUND;
	}

	
	psDCInfo = (PVRSRV_DISPLAYCLASS_INFO*)psDeviceNode->pvDevice;

	


	if(psDCInfo->ui32RefCount == 0)
	{
		

		__DtTestPoint(__DtFunc_PVRSRVRemoveDCDeviceKM, __DtStep_3)
		List_PVRSRV_DEVICE_NODE_Remove(psDeviceNode);

		
		SysRemoveExternalDevice(psDeviceNode);

		


		PVR_ASSERT(psDCInfo->ui32RefCount == 0);
		(IMG_VOID)FreeDeviceID(psSysData, ui32DevIndex);
		(IMG_VOID)OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(PVRSRV_DC_SRV2DISP_KMJTABLE), psDCInfo->psFuncTable, IMG_NULL);
		psDCInfo->psFuncTable = IMG_NULL;
		(IMG_VOID)OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(PVRSRV_DISPLAYCLASS_INFO), psDCInfo, IMG_NULL);
		
		(IMG_VOID)OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(PVRSRV_DEVICE_NODE), psDeviceNode, IMG_NULL);
		
	}
	else
	{
		__DtTestPoint(__DtFunc_PVRSRVRemoveDCDeviceKM, __DtStep_4)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVRemoveDCDeviceKM: failed as %d Services DC API connections are still open", psDCInfo->ui32RefCount));
		__DtTestPoint(__DtFunc_PVRSRVRemoveDCDeviceKM, __DtStep_5)
		return PVRSRV_ERROR_UNABLE_TO_REMOVE_DEVICE;
	}

	__DtTestPoint(__DtFunc_PVRSRVRemoveDCDeviceKM, __DtStep_6)
	return PVRSRV_OK;
}


static
PVRSRV_ERROR PVRSRVRegisterBCDeviceKM (PVRSRV_BC_SRV2BUFFER_KMJTABLE *psFuncTable,
									   IMG_UINT32	*pui32DeviceID)
{
	PVRSRV_BUFFERCLASS_INFO	*psBCInfo = IMG_NULL;
	PVRSRV_DEVICE_NODE		*psDeviceNode;
	SYS_DATA				*psSysData;
	













	__DtTestPoint(__DtFunc_PVRSRVRegisterBCDeviceKM, __DtStep_0)
	SysAcquireData(&psSysData);

	


	
	if(OSAllocMem( PVRSRV_OS_PAGEABLE_HEAP,
					 sizeof(*psBCInfo),
					 (IMG_VOID **)&psBCInfo, IMG_NULL,
					 "Buffer Class Info") != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVRegisterBCDeviceKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVRegisterBCDeviceKM: Failed psBCInfo alloc"));
		__DtTestPoint(__DtFunc_PVRSRVRegisterBCDeviceKM, __DtStep_2)
		return PVRSRV_ERROR_OUT_OF_MEMORY;
	}
	OSMemSet (psBCInfo, 0, sizeof(*psBCInfo));

	
	if(OSAllocMem( PVRSRV_OS_PAGEABLE_HEAP,
					 sizeof(PVRSRV_BC_SRV2BUFFER_KMJTABLE),
					 (IMG_VOID **)&psBCInfo->psFuncTable, IMG_NULL,
					 "Function table for SRVKM->BUFFER") != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVRegisterBCDeviceKM, __DtStep_3)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVRegisterBCDeviceKM: Failed psFuncTable alloc"));
		goto ErrorExit;
	}
	OSMemSet (psBCInfo->psFuncTable, 0, sizeof(PVRSRV_BC_SRV2BUFFER_KMJTABLE));

	
	*psBCInfo->psFuncTable = *psFuncTable;

	
	if(OSAllocMem( PVRSRV_OS_PAGEABLE_HEAP,
					 sizeof(PVRSRV_DEVICE_NODE),
					 (IMG_VOID **)&psDeviceNode, IMG_NULL,
					 "Device Node") != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVRegisterBCDeviceKM, __DtStep_4)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVRegisterBCDeviceKM: Failed psDeviceNode alloc"));
		goto ErrorExit;
	}
	OSMemSet (psDeviceNode, 0, sizeof(PVRSRV_DEVICE_NODE));

	psDeviceNode->pvDevice = (IMG_VOID*)psBCInfo;
	psDeviceNode->ui32pvDeviceSize = sizeof(*psBCInfo);
	psDeviceNode->ui32RefCount = 1;
	psDeviceNode->sDevId.eDeviceType = PVRSRV_DEVICE_TYPE_EXT;
	psDeviceNode->sDevId.eDeviceClass = PVRSRV_DEVICE_CLASS_BUFFER;
	psDeviceNode->psSysData = psSysData;

	
	if (AllocateDeviceID(psSysData, &psDeviceNode->sDevId.ui32DeviceIndex) != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVRegisterBCDeviceKM, __DtStep_5)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVRegisterBCDeviceKM: Failed to allocate Device ID"));
		goto ErrorExit;
	}
	psBCInfo->ui32DeviceID = psDeviceNode->sDevId.ui32DeviceIndex;
	if (pui32DeviceID)
	{
		__DtTestPoint(__DtFunc_PVRSRVRegisterBCDeviceKM, __DtStep_6)
		*pui32DeviceID = psDeviceNode->sDevId.ui32DeviceIndex;
	}

	
	List_PVRSRV_DEVICE_NODE_Insert(&psSysData->psDeviceNodeList, psDeviceNode);

	__DtTestPoint(__DtFunc_PVRSRVRegisterBCDeviceKM, __DtStep_7)
	return PVRSRV_OK;

ErrorExit:

	if(psBCInfo->psFuncTable)
	{
		__DtTestPoint(__DtFunc_PVRSRVRegisterBCDeviceKM, __DtStep_8)
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(PPVRSRV_BC_SRV2BUFFER_KMJTABLE), psBCInfo->psFuncTable, IMG_NULL);
		psBCInfo->psFuncTable = IMG_NULL;
	}

	OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(PVRSRV_BUFFERCLASS_INFO), psBCInfo, IMG_NULL);
	

	__DtTestPoint(__DtFunc_PVRSRVRegisterBCDeviceKM, __DtStep_9)
	return PVRSRV_ERROR_OUT_OF_MEMORY;
}


static PVRSRV_ERROR PVRSRVRemoveBCDeviceKM(IMG_UINT32 ui32DevIndex)
{
	SYS_DATA					*psSysData;
	PVRSRV_DEVICE_NODE			*psDevNode;
	PVRSRV_BUFFERCLASS_INFO		*psBCInfo;

	__DtTestPoint(__DtFunc_PVRSRVRemoveBCDeviceKM, __DtStep_0)
	SysAcquireData(&psSysData);

	
	psDevNode = (PVRSRV_DEVICE_NODE*)
		List_PVRSRV_DEVICE_NODE_Any_va(psSysData->psDeviceNodeList,
									   &MatchDeviceKM_AnyVaCb,
									   ui32DevIndex,
									   IMG_FALSE,
									   PVRSRV_DEVICE_CLASS_BUFFER);

	if (!psDevNode)
	{
		__DtTestPoint(__DtFunc_PVRSRVRemoveBCDeviceKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVRemoveBCDeviceKM: requested device %d not present", ui32DevIndex));
		__DtTestPoint(__DtFunc_PVRSRVRemoveBCDeviceKM, __DtStep_2)
		return PVRSRV_ERROR_NO_DEVICENODE_FOUND;
	}

	
	
	psBCInfo = (PVRSRV_BUFFERCLASS_INFO*)psDevNode->pvDevice;

	


	if(psBCInfo->ui32RefCount == 0)
	{
		

		__DtTestPoint(__DtFunc_PVRSRVRemoveBCDeviceKM, __DtStep_3)
		List_PVRSRV_DEVICE_NODE_Remove(psDevNode);

		


		(IMG_VOID)FreeDeviceID(psSysData, ui32DevIndex);
		
		
		(IMG_VOID)OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(PVRSRV_BC_SRV2BUFFER_KMJTABLE), psBCInfo->psFuncTable, IMG_NULL);
		psBCInfo->psFuncTable = IMG_NULL;
		(IMG_VOID)OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(PVRSRV_BUFFERCLASS_INFO), psBCInfo, IMG_NULL);
		
		(IMG_VOID)OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(PVRSRV_DEVICE_NODE), psDevNode, IMG_NULL);
		
	}
	else
	{
		__DtTestPoint(__DtFunc_PVRSRVRemoveBCDeviceKM, __DtStep_4)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVRemoveBCDeviceKM: failed as %d Services BC API connections are still open", psBCInfo->ui32RefCount));
		__DtTestPoint(__DtFunc_PVRSRVRemoveBCDeviceKM, __DtStep_5)
		return PVRSRV_ERROR_UNABLE_TO_REMOVE_DEVICE;
	}

	__DtTestPoint(__DtFunc_PVRSRVRemoveBCDeviceKM, __DtStep_6)
	return PVRSRV_OK;
}



IMG_EXPORT
PVRSRV_ERROR PVRSRVCloseDCDeviceKM (IMG_HANDLE	hDeviceKM,
									IMG_BOOL	bResManCallback)
{
	PVRSRV_ERROR eError;
	PVRSRV_DISPLAYCLASS_PERCONTEXT_INFO *psDCPerContextInfo;

	__DtTestPoint(__DtFunc_PVRSRVCloseDCDeviceKM, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(bResManCallback);

	psDCPerContextInfo = (PVRSRV_DISPLAYCLASS_PERCONTEXT_INFO *)hDeviceKM;

	
	eError = ResManFreeResByPtr(psDCPerContextInfo->hResItem);

	__DtTestPoint(__DtFunc_PVRSRVCloseDCDeviceKM, __DtStep_1)
	return eError;
}


static PVRSRV_ERROR CloseDCDeviceCallBack(IMG_PVOID		pvParam,
										  IMG_UINT32	ui32Param)
{
	PVRSRV_DISPLAYCLASS_PERCONTEXT_INFO *psDCPerContextInfo;
	PVRSRV_DISPLAYCLASS_INFO *psDCInfo;

	__DtTestPoint(__DtFunc_CloseDCDeviceCallBack, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(ui32Param);

	psDCPerContextInfo = (PVRSRV_DISPLAYCLASS_PERCONTEXT_INFO *)pvParam;
	psDCInfo = psDCPerContextInfo->psDCInfo;

	psDCInfo->ui32RefCount--;
	if(psDCInfo->ui32RefCount == 0)
	{
		
		__DtTestPoint(__DtFunc_CloseDCDeviceCallBack, __DtStep_1)
		psDCInfo->psFuncTable->pfnCloseDCDevice(psDCInfo->hExtDevice);

		if (--psDCInfo->sSystemBuffer.sDeviceClassBuffer.psKernelSyncInfo->ui32RefCount == 0)
		{
			__DtTestPoint(__DtFunc_CloseDCDeviceCallBack, __DtStep_2)
			PVRSRVFreeSyncInfoKM(psDCInfo->sSystemBuffer.sDeviceClassBuffer.psKernelSyncInfo);
		}

		psDCInfo->hDevMemContext = IMG_NULL;
		psDCInfo->hExtDevice = IMG_NULL;
	}

	OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(PVRSRV_DISPLAYCLASS_PERCONTEXT_INFO), psDCPerContextInfo, IMG_NULL);
	

	__DtTestPoint(__DtFunc_CloseDCDeviceCallBack, __DtStep_3)
	return PVRSRV_OK;
}


IMG_EXPORT
PVRSRV_ERROR PVRSRVOpenDCDeviceKM (PVRSRV_PER_PROCESS_DATA	*psPerProc,
								   IMG_UINT32				ui32DeviceID,
								   IMG_HANDLE				hDevCookie,
								   IMG_HANDLE				*phDeviceKM)
{
	PVRSRV_DISPLAYCLASS_INFO *psDCInfo;
	PVRSRV_DISPLAYCLASS_PERCONTEXT_INFO *psDCPerContextInfo;
	PVRSRV_DEVICE_NODE	*psDeviceNode;
	SYS_DATA			*psSysData;
	PVRSRV_ERROR eError;

	__DtTestPoint(__DtFunc_PVRSRVOpenDCDeviceKM, __DtStep_0)
	if(!phDeviceKM || !hDevCookie)
	{
		__DtTestPoint(__DtFunc_PVRSRVOpenDCDeviceKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVOpenDCDeviceKM: Invalid params"));
		__DtTestPoint(__DtFunc_PVRSRVOpenDCDeviceKM, __DtStep_2)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	SysAcquireData(&psSysData);

	
	psDeviceNode = (PVRSRV_DEVICE_NODE*)
			List_PVRSRV_DEVICE_NODE_Any_va(psSysData->psDeviceNodeList,
										   &MatchDeviceKM_AnyVaCb,
										   ui32DeviceID,
										   IMG_FALSE,
										   PVRSRV_DEVICE_CLASS_DISPLAY);
	if (!psDeviceNode)
	{
		__DtTestPoint(__DtFunc_PVRSRVOpenDCDeviceKM, __DtStep_3)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVOpenDCDeviceKM: no devnode matching index %d", ui32DeviceID));
		__DtTestPoint(__DtFunc_PVRSRVOpenDCDeviceKM, __DtStep_4)
		return PVRSRV_ERROR_NO_DEVICENODE_FOUND;
	}
	psDCInfo = (PVRSRV_DISPLAYCLASS_INFO*)psDeviceNode->pvDevice;

	


	if(OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP,
				  sizeof(*psDCPerContextInfo),
				  (IMG_VOID **)&psDCPerContextInfo, IMG_NULL,
				  "Display Class per Context Info") != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVOpenDCDeviceKM, __DtStep_5)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVOpenDCDeviceKM: Failed psDCPerContextInfo alloc"));
		__DtTestPoint(__DtFunc_PVRSRVOpenDCDeviceKM, __DtStep_6)
		return PVRSRV_ERROR_OUT_OF_MEMORY;
	}
	OSMemSet(psDCPerContextInfo, 0, sizeof(*psDCPerContextInfo));

	if(psDCInfo->ui32RefCount++ == 0)
	{

		__DtTestPoint(__DtFunc_PVRSRVOpenDCDeviceKM, __DtStep_7)
		psDeviceNode = (PVRSRV_DEVICE_NODE *)hDevCookie;

		
		psDCInfo->hDevMemContext = (IMG_HANDLE)psDeviceNode->sDevMemoryInfo.pBMKernelContext;

		
		eError = PVRSRVAllocSyncInfoKM(IMG_NULL,
									(IMG_HANDLE)psDeviceNode->sDevMemoryInfo.pBMKernelContext,
									&psDCInfo->sSystemBuffer.sDeviceClassBuffer.psKernelSyncInfo);
		if(eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PVRSRVOpenDCDeviceKM, __DtStep_8)
			PVR_DPF((PVR_DBG_ERROR,"PVRSRVOpenDCDeviceKM: Failed sync info alloc"));
			psDCInfo->ui32RefCount--;
			__DtTestPoint(__DtFunc_PVRSRVOpenDCDeviceKM, __DtStep_9)
			return eError;
		}

		
		eError = psDCInfo->psFuncTable->pfnOpenDCDevice(ui32DeviceID,
                                                        	&psDCInfo->hExtDevice,
								(PVRSRV_SYNC_DATA*)psDCInfo->sSystemBuffer.sDeviceClassBuffer.psKernelSyncInfo->psSyncDataMemInfoKM->pvLinAddrKM);
		if(eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PVRSRVOpenDCDeviceKM, __DtStep_10)
			PVR_DPF((PVR_DBG_ERROR,"PVRSRVOpenDCDeviceKM: Failed to open external DC device"));
			psDCInfo->ui32RefCount--;
			PVRSRVFreeSyncInfoKM(psDCInfo->sSystemBuffer.sDeviceClassBuffer.psKernelSyncInfo);
			__DtTestPoint(__DtFunc_PVRSRVOpenDCDeviceKM, __DtStep_11)
			return eError;
		}

		psDCInfo->sSystemBuffer.sDeviceClassBuffer.psKernelSyncInfo->ui32RefCount++;
	}

	psDCPerContextInfo->psDCInfo = psDCInfo;
	psDCPerContextInfo->hResItem = ResManRegisterRes(psPerProc->hResManContext,
													 RESMAN_TYPE_DISPLAYCLASS_DEVICE,
													 psDCPerContextInfo,
													 0,
													 &CloseDCDeviceCallBack);

	
	*phDeviceKM = (IMG_HANDLE)psDCPerContextInfo;

	__DtTestPoint(__DtFunc_PVRSRVOpenDCDeviceKM, __DtStep_12)
	return PVRSRV_OK;
}


IMG_EXPORT
PVRSRV_ERROR PVRSRVEnumDCFormatsKM (IMG_HANDLE hDeviceKM,
									IMG_UINT32 *pui32Count,
									DISPLAY_FORMAT *psFormat)
{
	PVRSRV_DISPLAYCLASS_INFO *psDCInfo;

	__DtTestPoint(__DtFunc_PVRSRVEnumDCFormatsKM, __DtStep_0)
	if(!hDeviceKM || !pui32Count || !psFormat)
	{
		__DtTestPoint(__DtFunc_PVRSRVEnumDCFormatsKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVEnumDCFormatsKM: Invalid parameters"));
		__DtTestPoint(__DtFunc_PVRSRVEnumDCFormatsKM, __DtStep_2)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	psDCInfo = DCDeviceHandleToDCInfo(hDeviceKM);

	
	__DtTestPoint(__DtFunc_PVRSRVEnumDCFormatsKM, __DtStep_3)
	return psDCInfo->psFuncTable->pfnEnumDCFormats(psDCInfo->hExtDevice, pui32Count, psFormat);
}



IMG_EXPORT
PVRSRV_ERROR PVRSRVEnumDCDimsKM (IMG_HANDLE hDeviceKM,
								 DISPLAY_FORMAT *psFormat,
								 IMG_UINT32 *pui32Count,
								 DISPLAY_DIMS *psDim)
{
	PVRSRV_DISPLAYCLASS_INFO *psDCInfo;

	__DtTestPoint(__DtFunc_PVRSRVEnumDCDimsKM, __DtStep_0)
	if(!hDeviceKM || !pui32Count || !psFormat)	
	{
		__DtTestPoint(__DtFunc_PVRSRVEnumDCDimsKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVEnumDCDimsKM: Invalid parameters"));
		__DtTestPoint(__DtFunc_PVRSRVEnumDCDimsKM, __DtStep_2)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	psDCInfo = DCDeviceHandleToDCInfo(hDeviceKM);

	
	__DtTestPoint(__DtFunc_PVRSRVEnumDCDimsKM, __DtStep_3)
	return psDCInfo->psFuncTable->pfnEnumDCDims(psDCInfo->hExtDevice, psFormat, pui32Count, psDim);
}


IMG_EXPORT
PVRSRV_ERROR PVRSRVGetDCSystemBufferKM (IMG_HANDLE hDeviceKM,
										IMG_HANDLE *phBuffer)
{
	PVRSRV_ERROR eError;
	PVRSRV_DISPLAYCLASS_INFO *psDCInfo;
	IMG_HANDLE hExtBuffer;

	__DtTestPoint(__DtFunc_PVRSRVGetDCSystemBufferKM, __DtStep_0)
	if(!hDeviceKM || !phBuffer)
	{
		__DtTestPoint(__DtFunc_PVRSRVGetDCSystemBufferKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVGetDCSystemBufferKM: Invalid parameters"));
		__DtTestPoint(__DtFunc_PVRSRVGetDCSystemBufferKM, __DtStep_2)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	psDCInfo = DCDeviceHandleToDCInfo(hDeviceKM);

	
	eError = psDCInfo->psFuncTable->pfnGetDCSystemBuffer(psDCInfo->hExtDevice, &hExtBuffer);
	if(eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVGetDCSystemBufferKM, __DtStep_3)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVGetDCSystemBufferKM: Failed to get valid buffer handle from external driver"));
		__DtTestPoint(__DtFunc_PVRSRVGetDCSystemBufferKM, __DtStep_4)
		return eError;
	}

	
	psDCInfo->sSystemBuffer.sDeviceClassBuffer.pfnGetBufferAddr = psDCInfo->psFuncTable->pfnGetBufferAddr;
	psDCInfo->sSystemBuffer.sDeviceClassBuffer.hDevMemContext = psDCInfo->hDevMemContext;
	psDCInfo->sSystemBuffer.sDeviceClassBuffer.hExtDevice = psDCInfo->hExtDevice;
	psDCInfo->sSystemBuffer.sDeviceClassBuffer.hExtBuffer = hExtBuffer;

	psDCInfo->sSystemBuffer.psDCInfo = psDCInfo;

	
	*phBuffer = (IMG_HANDLE)&(psDCInfo->sSystemBuffer);

	__DtTestPoint(__DtFunc_PVRSRVGetDCSystemBufferKM, __DtStep_5)
	return PVRSRV_OK;
}


IMG_EXPORT
PVRSRV_ERROR PVRSRVGetDCInfoKM (IMG_HANDLE hDeviceKM,
								DISPLAY_INFO *psDisplayInfo)
{
	PVRSRV_DISPLAYCLASS_INFO *psDCInfo;
	PVRSRV_ERROR eError;

	__DtTestPoint(__DtFunc_PVRSRVGetDCInfoKM, __DtStep_0)
	if(!hDeviceKM || !psDisplayInfo)
	{
		__DtTestPoint(__DtFunc_PVRSRVGetDCInfoKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVGetDCInfoKM: Invalid parameters"));
		__DtTestPoint(__DtFunc_PVRSRVGetDCInfoKM, __DtStep_2)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	psDCInfo = DCDeviceHandleToDCInfo(hDeviceKM);

	
	eError = psDCInfo->psFuncTable->pfnGetDCInfo(psDCInfo->hExtDevice, psDisplayInfo);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVGetDCInfoKM, __DtStep_3)
		return eError;
	}

	if (psDisplayInfo->ui32MaxSwapChainBuffers > PVRSRV_MAX_DC_SWAPCHAIN_BUFFERS)
	{
		__DtTestPoint(__DtFunc_PVRSRVGetDCInfoKM, __DtStep_4)
		psDisplayInfo->ui32MaxSwapChainBuffers = PVRSRV_MAX_DC_SWAPCHAIN_BUFFERS;
	}

	__DtTestPoint(__DtFunc_PVRSRVGetDCInfoKM, __DtStep_5)
	return PVRSRV_OK;
}


IMG_EXPORT
PVRSRV_ERROR PVRSRVDestroyDCSwapChainKM(IMG_HANDLE hSwapChainRef)
{
	PVRSRV_ERROR eError;
	PVRSRV_DC_SWAPCHAIN_REF *psSwapChainRef;

	__DtTestPoint(__DtFunc_PVRSRVDestroyDCSwapChainKM, __DtStep_0)
	if(!hSwapChainRef)
	{
		__DtTestPoint(__DtFunc_PVRSRVDestroyDCSwapChainKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVDestroyDCSwapChainKM: Invalid parameters"));
		__DtTestPoint(__DtFunc_PVRSRVDestroyDCSwapChainKM, __DtStep_2)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	psSwapChainRef = hSwapChainRef;

	eError = ResManFreeResByPtr(psSwapChainRef->hResItem);

	__DtTestPoint(__DtFunc_PVRSRVDestroyDCSwapChainKM, __DtStep_3)
	return eError;
}


static PVRSRV_ERROR DestroyDCSwapChain(PVRSRV_DC_SWAPCHAIN *psSwapChain)
{
	PVRSRV_ERROR				eError;
	PVRSRV_DISPLAYCLASS_INFO	*psDCInfo = psSwapChain->psDCInfo;
	IMG_UINT32 i;


	
	__DtTestPoint(__DtFunc_DestroyDCSwapChain, __DtStep_0)
	if( psDCInfo->psDCSwapChainShared )
	{
		__DtTestPoint(__DtFunc_DestroyDCSwapChain, __DtStep_1)
		if( psDCInfo->psDCSwapChainShared == psSwapChain )
		{
			__DtTestPoint(__DtFunc_DestroyDCSwapChain, __DtStep_2)
			psDCInfo->psDCSwapChainShared = psSwapChain->psNext;
		}
		else 
		{
			PVRSRV_DC_SWAPCHAIN *psCurrentSwapChain;
			__DtTestPoint(__DtFunc_DestroyDCSwapChain, __DtStep_3)
			psCurrentSwapChain = psDCInfo->psDCSwapChainShared; 		
			while( psCurrentSwapChain->psNext )
			{
				__DtTestPoint(__DtFunc_DestroyDCSwapChain, __DtStep_4)
				if( psCurrentSwapChain->psNext != psSwapChain ) 
				{
					__DtTestPoint(__DtFunc_DestroyDCSwapChain, __DtStep_5)
					psCurrentSwapChain = psCurrentSwapChain->psNext;
					continue;
				}
				psCurrentSwapChain->psNext = psSwapChain->psNext;
				break;				
			}
		}
	}

	
	PVRSRVDestroyCommandQueueKM(psSwapChain->psQueue);

	
	eError = psDCInfo->psFuncTable->pfnDestroyDCSwapChain(psDCInfo->hExtDevice,
															psSwapChain->hExtSwapChain);

	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_DestroyDCSwapChain, __DtStep_6)
		PVR_DPF((PVR_DBG_ERROR,"DestroyDCSwapChainCallBack: Failed to destroy DC swap chain"));
		__DtTestPoint(__DtFunc_DestroyDCSwapChain, __DtStep_7)
		return eError;
	}

	
	for(i=0; i<psSwapChain->ui32BufferCount; i++)
	{
		__DtTestPoint(__DtFunc_DestroyDCSwapChain, __DtStep_8)
		if(psSwapChain->asBuffer[i].sDeviceClassBuffer.psKernelSyncInfo)
		{
			__DtTestPoint(__DtFunc_DestroyDCSwapChain, __DtStep_9)
			if (--psSwapChain->asBuffer[i].sDeviceClassBuffer.psKernelSyncInfo->ui32RefCount == 0)
			{
				__DtTestPoint(__DtFunc_DestroyDCSwapChain, __DtStep_10)
				PVRSRVFreeSyncInfoKM(psSwapChain->asBuffer[i].sDeviceClassBuffer.psKernelSyncInfo);
			}
		}
	}

	OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(PVRSRV_DC_SWAPCHAIN), psSwapChain, IMG_NULL);
	

	__DtTestPoint(__DtFunc_DestroyDCSwapChain, __DtStep_11)
	return eError;
}


static PVRSRV_ERROR DestroyDCSwapChainRefCallBack(IMG_PVOID pvParam, IMG_UINT32 ui32Param)
{
	PVRSRV_DC_SWAPCHAIN_REF *psSwapChainRef = (PVRSRV_DC_SWAPCHAIN_REF *) pvParam;
	PVRSRV_ERROR eError = PVRSRV_OK;

	__DtTestPoint(__DtFunc_DestroyDCSwapChainRefCallBack, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(ui32Param);

	if(--psSwapChainRef->psSwapChain->ui32RefCount == 0) 
	{
		__DtTestPoint(__DtFunc_DestroyDCSwapChainRefCallBack, __DtStep_1)
		eError = DestroyDCSwapChain(psSwapChainRef->psSwapChain);
	}

	OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(PVRSRV_DC_SWAPCHAIN_REF), psSwapChainRef, IMG_NULL);
	__DtTestPoint(__DtFunc_DestroyDCSwapChainRefCallBack, __DtStep_2)
	return eError;
}

static PVRSRV_DC_SWAPCHAIN* PVRSRVFindSharedDCSwapChainKM(PVRSRV_DISPLAYCLASS_INFO *psDCInfo,
														 IMG_UINT32 ui32SwapChainID)
{
	PVRSRV_DC_SWAPCHAIN *psCurrentSwapChain;

	__DtTestPoint(__DtFunc_PVRSRVFindSharedDCSwapChainKM, __DtStep_0)
	for(psCurrentSwapChain = psDCInfo->psDCSwapChainShared; 
		psCurrentSwapChain; 
		psCurrentSwapChain = psCurrentSwapChain->psNext) 
	{
		__DtTestPoint(__DtFunc_PVRSRVFindSharedDCSwapChainKM, __DtStep_1)
		if(psCurrentSwapChain->ui32SwapChainID == ui32SwapChainID)
			return psCurrentSwapChain;
	}
	__DtTestPoint(__DtFunc_PVRSRVFindSharedDCSwapChainKM, __DtStep_2)
	return IMG_NULL;
}

static PVRSRV_ERROR PVRSRVCreateDCSwapChainRefKM(PVRSRV_PER_PROCESS_DATA	*psPerProc,
												 PVRSRV_DC_SWAPCHAIN 		*psSwapChain, 
												 PVRSRV_DC_SWAPCHAIN_REF 	**ppsSwapChainRef)
{
	PVRSRV_DC_SWAPCHAIN_REF *psSwapChainRef = IMG_NULL;

	
	__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainRefKM, __DtStep_0)
	if(OSAllocMem( PVRSRV_OS_PAGEABLE_HEAP,
					 sizeof(PVRSRV_DC_SWAPCHAIN_REF),
					 (IMG_VOID **)&psSwapChainRef, IMG_NULL,
					 "Display Class Swapchain Reference") != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainRefKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVCreateDCSwapChainRefKM: Failed psSwapChainRef alloc"));
		__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainRefKM, __DtStep_2)
		return  PVRSRV_ERROR_OUT_OF_MEMORY;
	}
	OSMemSet (psSwapChainRef, 0, sizeof(PVRSRV_DC_SWAPCHAIN_REF));

	
	psSwapChain->ui32RefCount++;

	
	psSwapChainRef->psSwapChain = psSwapChain;
	psSwapChainRef->hResItem = ResManRegisterRes(psPerProc->hResManContext,
												  RESMAN_TYPE_DISPLAYCLASS_SWAPCHAIN_REF,
												  psSwapChainRef,
												  0,
												  &DestroyDCSwapChainRefCallBack);
	*ppsSwapChainRef = psSwapChainRef;

	__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainRefKM, __DtStep_3)
	return PVRSRV_OK;
}


IMG_EXPORT
PVRSRV_ERROR PVRSRVCreateDCSwapChainKM (PVRSRV_PER_PROCESS_DATA	*psPerProc,
										IMG_HANDLE				hDeviceKM,
										IMG_UINT32				ui32Flags,
										DISPLAY_SURF_ATTRIBUTES	*psDstSurfAttrib,
										DISPLAY_SURF_ATTRIBUTES *psSrcSurfAttrib,
										IMG_UINT32				ui32BufferCount,
										IMG_UINT32				ui32OEMFlags,
										IMG_HANDLE				*phSwapChainRef,
										IMG_UINT32				*pui32SwapChainID)
{
	PVRSRV_DISPLAYCLASS_INFO *psDCInfo;
	PVRSRV_DC_SWAPCHAIN *psSwapChain = IMG_NULL;
	PVRSRV_DC_SWAPCHAIN_REF *psSwapChainRef = IMG_NULL;
	PVRSRV_SYNC_DATA *apsSyncData[PVRSRV_MAX_DC_SWAPCHAIN_BUFFERS];
	PVRSRV_QUEUE_INFO *psQueue = IMG_NULL;
	PVRSRV_ERROR eError;
	IMG_UINT32 i;
	DISPLAY_INFO sDisplayInfo;


	__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_0)
	if(!hDeviceKM
	|| !psDstSurfAttrib
	|| !psSrcSurfAttrib
	|| !phSwapChainRef
	|| !pui32SwapChainID)
	{
		__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVCreateDCSwapChainKM: Invalid parameters"));
		__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_2)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	if (ui32BufferCount > PVRSRV_MAX_DC_SWAPCHAIN_BUFFERS)
	{
		__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_3)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVCreateDCSwapChainKM: Too many buffers"));
		__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_4)
		return PVRSRV_ERROR_TOOMANYBUFFERS;
	}

	if (ui32BufferCount < 2)
	{
		__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_5)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVCreateDCSwapChainKM: Too few buffers"));
		__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_6)
		return PVRSRV_ERROR_TOO_FEW_BUFFERS;
	}

	psDCInfo = DCDeviceHandleToDCInfo(hDeviceKM);

	if( ui32Flags & PVRSRV_CREATE_SWAPCHAIN_QUERY )
	{
		
		__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_7)
		psSwapChain = PVRSRVFindSharedDCSwapChainKM(psDCInfo, *pui32SwapChainID );
		if( psSwapChain  ) 
		{	
					   
			__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_8)
			eError = PVRSRVCreateDCSwapChainRefKM(psPerProc, 
												  psSwapChain, 
												  &psSwapChainRef);
			if( eError != PVRSRV_OK ) 
			{
				__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_9)
				PVR_DPF((PVR_DBG_ERROR,"PVRSRVCreateDCSwapChainKM: Couldn't create swap chain reference"));
				__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_10)
				return eError;
			}

			*phSwapChainRef = (IMG_HANDLE)psSwapChainRef;
			__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_11)
			return PVRSRV_OK;
		}
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVCreateDCSwapChainKM: No shared SwapChain found for query"));
		__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_12)
		return PVRSRV_ERROR_FLIP_CHAIN_EXISTS;		
	}

	
	if(OSAllocMem( PVRSRV_OS_PAGEABLE_HEAP,
					 sizeof(PVRSRV_DC_SWAPCHAIN),
					 (IMG_VOID **)&psSwapChain, IMG_NULL,
					 "Display Class Swapchain") != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_13)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVCreateDCSwapChainKM: Failed psSwapChain alloc"));
		eError = PVRSRV_ERROR_OUT_OF_MEMORY;
		goto ErrorExit;
	}
	OSMemSet (psSwapChain, 0, sizeof(PVRSRV_DC_SWAPCHAIN));

	
	eError = PVRSRVCreateCommandQueueKM(1024, &psQueue);
	if(eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_14)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVCreateDCSwapChainKM: Failed to create CmdQueue"));
		goto ErrorExit;
	}

	
	psSwapChain->psQueue = psQueue;

	
	for(i=0; i<ui32BufferCount; i++)
	{
		__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_15)
		eError = PVRSRVAllocSyncInfoKM(IMG_NULL,
										psDCInfo->hDevMemContext,
										&psSwapChain->asBuffer[i].sDeviceClassBuffer.psKernelSyncInfo);
		if(eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_16)
			PVR_DPF((PVR_DBG_ERROR,"PVRSRVCreateDCSwapChainKM: Failed to alloc syninfo for psSwapChain"));
			goto ErrorExit;
		}

		psSwapChain->asBuffer[i].sDeviceClassBuffer.psKernelSyncInfo->ui32RefCount++;

		
		psSwapChain->asBuffer[i].sDeviceClassBuffer.pfnGetBufferAddr = psDCInfo->psFuncTable->pfnGetBufferAddr;
		psSwapChain->asBuffer[i].sDeviceClassBuffer.hDevMemContext = psDCInfo->hDevMemContext;
		psSwapChain->asBuffer[i].sDeviceClassBuffer.hExtDevice = psDCInfo->hExtDevice;

		
		psSwapChain->asBuffer[i].psDCInfo = psDCInfo;
		psSwapChain->asBuffer[i].psSwapChain = psSwapChain;

		
		apsSyncData[i] = (PVRSRV_SYNC_DATA*)psSwapChain->asBuffer[i].sDeviceClassBuffer.psKernelSyncInfo->psSyncDataMemInfoKM->pvLinAddrKM;
	}

	psSwapChain->ui32BufferCount = ui32BufferCount;
	psSwapChain->psDCInfo = psDCInfo;

#if defined(PDUMP)
	PDUMPCOMMENT("Allocate DC swap chain (SwapChainID == %u, BufferCount == %u)",
			*pui32SwapChainID,
			ui32BufferCount);
	PDUMPCOMMENT("  Src surface dimensions == %u x %u",
			psSrcSurfAttrib->sDims.ui32Width,
			psSrcSurfAttrib->sDims.ui32Height);
	PDUMPCOMMENT("  Dst surface dimensions == %u x %u",
			psDstSurfAttrib->sDims.ui32Width,
			psDstSurfAttrib->sDims.ui32Height);
#endif

	eError = psDCInfo->psFuncTable->pfnGetDCInfo(psDCInfo->hExtDevice, &sDisplayInfo);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_17)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVCreateDCSwapChainKM: Failed to get DC info"));
		__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_18)
		return eError;
	}
	
	psSwapChain->ui32MinSwapInterval = sDisplayInfo.ui32MinSwapInterval;
	psSwapChain->ui32MaxSwapInterval = sDisplayInfo.ui32MaxSwapInterval;

	
	eError =  psDCInfo->psFuncTable->pfnCreateDCSwapChain(psDCInfo->hExtDevice,
														ui32Flags,
														psDstSurfAttrib,
														psSrcSurfAttrib,
														ui32BufferCount,
														apsSyncData,
														ui32OEMFlags,
														&psSwapChain->hExtSwapChain,
														&psSwapChain->ui32SwapChainID);
	if(eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_19)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVCreateDCSwapChainKM: Failed to create 3rd party SwapChain"));
		PDUMPCOMMENT("Swapchain allocation failed.");
		goto ErrorExit;
	}

			   
	eError = PVRSRVCreateDCSwapChainRefKM(psPerProc, 
										  psSwapChain, 
										  &psSwapChainRef);
	if( eError != PVRSRV_OK ) 
	{
		__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_20)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVCreateDCSwapChainKM: Couldn't create swap chain reference"));
		PDUMPCOMMENT("Swapchain allocation failed.");
		goto ErrorExit;
	}

	psSwapChain->ui32RefCount = 1;
	psSwapChain->ui32Flags = ui32Flags;

	
	if( ui32Flags & PVRSRV_CREATE_SWAPCHAIN_SHARED )
	{
   		__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_21)
   		if(! psDCInfo->psDCSwapChainShared ) 
		{
			__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_22)
			psDCInfo->psDCSwapChainShared = psSwapChain;
		} 
		else 
		{	
			PVRSRV_DC_SWAPCHAIN *psOldHead = psDCInfo->psDCSwapChainShared;
			__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_23)
			psDCInfo->psDCSwapChainShared = psSwapChain;
			psSwapChain->psNext = psOldHead;
		}
	}

	
	*pui32SwapChainID = psSwapChain->ui32SwapChainID;

	
	*phSwapChainRef= (IMG_HANDLE)psSwapChainRef;

	__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_24)
	return eError;

ErrorExit:

	for(i=0; i<ui32BufferCount; i++)
	{
		__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_25)
		if(psSwapChain->asBuffer[i].sDeviceClassBuffer.psKernelSyncInfo)
		{
			__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_26)
			if (--psSwapChain->asBuffer[i].sDeviceClassBuffer.psKernelSyncInfo->ui32RefCount == 0)
			{
				__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_27)
				PVRSRVFreeSyncInfoKM(psSwapChain->asBuffer[i].sDeviceClassBuffer.psKernelSyncInfo);
			}
		}
	}

	if(psQueue)
	{
		__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_28)
		PVRSRVDestroyCommandQueueKM(psQueue);
	}

	if(psSwapChain)
	{
		__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_29)
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(PVRSRV_DC_SWAPCHAIN), psSwapChain, IMG_NULL);
		
	}

	__DtTestPoint(__DtFunc_PVRSRVCreateDCSwapChainKM, __DtStep_30)
	return eError;
}




IMG_EXPORT
PVRSRV_ERROR PVRSRVSetDCDstRectKM(IMG_HANDLE	hDeviceKM,
								  IMG_HANDLE	hSwapChainRef,
								  IMG_RECT		*psRect)
{
	PVRSRV_DISPLAYCLASS_INFO *psDCInfo;
	PVRSRV_DC_SWAPCHAIN *psSwapChain;

	__DtTestPoint(__DtFunc_PVRSRVSetDCDstRectKM, __DtStep_0)
	if(!hDeviceKM || !hSwapChainRef)
	{
		__DtTestPoint(__DtFunc_PVRSRVSetDCDstRectKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVSetDCDstRectKM: Invalid parameters"));
		__DtTestPoint(__DtFunc_PVRSRVSetDCDstRectKM, __DtStep_2)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	psDCInfo = DCDeviceHandleToDCInfo(hDeviceKM);
	psSwapChain = ((PVRSRV_DC_SWAPCHAIN_REF*)hSwapChainRef)->psSwapChain;

	__DtTestPoint(__DtFunc_PVRSRVSetDCDstRectKM, __DtStep_3)
	return psDCInfo->psFuncTable->pfnSetDCDstRect(psDCInfo->hExtDevice,
													psSwapChain->hExtSwapChain,
													psRect);
}


IMG_EXPORT
PVRSRV_ERROR PVRSRVSetDCSrcRectKM(IMG_HANDLE	hDeviceKM,
								  IMG_HANDLE	hSwapChainRef,
								  IMG_RECT		*psRect)
{
	PVRSRV_DISPLAYCLASS_INFO *psDCInfo;
	PVRSRV_DC_SWAPCHAIN *psSwapChain;

	__DtTestPoint(__DtFunc_PVRSRVSetDCSrcRectKM, __DtStep_0)
	if(!hDeviceKM || !hSwapChainRef)
	{
		__DtTestPoint(__DtFunc_PVRSRVSetDCSrcRectKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVSetDCSrcRectKM: Invalid parameters"));
		__DtTestPoint(__DtFunc_PVRSRVSetDCSrcRectKM, __DtStep_2)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	psDCInfo = DCDeviceHandleToDCInfo(hDeviceKM);
	psSwapChain = ((PVRSRV_DC_SWAPCHAIN_REF*)hSwapChainRef)->psSwapChain;

	__DtTestPoint(__DtFunc_PVRSRVSetDCSrcRectKM, __DtStep_3)
	return psDCInfo->psFuncTable->pfnSetDCSrcRect(psDCInfo->hExtDevice,
													psSwapChain->hExtSwapChain,
													psRect);
}


IMG_EXPORT
PVRSRV_ERROR PVRSRVSetDCDstColourKeyKM(IMG_HANDLE	hDeviceKM,
									   IMG_HANDLE	hSwapChainRef,
									   IMG_UINT32	ui32CKColour)
{
	PVRSRV_DISPLAYCLASS_INFO *psDCInfo;
	PVRSRV_DC_SWAPCHAIN *psSwapChain;

	__DtTestPoint(__DtFunc_PVRSRVSetDCDstColourKeyKM, __DtStep_0)
	if(!hDeviceKM || !hSwapChainRef)
	{
		__DtTestPoint(__DtFunc_PVRSRVSetDCDstColourKeyKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVSetDCDstColourKeyKM: Invalid parameters"));
		__DtTestPoint(__DtFunc_PVRSRVSetDCDstColourKeyKM, __DtStep_2)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	psDCInfo = DCDeviceHandleToDCInfo(hDeviceKM);
	psSwapChain = ((PVRSRV_DC_SWAPCHAIN_REF*)hSwapChainRef)->psSwapChain;

	__DtTestPoint(__DtFunc_PVRSRVSetDCDstColourKeyKM, __DtStep_3)
	return psDCInfo->psFuncTable->pfnSetDCDstColourKey(psDCInfo->hExtDevice,
														psSwapChain->hExtSwapChain,
														ui32CKColour);
}


IMG_EXPORT
PVRSRV_ERROR PVRSRVSetDCSrcColourKeyKM(IMG_HANDLE	hDeviceKM,
									   IMG_HANDLE	hSwapChainRef,
									   IMG_UINT32	ui32CKColour)
{
	PVRSRV_DISPLAYCLASS_INFO *psDCInfo;
	PVRSRV_DC_SWAPCHAIN *psSwapChain;

	__DtTestPoint(__DtFunc_PVRSRVSetDCSrcColourKeyKM, __DtStep_0)
	if(!hDeviceKM || !hSwapChainRef)
	{
		__DtTestPoint(__DtFunc_PVRSRVSetDCSrcColourKeyKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVSetDCSrcColourKeyKM: Invalid parameters"));
		__DtTestPoint(__DtFunc_PVRSRVSetDCSrcColourKeyKM, __DtStep_2)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	psDCInfo = DCDeviceHandleToDCInfo(hDeviceKM);
	psSwapChain = ((PVRSRV_DC_SWAPCHAIN_REF*)hSwapChainRef)->psSwapChain;

	__DtTestPoint(__DtFunc_PVRSRVSetDCSrcColourKeyKM, __DtStep_3)
	return psDCInfo->psFuncTable->pfnSetDCSrcColourKey(psDCInfo->hExtDevice,
														psSwapChain->hExtSwapChain,
														ui32CKColour);
}


IMG_EXPORT
PVRSRV_ERROR PVRSRVGetDCBuffersKM(IMG_HANDLE	hDeviceKM,
								  IMG_HANDLE	hSwapChainRef,
								  IMG_UINT32	*pui32BufferCount,
								  IMG_HANDLE	*phBuffer)
{
	PVRSRV_DISPLAYCLASS_INFO *psDCInfo;
	PVRSRV_DC_SWAPCHAIN *psSwapChain;
	IMG_HANDLE ahExtBuffer[PVRSRV_MAX_DC_SWAPCHAIN_BUFFERS];
	PVRSRV_ERROR eError;
	IMG_UINT32 i;

	__DtTestPoint(__DtFunc_PVRSRVGetDCBuffersKM, __DtStep_0)
	if(!hDeviceKM || !hSwapChainRef || !phBuffer)
	{
		__DtTestPoint(__DtFunc_PVRSRVGetDCBuffersKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVGetDCBuffersKM: Invalid parameters"));
		__DtTestPoint(__DtFunc_PVRSRVGetDCBuffersKM, __DtStep_2)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	psDCInfo = DCDeviceHandleToDCInfo(hDeviceKM);
	psSwapChain = ((PVRSRV_DC_SWAPCHAIN_REF*)hSwapChainRef)->psSwapChain;

	
	eError = psDCInfo->psFuncTable->pfnGetDCBuffers(psDCInfo->hExtDevice,
													psSwapChain->hExtSwapChain,
													pui32BufferCount,
													ahExtBuffer);

	PVR_ASSERT(*pui32BufferCount <= PVRSRV_MAX_DC_SWAPCHAIN_BUFFERS);

	


	for(i=0; i<*pui32BufferCount; i++)
	{
		__DtTestPoint(__DtFunc_PVRSRVGetDCBuffersKM, __DtStep_3)
		psSwapChain->asBuffer[i].sDeviceClassBuffer.hExtBuffer = ahExtBuffer[i];
		phBuffer[i] = (IMG_HANDLE)&psSwapChain->asBuffer[i];
	}

	__DtTestPoint(__DtFunc_PVRSRVGetDCBuffersKM, __DtStep_4)
	return eError;
}


IMG_EXPORT
PVRSRV_ERROR PVRSRVSwapToDCBufferKM(IMG_HANDLE	hDeviceKM,
									IMG_HANDLE	hBuffer,
									IMG_UINT32	ui32SwapInterval,
									IMG_HANDLE	hPrivateTag,
									IMG_UINT32	ui32ClipRectCount,
									IMG_RECT	*psClipRect)
{
	PVRSRV_ERROR eError;
	PVRSRV_DISPLAYCLASS_INFO *psDCInfo;
	PVRSRV_DC_BUFFER *psBuffer;
	PVRSRV_QUEUE_INFO *psQueue;
	DISPLAYCLASS_FLIP_COMMAND *psFlipCmd;
	IMG_UINT32 i;
	IMG_BOOL bAddReferenceToLast = IMG_TRUE;
	IMG_UINT16 ui16SwapCommandID = DC_FLIP_COMMAND;
	IMG_UINT32 ui32NumSrcSyncs = 1;
	PVRSRV_KERNEL_SYNC_INFO *apsSrcSync[2];
	PVRSRV_COMMAND *psCommand;

	__DtTestPoint(__DtFunc_PVRSRVSwapToDCBufferKM, __DtStep_0)
	if(!hDeviceKM || !hBuffer || !psClipRect)
	{
		__DtTestPoint(__DtFunc_PVRSRVSwapToDCBufferKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVSwapToDCBufferKM: Invalid parameters"));
		__DtTestPoint(__DtFunc_PVRSRVSwapToDCBufferKM, __DtStep_2)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

#if defined(SUPPORT_LMA)
	eError = PVRSRVPowerLock(KERNEL_ID, IMG_FALSE);
	if(eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVSwapToDCBufferKM, __DtStep_3)
		return eError;
	}
#endif 

	psBuffer = (PVRSRV_DC_BUFFER*)hBuffer;
	psDCInfo = DCDeviceHandleToDCInfo(hDeviceKM);

	
	if(ui32SwapInterval < psBuffer->psSwapChain->ui32MinSwapInterval ||
		ui32SwapInterval > psBuffer->psSwapChain->ui32MaxSwapInterval)
	{
		__DtTestPoint(__DtFunc_PVRSRVSwapToDCBufferKM, __DtStep_4)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVSwapToDCBufferKM: Invalid swap interval. Requested %u, Allowed range %u-%u",
				 ui32SwapInterval, psBuffer->psSwapChain->ui32MinSwapInterval, psBuffer->psSwapChain->ui32MaxSwapInterval));
		__DtTestPoint(__DtFunc_PVRSRVSwapToDCBufferKM, __DtStep_5)
		return PVRSRV_ERROR_INVALID_SWAPINTERVAL;
	}

#if defined(SUPPORT_CUSTOM_SWAP_OPERATIONS)

	if(psDCInfo->psFuncTable->pfnQuerySwapCommandID != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_PVRSRVSwapToDCBufferKM, __DtStep_6)
		psDCInfo->psFuncTable->pfnQuerySwapCommandID(psDCInfo->hExtDevice,
													 psBuffer->psSwapChain->hExtSwapChain,
													 psBuffer->sDeviceClassBuffer.hExtBuffer, 
													 hPrivateTag, 
													 &ui16SwapCommandID,
													 &bAddReferenceToLast);
		
	}

#endif

	
	psQueue = psBuffer->psSwapChain->psQueue;

	
	apsSrcSync[0] = psBuffer->sDeviceClassBuffer.psKernelSyncInfo;
	


	if(bAddReferenceToLast && psBuffer->psSwapChain->psLastFlipBuffer &&
		psBuffer != psBuffer->psSwapChain->psLastFlipBuffer)
	{
		__DtTestPoint(__DtFunc_PVRSRVSwapToDCBufferKM, __DtStep_7)
		apsSrcSync[1] = psBuffer->psSwapChain->psLastFlipBuffer->sDeviceClassBuffer.psKernelSyncInfo;
		


		ui32NumSrcSyncs++;
	}

	
	eError = PVRSRVInsertCommandKM (psQueue,
									&psCommand,
									psDCInfo->ui32DeviceID,
									ui16SwapCommandID,
									0,
									IMG_NULL,
									ui32NumSrcSyncs,
									apsSrcSync,
									sizeof(DISPLAYCLASS_FLIP_COMMAND) + (sizeof(IMG_RECT) * ui32ClipRectCount));
	if(eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVSwapToDCBufferKM, __DtStep_8)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVSwapToDCBufferKM: Failed to get space in queue"));
		goto Exit;
	}

	
	psFlipCmd = (DISPLAYCLASS_FLIP_COMMAND*)psCommand->pvData;

	
	psFlipCmd->hExtDevice = psDCInfo->hExtDevice;

	
	psFlipCmd->hExtSwapChain = psBuffer->psSwapChain->hExtSwapChain;

	
	psFlipCmd->hExtBuffer = psBuffer->sDeviceClassBuffer.hExtBuffer;

	
	psFlipCmd->hPrivateTag = hPrivateTag;

	
	psFlipCmd->ui32ClipRectCount = ui32ClipRectCount;
	
	psFlipCmd->psClipRect = (IMG_RECT*)((IMG_UINT8*)psFlipCmd + sizeof(DISPLAYCLASS_FLIP_COMMAND));	
	
	for(i=0; i<ui32ClipRectCount; i++)
	{
		__DtTestPoint(__DtFunc_PVRSRVSwapToDCBufferKM, __DtStep_9)
		psFlipCmd->psClipRect[i] = psClipRect[i];
	}

	
	psFlipCmd->ui32SwapInterval = ui32SwapInterval;

	
	eError = PVRSRVSubmitCommandKM (psQueue, psCommand);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVSwapToDCBufferKM, __DtStep_10)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVSwapToDCBufferKM: Failed to submit command"));
		goto Exit;
	}

	







	 
	LOOP_UNTIL_TIMEOUT(MAX_HW_TIME_US)
	{
		if(PVRSRVProcessQueues(KERNEL_ID, IMG_FALSE) != PVRSRV_ERROR_PROCESSING_BLOCKED)
		{
			__DtTestPoint(__DtFunc_PVRSRVSwapToDCBufferKM, __DtStep_11)
			goto ProcessedQueues;
		}
		OSWaitus(MAX_HW_TIME_US/WAIT_TRY_COUNT);
	} END_LOOP_UNTIL_TIMEOUT();

	PVR_DPF((PVR_DBG_ERROR,"PVRSRVSwapToDCBufferKM: Failed to process queues"));

	eError = PVRSRV_ERROR_FAILED_TO_PROCESS_QUEUE;
	goto Exit;

ProcessedQueues:
	
	psBuffer->psSwapChain->psLastFlipBuffer = psBuffer;

Exit:

	if(eError == PVRSRV_ERROR_CANNOT_GET_QUEUE_SPACE)
	{
		__DtTestPoint(__DtFunc_PVRSRVSwapToDCBufferKM, __DtStep_12)
		eError = PVRSRV_ERROR_RETRY;
	}

#if defined(SUPPORT_LMA)
	PVRSRVPowerUnlock(KERNEL_ID);
#endif
	return eError;
}


IMG_EXPORT
PVRSRV_ERROR PVRSRVSwapToDCSystemKM(IMG_HANDLE	hDeviceKM,
									IMG_HANDLE	hSwapChainRef)
{
	PVRSRV_ERROR eError;
	PVRSRV_QUEUE_INFO *psQueue;
	PVRSRV_DISPLAYCLASS_INFO *psDCInfo;
	PVRSRV_DC_SWAPCHAIN *psSwapChain;
	PVRSRV_DC_SWAPCHAIN_REF *psSwapChainRef;
	DISPLAYCLASS_FLIP_COMMAND *psFlipCmd;
	IMG_UINT32 ui32NumSrcSyncs = 1;
	PVRSRV_KERNEL_SYNC_INFO *apsSrcSync[2];
	PVRSRV_COMMAND *psCommand;
	IMG_BOOL bAddReferenceToLast = IMG_TRUE;
	IMG_UINT16 ui16SwapCommandID = DC_FLIP_COMMAND;

	__DtTestPoint(__DtFunc_PVRSRVSwapToDCSystemKM, __DtStep_0)
	if(!hDeviceKM || !hSwapChainRef)
	{
		__DtTestPoint(__DtFunc_PVRSRVSwapToDCSystemKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVSwapToDCSystemKM: Invalid parameters"));
		__DtTestPoint(__DtFunc_PVRSRVSwapToDCSystemKM, __DtStep_2)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

#if defined(SUPPORT_LMA)
	eError = PVRSRVPowerLock(KERNEL_ID, IMG_FALSE);
	if(eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVSwapToDCSystemKM, __DtStep_3)
		return eError;
	}
#endif 

	psDCInfo = DCDeviceHandleToDCInfo(hDeviceKM);
	psSwapChainRef = (PVRSRV_DC_SWAPCHAIN_REF*)hSwapChainRef;
	psSwapChain = psSwapChainRef->psSwapChain;

	
	psQueue = psSwapChain->psQueue;

#if defined(SUPPORT_CUSTOM_SWAP_OPERATIONS)

	if(psDCInfo->psFuncTable->pfnQuerySwapCommandID != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_PVRSRVSwapToDCSystemKM, __DtStep_4)
		psDCInfo->psFuncTable->pfnQuerySwapCommandID(psDCInfo->hExtDevice,
													 psSwapChain->hExtSwapChain,
													 psDCInfo->sSystemBuffer.sDeviceClassBuffer.hExtBuffer, 
													 0, 
													 &ui16SwapCommandID,
													 &bAddReferenceToLast);
		
	}

#endif

	
	apsSrcSync[0] = psDCInfo->sSystemBuffer.sDeviceClassBuffer.psKernelSyncInfo;
	


	if(bAddReferenceToLast && psSwapChain->psLastFlipBuffer)
	{
		
		__DtTestPoint(__DtFunc_PVRSRVSwapToDCSystemKM, __DtStep_5)
		if (apsSrcSync[0] != psSwapChain->psLastFlipBuffer->sDeviceClassBuffer.psKernelSyncInfo)
		{
			__DtTestPoint(__DtFunc_PVRSRVSwapToDCSystemKM, __DtStep_6)
			apsSrcSync[1] = psSwapChain->psLastFlipBuffer->sDeviceClassBuffer.psKernelSyncInfo;
			


			ui32NumSrcSyncs++;
		}
	}

	
	eError = PVRSRVInsertCommandKM (psQueue,
									&psCommand,
									psDCInfo->ui32DeviceID,
									ui16SwapCommandID,
									0,
									IMG_NULL,
									ui32NumSrcSyncs,
									apsSrcSync,
									sizeof(DISPLAYCLASS_FLIP_COMMAND));
	if(eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVSwapToDCSystemKM, __DtStep_7)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVSwapToDCSystemKM: Failed to get space in queue"));
		goto Exit;
	}

	
	psFlipCmd = (DISPLAYCLASS_FLIP_COMMAND*)psCommand->pvData;

	
	psFlipCmd->hExtDevice = psDCInfo->hExtDevice;

	
	psFlipCmd->hExtSwapChain = psSwapChain->hExtSwapChain;

	
	psFlipCmd->hExtBuffer = psDCInfo->sSystemBuffer.sDeviceClassBuffer.hExtBuffer;

	
	psFlipCmd->hPrivateTag = IMG_NULL;

	
	psFlipCmd->ui32ClipRectCount = 0;

	psFlipCmd->ui32SwapInterval = 1;

	
	eError = PVRSRVSubmitCommandKM (psQueue, psCommand);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVSwapToDCSystemKM, __DtStep_8)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVSwapToDCSystemKM: Failed to submit command"));
		goto Exit;
	}

	






	 
	LOOP_UNTIL_TIMEOUT(MAX_HW_TIME_US)
	{
		if(PVRSRVProcessQueues(KERNEL_ID, IMG_FALSE) != PVRSRV_ERROR_PROCESSING_BLOCKED)
		{
			__DtTestPoint(__DtFunc_PVRSRVSwapToDCSystemKM, __DtStep_9)
			goto ProcessedQueues;
		}

		OSWaitus(MAX_HW_TIME_US/WAIT_TRY_COUNT);
	} END_LOOP_UNTIL_TIMEOUT();

	PVR_DPF((PVR_DBG_ERROR,"PVRSRVSwapToDCSystemKM: Failed to process queues"));
	eError = PVRSRV_ERROR_FAILED_TO_PROCESS_QUEUE;
	goto Exit;

ProcessedQueues:
	
	psSwapChain->psLastFlipBuffer = &psDCInfo->sSystemBuffer;

	eError = PVRSRV_OK;

Exit:

	if(eError == PVRSRV_ERROR_CANNOT_GET_QUEUE_SPACE)
	{
		__DtTestPoint(__DtFunc_PVRSRVSwapToDCSystemKM, __DtStep_10)
		eError = PVRSRV_ERROR_RETRY;
	}

#if defined(SUPPORT_LMA)
	PVRSRVPowerUnlock(KERNEL_ID);
#endif
	return eError;
}


static
PVRSRV_ERROR PVRSRVRegisterSystemISRHandler (PFN_ISR_HANDLER	pfnISRHandler,
											 IMG_VOID			*pvISRHandlerData,
											 IMG_UINT32			ui32ISRSourceMask,
											 IMG_UINT32			ui32DeviceID)
{
	SYS_DATA 			*psSysData;
	PVRSRV_DEVICE_NODE	*psDevNode;

	__DtTestPoint(__DtFunc_PVRSRVRegisterSystemISRHandler, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(ui32ISRSourceMask);

	SysAcquireData(&psSysData);

	
	psDevNode = (PVRSRV_DEVICE_NODE*)
				List_PVRSRV_DEVICE_NODE_Any_va(psSysData->psDeviceNodeList,
												&MatchDeviceKM_AnyVaCb,
												ui32DeviceID,
												IMG_TRUE);

	if (psDevNode == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_PVRSRVRegisterSystemISRHandler, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVRegisterSystemISRHandler: Failed to get psDevNode"));
		PVR_DBG_BREAK;
		__DtTestPoint(__DtFunc_PVRSRVRegisterSystemISRHandler, __DtStep_2)
		return PVRSRV_ERROR_NO_DEVICENODE_FOUND;
	}

	
	psDevNode->pvISRData = (IMG_VOID*) pvISRHandlerData;

	
	psDevNode->pfnDeviceISR	= pfnISRHandler;

	__DtTestPoint(__DtFunc_PVRSRVRegisterSystemISRHandler, __DtStep_3)
	return PVRSRV_OK;
}

static
IMG_VOID PVRSRVSetDCState_ForEachVaCb(PVRSRV_DEVICE_NODE *psDeviceNode, va_list va)
{
	PVRSRV_DISPLAYCLASS_INFO *psDCInfo;
	IMG_UINT32 ui32State;
	__DtTestPoint(__DtFunc_PVRSRVSetDCState_ForEachVaCb, __DtStep_0)
	ui32State = va_arg(va, IMG_UINT32);

	if (psDeviceNode->sDevId.eDeviceClass == PVRSRV_DEVICE_CLASS_DISPLAY)
	{
		__DtTestPoint(__DtFunc_PVRSRVSetDCState_ForEachVaCb, __DtStep_1)
		psDCInfo = (PVRSRV_DISPLAYCLASS_INFO *)psDeviceNode->pvDevice;
		if (psDCInfo->psFuncTable->pfnSetDCState && psDCInfo->hExtDevice)
		{
			__DtTestPoint(__DtFunc_PVRSRVSetDCState_ForEachVaCb, __DtStep_2)
			psDCInfo->psFuncTable->pfnSetDCState(psDCInfo->hExtDevice, ui32State);
		}
	}
	__DtTestPoint(__DtFunc_PVRSRVSetDCState_ForEachVaCb, __DtStep_3)
}


IMG_VOID IMG_CALLCONV PVRSRVSetDCState(IMG_UINT32 ui32State)
{
	SYS_DATA					*psSysData;

	__DtTestPoint(__DtFunc_PVRSRVSetDCState, __DtStep_0)
	SysAcquireData(&psSysData);

	List_PVRSRV_DEVICE_NODE_ForEach_va(psSysData->psDeviceNodeList,
										&PVRSRVSetDCState_ForEachVaCb,
										ui32State);
	__DtTestPoint(__DtFunc_PVRSRVSetDCState, __DtStep_1)
}


IMG_EXPORT
IMG_BOOL PVRGetDisplayClassJTable(PVRSRV_DC_DISP2SRV_KMJTABLE *psJTable)
{
	__DtTestPoint(__DtFunc_PVRGetDisplayClassJTable, __DtStep_0)
	psJTable->ui32TableSize = sizeof(PVRSRV_DC_DISP2SRV_KMJTABLE);
	psJTable->pfnPVRSRVRegisterDCDevice = &PVRSRVRegisterDCDeviceKM;
	psJTable->pfnPVRSRVRemoveDCDevice = &PVRSRVRemoveDCDeviceKM;
	psJTable->pfnPVRSRVOEMFunction = &SysOEMFunction;
	psJTable->pfnPVRSRVRegisterCmdProcList = &PVRSRVRegisterCmdProcListKM;
	psJTable->pfnPVRSRVRemoveCmdProcList = &PVRSRVRemoveCmdProcListKM;
#if defined(SUPPORT_MISR_IN_THREAD)
        psJTable->pfnPVRSRVCmdComplete = &OSVSyncMISR;
#else
        psJTable->pfnPVRSRVCmdComplete = &PVRSRVCommandCompleteKM;
#endif
	psJTable->pfnPVRSRVRegisterSystemISRHandler = &PVRSRVRegisterSystemISRHandler;
	psJTable->pfnPVRSRVRegisterPowerDevice = &PVRSRVRegisterPowerDevice;
#if defined(SUPPORT_CUSTOM_SWAP_OPERATIONS)
	psJTable->pfnPVRSRVFreeCmdCompletePacket = &PVRSRVFreeCommandCompletePacketKM;
#endif

	return IMG_TRUE;
}



IMG_EXPORT
PVRSRV_ERROR PVRSRVCloseBCDeviceKM (IMG_HANDLE	hDeviceKM,
									IMG_BOOL	bResManCallback)
{
	PVRSRV_ERROR eError;
	PVRSRV_BUFFERCLASS_PERCONTEXT_INFO *psBCPerContextInfo;

	__DtTestPoint(__DtFunc_PVRSRVCloseBCDeviceKM, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(bResManCallback);

	psBCPerContextInfo = (PVRSRV_BUFFERCLASS_PERCONTEXT_INFO *)hDeviceKM;

	
	eError = ResManFreeResByPtr(psBCPerContextInfo->hResItem);

	__DtTestPoint(__DtFunc_PVRSRVCloseBCDeviceKM, __DtStep_1)
	return eError;
}


static PVRSRV_ERROR CloseBCDeviceCallBack(IMG_PVOID		pvParam,
										  IMG_UINT32	ui32Param)
{
	PVRSRV_BUFFERCLASS_PERCONTEXT_INFO *psBCPerContextInfo;
	PVRSRV_BUFFERCLASS_INFO *psBCInfo;

	__DtTestPoint(__DtFunc_CloseBCDeviceCallBack, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(ui32Param);

	psBCPerContextInfo = (PVRSRV_BUFFERCLASS_PERCONTEXT_INFO *)pvParam;
	psBCInfo = psBCPerContextInfo->psBCInfo;

	psBCInfo->ui32RefCount--;
	if(psBCInfo->ui32RefCount == 0)
	{
		IMG_UINT32 i;

		
		__DtTestPoint(__DtFunc_CloseBCDeviceCallBack, __DtStep_1)
		psBCInfo->psFuncTable->pfnCloseBCDevice(psBCInfo->ui32DeviceID, psBCInfo->hExtDevice);

		
		for(i=0; i<psBCInfo->ui32BufferCount; i++)
		{
			__DtTestPoint(__DtFunc_CloseBCDeviceCallBack, __DtStep_2)
			if(psBCInfo->psBuffer[i].sDeviceClassBuffer.psKernelSyncInfo)
			{
				__DtTestPoint(__DtFunc_CloseBCDeviceCallBack, __DtStep_3)
				if (--psBCInfo->psBuffer[i].sDeviceClassBuffer.psKernelSyncInfo->ui32RefCount == 0)
				{
					__DtTestPoint(__DtFunc_CloseBCDeviceCallBack, __DtStep_4)
					PVRSRVFreeSyncInfoKM(psBCInfo->psBuffer[i].sDeviceClassBuffer.psKernelSyncInfo);
				}
			}
		}

		
		if(psBCInfo->psBuffer)
		{
			__DtTestPoint(__DtFunc_CloseBCDeviceCallBack, __DtStep_5)
			OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(PVRSRV_BC_BUFFER) * psBCInfo->ui32BufferCount, psBCInfo->psBuffer, IMG_NULL);
			psBCInfo->psBuffer = IMG_NULL;
		}
	}

	OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(PVRSRV_BUFFERCLASS_PERCONTEXT_INFO), psBCPerContextInfo, IMG_NULL);
	

	__DtTestPoint(__DtFunc_CloseBCDeviceCallBack, __DtStep_6)
	return PVRSRV_OK;
}


IMG_EXPORT
PVRSRV_ERROR PVRSRVOpenBCDeviceKM (PVRSRV_PER_PROCESS_DATA	*psPerProc,
								   IMG_UINT32				ui32DeviceID,
								   IMG_HANDLE				hDevCookie,
								   IMG_HANDLE				*phDeviceKM)
{
	PVRSRV_BUFFERCLASS_INFO	*psBCInfo;
	PVRSRV_BUFFERCLASS_PERCONTEXT_INFO	*psBCPerContextInfo;
	PVRSRV_DEVICE_NODE		*psDeviceNode;
	SYS_DATA 				*psSysData;
	IMG_UINT32 				i;
	PVRSRV_ERROR			eError;

	__DtTestPoint(__DtFunc_PVRSRVOpenBCDeviceKM, __DtStep_0)
	if(!phDeviceKM || !hDevCookie)
	{
		__DtTestPoint(__DtFunc_PVRSRVOpenBCDeviceKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVOpenBCDeviceKM: Invalid params"));
		__DtTestPoint(__DtFunc_PVRSRVOpenBCDeviceKM, __DtStep_2)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	SysAcquireData(&psSysData);

	
	psDeviceNode = (PVRSRV_DEVICE_NODE*)
			List_PVRSRV_DEVICE_NODE_Any_va(psSysData->psDeviceNodeList,
										   &MatchDeviceKM_AnyVaCb,
										   ui32DeviceID,
										   IMG_FALSE,
										   PVRSRV_DEVICE_CLASS_BUFFER);
	if (!psDeviceNode)
	{
		__DtTestPoint(__DtFunc_PVRSRVOpenBCDeviceKM, __DtStep_3)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVOpenBCDeviceKM: No devnode matching index %d", ui32DeviceID));
		__DtTestPoint(__DtFunc_PVRSRVOpenBCDeviceKM, __DtStep_4)
		return PVRSRV_ERROR_NO_DEVICENODE_FOUND;
	}
	psBCInfo = (PVRSRV_BUFFERCLASS_INFO*)psDeviceNode->pvDevice;

	


	if(OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP,
				  sizeof(*psBCPerContextInfo),
				  (IMG_VOID **)&psBCPerContextInfo, IMG_NULL,
				  "Buffer Class per Context Info") != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVOpenBCDeviceKM, __DtStep_5)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVOpenBCDeviceKM: Failed psBCPerContextInfo alloc"));
		__DtTestPoint(__DtFunc_PVRSRVOpenBCDeviceKM, __DtStep_6)
		return PVRSRV_ERROR_OUT_OF_MEMORY;
	}
	OSMemSet(psBCPerContextInfo, 0, sizeof(*psBCPerContextInfo));

	if(psBCInfo->ui32RefCount++ == 0)
	{
		BUFFER_INFO sBufferInfo;

		__DtTestPoint(__DtFunc_PVRSRVOpenBCDeviceKM, __DtStep_7)
		psDeviceNode = (PVRSRV_DEVICE_NODE *)hDevCookie;

		
		psBCInfo->hDevMemContext = (IMG_HANDLE)psDeviceNode->sDevMemoryInfo.pBMKernelContext;

		
		eError = psBCInfo->psFuncTable->pfnOpenBCDevice(ui32DeviceID, &psBCInfo->hExtDevice);
		if(eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PVRSRVOpenBCDeviceKM, __DtStep_8)
			PVR_DPF((PVR_DBG_ERROR,"PVRSRVOpenBCDeviceKM: Failed to open external BC device"));
			__DtTestPoint(__DtFunc_PVRSRVOpenBCDeviceKM, __DtStep_9)
			return eError;
		}

		
		eError = psBCInfo->psFuncTable->pfnGetBCInfo(psBCInfo->hExtDevice, &sBufferInfo);
		if(eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PVRSRVOpenBCDeviceKM, __DtStep_10)
			PVR_DPF((PVR_DBG_ERROR,"PVRSRVOpenBCDeviceKM : Failed to get BC Info"));
			__DtTestPoint(__DtFunc_PVRSRVOpenBCDeviceKM, __DtStep_11)
			return eError;
		}

		
		psBCInfo->ui32BufferCount = sBufferInfo.ui32BufferCount;
		

		
		eError = OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP,
							  sizeof(PVRSRV_BC_BUFFER) * sBufferInfo.ui32BufferCount,
							  (IMG_VOID **)&psBCInfo->psBuffer,
						 	  IMG_NULL,
							  "Array of Buffer Class Buffer");
		if(eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PVRSRVOpenBCDeviceKM, __DtStep_12)
			PVR_DPF((PVR_DBG_ERROR,"PVRSRVOpenBCDeviceKM: Failed to allocate BC buffers"));
			__DtTestPoint(__DtFunc_PVRSRVOpenBCDeviceKM, __DtStep_13)
			return eError;
		}
		OSMemSet (psBCInfo->psBuffer,
					0,
					sizeof(PVRSRV_BC_BUFFER) * sBufferInfo.ui32BufferCount);

		for(i=0; i<psBCInfo->ui32BufferCount; i++)
		{
			
			__DtTestPoint(__DtFunc_PVRSRVOpenBCDeviceKM, __DtStep_14)
			eError = PVRSRVAllocSyncInfoKM(IMG_NULL,
										psBCInfo->hDevMemContext,
										&psBCInfo->psBuffer[i].sDeviceClassBuffer.psKernelSyncInfo);
			if(eError != PVRSRV_OK)
			{
				__DtTestPoint(__DtFunc_PVRSRVOpenBCDeviceKM, __DtStep_15)
				PVR_DPF((PVR_DBG_ERROR,"PVRSRVOpenBCDeviceKM: Failed sync info alloc"));
				goto ErrorExit;
			}

			psBCInfo->psBuffer[i].sDeviceClassBuffer.psKernelSyncInfo->ui32RefCount++;
			
			


			eError = psBCInfo->psFuncTable->pfnGetBCBuffer(psBCInfo->hExtDevice,
															i,
															psBCInfo->psBuffer[i].sDeviceClassBuffer.psKernelSyncInfo->psSyncData,
															&psBCInfo->psBuffer[i].sDeviceClassBuffer.hExtBuffer);
			if(eError != PVRSRV_OK)
			{
				__DtTestPoint(__DtFunc_PVRSRVOpenBCDeviceKM, __DtStep_16)
				PVR_DPF((PVR_DBG_ERROR,"PVRSRVOpenBCDeviceKM: Failed to get BC buffers"));
				goto ErrorExit;
			}

			
			psBCInfo->psBuffer[i].sDeviceClassBuffer.pfnGetBufferAddr = psBCInfo->psFuncTable->pfnGetBufferAddr;
			psBCInfo->psBuffer[i].sDeviceClassBuffer.hDevMemContext = psBCInfo->hDevMemContext;
			psBCInfo->psBuffer[i].sDeviceClassBuffer.hExtDevice = psBCInfo->hExtDevice;
		}
	}

	psBCPerContextInfo->psBCInfo = psBCInfo;
	psBCPerContextInfo->hResItem = ResManRegisterRes(psPerProc->hResManContext,
													 RESMAN_TYPE_BUFFERCLASS_DEVICE,
													 psBCPerContextInfo,
													 0,
													 &CloseBCDeviceCallBack);

	
	*phDeviceKM = (IMG_HANDLE)psBCPerContextInfo;

	__DtTestPoint(__DtFunc_PVRSRVOpenBCDeviceKM, __DtStep_17)
	return PVRSRV_OK;

ErrorExit:

	
	for(i=0; i<psBCInfo->ui32BufferCount; i++)
	{
		__DtTestPoint(__DtFunc_PVRSRVOpenBCDeviceKM, __DtStep_18)
		if(psBCInfo->psBuffer[i].sDeviceClassBuffer.psKernelSyncInfo)
		{
			__DtTestPoint(__DtFunc_PVRSRVOpenBCDeviceKM, __DtStep_19)
			if (--psBCInfo->psBuffer[i].sDeviceClassBuffer.psKernelSyncInfo->ui32RefCount == 0)
			{
				__DtTestPoint(__DtFunc_PVRSRVOpenBCDeviceKM, __DtStep_20)
				PVRSRVFreeSyncInfoKM(psBCInfo->psBuffer[i].sDeviceClassBuffer.psKernelSyncInfo);
			}
		}
	}

	
	if(psBCInfo->psBuffer)
	{
		__DtTestPoint(__DtFunc_PVRSRVOpenBCDeviceKM, __DtStep_21)
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(PVRSRV_BC_BUFFER), psBCInfo->psBuffer, IMG_NULL);
		psBCInfo->psBuffer = IMG_NULL;
	}

	__DtTestPoint(__DtFunc_PVRSRVOpenBCDeviceKM, __DtStep_22)
	return eError;
}




IMG_EXPORT
PVRSRV_ERROR PVRSRVGetBCInfoKM (IMG_HANDLE hDeviceKM,
								BUFFER_INFO *psBufferInfo)
{
	PVRSRV_BUFFERCLASS_INFO *psBCInfo;
	PVRSRV_ERROR 			eError;

	__DtTestPoint(__DtFunc_PVRSRVGetBCInfoKM, __DtStep_0)
	if(!hDeviceKM || !psBufferInfo)
	{
		__DtTestPoint(__DtFunc_PVRSRVGetBCInfoKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVGetBCInfoKM: Invalid parameters"));
		__DtTestPoint(__DtFunc_PVRSRVGetBCInfoKM, __DtStep_2)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	psBCInfo = BCDeviceHandleToBCInfo(hDeviceKM);

	eError = psBCInfo->psFuncTable->pfnGetBCInfo(psBCInfo->hExtDevice, psBufferInfo);

	if(eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVGetBCInfoKM, __DtStep_3)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVGetBCInfoKM : Failed to get BC Info"));
		__DtTestPoint(__DtFunc_PVRSRVGetBCInfoKM, __DtStep_4)
		return eError;
	}

	__DtTestPoint(__DtFunc_PVRSRVGetBCInfoKM, __DtStep_5)
	return PVRSRV_OK;
}


IMG_EXPORT
PVRSRV_ERROR PVRSRVGetBCBufferKM (IMG_HANDLE hDeviceKM,
								  IMG_UINT32 ui32BufferIndex,
								  IMG_HANDLE *phBuffer)
{
	PVRSRV_BUFFERCLASS_INFO *psBCInfo;

	__DtTestPoint(__DtFunc_PVRSRVGetBCBufferKM, __DtStep_0)
	if(!hDeviceKM || !phBuffer)
	{
		__DtTestPoint(__DtFunc_PVRSRVGetBCBufferKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVGetBCBufferKM: Invalid parameters"));
		__DtTestPoint(__DtFunc_PVRSRVGetBCBufferKM, __DtStep_2)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	psBCInfo = BCDeviceHandleToBCInfo(hDeviceKM);

	if(ui32BufferIndex < psBCInfo->ui32BufferCount)
	{
		__DtTestPoint(__DtFunc_PVRSRVGetBCBufferKM, __DtStep_3)
		*phBuffer = (IMG_HANDLE)&psBCInfo->psBuffer[ui32BufferIndex];
	}
	else
	{
		__DtTestPoint(__DtFunc_PVRSRVGetBCBufferKM, __DtStep_4)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVGetBCBufferKM: Buffer index %d out of range (%d)", ui32BufferIndex,psBCInfo->ui32BufferCount));
		__DtTestPoint(__DtFunc_PVRSRVGetBCBufferKM, __DtStep_5)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	__DtTestPoint(__DtFunc_PVRSRVGetBCBufferKM, __DtStep_6)
	return PVRSRV_OK;
}


IMG_EXPORT
IMG_BOOL PVRGetBufferClassJTable(PVRSRV_BC_BUFFER2SRV_KMJTABLE *psJTable)
{
	__DtTestPoint(__DtFunc_PVRGetBufferClassJTable, __DtStep_0)
	psJTable->ui32TableSize = sizeof(PVRSRV_BC_BUFFER2SRV_KMJTABLE);

	psJTable->pfnPVRSRVRegisterBCDevice = &PVRSRVRegisterBCDeviceKM;
	psJTable->pfnPVRSRVScheduleDevices = &PVRSRVScheduleDevicesKM;
	psJTable->pfnPVRSRVRemoveBCDevice = &PVRSRVRemoveBCDeviceKM;

	__DtTestPoint(__DtFunc_PVRGetBufferClassJTable, __DtStep_1)
	return IMG_TRUE;
}

