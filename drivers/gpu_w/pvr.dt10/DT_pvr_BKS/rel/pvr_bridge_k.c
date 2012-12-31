#include	"DT_pvr_bridge_k.h"	/* For DynamicTracer-TestPoint */
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

#include "img_defs.h"
#include "services.h"
#include "pvr_bridge.h"
#include "perproc.h"
#include "mutex.h"
#include "syscommon.h"
#include "pvr_debug.h"
#include "proc.h"
#include "private_data.h"
#include "linkage.h"
#include "pvr_bridge_km.h"

#if defined(SUPPORT_DRI_DRM)
#include <drm/drmP.h>
#include "pvr_drm.h"
#if defined(PVR_SECURE_DRM_AUTH_EXPORT)
#include "env_perproc.h"
#endif
#endif

#if defined(SUPPORT_VGX)
#include "vgx_bridge.h"
#endif

#if defined(SUPPORT_SGX)
#include "sgx_bridge.h"
#endif

#include "bridged_pvr_bridge.h"

#ifdef MODULE_TEST
#include "pvr_test_bridge.h"
#include "kern_test.h"
#endif


#if defined(SUPPORT_DRI_DRM)
#define	PRIVATE_DATA(pFile) ((pFile)->driver_priv)
#else
#define	PRIVATE_DATA(pFile) ((pFile)->private_data)
#endif

#if defined(DEBUG_BRIDGE_KM)

static struct proc_dir_entry *g_ProcBridgeStats =0;
static void* ProcSeqNextBridgeStats(struct seq_file *sfile,void* el,loff_t off);
static void ProcSeqShowBridgeStats(struct seq_file *sfile,void* el);
static void* ProcSeqOff2ElementBridgeStats(struct seq_file * sfile, loff_t off);
static void ProcSeqStartstopBridgeStats(struct seq_file *sfile,IMG_BOOL start);

#endif

extern PVRSRV_LINUX_MUTEX gPVRSRVLock;

#if defined(SUPPORT_MEMINFO_IDS)
static IMG_UINT64 ui64Stamp;
#endif 

PVRSRV_ERROR
LinuxBridgeInit(IMG_VOID)
{
#if defined(DEBUG_BRIDGE_KM)
	{
		g_ProcBridgeStats = CreateProcReadEntrySeq(
												  "bridge_stats", 
												  NULL,
												  ProcSeqNextBridgeStats,
												  ProcSeqShowBridgeStats,
												  ProcSeqOff2ElementBridgeStats,
												  ProcSeqStartstopBridgeStats
						  						 );
		if(!g_ProcBridgeStats)
		{
			__DtTestPoint(__DtFunc_LinuxBridgeInit, __DtStep_0)
			return PVRSRV_ERROR_OUT_OF_MEMORY;
		}
	}
#endif
	return CommonBridgeInit();
}

IMG_VOID
LinuxBridgeDeInit(IMG_VOID)
{
#if defined(DEBUG_BRIDGE_KM)
    RemoveProcEntrySeq(g_ProcBridgeStats);
#endif
	__DtTestPoint(__DtFunc_LinuxBridgeDeInit, __DtStep_0)
}

#if defined(DEBUG_BRIDGE_KM)

static void ProcSeqStartstopBridgeStats(struct seq_file *sfile,IMG_BOOL start) 
{
	__DtTestPoint(__DtFunc_ProcSeqStartstopBridgeStats, __DtStep_0)
	if(start) 
	{
		__DtTestPoint(__DtFunc_ProcSeqStartstopBridgeStats, __DtStep_1)
		LinuxLockMutex(&gPVRSRVLock);
	}
	else
	{
		__DtTestPoint(__DtFunc_ProcSeqStartstopBridgeStats, __DtStep_2)
		LinuxUnLockMutex(&gPVRSRVLock);
	}
	__DtTestPoint(__DtFunc_ProcSeqStartstopBridgeStats, __DtStep_3)
}


