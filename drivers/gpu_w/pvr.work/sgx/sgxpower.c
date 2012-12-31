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
#include "services_headers.h"
#include "sgxapi_km.h"
#include "sgx_mkif_km.h"
#include "sgxutils.h"
#include "pdump_km.h"

#include "../falinux_debug.h"

#ifdef CONFIG_FALINUX_ZEROBOOT
extern int zeroboot_pvr_init_flag;
#endif

#if defined(SUPPORT_HW_RECOVERY)
static PVRSRV_ERROR SGXAddTimer(PVRSRV_DEVICE_NODE		*psDeviceNode,
								SGX_TIMING_INFORMATION	*psSGXTimingInfo,
								IMG_HANDLE				*phTimer)
{
	*phTimer = OSAddTimer(SGXOSTimer, psDeviceNode,
						  1000 * 50 / psSGXTimingInfo->ui32uKernelFreq);
	FA_ENTER();
	if(*phTimer == IMG_NULL)
	{
		PVR_DPF((PVR_DBG_ERROR,"SGXAddTimer : Failed to register timer callback function"));
		FA_EXIT();
		return PVRSRV_ERROR_OUT_OF_MEMORY;
	}
	FA_EXIT();
	return PVRSRV_OK;
}
#endif 

static PVRSRV_ERROR SGXUpdateTimingInfo(PVRSRV_DEVICE_NODE	*psDeviceNode)
{
	PVRSRV_SGXDEV_INFO	*psDevInfo = psDeviceNode->pvDevice;
#if defined(SGX_DYNAMIC_TIMING_INFO)
	SGX_TIMING_INFORMATION	sSGXTimingInfo = {0};
#else
	SGX_DEVICE_MAP		*psSGXDeviceMap;
#endif
	IMG_UINT32		ui32ActivePowManSampleRate;
	SGX_TIMING_INFORMATION	*psSGXTimingInfo;
	FA_ENTER();
	
#if defined(SGX_DYNAMIC_TIMING_INFO)		// 정의되지 않음
	psSGXTimingInfo = &sSGXTimingInfo;
	SysGetSGXTimingInformation(psSGXTimingInfo);
#else
printk( "-----%s(%d)\n", __func__,__LINE__ );
	SysGetDeviceMemoryMap(PVRSRV_DEVICE_TYPE_SGX,
						  (IMG_VOID**)&psSGXDeviceMap);
	psSGXTimingInfo = &psSGXDeviceMap->sTimingInfo;
#endif

#if defined(SUPPORT_HW_RECOVERY)
	{
		PVRSRV_ERROR			eError;
		IMG_UINT32	ui32OlduKernelFreq;


#ifdef CONFIG_FALINUX_ZEROBOOT
		if (psDevInfo->hTimer != IMG_NULL)
		{
			if ( 0 == strcmp( current->comm, "pvrsrvinit" ) )
			{
				printk( " ..<fake>	psDevInfo->hTimer=NULL\n" );
				psDevInfo->hTimer = NULL;
			}
		}
#endif			

		if (psDevInfo->hTimer != IMG_NULL)
		{
printk( "-----%s(%d)\n", __func__,__LINE__ );
			ui32OlduKernelFreq = psDevInfo->ui32CoreClockSpeed / psDevInfo->ui32uKernelTimerClock;
			if (ui32OlduKernelFreq != psSGXTimingInfo->ui32uKernelFreq)
			{
				IMG_HANDLE hNewTimer;
				
				eError = SGXAddTimer(psDeviceNode, psSGXTimingInfo, &hNewTimer);
				if (eError == PVRSRV_OK)
				{
					eError = OSRemoveTimer(psDevInfo->hTimer);
					if (eError != PVRSRV_OK)
					{
						PVR_DPF((PVR_DBG_ERROR,"SGXUpdateTimingInfo: Failed to remove timer"));
					}
					psDevInfo->hTimer = hNewTimer;
				}
				else
				{
					
				}
			}
		}
		else
		{
printk( "-----%s(%d)\n", __func__,__LINE__ );
			eError = SGXAddTimer(psDeviceNode, psSGXTimingInfo, &psDevInfo->hTimer);
			if (eError != PVRSRV_OK)
			{
				FA_EXIT();
				return eError;
			}
		}

		psDevInfo->psSGXHostCtl->ui32HWRecoverySampleRate =
			psSGXTimingInfo->ui32uKernelFreq / psSGXTimingInfo->ui32HWRecoveryFreq;
	}
#endif 
	
	psDevInfo->ui32CoreClockSpeed = psSGXTimingInfo->ui32CoreClockSpeed;
	psDevInfo->ui32uKernelTimerClock = psSGXTimingInfo->ui32CoreClockSpeed / psSGXTimingInfo->ui32uKernelFreq;

	
	psDevInfo->psSGXHostCtl->ui32uKernelTimerClock = psDevInfo->ui32uKernelTimerClock;
#if defined(PDUMP)
	PDUMPCOMMENT("Host Control - Microkernel clock");
	PDUMPMEM(IMG_NULL, psDevInfo->psKernelSGXHostCtlMemInfo,
			 offsetof(SGXMKIF_HOST_CTL, ui32uKernelTimerClock),
			 sizeof(IMG_UINT32), PDUMP_FLAGS_CONTINUOUS,
			 MAKEUNIQUETAG(psDevInfo->psKernelSGXHostCtlMemInfo));
#endif 

	if (psSGXTimingInfo->bEnableActivePM)
	{
		ui32ActivePowManSampleRate =
			psSGXTimingInfo->ui32uKernelFreq * psSGXTimingInfo->ui32ActivePowManLatencyms / 1000;

		ui32ActivePowManSampleRate += 1;
printk( "-----%s(%d)\n", __func__,__LINE__ );		
	}
	else
	{
printk( "-----%s(%d)\n", __func__,__LINE__ );
		ui32ActivePowManSampleRate = 0;
	}

	psDevInfo->psSGXHostCtl->ui32ActivePowManSampleRate = ui32ActivePowManSampleRate;
#if defined(PDUMP)
	PDUMPMEM(IMG_NULL, psDevInfo->psKernelSGXHostCtlMemInfo,
			 offsetof(SGXMKIF_HOST_CTL, ui32ActivePowManSampleRate),
			 sizeof(IMG_UINT32), PDUMP_FLAGS_CONTINUOUS,
			 MAKEUNIQUETAG(psDevInfo->psKernelSGXHostCtlMemInfo));
#endif 
	FA_EXIT();
	return PVRSRV_OK;
}


