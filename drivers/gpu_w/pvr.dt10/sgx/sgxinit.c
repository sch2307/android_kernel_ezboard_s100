#include	"DT_sgxinit.h"	/* For DynamicTracer-TestPoint */
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

#include "sgxdefs.h"
#include "sgxmmu.h"
#include "services_headers.h"
#include "buffer_manager.h"
#include "sgxapi_km.h"
#include "sgxinfo.h"
#include "sgx_mkif_km.h"
#include "sgxconfig.h"
#include "sysconfig.h"
#include "pvr_bridge_km.h"

#include "sgx_bridge_km.h"

#include "pdump_km.h"
#include "ra.h"
#include "mmu.h"
#include "handle.h"
#include "perproc.h"

#include "sgxutils.h"
#include "pvrversion.h"
#include "sgx_options.h"

#include "lists.h"
#include "srvkm.h"

#define VAR(x) #x

 
#define CHECK_SIZE(NAME) \
{	\
	if (psSGXStructSizes->ui32Sizeof_##NAME != psDevInfo->sSGXStructSizes.ui32Sizeof_##NAME) \
	{	\
		PVR_DPF((PVR_DBG_ERROR, "SGXDevInitCompatCheck: Size check failed for SGXMKIF_%s (client) = %d bytes, (ukernel) = %d bytes\n", \
			VAR(NAME), \
			psDevInfo->sSGXStructSizes.ui32Sizeof_##NAME, \
			psSGXStructSizes->ui32Sizeof_##NAME )); \
		bStructSizesFailed = IMG_TRUE; \
	}	\
}

#if defined (SYS_USING_INTERRUPTS)
IMG_BOOL SGX_ISRHandler(IMG_VOID *pvData);
#endif


static
PVRSRV_ERROR SGXGetMiscInfoUkernel(PVRSRV_SGXDEV_INFO	*psDevInfo,
								   PVRSRV_DEVICE_NODE 	*psDeviceNode);
#if defined(PDUMP)
static
PVRSRV_ERROR SGXResetPDump(PVRSRV_DEVICE_NODE *psDeviceNode);
#endif

static IMG_VOID SGXCommandComplete(PVRSRV_DEVICE_NODE *psDeviceNode)
{
#if defined(OS_SUPPORTS_IN_LISR)
	if (OSInLISR(psDeviceNode->psSysData))
	{
		
		__DtTestPoint(__DtFunc_SGXCommandComplete, __DtStep_0)
		psDeviceNode->bReProcessDeviceCommandComplete = IMG_TRUE;
	}
	else
	{
		__DtTestPoint(__DtFunc_SGXCommandComplete, __DtStep_1)
		SGXScheduleProcessQueuesKM(psDeviceNode);
	}
#else
	SGXScheduleProcessQueuesKM(psDeviceNode);
#endif
	__DtTestPoint(__DtFunc_SGXCommandComplete, __DtStep_2)
}

static IMG_UINT32 DeinitDevInfo(PVRSRV_SGXDEV_INFO *psDevInfo)
{
	__DtTestPoint(__DtFunc_DeinitDevInfo, __DtStep_0)
	if (psDevInfo->psKernelCCBInfo != IMG_NULL)
	{
		

		__DtTestPoint(__DtFunc_DeinitDevInfo, __DtStep_1)
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(PVRSRV_SGX_CCB_INFO), psDevInfo->psKernelCCBInfo, IMG_NULL);
	}

	__DtTestPoint(__DtFunc_DeinitDevInfo, __DtStep_2)
	return PVRSRV_OK;
}

static PVRSRV_ERROR InitDevInfo(PVRSRV_PER_PROCESS_DATA *psPerProc,
								PVRSRV_DEVICE_NODE *psDeviceNode,
								SGX_BRIDGE_INIT_INFO *psInitInfo)
{
	PVRSRV_SGXDEV_INFO *psDevInfo = (PVRSRV_SGXDEV_INFO *)psDeviceNode->pvDevice;
	PVRSRV_ERROR		eError;

	PVRSRV_SGX_CCB_INFO	*psKernelCCBInfo = IMG_NULL;

	__DtTestPoint(__DtFunc_InitDevInfo, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(psPerProc);
	psDevInfo->sScripts = psInitInfo->sScripts;

	psDevInfo->psKernelCCBMemInfo = (PVRSRV_KERNEL_MEM_INFO *)psInitInfo->hKernelCCBMemInfo;
	psDevInfo->psKernelCCB = (PVRSRV_SGX_KERNEL_CCB *) psDevInfo->psKernelCCBMemInfo->pvLinAddrKM;

	psDevInfo->psKernelCCBCtlMemInfo = (PVRSRV_KERNEL_MEM_INFO *)psInitInfo->hKernelCCBCtlMemInfo;
	psDevInfo->psKernelCCBCtl = (PVRSRV_SGX_CCB_CTL *) psDevInfo->psKernelCCBCtlMemInfo->pvLinAddrKM;

	psDevInfo->psKernelCCBEventKickerMemInfo = (PVRSRV_KERNEL_MEM_INFO *)psInitInfo->hKernelCCBEventKickerMemInfo;
	psDevInfo->pui32KernelCCBEventKicker = (IMG_UINT32 *)psDevInfo->psKernelCCBEventKickerMemInfo->pvLinAddrKM;

	psDevInfo->psKernelSGXHostCtlMemInfo = (PVRSRV_KERNEL_MEM_INFO *)psInitInfo->hKernelSGXHostCtlMemInfo;
	psDevInfo->psSGXHostCtl = (SGXMKIF_HOST_CTL *)psDevInfo->psKernelSGXHostCtlMemInfo->pvLinAddrKM;

	psDevInfo->psKernelSGXTA3DCtlMemInfo = (PVRSRV_KERNEL_MEM_INFO *)psInitInfo->hKernelSGXTA3DCtlMemInfo;

 	psDevInfo->psKernelSGXMiscMemInfo = (PVRSRV_KERNEL_MEM_INFO *)psInitInfo->hKernelSGXMiscMemInfo;

#if defined(SGX_SUPPORT_HWPROFILING)
	psDevInfo->psKernelHWProfilingMemInfo = (PVRSRV_KERNEL_MEM_INFO *)psInitInfo->hKernelHWProfilingMemInfo;
#endif
#if defined(SUPPORT_SGX_HWPERF)
	psDevInfo->psKernelHWPerfCBMemInfo = (PVRSRV_KERNEL_MEM_INFO *)psInitInfo->hKernelHWPerfCBMemInfo;
#endif
	psDevInfo->psKernelTASigBufferMemInfo = psInitInfo->hKernelTASigBufferMemInfo;
	psDevInfo->psKernel3DSigBufferMemInfo = psInitInfo->hKernel3DSigBufferMemInfo;
#if defined(FIX_HW_BRN_29702)
	psDevInfo->psKernelCFIMemInfo = (PVRSRV_KERNEL_MEM_INFO *)psInitInfo->hKernelCFIMemInfo;
#endif
#if defined(FIX_HW_BRN_29823)
	psDevInfo->psKernelDummyTermStreamMemInfo = (PVRSRV_KERNEL_MEM_INFO *)psInitInfo->hKernelDummyTermStreamMemInfo;
#endif
#if defined(PVRSRV_USSE_EDM_STATUS_DEBUG)
	psDevInfo->psKernelEDMStatusBufferMemInfo = (PVRSRV_KERNEL_MEM_INFO *)psInitInfo->hKernelEDMStatusBufferMemInfo;
#endif
#if defined(SGX_FEATURE_OVERLAPPED_SPM)
	psDevInfo->psKernelTmpRgnHeaderMemInfo = (PVRSRV_KERNEL_MEM_INFO *)psInitInfo->hKernelTmpRgnHeaderMemInfo;
#endif
#if defined(SGX_FEATURE_SPM_MODE_0)
	psDevInfo->psKernelTmpDPMStateMemInfo = (PVRSRV_KERNEL_MEM_INFO *)psInitInfo->hKernelTmpDPMStateMemInfo;
#endif
	
	psDevInfo->ui32ClientBuildOptions = psInitInfo->ui32ClientBuildOptions;

	
	psDevInfo->sSGXStructSizes = psInitInfo->sSGXStructSizes;

	

	eError = OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP,
						sizeof(PVRSRV_SGX_CCB_INFO),
						(IMG_VOID **)&psKernelCCBInfo, 0,
						"SGX Circular Command Buffer Info");
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_InitDevInfo, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"InitDevInfo: Failed to alloc memory"));
		goto failed_allockernelccb;
	}


	OSMemSet(psKernelCCBInfo, 0, sizeof(PVRSRV_SGX_CCB_INFO));
	psKernelCCBInfo->psCCBMemInfo		= psDevInfo->psKernelCCBMemInfo;
	psKernelCCBInfo->psCCBCtlMemInfo	= psDevInfo->psKernelCCBCtlMemInfo;
	psKernelCCBInfo->psCommands			= psDevInfo->psKernelCCB->asCommands;
	psKernelCCBInfo->pui32WriteOffset	= &psDevInfo->psKernelCCBCtl->ui32WriteOffset;
	psKernelCCBInfo->pui32ReadOffset	= &psDevInfo->psKernelCCBCtl->ui32ReadOffset;
	psDevInfo->psKernelCCBInfo = psKernelCCBInfo;

	

	OSMemCopy(psDevInfo->aui32HostKickAddr, psInitInfo->aui32HostKickAddr,
			  SGXMKIF_CMD_MAX * sizeof(psDevInfo->aui32HostKickAddr[0]));

 	psDevInfo->bForcePTOff = IMG_FALSE;

	psDevInfo->ui32CacheControl = psInitInfo->ui32CacheControl;

	psDevInfo->ui32EDMTaskReg0 = psInitInfo->ui32EDMTaskReg0;
	psDevInfo->ui32EDMTaskReg1 = psInitInfo->ui32EDMTaskReg1;
	psDevInfo->ui32ClkGateStatusReg = psInitInfo->ui32ClkGateStatusReg;
	psDevInfo->ui32ClkGateStatusMask = psInitInfo->ui32ClkGateStatusMask;
#if defined(SGX_FEATURE_MP)
	psDevInfo->ui32MasterClkGateStatusReg = psInitInfo->ui32MasterClkGateStatusReg;
	psDevInfo->ui32MasterClkGateStatusMask = psInitInfo->ui32MasterClkGateStatusMask;
	psDevInfo->ui32MasterClkGateStatus2Reg = psInitInfo->ui32MasterClkGateStatus2Reg;
	psDevInfo->ui32MasterClkGateStatus2Mask = psInitInfo->ui32MasterClkGateStatus2Mask;
#endif 


	
	OSMemCopy(&psDevInfo->asSGXDevData, &psInitInfo->asInitDevData, sizeof(psDevInfo->asSGXDevData));

	__DtTestPoint(__DtFunc_InitDevInfo, __DtStep_2)
	return PVRSRV_OK;

failed_allockernelccb:
	DeinitDevInfo(psDevInfo);

	__DtTestPoint(__DtFunc_InitDevInfo, __DtStep_3)
	return eError;
}




static PVRSRV_ERROR SGXRunScript(PVRSRV_SGXDEV_INFO *psDevInfo, SGX_INIT_COMMAND *psScript, IMG_UINT32 ui32NumInitCommands)
{
	IMG_UINT32 ui32PC;
	SGX_INIT_COMMAND *psComm;

	__DtTestPoint(__DtFunc_SGXRunScript, __DtStep_0)
	for (ui32PC = 0, psComm = psScript;
		ui32PC < ui32NumInitCommands;
		ui32PC++, psComm++)
	{
		__DtTestPoint(__DtFunc_SGXRunScript, __DtStep_1)
		switch (psComm->eOp)
		{
			case SGX_INIT_OP_WRITE_HW_REG:
			{
				__DtTestPoint(__DtFunc_SGXRunScript, __DtStep_2)
				OSWriteHWReg(psDevInfo->pvRegsBaseKM, psComm->sWriteHWReg.ui32Offset, psComm->sWriteHWReg.ui32Value);
				PDUMPCOMMENT("SGXRunScript: Write HW reg operation");
				PDUMPREG(SGX_PDUMPREG_NAME, psComm->sWriteHWReg.ui32Offset, psComm->sWriteHWReg.ui32Value);
				break;
			}
#if defined(PDUMP)
			case SGX_INIT_OP_PDUMP_HW_REG:
			{
				__DtTestPoint(__DtFunc_SGXRunScript, __DtStep_3)
				PDUMPCOMMENT("SGXRunScript: Dump HW reg operation");
				PDUMPREG(SGX_PDUMPREG_NAME, psComm->sPDumpHWReg.ui32Offset, psComm->sPDumpHWReg.ui32Value);
				break;
			}
#endif
			case SGX_INIT_OP_HALT:
			{
				__DtTestPoint(__DtFunc_SGXRunScript, __DtStep_4)
				return PVRSRV_OK;
			}
			case SGX_INIT_OP_ILLEGAL:
			
			default:
			{
				__DtTestPoint(__DtFunc_SGXRunScript, __DtStep_5)
				PVR_DPF((PVR_DBG_ERROR,"SGXRunScript: PC %d: Illegal command: %d", ui32PC, psComm->eOp));
				__DtTestPoint(__DtFunc_SGXRunScript, __DtStep_6)
				return PVRSRV_ERROR_UNKNOWN_SCRIPT_OPERATION;
			}
		}

	}

	__DtTestPoint(__DtFunc_SGXRunScript, __DtStep_7)
	return PVRSRV_ERROR_UNKNOWN_SCRIPT_OPERATION;
}

PVRSRV_ERROR SGXInitialise(PVRSRV_SGXDEV_INFO	*psDevInfo,
						   IMG_BOOL				bHardwareRecovery)
{
	PVRSRV_ERROR			eError;
	PVRSRV_KERNEL_MEM_INFO	*psSGXHostCtlMemInfo = psDevInfo->psKernelSGXHostCtlMemInfo;
	SGXMKIF_HOST_CTL		*psSGXHostCtl = psSGXHostCtlMemInfo->pvLinAddrKM;
	static IMG_BOOL			bFirstTime = IMG_TRUE;
#if defined(PDUMP)
	IMG_BOOL				bPDumpIsSuspended = PDumpIsSuspended();
#endif 

	

	PDUMPCOMMENTWITHFLAGS(PDUMP_FLAGS_CONTINUOUS, "SGX initialisation script part 1\n");
	eError = SGXRunScript(psDevInfo, psDevInfo->sScripts.asInitCommandsPart1, SGX_MAX_INIT_COMMANDS);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXInitialise, __DtStep_0)
		PVR_DPF((PVR_DBG_ERROR,"SGXInitialise: SGXRunScript (part 1) failed (%d)", eError));
		__DtTestPoint(__DtFunc_SGXInitialise, __DtStep_1)
		return eError;
	}
	PDUMPCOMMENTWITHFLAGS(PDUMP_FLAGS_CONTINUOUS, "End of SGX initialisation script part 1\n");

	
	SGXReset(psDevInfo, bFirstTime || bHardwareRecovery, PDUMP_FLAGS_CONTINUOUS);

#if defined(EUR_CR_POWER)
#if defined(SGX531)
	




	OSWriteHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_POWER, 1);
	PDUMPREG(SGX_PDUMPREG_NAME, EUR_CR_POWER, 1);
#else
	
	OSWriteHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_POWER, 0);
	PDUMPREG(SGX_PDUMPREG_NAME, EUR_CR_POWER, 0);
#endif
#endif

	
	*psDevInfo->pui32KernelCCBEventKicker = 0;
#if defined(PDUMP)
	if (!bPDumpIsSuspended)
	{
		__DtTestPoint(__DtFunc_SGXInitialise, __DtStep_2)
		psDevInfo->ui32KernelCCBEventKickerDumpVal = 0;
		PDUMPMEM(&psDevInfo->ui32KernelCCBEventKickerDumpVal,
				 psDevInfo->psKernelCCBEventKickerMemInfo, 0,
				 sizeof(*psDevInfo->pui32KernelCCBEventKicker), PDUMP_FLAGS_CONTINUOUS,
				 MAKEUNIQUETAG(psDevInfo->psKernelCCBEventKickerMemInfo));
	}
#endif 

	

	PDUMPCOMMENTWITHFLAGS(PDUMP_FLAGS_CONTINUOUS, "SGX initialisation script part 2\n");
	eError = SGXRunScript(psDevInfo, psDevInfo->sScripts.asInitCommandsPart2, SGX_MAX_INIT_COMMANDS);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXInitialise, __DtStep_3)
		PVR_DPF((PVR_DBG_ERROR,"SGXInitialise: SGXRunScript (part 2) failed (%d)", eError));
		__DtTestPoint(__DtFunc_SGXInitialise, __DtStep_4)
		return eError;
	}
	PDUMPCOMMENTWITHFLAGS(PDUMP_FLAGS_CONTINUOUS, "End of SGX initialisation script part 2\n");

	
	psSGXHostCtl->ui32HostClock = OSClockus();

	psSGXHostCtl->ui32InitStatus = 0;