static void* ProcSeqOff2ElementBridgeStats(struct seq_file *sfile, loff_t off)
{
	__DtTestPoint(__DtFunc_ProcSeqOff2ElementBridgeStats, __DtStep_0)
	if(!off) 
	{
		__DtTestPoint(__DtFunc_ProcSeqOff2ElementBridgeStats, __DtStep_1)
		return PVR_PROC_SEQ_START_TOKEN;
	}

	if(off > BRIDGE_DISPATCH_TABLE_ENTRY_COUNT)
	{
		__DtTestPoint(__DtFunc_ProcSeqOff2ElementBridgeStats, __DtStep_2)
		return (void*)0;
	}


	__DtTestPoint(__DtFunc_ProcSeqOff2ElementBridgeStats, __DtStep_3)
	return (void*)&g_BridgeDispatchTable[off-1];
}

static void* ProcSeqNextBridgeStats(struct seq_file *sfile,void* el,loff_t off)
{
	__DtTestPoint(__DtFunc_ProcSeqNextBridgeStats, __DtStep_0)
	return ProcSeqOff2ElementBridgeStats(sfile,off);
}


static void ProcSeqShowBridgeStats(struct seq_file *sfile,void* el)
{
	PVRSRV_BRIDGE_DISPATCH_TABLE_ENTRY *psEntry = (	PVRSRV_BRIDGE_DISPATCH_TABLE_ENTRY*)el;

	__DtTestPoint(__DtFunc_ProcSeqShowBridgeStats, __DtStep_0)
	if(el == PVR_PROC_SEQ_START_TOKEN) 
	{
		__DtTestPoint(__DtFunc_ProcSeqShowBridgeStats, __DtStep_1)
		seq_printf(sfile,
						  "Total ioctl call count = %u\n"
						  "Total number of bytes copied via copy_from_user = %u\n"
						  "Total number of bytes copied via copy_to_user = %u\n"
						  "Total number of bytes copied via copy_*_user = %u\n\n"
						  "%-45s | %-40s | %10s | %20s | %10s\n",
						  g_BridgeGlobalStats.ui32IOCTLCount,
						  g_BridgeGlobalStats.ui32TotalCopyFromUserBytes,
						  g_BridgeGlobalStats.ui32TotalCopyToUserBytes,
						  g_BridgeGlobalStats.ui32TotalCopyFromUserBytes+g_BridgeGlobalStats.ui32TotalCopyToUserBytes,
						  "Bridge Name",
						  "Wrapper Function",
						  "Call Count",
						  "copy_from_user Bytes",
						  "copy_to_user Bytes"
						 );
		__DtTestPoint(__DtFunc_ProcSeqShowBridgeStats, __DtStep_2)
		return;
	}

	seq_printf(sfile,
				   "%-45s   %-40s   %-10u   %-20u   %-10u\n",
				   psEntry->pszIOCName,
				   psEntry->pszFunctionName,
				   psEntry->ui32CallCount,
				   psEntry->ui32CopyFromUserTotalBytes,
				   psEntry->ui32CopyToUserTotalBytes);
	__DtTestPoint(__DtFunc_ProcSeqShowBridgeStats, __DtStep_3)
}

#endif 


