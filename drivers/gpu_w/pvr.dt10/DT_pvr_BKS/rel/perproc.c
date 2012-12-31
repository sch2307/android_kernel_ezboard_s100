#include	"DT_perproc.h"	/* For DynamicTracer-TestPoint */
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
#include "resman.h"
#include "handle.h"
#include "perproc.h"
#include "osperproc.h"

#define	HASH_TAB_INIT_SIZE 32

static HASH_TABLE *psHashTab = IMG_NULL;

static PVRSRV_ERROR FreePerProcessData(PVRSRV_PER_PROCESS_DATA *psPerProc)
{
	PVRSRV_ERROR eError;
	IMG_UINTPTR_T uiPerProc;

	__DtTestPoint(__DtFunc_FreePerProcessData, __DtStep_0)
	PVR_ASSERT(psPerProc != IMG_NULL);

	if (psPerProc == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_FreePerProcessData, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "FreePerProcessData: invalid parameter"));
		__DtTestPoint(__DtFunc_FreePerProcessData, __DtStep_2)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	uiPerProc = HASH_Remove(psHashTab, (IMG_UINTPTR_T)psPerProc->ui32PID);
	if (uiPerProc == 0)
	{
		__DtTestPoint(__DtFunc_FreePerProcessData, __DtStep_3)
		PVR_DPF((PVR_DBG_ERROR, "FreePerProcessData: Couldn't find process in per-process data hash table"));
		
		PVR_ASSERT(psPerProc->ui32PID == 0);
	}
	else
	{
		__DtTestPoint(__DtFunc_FreePerProcessData, __DtStep_4)
		PVR_ASSERT((PVRSRV_PER_PROCESS_DATA *)uiPerProc == psPerProc);
		PVR_ASSERT(((PVRSRV_PER_PROCESS_DATA *)uiPerProc)->ui32PID == psPerProc->ui32PID);
	}

	
	if (psPerProc->psHandleBase != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_FreePerProcessData, __DtStep_5)
		eError = PVRSRVFreeHandleBase(psPerProc->psHandleBase);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_FreePerProcessData, __DtStep_6)
			PVR_DPF((PVR_DBG_ERROR, "FreePerProcessData: Couldn't free handle base for process (%d)", eError));
			__DtTestPoint(__DtFunc_FreePerProcessData, __DtStep_7)
			return eError;
		}
	}

	
	if (psPerProc->hPerProcData != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_FreePerProcessData, __DtStep_8)
		eError = PVRSRVReleaseHandle(KERNEL_HANDLE_BASE, psPerProc->hPerProcData, PVRSRV_HANDLE_TYPE_PERPROC_DATA);

		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_FreePerProcessData, __DtStep_9)
			PVR_DPF((PVR_DBG_ERROR, "FreePerProcessData: Couldn't release per-process data handle (%d)", eError));
			__DtTestPoint(__DtFunc_FreePerProcessData, __DtStep_10)
			return eError;
		}
	}

	
	eError = OSPerProcessPrivateDataDeInit(psPerProc->hOsPrivateData);
	if (eError != PVRSRV_OK)
	{
		 __DtTestPoint(__DtFunc_FreePerProcessData, __DtStep_11)
		 PVR_DPF((PVR_DBG_ERROR, "FreePerProcessData: OSPerProcessPrivateDataDeInit failed (%d)", eError));
		__DtTestPoint(__DtFunc_FreePerProcessData, __DtStep_12)
		return eError;
	}

	eError = OSFreeMem(PVRSRV_OS_NON_PAGEABLE_HEAP,
		sizeof(*psPerProc),
		psPerProc,
		psPerProc->hBlockAlloc);
	
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_FreePerProcessData, __DtStep_13)
		PVR_DPF((PVR_DBG_ERROR, "FreePerProcessData: Couldn't free per-process data (%d)", eError));
		__DtTestPoint(__DtFunc_FreePerProcessData, __DtStep_14)
		return eError;
	}

	__DtTestPoint(__DtFunc_FreePerProcessData, __DtStep_15)
	return PVRSRV_OK;
}


PVRSRV_PER_PROCESS_DATA *PVRSRVPerProcessData(IMG_UINT32 ui32PID)
{
	PVRSRV_PER_PROCESS_DATA *psPerProc;

	__DtTestPoint(__DtFunc_PVRSRVPerProcessData, __DtStep_0)
	PVR_ASSERT(psHashTab != IMG_NULL);

	
	psPerProc = (PVRSRV_PER_PROCESS_DATA *)HASH_Retrieve(psHashTab, (IMG_UINTPTR_T)ui32PID);
	__DtTestPoint(__DtFunc_PVRSRVPerProcessData, __DtStep_1)
	return psPerProc;
}