#if defined(PDUMP)
	PDUMPCOMMENTWITHFLAGS(PDUMP_FLAGS_CONTINUOUS,
						  "Reset the SGX microkernel initialisation status\n");
	PDUMPMEM(IMG_NULL, psSGXHostCtlMemInfo,
			 offsetof(SGXMKIF_HOST_CTL, ui32InitStatus),
			 sizeof(IMG_UINT32), PDUMP_FLAGS_CONTINUOUS,
			 MAKEUNIQUETAG(psSGXHostCtlMemInfo));
	PDUMPCOMMENTWITHFLAGS(PDUMP_FLAGS_CONTINUOUS,
						  "Initialise the microkernel\n");
#endif 

#if defined(SGX_FEATURE_MULTI_EVENT_KICK)
	OSWriteMemoryBarrier();
	OSWriteHWReg(psDevInfo->pvRegsBaseKM,
				 SGX_MP_CORE_SELECT(EUR_CR_EVENT_KICK2, 0),
				 EUR_CR_EVENT_KICK2_NOW_MASK);
#else
	*psDevInfo->pui32KernelCCBEventKicker = (*psDevInfo->pui32KernelCCBEventKicker + 1) & 0xFF;
	OSWriteMemoryBarrier();
	OSWriteHWReg(psDevInfo->pvRegsBaseKM,
				 SGX_MP_CORE_SELECT(EUR_CR_EVENT_KICK, 0),
				 EUR_CR_EVENT_KICK_NOW_MASK);
#endif 

	OSMemoryBarrier();

#if defined(PDUMP)
	

	if (!bPDumpIsSuspended)
	{
#if defined(SGX_FEATURE_MULTI_EVENT_KICK)
		PDUMPREG(SGX_PDUMPREG_NAME, SGX_MP_CORE_SELECT(EUR_CR_EVENT_KICK2, 0), EUR_CR_EVENT_KICK2_NOW_MASK);
#else
		psDevInfo->ui32KernelCCBEventKickerDumpVal = 1;
		PDUMPCOMMENTWITHFLAGS(PDUMP_FLAGS_CONTINUOUS,
							  "First increment of the SGX event kicker value\n");
		PDUMPMEM(&psDevInfo->ui32KernelCCBEventKickerDumpVal,
				 psDevInfo->psKernelCCBEventKickerMemInfo,
				 0,
				 sizeof(IMG_UINT32),
				 PDUMP_FLAGS_CONTINUOUS,
				 MAKEUNIQUETAG(psDevInfo->psKernelCCBEventKickerMemInfo));
		PDUMPREG(SGX_PDUMPREG_NAME, SGX_MP_CORE_SELECT(EUR_CR_EVENT_KICK, 0), EUR_CR_EVENT_KICK_NOW_MASK);
#endif 
	}
#endif 

#if !defined(NO_HARDWARE)
	

	if (PollForValueKM(&psSGXHostCtl->ui32InitStatus,
					   PVRSRV_USSE_EDM_INIT_COMPLETE,
					   PVRSRV_USSE_EDM_INIT_COMPLETE,
					   MAX_HW_TIME_US,
					   MAX_HW_TIME_US/WAIT_TRY_COUNT,
					   IMG_FALSE) != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXInitialise, __DtStep_5)
		PVR_DPF((PVR_DBG_ERROR, "SGXInitialise: Wait for uKernel initialisation failed"));
		#if !defined(FIX_HW_BRN_23281)
		PVR_DBG_BREAK;
		#endif 
		return PVRSRV_ERROR_RETRY;
	}
#endif 

#if defined(PDUMP)
	PDUMPCOMMENTWITHFLAGS(PDUMP_FLAGS_CONTINUOUS,
						  "Wait for the SGX microkernel initialisation to complete");
	PDUMPMEMPOL(psSGXHostCtlMemInfo,
				offsetof(SGXMKIF_HOST_CTL, ui32InitStatus),
				PVRSRV_USSE_EDM_INIT_COMPLETE,
				PVRSRV_USSE_EDM_INIT_COMPLETE,
				PDUMP_POLL_OPERATOR_EQUAL,
				PDUMP_FLAGS_CONTINUOUS,
				MAKEUNIQUETAG(psSGXHostCtlMemInfo));
#endif 

#if defined(FIX_HW_BRN_22997) && defined(FIX_HW_BRN_23030) && defined(SGX_FEATURE_HOST_PORT)
	


	WorkaroundBRN22997ReadHostPort(psDevInfo);
#endif 

	PVR_ASSERT(psDevInfo->psKernelCCBCtl->ui32ReadOffset == psDevInfo->psKernelCCBCtl->ui32WriteOffset);

	bFirstTime = IMG_FALSE;
	
	__DtTestPoint(__DtFunc_SGXInitialise, __DtStep_6)
	return PVRSRV_OK;
}

PVRSRV_ERROR SGXDeinitialise(IMG_HANDLE hDevCookie)

{
	PVRSRV_SGXDEV_INFO	*psDevInfo = (PVRSRV_SGXDEV_INFO *) hDevCookie;
	PVRSRV_ERROR		eError;

	
	__DtTestPoint(__DtFunc_SGXDeinitialise, __DtStep_0)
	if (psDevInfo->pvRegsBaseKM == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_SGXDeinitialise, __DtStep_1)
		return PVRSRV_OK;
	}

	eError = SGXRunScript(psDevInfo, psDevInfo->sScripts.asDeinitCommands, SGX_MAX_DEINIT_COMMANDS);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDeinitialise, __DtStep_2)
		PVR_DPF((PVR_DBG_ERROR,"SGXDeinitialise: SGXRunScript failed (%d)", eError));
		__DtTestPoint(__DtFunc_SGXDeinitialise, __DtStep_3)
		return eError;
	}

	__DtTestPoint(__DtFunc_SGXDeinitialise, __DtStep_4)
	return PVRSRV_OK;
}


static PVRSRV_ERROR DevInitSGXPart1 (IMG_VOID *pvDeviceNode)
{
	IMG_HANDLE hDevMemHeap = IMG_NULL;
	PVRSRV_SGXDEV_INFO	*psDevInfo;
	IMG_HANDLE		hKernelDevMemContext;
	IMG_DEV_PHYADDR		sPDDevPAddr;
	IMG_UINT32		i;
	PVRSRV_DEVICE_NODE  *psDeviceNode = (PVRSRV_DEVICE_NODE *)pvDeviceNode;
	DEVICE_MEMORY_HEAP_INFO *psDeviceMemoryHeap = psDeviceNode->sDevMemoryInfo.psDeviceMemoryHeap;
	PVRSRV_ERROR		eError;

	
	__DtTestPoint(__DtFunc_DevInitSGXPart1, __DtStep_0)
	PDUMPCOMMENT("SGX Core Version Information: %s", SGX_CORE_FRIENDLY_NAME);
	
	#if defined(SGX_FEATURE_MP)
	PDUMPCOMMENT("SGX Multi-processor: %d cores", SGX_FEATURE_MP_CORE_COUNT);
	#endif 

#if (SGX_CORE_REV == 0)
	PDUMPCOMMENT("SGX Core Revision Information: head RTL");
#else
	PDUMPCOMMENT("SGX Core Revision Information: %d", SGX_CORE_REV);
#endif

	#if defined(SGX_FEATURE_SYSTEM_CACHE)
	PDUMPCOMMENT("SGX System Level Cache is present\r\n");
	#if defined(SGX_BYPASS_SYSTEM_CACHE)
	PDUMPCOMMENT("SGX System Level Cache is bypassed\r\n");
	#endif 
	#endif 

	PDUMPCOMMENT("SGX Initialisation Part 1");

	
	if(OSAllocMem( PVRSRV_OS_NON_PAGEABLE_HEAP,
					 sizeof(PVRSRV_SGXDEV_INFO),
					 (IMG_VOID **)&psDevInfo, IMG_NULL,
					 "SGX Device Info") != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_DevInitSGXPart1, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"DevInitSGXPart1 : Failed to alloc memory for DevInfo"));
		__DtTestPoint(__DtFunc_DevInitSGXPart1, __DtStep_2)
		return (PVRSRV_ERROR_OUT_OF_MEMORY);
	}
	OSMemSet (psDevInfo, 0, sizeof(PVRSRV_SGXDEV_INFO));

	
	psDevInfo->eDeviceType 		= DEV_DEVICE_TYPE;
	psDevInfo->eDeviceClass 	= DEV_DEVICE_CLASS;

	
	psDeviceNode->pvDevice = (IMG_PVOID)psDevInfo;

	
	psDevInfo->pvDeviceMemoryHeap = (IMG_VOID*)psDeviceMemoryHeap;

	
	hKernelDevMemContext = BM_CreateContext(psDeviceNode,
											&sPDDevPAddr,
											IMG_NULL,
											IMG_NULL);
	if (hKernelDevMemContext == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_DevInitSGXPart1, __DtStep_3)
		PVR_DPF((PVR_DBG_ERROR,"DevInitSGXPart1: Failed BM_CreateContext"));
		__DtTestPoint(__DtFunc_DevInitSGXPart1, __DtStep_4)
		return PVRSRV_ERROR_OUT_OF_MEMORY;
	}

	psDevInfo->sKernelPDDevPAddr = sPDDevPAddr;

	
	for(i=0; i<psDeviceNode->sDevMemoryInfo.ui32HeapCount; i++)
	{
		__DtTestPoint(__DtFunc_DevInitSGXPart1, __DtStep_5)
		switch(psDeviceMemoryHeap[i].DevMemHeapType)
		{
			case DEVICE_MEMORY_HEAP_KERNEL:
			case DEVICE_MEMORY_HEAP_SHARED:
			case DEVICE_MEMORY_HEAP_SHARED_EXPORTED:
			{
				__DtTestPoint(__DtFunc_DevInitSGXPart1, __DtStep_6)
				hDevMemHeap = BM_CreateHeap (hKernelDevMemContext,
												&psDeviceMemoryHeap[i]);
				


				psDeviceMemoryHeap[i].hDevMemHeap = hDevMemHeap;
				break;
			}
		}
	}
#if defined(PDUMP)
	if(hDevMemHeap)
	{
		
		__DtTestPoint(__DtFunc_DevInitSGXPart1, __DtStep_7)
		psDevInfo->sMMUAttrib = *((BM_HEAP*)hDevMemHeap)->psMMUAttrib;
	}
#endif
	eError = MMU_BIFResetPDAlloc(psDevInfo);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_DevInitSGXPart1, __DtStep_8)
		PVR_DPF((PVR_DBG_ERROR,"DevInitSGX : Failed to alloc memory for BIF reset"));
		__DtTestPoint(__DtFunc_DevInitSGXPart1, __DtStep_9)
		return eError;
	}

	__DtTestPoint(__DtFunc_DevInitSGXPart1, __DtStep_10)
	return PVRSRV_OK;
}

IMG_EXPORT
PVRSRV_ERROR SGXGetInfoForSrvinitKM(IMG_HANDLE hDevHandle, SGX_BRIDGE_INFO_FOR_SRVINIT *psInitInfo)
{
	PVRSRV_DEVICE_NODE	*psDeviceNode;
	PVRSRV_SGXDEV_INFO	*psDevInfo;
	PVRSRV_ERROR		eError;

	__DtTestPoint(__DtFunc_SGXGetInfoForSrvinitKM, __DtStep_0)
	PDUMPCOMMENT("SGXGetInfoForSrvinit");

	psDeviceNode = (PVRSRV_DEVICE_NODE *)hDevHandle;
	psDevInfo = (PVRSRV_SGXDEV_INFO *)psDeviceNode->pvDevice;

	psInitInfo->sPDDevPAddr = psDevInfo->sKernelPDDevPAddr;

	eError = PVRSRVGetDeviceMemHeapsKM(hDevHandle, &psInitInfo->asHeapInfo[0]);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXGetInfoForSrvinitKM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"SGXGetInfoForSrvinit: PVRSRVGetDeviceMemHeapsKM failed (%d)", eError));
		__DtTestPoint(__DtFunc_SGXGetInfoForSrvinitKM, __DtStep_2)
		return eError;
	}

	__DtTestPoint(__DtFunc_SGXGetInfoForSrvinitKM, __DtStep_3)
	return eError;
}

IMG_EXPORT
PVRSRV_ERROR DevInitSGXPart2KM (PVRSRV_PER_PROCESS_DATA *psPerProc,
                                IMG_HANDLE hDevHandle,
                                SGX_BRIDGE_INIT_INFO *psInitInfo)
{
	PVRSRV_DEVICE_NODE		*psDeviceNode;
	PVRSRV_SGXDEV_INFO		*psDevInfo;
	PVRSRV_ERROR			eError;
	SGX_DEVICE_MAP			*psSGXDeviceMap;
	PVRSRV_DEV_POWER_STATE	eDefaultPowerState;

	__DtTestPoint(__DtFunc_DevInitSGXPart2KM, __DtStep_0)
	PDUMPCOMMENT("SGX Initialisation Part 2");

	psDeviceNode = (PVRSRV_DEVICE_NODE *)hDevHandle;
	psDevInfo = (PVRSRV_SGXDEV_INFO *)psDeviceNode->pvDevice;

	

	eError = InitDevInfo(psPerProc, psDeviceNode, psInitInfo);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_DevInitSGXPart2KM, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"DevInitSGXPart2KM: Failed to load EDM program"));
		goto failed_init_dev_info;
	}


	eError = SysGetDeviceMemoryMap(PVRSRV_DEVICE_TYPE_SGX,
									(IMG_VOID**)&psSGXDeviceMap);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_DevInitSGXPart2KM, __DtStep_2)
		PVR_DPF((PVR_DBG_ERROR,"DevInitSGXPart2KM: Failed to get device memory map!"));
		__DtTestPoint(__DtFunc_DevInitSGXPart2KM, __DtStep_3)
		return PVRSRV_ERROR_INIT_FAILURE;
	}

	
	if (psSGXDeviceMap->pvRegsCpuVBase)
	{
		__DtTestPoint(__DtFunc_DevInitSGXPart2KM, __DtStep_4)
		psDevInfo->pvRegsBaseKM = psSGXDeviceMap->pvRegsCpuVBase;
	}
	else
	{
		
		__DtTestPoint(__DtFunc_DevInitSGXPart2KM, __DtStep_5)
		psDevInfo->pvRegsBaseKM = OSMapPhysToLin(psSGXDeviceMap->sRegsCpuPBase,
											   psSGXDeviceMap->ui32RegsSize,
											   PVRSRV_HAP_KERNEL_ONLY|PVRSRV_HAP_UNCACHED,
											   IMG_NULL);
		if (!psDevInfo->pvRegsBaseKM)
		{
			__DtTestPoint(__DtFunc_DevInitSGXPart2KM, __DtStep_6)
			PVR_DPF((PVR_DBG_ERROR,"DevInitSGXPart2KM: Failed to map in regs\n"));
			__DtTestPoint(__DtFunc_DevInitSGXPart2KM, __DtStep_7)
			return PVRSRV_ERROR_BAD_MAPPING;
		}
	}
	psDevInfo->ui32RegSize = psSGXDeviceMap->ui32RegsSize;
	psDevInfo->sRegsPhysBase = psSGXDeviceMap->sRegsSysPBase;


#if defined(SGX_FEATURE_HOST_PORT)
	if (psSGXDeviceMap->ui32Flags & SGX_HOSTPORT_PRESENT)
	{
		
		__DtTestPoint(__DtFunc_DevInitSGXPart2KM, __DtStep_8)
		psDevInfo->pvHostPortBaseKM = OSMapPhysToLin(psSGXDeviceMap->sHPCpuPBase,
									  	           psSGXDeviceMap->ui32HPSize,
									  	           PVRSRV_HAP_KERNEL_ONLY|PVRSRV_HAP_UNCACHED,
									  	           IMG_NULL);
		if (!psDevInfo->pvHostPortBaseKM)
		{
			__DtTestPoint(__DtFunc_DevInitSGXPart2KM, __DtStep_9)
			PVR_DPF((PVR_DBG_ERROR,"DevInitSGXPart2KM: Failed to map in host port\n"));
			__DtTestPoint(__DtFunc_DevInitSGXPart2KM, __DtStep_10)
			return PVRSRV_ERROR_BAD_MAPPING;
		}
		psDevInfo->ui32HPSize = psSGXDeviceMap->ui32HPSize;
		psDevInfo->sHPSysPAddr = psSGXDeviceMap->sHPSysPBase;
	}
#endif

#if defined (SYS_USING_INTERRUPTS)

	
	psDeviceNode->pvISRData = psDeviceNode;
	
	PVR_ASSERT(psDeviceNode->pfnDeviceISR == SGX_ISRHandler);

#endif 

	
	psDevInfo->psSGXHostCtl->ui32PowerStatus |= PVRSRV_USSE_EDM_POWMAN_NO_WORK;
	eDefaultPowerState = PVRSRV_DEV_POWER_STATE_OFF;
	
	eError = PVRSRVRegisterPowerDevice (psDeviceNode->sDevId.ui32DeviceIndex,
										&SGXPrePowerState, &SGXPostPowerState,
										&SGXPreClockSpeedChange, &SGXPostClockSpeedChange,
										(IMG_HANDLE)psDeviceNode,
										PVRSRV_DEV_POWER_STATE_OFF,
										eDefaultPowerState);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_DevInitSGXPart2KM, __DtStep_11)
		PVR_DPF((PVR_DBG_ERROR,"DevInitSGXPart2KM: failed to register device with power manager"));
		__DtTestPoint(__DtFunc_DevInitSGXPart2KM, __DtStep_12)
		return eError;
	}