#if defined(SUPPORT_DRI_DRM)
int
PVRSRV_BridgeDispatchKM(struct drm_device unref__ *dev, void *arg, struct drm_file *pFile)
#else
long
PVRSRV_BridgeDispatchKM(struct file *pFile, unsigned int unref__ ioctlCmd, unsigned long arg)
#endif
{
	IMG_UINT32 cmd;
#if !defined(SUPPORT_DRI_DRM)
	PVRSRV_BRIDGE_PACKAGE *psBridgePackageUM = (PVRSRV_BRIDGE_PACKAGE *)arg;
	PVRSRV_BRIDGE_PACKAGE sBridgePackageKM;
#endif
	PVRSRV_BRIDGE_PACKAGE *psBridgePackageKM;
	IMG_UINT32 ui32PID = OSGetCurrentProcessIDKM();
	PVRSRV_PER_PROCESS_DATA *psPerProc;
	IMG_INT err = -EFAULT;

	LinuxLockMutex(&gPVRSRVLock);

#if defined(SUPPORT_DRI_DRM)
	psBridgePackageKM = (PVRSRV_BRIDGE_PACKAGE *)arg;
	PVR_ASSERT(psBridgePackageKM != IMG_NULL);
#else
	psBridgePackageKM = &sBridgePackageKM;

	if(!OSAccessOK(PVR_VERIFY_WRITE,
				   psBridgePackageUM,
				   sizeof(PVRSRV_BRIDGE_PACKAGE)))
	{
		__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_0)
		PVR_DPF((PVR_DBG_ERROR, "%s: Received invalid pointer to function arguments",
				 __FUNCTION__));

		goto unlock_and_return;
	}
	
	
	if(OSCopyFromUser(IMG_NULL,
					  psBridgePackageKM,
					  psBridgePackageUM,
					  sizeof(PVRSRV_BRIDGE_PACKAGE))
	  != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_1)
		goto unlock_and_return;
	}
#endif

	cmd = psBridgePackageKM->ui32BridgeID;