static IMG_VOID SGXStartTimer(PVRSRV_SGXDEV_INFO	*psDevInfo)
{
	#if defined(SUPPORT_HW_RECOVERY)
	PVRSRV_ERROR	eError;
	FA_ENTER();
printk( "-----%s(%d)\n", __func__,__LINE__ );
	eError = OSEnableTimer(psDevInfo->hTimer);
	if (eError != PVRSRV_OK)
	{
		PVR_DPF((PVR_DBG_ERROR,"SGXStartTimer : Failed to enable host timer"));
	}
	#else
	PVR_UNREFERENCED_PARAMETER(psDevInfo);
	#endif 
	FA_EXIT();
}


static IMG_VOID SGXPollForClockGating (PVRSRV_SGXDEV_INFO	*psDevInfo,
									   IMG_UINT32			ui32Register,
									   IMG_UINT32			ui32RegisterValue,
									   IMG_CHAR				*pszComment)
{
	FA_ENTER();
	
	PVR_UNREFERENCED_PARAMETER(psDevInfo);
	PVR_UNREFERENCED_PARAMETER(ui32Register);
	PVR_UNREFERENCED_PARAMETER(ui32RegisterValue);
	PVR_UNREFERENCED_PARAMETER(pszComment);

	#if !defined(NO_HARDWARE)
	PVR_ASSERT(psDevInfo != IMG_NULL);

	 
	if (PollForValueKM((IMG_UINT32 *)psDevInfo->pvRegsBaseKM + (ui32Register >> 2),
						0,
						ui32RegisterValue,
						MAX_HW_TIME_US,
						MAX_HW_TIME_US/WAIT_TRY_COUNT,
						IMG_FALSE) != PVRSRV_OK)
	{
		PVR_DPF((PVR_DBG_ERROR,"SGXPollForClockGating: %s failed.", pszComment));
		PVR_DBG_BREAK;
	}
	#endif 

	PDUMPCOMMENT("%s", pszComment);
	PDUMPREGPOL(SGX_PDUMPREG_NAME, ui32Register, 0, ui32RegisterValue);
	FA_EXIT();
}