#if defined(FIX_HW_BRN_22997) && defined(FIX_HW_BRN_23030) && defined(SGX_FEATURE_HOST_PORT)
	eError = WorkaroundBRN22997Alloc(psDeviceNode);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_DevInitSGXPart2KM, __DtStep_13)
		PVR_DPF((PVR_DBG_ERROR,"SGXInitialise : Failed to alloc memory for BRN22997 workaround"));
		__DtTestPoint(__DtFunc_DevInitSGXPart2KM, __DtStep_14)
		return eError;
	}
#endif 

#if defined(SUPPORT_EXTERNAL_SYSTEM_CACHE)
	
	psDevInfo->ui32ExtSysCacheRegsSize = psSGXDeviceMap->ui32ExtSysCacheRegsSize;
	psDevInfo->sExtSysCacheRegsDevPBase = psSGXDeviceMap->sExtSysCacheRegsDevPBase;
	eError = MMU_MapExtSystemCacheRegs(psDeviceNode);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_DevInitSGXPart2KM, __DtStep_15)
		PVR_DPF((PVR_DBG_ERROR,"SGXInitialise : Failed to map external system cache registers"));
		__DtTestPoint(__DtFunc_DevInitSGXPart2KM, __DtStep_16)
		return eError;
	}
#endif 

	

	OSMemSet(psDevInfo->psKernelCCB, 0, sizeof(PVRSRV_SGX_KERNEL_CCB));
	OSMemSet(psDevInfo->psKernelCCBCtl, 0, sizeof(PVRSRV_SGX_CCB_CTL));
	OSMemSet(psDevInfo->pui32KernelCCBEventKicker, 0, sizeof(*psDevInfo->pui32KernelCCBEventKicker));
	PDUMPCOMMENT("Initialise Kernel CCB");
	PDUMPMEM(IMG_NULL, psDevInfo->psKernelCCBMemInfo, 0, sizeof(PVRSRV_SGX_KERNEL_CCB), PDUMP_FLAGS_CONTINUOUS, MAKEUNIQUETAG(psDevInfo->psKernelCCBMemInfo));
	PDUMPCOMMENT("Initialise Kernel CCB Control");
	PDUMPMEM(IMG_NULL, psDevInfo->psKernelCCBCtlMemInfo, 0, sizeof(PVRSRV_SGX_CCB_CTL), PDUMP_FLAGS_CONTINUOUS, MAKEUNIQUETAG(psDevInfo->psKernelCCBCtlMemInfo));
	PDUMPCOMMENT("Initialise Kernel CCB Event Kicker");
	PDUMPMEM(IMG_NULL, psDevInfo->psKernelCCBEventKickerMemInfo, 0, sizeof(*psDevInfo->pui32KernelCCBEventKicker), PDUMP_FLAGS_CONTINUOUS, MAKEUNIQUETAG(psDevInfo->psKernelCCBEventKickerMemInfo));

	__DtTestPoint(__DtFunc_DevInitSGXPart2KM, __DtStep_17)
	return PVRSRV_OK;

failed_init_dev_info:
	__DtTestPoint(__DtFunc_DevInitSGXPart2KM, __DtStep_18)
	return eError;
}

static PVRSRV_ERROR DevDeInitSGX (IMG_VOID *pvDeviceNode)
{
	PVRSRV_DEVICE_NODE			*psDeviceNode = (PVRSRV_DEVICE_NODE *)pvDeviceNode;
	PVRSRV_SGXDEV_INFO			*psDevInfo = (PVRSRV_SGXDEV_INFO*)psDeviceNode->pvDevice;
	PVRSRV_ERROR				eError;
	IMG_UINT32					ui32Heap;
	DEVICE_MEMORY_HEAP_INFO		*psDeviceMemoryHeap;
	SGX_DEVICE_MAP				*psSGXDeviceMap;

	__DtTestPoint(__DtFunc_DevDeInitSGX, __DtStep_0)
	if (!psDevInfo)
	{
		
		__DtTestPoint(__DtFunc_DevDeInitSGX, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"DevDeInitSGX: Null DevInfo"));
		__DtTestPoint(__DtFunc_DevDeInitSGX, __DtStep_2)
		return PVRSRV_OK;
	}

#if defined(SUPPORT_HW_RECOVERY)
	if (psDevInfo->hTimer)
	{
		__DtTestPoint(__DtFunc_DevDeInitSGX, __DtStep_3)
		eError = OSRemoveTimer(psDevInfo->hTimer);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_DevDeInitSGX, __DtStep_4)
			PVR_DPF((PVR_DBG_ERROR,"DevDeInitSGX: Failed to remove timer"));
			__DtTestPoint(__DtFunc_DevDeInitSGX, __DtStep_5)
			return 	eError;
		}
		psDevInfo->hTimer = IMG_NULL;
	}
#endif 

#if defined(SUPPORT_EXTERNAL_SYSTEM_CACHE)
	
	eError = MMU_UnmapExtSystemCacheRegs(psDeviceNode);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_DevDeInitSGX, __DtStep_6)
		PVR_DPF((PVR_DBG_ERROR,"DevDeInitSGX: Failed to unmap ext system cache registers"));
		__DtTestPoint(__DtFunc_DevDeInitSGX, __DtStep_7)
		return eError;
	}
#endif 

#if defined(FIX_HW_BRN_22997) && defined(FIX_HW_BRN_23030) && defined(SGX_FEATURE_HOST_PORT)
	WorkaroundBRN22997Free(psDeviceNode);
#endif 

	MMU_BIFResetPDFree(psDevInfo);

	

	DeinitDevInfo(psDevInfo);

	
	psDeviceMemoryHeap = (DEVICE_MEMORY_HEAP_INFO *)psDevInfo->pvDeviceMemoryHeap;
	for(ui32Heap=0; ui32Heap<psDeviceNode->sDevMemoryInfo.ui32HeapCount; ui32Heap++)
	{
		__DtTestPoint(__DtFunc_DevDeInitSGX, __DtStep_8)
		switch(psDeviceMemoryHeap[ui32Heap].DevMemHeapType)
		{
			case DEVICE_MEMORY_HEAP_KERNEL:
			case DEVICE_MEMORY_HEAP_SHARED:
			case DEVICE_MEMORY_HEAP_SHARED_EXPORTED:
			{
				__DtTestPoint(__DtFunc_DevDeInitSGX, __DtStep_9)
				if (psDeviceMemoryHeap[ui32Heap].hDevMemHeap != IMG_NULL)
				{
					__DtTestPoint(__DtFunc_DevDeInitSGX, __DtStep_10)
					BM_DestroyHeap(psDeviceMemoryHeap[ui32Heap].hDevMemHeap);
				}
				break;
			}
		}
	}

	
	eError = BM_DestroyContext(psDeviceNode->sDevMemoryInfo.pBMKernelContext, IMG_NULL);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_DevDeInitSGX, __DtStep_11)
		PVR_DPF((PVR_DBG_ERROR,"DevDeInitSGX : Failed to destroy kernel context"));
		__DtTestPoint(__DtFunc_DevDeInitSGX, __DtStep_12)
		return eError;
	}

	
	eError = PVRSRVRemovePowerDevice (((PVRSRV_DEVICE_NODE*)pvDeviceNode)->sDevId.ui32DeviceIndex);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_DevDeInitSGX, __DtStep_13)
		return eError;
	}

	eError = SysGetDeviceMemoryMap(PVRSRV_DEVICE_TYPE_SGX,
									(IMG_VOID**)&psSGXDeviceMap);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_DevDeInitSGX, __DtStep_14)
		PVR_DPF((PVR_DBG_ERROR,"DevDeInitSGX: Failed to get device memory map!"));
		__DtTestPoint(__DtFunc_DevDeInitSGX, __DtStep_15)
		return eError;
	}

	
	if (!psSGXDeviceMap->pvRegsCpuVBase)
	{
		
		__DtTestPoint(__DtFunc_DevDeInitSGX, __DtStep_16)
		if (psDevInfo->pvRegsBaseKM != IMG_NULL)
		{
			__DtTestPoint(__DtFunc_DevDeInitSGX, __DtStep_17)
			OSUnMapPhysToLin(psDevInfo->pvRegsBaseKM,
							 psDevInfo->ui32RegSize,
							 PVRSRV_HAP_KERNEL_ONLY|PVRSRV_HAP_UNCACHED,
							 IMG_NULL);
		}
	}

#if defined(SGX_FEATURE_HOST_PORT)
	if (psSGXDeviceMap->ui32Flags & SGX_HOSTPORT_PRESENT)
	{
		
		__DtTestPoint(__DtFunc_DevDeInitSGX, __DtStep_18)
		if (psDevInfo->pvHostPortBaseKM != IMG_NULL)
		{
			__DtTestPoint(__DtFunc_DevDeInitSGX, __DtStep_19)
			OSUnMapPhysToLin(psDevInfo->pvHostPortBaseKM,
						   psDevInfo->ui32HPSize,
						   PVRSRV_HAP_KERNEL_ONLY|PVRSRV_HAP_UNCACHED,
						   IMG_NULL);
		}
	}
#endif 


	
	OSFreeMem(PVRSRV_OS_NON_PAGEABLE_HEAP,
				sizeof(PVRSRV_SGXDEV_INFO),
				psDevInfo,
				0);

	psDeviceNode->pvDevice = IMG_NULL;

	if (psDeviceMemoryHeap != IMG_NULL)
	{
	
		__DtTestPoint(__DtFunc_DevDeInitSGX, __DtStep_20)
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP,
				sizeof(DEVICE_MEMORY_HEAP_INFO) * SGX_MAX_HEAP_ID,
				psDeviceMemoryHeap,
				0);
	}

	__DtTestPoint(__DtFunc_DevDeInitSGX, __DtStep_21)
	return PVRSRV_OK;
}


static IMG_VOID SGXDumpDebugReg (PVRSRV_SGXDEV_INFO	*psDevInfo,
								 IMG_UINT32			ui32CoreNum,
								 IMG_CHAR			*pszName,
								 IMG_UINT32			ui32RegAddr)
{
	IMG_UINT32	ui32RegVal;
	__DtTestPoint(__DtFunc_SGXDumpDebugReg, __DtStep_0)
	ui32RegVal = OSReadHWReg(psDevInfo->pvRegsBaseKM, SGX_MP_CORE_SELECT(ui32RegAddr, ui32CoreNum));
	PVR_LOG(("(P%u) %s%08X", ui32CoreNum, pszName, ui32RegVal));
	__DtTestPoint(__DtFunc_SGXDumpDebugReg, __DtStep_1)
}

static IMG_VOID SGXDumpDebugInfo (PVRSRV_SGXDEV_INFO	*psDevInfo,
								  IMG_BOOL				bDumpSGXRegs)
{
	IMG_UINT32	ui32CoreNum;

	__DtTestPoint(__DtFunc_SGXDumpDebugInfo, __DtStep_0)
	PVR_LOG(("SGX debug (%s)", PVRVERSION_STRING));

	if (bDumpSGXRegs)
	{
		__DtTestPoint(__DtFunc_SGXDumpDebugInfo, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"SGX Register Base Address (Linear):   0x%08X", (IMG_UINTPTR_T)psDevInfo->pvRegsBaseKM));
		PVR_DPF((PVR_DBG_ERROR,"SGX Register Base Address (Physical): 0x%08X", psDevInfo->sRegsPhysBase.uiAddr));

		for (ui32CoreNum = 0; ui32CoreNum < SGX_FEATURE_MP_CORE_COUNT; ui32CoreNum++)
		{
		
			__DtTestPoint(__DtFunc_SGXDumpDebugInfo, __DtStep_2)
			SGXDumpDebugReg(psDevInfo, ui32CoreNum, "EUR_CR_EVENT_STATUS:     ", EUR_CR_EVENT_STATUS);
			SGXDumpDebugReg(psDevInfo, ui32CoreNum, "EUR_CR_EVENT_STATUS2:    ", EUR_CR_EVENT_STATUS2);
			SGXDumpDebugReg(psDevInfo, ui32CoreNum, "EUR_CR_BIF_CTRL:         ", EUR_CR_BIF_CTRL);
		#if defined(EUR_CR_BIF_BANK0)
			SGXDumpDebugReg(psDevInfo, ui32CoreNum, "EUR_CR_BIF_BANK0:        ", EUR_CR_BIF_BANK0);
		#endif
			SGXDumpDebugReg(psDevInfo, ui32CoreNum, "EUR_CR_BIF_INT_STAT:     ", EUR_CR_BIF_INT_STAT);
			SGXDumpDebugReg(psDevInfo, ui32CoreNum, "EUR_CR_BIF_FAULT:        ", EUR_CR_BIF_FAULT);
			SGXDumpDebugReg(psDevInfo, ui32CoreNum, "EUR_CR_BIF_MEM_REQ_STAT: ", EUR_CR_BIF_MEM_REQ_STAT);
			SGXDumpDebugReg(psDevInfo, ui32CoreNum, "EUR_CR_CLKGATECTL:       ", EUR_CR_CLKGATECTL);
		#if defined(EUR_CR_PDS_PC_BASE)
			SGXDumpDebugReg(psDevInfo, ui32CoreNum, "EUR_CR_PDS_PC_BASE:      ", EUR_CR_PDS_PC_BASE);
		#endif
		}
	}

	

	QueueDumpDebugInfo();

	{
		

		IMG_UINT32	*pui32HostCtlBuffer = (IMG_UINT32 *)psDevInfo->psSGXHostCtl;
		IMG_UINT32	ui32LoopCounter;

		PVR_LOG(("SGX Host control:"));

		for (ui32LoopCounter = 0;
			 ui32LoopCounter < sizeof(*psDevInfo->psSGXHostCtl) / sizeof(*pui32HostCtlBuffer);
			 ui32LoopCounter += 4)
		{
			__DtTestPoint(__DtFunc_SGXDumpDebugInfo, __DtStep_3)
			PVR_LOG(("\t(HC-%X) 0x%08X 0x%08X 0x%08X 0x%08X", ui32LoopCounter * sizeof(*pui32HostCtlBuffer),
					pui32HostCtlBuffer[ui32LoopCounter + 0], pui32HostCtlBuffer[ui32LoopCounter + 1],
					pui32HostCtlBuffer[ui32LoopCounter + 2], pui32HostCtlBuffer[ui32LoopCounter + 3]));
		}
	}

	{
		

		IMG_UINT32	*pui32TA3DCtlBuffer = psDevInfo->psKernelSGXTA3DCtlMemInfo->pvLinAddrKM;
		IMG_UINT32	ui32LoopCounter;

		PVR_LOG(("SGX TA/3D control:"));

		for (ui32LoopCounter = 0;
			 ui32LoopCounter < psDevInfo->psKernelSGXTA3DCtlMemInfo->ui32AllocSize / sizeof(*pui32TA3DCtlBuffer);
			 ui32LoopCounter += 4)
		{
			__DtTestPoint(__DtFunc_SGXDumpDebugInfo, __DtStep_4)
			PVR_LOG(("\t(T3C-%X) 0x%08X 0x%08X 0x%08X 0x%08X", ui32LoopCounter * sizeof(*pui32TA3DCtlBuffer),
					pui32TA3DCtlBuffer[ui32LoopCounter + 0], pui32TA3DCtlBuffer[ui32LoopCounter + 1],
					pui32TA3DCtlBuffer[ui32LoopCounter + 2], pui32TA3DCtlBuffer[ui32LoopCounter + 3]));
		}
	}

	#if defined(PVRSRV_USSE_EDM_STATUS_DEBUG)
	{
		IMG_UINT32	*pui32MKTraceBuffer = psDevInfo->psKernelEDMStatusBufferMemInfo->pvLinAddrKM;
		IMG_UINT32	ui32LastStatusCode, ui32WriteOffset;

		ui32LastStatusCode = *pui32MKTraceBuffer;
		pui32MKTraceBuffer++;
		ui32WriteOffset = *pui32MKTraceBuffer;
		pui32MKTraceBuffer++;

		PVR_LOG(("Last SGX microkernel status code: %08X", ui32LastStatusCode));

		#if defined(PVRSRV_DUMP_MK_TRACE)
		

		{
			IMG_UINT32	ui32LoopCounter;

			for (ui32LoopCounter = 0;
				 ui32LoopCounter < SGXMK_TRACE_BUFFER_SIZE;
				 ui32LoopCounter++)
			{
				IMG_UINT32	*pui32BufPtr;
				__DtTestPoint(__DtFunc_SGXDumpDebugInfo, __DtStep_5)
				pui32BufPtr = pui32MKTraceBuffer +
								(((ui32WriteOffset + ui32LoopCounter) % SGXMK_TRACE_BUFFER_SIZE) * 4);
				PVR_LOG(("\t(MKT-%X) %08X %08X %08X %08X", ui32LoopCounter,
						 pui32BufPtr[2], pui32BufPtr[3], pui32BufPtr[1], pui32BufPtr[0]));
			}
		}
		#endif 
	}
	#endif 

	{
		

		PVR_LOG(("SGX Kernel CCB WO:0x%X RO:0x%X",
				psDevInfo->psKernelCCBCtl->ui32WriteOffset,
				psDevInfo->psKernelCCBCtl->ui32ReadOffset));

		#if defined(PVRSRV_DUMP_KERNEL_CCB)
		{
			IMG_UINT32	ui32LoopCounter;

			for (ui32LoopCounter = 0;
				 ui32LoopCounter < sizeof(psDevInfo->psKernelCCB->asCommands) /
				 					sizeof(psDevInfo->psKernelCCB->asCommands[0]);
				 ui32LoopCounter++)
			{
				SGXMKIF_COMMAND	*psCommand = &psDevInfo->psKernelCCB->asCommands[ui32LoopCounter];

				__DtTestPoint(__DtFunc_SGXDumpDebugInfo, __DtStep_6)
				PVR_LOG(("\t(KCCB-%X) %08X %08X - %08X %08X %08X %08X", ui32LoopCounter,
						psCommand->ui32ServiceAddress, psCommand->ui32CacheControl,
						psCommand->ui32Data[0], psCommand->ui32Data[1],
						psCommand->ui32Data[2], psCommand->ui32Data[3]));
			}
		}
		#endif 
	}
	__DtTestPoint(__DtFunc_SGXDumpDebugInfo, __DtStep_7)
}