PVRSRV_ERROR PVRSRVPerProcessDataConnect(IMG_UINT32	ui32PID, IMG_UINT32 ui32Flags)
{
	PVRSRV_PER_PROCESS_DATA *psPerProc;
	IMG_HANDLE hBlockAlloc;
	PVRSRV_ERROR eError = PVRSRV_OK;

	__DtTestPoint(__DtFunc_PVRSRVPerProcessDataConnect, __DtStep_0)
	PVR_ASSERT(psHashTab != IMG_NULL);

	
	psPerProc = (PVRSRV_PER_PROCESS_DATA *)HASH_Retrieve(psHashTab, (IMG_UINTPTR_T)ui32PID);

	if (psPerProc == IMG_NULL)
	{
		
		__DtTestPoint(__DtFunc_PVRSRVPerProcessDataConnect, __DtStep_1)
		eError = OSAllocMem(PVRSRV_OS_NON_PAGEABLE_HEAP,
							sizeof(*psPerProc),
							(IMG_PVOID *)&psPerProc,
							&hBlockAlloc,
							"Per Process Data");
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PVRSRVPerProcessDataConnect, __DtStep_2)
			PVR_DPF((PVR_DBG_ERROR, "PVRSRVPerProcessDataConnect: Couldn't allocate per-process data (%d)", eError));
			__DtTestPoint(__DtFunc_PVRSRVPerProcessDataConnect, __DtStep_3)
			return eError;
		}
		OSMemSet(psPerProc, 0, sizeof(*psPerProc));
		psPerProc->hBlockAlloc = hBlockAlloc;

		if (!HASH_Insert(psHashTab, (IMG_UINTPTR_T)ui32PID, (IMG_UINTPTR_T)psPerProc))
		{
			__DtTestPoint(__DtFunc_PVRSRVPerProcessDataConnect, __DtStep_4)
			PVR_DPF((PVR_DBG_ERROR, "PVRSRVPerProcessDataConnect: Couldn't insert per-process data into hash table"));
			eError = PVRSRV_ERROR_INSERT_HASH_TABLE_DATA_FAILED;
			goto failure;
		}

		psPerProc->ui32PID = ui32PID;
		psPerProc->ui32RefCount = 0;

#if defined(SUPPORT_PDUMP_MULTI_PROCESS)
		if (ui32Flags == SRV_FLAGS_PDUMP_ACTIVE)
		{
			__DtTestPoint(__DtFunc_PVRSRVPerProcessDataConnect, __DtStep_5)
			psPerProc->bPDumpActive = IMG_TRUE;
		}
#else
		PVR_UNREFERENCED_PARAMETER(ui32Flags); 
#endif

		
		eError = OSPerProcessPrivateDataInit(&psPerProc->hOsPrivateData);
		if (eError != PVRSRV_OK)
		{
			 __DtTestPoint(__DtFunc_PVRSRVPerProcessDataConnect, __DtStep_6)
			 PVR_DPF((PVR_DBG_ERROR, "PVRSRVPerProcessDataConnect: OSPerProcessPrivateDataInit failed (%d)", eError));
			goto failure;
		}

		
		eError = PVRSRVAllocHandle(KERNEL_HANDLE_BASE,
								   &psPerProc->hPerProcData,
								   psPerProc,
								   PVRSRV_HANDLE_TYPE_PERPROC_DATA,
								   PVRSRV_HANDLE_ALLOC_FLAG_NONE);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PVRSRVPerProcessDataConnect, __DtStep_7)
			PVR_DPF((PVR_DBG_ERROR, "PVRSRVPerProcessDataConnect: Couldn't allocate handle for per-process data (%d)", eError));
			goto failure;
		}

		
		eError = PVRSRVAllocHandleBase(&psPerProc->psHandleBase);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PVRSRVPerProcessDataConnect, __DtStep_8)
			PVR_DPF((PVR_DBG_ERROR, "PVRSRVPerProcessDataConnect: Couldn't allocate handle base for process (%d)", eError));
			goto failure;
		}

		
		eError = OSPerProcessSetHandleOptions(psPerProc->psHandleBase);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PVRSRVPerProcessDataConnect, __DtStep_9)
			PVR_DPF((PVR_DBG_ERROR, "PVRSRVPerProcessDataConnect: Couldn't set handle options (%d)", eError));
			goto failure;
		}
		
		
		eError = PVRSRVResManConnect(psPerProc, &psPerProc->hResManContext);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PVRSRVPerProcessDataConnect, __DtStep_10)
			PVR_DPF((PVR_DBG_ERROR, "PVRSRVPerProcessDataConnect: Couldn't register with the resource manager"));
			goto failure;
		}
	}
	
	psPerProc->ui32RefCount++;
	PVR_DPF((PVR_DBG_MESSAGE,
			"PVRSRVPerProcessDataConnect: Process 0x%x has ref-count %d",
			ui32PID, psPerProc->ui32RefCount));

	__DtTestPoint(__DtFunc_PVRSRVPerProcessDataConnect, __DtStep_11)
	return eError;

