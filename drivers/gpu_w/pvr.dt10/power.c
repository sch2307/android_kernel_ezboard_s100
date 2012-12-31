#include	"DT_power.h"	/* For DynamicTracer-TestPoint */
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
#include "pdump_km.h"

#include "lists.h"

#ifdef __linux__
#include <linux/delay.h>
#endif

static IMG_BOOL gbInitServerRunning = IMG_FALSE;
static IMG_BOOL gbInitServerRan = IMG_FALSE;
static IMG_BOOL gbInitSuccessful = IMG_FALSE;

IMG_EXPORT
PVRSRV_ERROR PVRSRVSetInitServerState(PVRSRV_INIT_SERVER_STATE eInitServerState, IMG_BOOL bState)
{

	__DtTestPoint(__DtFunc_PVRSRVSetInitServerState, __DtStep_0)
	switch(eInitServerState)
	{
		case PVRSRV_INIT_SERVER_RUNNING:
			__DtTestPoint(__DtFunc_PVRSRVSetInitServerState, __DtStep_1)
			gbInitServerRunning	= bState;
			break;
		case PVRSRV_INIT_SERVER_RAN:
			__DtTestPoint(__DtFunc_PVRSRVSetInitServerState, __DtStep_2)
			gbInitServerRan	= bState;
			break;
		case PVRSRV_INIT_SERVER_SUCCESSFUL:
			__DtTestPoint(__DtFunc_PVRSRVSetInitServerState, __DtStep_3)
			gbInitSuccessful = bState;
			break;
		default:
			__DtTestPoint(__DtFunc_PVRSRVSetInitServerState, __DtStep_4)
			PVR_DPF((PVR_DBG_ERROR,
				"PVRSRVSetInitServerState : Unknown state %x", eInitServerState));
			__DtTestPoint(__DtFunc_PVRSRVSetInitServerState, __DtStep_5)
			return PVRSRV_ERROR_UNKNOWN_INIT_SERVER_STATE;
	}

	__DtTestPoint(__DtFunc_PVRSRVSetInitServerState, __DtStep_6)
	return PVRSRV_OK;
}

IMG_EXPORT
IMG_BOOL PVRSRVGetInitServerState(PVRSRV_INIT_SERVER_STATE eInitServerState)
{
	IMG_BOOL	bReturnVal;

	__DtTestPoint(__DtFunc_PVRSRVGetInitServerState, __DtStep_0)
	switch(eInitServerState)
	{
		case PVRSRV_INIT_SERVER_RUNNING:
			__DtTestPoint(__DtFunc_PVRSRVGetInitServerState, __DtStep_1)
			bReturnVal = gbInitServerRunning;
			break;
		case PVRSRV_INIT_SERVER_RAN:
			__DtTestPoint(__DtFunc_PVRSRVGetInitServerState, __DtStep_2)
			bReturnVal = gbInitServerRan;
			break;
		case PVRSRV_INIT_SERVER_SUCCESSFUL:
			__DtTestPoint(__DtFunc_PVRSRVGetInitServerState, __DtStep_3)
			bReturnVal = gbInitSuccessful;
			break;
		default:
			__DtTestPoint(__DtFunc_PVRSRVGetInitServerState, __DtStep_4)
			PVR_DPF((PVR_DBG_ERROR,
				"PVRSRVGetInitServerState : Unknown state %x", eInitServerState));
			bReturnVal = IMG_FALSE;
	}

	__DtTestPoint(__DtFunc_PVRSRVGetInitServerState, __DtStep_5)
	return bReturnVal;
}

static IMG_BOOL _IsSystemStatePowered(PVRSRV_SYS_POWER_STATE eSystemPowerState)
{
	__DtTestPoint(__DtFunc__IsSystemStatePowered, __DtStep_0)
	return (IMG_BOOL)(eSystemPowerState < PVRSRV_SYS_POWER_STATE_D2);
}


IMG_EXPORT
PVRSRV_ERROR PVRSRVPowerLock(IMG_UINT32	ui32CallerID,
							 IMG_BOOL	bSystemPowerEvent)
{
	PVRSRV_ERROR	eError;
	SYS_DATA		*psSysData;

#ifdef __linux__	
	IMG_UINT32		ui32Timeout = 1000;
#else
	IMG_UINT32		ui32Timeout = 1000000;
#endif

#if defined(SUPPORT_LMA)
	
	ui32Timeout *= 60;
#endif 

	SysAcquireData(&psSysData);

#if defined(SYS_CUSTOM_POWERLOCK_WRAP)
	eError = SysPowerLockWrap(psSysData);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVPowerLock, __DtStep_0)
		return eError;
	}