#if defined(SYS_USING_INTERRUPTS) || defined(SUPPORT_HW_RECOVERY)
static
IMG_VOID HWRecoveryResetSGX (PVRSRV_DEVICE_NODE *psDeviceNode,
							 IMG_UINT32 		ui32Component,
							 IMG_UINT32			ui32CallerID)
{
	PVRSRV_ERROR		eError;
	PVRSRV_SGXDEV_INFO	*psDevInfo = (PVRSRV_SGXDEV_INFO*)psDeviceNode->pvDevice;
	SGXMKIF_HOST_CTL	*psSGXHostCtl = (SGXMKIF_HOST_CTL *)psDevInfo->psSGXHostCtl;

	__DtTestPoint(__DtFunc_HWRecoveryResetSGX, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(ui32Component);

	

	eError = PVRSRVPowerLock(ui32CallerID, IMG_FALSE);
	if(eError != PVRSRV_OK)
	{
		


		__DtTestPoint(__DtFunc_HWRecoveryResetSGX, __DtStep_1)
		PVR_DPF((PVR_DBG_WARNING,"HWRecoveryResetSGX: Power transition in progress"));
		__DtTestPoint(__DtFunc_HWRecoveryResetSGX, __DtStep_2)
		return;
	}

	psSGXHostCtl->ui32InterruptClearFlags |= PVRSRV_USSE_EDM_INTERRUPT_HWR;

	PVR_LOG(("HWRecoveryResetSGX: SGX Hardware Recovery triggered"));

	SGXDumpDebugInfo(psDeviceNode->pvDevice, IMG_TRUE);

	
	PDUMPSUSPEND();

	
#if defined(FIX_HW_BRN_23281)
	
	for (eError = PVRSRV_ERROR_RETRY; eError == PVRSRV_ERROR_RETRY;)
#endif 
	{
	eError = SGXInitialise(psDevInfo, IMG_TRUE);
	}

	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_HWRecoveryResetSGX, __DtStep_3)
		PVR_DPF((PVR_DBG_ERROR,"HWRecoveryResetSGX: SGXInitialise failed (%d)", eError));
	}

	
	PDUMPRESUME();

	PVRSRVPowerUnlock(ui32CallerID);

	
	SGXScheduleProcessQueuesKM(psDeviceNode);

	
	
	PVRSRVProcessQueues(ui32CallerID, IMG_TRUE);
	__DtTestPoint(__DtFunc_HWRecoveryResetSGX, __DtStep_4)
}
#endif 


#if defined(SUPPORT_HW_RECOVERY)
IMG_VOID SGXOSTimer(IMG_VOID *pvData)
{
	PVRSRV_DEVICE_NODE *psDeviceNode = pvData;
	PVRSRV_SGXDEV_INFO *psDevInfo = psDeviceNode->pvDevice;
	static IMG_UINT32	ui32EDMTasks = 0;
	static IMG_UINT32	ui32LockupCounter = 0; 
	static IMG_UINT32	ui32NumResets = 0;
#if defined(FIX_HW_BRN_31093)
	static IMG_BOOL		bBRN31093Inval = IMG_FALSE;
#endif
	IMG_UINT32		ui32CurrentEDMTasks;
	IMG_BOOL		bLockup = IMG_FALSE;
	IMG_BOOL		bPoweredDown;

	
	psDevInfo->ui32TimeStamp++;

#if defined(NO_HARDWARE)
	bPoweredDown = IMG_TRUE;
#else
	bPoweredDown = (SGXIsDevicePowered(psDeviceNode)) ? IMG_FALSE : IMG_TRUE;
#endif 

	
	
	if (bPoweredDown)
	{
		__DtTestPoint(__DtFunc_SGXOSTimer, __DtStep_0)
		ui32LockupCounter = 0;
	#if defined(FIX_HW_BRN_31093)
		bBRN31093Inval = IMG_FALSE;
	#endif
	}
	else
	{
		
		__DtTestPoint(__DtFunc_SGXOSTimer, __DtStep_1)
		ui32CurrentEDMTasks = OSReadHWReg(psDevInfo->pvRegsBaseKM, psDevInfo->ui32EDMTaskReg0);
		if (psDevInfo->ui32EDMTaskReg1 != 0)
		{
			__DtTestPoint(__DtFunc_SGXOSTimer, __DtStep_2)
			ui32CurrentEDMTasks ^= OSReadHWReg(psDevInfo->pvRegsBaseKM, psDevInfo->ui32EDMTaskReg1);
		}
		if ((ui32CurrentEDMTasks == ui32EDMTasks) &&
			(psDevInfo->ui32NumResets == ui32NumResets))
		{
			__DtTestPoint(__DtFunc_SGXOSTimer, __DtStep_3)
			ui32LockupCounter++;
			if (ui32LockupCounter == 3)
			{
				__DtTestPoint(__DtFunc_SGXOSTimer, __DtStep_4)
				ui32LockupCounter = 0;
	
	#if defined(FIX_HW_BRN_31093)
				if (bBRN31093Inval == IMG_FALSE)
				{
					
		#if defined(FIX_HW_BRN_29997)
					IMG_UINT32	ui32BIFCtrl;
					
					ui32BIFCtrl = OSReadHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_BIF_CTRL);
					OSWriteHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_BIF_CTRL, ui32BIFCtrl | EUR_CR_BIF_CTRL_PAUSE_MASK);
					
					OSWaitus(200 * 1000000 / psDevInfo->ui32CoreClockSpeed);
		#endif
					
					bBRN31093Inval = IMG_TRUE;
					
					OSWriteHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_BIF_CTRL_INVAL, EUR_CR_BIF_CTRL_INVAL_PTE_MASK);
					
					OSWaitus(200 * 1000000 / psDevInfo->ui32CoreClockSpeed);
						
		#if defined(FIX_HW_BRN_29997)	
						
					OSWriteHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_BIF_CTRL, ui32BIFCtrl);
		#endif
				}
				else
	#endif
				{
					PVR_DPF((PVR_DBG_ERROR, "SGXOSTimer() detected SGX lockup (0x%x tasks)", ui32EDMTasks));

					bLockup = IMG_TRUE;
				}
			}
		}
		else
		{
	#if defined(FIX_HW_BRN_31093)
			bBRN31093Inval = IMG_FALSE;
	#endif
			ui32LockupCounter = 0;
			ui32EDMTasks = ui32CurrentEDMTasks;
			ui32NumResets = psDevInfo->ui32NumResets;
		}
	}

	if (bLockup)
	{
		SGXMKIF_HOST_CTL	*psSGXHostCtl = (SGXMKIF_HOST_CTL *)psDevInfo->psSGXHostCtl;

		
		__DtTestPoint(__DtFunc_SGXOSTimer, __DtStep_5)
		psSGXHostCtl->ui32HostDetectedLockups ++;

		
		HWRecoveryResetSGX(psDeviceNode, 0, KERNEL_ID);
	}
	__DtTestPoint(__DtFunc_SGXOSTimer, __DtStep_6)
}
#endif 


#if defined(SYS_USING_INTERRUPTS)

IMG_BOOL SGX_ISRHandler (IMG_VOID *pvData)
{
	IMG_BOOL bInterruptProcessed = IMG_FALSE;


	
	__DtTestPoint(__DtFunc_SGX_ISRHandler, __DtStep_0)
	{
		IMG_UINT32 ui32EventStatus, ui32EventEnable;
		IMG_UINT32 ui32EventClear = 0;
#if defined(SGX_FEATURE_DATA_BREAKPOINTS)
		IMG_UINT32 ui32EventStatus2, ui32EventEnable2;
#endif		
		IMG_UINT32 ui32EventClear2 = 0;
		PVRSRV_DEVICE_NODE *psDeviceNode;
		PVRSRV_SGXDEV_INFO *psDevInfo;

		
		if(pvData == IMG_NULL)
		{
			__DtTestPoint(__DtFunc_SGX_ISRHandler, __DtStep_1)
			PVR_DPF((PVR_DBG_ERROR, "SGX_ISRHandler: Invalid params\n"));
			__DtTestPoint(__DtFunc_SGX_ISRHandler, __DtStep_2)
			return bInterruptProcessed;
		}

		psDeviceNode = (PVRSRV_DEVICE_NODE *)pvData;
		psDevInfo = (PVRSRV_SGXDEV_INFO *)psDeviceNode->pvDevice;

		ui32EventStatus = OSReadHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_EVENT_STATUS);
		ui32EventEnable = OSReadHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_EVENT_HOST_ENABLE);

		
		ui32EventStatus &= ui32EventEnable;

#if defined(SGX_FEATURE_DATA_BREAKPOINTS)
		ui32EventStatus2 = OSReadHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_EVENT_STATUS2);
		ui32EventEnable2 = OSReadHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_EVENT_HOST_ENABLE2);

		
		ui32EventStatus2 &= ui32EventEnable2;
#endif 

		

		if (ui32EventStatus & EUR_CR_EVENT_STATUS_SW_EVENT_MASK)
		{
			__DtTestPoint(__DtFunc_SGX_ISRHandler, __DtStep_3)
			ui32EventClear |= EUR_CR_EVENT_HOST_CLEAR_SW_EVENT_MASK;
		}

#if defined(SGX_FEATURE_DATA_BREAKPOINTS)
		if (ui32EventStatus2 & EUR_CR_EVENT_STATUS2_DATA_BREAKPOINT_UNTRAPPED_MASK)
		{
			__DtTestPoint(__DtFunc_SGX_ISRHandler, __DtStep_4)
			ui32EventClear2 |= EUR_CR_EVENT_HOST_CLEAR2_DATA_BREAKPOINT_UNTRAPPED_MASK;
		}

		if (ui32EventStatus2 & EUR_CR_EVENT_STATUS2_DATA_BREAKPOINT_TRAPPED_MASK)
		{
			__DtTestPoint(__DtFunc_SGX_ISRHandler, __DtStep_5)
			ui32EventClear2 |= EUR_CR_EVENT_HOST_CLEAR2_DATA_BREAKPOINT_TRAPPED_MASK;
		}
#endif 

		if (ui32EventClear || ui32EventClear2)
		{
			__DtTestPoint(__DtFunc_SGX_ISRHandler, __DtStep_6)
			bInterruptProcessed = IMG_TRUE;

			
			ui32EventClear |= EUR_CR_EVENT_HOST_CLEAR_MASTER_INTERRUPT_MASK;

			
			OSWriteHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_EVENT_HOST_CLEAR, ui32EventClear);
			OSWriteHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_EVENT_HOST_CLEAR2, ui32EventClear2);
		}
	}

	__DtTestPoint(__DtFunc_SGX_ISRHandler, __DtStep_7)
	return bInterruptProcessed;
}


static IMG_VOID SGX_MISRHandler (IMG_VOID *pvData)
{
	PVRSRV_DEVICE_NODE	*psDeviceNode = (PVRSRV_DEVICE_NODE *)pvData;
	PVRSRV_SGXDEV_INFO	*psDevInfo = (PVRSRV_SGXDEV_INFO*)psDeviceNode->pvDevice;
	SGXMKIF_HOST_CTL	*psSGXHostCtl = (SGXMKIF_HOST_CTL *)psDevInfo->psSGXHostCtl;

	__DtTestPoint(__DtFunc_SGX_MISRHandler, __DtStep_0)
	if (((psSGXHostCtl->ui32InterruptFlags & PVRSRV_USSE_EDM_INTERRUPT_HWR) != 0UL) &&
		((psSGXHostCtl->ui32InterruptClearFlags & PVRSRV_USSE_EDM_INTERRUPT_HWR) == 0UL))
	{
		__DtTestPoint(__DtFunc_SGX_MISRHandler, __DtStep_1)
		HWRecoveryResetSGX(psDeviceNode, 0, ISR_ID);
	}

#if defined(OS_SUPPORTS_IN_LISR)
	if (psDeviceNode->bReProcessDeviceCommandComplete)
	{
		__DtTestPoint(__DtFunc_SGX_MISRHandler, __DtStep_2)
		SGXScheduleProcessQueuesKM(psDeviceNode);
	}
#endif

	SGXTestActivePowerEvent(psDeviceNode, ISR_ID);
	__DtTestPoint(__DtFunc_SGX_MISRHandler, __DtStep_3)
}
#endif 



#if defined(SUPPORT_MEMORY_TILING)
PVRSRV_ERROR SGX_AllocMemTilingRange(PVRSRV_DEVICE_NODE *psDeviceNode,
										PVRSRV_KERNEL_MEM_INFO	*psMemInfo,
										IMG_UINT32 ui32TilingStride,
										IMG_UINT32 *pui32RangeIndex)
{
#if defined(SGX_FEATURE_BIF_WIDE_TILING_AND_4K_ADDRESS)
	PVRSRV_SGXDEV_INFO *psDevInfo = psDeviceNode->pvDevice;
	IMG_UINT32 i;
	IMG_UINT32 ui32Start;
	IMG_UINT32 ui32End;
	IMG_UINT32 ui32Offset;
	IMG_UINT32 ui32Val;

	
	for(i=0; i<10; i++)
	{
		__DtTestPoint(__DtFunc_SGX_AllocMemTilingRange, __DtStep_0)
		if((psDevInfo->ui32MemTilingUsage & (1U << i)) == 0)
		{
			
			__DtTestPoint(__DtFunc_SGX_AllocMemTilingRange, __DtStep_1)
			psDevInfo->ui32MemTilingUsage |= 1U << i;
			
			*pui32RangeIndex = i;
			goto RangeAllocated;
		}
	}

	PVR_DPF((PVR_DBG_ERROR,"SGX_AllocMemTilingRange: all tiling ranges in use"));
	__DtTestPoint(__DtFunc_SGX_AllocMemTilingRange, __DtStep_2)
	return PVRSRV_ERROR_EXCEEDED_HW_LIMITS;

RangeAllocated:
	ui32Offset = EUR_CR_BIF_TILE0 + (i<<2);

	ui32Start = psMemInfo->sDevVAddr.uiAddr;
	ui32End = ui32Start + psMemInfo->ui32AllocSize + SGX_MMU_PAGE_SIZE - 1;

	ui32Val = ((ui32TilingStride << EUR_CR_BIF_TILE0_CFG_SHIFT) & EUR_CR_BIF_TILE0_CFG_MASK)
			| (((ui32End>>20) << EUR_CR_BIF_TILE0_MAX_ADDRESS_SHIFT) & EUR_CR_BIF_TILE0_MAX_ADDRESS_MASK)
			| (((ui32Start>>20) << EUR_CR_BIF_TILE0_MIN_ADDRESS_SHIFT) & EUR_CR_BIF_TILE0_MIN_ADDRESS_MASK)
			| (0x8 << EUR_CR_BIF_TILE0_CFG_SHIFT);

	
	OSWriteHWReg(psDevInfo->pvRegsBaseKM, ui32Offset, ui32Val);
	PDUMPREG(SGX_PDUMPREG_NAME, ui32Offset, ui32Val);

	ui32Offset = EUR_CR_BIF_TILE0_ADDR_EXT + (i<<2);

	ui32Val = (((ui32End>>12) << EUR_CR_BIF_TILE0_ADDR_EXT_MAX_SHIFT) & EUR_CR_BIF_TILE0_ADDR_EXT_MAX_MASK)
			| (((ui32Start>>12) << EUR_CR_BIF_TILE0_ADDR_EXT_MIN_SHIFT) & EUR_CR_BIF_TILE0_ADDR_EXT_MIN_MASK);

	
	OSWriteHWReg(psDevInfo->pvRegsBaseKM, ui32Offset, ui32Val);
	PDUMPREG(SGX_PDUMPREG_NAME, ui32Offset, ui32Val);

	__DtTestPoint(__DtFunc_SGX_AllocMemTilingRange, __DtStep_3)
	return PVRSRV_OK;
#else
	PVR_UNREFERENCED_PARAMETER(psDeviceNode);
	PVR_UNREFERENCED_PARAMETER(psMemInfo);
	PVR_UNREFERENCED_PARAMETER(ui32TilingStride);
	PVR_UNREFERENCED_PARAMETER(pui32RangeIndex);

	PVR_DPF((PVR_DBG_ERROR,"SGX_AllocMemTilingRange: device does not support memory tiling"));
	__DtTestPoint(__DtFunc_SGX_AllocMemTilingRange, __DtStep_4)
	return PVRSRV_ERROR_NOT_SUPPORTED;
#endif
}