#if defined(MODULE_TEST)
	switch (cmd)
	{
		case PVRSRV_BRIDGE_SERVICES_TEST_MEM1:
			{
				PVRSRV_ERROR eError = MemTest1();
				__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_2)
				if (psBridgePackageKM->ui32OutBufferSize == sizeof(PVRSRV_BRIDGE_RETURN))
				{
					PVRSRV_BRIDGE_RETURN* pReturn = (PVRSRV_BRIDGE_RETURN*)psBridgePackageKM->pvParamOut ;
					__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_3)
					pReturn->eError = eError;
				}
			}
			err = 0;
			goto unlock_and_return;
		case PVRSRV_BRIDGE_SERVICES_TEST_MEM2:
			{
				PVRSRV_ERROR eError = MemTest2();
				__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_4)
				if (psBridgePackageKM->ui32OutBufferSize == sizeof(PVRSRV_BRIDGE_RETURN))
				{
					PVRSRV_BRIDGE_RETURN* pReturn = (PVRSRV_BRIDGE_RETURN*)psBridgePackageKM->pvParamOut ;
					__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_5)
					pReturn->eError = eError;
				}
			}
			err = 0;
			goto unlock_and_return;

		case PVRSRV_BRIDGE_SERVICES_TEST_RESOURCE:
			{
				PVRSRV_ERROR eError = ResourceTest();
				__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_6)
				if (psBridgePackageKM->ui32OutBufferSize == sizeof(PVRSRV_BRIDGE_RETURN))
				{
					PVRSRV_BRIDGE_RETURN* pReturn = (PVRSRV_BRIDGE_RETURN*)psBridgePackageKM->pvParamOut ;
					__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_7)
					pReturn->eError = eError;
				}
			}
			err = 0;
			goto unlock_and_return;

		case PVRSRV_BRIDGE_SERVICES_TEST_EVENTOBJECT:
			{
				PVRSRV_ERROR eError = EventObjectTest();
				__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_8)
				if (psBridgePackageKM->ui32OutBufferSize == sizeof(PVRSRV_BRIDGE_RETURN))
				{
					PVRSRV_BRIDGE_RETURN* pReturn = (PVRSRV_BRIDGE_RETURN*)psBridgePackageKM->pvParamOut ;
					__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_9)
					pReturn->eError = eError;
				}
			}
			err = 0;
			goto unlock_and_return;

		case PVRSRV_BRIDGE_SERVICES_TEST_MEMMAPPING:
			{
				PVRSRV_ERROR eError = MemMappingTest();
				__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_10)
				if (psBridgePackageKM->ui32OutBufferSize == sizeof(PVRSRV_BRIDGE_RETURN))
				{
					PVRSRV_BRIDGE_RETURN* pReturn = (PVRSRV_BRIDGE_RETURN*)psBridgePackageKM->pvParamOut ;
					__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_11)
					pReturn->eError = eError;
				}
			}
			err = 0;
			goto unlock_and_return;

		case PVRSRV_BRIDGE_SERVICES_TEST_PROCESSID:
			{
				PVRSRV_ERROR eError = ProcessIDTest();
				__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_12)
				if (psBridgePackageKM->ui32OutBufferSize == sizeof(PVRSRV_BRIDGE_RETURN))
				{
					PVRSRV_BRIDGE_RETURN* pReturn = (PVRSRV_BRIDGE_RETURN*)psBridgePackageKM->pvParamOut ;
					__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_13)
					pReturn->eError = eError;
				}
			}
			err = 0;
			goto unlock_and_return;

		case PVRSRV_BRIDGE_SERVICES_TEST_CLOCKUSWAITUS:
			{
				PVRSRV_ERROR eError = ClockusWaitusTest();
				__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_14)
				if (psBridgePackageKM->ui32OutBufferSize == sizeof(PVRSRV_BRIDGE_RETURN))
				{
					PVRSRV_BRIDGE_RETURN* pReturn = (PVRSRV_BRIDGE_RETURN*)psBridgePackageKM->pvParamOut ;
					__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_15)
					pReturn->eError = eError;
				}
			}
			err = 0;
			goto unlock_and_return;

		case PVRSRV_BRIDGE_SERVICES_TEST_TIMER:
			{
				PVRSRV_ERROR eError = TimerTest();
				__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_16)
				if (psBridgePackageKM->ui32OutBufferSize == sizeof(PVRSRV_BRIDGE_RETURN))
				{
					PVRSRV_BRIDGE_RETURN* pReturn = (PVRSRV_BRIDGE_RETURN*)psBridgePackageKM->pvParamOut ;
					__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_17)
					pReturn->eError = eError;
				}
			}
			err = 0;
			goto unlock_and_return;

		case PVRSRV_BRIDGE_SERVICES_TEST_PRIVSRV:
			{
				PVRSRV_ERROR eError = PrivSrvTest();
				__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_18)
				if (psBridgePackageKM->ui32OutBufferSize == sizeof(PVRSRV_BRIDGE_RETURN))
				{
					PVRSRV_BRIDGE_RETURN* pReturn = (PVRSRV_BRIDGE_RETURN*)psBridgePackageKM->pvParamOut ;
					__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_19)
					pReturn->eError = eError;
				}
			}
			err = 0;
			goto unlock_and_return;
		case PVRSRV_BRIDGE_SERVICES_TEST_COPYDATA:
		{
			IMG_UINT32               ui32PID;
			PVRSRV_PER_PROCESS_DATA *psPerProc;
			PVRSRV_ERROR eError;
			
			__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_20)
			ui32PID = OSGetCurrentProcessIDKM();
		
			PVRSRVTrace("PVRSRV_BRIDGE_SERVICES_TEST_COPYDATA %d", ui32PID);
			
			psPerProc = PVRSRVPerProcessData(ui32PID);
						
			eError = CopyDataTest(psBridgePackageKM->pvParamIn, psBridgePackageKM->pvParamOut, psPerProc);
			
			*(PVRSRV_ERROR*)psBridgePackageKM->pvParamOut = eError;
			err = 0;
			goto unlock_and_return;
		}


		case PVRSRV_BRIDGE_SERVICES_TEST_POWERMGMT:
    			{
				PVRSRV_ERROR eError = PowerMgmtTest();
				__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_21)
				if (psBridgePackageKM->ui32OutBufferSize == sizeof(PVRSRV_BRIDGE_RETURN))
				{
					PVRSRV_BRIDGE_RETURN* pReturn = (PVRSRV_BRIDGE_RETURN*)psBridgePackageKM->pvParamOut ;
					__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_22)
					pReturn->eError = eError;
				}
			}
			err = 0;
			goto unlock_and_return;

	}