#endif
	do
	{
		__DtTestPoint(__DtFunc_PVRSRVPowerLock, __DtStep_1)
		eError = OSLockResource(&psSysData->sPowerStateChangeResource,
								ui32CallerID);
		if (eError == PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PVRSRVPowerLock, __DtStep_2)
			break;
		}
		else if (ui32CallerID == ISR_ID)
		{
			

			__DtTestPoint(__DtFunc_PVRSRVPowerLock, __DtStep_3)
			eError = PVRSRV_ERROR_RETRY;
			break;
		}

#ifdef __linux__
		msleep(1);
#else
		OSWaitus(1);
#endif
		ui32Timeout--;
	} while (ui32Timeout > 0);

#if defined(SYS_CUSTOM_POWERLOCK_WRAP)
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVPowerLock, __DtStep_4)
		SysPowerLockUnwrap(psSysData);
	}
#endif
	 
	if ((eError == PVRSRV_OK) &&
		!bSystemPowerEvent &&
		!_IsSystemStatePowered(psSysData->eCurrentPowerState))
	{
		
		__DtTestPoint(__DtFunc_PVRSRVPowerLock, __DtStep_5)
		PVRSRVPowerUnlock(ui32CallerID);
		eError = PVRSRV_ERROR_RETRY;
	}

	__DtTestPoint(__DtFunc_PVRSRVPowerLock, __DtStep_6)
	return eError;
}


IMG_EXPORT
IMG_VOID PVRSRVPowerUnlock(IMG_UINT32	ui32CallerID)
{
	__DtTestPoint(__DtFunc_PVRSRVPowerUnlock, __DtStep_0)
	OSUnlockResource(&gpsSysData->sPowerStateChangeResource, ui32CallerID);
#if defined(SYS_CUSTOM_POWERLOCK_WRAP)
	SysPowerLockUnwrap(gpsSysData);
#endif
	__DtTestPoint(__DtFunc_PVRSRVPowerUnlock, __DtStep_1)
}


static PVRSRV_ERROR PVRSRVDevicePrePowerStateKM_AnyVaCb(PVRSRV_POWER_DEV *psPowerDevice, va_list va)
{
	PVRSRV_DEV_POWER_STATE	eNewDevicePowerState;
	PVRSRV_ERROR			eError;

	
	IMG_BOOL				bAllDevices;
	IMG_UINT32				ui32DeviceIndex;
	PVRSRV_DEV_POWER_STATE	eNewPowerState;

	
	__DtTestPoint(__DtFunc_PVRSRVDevicePrePowerStateKM_AnyVaCb, __DtStep_0)
	bAllDevices = va_arg(va, IMG_BOOL);
	ui32DeviceIndex = va_arg(va, IMG_UINT32);
	eNewPowerState = va_arg(va, PVRSRV_DEV_POWER_STATE);

	if (bAllDevices || (ui32DeviceIndex == psPowerDevice->ui32DeviceIndex))
	{
		__DtTestPoint(__DtFunc_PVRSRVDevicePrePowerStateKM_AnyVaCb, __DtStep_1)
		eNewDevicePowerState = (eNewPowerState == PVRSRV_DEV_POWER_STATE_DEFAULT) ?
							psPowerDevice->eDefaultPowerState : eNewPowerState;

		if (psPowerDevice->eCurrentPowerState != eNewDevicePowerState)
		{
			__DtTestPoint(__DtFunc_PVRSRVDevicePrePowerStateKM_AnyVaCb, __DtStep_2)
			if (psPowerDevice->pfnPrePower != IMG_NULL)
			{
				
				__DtTestPoint(__DtFunc_PVRSRVDevicePrePowerStateKM_AnyVaCb, __DtStep_3)
				eError = psPowerDevice->pfnPrePower(psPowerDevice->hDevCookie,
													eNewDevicePowerState,
													psPowerDevice->eCurrentPowerState);
				if (eError != PVRSRV_OK)
				{
					__DtTestPoint(__DtFunc_PVRSRVDevicePrePowerStateKM_AnyVaCb, __DtStep_4)
					return eError;
				}
			}

			
			eError = SysDevicePrePowerState(psPowerDevice->ui32DeviceIndex,
											eNewDevicePowerState,
											psPowerDevice->eCurrentPowerState);
			if (eError != PVRSRV_OK)
			{
				__DtTestPoint(__DtFunc_PVRSRVDevicePrePowerStateKM_AnyVaCb, __DtStep_5)
				return eError;
			}
		}
	}

	__DtTestPoint(__DtFunc_PVRSRVDevicePrePowerStateKM_AnyVaCb, __DtStep_6)
	return  PVRSRV_OK;
}