PVRSRV_ERROR SGXPrePowerState (IMG_HANDLE				hDevHandle,
							   PVRSRV_DEV_POWER_STATE	eNewPowerState,
							   PVRSRV_DEV_POWER_STATE	eCurrentPowerState)
{
	FA_ENTER();
	if ((eNewPowerState != eCurrentPowerState) &&
		(eNewPowerState != PVRSRV_DEV_POWER_STATE_ON))
	{
		PVRSRV_ERROR		eError;
		PVRSRV_DEVICE_NODE	*psDeviceNode = hDevHandle;
		PVRSRV_SGXDEV_INFO	*psDevInfo = psDeviceNode->pvDevice;
		IMG_UINT32			ui32PowerCmd, ui32CompleteStatus;
		SGXMKIF_COMMAND		sCommand = {0};
		IMG_UINT32			ui32Core;

		#if defined(SUPPORT_HW_RECOVERY)
		
		eError = OSDisableTimer(psDevInfo->hTimer);
		if (eError != PVRSRV_OK)
		{
			PVR_DPF((PVR_DBG_ERROR,"SGXPrePowerState: Failed to disable timer"));
			FA_EXIT();
			return eError;
		}
		#endif 

		if (eNewPowerState == PVRSRV_DEV_POWER_STATE_OFF)
		{
			
			ui32PowerCmd = PVRSRV_POWERCMD_POWEROFF;
			ui32CompleteStatus = PVRSRV_USSE_EDM_POWMAN_POWEROFF_COMPLETE;
			PDUMPCOMMENT("SGX power off request");
		}
		else
		{
			
			ui32PowerCmd = PVRSRV_POWERCMD_IDLE;
			ui32CompleteStatus = PVRSRV_USSE_EDM_POWMAN_IDLE_COMPLETE;
			PDUMPCOMMENT("SGX idle request");
		}

		sCommand.ui32Data[1] = ui32PowerCmd;

		eError = SGXScheduleCCBCommand(psDevInfo, SGXMKIF_CMD_POWER, &sCommand, KERNEL_ID, 0, IMG_FALSE);
		if (eError != PVRSRV_OK)
		{
			PVR_DPF((PVR_DBG_ERROR,"SGXPrePowerState: Failed to submit power down command"));
			FA_EXIT();
			return eError;
		}

		
		#if !defined(NO_HARDWARE)
		if (PollForValueKM(&psDevInfo->psSGXHostCtl->ui32PowerStatus,
							ui32CompleteStatus,
							ui32CompleteStatus,
							MAX_HW_TIME_US,
							MAX_HW_TIME_US/WAIT_TRY_COUNT,
							IMG_FALSE) != PVRSRV_OK)
		{
			PVR_DPF((PVR_DBG_ERROR,"SGXPrePowerState: Wait for SGX ukernel power transition failed."));
			PVR_DBG_BREAK;
		}
		#endif 

		#if defined(PDUMP)
		PDUMPCOMMENT("TA/3D CCB Control - Wait for power event on uKernel.");
		PDUMPMEMPOL(psDevInfo->psKernelSGXHostCtlMemInfo,
					offsetof(SGXMKIF_HOST_CTL, ui32PowerStatus),
					ui32CompleteStatus,
					ui32CompleteStatus,
					PDUMP_POLL_OPERATOR_EQUAL,
					0,
					MAKEUNIQUETAG(psDevInfo->psKernelSGXHostCtlMemInfo));
		#endif 

		for (ui32Core = 0; ui32Core < SGX_FEATURE_MP_CORE_COUNT; ui32Core++)
		{
			
			SGXPollForClockGating(psDevInfo,
								  SGX_MP_CORE_SELECT(psDevInfo->ui32ClkGateStatusReg, ui32Core),
								  psDevInfo->ui32ClkGateStatusMask,
								  "Wait for SGX clock gating");
		}

		#if defined(SGX_FEATURE_MP)
		
		SGXPollForClockGating(psDevInfo,
							  psDevInfo->ui32MasterClkGateStatusReg,
							  psDevInfo->ui32MasterClkGateStatusMask,
							  "Wait for SGX master clock gating");

		SGXPollForClockGating(psDevInfo,
							  psDevInfo->ui32MasterClkGateStatus2Reg,
							  psDevInfo->ui32MasterClkGateStatus2Mask,
							  "Wait for SGX master clock gating (2)");
		#endif 

		if (eNewPowerState == PVRSRV_DEV_POWER_STATE_OFF)
		{
			
			eError = SGXDeinitialise(psDevInfo);
			if (eError != PVRSRV_OK)
			{
				PVR_DPF((PVR_DBG_ERROR,"SGXPrePowerState: SGXDeinitialise failed: %u", eError));
				FA_EXIT();
				return eError;
			}
		}
	}
	FA_EXIT();
	return PVRSRV_OK;
}