#endif
	
	if(cmd != PVRSRV_BRIDGE_CONNECT_SERVICES)
	{
		PVRSRV_ERROR eError;

		__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_23)
		eError = PVRSRVLookupHandle(KERNEL_HANDLE_BASE,
									(IMG_PVOID *)&psPerProc,
									psBridgePackageKM->hKernelServices,
									PVRSRV_HANDLE_TYPE_PERPROC_DATA);
		if(eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_24)
			PVR_DPF((PVR_DBG_ERROR, "%s: Invalid kernel services handle (%d)",
					 __FUNCTION__, eError));
			goto unlock_and_return;
		}

		if(psPerProc->ui32PID != ui32PID)
		{
			__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_25)
			PVR_DPF((PVR_DBG_ERROR, "%s: Process %d tried to access data "
					 "belonging to process %d", __FUNCTION__, ui32PID,
					 psPerProc->ui32PID));
			goto unlock_and_return;
		}
	}
	else
	{
		
		__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_26)
		psPerProc = PVRSRVPerProcessData(ui32PID);
		if(psPerProc == IMG_NULL)
		{
			__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_27)
			PVR_DPF((PVR_DBG_ERROR, "PVRSRV_BridgeDispatchKM: "
					 "Couldn't create per-process data area"));
			goto unlock_and_return;
		}
	}

	psBridgePackageKM->ui32BridgeID = PVRSRV_GET_BRIDGE_ID(psBridgePackageKM->ui32BridgeID);

#if defined(PVR_SECURE_FD_EXPORT)
	switch(cmd)
	{
		case PVRSRV_BRIDGE_EXPORT_DEVICEMEM:
		{
			PVRSRV_FILE_PRIVATE_DATA *psPrivateData = PRIVATE_DATA(pFile);

			__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_28)
			if(psPrivateData->hKernelMemInfo)
			{
				__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_29)
				PVR_DPF((PVR_DBG_ERROR, "%s: Can only export one MemInfo "
						 "per file descriptor", __FUNCTION__));
				err = -EINVAL;
				goto unlock_and_return;
			}
			break;
		}

		case PVRSRV_BRIDGE_MAP_DEV_MEMORY:
		{
			PVRSRV_BRIDGE_IN_MAP_DEV_MEMORY *psMapDevMemIN =
				(PVRSRV_BRIDGE_IN_MAP_DEV_MEMORY *)psBridgePackageKM->pvParamIn;
			PVRSRV_FILE_PRIVATE_DATA *psPrivateData = PRIVATE_DATA(pFile);

			__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_30)
			if(!psPrivateData->hKernelMemInfo)
			{
				__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_31)
				PVR_DPF((PVR_DBG_ERROR, "%s: File descriptor has no "
						 "associated MemInfo handle", __FUNCTION__));
				err = -EINVAL;
				goto unlock_and_return;
			}

			psMapDevMemIN->hKernelMemInfo = psPrivateData->hKernelMemInfo;
			break;
		}

		default:
		{
			PVRSRV_FILE_PRIVATE_DATA *psPrivateData = PRIVATE_DATA(pFile);

			__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_32)
			if(psPrivateData->hKernelMemInfo)
			{
				__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_33)
				PVR_DPF((PVR_DBG_ERROR, "%s: Import/Export handle tried "
						 "to use privileged service", __FUNCTION__));
				goto unlock_and_return;
			}
			break;
		}
	}