static
PVRSRV_ERROR PVRSRVDevicePrePowerStateKM(IMG_BOOL				bAllDevices,
										 IMG_UINT32				ui32DeviceIndex,
										 PVRSRV_DEV_POWER_STATE	eNewPowerState)
{
	PVRSRV_ERROR		eError;
	SYS_DATA			*psSysData;

	__DtTestPoint(__DtFunc_PVRSRVDevicePrePowerStateKM, __DtStep_0)
	SysAcquireData(&psSysData);

	
	eError = List_PVRSRV_POWER_DEV_PVRSRV_ERROR_Any_va(psSysData->psPowerDeviceList,
														&PVRSRVDevicePrePowerStateKM_AnyVaCb,
														bAllDevices,
														ui32DeviceIndex,
														eNewPowerState);

	__DtTestPoint(__DtFunc_PVRSRVDevicePrePowerStateKM, __DtStep_1)
	return eError;
}

static PVRSRV_ERROR PVRSRVDevicePostPowerStateKM_AnyVaCb(PVRSRV_POWER_DEV *psPowerDevice, va_list va)
{
	PVRSRV_DEV_POWER_STATE	eNewDevicePowerState;
	PVRSRV_ERROR			eError;

	
	IMG_BOOL				bAllDevices;
	IMG_UINT32				ui32DeviceIndex;
	PVRSRV_DEV_POWER_STATE	eNewPowerState;

	
	__DtTestPoint(__DtFunc_PVRSRVDevicePostPowerStateKM_AnyVaCb, __DtStep_0)
	bAllDevices = va_arg(va, IMG_BOOL);
	ui32DeviceIndex = va_arg(va, IMG_UINT32);
	eNewPowerState = va_arg(va, PVRSRV_DEV_POWER_STATE);

	if (bAllDevices || (ui32DeviceIndex == psPowerDevice->ui32DeviceIndex))
	{
		__DtTestPoint(__DtFunc_PVRSRVDevicePostPowerStateKM_AnyVaCb, __DtStep_1)
		eNewDevicePowerState = (eNewPowerState == PVRSRV_DEV_POWER_STATE_DEFAULT) ?
								psPowerDevice->eDefaultPowerState : eNewPowerState;

		if (psPowerDevice->eCurrentPowerState != eNewDevicePowerState)
		{
			
			__DtTestPoint(__DtFunc_PVRSRVDevicePostPowerStateKM_AnyVaCb, __DtStep_2)
			eError = SysDevicePostPowerState(psPowerDevice->ui32DeviceIndex,
											 eNewDevicePowerState,
											 psPowerDevice->eCurrentPowerState);
			if (eError != PVRSRV_OK)
			{
				__DtTestPoint(__DtFunc_PVRSRVDevicePostPowerStateKM_AnyVaCb, __DtStep_3)
				return eError;
			}

			if (psPowerDevice->pfnPostPower != IMG_NULL)
			{
				
				__DtTestPoint(__DtFunc_PVRSRVDevicePostPowerStateKM_AnyVaCb, __DtStep_4)
				eError = psPowerDevice->pfnPostPower(psPowerDevice->hDevCookie,
													 eNewDevicePowerState,
													 psPowerDevice->eCurrentPowerState);
				if (eError != PVRSRV_OK)
				{
					__DtTestPoint(__DtFunc_PVRSRVDevicePostPowerStateKM_AnyVaCb, __DtStep_5)
					return eError;
				}
			}

			psPowerDevice->eCurrentPowerState = eNewDevicePowerState;
		}
	}
	__DtTestPoint(__DtFunc_PVRSRVDevicePostPowerStateKM_AnyVaCb, __DtStep_6)
	return PVRSRV_OK;
}