#ifdef CONFIG_FALINUX_ZEROBOOT
//extern PVRSRV_ERROR PVRSRVFinaliseSystem_SetPowerState_AnyCb(PVRSRV_DEVICE_NODE *psDeviceNode);
//extern PVRSRV_ERROR SGXPostPowerState (IMG_HANDLE hDevHandle, PVRSRV_DEV_POWER_STATE eNewPowerState, PVRSRV_DEV_POWER_STATE eCurrentPowerState);
extern PVRSRV_ERROR EnableSGXClocks(void);	// driver/gpu/pvr/s5pc110/sysconfig.c
extern PVRSRV_ERROR DisableSGXClocks(void);	// driver/gpu/pvr/s5pc110/sysconfig.c
void zb_raw_sgx_init( void );
static PVRSRV_DEVICE_NODE *zb_psDeviceNode = NULL;
void zb_raw_sgx_init( void )
{
	FA_ENTER();
	EnableSGXClocks();

	{
		SGX_TIMING_INFORMATION	*psSGXTimingInfo;  
		SGX_DEVICE_MAP			*psSGXDeviceMap;

		SysGetDeviceMemoryMap(PVRSRV_DEVICE_TYPE_SGX, (IMG_VOID**)&psSGXDeviceMap);		
		psSGXTimingInfo = &psSGXDeviceMap->sTimingInfo;   
                                                 
		// sysconfig.SysInitialise() 함수    
		psSGXTimingInfo = &psSGXDeviceMap->sTimingInfo;		
		psSGXTimingInfo->ui32CoreClockSpeed = 200000000; 	// SYS_SGX_CLOCK_SPEED;
		psSGXTimingInfo->ui32HWRecoveryFreq = 1000;			// SYS_SGX_HWRECOVERY_TIMEOUT_FREQ; 
		psSGXTimingInfo->ui32ActivePowManLatencyms = 500; 	// SYS_SGX_ACTIVE_POWER_LATENCY_MS; 
		psSGXTimingInfo->ui32uKernelFreq = 1000; 			// SYS_SGX_PDS_TIMER_FREQ; 
		psSGXTimingInfo->bEnableActivePM = IMG_TRUE;	
	}	

	{
		SYS_DATA			*psSysData;
		PVRSRV_POWER_DEV	*psPowerDevice;
		PVRSRV_DEVICE_NODE  *psDeviceNode;

		SysAcquireData(&psSysData);

		psPowerDevice = (PVRSRV_POWER_DEV  *)psSysData->psPowerDeviceList;
		psDeviceNode  = (PVRSRV_DEVICE_NODE*)psSysData->psDeviceNodeList->psNext;

		printk( "      psPowerDevice=%p\n", psPowerDevice );
		printk( "      psDeviceNode=%p \n", psDeviceNode  );

		//psPowerDevice->eCurrentPowerState = PVRSRV_DEV_POWER_STATE_ON;
	}


	printk( "      zb_psDeviceNode=%p \n", zb_psDeviceNode  );


	//SGXPrePowerState (zb_psDeviceNode, PVRSRV_DEV_POWER_STATE_IDLE,  PVRSRV_DEV_POWER_STATE_ON);

	SGXPostPowerState(zb_psDeviceNode, PVRSRV_DEV_POWER_STATE_ON  , PVRSRV_DEV_POWER_STATE_OFF );	// 0, 2

	//PVRSRVFinaliseSystem_SetPowerState_AnyCb( zb_psDeviceNode );

	//SGXUpdateTimingInfo(zb_psDeviceNode);
	//SGXInitialise(zb_psDeviceNode->pvDevice, IMG_FALSE );
	FA_EXIT();
}
#endif