PVRSRV_ERROR SGX_FreeMemTilingRange(PVRSRV_DEVICE_NODE *psDeviceNode,
										IMG_UINT32 ui32RangeIndex)
{
#if defined(SGX_FEATURE_BIF_WIDE_TILING_AND_4K_ADDRESS)
	PVRSRV_SGXDEV_INFO *psDevInfo = psDeviceNode->pvDevice;
	IMG_UINT32 ui32Offset;
	IMG_UINT32 ui32Val;

	if(ui32RangeIndex >= 10)
	{
		__DtTestPoint(__DtFunc_SGX_FreeMemTilingRange, __DtStep_0)
		PVR_DPF((PVR_DBG_ERROR,"SGX_FreeMemTilingRange: invalid Range index "));
		__DtTestPoint(__DtFunc_SGX_FreeMemTilingRange, __DtStep_1)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	
	psDevInfo->ui32MemTilingUsage &= ~(1<<ui32RangeIndex);

	
	ui32Offset = EUR_CR_BIF_TILE0 + (ui32RangeIndex<<2);
	ui32Val = 0;

	
	OSWriteHWReg(psDevInfo->pvRegsBaseKM, ui32Offset, ui32Val);
	PDUMPREG(SGX_PDUMPREG_NAME, ui32Offset, ui32Val);

	__DtTestPoint(__DtFunc_SGX_FreeMemTilingRange, __DtStep_2)
	return PVRSRV_OK;
#else
	PVR_UNREFERENCED_PARAMETER(psDeviceNode);
	PVR_UNREFERENCED_PARAMETER(ui32RangeIndex);

	PVR_DPF((PVR_DBG_ERROR,"SGX_FreeMemTilingRange: device does not support memory tiling"));
	__DtTestPoint(__DtFunc_SGX_FreeMemTilingRange, __DtStep_3)
	return PVRSRV_ERROR_NOT_SUPPORTED;
#endif
}
#endif


PVRSRV_ERROR SGXRegisterDevice (PVRSRV_DEVICE_NODE *psDeviceNode)
{
	DEVICE_MEMORY_INFO *psDevMemoryInfo;
	DEVICE_MEMORY_HEAP_INFO *psDeviceMemoryHeap;

	
	__DtTestPoint(__DtFunc_SGXRegisterDevice, __DtStep_0)
	psDeviceNode->sDevId.eDeviceType		= DEV_DEVICE_TYPE;
	psDeviceNode->sDevId.eDeviceClass		= DEV_DEVICE_CLASS;
#if defined(PDUMP)
	{
		
		SGX_DEVICE_MAP *psSGXDeviceMemMap;
		SysGetDeviceMemoryMap(PVRSRV_DEVICE_TYPE_SGX,
							  (IMG_VOID**)&psSGXDeviceMemMap);

		psDeviceNode->sDevId.pszPDumpDevName = psSGXDeviceMemMap->pszPDumpDevName;
		PVR_ASSERT(psDeviceNode->sDevId.pszPDumpDevName != IMG_NULL);
	}
	
	psDeviceNode->sDevId.pszPDumpRegName	= SGX_PDUMPREG_NAME;
#endif 

	psDeviceNode->pfnInitDevice		= &DevInitSGXPart1;
	psDeviceNode->pfnDeInitDevice	= &DevDeInitSGX;

	psDeviceNode->pfnInitDeviceCompatCheck	= &SGXDevInitCompatCheck;
#if defined(PDUMP)
	psDeviceNode->pfnPDumpInitDevice = &SGXResetPDump;
	psDeviceNode->pfnMMUGetContextID = &MMU_GetPDumpContextID;
#endif
	

	psDeviceNode->pfnMMUInitialise = &MMU_Initialise;
	psDeviceNode->pfnMMUFinalise = &MMU_Finalise;
	psDeviceNode->pfnMMUInsertHeap = &MMU_InsertHeap;
	psDeviceNode->pfnMMUCreate = &MMU_Create;
	psDeviceNode->pfnMMUDelete = &MMU_Delete;
	psDeviceNode->pfnMMUAlloc = &MMU_Alloc;
	psDeviceNode->pfnMMUFree = &MMU_Free;
	psDeviceNode->pfnMMUMapPages = &MMU_MapPages;
	psDeviceNode->pfnMMUMapShadow = &MMU_MapShadow;
	psDeviceNode->pfnMMUUnmapPages = &MMU_UnmapPages;
	psDeviceNode->pfnMMUMapScatter = &MMU_MapScatter;
	psDeviceNode->pfnMMUGetPhysPageAddr = &MMU_GetPhysPageAddr;
	psDeviceNode->pfnMMUGetPDDevPAddr = &MMU_GetPDDevPAddr;
#if defined(SUPPORT_PDUMP_MULTI_PROCESS)
	psDeviceNode->pfnMMUIsHeapShared = &MMU_IsHeapShared;
#endif

#if defined (SYS_USING_INTERRUPTS)
	

	psDeviceNode->pfnDeviceISR = SGX_ISRHandler;
	psDeviceNode->pfnDeviceMISR = SGX_MISRHandler;
#endif

#if defined(SUPPORT_MEMORY_TILING)
	psDeviceNode->pfnAllocMemTilingRange = SGX_AllocMemTilingRange;
	psDeviceNode->pfnFreeMemTilingRange = SGX_FreeMemTilingRange;
#endif

	

	psDeviceNode->pfnDeviceCommandComplete = &SGXCommandComplete;

	

	psDevMemoryInfo = &psDeviceNode->sDevMemoryInfo;
	
	psDevMemoryInfo->ui32AddressSpaceSizeLog2 = SGX_FEATURE_ADDRESS_SPACE_SIZE;

	
	psDevMemoryInfo->ui32Flags = 0;

	
	if(OSAllocMem( PVRSRV_OS_PAGEABLE_HEAP,
					 sizeof(DEVICE_MEMORY_HEAP_INFO) * SGX_MAX_HEAP_ID,
					 (IMG_VOID **)&psDevMemoryInfo->psDeviceMemoryHeap, 0,
					 "Array of Device Memory Heap Info") != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXRegisterDevice, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"SGXRegisterDevice : Failed to alloc memory for DEVICE_MEMORY_HEAP_INFO"));
		__DtTestPoint(__DtFunc_SGXRegisterDevice, __DtStep_2)
		return (PVRSRV_ERROR_OUT_OF_MEMORY);
	}
	OSMemSet(psDevMemoryInfo->psDeviceMemoryHeap, 0, sizeof(DEVICE_MEMORY_HEAP_INFO) * SGX_MAX_HEAP_ID);

	psDeviceMemoryHeap = psDevMemoryInfo->psDeviceMemoryHeap;

	


	
	psDeviceMemoryHeap->ui32HeapID = HEAP_ID( PVRSRV_DEVICE_TYPE_SGX, SGX_GENERAL_HEAP_ID);
	psDeviceMemoryHeap->sDevVAddrBase.uiAddr = SGX_GENERAL_HEAP_BASE;
	psDeviceMemoryHeap->ui32HeapSize = SGX_GENERAL_HEAP_SIZE;
	psDeviceMemoryHeap->ui32Attribs = PVRSRV_HAP_WRITECOMBINE
														| PVRSRV_MEM_RAM_BACKED_ALLOCATION
														| PVRSRV_HAP_SINGLE_PROCESS;
	psDeviceMemoryHeap->pszName = "General";
	psDeviceMemoryHeap->pszBSName = "General BS";
	psDeviceMemoryHeap->DevMemHeapType = DEVICE_MEMORY_HEAP_PERCONTEXT;
	
	psDeviceMemoryHeap->ui32DataPageSize = SGX_MMU_PAGE_SIZE;
#if !defined(SUPPORT_SGX_GENERAL_MAPPING_HEAP)
	
	psDevMemoryInfo->ui32MappingHeapID = (IMG_UINT32)(psDeviceMemoryHeap - psDevMemoryInfo->psDeviceMemoryHeap);
#endif
	psDeviceMemoryHeap++;


	
	psDeviceMemoryHeap->ui32HeapID = HEAP_ID( PVRSRV_DEVICE_TYPE_SGX, SGX_TADATA_HEAP_ID);
	psDeviceMemoryHeap->sDevVAddrBase.uiAddr = SGX_TADATA_HEAP_BASE;
	psDeviceMemoryHeap->ui32HeapSize = SGX_TADATA_HEAP_SIZE;
	psDeviceMemoryHeap->ui32Attribs = PVRSRV_HAP_WRITECOMBINE
														| PVRSRV_MEM_RAM_BACKED_ALLOCATION
														| PVRSRV_HAP_MULTI_PROCESS;
	psDeviceMemoryHeap->pszName = "TA Data";
	psDeviceMemoryHeap->pszBSName = "TA Data BS";
	psDeviceMemoryHeap->DevMemHeapType = DEVICE_MEMORY_HEAP_PERCONTEXT;
	
	psDeviceMemoryHeap->ui32DataPageSize = SGX_MMU_PAGE_SIZE;
	psDeviceMemoryHeap++;


	
	psDeviceMemoryHeap->ui32HeapID = HEAP_ID( PVRSRV_DEVICE_TYPE_SGX, SGX_KERNEL_CODE_HEAP_ID);
	psDeviceMemoryHeap->sDevVAddrBase.uiAddr = SGX_KERNEL_CODE_HEAP_BASE;
	psDeviceMemoryHeap->ui32HeapSize = SGX_KERNEL_CODE_HEAP_SIZE;
	psDeviceMemoryHeap->ui32Attribs = PVRSRV_HAP_WRITECOMBINE
															| PVRSRV_MEM_RAM_BACKED_ALLOCATION
															| PVRSRV_HAP_MULTI_PROCESS;
	psDeviceMemoryHeap->pszName = "Kernel Code";
	psDeviceMemoryHeap->pszBSName = "Kernel Code BS";
	psDeviceMemoryHeap->DevMemHeapType = DEVICE_MEMORY_HEAP_SHARED_EXPORTED;
	
	psDeviceMemoryHeap->ui32DataPageSize = SGX_MMU_PAGE_SIZE;
	psDeviceMemoryHeap++;


	
	psDeviceMemoryHeap->ui32HeapID = HEAP_ID( PVRSRV_DEVICE_TYPE_SGX, SGX_KERNEL_DATA_HEAP_ID);
	psDeviceMemoryHeap->sDevVAddrBase.uiAddr = SGX_KERNEL_DATA_HEAP_BASE;
	psDeviceMemoryHeap->ui32HeapSize = SGX_KERNEL_DATA_HEAP_SIZE;
	psDeviceMemoryHeap->ui32Attribs = PVRSRV_HAP_WRITECOMBINE
																| PVRSRV_MEM_RAM_BACKED_ALLOCATION
																| PVRSRV_HAP_MULTI_PROCESS;
	psDeviceMemoryHeap->pszName = "KernelData";
	psDeviceMemoryHeap->pszBSName = "KernelData BS";
	psDeviceMemoryHeap->DevMemHeapType = DEVICE_MEMORY_HEAP_SHARED_EXPORTED;
	
	psDeviceMemoryHeap->ui32DataPageSize = SGX_MMU_PAGE_SIZE;
	psDeviceMemoryHeap++;


	
	psDeviceMemoryHeap->ui32HeapID = HEAP_ID( PVRSRV_DEVICE_TYPE_SGX, SGX_PIXELSHADER_HEAP_ID);
	psDeviceMemoryHeap->sDevVAddrBase.uiAddr = SGX_PIXELSHADER_HEAP_BASE;
	





	psDeviceMemoryHeap->ui32HeapSize = ((10 << SGX_USE_CODE_SEGMENT_RANGE_BITS) - 0x00001000);
	PVR_ASSERT(psDeviceMemoryHeap->ui32HeapSize <= SGX_PIXELSHADER_HEAP_SIZE);
	psDeviceMemoryHeap->ui32Attribs = PVRSRV_HAP_WRITECOMBINE
																| PVRSRV_MEM_RAM_BACKED_ALLOCATION
																| PVRSRV_HAP_SINGLE_PROCESS;
	psDeviceMemoryHeap->pszName = "PixelShaderUSSE";
	psDeviceMemoryHeap->pszBSName = "PixelShaderUSSE BS";
	psDeviceMemoryHeap->DevMemHeapType = DEVICE_MEMORY_HEAP_PERCONTEXT;
	
	psDeviceMemoryHeap->ui32DataPageSize = SGX_MMU_PAGE_SIZE;
	psDeviceMemoryHeap++;


	
	psDeviceMemoryHeap->ui32HeapID = HEAP_ID( PVRSRV_DEVICE_TYPE_SGX, SGX_VERTEXSHADER_HEAP_ID);
	psDeviceMemoryHeap->sDevVAddrBase.uiAddr = SGX_VERTEXSHADER_HEAP_BASE;
	
	psDeviceMemoryHeap->ui32HeapSize = ((4 << SGX_USE_CODE_SEGMENT_RANGE_BITS) - 0x00001000);
	PVR_ASSERT(psDeviceMemoryHeap->ui32HeapSize <= SGX_VERTEXSHADER_HEAP_SIZE);
	psDeviceMemoryHeap->ui32Attribs = PVRSRV_HAP_WRITECOMBINE
																| PVRSRV_MEM_RAM_BACKED_ALLOCATION
																| PVRSRV_HAP_SINGLE_PROCESS;
	psDeviceMemoryHeap->pszName = "VertexShaderUSSE";
	psDeviceMemoryHeap->pszBSName = "VertexShaderUSSE BS";
	psDeviceMemoryHeap->DevMemHeapType = DEVICE_MEMORY_HEAP_PERCONTEXT;
	
	psDeviceMemoryHeap->ui32DataPageSize = SGX_MMU_PAGE_SIZE;
	psDeviceMemoryHeap++;


	
	psDeviceMemoryHeap->ui32HeapID = HEAP_ID( PVRSRV_DEVICE_TYPE_SGX, SGX_PDSPIXEL_CODEDATA_HEAP_ID);
	psDeviceMemoryHeap->sDevVAddrBase.uiAddr = SGX_PDSPIXEL_CODEDATA_HEAP_BASE;
	psDeviceMemoryHeap->ui32HeapSize = SGX_PDSPIXEL_CODEDATA_HEAP_SIZE;
	psDeviceMemoryHeap->ui32Attribs = PVRSRV_HAP_WRITECOMBINE
																| PVRSRV_MEM_RAM_BACKED_ALLOCATION
																| PVRSRV_HAP_SINGLE_PROCESS;
	psDeviceMemoryHeap->pszName = "PDSPixelCodeData";
	psDeviceMemoryHeap->pszBSName = "PDSPixelCodeData BS";
	psDeviceMemoryHeap->DevMemHeapType = DEVICE_MEMORY_HEAP_PERCONTEXT;
	
	psDeviceMemoryHeap->ui32DataPageSize = SGX_MMU_PAGE_SIZE;
	psDeviceMemoryHeap++;


	
	psDeviceMemoryHeap->ui32HeapID = HEAP_ID( PVRSRV_DEVICE_TYPE_SGX, SGX_PDSVERTEX_CODEDATA_HEAP_ID);
	psDeviceMemoryHeap->sDevVAddrBase.uiAddr = SGX_PDSVERTEX_CODEDATA_HEAP_BASE;
	psDeviceMemoryHeap->ui32HeapSize = SGX_PDSVERTEX_CODEDATA_HEAP_SIZE;
	psDeviceMemoryHeap->ui32Attribs = PVRSRV_HAP_WRITECOMBINE
																| PVRSRV_MEM_RAM_BACKED_ALLOCATION
																| PVRSRV_HAP_SINGLE_PROCESS;
	psDeviceMemoryHeap->pszName = "PDSVertexCodeData";
	psDeviceMemoryHeap->pszBSName = "PDSVertexCodeData BS";
	psDeviceMemoryHeap->DevMemHeapType = DEVICE_MEMORY_HEAP_PERCONTEXT;
	
	psDeviceMemoryHeap->ui32DataPageSize = SGX_MMU_PAGE_SIZE;
	psDeviceMemoryHeap++;


	
	psDeviceMemoryHeap->ui32HeapID = HEAP_ID( PVRSRV_DEVICE_TYPE_SGX, SGX_SYNCINFO_HEAP_ID);
	psDeviceMemoryHeap->sDevVAddrBase.uiAddr = SGX_SYNCINFO_HEAP_BASE;
	psDeviceMemoryHeap->ui32HeapSize = SGX_SYNCINFO_HEAP_SIZE;
	psDeviceMemoryHeap->ui32Attribs = PVRSRV_HAP_WRITECOMBINE
														| PVRSRV_MEM_RAM_BACKED_ALLOCATION
														| PVRSRV_HAP_MULTI_PROCESS;
	psDeviceMemoryHeap->pszName = "CacheCoherent";
	psDeviceMemoryHeap->pszBSName = "CacheCoherent BS";
	psDeviceMemoryHeap->DevMemHeapType = DEVICE_MEMORY_HEAP_SHARED_EXPORTED;
	
	psDeviceMemoryHeap->ui32DataPageSize = SGX_MMU_PAGE_SIZE;
	
	psDevMemoryInfo->ui32SyncHeapID = (IMG_UINT32)(psDeviceMemoryHeap - psDevMemoryInfo->psDeviceMemoryHeap);
	psDeviceMemoryHeap++;


	
	psDeviceMemoryHeap->ui32HeapID = HEAP_ID( PVRSRV_DEVICE_TYPE_SGX, SGX_3DPARAMETERS_HEAP_ID);
	psDeviceMemoryHeap->sDevVAddrBase.uiAddr = SGX_3DPARAMETERS_HEAP_BASE;
	psDeviceMemoryHeap->ui32HeapSize = SGX_3DPARAMETERS_HEAP_SIZE;
	psDeviceMemoryHeap->pszName = "3DParameters";
	psDeviceMemoryHeap->pszBSName = "3DParameters BS";