static
PVRSRV_ERROR PVRSRVDevicePostPowerStateKM(IMG_BOOL					bAllDevices,
										  IMG_UINT32				ui32DeviceIndex,
										  PVRSRV_DEV_POWER_STATE	eNewPowerState)
{
	PVRSRV_ERROR		eError;
	SYS_DATA			*psSysData;

	__DtTestPoint(__DtFunc_PVRSRVDevicePostPowerStateKM, __DtStep_0)
	SysAcquireData(&psSysData);

	
	eError = List_PVRSRV_POWER_DEV_PVRSRV_ERROR_Any_va(psSysData->psPowerDeviceList,
														&PVRSRVDevicePostPowerStateKM_AnyVaCb,
														bAllDevices,
														ui32DeviceIndex,
														eNewPowerState);

	__DtTestPoint(__DtFunc_PVRSRVDevicePostPowerStateKM, __DtStep_1)
	return eError;
}


IMG_EXPORT
PVRSRV_ERROR PVRSRVSetDevicePowerStateKM(IMG_UINT32				ui32DeviceIndex,
										 PVRSRV_DEV_POWER_STATE	eNewPowerState,
										 IMG_UINT32				ui32CallerID,
										 IMG_BOOL				bRetainMutex)
{
	PVRSRV_ERROR	eError;
	SYS_DATA		*psSysData;

	__DtTestPoint(__DtFunc_PVRSRVSetDevicePowerStateKM, __DtStep_0)
	SysAcquireData(&psSysData);

	eError = PVRSRVPowerLock(ui32CallerID, IMG_FALSE);
	if(eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVSetDevicePowerStateKM, __DtStep_1)
		return eError;
	}

	#if defined(PDUMP)
	if (eNewPowerState == PVRSRV_DEV_POWER_STATE_DEFAULT)
	{
		



		__DtTestPoint(__DtFunc_PVRSRVSetDevicePowerStateKM, __DtStep_2)
		eError = PVRSRVDevicePrePowerStateKM(IMG_FALSE, ui32DeviceIndex, PVRSRV_DEV_POWER_STATE_ON);
		if(eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PVRSRVSetDevicePowerStateKM, __DtStep_3)
			goto Exit;
		}

		eError = PVRSRVDevicePostPowerStateKM(IMG_FALSE, ui32DeviceIndex, PVRSRV_DEV_POWER_STATE_ON);

		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PVRSRVSetDevicePowerStateKM, __DtStep_4)
			goto Exit;
		}

		PDUMPSUSPEND();
	}
	#endif 

	eError = PVRSRVDevicePrePowerStateKM(IMG_FALSE, ui32DeviceIndex, eNewPowerState);
	if(eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVSetDevicePowerStateKM, __DtStep_5)
		if (eNewPowerState == PVRSRV_DEV_POWER_STATE_DEFAULT)
		{
			__DtTestPoint(__DtFunc_PVRSRVSetDevicePowerStateKM, __DtStep_6)
			PDUMPRESUME();
		}
		goto Exit;
	}

	eError = PVRSRVDevicePostPowerStateKM(IMG_FALSE, ui32DeviceIndex, eNewPowerState);

	if (eNewPowerState == PVRSRV_DEV_POWER_STATE_DEFAULT)
	{
		__DtTestPoint(__DtFunc_PVRSRVSetDevicePowerStateKM, __DtStep_7)
		PDUMPRESUME();
	}

Exit:

	if(eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVSetDevicePowerStateKM, __DtStep_8)
		PVR_DPF((PVR_DBG_ERROR,
				"PVRSRVSetDevicePowerStateKM : Transition to %d FAILED 0x%x", eNewPowerState, eError));
	}

	if (!bRetainMutex || (eError != PVRSRV_OK))
	{
		__DtTestPoint(__DtFunc_PVRSRVSetDevicePowerStateKM, __DtStep_9)
		PVRSRVPowerUnlock(ui32CallerID);
	}

	__DtTestPoint(__DtFunc_PVRSRVSetDevicePowerStateKM, __DtStep_10)
	return eError;
}