PVRSRV_ERROR SGXPostPowerState (IMG_HANDLE				hDevHandle,
								PVRSRV_DEV_POWER_STATE	eNewPowerState,
								PVRSRV_DEV_POWER_STATE	eCurrentPowerState)
{
	FA_ENTER();
	if ((eNewPowerState != eCurrentPowerState) &&
		(eCurrentPowerState != PVRSRV_DEV_POWER_STATE_ON))
	{
		PVRSRV_ERROR		eError;
		PVRSRV_DEVICE_NODE	*psDeviceNode = hDevHandle;
		PVRSRV_SGXDEV_INFO	*psDevInfo = psDeviceNode->pvDevice;
		SGXMKIF_HOST_CTL *psSGXHostCtl = psDevInfo->psSGXHostCtl;

printk( "__@@_____%s(%d) pname=%s\n", __func__,__LINE__, current->comm );
		psSGXHostCtl->ui32PowerStatus = 0;
		#if defined(PDUMP)
		PDUMPCOMMENT("Host Control - Reset power status");
		PDUMPMEM(IMG_NULL, psDevInfo->psKernelSGXHostCtlMemInfo,
				 offsetof(SGXMKIF_HOST_CTL, ui32PowerStatus),
				 sizeof(IMG_UINT32), PDUMP_FLAGS_CONTINUOUS,
				 MAKEUNIQUETAG(psDevInfo->psKernelSGXHostCtlMemInfo));
		#endif 

		if (eCurrentPowerState == PVRSRV_DEV_POWER_STATE_OFF)
		{
printk( "-----%s(%d)  PVRSRV_DEV_POWER_STATE_OFF\n", __func__,__LINE__ );	
#ifdef CONFIG_FALINUX_ZEROBOOT
			zb_psDeviceNode = psDeviceNode;
#endif		
			eError = SGXUpdateTimingInfo(psDeviceNode);
			if (eError != PVRSRV_OK)
			{
				PVR_DPF((PVR_DBG_ERROR,"SGXPostPowerState: SGXUpdateTimingInfo failed"));
				FA_EXIT();
				return eError;
			}

printk( "-----%s(%d)\n", __func__,__LINE__ );	
			eError = SGXInitialise(psDevInfo, IMG_FALSE);
			if (eError != PVRSRV_OK)
			{
				PVR_DPF((PVR_DBG_ERROR,"SGXPostPowerState: SGXInitialise failed"));
				FA_EXIT();
				return eError;
			}
		}
		else
		{
			SGXMKIF_COMMAND		sCommand = {0};

printk( "-----%s(%d)  PVRSRV_DEV_POWER_STATE_ON\n", __func__,__LINE__ );	
			sCommand.ui32Data[1] = PVRSRV_POWERCMD_RESUME;
			eError = SGXScheduleCCBCommand(psDevInfo, SGXMKIF_CMD_POWER, &sCommand, ISR_ID, 0, IMG_FALSE);
			if (eError != PVRSRV_OK)
			{
				PVR_DPF((PVR_DBG_ERROR,"SGXPostPowerState failed to schedule CCB command: %u", eError));
				FA_EXIT();
				return eError;
			}
		}

printk( "-----%s(%d)\n", __func__,__LINE__ );	
		SGXStartTimer(psDevInfo);
	}

printk( "-----%s(%d)\n", __func__,__LINE__ );	
	FA_EXIT();
	return PVRSRV_OK;
}