#if defined(SUPPORT_PERCONTEXT_PB)
	psDeviceMemoryHeap->ui32Attribs = PVRSRV_HAP_WRITECOMBINE
															| PVRSRV_MEM_RAM_BACKED_ALLOCATION
															| PVRSRV_HAP_SINGLE_PROCESS;
	psDeviceMemoryHeap->DevMemHeapType = DEVICE_MEMORY_HEAP_PERCONTEXT;
#else
	psDeviceMemoryHeap->ui32Attribs = PVRSRV_HAP_WRITECOMBINE
													| PVRSRV_MEM_RAM_BACKED_ALLOCATION
													| PVRSRV_HAP_MULTI_PROCESS;
	psDeviceMemoryHeap->DevMemHeapType = DEVICE_MEMORY_HEAP_SHARED_EXPORTED;
#endif
	
	psDeviceMemoryHeap->ui32DataPageSize = SGX_MMU_PAGE_SIZE;
	psDeviceMemoryHeap++;


#if defined(SUPPORT_SGX_GENERAL_MAPPING_HEAP)
	
	psDeviceMemoryHeap->ui32HeapID = HEAP_ID( PVRSRV_DEVICE_TYPE_SGX, SGX_GENERAL_MAPPING_HEAP_ID);
	psDeviceMemoryHeap->sDevVAddrBase.uiAddr = SGX_GENERAL_MAPPING_HEAP_BASE;
	psDeviceMemoryHeap->ui32HeapSize = SGX_GENERAL_MAPPING_HEAP_SIZE;
	psDeviceMemoryHeap->ui32Attribs = PVRSRV_HAP_WRITECOMBINE | PVRSRV_HAP_MULTI_PROCESS;
	psDeviceMemoryHeap->pszName = "GeneralMapping";
	psDeviceMemoryHeap->pszBSName = "GeneralMapping BS";
	#if defined(SGX_FEATURE_MULTIPLE_MEM_CONTEXTS) && defined(FIX_HW_BRN_23410)
	






		psDeviceMemoryHeap->DevMemHeapType = DEVICE_MEMORY_HEAP_SHARED_EXPORTED;
	#else 
		psDeviceMemoryHeap->DevMemHeapType = DEVICE_MEMORY_HEAP_PERCONTEXT;
	#endif 

	
	psDeviceMemoryHeap->ui32DataPageSize = SGX_MMU_PAGE_SIZE;
	
	psDevMemoryInfo->ui32MappingHeapID = (IMG_UINT32)(psDeviceMemoryHeap - psDevMemoryInfo->psDeviceMemoryHeap);
	psDeviceMemoryHeap++;
#endif 


#if defined(SGX_FEATURE_2D_HARDWARE)
	
	psDeviceMemoryHeap->ui32HeapID = HEAP_ID( PVRSRV_DEVICE_TYPE_SGX, SGX_2D_HEAP_ID);
	psDeviceMemoryHeap->sDevVAddrBase.uiAddr = SGX_2D_HEAP_BASE;
	psDeviceMemoryHeap->ui32HeapSize = SGX_2D_HEAP_SIZE;
	psDeviceMemoryHeap->ui32Attribs = PVRSRV_HAP_WRITECOMBINE
														| PVRSRV_MEM_RAM_BACKED_ALLOCATION
														| PVRSRV_HAP_SINGLE_PROCESS;
	psDeviceMemoryHeap->pszName = "2D";
	psDeviceMemoryHeap->pszBSName = "2D BS";
	
	psDeviceMemoryHeap->DevMemHeapType = DEVICE_MEMORY_HEAP_SHARED_EXPORTED;
	
	psDeviceMemoryHeap->ui32DataPageSize = SGX_MMU_PAGE_SIZE;
	psDeviceMemoryHeap++;
#endif 


#if defined(FIX_HW_BRN_26915)
	
	
	psDeviceMemoryHeap->ui32HeapID = HEAP_ID( PVRSRV_DEVICE_TYPE_SGX, SGX_CGBUFFER_HEAP_ID);
	psDeviceMemoryHeap->sDevVAddrBase.uiAddr = SGX_CGBUFFER_HEAP_BASE;
	psDeviceMemoryHeap->ui32HeapSize = SGX_CGBUFFER_HEAP_SIZE;
	psDeviceMemoryHeap->ui32Attribs = PVRSRV_HAP_WRITECOMBINE
														| PVRSRV_MEM_RAM_BACKED_ALLOCATION
														| PVRSRV_HAP_SINGLE_PROCESS;
	psDeviceMemoryHeap->pszName = "CGBuffer";
	psDeviceMemoryHeap->pszBSName = "CGBuffer BS";

	psDeviceMemoryHeap->DevMemHeapType = DEVICE_MEMORY_HEAP_PERCONTEXT;
	
	psDeviceMemoryHeap->ui32DataPageSize = SGX_MMU_PAGE_SIZE;
	psDeviceMemoryHeap++;
#endif 

	
	psDevMemoryInfo->ui32HeapCount = (IMG_UINT32)(psDeviceMemoryHeap - psDevMemoryInfo->psDeviceMemoryHeap);

	__DtTestPoint(__DtFunc_SGXRegisterDevice, __DtStep_3)
	return PVRSRV_OK;
}

#if defined(PDUMP)
static
PVRSRV_ERROR SGXResetPDump(PVRSRV_DEVICE_NODE *psDeviceNode)
{
	PVRSRV_SGXDEV_INFO *psDevInfo = (PVRSRV_SGXDEV_INFO *)(psDeviceNode->pvDevice);
	__DtTestPoint(__DtFunc_SGXResetPDump, __DtStep_0)
	psDevInfo->psKernelCCBInfo->ui32CCBDumpWOff = 0;
	PVR_DPF((PVR_DBG_MESSAGE, "Reset pdump CCB write offset."));
	
	__DtTestPoint(__DtFunc_SGXResetPDump, __DtStep_1)
	return PVRSRV_OK;
}
#endif 


IMG_EXPORT
PVRSRV_ERROR SGXGetClientInfoKM(IMG_HANDLE					hDevCookie,
								SGX_CLIENT_INFO*		psClientInfo)
{
	PVRSRV_SGXDEV_INFO *psDevInfo = (PVRSRV_SGXDEV_INFO *)((PVRSRV_DEVICE_NODE *)hDevCookie)->pvDevice;

	

	__DtTestPoint(__DtFunc_SGXGetClientInfoKM, __DtStep_0)
	psDevInfo->ui32ClientRefCount++;

	

	psClientInfo->ui32ProcessID = OSGetCurrentProcessIDKM();

	

	OSMemCopy(&psClientInfo->asDevData, &psDevInfo->asSGXDevData, sizeof(psClientInfo->asDevData));

	
	__DtTestPoint(__DtFunc_SGXGetClientInfoKM, __DtStep_1)
	return PVRSRV_OK;
}


IMG_VOID SGXPanic(PVRSRV_SGXDEV_INFO	*psDevInfo)
{
	__DtTestPoint(__DtFunc_SGXPanic, __DtStep_0)
	PVR_LOG(("SGX panic"));
	SGXDumpDebugInfo(psDevInfo, IMG_FALSE);
	OSPanic();
	__DtTestPoint(__DtFunc_SGXPanic, __DtStep_1)
}


PVRSRV_ERROR SGXDevInitCompatCheck(PVRSRV_DEVICE_NODE *psDeviceNode)
{
	PVRSRV_ERROR	eError;
	PVRSRV_SGXDEV_INFO 				*psDevInfo;
	IMG_UINT32 			ui32BuildOptions, ui32BuildOptionsMismatch;
#if !defined(NO_HARDWARE)
	PPVRSRV_KERNEL_MEM_INFO			psMemInfo;
	PVRSRV_SGX_MISCINFO_INFO		*psSGXMiscInfoInt; 	
	PVRSRV_SGX_MISCINFO_FEATURES	*psSGXFeatures;
	SGX_MISCINFO_STRUCT_SIZES		*psSGXStructSizes;	
	IMG_BOOL						bStructSizesFailed;

	
	IMG_BOOL	bCheckCoreRev;
	const IMG_UINT32 aui32CoreRevExceptions[] =
	{
		0x10100, 0x10101
	};
	const IMG_UINT32	ui32NumCoreExceptions = sizeof(aui32CoreRevExceptions) / (2*sizeof(IMG_UINT32));
	IMG_UINT	i;
#endif

	
	if(psDeviceNode->sDevId.eDeviceType != PVRSRV_DEVICE_TYPE_SGX)
	{
		__DtTestPoint(__DtFunc_SGXDevInitCompatCheck, __DtStep_0)
		PVR_LOG(("(FAIL) SGXInit: Device not of type SGX"));
		eError = PVRSRV_ERROR_INVALID_PARAMS;
		goto chk_exit;
	}

	psDevInfo = psDeviceNode->pvDevice;

	
	
	ui32BuildOptions = (SGX_BUILD_OPTIONS);
	if (ui32BuildOptions != psDevInfo->ui32ClientBuildOptions)
	{
		__DtTestPoint(__DtFunc_SGXDevInitCompatCheck, __DtStep_1)
		ui32BuildOptionsMismatch = ui32BuildOptions ^ psDevInfo->ui32ClientBuildOptions;
		if ( (psDevInfo->ui32ClientBuildOptions & ui32BuildOptionsMismatch) != 0)
		{
			__DtTestPoint(__DtFunc_SGXDevInitCompatCheck, __DtStep_2)
			PVR_LOG(("(FAIL) SGXInit: Mismatch in client-side and KM driver build options; "
				"extra options present in client-side driver: (0x%x). Please check sgx_options.h",
				psDevInfo->ui32ClientBuildOptions & ui32BuildOptionsMismatch ));
		}

		if ( (ui32BuildOptions & ui32BuildOptionsMismatch) != 0)
		{
			__DtTestPoint(__DtFunc_SGXDevInitCompatCheck, __DtStep_3)
			PVR_LOG(("(FAIL) SGXInit: Mismatch in client-side and KM driver build options; "
				"extra options present in KM: (0x%x). Please check sgx_options.h",
				ui32BuildOptions & ui32BuildOptionsMismatch ));
		}
		eError = PVRSRV_ERROR_BUILD_MISMATCH;
		goto chk_exit;
	}
	else
	{
		__DtTestPoint(__DtFunc_SGXDevInitCompatCheck, __DtStep_4)
		PVR_DPF((PVR_DBG_MESSAGE, "SGXInit: Client-side and KM driver build options match. [ OK ]"));
	}

#if !defined (NO_HARDWARE)
	psMemInfo = psDevInfo->psKernelSGXMiscMemInfo;

	
	psSGXMiscInfoInt = psMemInfo->pvLinAddrKM;
	psSGXMiscInfoInt->ui32MiscInfoFlags = 0;
	psSGXMiscInfoInt->ui32MiscInfoFlags |= PVRSRV_USSE_MISCINFO_GET_STRUCT_SIZES;
	eError = SGXGetMiscInfoUkernel(psDevInfo, psDeviceNode);

	
	if(eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXDevInitCompatCheck, __DtStep_5)
		PVR_LOG(("(FAIL) SGXInit: Unable to validate device DDK version"));
		goto chk_exit;
	}
	psSGXFeatures = &((PVRSRV_SGX_MISCINFO_INFO*)(psMemInfo->pvLinAddrKM))->sSGXFeatures;
	if( (psSGXFeatures->ui32DDKVersion !=
		((PVRVERSION_MAJ << 16) |
		 (PVRVERSION_MIN << 8) |
		  PVRVERSION_BRANCH) ) ||
		(psSGXFeatures->ui32DDKBuild != PVRVERSION_BUILD) )
	{
		__DtTestPoint(__DtFunc_SGXDevInitCompatCheck, __DtStep_6)
		PVR_LOG(("(FAIL) SGXInit: Incompatible driver DDK revision (%d)/device DDK revision (%d).",
				PVRVERSION_BUILD, psSGXFeatures->ui32DDKBuild));
		eError = PVRSRV_ERROR_DDK_VERSION_MISMATCH;
		PVR_DBG_BREAK;
		goto chk_exit;
	}
	else
	{
		__DtTestPoint(__DtFunc_SGXDevInitCompatCheck, __DtStep_7)
		PVR_DPF((PVR_DBG_MESSAGE, "SGXInit: driver DDK (%d) and device DDK (%d) match. [ OK ]",
				PVRVERSION_BUILD, psSGXFeatures->ui32DDKBuild));
	}

	
	if (psSGXFeatures->ui32CoreRevSW == 0)
	{
		

		__DtTestPoint(__DtFunc_SGXDevInitCompatCheck, __DtStep_8)
		PVR_LOG(("SGXInit: HW core rev (%x) check skipped.",
				psSGXFeatures->ui32CoreRev));
	}
	else
	{
		
		__DtTestPoint(__DtFunc_SGXDevInitCompatCheck, __DtStep_9)
		bCheckCoreRev = IMG_TRUE;
		for(i=0; i<ui32NumCoreExceptions; i+=2)
		{
			__DtTestPoint(__DtFunc_SGXDevInitCompatCheck, __DtStep_10)
			if( (psSGXFeatures->ui32CoreRev==aui32CoreRevExceptions[i]) &&
				(psSGXFeatures->ui32CoreRevSW==aui32CoreRevExceptions[i+1])	)
			{
				__DtTestPoint(__DtFunc_SGXDevInitCompatCheck, __DtStep_11)
				PVR_LOG(("SGXInit: HW core rev (%x), SW core rev (%x) check skipped.",
						psSGXFeatures->ui32CoreRev,
						psSGXFeatures->ui32CoreRevSW));
				bCheckCoreRev = IMG_FALSE;
			}
		}

		if (bCheckCoreRev)
		{
			__DtTestPoint(__DtFunc_SGXDevInitCompatCheck, __DtStep_12)
			if (psSGXFeatures->ui32CoreRev != psSGXFeatures->ui32CoreRevSW)
			{
				__DtTestPoint(__DtFunc_SGXDevInitCompatCheck, __DtStep_13)
				PVR_LOG(("(FAIL) SGXInit: Incompatible HW core rev (%x) and SW core rev (%x).",
						psSGXFeatures->ui32CoreRev, psSGXFeatures->ui32CoreRevSW));
						eError = PVRSRV_ERROR_BUILD_MISMATCH;
						goto chk_exit;
			}
			else
			{
				__DtTestPoint(__DtFunc_SGXDevInitCompatCheck, __DtStep_14)
				PVR_DPF((PVR_DBG_MESSAGE, "SGXInit: HW core rev (%x) and SW core rev (%x) match. [ OK ]",
						psSGXFeatures->ui32CoreRev, psSGXFeatures->ui32CoreRevSW));
			}
		}
	}

	
	psSGXStructSizes = &((PVRSRV_SGX_MISCINFO_INFO*)(psMemInfo->pvLinAddrKM))->sSGXStructSizes;

	bStructSizesFailed = IMG_FALSE;

	CHECK_SIZE(HOST_CTL);
	CHECK_SIZE(COMMAND);
#if defined(SGX_FEATURE_2D_HARDWARE)
	CHECK_SIZE(2DCMD);
	CHECK_SIZE(2DCMD_SHARED);