IMG_EXPORT
PVRSRV_ERROR PVRSRVSystemPrePowerStateKM(PVRSRV_SYS_POWER_STATE eNewSysPowerState)
{
	PVRSRV_ERROR			eError;
	SYS_DATA				*psSysData;
	PVRSRV_DEV_POWER_STATE	eNewDevicePowerState;

	__DtTestPoint(__DtFunc_PVRSRVSystemPrePowerStateKM, __DtStep_0)
	SysAcquireData(&psSysData);

	
	eError = PVRSRVPowerLock(KERNEL_ID, IMG_TRUE);
	if(eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVSystemPrePowerStateKM, __DtStep_1)
		return eError;
	}

	if (_IsSystemStatePowered(eNewSysPowerState) !=
		_IsSystemStatePowered(psSysData->eCurrentPowerState))
	{
		__DtTestPoint(__DtFunc_PVRSRVSystemPrePowerStateKM, __DtStep_2)
		if (_IsSystemStatePowered(eNewSysPowerState))
		{
			
			__DtTestPoint(__DtFunc_PVRSRVSystemPrePowerStateKM, __DtStep_3)
			eNewDevicePowerState = PVRSRV_DEV_POWER_STATE_DEFAULT;
		}
		else
		{
			__DtTestPoint(__DtFunc_PVRSRVSystemPrePowerStateKM, __DtStep_4)
			eNewDevicePowerState = PVRSRV_DEV_POWER_STATE_OFF;
		}

		
		eError = PVRSRVDevicePrePowerStateKM(IMG_TRUE, 0, eNewDevicePowerState);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PVRSRVSystemPrePowerStateKM, __DtStep_5)
			goto ErrorExit;
		}
	}

	if (eNewSysPowerState != psSysData->eCurrentPowerState)
	{
		
		__DtTestPoint(__DtFunc_PVRSRVSystemPrePowerStateKM, __DtStep_6)
		eError = SysSystemPrePowerState(eNewSysPowerState);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PVRSRVSystemPrePowerStateKM, __DtStep_7)
			goto ErrorExit;
		}
	}

	__DtTestPoint(__DtFunc_PVRSRVSystemPrePowerStateKM, __DtStep_8)
	return eError;

ErrorExit:

	PVR_DPF((PVR_DBG_ERROR,
			"PVRSRVSystemPrePowerStateKM: Transition from %d to %d FAILED 0x%x",
			psSysData->eCurrentPowerState, eNewSysPowerState, eError));

	
	psSysData->eFailedPowerState = eNewSysPowerState;

	PVRSRVPowerUnlock(KERNEL_ID);

	__DtTestPoint(__DtFunc_PVRSRVSystemPrePowerStateKM, __DtStep_9)
	return eError;
}


IMG_EXPORT
PVRSRV_ERROR PVRSRVSystemPostPowerStateKM(PVRSRV_SYS_POWER_STATE eNewSysPowerState)
{
	PVRSRV_ERROR			eError = PVRSRV_OK;
	SYS_DATA				*psSysData;
	PVRSRV_DEV_POWER_STATE	eNewDevicePowerState;

	__DtTestPoint(__DtFunc_PVRSRVSystemPostPowerStateKM, __DtStep_0)
	SysAcquireData(&psSysData);

	if (eNewSysPowerState != psSysData->eCurrentPowerState)
	{
		
		__DtTestPoint(__DtFunc_PVRSRVSystemPostPowerStateKM, __DtStep_1)
		eError = SysSystemPostPowerState(eNewSysPowerState);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PVRSRVSystemPostPowerStateKM, __DtStep_2)
			goto Exit;
		}
	}

	if (_IsSystemStatePowered(eNewSysPowerState) !=
		_IsSystemStatePowered(psSysData->eCurrentPowerState))
	{
		__DtTestPoint(__DtFunc_PVRSRVSystemPostPowerStateKM, __DtStep_3)
		if (_IsSystemStatePowered(eNewSysPowerState))
		{
			
			__DtTestPoint(__DtFunc_PVRSRVSystemPostPowerStateKM, __DtStep_4)
			eNewDevicePowerState = PVRSRV_DEV_POWER_STATE_DEFAULT;
		}
		else
		{
			__DtTestPoint(__DtFunc_PVRSRVSystemPostPowerStateKM, __DtStep_5)
			eNewDevicePowerState = PVRSRV_DEV_POWER_STATE_OFF;
		}

		
		eError = PVRSRVDevicePostPowerStateKM(IMG_TRUE, 0, eNewDevicePowerState);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PVRSRVSystemPostPowerStateKM, __DtStep_6)
			goto Exit;
		}
	}

	PVR_DPF((PVR_DBG_MESSAGE,
			"PVRSRVSystemPostPowerStateKM: System Power Transition from %d to %d OK",
			psSysData->eCurrentPowerState, eNewSysPowerState));

	psSysData->eCurrentPowerState = eNewSysPowerState;