#endif 
#if defined(SUPPORT_DRI_DRM) && defined(PVR_SECURE_DRM_AUTH_EXPORT)
	switch(cmd)
	{
		case PVRSRV_BRIDGE_MAP_DEV_MEMORY:
		case PVRSRV_BRIDGE_MAP_DEVICECLASS_MEMORY:
		{
			PVRSRV_FILE_PRIVATE_DATA *psPrivateData;
			int authenticated = pFile->authenticated;
			PVRSRV_ENV_PER_PROCESS_DATA *psEnvPerProc;

			__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_34)
			if (authenticated)
			{
				__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_35)
				break;
			}

			
			psEnvPerProc = (PVRSRV_ENV_PER_PROCESS_DATA *)PVRSRVProcessPrivateData(psPerProc);
			if (psEnvPerProc == IMG_NULL)
			{
				__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_36)
				PVR_DPF((PVR_DBG_ERROR, "%s: Process private data not allocated", __FUNCTION__));
				err = -EFAULT;
				goto unlock_and_return;
			}

			list_for_each_entry(psPrivateData, &psEnvPerProc->sDRMAuthListHead, sDRMAuthListItem)
			{
				struct drm_file *psDRMFile = psPrivateData->psDRMFile;

				if (pFile->master == psDRMFile->master)
				{
					__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_37)
					authenticated |= psDRMFile->authenticated;
					if (authenticated)
					{
						__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_38)
						break;
					}
				}
			}

			if (!authenticated)
			{
				__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_39)
				PVR_DPF((PVR_DBG_ERROR, "%s: Not authenticated for mapping device or device class memory", __FUNCTION__));
				err = -EPERM;
				goto unlock_and_return;
			}
			break;
		}
		default:
			__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_40)
			break;
	}
#endif 

	err = BridgedDispatchKM(psPerProc, psBridgePackageKM);
	if(err != PVRSRV_OK)
		goto unlock_and_return;

	switch(cmd)
	{
#if defined(PVR_SECURE_FD_EXPORT)
		case PVRSRV_BRIDGE_EXPORT_DEVICEMEM:
		{
			PVRSRV_BRIDGE_OUT_EXPORTDEVICEMEM *psExportDeviceMemOUT =
				(PVRSRV_BRIDGE_OUT_EXPORTDEVICEMEM *)psBridgePackageKM->pvParamOut;
			PVRSRV_FILE_PRIVATE_DATA *psPrivateData = PRIVATE_DATA(pFile);

			__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_41)
			psPrivateData->hKernelMemInfo = psExportDeviceMemOUT->hMemInfo;
#if defined(SUPPORT_MEMINFO_IDS)
			psExportDeviceMemOUT->ui64Stamp = psPrivateData->ui64Stamp = ++ui64Stamp;
#endif
			break;
		}
#endif 

#if defined(SUPPORT_MEMINFO_IDS)
		case PVRSRV_BRIDGE_MAP_DEV_MEMORY:
		{
			PVRSRV_BRIDGE_OUT_MAP_DEV_MEMORY *psMapDeviceMemoryOUT =
				(PVRSRV_BRIDGE_OUT_MAP_DEV_MEMORY *)psBridgePackageKM->pvParamOut;
			PVRSRV_FILE_PRIVATE_DATA *psPrivateData = PRIVATE_DATA(pFile);
			__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_42)
			psMapDeviceMemoryOUT->sDstClientMemInfo.ui64Stamp =	psPrivateData->ui64Stamp;
			break;
		}

		case PVRSRV_BRIDGE_MAP_DEVICECLASS_MEMORY:
		{
			PVRSRV_BRIDGE_OUT_MAP_DEVICECLASS_MEMORY *psDeviceClassMemoryOUT =
				(PVRSRV_BRIDGE_OUT_MAP_DEVICECLASS_MEMORY *)psBridgePackageKM->pvParamOut;
			__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_43)
			psDeviceClassMemoryOUT->sClientMemInfo.ui64Stamp = ++ui64Stamp;
			break;
		}
#endif 

		default:
			__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_44)
			break;
	}

unlock_and_return:
	LinuxUnLockMutex(&gPVRSRVLock);
	__DtTestPoint(__DtFunc_PVRSRV_BridgeDispatchKM, __DtStep_45)
	return err;
}