#endif
	CHECK_SIZE(CMDTA);
	CHECK_SIZE(CMDTA_SHARED);
	CHECK_SIZE(TRANSFERCMD);
	CHECK_SIZE(TRANSFERCMD_SHARED);

	CHECK_SIZE(3DREGISTERS);
	CHECK_SIZE(HWPBDESC);
	CHECK_SIZE(HWRENDERCONTEXT);
	CHECK_SIZE(HWRENDERDETAILS);
	CHECK_SIZE(HWRTDATA);
	CHECK_SIZE(HWRTDATASET);
	CHECK_SIZE(HWTRANSFERCONTEXT);

	if (bStructSizesFailed == IMG_TRUE)
	{
		__DtTestPoint(__DtFunc_SGXDevInitCompatCheck, __DtStep_15)
		PVR_LOG(("(FAIL) SGXInit: Mismatch in SGXMKIF structure sizes."));
		eError = PVRSRV_ERROR_BUILD_MISMATCH;
		goto chk_exit;
	}
	else
	{
		__DtTestPoint(__DtFunc_SGXDevInitCompatCheck, __DtStep_16)
		PVR_DPF((PVR_DBG_MESSAGE, "SGXInit: SGXMKIF structure sizes match. [ OK ]"));
	}

	
	ui32BuildOptions = psSGXFeatures->ui32BuildOptions;
	if (ui32BuildOptions != (SGX_BUILD_OPTIONS))
	{
		__DtTestPoint(__DtFunc_SGXDevInitCompatCheck, __DtStep_17)
		ui32BuildOptionsMismatch = ui32BuildOptions ^ (SGX_BUILD_OPTIONS);
		if ( ((SGX_BUILD_OPTIONS) & ui32BuildOptionsMismatch) != 0)
		{
			__DtTestPoint(__DtFunc_SGXDevInitCompatCheck, __DtStep_18)
			PVR_LOG(("(FAIL) SGXInit: Mismatch in driver and microkernel build options; "
				"extra options present in driver: (0x%x). Please check sgx_options.h",
				(SGX_BUILD_OPTIONS) & ui32BuildOptionsMismatch ));
		}

		if ( (ui32BuildOptions & ui32BuildOptionsMismatch) != 0)
		{
			__DtTestPoint(__DtFunc_SGXDevInitCompatCheck, __DtStep_19)
			PVR_LOG(("(FAIL) SGXInit: Mismatch in driver and microkernel build options; "
				"extra options present in microkernel: (0x%x). Please check sgx_options.h",
				ui32BuildOptions & ui32BuildOptionsMismatch ));
		}
		eError = PVRSRV_ERROR_BUILD_MISMATCH;
		goto chk_exit;
	}
	else
	{
		__DtTestPoint(__DtFunc_SGXDevInitCompatCheck, __DtStep_20)
		PVR_DPF((PVR_DBG_MESSAGE, "SGXInit: Driver and microkernel build options match. [ OK ]"));
	}
#endif 

	eError = PVRSRV_OK;
chk_exit:
#if defined(IGNORE_SGX_INIT_COMPATIBILITY_CHECK)
	return PVRSRV_OK;
#else
	return eError;
#endif
}

static
PVRSRV_ERROR SGXGetMiscInfoUkernel(PVRSRV_SGXDEV_INFO	*psDevInfo,
								   PVRSRV_DEVICE_NODE 	*psDeviceNode)
{
	PVRSRV_ERROR		eError;
	SGXMKIF_COMMAND		sCommandData;  
	PVRSRV_SGX_MISCINFO_INFO			*psSGXMiscInfoInt; 	
	PVRSRV_SGX_MISCINFO_FEATURES		*psSGXFeatures;		
	SGX_MISCINFO_STRUCT_SIZES			*psSGXStructSizes;	

	PPVRSRV_KERNEL_MEM_INFO	psMemInfo = psDevInfo->psKernelSGXMiscMemInfo;

	__DtTestPoint(__DtFunc_SGXGetMiscInfoUkernel, __DtStep_0)
	if (! psMemInfo->pvLinAddrKM)
	{
		__DtTestPoint(__DtFunc_SGXGetMiscInfoUkernel, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "SGXGetMiscInfoUkernel: Invalid address."));
		__DtTestPoint(__DtFunc_SGXGetMiscInfoUkernel, __DtStep_2)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}
	psSGXMiscInfoInt = psMemInfo->pvLinAddrKM;
	psSGXFeatures = &psSGXMiscInfoInt->sSGXFeatures;
	psSGXStructSizes = &psSGXMiscInfoInt->sSGXStructSizes;

	psSGXMiscInfoInt->ui32MiscInfoFlags &= ~PVRSRV_USSE_MISCINFO_READY;

	
	OSMemSet(psSGXFeatures, 0, sizeof(*psSGXFeatures));
	OSMemSet(psSGXStructSizes, 0, sizeof(*psSGXStructSizes));

	
	sCommandData.ui32Data[1] = psMemInfo->sDevVAddr.uiAddr; 

	PDUMPCOMMENT("Microkernel kick for SGXGetMiscInfo");
	eError = SGXScheduleCCBCommandKM(psDeviceNode,
									 SGXMKIF_CMD_GETMISCINFO,
									 &sCommandData,
									 KERNEL_ID,
									 0,
									 IMG_FALSE);

	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SGXGetMiscInfoUkernel, __DtStep_3)
		PVR_DPF((PVR_DBG_ERROR, "SGXGetMiscInfoUkernel: SGXScheduleCCBCommandKM failed."));
		__DtTestPoint(__DtFunc_SGXGetMiscInfoUkernel, __DtStep_4)
		return eError;
	}

	
#if !defined(NO_HARDWARE)
	{
		IMG_BOOL bExit;

		bExit = IMG_FALSE;
		LOOP_UNTIL_TIMEOUT(MAX_HW_TIME_US)
		{
			if ((psSGXMiscInfoInt->ui32MiscInfoFlags & PVRSRV_USSE_MISCINFO_READY) != 0)
			{
				__DtTestPoint(__DtFunc_SGXGetMiscInfoUkernel, __DtStep_5)
				bExit = IMG_TRUE;
				break;
			}
		} END_LOOP_UNTIL_TIMEOUT();

		
		if (!bExit)
		{
			__DtTestPoint(__DtFunc_SGXGetMiscInfoUkernel, __DtStep_6)
			PVR_DPF((PVR_DBG_ERROR, "SGXGetMiscInfoUkernel: Timeout occurred waiting for misc info."));
			__DtTestPoint(__DtFunc_SGXGetMiscInfoUkernel, __DtStep_7)
			return PVRSRV_ERROR_TIMEOUT;
		}
	}
#endif 

	return PVRSRV_OK;
}



IMG_EXPORT
PVRSRV_ERROR SGXGetMiscInfoKM(PVRSRV_SGXDEV_INFO	*psDevInfo,
							  SGX_MISC_INFO			*psMiscInfo,
 							  PVRSRV_DEVICE_NODE 	*psDeviceNode,
 							  IMG_HANDLE 			 hDevMemContext)
{
	PVRSRV_ERROR eError;
	PPVRSRV_KERNEL_MEM_INFO	psMemInfo = psDevInfo->psKernelSGXMiscMemInfo;
	IMG_UINT32	*pui32MiscInfoFlags = &((PVRSRV_SGX_MISCINFO_INFO*)(psMemInfo->pvLinAddrKM))->ui32MiscInfoFlags;

	
	__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_0)
	*pui32MiscInfoFlags = 0;

#if !defined(SUPPORT_SGX_EDM_MEMORY_DEBUG)
	PVR_UNREFERENCED_PARAMETER(hDevMemContext);
#endif

	switch(psMiscInfo->eRequest)
	{
#if defined(SGX_FEATURE_DATA_BREAKPOINTS)
		case SGX_MISC_INFO_REQUEST_SET_BREAKPOINT:
		{
			IMG_UINT32      ui32MaskDM;
			IMG_UINT32      ui32CtrlWEnable;
			IMG_UINT32      ui32CtrlREnable;
			IMG_UINT32      ui32CtrlTrapEnable;
			IMG_UINT32		ui32RegVal;
			IMG_UINT32		ui32StartRegVal;
			IMG_UINT32		ui32EndRegVal;
			SGXMKIF_COMMAND	sCommandData;

			
			__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_1)
			if(psMiscInfo->uData.sSGXBreakpointInfo.bBPEnable)
			{
				
				IMG_DEV_VIRTADDR sBPDevVAddr = psMiscInfo->uData.sSGXBreakpointInfo.sBPDevVAddr;
				IMG_DEV_VIRTADDR sBPDevVAddrEnd = psMiscInfo->uData.sSGXBreakpointInfo.sBPDevVAddrEnd;

				
				__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_2)
				ui32StartRegVal = sBPDevVAddr.uiAddr & EUR_CR_BREAKPOINT0_START_ADDRESS_MASK;
				ui32EndRegVal = sBPDevVAddrEnd.uiAddr & EUR_CR_BREAKPOINT0_END_ADDRESS_MASK;

				ui32MaskDM = psMiscInfo->uData.sSGXBreakpointInfo.ui32DataMasterMask;
				ui32CtrlWEnable = psMiscInfo->uData.sSGXBreakpointInfo.bWrite;
				ui32CtrlREnable = psMiscInfo->uData.sSGXBreakpointInfo.bRead;
				ui32CtrlTrapEnable = psMiscInfo->uData.sSGXBreakpointInfo.bTrapped;

				
				ui32RegVal = ((ui32MaskDM<<EUR_CR_BREAKPOINT0_MASK_DM_SHIFT) & EUR_CR_BREAKPOINT0_MASK_DM_MASK) |
							 ((ui32CtrlWEnable<<EUR_CR_BREAKPOINT0_CTRL_WENABLE_SHIFT) & EUR_CR_BREAKPOINT0_CTRL_WENABLE_MASK) |
							 ((ui32CtrlREnable<<EUR_CR_BREAKPOINT0_CTRL_RENABLE_SHIFT) & EUR_CR_BREAKPOINT0_CTRL_RENABLE_MASK) |
							 ((ui32CtrlTrapEnable<<EUR_CR_BREAKPOINT0_CTRL_TRAPENABLE_SHIFT) & EUR_CR_BREAKPOINT0_CTRL_TRAPENABLE_MASK);
			}
			else
			{
				
				__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_3)
				ui32RegVal = ui32StartRegVal = ui32EndRegVal = 0;
			}

			
			sCommandData.ui32Data[0] = psMiscInfo->uData.sSGXBreakpointInfo.ui32BPIndex;
			sCommandData.ui32Data[1] = ui32StartRegVal;
			sCommandData.ui32Data[2] = ui32EndRegVal;
			sCommandData.ui32Data[3] = ui32RegVal;

			
			psDevInfo->psSGXHostCtl->ui32BPSetClearSignal = 0;

			PDUMPCOMMENT("Microkernel kick for setting a data breakpoint");
			eError = SGXScheduleCCBCommandKM(psDeviceNode,
											 SGXMKIF_CMD_DATABREAKPOINT,
											 &sCommandData,
											 KERNEL_ID,
											 0,
											 IMG_FALSE);

			if (eError != PVRSRV_OK)
			{
				__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_4)
				PVR_DPF((PVR_DBG_ERROR, "SGXGetMiscInfoKM: SGXScheduleCCBCommandKM failed."));
				__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_5)
				return eError;
			}

#if defined(NO_HARDWARE)
			
			psDevInfo->psSGXHostCtl->ui32BPSetClearSignal = 0;
#else
			{
				IMG_BOOL bExit;

				bExit = IMG_FALSE;
				LOOP_UNTIL_TIMEOUT(MAX_HW_TIME_US)
				{
					if (psDevInfo->psSGXHostCtl->ui32BPSetClearSignal != 0)
					{
						__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_6)
						bExit = IMG_TRUE;
						
						psDevInfo->psSGXHostCtl->ui32BPSetClearSignal = 0;
						break;
					}
				} END_LOOP_UNTIL_TIMEOUT();

				
				if (!bExit)
				{
					__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_7)
					PVR_DPF((PVR_DBG_ERROR, "SGXGetMiscInfoKM: Timeout occurred waiting BP set/clear"));
					__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_8)
					return PVRSRV_ERROR_TIMEOUT;
				}
			}
#endif 

			return PVRSRV_OK;
		}

		case SGX_MISC_INFO_REQUEST_WAIT_FOR_BREAKPOINT:
		{
			
			
			__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_9)
			PDUMPCOMMENT("Wait for data breakpoint hit");

#if defined(NO_HARDWARE) && defined(PDUMP)
			{
				PDUMPREGPOL(SGX_PDUMPREG_NAME,
							EUR_CR_EVENT_STATUS2,
							EUR_CR_EVENT_STATUS2_DATA_BREAKPOINT_TRAPPED_MASK,
							EUR_CR_EVENT_STATUS2_DATA_BREAKPOINT_TRAPPED_MASK);

				PDUMPREG(SGX_PDUMPREG_NAME,
						 EUR_CR_EVENT_HOST_CLEAR2,
						 EUR_CR_EVENT_HOST_CLEAR2_DATA_BREAKPOINT_TRAPPED_MASK);

				PDUMPCOMMENT("Breakpoint detected.  Wait a bit to show that pipeline stops in simulation");
				PDUMPIDL(2000);

				PDUMPCOMMENT("Now we can resume");
				PDUMPREG(SGX_PDUMPREG_NAME, EUR_CR_BREAKPOINT_TRAP, EUR_CR_BREAKPOINT_TRAP_WRNOTIFY_MASK | EUR_CR_BREAKPOINT_TRAP_CONTINUE_MASK);
			}
#else
			{
				
			}
#endif 
			return PVRSRV_OK;
		}

		case SGX_MISC_INFO_REQUEST_POLL_BREAKPOINT:
		{
			




			

#if !defined(NO_HARDWARE)
			IMG_BOOL bTrappedBPMaster;
			IMG_BOOL abTrappedBPPerCore[SGX_FEATURE_MP_CORE_COUNT];
			IMG_UINT32 ui32CoreNum, ui32TrappedBPCoreNum;
			IMG_BOOL bTrappedBPAny;

			ui32TrappedBPCoreNum = 0;
			bTrappedBPMaster = !!(EUR_CR_MASTER_BREAKPOINT_TRAPPED_MASK & OSReadHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_MASTER_BREAKPOINT));
			bTrappedBPAny = bTrappedBPMaster;
			for (ui32CoreNum = 0; ui32CoreNum < SGX_FEATURE_MP_CORE_COUNT; ui32CoreNum++)
			{
				__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_10)
				abTrappedBPPerCore[ui32CoreNum] = !!(EUR_CR_BREAKPOINT_TRAPPED_MASK & OSReadHWReg(psDevInfo->pvRegsBaseKM, SGX_MP_CORE_SELECT(EUR_CR_BREAKPOINT, ui32CoreNum)));
				if (abTrappedBPPerCore[ui32CoreNum])
				{
					__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_11)
					bTrappedBPAny = IMG_TRUE;
					ui32TrappedBPCoreNum = ui32CoreNum;
				}
			}

			psMiscInfo->uData.sSGXBreakpointInfo.bTrappedBP = bTrappedBPAny;

			if (psMiscInfo->uData.sSGXBreakpointInfo.bTrappedBP)
			{
				IMG_UINT32 ui32Info0, ui32Info1;

				__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_12)
				ui32Info0 = OSReadHWReg(psDevInfo->pvRegsBaseKM, bTrappedBPMaster?EUR_CR_MASTER_BREAKPOINT_TRAP_INFO0:SGX_MP_CORE_SELECT(EUR_CR_BREAKPOINT_TRAP_INFO0, ui32TrappedBPCoreNum));
				ui32Info1 = OSReadHWReg(psDevInfo->pvRegsBaseKM, bTrappedBPMaster?EUR_CR_MASTER_BREAKPOINT_TRAP_INFO1:SGX_MP_CORE_SELECT(EUR_CR_BREAKPOINT_TRAP_INFO1, ui32TrappedBPCoreNum));

				psMiscInfo->uData.sSGXBreakpointInfo.ui32BPIndex = (ui32Info1 & EUR_CR_BREAKPOINT_TRAP_INFO1_NUMBER_MASK) >> EUR_CR_BREAKPOINT_TRAP_INFO1_NUMBER_SHIFT;
				psMiscInfo->uData.sSGXBreakpointInfo.sTrappedBPDevVAddr.uiAddr = ui32Info0 & EUR_CR_BREAKPOINT_TRAP_INFO0_ADDRESS_MASK;
				psMiscInfo->uData.sSGXBreakpointInfo.ui32TrappedBPBurstLength = (ui32Info1 & EUR_CR_BREAKPOINT_TRAP_INFO1_SIZE_MASK) >> EUR_CR_BREAKPOINT_TRAP_INFO1_SIZE_SHIFT;
				psMiscInfo->uData.sSGXBreakpointInfo.bTrappedBPRead = !!(ui32Info1 & EUR_CR_BREAKPOINT_TRAP_INFO1_RNW_MASK);
				psMiscInfo->uData.sSGXBreakpointInfo.ui32TrappedBPDataMaster = (ui32Info1 & EUR_CR_BREAKPOINT_TRAP_INFO1_DATA_MASTER_MASK) >> EUR_CR_BREAKPOINT_TRAP_INFO1_DATA_MASTER_SHIFT;
				psMiscInfo->uData.sSGXBreakpointInfo.ui32TrappedBPTag = (ui32Info1 & EUR_CR_BREAKPOINT_TRAP_INFO1_TAG_MASK) >> EUR_CR_BREAKPOINT_TRAP_INFO1_TAG_SHIFT;
				psMiscInfo->uData.sSGXBreakpointInfo.ui32CoreNum = bTrappedBPMaster?65535:ui32TrappedBPCoreNum;
			}