Exit:

	PVRSRVPowerUnlock(KERNEL_ID);

	 
	if (_IsSystemStatePowered(eNewSysPowerState) &&
			PVRSRVGetInitServerState(PVRSRV_INIT_SERVER_SUCCESSFUL))
	{
		


		__DtTestPoint(__DtFunc_PVRSRVSystemPostPowerStateKM, __DtStep_7)
		PVRSRVScheduleDeviceCallbacks();
	}

	__DtTestPoint(__DtFunc_PVRSRVSystemPostPowerStateKM, __DtStep_8)
	return eError;
}


IMG_EXPORT
PVRSRV_ERROR PVRSRVSetPowerStateKM(PVRSRV_SYS_POWER_STATE eNewSysPowerState)
{
	PVRSRV_ERROR	eError;
	SYS_DATA		*psSysData;

	__DtTestPoint(__DtFunc_PVRSRVSetPowerStateKM, __DtStep_0)
	SysAcquireData(&psSysData);

	eError = PVRSRVSystemPrePowerStateKM(eNewSysPowerState);
	if(eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVSetPowerStateKM, __DtStep_1)
		goto ErrorExit;
	}

	eError = PVRSRVSystemPostPowerStateKM(eNewSysPowerState);
	if(eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVSetPowerStateKM, __DtStep_2)
		goto ErrorExit;
	}

	
	psSysData->eFailedPowerState = PVRSRV_SYS_POWER_STATE_Unspecified;

	__DtTestPoint(__DtFunc_PVRSRVSetPowerStateKM, __DtStep_3)
	return PVRSRV_OK;

ErrorExit:

	PVR_DPF((PVR_DBG_ERROR,
			"PVRSRVSetPowerStateKM: Transition from %d to %d FAILED 0x%x",
			psSysData->eCurrentPowerState, eNewSysPowerState, eError));

	
	psSysData->eFailedPowerState = eNewSysPowerState;

	__DtTestPoint(__DtFunc_PVRSRVSetPowerStateKM, __DtStep_4)
	return eError;
}
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
int zero_pvr_suspend(int a)
{
	__DtTestPoint(__DtFunc_zero_pvr_suspend, __DtStep_0)
	if (PVRSRVSetPowerStateKM(PVRSRV_SYS_POWER_STATE_D4) != PVRSRV_OK)	printk("----- suspend error\n");
	else																printk("----- suspend success\n");
	__DtTestPoint(__DtFunc_zero_pvr_suspend, __DtStep_1)
	return 0;
}
EXPORT_SYMBOL(zero_pvr_suspend);
int zero_pvr_resume(int a)
{
	__DtTestPoint(__DtFunc_zero_pvr_resume, __DtStep_0)
	if (PVRSRVSetPowerStateKM(PVRSRV_SYS_POWER_STATE_D0) != PVRSRV_OK)	printk("----- resume error\n");
	else																printk("----- resume success\n");
	__DtTestPoint(__DtFunc_zero_pvr_resume, __DtStep_1)
	return 0;
}
EXPORT_SYMBOL(zero_pvr_resume);


