#include	"DT_sysconfig.h"	/* For DynamicTracer-TestPoint */
/**********************************************************************
 *
 * Copyright(c) 2008 Imagination Technologies Ltd. All rights reserved.
 * 		Samsung Electronics System LSI. modify
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

#include "sgxdefs.h"
#include "services_headers.h"
#include "kerneldisplay.h"
#include "oemfuncs.h"
#include "sgxinfo.h"
#include "sgxinfokm.h"
#include "pdump_km.h"
#include "servicesext.h"


#if defined(SLSI_S5PC110)
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>
#include <linux/clk.h>
#include <linux/err.h>
#endif

#define REAL_HARDWARE 1
#define SGX540_BASEADDR 0xf3000000
#define MAPPING_SIZE 0x10000
//#define IRQ_3D 106
#define SGX540_IRQ IRQ_3D
//static struct resource		*mem;
//static void __iomem		*io;

#define SYS_SGX_CLOCK_SPEED					(200000000)

#if 0
#define SYS_SGX_HWRECOVERY_TIMEOUT_FREQ		(100) // 10ms (100hz)
#define SYS_SGX_PDS_TIMER_FREQ				(1000) // 1ms (1000hz)
#define SYS_SGX_ACTIVE_POWER_LATENCY_MS		(500)
#else
#define SYS_SGX_HWRECOVERY_TIMEOUT_FREQ		(100) // 10ms (100hz)
#define SYS_SGX_PDS_TIMER_FREQ				(1000) // 1ms (1000hz)
#ifndef SYS_SGX_ACTIVE_POWER_LATENCY_MS
#define SYS_SGX_ACTIVE_POWER_LATENCY_MS		(500)
#endif
#endif

typedef struct _SYS_SPECIFIC_DATA_TAG_
{
	IMG_UINT32 ui32SysSpecificData;

} SYS_SPECIFIC_DATA;
#define SYS_SPECIFIC_DATA_ENABLE_IRQ		0x00000001UL
#define SYS_SPECIFIC_DATA_ENABLE_LISR		0x00000002UL
#define SYS_SPECIFIC_DATA_ENABLE_MISR		0x00000004UL

SYS_SPECIFIC_DATA gsSysSpecificData;


SYS_DATA* gpsSysData = (SYS_DATA*)IMG_NULL;
SYS_DATA  gsSysData;


static IMG_UINT32		gui32SGXDeviceID;
static SGX_DEVICE_MAP	gsSGXDeviceMap;


IMG_CPU_VIRTADDR gsSGXRegsCPUVAddr;
IMG_CPU_VIRTADDR gsSGXSPCPUVAddr;

char version_string[] = "SGX540 S5PC110";

IMG_UINT32   PVRSRV_BridgeDispatchKM( IMG_UINT32  Ioctl,
									IMG_BYTE   *pInBuf,
									IMG_UINT32  InBufLen, 
									IMG_BYTE   *pOutBuf,
									IMG_UINT32  OutBufLen,
									IMG_UINT32 *pdwBytesTransferred);

#if defined(SLSI_S5PC110)
static struct clk		*g3d_clock;
static struct regulator		*g3d_pd_regulator;
static PVRSRV_ERROR EnableSGXClocks(void)
{
	__DtTestPoint(__DtFunc_EnableSGXClocks, __DtStep_0)
	regulator_enable(g3d_pd_regulator);
	clk_enable(g3d_clock);

	__DtTestPoint(__DtFunc_EnableSGXClocks, __DtStep_1)
	return PVRSRV_OK;
}
#if defined(SUPPORT_ACTIVE_POWER_MANAGEMENT)
static PVRSRV_ERROR DisableSGXClocks(void)
{
	__DtTestPoint(__DtFunc_DisableSGXClocks, __DtStep_0)
	clk_disable(g3d_clock);
	regulator_disable(g3d_pd_regulator);

	__DtTestPoint(__DtFunc_DisableSGXClocks, __DtStep_1)
	return PVRSRV_OK;
}
#endif
#endif

static PVRSRV_ERROR SysLocateDevices(SYS_DATA *psSysData)
{
//	PVRSRV_ERROR eError;
//	IMG_CPU_PHYADDR sCpuPAddr;

	__DtTestPoint(__DtFunc_SysLocateDevices, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(psSysData);


#if 0
	
	gsSGXDeviceMap.ui32Flags = 0x0;
	sCpuPAddr.uiAddr = SGX540_BASEADDR;
	gsSGXDeviceMap.sRegsCpuPBase = sCpuPAddr;
	gsSGXDeviceMap.sRegsSysPBase = SysCpuPAddrToSysPAddr(gsSGXDeviceMap.sRegsCpuPBase);;
	gsSGXDeviceMap.ui32RegsSize = SGX_REG_SIZE;
//	gsSGXDeviceMap.pvRegsCpuVBase = (IMG_CPU_VIRTADDR)io;

#else

	gsSGXDeviceMap.sRegsSysPBase.uiAddr = SGX540_BASEADDR;
	gsSGXDeviceMap.sRegsCpuPBase = SysSysPAddrToCpuPAddr(gsSGXDeviceMap.sRegsSysPBase);
	gsSGXDeviceMap.ui32RegsSize = SGX_REG_SIZE;
	gsSGXDeviceMap.ui32IRQ = SGX540_IRQ;
#endif


#if defined(SGX_FEATURE_HOST_PORT)
	
	gsSGXDeviceMap.sHPSysPBase.uiAddr = 0;
	gsSGXDeviceMap.sHPCpuPBase.uiAddr = 0;
	gsSGXDeviceMap.ui32HPSize = 0;
#endif

	
	gsSGXDeviceMap.sLocalMemSysPBase.uiAddr = 0;
	gsSGXDeviceMap.sLocalMemDevPBase.uiAddr = 0;
	gsSGXDeviceMap.sLocalMemCpuPBase.uiAddr = 0;
	gsSGXDeviceMap.ui32LocalMemSize = 0;

	
	gsSGXDeviceMap.ui32IRQ = SGX540_IRQ;

	

	__DtTestPoint(__DtFunc_SysLocateDevices, __DtStep_1)
	return PVRSRV_OK;
}



PVRSRV_ERROR SysInitialise()
{
	IMG_UINT32			i;
	PVRSRV_ERROR 		eError;
	PVRSRV_DEVICE_NODE	*psDeviceNode;
	SGX_TIMING_INFORMATION*	psTimingInfo;
	struct platform_device	*pdev;

	__DtTestPoint(__DtFunc_SysInitialise, __DtStep_0)
	gpsSysData = &gsSysData;
	OSMemSet(gpsSysData, 0, sizeof(SYS_DATA));

	pdev = gpsPVRLDMDev;
	g3d_pd_regulator = regulator_get(&pdev->dev, "pd");
	if (IS_ERR(g3d_pd_regulator)) {
		__DtTestPoint(__DtFunc_SysInitialise, __DtStep_1)
		printk("\nG3D failed to find g3d power domain\n");
		__DtTestPoint(__DtFunc_SysInitialise, __DtStep_2)
		return PVRSRV_ERROR_INIT_FAILURE;
	}

	g3d_clock = clk_get(&pdev->dev, "sclk");
	if (IS_ERR(g3d_clock)) {
		__DtTestPoint(__DtFunc_SysInitialise, __DtStep_3)
		printk("\n3D failed to find g3d clock source-enable\n");
		__DtTestPoint(__DtFunc_SysInitialise, __DtStep_4)
		return PVRSRV_ERROR_INIT_FAILURE;
	}

	EnableSGXClocks();

	eError = OSInitEnvData(&gpsSysData->pvEnvSpecificData);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SysInitialise, __DtStep_5)
		PVR_DPF((PVR_DBG_ERROR,"SysInitialise: Failed to setup env structure"));
		SysDeinitialise(gpsSysData);
		gpsSysData = IMG_NULL;
		__DtTestPoint(__DtFunc_SysInitialise, __DtStep_6)
		return eError;
	}
#if defined(SGX540)
//printk("SGX540 defined\n");
#endif	
//printk("SGX_CORE_REV=%d\n",SGX_CORE_REV);
#if defined(SGX_FEATURE_SYSTEM_CACHE)
//printk("SGX_FEATURE_SYSTEM_CACHE defined!!!!!!!!!!!!!!\n");
#if defined(FIX_HW_BRN_25659)
//printk("FIX_HW_BRN_25659 defined!!!!!!!!!!!!!!\n");

#endif
#endif

#if defined(SGX_BYPASS_SYSTEM_CACHE)
//	printk("SGX_BYPASS_SYSTEM_CACHE defined!!!!!!!!!!!!!!!\n");
#endif

	gpsSysData->pvSysSpecificData = (IMG_PVOID)&gsSysSpecificData;
	OSMemSet(&gsSGXDeviceMap, 0, sizeof(SGX_DEVICE_MAP));
	
	
	psTimingInfo = &gsSGXDeviceMap.sTimingInfo;
	psTimingInfo->ui32CoreClockSpeed = SYS_SGX_CLOCK_SPEED;
	psTimingInfo->ui32HWRecoveryFreq = SYS_SGX_HWRECOVERY_TIMEOUT_FREQ; 
	psTimingInfo->ui32ActivePowManLatencyms = SYS_SGX_ACTIVE_POWER_LATENCY_MS; 
	psTimingInfo->ui32uKernelFreq = SYS_SGX_PDS_TIMER_FREQ; 

#if defined(SUPPORT_ACTIVE_POWER_MANAGEMENT)
	psTimingInfo->bEnableActivePM = IMG_TRUE;
#else	
	psTimingInfo->bEnableActivePM = IMG_FALSE;
#endif 
	gpsSysData->ui32NumDevices = SYS_DEVICE_COUNT;

	
	for(i=0; i<SYS_DEVICE_COUNT; i++)
	{
		__DtTestPoint(__DtFunc_SysInitialise, __DtStep_7)
		gpsSysData->sDeviceID[i].uiID = i;
		gpsSysData->sDeviceID[i].bInUse = IMG_FALSE;
	}

	gpsSysData->psDeviceNodeList = IMG_NULL;
	gpsSysData->psQueueList = IMG_NULL;

	eError = SysInitialiseCommon(gpsSysData);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SysInitialise, __DtStep_8)
		PVR_DPF((PVR_DBG_ERROR,"SysInitialise: Failed in SysInitialiseCommon"));
		SysDeinitialise(gpsSysData);
		gpsSysData = IMG_NULL;
		__DtTestPoint(__DtFunc_SysInitialise, __DtStep_9)
		return eError;
	}

	
	eError = SysLocateDevices(gpsSysData);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SysInitialise, __DtStep_10)
		PVR_DPF((PVR_DBG_ERROR,"SysInitialise: Failed to locate devices"));
		SysDeinitialise(gpsSysData);
		gpsSysData = IMG_NULL;
		__DtTestPoint(__DtFunc_SysInitialise, __DtStep_11)
		return eError;
	}

	
	eError = PVRSRVRegisterDevice(gpsSysData, SGXRegisterDevice, 1, &gui32SGXDeviceID);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SysInitialise, __DtStep_12)
		PVR_DPF((PVR_DBG_ERROR,"SysInitialise: Failed to register device!"));
		SysDeinitialise(gpsSysData);
		gpsSysData = IMG_NULL;
		__DtTestPoint(__DtFunc_SysInitialise, __DtStep_13)
		return eError;
	}

		
	psDeviceNode = gpsSysData->psDeviceNodeList;
	while(psDeviceNode)
	{
		
		__DtTestPoint(__DtFunc_SysInitialise, __DtStep_14)
		switch(psDeviceNode->sDevId.eDeviceType)
		{
			case PVRSRV_DEVICE_TYPE_SGX:
			{
				DEVICE_MEMORY_INFO *psDevMemoryInfo;
				DEVICE_MEMORY_HEAP_INFO *psDeviceMemoryHeap;
				IMG_UINT32 ui32MemConfig;

				__DtTestPoint(__DtFunc_SysInitialise, __DtStep_15)
				if(gpsSysData->apsLocalDevMemArena[0] != IMG_NULL)
				{
					
					__DtTestPoint(__DtFunc_SysInitialise, __DtStep_16)
					psDeviceNode->psLocalDevMemArena = gpsSysData->apsLocalDevMemArena[0];
					ui32MemConfig = PVRSRV_BACKINGSTORE_LOCALMEM_CONTIG;
				}
				else
				{
					
					__DtTestPoint(__DtFunc_SysInitialise, __DtStep_17)
					psDeviceNode->psLocalDevMemArena = IMG_NULL;
					ui32MemConfig = PVRSRV_BACKINGSTORE_SYSMEM_NONCONTIG;
				}

				
				psDevMemoryInfo = &psDeviceNode->sDevMemoryInfo;
				psDeviceMemoryHeap = psDevMemoryInfo->psDeviceMemoryHeap;

				
				for(i=0; i<psDevMemoryInfo->ui32HeapCount; i++)
				{
#if defined(SGX_FEATURE_VARIABLE_MMU_PAGE_SIZE)
					IMG_CHAR *pStr;
								
					switch(psDeviceMemoryHeap[i].ui32HeapID)
					{
						case HEAP_ID(PVRSRV_DEVICE_TYPE_SGX, SGX_GENERAL_HEAP_ID):
						{
							__DtTestPoint(__DtFunc_SysInitialise, __DtStep_18)
							pStr = "GeneralHeapPageSize";
							break;
						}
						case HEAP_ID(PVRSRV_DEVICE_TYPE_SGX, SGX_GENERAL_MAPPING_HEAP_ID):
						{
							__DtTestPoint(__DtFunc_SysInitialise, __DtStep_19)
							pStr = "GeneralMappingHeapPageSize";
							break;
						}
						case HEAP_ID(PVRSRV_DEVICE_TYPE_SGX, SGX_TADATA_HEAP_ID):
						{
							__DtTestPoint(__DtFunc_SysInitialise, __DtStep_20)
							pStr = "TADataHeapPageSize";
							break;
						}
						case HEAP_ID(PVRSRV_DEVICE_TYPE_SGX, SGX_KERNEL_CODE_HEAP_ID):
						{
							__DtTestPoint(__DtFunc_SysInitialise, __DtStep_21)
							pStr = "KernelCodeHeapPageSize";
							break;
						}
						case HEAP_ID(PVRSRV_DEVICE_TYPE_SGX, SGX_KERNEL_DATA_HEAP_ID):
						{
							__DtTestPoint(__DtFunc_SysInitialise, __DtStep_22)
							pStr = "KernelDataHeapPageSize";
							break;
						}
						case HEAP_ID(PVRSRV_DEVICE_TYPE_SGX, SGX_PIXELSHADER_HEAP_ID):
						{
							__DtTestPoint(__DtFunc_SysInitialise, __DtStep_23)
							pStr = "PixelShaderHeapPageSize";
							break;
						}
						case HEAP_ID(PVRSRV_DEVICE_TYPE_SGX, SGX_VERTEXSHADER_HEAP_ID):
						{
							__DtTestPoint(__DtFunc_SysInitialise, __DtStep_24)
							pStr = "VertexShaderHeapPageSize";
							break;
						}
						case HEAP_ID(PVRSRV_DEVICE_TYPE_SGX, SGX_PDSPIXEL_CODEDATA_HEAP_ID):
						{
							__DtTestPoint(__DtFunc_SysInitialise, __DtStep_25)
							pStr = "PDSPixelHeapPageSize";
							break;
						}
						case HEAP_ID(PVRSRV_DEVICE_TYPE_SGX, SGX_PDSVERTEX_CODEDATA_HEAP_ID):
						{
							__DtTestPoint(__DtFunc_SysInitialise, __DtStep_26)
							pStr = "PDSVertexHeapPageSize";
							break;
						}
						case HEAP_ID(PVRSRV_DEVICE_TYPE_SGX, SGX_SYNCINFO_HEAP_ID):
						{
							__DtTestPoint(__DtFunc_SysInitialise, __DtStep_27)
							pStr = "SyncInfoHeapPageSize";
							break;
						}
						case HEAP_ID(PVRSRV_DEVICE_TYPE_SGX, SGX_3DPARAMETERS_HEAP_ID):
						{
							__DtTestPoint(__DtFunc_SysInitialise, __DtStep_28)
							pStr = "3DParametersHeapPageSize";
							break;
						}
						default:
						{
							
							__DtTestPoint(__DtFunc_SysInitialise, __DtStep_29)
							pStr = IMG_NULL;
							break;	
						}
					}
					if (pStr 
					&&	OSReadRegistryDWORDFromString(0,
														PVRSRV_REGISTRY_ROOT,
														pStr,
														&psDeviceMemoryHeap[i].ui32DataPageSize) == IMG_TRUE)
					{
						__DtTestPoint(__DtFunc_SysInitialise, __DtStep_30)
						PVR_DPF((PVR_DBG_VERBOSE,"SysInitialise: set Heap %s page size to %d", pStr, psDeviceMemoryHeap[i].ui32DataPageSize));
					}
#endif
					
					psDeviceMemoryHeap[i].psLocalDevMemArena = gpsSysData->apsLocalDevMemArena[0];

					
					psDeviceMemoryHeap[i].ui32Attribs |= ui32MemConfig;
				}

				break;
			}
			default:
				__DtTestPoint(__DtFunc_SysInitialise, __DtStep_31)
				PVR_DPF((PVR_DBG_ERROR,"SysInitialise: Failed to find SGX device node!"));
				__DtTestPoint(__DtFunc_SysInitialise, __DtStep_32)
				return PVRSRV_ERROR_INIT_FAILURE;
		}

		
		psDeviceNode = psDeviceNode->psNext;
	}



	PDUMPINIT();

	
	eError = PVRSRVInitialiseDevice (gui32SGXDeviceID);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SysInitialise, __DtStep_33)
		PVR_DPF((PVR_DBG_ERROR,"SysInitialise: Failed to initialise device!"));
		SysDeinitialise(gpsSysData);
		gpsSysData = IMG_NULL;
		__DtTestPoint(__DtFunc_SysInitialise, __DtStep_34)
		return eError;
	}

#if defined(SUPPORT_ACTIVE_POWER_MANAGEMENT)
	
	DisableSGXClocks();
#endif

	return PVRSRV_OK;
}


PVRSRV_ERROR SysFinalise(IMG_VOID)
{
	PVRSRV_ERROR eError;
    
#if defined(SUPPORT_ACTIVE_POWER_MANAGEMENT)
	eError = EnableSGXClocks();
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SysFinalise, __DtStep_0)
		PVR_DPF((PVR_DBG_ERROR,"SysInitialise: Failed to Enable SGX clocks (%d)", eError));
		(IMG_VOID)SysDeinitialise(gpsSysData);
		gpsSysData = IMG_NULL;
		__DtTestPoint(__DtFunc_SysFinalise, __DtStep_1)
		return eError;
	}
#endif	
#if defined(SYS_USING_INTERRUPTS)

	eError = OSInstallMISR(gpsSysData);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SysFinalise, __DtStep_2)
		PVR_DPF((PVR_DBG_ERROR,"OSInstallMISR: Failed to install MISR"));
		SysDeinitialise(gpsSysData);
		gpsSysData = IMG_NULL;
		__DtTestPoint(__DtFunc_SysFinalise, __DtStep_3)
		return eError;
	}
	gsSysSpecificData.ui32SysSpecificData |= SYS_SPECIFIC_DATA_ENABLE_MISR;

	
	eError = OSInstallSystemLISR(gpsSysData, gsSGXDeviceMap.ui32IRQ);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SysFinalise, __DtStep_4)
		PVR_DPF((PVR_DBG_ERROR,"OSInstallSystemLISR: Failed to install ISR"));
		OSUninstallMISR(gpsSysData);
		SysDeinitialise(gpsSysData);
		gpsSysData = IMG_NULL;
		__DtTestPoint(__DtFunc_SysFinalise, __DtStep_5)
		return eError;
	}
	gsSysSpecificData.ui32SysSpecificData |= SYS_SPECIFIC_DATA_ENABLE_LISR;
	
//	SysEnableInterrupts(gpsSysData);
	gsSysSpecificData.ui32SysSpecificData |= SYS_SPECIFIC_DATA_ENABLE_IRQ;
#endif 

	
#if 0
	gpsSysData->pszVersionString = SysCreateVersionString(gsSGXDeviceMap.sRegsCpuPBase);
#else
	gpsSysData->pszVersionString=version_string;
#endif
	if (!gpsSysData->pszVersionString)
	{
		__DtTestPoint(__DtFunc_SysFinalise, __DtStep_6)
		PVR_DPF((PVR_DBG_ERROR,"SysFinalise: Failed to create a system version string"));
    }
	else
	{
		__DtTestPoint(__DtFunc_SysFinalise, __DtStep_7)
		PVR_DPF((PVR_DBG_WARNING, "SysFinalise: Version string: %s", gpsSysData->pszVersionString));
	}

#if defined(SUPPORT_ACTIVE_POWER_MANAGEMENT)
	
	DisableSGXClocks();
#endif	
//	gsSysSpecificData.bSGXInitComplete= IMG_TRUE; don't know
	
	return PVRSRV_OK;
}



PVRSRV_ERROR SysDeinitialise (SYS_DATA *psSysData)
{
	SYS_SPECIFIC_DATA * psSysSpecData;
	PVRSRV_ERROR eError;


	__DtTestPoint(__DtFunc_SysDeinitialise, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(psSysData);

	if (psSysData == IMG_NULL) {
		__DtTestPoint(__DtFunc_SysDeinitialise, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "SysDeinitialise: Called with NULL SYS_DATA pointer.  Probably called before."));
		__DtTestPoint(__DtFunc_SysDeinitialise, __DtStep_2)
		return PVRSRV_OK;
	}

#if defined(SYS_USING_INTERRUPTS)

	psSysSpecData = (SYS_SPECIFIC_DATA *) psSysData->pvSysSpecificData;

	if (psSysSpecData->ui32SysSpecificData & SYS_SPECIFIC_DATA_ENABLE_IRQ) 	
	{
//		SysDisableInterrupts(psSysData);
		__DtTestPoint(__DtFunc_SysDeinitialise, __DtStep_3)
	}
	if (psSysSpecData->ui32SysSpecificData & SYS_SPECIFIC_DATA_ENABLE_LISR)
	{	
		__DtTestPoint(__DtFunc_SysDeinitialise, __DtStep_4)
		eError = OSUninstallSystemLISR(psSysData);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SysDeinitialise, __DtStep_5)
			PVR_DPF((PVR_DBG_ERROR,"SysDeinitialise: OSUninstallSystemLISR failed"));
			__DtTestPoint(__DtFunc_SysDeinitialise, __DtStep_6)
			return eError;
		}
	}
	if (psSysSpecData->ui32SysSpecificData & SYS_SPECIFIC_DATA_ENABLE_MISR)
	{
		__DtTestPoint(__DtFunc_SysDeinitialise, __DtStep_7)
		eError = OSUninstallMISR(psSysData);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_SysDeinitialise, __DtStep_8)
			PVR_DPF((PVR_DBG_ERROR,"SysDeinitialise: OSUninstallMISR failed"));
			__DtTestPoint(__DtFunc_SysDeinitialise, __DtStep_9)
			return eError;
		}
	}
#endif

	
	eError = PVRSRVDeinitialiseDevice (gui32SGXDeviceID);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SysDeinitialise, __DtStep_10)
		PVR_DPF((PVR_DBG_ERROR,"SysDeinitialise: failed to de-init the device"));
		__DtTestPoint(__DtFunc_SysDeinitialise, __DtStep_11)
		return eError;
	}

	eError = OSDeInitEnvData(gpsSysData->pvEnvSpecificData);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_SysDeinitialise, __DtStep_12)
		PVR_DPF((PVR_DBG_ERROR,"SysDeinitialise: failed to de-init env structure"));
		__DtTestPoint(__DtFunc_SysDeinitialise, __DtStep_13)
		return eError;
	}

	SysDeinitialiseCommon(gpsSysData);


	#if REAL_HARDWARE
//	iounmap(io);
//	release_resource(mem);
//	kfree(mem);
	#else
	
	OSBaseFreeContigMemory(SGX_REG_SIZE, gsSGXRegsCPUVAddr, gsSGXDeviceMap.sRegsCpuPBase);
	OSBaseFreeContigMemory(SGX_SP_SIZE, gsSGXSPCPUVAddr, gsSGXDeviceMap.sSPCpuPBase);
	#endif

	gpsSysData = IMG_NULL;

	PDUMPDEINIT();

	__DtTestPoint(__DtFunc_SysDeinitialise, __DtStep_14)
	return PVRSRV_OK;
}



PVRSRV_ERROR SysGetDeviceMemoryMap(PVRSRV_DEVICE_TYPE eDeviceType,
									IMG_VOID **ppvDeviceMap)
{

	__DtTestPoint(__DtFunc_SysGetDeviceMemoryMap, __DtStep_0)
	switch(eDeviceType)
	{
		case PVRSRV_DEVICE_TYPE_SGX:
		{
			
			__DtTestPoint(__DtFunc_SysGetDeviceMemoryMap, __DtStep_1)
			*ppvDeviceMap = (IMG_VOID*)&gsSGXDeviceMap;

			break;
		}
		default:
		{
			__DtTestPoint(__DtFunc_SysGetDeviceMemoryMap, __DtStep_2)
			PVR_DPF((PVR_DBG_ERROR,"SysGetDeviceMemoryMap: unsupported device type"));
		}
	}
	__DtTestPoint(__DtFunc_SysGetDeviceMemoryMap, __DtStep_3)
	return PVRSRV_OK;
}



IMG_DEV_PHYADDR SysCpuPAddrToDevPAddr (PVRSRV_DEVICE_TYPE eDeviceType, 
										IMG_CPU_PHYADDR CpuPAddr)
{
	IMG_DEV_PHYADDR DevPAddr;

	__DtTestPoint(__DtFunc_SysCpuPAddrToDevPAddr, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(eDeviceType);

	
	DevPAddr.uiAddr = CpuPAddr.uiAddr;
	
	__DtTestPoint(__DtFunc_SysCpuPAddrToDevPAddr, __DtStep_1)
	return DevPAddr;
}


IMG_CPU_PHYADDR SysSysPAddrToCpuPAddr (IMG_SYS_PHYADDR sys_paddr)
{
	IMG_CPU_PHYADDR cpu_paddr;

	
	__DtTestPoint(__DtFunc_SysSysPAddrToCpuPAddr, __DtStep_0)
	cpu_paddr.uiAddr = sys_paddr.uiAddr;
	__DtTestPoint(__DtFunc_SysSysPAddrToCpuPAddr, __DtStep_1)
	return cpu_paddr;
}


IMG_SYS_PHYADDR SysCpuPAddrToSysPAddr (IMG_CPU_PHYADDR cpu_paddr)
{
	IMG_SYS_PHYADDR sys_paddr;

	
	__DtTestPoint(__DtFunc_SysCpuPAddrToSysPAddr, __DtStep_0)
	sys_paddr.uiAddr = cpu_paddr.uiAddr;
	__DtTestPoint(__DtFunc_SysCpuPAddrToSysPAddr, __DtStep_1)
	return sys_paddr;
}



IMG_DEV_PHYADDR SysSysPAddrToDevPAddr (PVRSRV_DEVICE_TYPE eDeviceType, IMG_SYS_PHYADDR SysPAddr)
{
	IMG_DEV_PHYADDR DevPAddr;

	__DtTestPoint(__DtFunc_SysSysPAddrToDevPAddr, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(eDeviceType);

	
	DevPAddr.uiAddr = SysPAddr.uiAddr;

	__DtTestPoint(__DtFunc_SysSysPAddrToDevPAddr, __DtStep_1)
	return DevPAddr;
}



IMG_SYS_PHYADDR SysDevPAddrToSysPAddr (PVRSRV_DEVICE_TYPE eDeviceType, IMG_DEV_PHYADDR DevPAddr)
{
	IMG_SYS_PHYADDR SysPAddr;

	__DtTestPoint(__DtFunc_SysDevPAddrToSysPAddr, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(eDeviceType);

	
	SysPAddr.uiAddr = DevPAddr.uiAddr;

	__DtTestPoint(__DtFunc_SysDevPAddrToSysPAddr, __DtStep_1)
	return SysPAddr;
}



IMG_VOID SysRegisterExternalDevice(PVRSRV_DEVICE_NODE *psDeviceNode)
{
	__DtTestPoint(__DtFunc_SysRegisterExternalDevice, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(psDeviceNode);
	__DtTestPoint(__DtFunc_SysRegisterExternalDevice, __DtStep_1)
}



IMG_VOID SysRemoveExternalDevice(PVRSRV_DEVICE_NODE *psDeviceNode)
{
	__DtTestPoint(__DtFunc_SysRemoveExternalDevice, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(psDeviceNode);
	__DtTestPoint(__DtFunc_SysRemoveExternalDevice, __DtStep_1)
}



IMG_UINT32 SysGetInterruptSource(SYS_DATA* psSysData,
								 PVRSRV_DEVICE_NODE *psDeviceNode)
{	
	__DtTestPoint(__DtFunc_SysGetInterruptSource, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(psSysData);
#if defined(NO_HARDWARE)
	
	return 0xFFFFFFFF;
#else
	
	//return psDeviceNode->ui32SOCInterruptBit;
	return 0x1;
#endif
}



IMG_VOID SysClearInterrupts(SYS_DATA* psSysData, IMG_UINT32 ui32ClearBits)
{
	__DtTestPoint(__DtFunc_SysClearInterrupts, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(psSysData);
	PVR_UNREFERENCED_PARAMETER(ui32ClearBits);

	//printk("SysClearInterrupts\n");
#if !defined(NO_HARDWARE)
//	OSReadHWReg(((PVRSRV_SGXDEV_INFO *)gpsSGXDevNode->pvDevice)->pvRegsBaseKM,
//										EUR_CR_EVENT_HOST_CLEAR);	//do something here
#endif
	
	__DtTestPoint(__DtFunc_SysClearInterrupts, __DtStep_1)
}



PVRSRV_ERROR SysSystemPrePowerState(PVRSRV_SYS_POWER_STATE eNewPowerState)
{
	PVRSRV_ERROR eError = PVRSRV_OK;
	__DtTestPoint(__DtFunc_SysSystemPrePowerState, __DtStep_0)
	return eError;
}


PVRSRV_ERROR SysSystemPostPowerState(PVRSRV_SYS_POWER_STATE eNewPowerState)
{
	PVRSRV_ERROR eError = PVRSRV_OK;
	__DtTestPoint(__DtFunc_SysSystemPostPowerState, __DtStep_0)
	return eError;
}



PVRSRV_ERROR SysDevicePrePowerState(IMG_UINT32			ui32DeviceIndex,
									PVRSRV_DEV_POWER_STATE		eNewPowerState,
									PVRSRV_DEV_POWER_STATE		eCurrentPowerState)
{
	__DtTestPoint(__DtFunc_SysDevicePrePowerState, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(eCurrentPowerState);

	if (ui32DeviceIndex != gui32SGXDeviceID)
	{
		__DtTestPoint(__DtFunc_SysDevicePrePowerState, __DtStep_1)
		return PVRSRV_OK;
	}

#if defined(SUPPORT_ACTIVE_POWER_MANAGEMENT)
	if (eNewPowerState == PVRSRV_DEV_POWER_STATE_OFF)
	{
		__DtTestPoint(__DtFunc_SysDevicePrePowerState, __DtStep_2)
		PVRSRVSetDCState(DC_STATE_FLUSH_COMMANDS);
		PVR_DPF((PVR_DBG_MESSAGE, "SysDevicePrePowerState: SGX Entering state D3"));
		DisableSGXClocks();
		PVRSRVSetDCState(DC_STATE_NO_FLUSH_COMMANDS);
	}
#else	
	PVR_UNREFERENCED_PARAMETER(eNewPowerState);
#endif   
	return PVRSRV_OK;
}



PVRSRV_ERROR SysDevicePostPowerState(IMG_UINT32			ui32DeviceIndex,
									PVRSRV_DEV_POWER_STATE		eNewPowerState,
									PVRSRV_DEV_POWER_STATE		eCurrentPowerState)
{
	PVRSRV_ERROR eError = PVRSRV_OK;

	__DtTestPoint(__DtFunc_SysDevicePostPowerState, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(eNewPowerState);

	if (ui32DeviceIndex != gui32SGXDeviceID)
	{
		__DtTestPoint(__DtFunc_SysDevicePostPowerState, __DtStep_1)
		return eError;
	}

#if defined(SUPPORT_ACTIVE_POWER_MANAGEMENT)
	if (eNewPowerState == PVRSRV_DEV_POWER_STATE_ON)
	{
		__DtTestPoint(__DtFunc_SysDevicePostPowerState, __DtStep_2)
		PVR_DPF((PVR_DBG_MESSAGE, "SysDevicePostPowerState: SGX Leaving state D3"));
		eError = EnableSGXClocks();
	}
#else
	PVR_UNREFERENCED_PARAMETER(eCurrentPowerState);
#endif   

	return eError;
}



PVRSRV_ERROR SysOEMFunction(IMG_UINT32	ui32ID, 
							IMG_VOID	*pvIn,
							IMG_UINT32	ulInSize,
							IMG_VOID	*pvOut,
							IMG_UINT32	ulOutSize)
{
	__DtTestPoint(__DtFunc_SysOEMFunction, __DtStep_0)
	if (ulInSize || pvIn);

	if ((ui32ID == OEM_GET_EXT_FUNCS) &&
		(ulOutSize == sizeof(PVRSRV_DC_OEM_JTABLE)))
	{
		PVRSRV_DC_OEM_JTABLE *psOEMJTable = (PVRSRV_DC_OEM_JTABLE*)pvOut;
		__DtTestPoint(__DtFunc_SysOEMFunction, __DtStep_1)
		psOEMJTable->pfnOEMBridgeDispatch = &PVRSRV_BridgeDispatchKM;

		__DtTestPoint(__DtFunc_SysOEMFunction, __DtStep_2)
		return PVRSRV_OK;
	}

	__DtTestPoint(__DtFunc_SysOEMFunction, __DtStep_3)
	return PVRSRV_ERROR_INVALID_PARAMS;
}


PVRSRV_ERROR SysPowerLockWrap(SYS_DATA unref__ *psSysData)
{                                                               
	__DtTestPoint(__DtFunc_SysPowerLockWrap, __DtStep_0)
	return PVRSRV_OK;                                       
}                                                               
                                                                
IMG_VOID SysPowerLockUnwrap(SYS_DATA unref__ *psSysData)
{                                                               
	__DtTestPoint(__DtFunc_SysPowerLockUnwrap, __DtStep_0)
}                                                               