failure:
	(IMG_VOID)FreePerProcessData(psPerProc);
	__DtTestPoint(__DtFunc_PVRSRVPerProcessDataConnect, __DtStep_12)
	return eError;
}


IMG_VOID PVRSRVPerProcessDataDisconnect(IMG_UINT32	ui32PID)
{
	PVRSRV_ERROR eError;
	PVRSRV_PER_PROCESS_DATA *psPerProc;

	__DtTestPoint(__DtFunc_PVRSRVPerProcessDataDisconnect, __DtStep_0)
	PVR_ASSERT(psHashTab != IMG_NULL);

	psPerProc = (PVRSRV_PER_PROCESS_DATA *)HASH_Retrieve(psHashTab, (IMG_UINTPTR_T)ui32PID);
	if (psPerProc == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_PVRSRVPerProcessDataDisconnect, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "PVRSRVPerProcessDataDealloc: Couldn't locate per-process data for PID %u", ui32PID));
	}
	else
	{
		__DtTestPoint(__DtFunc_PVRSRVPerProcessDataDisconnect, __DtStep_2)
		psPerProc->ui32RefCount--;
		if (psPerProc->ui32RefCount == 0)
		{
			__DtTestPoint(__DtFunc_PVRSRVPerProcessDataDisconnect, __DtStep_3)
			PVR_DPF((PVR_DBG_MESSAGE, "PVRSRVPerProcessDataDisconnect: "
					"Last close from process 0x%x received", ui32PID));

			
			PVRSRVResManDisconnect(psPerProc->hResManContext, IMG_FALSE);
			
			
			eError = FreePerProcessData(psPerProc);
			if (eError != PVRSRV_OK)
			{
				__DtTestPoint(__DtFunc_PVRSRVPerProcessDataDisconnect, __DtStep_4)
				PVR_DPF((PVR_DBG_ERROR, "PVRSRVPerProcessDataDisconnect: Error freeing per-process data"));
			}
		}
	}

	eError = PVRSRVPurgeHandles(KERNEL_HANDLE_BASE);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVPerProcessDataDisconnect, __DtStep_5)
		PVR_DPF((PVR_DBG_ERROR, "PVRSRVPerProcessDataDisconnect: Purge of global handle pool failed (%d)", eError));
	}
	__DtTestPoint(__DtFunc_PVRSRVPerProcessDataDisconnect, __DtStep_6)
}


PVRSRV_ERROR PVRSRVPerProcessDataInit(IMG_VOID)
{
	__DtTestPoint(__DtFunc_PVRSRVPerProcessDataInit, __DtStep_0)
	PVR_ASSERT(psHashTab == IMG_NULL);

	
	psHashTab = HASH_Create(HASH_TAB_INIT_SIZE);
	if (psHashTab == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_PVRSRVPerProcessDataInit, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "PVRSRVPerProcessDataInit: Couldn't create per-process data hash table"));
		__DtTestPoint(__DtFunc_PVRSRVPerProcessDataInit, __DtStep_2)
		return PVRSRV_ERROR_UNABLE_TO_CREATE_HASH_TABLE;
	}

	__DtTestPoint(__DtFunc_PVRSRVPerProcessDataInit, __DtStep_3)
	return PVRSRV_OK;
}

PVRSRV_ERROR PVRSRVPerProcessDataDeInit(IMG_VOID)
{
	
	__DtTestPoint(__DtFunc_PVRSRVPerProcessDataDeInit, __DtStep_0)
	if (psHashTab != IMG_NULL)
	{
		
		__DtTestPoint(__DtFunc_PVRSRVPerProcessDataDeInit, __DtStep_1)
		HASH_Delete(psHashTab);
		psHashTab = IMG_NULL;
	}

	__DtTestPoint(__DtFunc_PVRSRVPerProcessDataDeInit, __DtStep_2)
	return PVRSRV_OK;
}