PVRSRV_ERROR PVRSRVRegisterPowerDevice(IMG_UINT32					ui32DeviceIndex,
									   PFN_PRE_POWER				pfnPrePower,
									   PFN_POST_POWER				pfnPostPower,
									   PFN_PRE_CLOCKSPEED_CHANGE	pfnPreClockSpeedChange,
									   PFN_POST_CLOCKSPEED_CHANGE	pfnPostClockSpeedChange,
									   IMG_HANDLE					hDevCookie,
									   PVRSRV_DEV_POWER_STATE		eCurrentPowerState,
									   PVRSRV_DEV_POWER_STATE		eDefaultPowerState)
{
	PVRSRV_ERROR		eError;
	SYS_DATA			*psSysData;
	PVRSRV_POWER_DEV	*psPowerDevice;

	__DtTestPoint(__DtFunc_PVRSRVRegisterPowerDevice, __DtStep_0)
	if (pfnPrePower == IMG_NULL &&
		pfnPostPower == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_PVRSRVRegisterPowerDevice, __DtStep_1)
		return PVRSRVRemovePowerDevice(ui32DeviceIndex);
	}

	SysAcquireData(&psSysData);

	eError = OSAllocMem( PVRSRV_OS_PAGEABLE_HEAP,
						 sizeof(PVRSRV_POWER_DEV),
						 (IMG_VOID **)&psPowerDevice, IMG_NULL,
						 "Power Device");
	if(eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVRegisterPowerDevice, __DtStep_2)
		PVR_DPF((PVR_DBG_ERROR,"PVRSRVRegisterPowerDevice: Failed to alloc PVRSRV_POWER_DEV"));
		__DtTestPoint(__DtFunc_PVRSRVRegisterPowerDevice, __DtStep_3)
		return eError;
	}

	
	psPowerDevice->pfnPrePower = pfnPrePower;
	psPowerDevice->pfnPostPower = pfnPostPower;
	psPowerDevice->pfnPreClockSpeedChange = pfnPreClockSpeedChange;
	psPowerDevice->pfnPostClockSpeedChange = pfnPostClockSpeedChange;
	psPowerDevice->hDevCookie = hDevCookie;
	psPowerDevice->ui32DeviceIndex = ui32DeviceIndex;
	psPowerDevice->eCurrentPowerState = eCurrentPowerState;
	psPowerDevice->eDefaultPowerState = eDefaultPowerState;

	
	List_PVRSRV_POWER_DEV_Insert(&(psSysData->psPowerDeviceList), psPowerDevice);

	__DtTestPoint(__DtFunc_PVRSRVRegisterPowerDevice, __DtStep_4)
	return (PVRSRV_OK);
}


PVRSRV_ERROR PVRSRVRemovePowerDevice (IMG_UINT32 ui32DeviceIndex)
{
	SYS_DATA			*psSysData;
	PVRSRV_POWER_DEV	*psPowerDev;

	__DtTestPoint(__DtFunc_PVRSRVRemovePowerDevice, __DtStep_0)
	SysAcquireData(&psSysData);

	
	psPowerDev = (PVRSRV_POWER_DEV*)
					List_PVRSRV_POWER_DEV_Any_va(psSysData->psPowerDeviceList,
												 &MatchPowerDeviceIndex_AnyVaCb,
												 ui32DeviceIndex);

	if (psPowerDev)
	{
		__DtTestPoint(__DtFunc_PVRSRVRemovePowerDevice, __DtStep_1)
		List_PVRSRV_POWER_DEV_Remove(psPowerDev);
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(PVRSRV_POWER_DEV), psPowerDev, IMG_NULL);
		
	}

	__DtTestPoint(__DtFunc_PVRSRVRemovePowerDevice, __DtStep_2)
	return (PVRSRV_OK);
}


IMG_EXPORT
IMG_BOOL PVRSRVIsDevicePowered(IMG_UINT32 ui32DeviceIndex)
{
	SYS_DATA			*psSysData;
	PVRSRV_POWER_DEV	*psPowerDevice;

	__DtTestPoint(__DtFunc_PVRSRVIsDevicePowered, __DtStep_0)
	SysAcquireData(&psSysData);

	 
	if (OSIsResourceLocked(&psSysData->sPowerStateChangeResource, KERNEL_ID) ||
		OSIsResourceLocked(&psSysData->sPowerStateChangeResource, ISR_ID))
	{
		__DtTestPoint(__DtFunc_PVRSRVIsDevicePowered, __DtStep_1)
		return IMG_FALSE;
	}

	psPowerDevice = (PVRSRV_POWER_DEV*)
					List_PVRSRV_POWER_DEV_Any_va(psSysData->psPowerDeviceList,
												 &MatchPowerDeviceIndex_AnyVaCb,
												 ui32DeviceIndex);
	__DtTestPoint(__DtFunc_PVRSRVIsDevicePowered, __DtStep_2)
	return (psPowerDevice && (psPowerDevice->eCurrentPowerState == PVRSRV_DEV_POWER_STATE_ON))
			? IMG_TRUE : IMG_FALSE;
}