#endif 
			return PVRSRV_OK;
		}

		case SGX_MISC_INFO_REQUEST_RESUME_BREAKPOINT:
		{
			
			
			
#if !defined(NO_HARDWARE)
			IMG_UINT32 ui32CoreNum;
			IMG_BOOL bMaster;
			IMG_UINT32 ui32OldSeqNum, ui32NewSeqNum;

			ui32CoreNum = psMiscInfo->uData.sSGXBreakpointInfo.ui32CoreNum;
			bMaster = ui32CoreNum > SGX_FEATURE_MP_CORE_COUNT;
			if (bMaster)
			{
				
				
				__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_13)
				ui32OldSeqNum = 0x1c & OSReadHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_MASTER_BREAKPOINT);
				OSWriteHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_MASTER_BREAKPOINT_TRAP, EUR_CR_MASTER_BREAKPOINT_TRAP_WRNOTIFY_MASK | EUR_CR_MASTER_BREAKPOINT_TRAP_CONTINUE_MASK);
				do
				{
					__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_14)
					ui32NewSeqNum = 0x1c & OSReadHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_MASTER_BREAKPOINT);
				}
				while (ui32OldSeqNum == ui32NewSeqNum);
			}
			else
			{
				
				__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_15)
				ui32OldSeqNum = 0x1c & OSReadHWReg(psDevInfo->pvRegsBaseKM, SGX_MP_CORE_SELECT(EUR_CR_BREAKPOINT, ui32CoreNum));
				OSWriteHWReg(psDevInfo->pvRegsBaseKM, SGX_MP_CORE_SELECT(EUR_CR_BREAKPOINT_TRAP, ui32CoreNum), EUR_CR_BREAKPOINT_TRAP_WRNOTIFY_MASK | EUR_CR_BREAKPOINT_TRAP_CONTINUE_MASK);
				do
				{
					__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_16)
					ui32NewSeqNum = 0x1c & OSReadHWReg(psDevInfo->pvRegsBaseKM, SGX_MP_CORE_SELECT(EUR_CR_BREAKPOINT, ui32CoreNum));
				}
				while (ui32OldSeqNum == ui32NewSeqNum);
			}
#endif 
			return PVRSRV_OK;
		}
#endif 

		case SGX_MISC_INFO_REQUEST_CLOCKSPEED:
		{
			__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_17)
			psMiscInfo->uData.ui32SGXClockSpeed = psDevInfo->ui32CoreClockSpeed;
			__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_18)
			return PVRSRV_OK;
		}

		case SGX_MISC_INFO_REQUEST_ACTIVEPOWER:
		{
			__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_19)
			psMiscInfo->uData.sActivePower.ui32NumActivePowerEvents = psDevInfo->psSGXHostCtl->ui32NumActivePowerEvents;
			__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_20)
			return PVRSRV_OK;
		}

		case SGX_MISC_INFO_REQUEST_LOCKUPS:
		{
#if defined(SUPPORT_HW_RECOVERY)
			psMiscInfo->uData.sLockups.ui32uKernelDetectedLockups = psDevInfo->psSGXHostCtl->ui32uKernelDetectedLockups;
			psMiscInfo->uData.sLockups.ui32HostDetectedLockups = psDevInfo->psSGXHostCtl->ui32HostDetectedLockups;
#else
			psMiscInfo->uData.sLockups.ui32uKernelDetectedLockups = 0;
			psMiscInfo->uData.sLockups.ui32HostDetectedLockups = 0;
#endif
			return PVRSRV_OK;
		}

		case SGX_MISC_INFO_REQUEST_SPM:
		{
			
			__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_21)
			return PVRSRV_OK;
		}

		case SGX_MISC_INFO_REQUEST_SGXREV:
		{
			PVRSRV_SGX_MISCINFO_FEATURES		*psSGXFeatures;
			__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_22)
			eError = SGXGetMiscInfoUkernel(psDevInfo, psDeviceNode);
			if(eError != PVRSRV_OK)
			{
				__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_23)
				PVR_DPF((PVR_DBG_ERROR, "An error occurred in SGXGetMiscInfoUkernel: %d\n",
						eError));
				__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_24)
				return eError;
			}
			psSGXFeatures = &((PVRSRV_SGX_MISCINFO_INFO*)(psMemInfo->pvLinAddrKM))->sSGXFeatures;

			
			psMiscInfo->uData.sSGXFeatures = *psSGXFeatures;

			
			PVR_DPF((PVR_DBG_MESSAGE, "SGXGetMiscInfoKM: Core 0x%x, sw ID 0x%x, sw Rev 0x%x\n",
					psSGXFeatures->ui32CoreRev,
					psSGXFeatures->ui32CoreIdSW,
					psSGXFeatures->ui32CoreRevSW));
			PVR_DPF((PVR_DBG_MESSAGE, "SGXGetMiscInfoKM: DDK version 0x%x, DDK build 0x%x\n",
					psSGXFeatures->ui32DDKVersion,
					psSGXFeatures->ui32DDKBuild));

			
			__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_25)
			return PVRSRV_OK;
		}

		case SGX_MISC_INFO_REQUEST_DRIVER_SGXREV:
		{
			PVRSRV_SGX_MISCINFO_FEATURES		*psSGXFeatures;

			__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_26)
			psSGXFeatures = &((PVRSRV_SGX_MISCINFO_INFO*)(psMemInfo->pvLinAddrKM))->sSGXFeatures;

			
			OSMemSet(psMemInfo->pvLinAddrKM, 0,
					sizeof(PVRSRV_SGX_MISCINFO_INFO));

			psSGXFeatures->ui32DDKVersion =
				(PVRVERSION_MAJ << 16) |
				(PVRVERSION_MIN << 8) |
				PVRVERSION_BRANCH;
			psSGXFeatures->ui32DDKBuild = PVRVERSION_BUILD;

			
			psSGXFeatures->ui32BuildOptions = (SGX_BUILD_OPTIONS);

#if defined(PVRSRV_USSE_EDM_STATUS_DEBUG)
			
			psSGXFeatures->sDevVAEDMStatusBuffer = psDevInfo->psKernelEDMStatusBufferMemInfo->sDevVAddr;
			psSGXFeatures->pvEDMStatusBuffer = psDevInfo->psKernelEDMStatusBufferMemInfo->pvLinAddrKM;
#endif

			
			psMiscInfo->uData.sSGXFeatures = *psSGXFeatures;
			__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_27)
			return PVRSRV_OK;
		}

#if defined(SUPPORT_SGX_EDM_MEMORY_DEBUG)
		case SGX_MISC_INFO_REQUEST_MEMREAD:
		case SGX_MISC_INFO_REQUEST_MEMCOPY:
		{
			PVRSRV_ERROR eError;
			PVRSRV_SGX_MISCINFO_FEATURES		*psSGXFeatures;
			PVRSRV_SGX_MISCINFO_MEMACCESS		*psSGXMemSrc;	
			PVRSRV_SGX_MISCINFO_MEMACCESS		*psSGXMemDest;	

			__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_28)
			{				
				
				*pui32MiscInfoFlags |= PVRSRV_USSE_MISCINFO_MEMREAD;
				psSGXMemSrc = &((PVRSRV_SGX_MISCINFO_INFO*)(psMemInfo->pvLinAddrKM))->sSGXMemAccessSrc;

				if(psMiscInfo->sDevVAddrSrc.uiAddr != 0)
				{
					__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_29)
					psSGXMemSrc->sDevVAddr = psMiscInfo->sDevVAddrSrc; 
				}
				else
				{
					__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_30)
					return PVRSRV_ERROR_INVALID_PARAMS;
				}				
			}

			if( psMiscInfo->eRequest == SGX_MISC_INFO_REQUEST_MEMCOPY)
			{				
				
				__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_31)
				*pui32MiscInfoFlags |= PVRSRV_USSE_MISCINFO_MEMWRITE;
				psSGXMemDest = &((PVRSRV_SGX_MISCINFO_INFO*)(psMemInfo->pvLinAddrKM))->sSGXMemAccessDest;
				
				if(psMiscInfo->sDevVAddrDest.uiAddr != 0)
				{
					__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_32)
					psSGXMemDest->sDevVAddr = psMiscInfo->sDevVAddrDest; 
				}
				else
				{
					__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_33)
					return PVRSRV_ERROR_INVALID_PARAMS;
				}
			}

			
			if(psMiscInfo->hDevMemContext != IMG_NULL)
			{
				__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_34)
				SGXGetMMUPDAddrKM( (IMG_HANDLE)psDeviceNode, hDevMemContext, &psSGXMemSrc->sPDDevPAddr);
				
				
				psSGXMemDest->sPDDevPAddr = psSGXMemSrc->sPDDevPAddr;
			}
			else
			{
				__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_35)
				return PVRSRV_ERROR_INVALID_PARAMS;
			}

			
			eError = SGXGetMiscInfoUkernel(psDevInfo, psDeviceNode);
			if(eError != PVRSRV_OK)
			{
				__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_36)
				PVR_DPF((PVR_DBG_ERROR, "An error occurred in SGXGetMiscInfoUkernel: %d\n",
						eError));
				__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_37)
				return eError;
			}
			psSGXFeatures = &((PVRSRV_SGX_MISCINFO_INFO*)(psMemInfo->pvLinAddrKM))->sSGXFeatures;

#if !defined(SGX_FEATURE_MULTIPLE_MEM_CONTEXTS)
			if(*pui32MiscInfoFlags & PVRSRV_USSE_MISCINFO_MEMREAD_FAIL)
			{
				__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_38)
				return PVRSRV_ERROR_INVALID_MISCINFO;
			}
#endif
			
			psMiscInfo->uData.sSGXFeatures = *psSGXFeatures;
			__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_39)
			return PVRSRV_OK;
		}
#endif 

#if defined(SUPPORT_SGX_HWPERF)
		case SGX_MISC_INFO_REQUEST_SET_HWPERF_STATUS:
		{
			PVRSRV_SGX_MISCINFO_SET_HWPERF_STATUS	*psSetHWPerfStatus = &psMiscInfo->uData.sSetHWPerfStatus;
			const IMG_UINT32	ui32ValidFlags = PVRSRV_SGX_HWPERF_STATUS_RESET_COUNTERS |
												 PVRSRV_SGX_HWPERF_STATUS_GRAPHICS_ON |
												 PVRSRV_SGX_HWPERF_STATUS_PERIODIC_ON |
												 PVRSRV_SGX_HWPERF_STATUS_MK_EXECUTION_ON;
			SGXMKIF_COMMAND		sCommandData = {0};

			
			__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_40)
			if ((psSetHWPerfStatus->ui32NewHWPerfStatus & ~ui32ValidFlags) != 0)
			{
				__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_41)
				return PVRSRV_ERROR_INVALID_PARAMS;
			}

			#if defined(PDUMP)
			PDUMPCOMMENTWITHFLAGS(PDUMP_FLAGS_CONTINUOUS,
								  "SGX ukernel HWPerf status %u\n",
								  psSetHWPerfStatus->ui32NewHWPerfStatus);
			#endif 

			
			#if defined(SGX_FEATURE_EXTENDED_PERF_COUNTERS)
			OSMemCopy(&psDevInfo->psSGXHostCtl->aui32PerfGroup[0],
					  &psSetHWPerfStatus->aui32PerfGroup[0],
					  sizeof(psDevInfo->psSGXHostCtl->aui32PerfGroup));
			OSMemCopy(&psDevInfo->psSGXHostCtl->aui32PerfBit[0],
					  &psSetHWPerfStatus->aui32PerfBit[0],
					  sizeof(psDevInfo->psSGXHostCtl->aui32PerfBit));
			#if defined(PDUMP)
			PDUMPMEM(IMG_NULL, psDevInfo->psKernelSGXHostCtlMemInfo,
					 offsetof(SGXMKIF_HOST_CTL, aui32PerfGroup),
					 sizeof(psDevInfo->psSGXHostCtl->aui32PerfGroup),
					 PDUMP_FLAGS_CONTINUOUS,
					 MAKEUNIQUETAG(psDevInfo->psKernelSGXHostCtlMemInfo));
			PDUMPMEM(IMG_NULL, psDevInfo->psKernelSGXHostCtlMemInfo,
					 offsetof(SGXMKIF_HOST_CTL, aui32PerfBit),
					 sizeof(psDevInfo->psSGXHostCtl->aui32PerfBit),
					 PDUMP_FLAGS_CONTINUOUS,
					 MAKEUNIQUETAG(psDevInfo->psKernelSGXHostCtlMemInfo));
			#endif 
			#else
			psDevInfo->psSGXHostCtl->ui32PerfGroup = psSetHWPerfStatus->ui32PerfGroup;
			#if defined(PDUMP)
			PDUMPMEM(IMG_NULL, psDevInfo->psKernelSGXHostCtlMemInfo,
					 offsetof(SGXMKIF_HOST_CTL, ui32PerfGroup),
					 sizeof(psDevInfo->psSGXHostCtl->ui32PerfGroup),
					 PDUMP_FLAGS_CONTINUOUS,
					 MAKEUNIQUETAG(psDevInfo->psKernelSGXHostCtlMemInfo));
			#endif 
			#endif 

			
			sCommandData.ui32Data[0] = psSetHWPerfStatus->ui32NewHWPerfStatus;
			eError = SGXScheduleCCBCommandKM(psDeviceNode,
											 SGXMKIF_CMD_SETHWPERFSTATUS,
											 &sCommandData,
											 KERNEL_ID,
											 0,
											 IMG_FALSE);
			__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_42)
			return eError;
		}
#endif 

		case SGX_MISC_INFO_DUMP_DEBUG_INFO:
		{
			__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_43)
			PVR_LOG(("User requested SGX debug info"));

			
			SGXDumpDebugInfo(psDeviceNode->pvDevice, IMG_FALSE);

			__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_44)
			return PVRSRV_OK;
		}

		case SGX_MISC_INFO_PANIC:
		{
			__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_45)
			PVR_LOG(("User requested SGX panic"));

			SGXPanic(psDeviceNode->pvDevice);

			__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_46)
			return PVRSRV_OK;
		}

		default:
		{
			
			__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_47)
			return PVRSRV_ERROR_INVALID_PARAMS;
		}
	}
	__DtTestPoint(__DtFunc_SGXGetMiscInfoKM, __DtStep_48)
}


IMG_EXPORT
PVRSRV_ERROR SGXReadHWPerfCBKM(IMG_HANDLE					hDevHandle,
							   IMG_UINT32					ui32ArraySize,
							   PVRSRV_SGX_HWPERF_CB_ENTRY	*psClientHWPerfEntry,
							   IMG_UINT32					*pui32DataCount,
							   IMG_UINT32					*pui32ClockSpeed,
							   IMG_UINT32					*pui32HostTimeStamp)
{
	PVRSRV_ERROR    	eError = PVRSRV_OK;
	PVRSRV_DEVICE_NODE	*psDeviceNode = hDevHandle;
	PVRSRV_SGXDEV_INFO	*psDevInfo = psDeviceNode->pvDevice;
	SGXMKIF_HWPERF_CB	*psHWPerfCB = psDevInfo->psKernelHWPerfCBMemInfo->pvLinAddrKM;
	IMG_UINT			i;

	__DtTestPoint(__DtFunc_SGXReadHWPerfCBKM, __DtStep_0)
	for (i = 0;
		 psHWPerfCB->ui32Woff != psHWPerfCB->ui32Roff && i < ui32ArraySize;
		 i++)
	{
		SGXMKIF_HWPERF_CB_ENTRY *psMKPerfEntry = &psHWPerfCB->psHWPerfCBData[psHWPerfCB->ui32Roff];

		__DtTestPoint(__DtFunc_SGXReadHWPerfCBKM, __DtStep_1)
		psClientHWPerfEntry[i].ui32FrameNo = psMKPerfEntry->ui32FrameNo;
		psClientHWPerfEntry[i].ui32Type = psMKPerfEntry->ui32Type;
		psClientHWPerfEntry[i].ui32Ordinal	= psMKPerfEntry->ui32Ordinal;
		psClientHWPerfEntry[i].ui32Info	= psMKPerfEntry->ui32Info;
		psClientHWPerfEntry[i].ui32Clocksx16 = SGXConvertTimeStamp(psDevInfo,
													psMKPerfEntry->ui32TimeWraps,
													psMKPerfEntry->ui32Time);
		OSMemCopy(&psClientHWPerfEntry[i].ui32Counters[0][0],
				  &psMKPerfEntry->ui32Counters[0][0],
				  sizeof(psMKPerfEntry->ui32Counters));

		psHWPerfCB->ui32Roff = (psHWPerfCB->ui32Roff + 1) & (SGXMKIF_HWPERF_CB_SIZE - 1);
	}

	*pui32DataCount = i;
	*pui32ClockSpeed = psDevInfo->ui32CoreClockSpeed;
	*pui32HostTimeStamp = OSClockus();

	__DtTestPoint(__DtFunc_SGXReadHWPerfCBKM, __DtStep_2)
	return eError;
}