PVRSRV_ERROR SGXPreClockSpeedChange (IMG_HANDLE				hDevHandle,
									 IMG_BOOL				bIdleDevice,
									 PVRSRV_DEV_POWER_STATE	eCurrentPowerState)
{
	PVRSRV_ERROR		eError;
	PVRSRV_DEVICE_NODE	*psDeviceNode = hDevHandle;
	PVRSRV_SGXDEV_INFO	*psDevInfo = psDeviceNode->pvDevice;
	FA_ENTER();
	PVR_UNREFERENCED_PARAMETER(psDevInfo);

	if (eCurrentPowerState == PVRSRV_DEV_POWER_STATE_ON)
	{
		if (bIdleDevice)
		{
			
			PDUMPSUSPEND();

			eError = SGXPrePowerState(hDevHandle, PVRSRV_DEV_POWER_STATE_IDLE,
									  PVRSRV_DEV_POWER_STATE_ON);

			if (eError != PVRSRV_OK)
			{
				PDUMPRESUME();
				FA_EXIT();
				return eError;
			}
		}
	}

	PVR_DPF((PVR_DBG_MESSAGE,"SGXPreClockSpeedChange: SGX clock speed was %uHz",
			psDevInfo->ui32CoreClockSpeed));
	FA_EXIT();
	return PVRSRV_OK;
}


PVRSRV_ERROR SGXPostClockSpeedChange (IMG_HANDLE				hDevHandle,
									  IMG_BOOL					bIdleDevice,
									  PVRSRV_DEV_POWER_STATE	eCurrentPowerState)
{
	PVRSRV_DEVICE_NODE	*psDeviceNode = hDevHandle;
	PVRSRV_SGXDEV_INFO	*psDevInfo = psDeviceNode->pvDevice;
	IMG_UINT32			ui32OldClockSpeed = psDevInfo->ui32CoreClockSpeed;
	FA_ENTER();
	PVR_UNREFERENCED_PARAMETER(ui32OldClockSpeed);

	if (eCurrentPowerState == PVRSRV_DEV_POWER_STATE_ON)
	{
		PVRSRV_ERROR eError;

printk( "-----%s(%d)\n", __func__,__LINE__ );		

		eError = SGXUpdateTimingInfo(psDeviceNode);
		if (eError != PVRSRV_OK)
		{
			PVR_DPF((PVR_DBG_ERROR,"SGXPostPowerState: SGXUpdateTimingInfo failed"));
			FA_EXIT();
			return eError;
		}

		if (bIdleDevice)
		{
printk( "-----%s(%d)\n", __func__,__LINE__ );			
			eError = SGXPostPowerState(hDevHandle, PVRSRV_DEV_POWER_STATE_ON,
									   PVRSRV_DEV_POWER_STATE_IDLE);

			PDUMPRESUME();

			if (eError != PVRSRV_OK)
			{
				FA_EXIT();
				return eError;
			}
		}
		else
		{
printk( "-----%s(%d)\n", __func__,__LINE__ );			
			SGXStartTimer(psDevInfo);
		}
	}

	PVR_DPF((PVR_DBG_MESSAGE,"SGXPostClockSpeedChange: SGX clock speed changed from %uHz to %uHz",
			ui32OldClockSpeed, psDevInfo->ui32CoreClockSpeed));
	FA_EXIT();
	return PVRSRV_OK;
}