PVRSRV_ERROR PVRSRVDevicePreClockSpeedChange(IMG_UINT32	ui32DeviceIndex,
											 IMG_BOOL	bIdleDevice,
											 IMG_VOID	*pvInfo)
{
	PVRSRV_ERROR		eError = PVRSRV_OK;
	SYS_DATA			*psSysData;
	PVRSRV_POWER_DEV	*psPowerDevice;

	__DtTestPoint(__DtFunc_PVRSRVDevicePreClockSpeedChange, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(pvInfo);

	SysAcquireData(&psSysData);

	if (bIdleDevice)
	{
		
		__DtTestPoint(__DtFunc_PVRSRVDevicePreClockSpeedChange, __DtStep_1)
		eError = PVRSRVPowerLock(KERNEL_ID, IMG_FALSE);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PVRSRVDevicePreClockSpeedChange, __DtStep_2)
			PVR_DPF((PVR_DBG_ERROR,	"PVRSRVDevicePreClockSpeedChange : failed to acquire lock, error:0x%x", eError));
			__DtTestPoint(__DtFunc_PVRSRVDevicePreClockSpeedChange, __DtStep_3)
			return eError;
		}
	}

	
	psPowerDevice = (PVRSRV_POWER_DEV*)
					List_PVRSRV_POWER_DEV_Any_va(psSysData->psPowerDeviceList,
												 &MatchPowerDeviceIndex_AnyVaCb,
												 ui32DeviceIndex);

	if (psPowerDevice && psPowerDevice->pfnPostClockSpeedChange)
	{
			__DtTestPoint(__DtFunc_PVRSRVDevicePreClockSpeedChange, __DtStep_4)
			eError = psPowerDevice->pfnPreClockSpeedChange(psPowerDevice->hDevCookie,
														   bIdleDevice,
														   psPowerDevice->eCurrentPowerState);
			if (eError != PVRSRV_OK)
			{
				__DtTestPoint(__DtFunc_PVRSRVDevicePreClockSpeedChange, __DtStep_5)
				PVR_DPF((PVR_DBG_ERROR,
						"PVRSRVDevicePreClockSpeedChange : Device %u failed, error:0x%x",
						ui32DeviceIndex, eError));
			}
	}

	if (bIdleDevice && eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVDevicePreClockSpeedChange, __DtStep_6)
		PVRSRVPowerUnlock(KERNEL_ID);
	}

	__DtTestPoint(__DtFunc_PVRSRVDevicePreClockSpeedChange, __DtStep_7)
	return eError;
}


IMG_VOID PVRSRVDevicePostClockSpeedChange(IMG_UINT32	ui32DeviceIndex,
										  IMG_BOOL		bIdleDevice,
										  IMG_VOID		*pvInfo)
{
	PVRSRV_ERROR		eError;
	SYS_DATA			*psSysData;
	PVRSRV_POWER_DEV	*psPowerDevice;

	__DtTestPoint(__DtFunc_PVRSRVDevicePostClockSpeedChange, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(pvInfo);

	SysAcquireData(&psSysData);

	
	psPowerDevice = (PVRSRV_POWER_DEV*)
					List_PVRSRV_POWER_DEV_Any_va(psSysData->psPowerDeviceList,
												 &MatchPowerDeviceIndex_AnyVaCb,
												 ui32DeviceIndex);

	if (psPowerDevice && psPowerDevice->pfnPostClockSpeedChange)
	{
		__DtTestPoint(__DtFunc_PVRSRVDevicePostClockSpeedChange, __DtStep_1)
		eError = psPowerDevice->pfnPostClockSpeedChange(psPowerDevice->hDevCookie,
														bIdleDevice,
														psPowerDevice->eCurrentPowerState);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PVRSRVDevicePostClockSpeedChange, __DtStep_2)
			PVR_DPF((PVR_DBG_ERROR,
					"PVRSRVDevicePostClockSpeedChange : Device %u failed, error:0x%x",
					ui32DeviceIndex, eError));
		}
	}


	if (bIdleDevice)
	{
		
		__DtTestPoint(__DtFunc_PVRSRVDevicePostClockSpeedChange, __DtStep_3)
		PVRSRVPowerUnlock(KERNEL_ID);
	}
	__DtTestPoint(__DtFunc_PVRSRVDevicePostClockSpeedChange, __DtStep_4)
}

