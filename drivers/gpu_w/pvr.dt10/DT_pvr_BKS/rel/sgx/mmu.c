#include	"DT_mmu.h"	/* For DynamicTracer-TestPoint */
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

#include "sgxdefs.h"
#include "sgxmmu.h"
#include "services_headers.h"
#include "buffer_manager.h"
#include "hash.h"
#include "ra.h"
#include "pdump_km.h"
#include "sgxapi_km.h"
#include "sgxinfo.h"
#include "sgxinfokm.h"
#include "mmu.h"
#include "sgxconfig.h"
#include "sgx_bridge_km.h"

#define UINT32_MAX_VALUE	0xFFFFFFFFUL

#define SGX_MAX_PD_ENTRIES	(1<<(SGX_FEATURE_ADDRESS_SPACE_SIZE - SGX_MMU_PT_SHIFT - SGX_MMU_PAGE_SHIFT))

typedef struct _MMU_PT_INFO_
{
	
	IMG_VOID *hPTPageOSMemHandle;
	IMG_CPU_VIRTADDR PTPageCpuVAddr;
	IMG_UINT32 ui32ValidPTECount;
} MMU_PT_INFO;

struct _MMU_CONTEXT_
{
	
	PVRSRV_DEVICE_NODE *psDeviceNode;

	
	IMG_CPU_VIRTADDR pvPDCpuVAddr;
	IMG_DEV_PHYADDR sPDDevPAddr;

	IMG_VOID *hPDOSMemHandle;

	
	MMU_PT_INFO *apsPTInfoList[SGX_MAX_PD_ENTRIES];

	PVRSRV_SGXDEV_INFO *psDevInfo;

#if defined(PDUMP)
	IMG_UINT32 ui32PDumpMMUContextID;
#if defined(SUPPORT_PDUMP_MULTI_PROCESS)
	IMG_BOOL bPDumpActive;
#endif
#endif

	struct _MMU_CONTEXT_ *psNext;
};

struct _MMU_HEAP_
{
	
	MMU_CONTEXT			*psMMUContext;

	

	
	IMG_UINT32			ui32PDBaseIndex;
	
	IMG_UINT32			ui32PageTableCount;
	
	IMG_UINT32			ui32PTETotal;
	
	IMG_UINT32			ui32PDEPageSizeCtrl;

	

	
	IMG_UINT32			ui32DataPageSize;
	
	IMG_UINT32			ui32DataPageBitWidth;
	
	IMG_UINT32			ui32DataPageMask;

	

	
	IMG_UINT32			ui32PTShift;
	
	IMG_UINT32			ui32PTBitWidth;
	
	IMG_UINT32			ui32PTMask;
	
	IMG_UINT32			ui32PTSize;
	
	IMG_UINT32			ui32PTECount;

	

	
	IMG_UINT32			ui32PDShift;
	
	IMG_UINT32			ui32PDBitWidth;
	
	IMG_UINT32			ui32PDMask;

	

	RA_ARENA *psVMArena;
	DEV_ARENA_DESCRIPTOR *psDevArena;
#if defined(PDUMP)
	PDUMP_MMU_ATTRIB sMMUAttrib;
#endif
};



#if defined (SUPPORT_SGX_MMU_DUMMY_PAGE)
#define DUMMY_DATA_PAGE_SIGNATURE	0xDEADBEEF
#endif

#if defined(PDUMP)
static IMG_VOID
MMU_PDumpPageTables	(MMU_HEAP *pMMUHeap,
					 IMG_DEV_VIRTADDR DevVAddr,
					 IMG_SIZE_T uSize,
					 IMG_BOOL bForUnmap,
					 IMG_HANDLE hUniqueTag);
#endif 

#define PAGE_TEST					0
#if PAGE_TEST
static IMG_VOID PageTest(IMG_VOID* pMem, IMG_DEV_PHYADDR sDevPAddr);
#endif

#define PT_DEBUG 0
#if PT_DEBUG
static IMG_VOID DumpPT(MMU_PT_INFO *psPTInfoList)
{
	IMG_UINT32 *p = (IMG_UINT32*)psPTInfoList->PTPageCpuVAddr;
	IMG_UINT32 i;

	
	__DtTestPoint(__DtFunc_DumpPT, __DtStep_0)
	for(i = 0; i < 1024; i += 8)
	{
		__DtTestPoint(__DtFunc_DumpPT, __DtStep_1)
		PVR_DPF((PVR_DBG_WARNING,
				 "%08X %08X %08X %08X %08X %08X %08X %08X\n",
				 p[i + 0], p[i + 1], p[i + 2], p[i + 3],
				 p[i + 4], p[i + 5], p[i + 6], p[i + 7]));
	}
	__DtTestPoint(__DtFunc_DumpPT, __DtStep_2)
}

static IMG_VOID CheckPT(MMU_PT_INFO *psPTInfoList)
{
	IMG_UINT32 *p = (IMG_UINT32*) psPTInfoList->PTPageCpuVAddr;
	IMG_UINT32 i, ui32Count = 0;

	
	__DtTestPoint(__DtFunc_CheckPT, __DtStep_0)
	for(i = 0; i < 1024; i++)
		if(p[i] & SGX_MMU_PTE_VALID)
			ui32Count++;

	if(psPTInfoList->ui32ValidPTECount != ui32Count)
	{
		__DtTestPoint(__DtFunc_CheckPT, __DtStep_1)
		PVR_DPF((PVR_DBG_WARNING, "ui32ValidPTECount: %u ui32Count: %u\n",
				 psPTInfoList->ui32ValidPTECount, ui32Count));
		DumpPT(psPTInfoList);
		BUG();
	}
	__DtTestPoint(__DtFunc_CheckPT, __DtStep_2)
}
#else 
static INLINE IMG_VOID DumpPT(MMU_PT_INFO *psPTInfoList)
{
	__DtTestPoint(__DtFunc_DumpPT, __DtStep_3)
	PVR_UNREFERENCED_PARAMETER(psPTInfoList);
	__DtTestPoint(__DtFunc_DumpPT, __DtStep_4)
}

static INLINE IMG_VOID CheckPT(MMU_PT_INFO *psPTInfoList)
{
	__DtTestPoint(__DtFunc_CheckPT, __DtStep_3)
	PVR_UNREFERENCED_PARAMETER(psPTInfoList);
	__DtTestPoint(__DtFunc_CheckPT, __DtStep_4)
}
#endif 


IMG_BOOL MMU_IsHeapShared(MMU_HEAP* pMMUHeap)
{
	__DtTestPoint(__DtFunc_MMU_IsHeapShared, __DtStep_0)
	switch(pMMUHeap->psDevArena->DevMemHeapType)
	{
		case DEVICE_MEMORY_HEAP_SHARED :
		case DEVICE_MEMORY_HEAP_SHARED_EXPORTED :
			__DtTestPoint(__DtFunc_MMU_IsHeapShared, __DtStep_1)
			return IMG_TRUE;
		case DEVICE_MEMORY_HEAP_PERCONTEXT :
		case DEVICE_MEMORY_HEAP_KERNEL :
			__DtTestPoint(__DtFunc_MMU_IsHeapShared, __DtStep_2)
			return IMG_FALSE;
		default:
		{
			__DtTestPoint(__DtFunc_MMU_IsHeapShared, __DtStep_3)
			PVR_DPF((PVR_DBG_ERROR, "MMU_IsHeapShared: ERROR invalid heap type"));
			__DtTestPoint(__DtFunc_MMU_IsHeapShared, __DtStep_4)
			return IMG_FALSE;
		}
	}
	__DtTestPoint(__DtFunc_MMU_IsHeapShared, __DtStep_5)
}

#ifdef SUPPORT_SGX_MMU_BYPASS
IMG_VOID
EnableHostAccess (MMU_CONTEXT *psMMUContext)
{
	IMG_UINT32 ui32RegVal;
	IMG_VOID *pvRegsBaseKM = psMMUContext->psDevInfo->pvRegsBaseKM;

	


	__DtTestPoint(__DtFunc_EnableHostAccess, __DtStep_0)
	ui32RegVal = OSReadHWReg(pvRegsBaseKM, EUR_CR_BIF_CTRL);

	OSWriteHWReg(pvRegsBaseKM,
				EUR_CR_BIF_CTRL,
				ui32RegVal | EUR_CR_BIF_CTRL_MMU_BYPASS_HOST_MASK);
	
	PDUMPREG(SGX_PDUMPREG_NAME, EUR_CR_BIF_CTRL, EUR_CR_BIF_CTRL_MMU_BYPASS_HOST_MASK);
	__DtTestPoint(__DtFunc_EnableHostAccess, __DtStep_1)
}

IMG_VOID
DisableHostAccess (MMU_CONTEXT *psMMUContext)
{
	IMG_UINT32 ui32RegVal;
	IMG_VOID *pvRegsBaseKM = psMMUContext->psDevInfo->pvRegsBaseKM;

	



	__DtTestPoint(__DtFunc_DisableHostAccess, __DtStep_0)
	OSWriteHWReg(pvRegsBaseKM,
				EUR_CR_BIF_CTRL,
				ui32RegVal & ~EUR_CR_BIF_CTRL_MMU_BYPASS_HOST_MASK);
	
	PDUMPREG(SGX_PDUMPREG_NAME, EUR_CR_BIF_CTRL, 0);
	__DtTestPoint(__DtFunc_DisableHostAccess, __DtStep_1)
}
#endif


#if defined(SGX_FEATURE_SYSTEM_CACHE)
static IMG_VOID MMU_InvalidateSystemLevelCache(PVRSRV_SGXDEV_INFO *psDevInfo)
{
	#if defined(SGX_FEATURE_MP)
	psDevInfo->ui32CacheControl |= SGXMKIF_CC_INVAL_BIF_SL;
	#else
	
	PVR_UNREFERENCED_PARAMETER(psDevInfo);
	#endif 
	__DtTestPoint(__DtFunc_MMU_InvalidateSystemLevelCache, __DtStep_0)
}
#endif 

IMG_VOID MMU_InvalidateDirectoryCache(PVRSRV_SGXDEV_INFO *psDevInfo)
{
	__DtTestPoint(__DtFunc_MMU_InvalidateDirectoryCache, __DtStep_0)
	psDevInfo->ui32CacheControl |= SGXMKIF_CC_INVAL_BIF_PD;
	#if defined(SGX_FEATURE_SYSTEM_CACHE)
	MMU_InvalidateSystemLevelCache(psDevInfo);
	#endif 
	__DtTestPoint(__DtFunc_MMU_InvalidateDirectoryCache, __DtStep_1)
}


static IMG_VOID MMU_InvalidatePageTableCache(PVRSRV_SGXDEV_INFO *psDevInfo)
{
	__DtTestPoint(__DtFunc_MMU_InvalidatePageTableCache, __DtStep_0)
	psDevInfo->ui32CacheControl |= SGXMKIF_CC_INVAL_BIF_PT;
	#if defined(SGX_FEATURE_SYSTEM_CACHE)
	MMU_InvalidateSystemLevelCache(psDevInfo);
	#endif 
	__DtTestPoint(__DtFunc_MMU_InvalidatePageTableCache, __DtStep_1)
}


static IMG_BOOL
_AllocPageTableMemory (MMU_HEAP *pMMUHeap,
						MMU_PT_INFO *psPTInfoList,
						IMG_DEV_PHYADDR	*psDevPAddr)
{
	IMG_DEV_PHYADDR	sDevPAddr;
	IMG_CPU_PHYADDR sCpuPAddr;

	


	__DtTestPoint(__DtFunc__AllocPageTableMemory, __DtStep_0)
	if(pMMUHeap->psDevArena->psDeviceMemoryHeapInfo->psLocalDevMemArena == IMG_NULL)
	{
		
		__DtTestPoint(__DtFunc__AllocPageTableMemory, __DtStep_1)
		if (OSAllocPages(PVRSRV_HAP_WRITECOMBINE | PVRSRV_HAP_KERNEL_ONLY,
						   pMMUHeap->ui32PTSize,
						   SGX_MMU_PAGE_SIZE,
						   (IMG_VOID **)&psPTInfoList->PTPageCpuVAddr,
						   &psPTInfoList->hPTPageOSMemHandle) != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc__AllocPageTableMemory, __DtStep_2)
			PVR_DPF((PVR_DBG_ERROR, "_AllocPageTableMemory: ERROR call to OSAllocPages failed"));
			__DtTestPoint(__DtFunc__AllocPageTableMemory, __DtStep_3)
			return IMG_FALSE;
		}

		
		if(psPTInfoList->PTPageCpuVAddr)
		{
			__DtTestPoint(__DtFunc__AllocPageTableMemory, __DtStep_4)
			sCpuPAddr = OSMapLinToCPUPhys(psPTInfoList->hPTPageOSMemHandle,
										  psPTInfoList->PTPageCpuVAddr);
		}
		else
		{
			
			__DtTestPoint(__DtFunc__AllocPageTableMemory, __DtStep_5)
			sCpuPAddr = OSMemHandleToCpuPAddr(psPTInfoList->hPTPageOSMemHandle, 0);
		}

		sDevPAddr = SysCpuPAddrToDevPAddr (PVRSRV_DEVICE_TYPE_SGX, sCpuPAddr);
	}
	else
	{
		IMG_SYS_PHYADDR sSysPAddr;

		


		
		__DtTestPoint(__DtFunc__AllocPageTableMemory, __DtStep_6)
		if(RA_Alloc(pMMUHeap->psDevArena->psDeviceMemoryHeapInfo->psLocalDevMemArena,
					SGX_MMU_PAGE_SIZE,
					IMG_NULL,
					IMG_NULL,
					0,
					SGX_MMU_PAGE_SIZE,
					0,
					&(sSysPAddr.uiAddr))!= IMG_TRUE)
		{
			__DtTestPoint(__DtFunc__AllocPageTableMemory, __DtStep_7)
			PVR_DPF((PVR_DBG_ERROR, "_AllocPageTableMemory: ERROR call to RA_Alloc failed"));
			__DtTestPoint(__DtFunc__AllocPageTableMemory, __DtStep_8)
			return IMG_FALSE;
		}

		
		sCpuPAddr = SysSysPAddrToCpuPAddr(sSysPAddr);
		
		psPTInfoList->PTPageCpuVAddr = OSMapPhysToLin(sCpuPAddr,
													SGX_MMU_PAGE_SIZE,
													PVRSRV_HAP_WRITECOMBINE|PVRSRV_HAP_KERNEL_ONLY,
													&psPTInfoList->hPTPageOSMemHandle);
		if(!psPTInfoList->PTPageCpuVAddr)
		{
			__DtTestPoint(__DtFunc__AllocPageTableMemory, __DtStep_9)
			PVR_DPF((PVR_DBG_ERROR, "_AllocPageTableMemory: ERROR failed to map page tables"));
			__DtTestPoint(__DtFunc__AllocPageTableMemory, __DtStep_10)
			return IMG_FALSE;
		}

		
		sDevPAddr = SysCpuPAddrToDevPAddr (PVRSRV_DEVICE_TYPE_SGX, sCpuPAddr);

		#if PAGE_TEST
		PageTest(psPTInfoList->PTPageCpuVAddr, sDevPAddr);
		#endif
	}

#if defined(SUPPORT_SGX_MMU_DUMMY_PAGE)
	{
		IMG_UINT32 *pui32Tmp;
		IMG_UINT32 i;

		pui32Tmp = (IMG_UINT32*)psPTInfoList->PTPageCpuVAddr;
		
		for(i=0; i<pMMUHeap->ui32PTECount; i++)
		{
			__DtTestPoint(__DtFunc__AllocPageTableMemory, __DtStep_11)
			pui32Tmp[i] = (pMMUHeap->psMMUContext->psDevInfo->sDummyDataDevPAddr.uiAddr>>SGX_MMU_PTE_ADDR_ALIGNSHIFT)
						| SGX_MMU_PTE_VALID;
		}
	}
#else
	
	OSMemSet(psPTInfoList->PTPageCpuVAddr, 0, pMMUHeap->ui32PTSize);
#endif

#if defined(PDUMP)
	{
		IMG_UINT32 ui32Flags = 0;
#if defined(SUPPORT_PDUMP_MULTI_PROCESS)
		
		ui32Flags |= ( MMU_IsHeapShared(pMMUHeap) ) ? PDUMP_FLAGS_PERSISTENT : 0;
#endif
		
		PDUMPMALLOCPAGETABLE(&pMMUHeap->psMMUContext->psDeviceNode->sDevId, psPTInfoList->hPTPageOSMemHandle, 0, psPTInfoList->PTPageCpuVAddr, pMMUHeap->ui32PTSize, ui32Flags, PDUMP_PT_UNIQUETAG);
		
		PDUMPMEMPTENTRIES(&pMMUHeap->sMMUAttrib, psPTInfoList->hPTPageOSMemHandle, psPTInfoList->PTPageCpuVAddr, pMMUHeap->ui32PTSize, ui32Flags, IMG_TRUE, PDUMP_PT_UNIQUETAG, PDUMP_PT_UNIQUETAG);
	}
#endif
	
	
	*psDevPAddr = sDevPAddr;

	__DtTestPoint(__DtFunc__AllocPageTableMemory, __DtStep_12)
	return IMG_TRUE;
}


static IMG_VOID
_FreePageTableMemory (MMU_HEAP *pMMUHeap, MMU_PT_INFO *psPTInfoList)
{
	



	__DtTestPoint(__DtFunc__FreePageTableMemory, __DtStep_0)
	if(pMMUHeap->psDevArena->psDeviceMemoryHeapInfo->psLocalDevMemArena == IMG_NULL)
	{
		
		__DtTestPoint(__DtFunc__FreePageTableMemory, __DtStep_1)
		OSFreePages(PVRSRV_HAP_WRITECOMBINE | PVRSRV_HAP_KERNEL_ONLY,
					  pMMUHeap->ui32PTSize,
					  psPTInfoList->PTPageCpuVAddr,
					  psPTInfoList->hPTPageOSMemHandle);
	}
	else
	{
		IMG_SYS_PHYADDR sSysPAddr;
		IMG_CPU_PHYADDR sCpuPAddr;

		
		__DtTestPoint(__DtFunc__FreePageTableMemory, __DtStep_2)
		sCpuPAddr = OSMapLinToCPUPhys(psPTInfoList->hPTPageOSMemHandle, 
									  psPTInfoList->PTPageCpuVAddr);
		sSysPAddr = SysCpuPAddrToSysPAddr (sCpuPAddr);

		
		
		OSUnMapPhysToLin(psPTInfoList->PTPageCpuVAddr,
                         SGX_MMU_PAGE_SIZE,
                         PVRSRV_HAP_WRITECOMBINE|PVRSRV_HAP_KERNEL_ONLY,
                         psPTInfoList->hPTPageOSMemHandle);

		


		RA_Free (pMMUHeap->psDevArena->psDeviceMemoryHeapInfo->psLocalDevMemArena, sSysPAddr.uiAddr, IMG_FALSE);
	}
	__DtTestPoint(__DtFunc__FreePageTableMemory, __DtStep_3)
}



static IMG_VOID
_DeferredFreePageTable (MMU_HEAP *pMMUHeap, IMG_UINT32 ui32PTIndex, IMG_BOOL bOSFreePT)
{
	IMG_UINT32 *pui32PDEntry;
	IMG_UINT32 i;
	IMG_UINT32 ui32PDIndex;
	SYS_DATA *psSysData;
	MMU_PT_INFO **ppsPTInfoList;

	__DtTestPoint(__DtFunc__DeferredFreePageTable, __DtStep_0)
	SysAcquireData(&psSysData);

	
	ui32PDIndex = pMMUHeap->psDevArena->BaseDevVAddr.uiAddr >> pMMUHeap->ui32PDShift;

	
	ppsPTInfoList = &pMMUHeap->psMMUContext->apsPTInfoList[ui32PDIndex];

	{
#if PT_DEBUG
		if(ppsPTInfoList[ui32PTIndex] && ppsPTInfoList[ui32PTIndex]->ui32ValidPTECount > 0)
		{
			__DtTestPoint(__DtFunc__DeferredFreePageTable, __DtStep_1)
			DumpPT(ppsPTInfoList[ui32PTIndex]);
			
		}
#endif

		
		PVR_ASSERT(ppsPTInfoList[ui32PTIndex] == IMG_NULL || ppsPTInfoList[ui32PTIndex]->ui32ValidPTECount == 0);
	}

#if defined(PDUMP)
	{
		IMG_UINT32 ui32Flags = 0;
#if defined(SUPPORT_PDUMP_MULTI_PROCESS)
		ui32Flags |= ( MMU_IsHeapShared(pMMUHeap) ) ? PDUMP_FLAGS_PERSISTENT : 0;
#endif
		
		PDUMPCOMMENT("Free page table (page count == %08X)", pMMUHeap->ui32PageTableCount);
		if(ppsPTInfoList[ui32PTIndex] && ppsPTInfoList[ui32PTIndex]->PTPageCpuVAddr)
		{
			__DtTestPoint(__DtFunc__DeferredFreePageTable, __DtStep_2)
			PDUMPFREEPAGETABLE(&pMMUHeap->psMMUContext->psDeviceNode->sDevId, ppsPTInfoList[ui32PTIndex]->hPTPageOSMemHandle, ppsPTInfoList[ui32PTIndex]->PTPageCpuVAddr, pMMUHeap->ui32PTSize, ui32Flags, PDUMP_PT_UNIQUETAG);
		}
	}
#endif

	switch(pMMUHeap->psDevArena->DevMemHeapType)
	{
		case DEVICE_MEMORY_HEAP_SHARED :
		case DEVICE_MEMORY_HEAP_SHARED_EXPORTED :
		{
			
			MMU_CONTEXT *psMMUContext = (MMU_CONTEXT*)pMMUHeap->psMMUContext->psDevInfo->pvMMUContextList;

			__DtTestPoint(__DtFunc__DeferredFreePageTable, __DtStep_3)
			while(psMMUContext)
			{
				
				__DtTestPoint(__DtFunc__DeferredFreePageTable, __DtStep_4)
				pui32PDEntry = (IMG_UINT32*)psMMUContext->pvPDCpuVAddr;
				pui32PDEntry += ui32PDIndex;

#if defined(SUPPORT_SGX_MMU_DUMMY_PAGE)
				
				pui32PDEntry[ui32PTIndex] = (psMMUContext->psDevInfo->sDummyPTDevPAddr.uiAddr
											>>SGX_MMU_PDE_ADDR_ALIGNSHIFT)
											| SGX_MMU_PDE_PAGE_SIZE_4K
											| SGX_MMU_PDE_VALID;
#else
				
				if(bOSFreePT)
				{
					__DtTestPoint(__DtFunc__DeferredFreePageTable, __DtStep_5)
					pui32PDEntry[ui32PTIndex] = 0;
				}
#endif
			#if defined(PDUMP)
				
			#if defined(SUPPORT_PDUMP_MULTI_PROCESS)
				if(psMMUContext->bPDumpActive)
			#endif
				{
					PDUMPPDENTRIES(&pMMUHeap->sMMUAttrib, psMMUContext->hPDOSMemHandle, (IMG_VOID*)&pui32PDEntry[ui32PTIndex], sizeof(IMG_UINT32), 0, IMG_FALSE, PDUMP_PT_UNIQUETAG, PDUMP_PT_UNIQUETAG);
				}
			#endif
				
				psMMUContext = psMMUContext->psNext;
			}
			break;
		}
		case DEVICE_MEMORY_HEAP_PERCONTEXT :
		case DEVICE_MEMORY_HEAP_KERNEL :
		{
			
			__DtTestPoint(__DtFunc__DeferredFreePageTable, __DtStep_6)
			pui32PDEntry = (IMG_UINT32*)pMMUHeap->psMMUContext->pvPDCpuVAddr;
			pui32PDEntry += ui32PDIndex;

#if defined(SUPPORT_SGX_MMU_DUMMY_PAGE)
			
			pui32PDEntry[ui32PTIndex] = (pMMUHeap->psMMUContext->psDevInfo->sDummyPTDevPAddr.uiAddr
										>>SGX_MMU_PDE_ADDR_ALIGNSHIFT)
										| SGX_MMU_PDE_PAGE_SIZE_4K
										| SGX_MMU_PDE_VALID;
#else
			
			if(bOSFreePT)
			{
				__DtTestPoint(__DtFunc__DeferredFreePageTable, __DtStep_7)
				pui32PDEntry[ui32PTIndex] = 0;
			}
#endif

			
			PDUMPPDENTRIES(&pMMUHeap->sMMUAttrib, pMMUHeap->psMMUContext->hPDOSMemHandle, (IMG_VOID*)&pui32PDEntry[ui32PTIndex], sizeof(IMG_UINT32), 0, IMG_FALSE, PDUMP_PD_UNIQUETAG, PDUMP_PT_UNIQUETAG);
			break;
		}
		default:
		{
			__DtTestPoint(__DtFunc__DeferredFreePageTable, __DtStep_8)
			PVR_DPF((PVR_DBG_ERROR, "_DeferredFreePagetable: ERROR invalid heap type"));
			__DtTestPoint(__DtFunc__DeferredFreePageTable, __DtStep_9)
			return;
		}
	}

	
	if(ppsPTInfoList[ui32PTIndex] != IMG_NULL)
	{
		__DtTestPoint(__DtFunc__DeferredFreePageTable, __DtStep_10)
		if(ppsPTInfoList[ui32PTIndex]->PTPageCpuVAddr != IMG_NULL)
		{
			IMG_PUINT32 pui32Tmp;

			__DtTestPoint(__DtFunc__DeferredFreePageTable, __DtStep_11)
			pui32Tmp = (IMG_UINT32*)ppsPTInfoList[ui32PTIndex]->PTPageCpuVAddr;

			
			for(i=0;
				(i<pMMUHeap->ui32PTETotal) && (i<pMMUHeap->ui32PTECount);
				 i++)
			{
				__DtTestPoint(__DtFunc__DeferredFreePageTable, __DtStep_12)
				pui32Tmp[i] = 0;
			}

			

			if(bOSFreePT)
			{
				__DtTestPoint(__DtFunc__DeferredFreePageTable, __DtStep_13)
				_FreePageTableMemory(pMMUHeap, ppsPTInfoList[ui32PTIndex]);
			}

			


			pMMUHeap->ui32PTETotal -= i;
		}
		else
		{
			
			__DtTestPoint(__DtFunc__DeferredFreePageTable, __DtStep_14)
			pMMUHeap->ui32PTETotal -= pMMUHeap->ui32PTECount;
		}

		if(bOSFreePT)
		{
			
			__DtTestPoint(__DtFunc__DeferredFreePageTable, __DtStep_15)
			OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP,
						sizeof(MMU_PT_INFO),
						ppsPTInfoList[ui32PTIndex],
						IMG_NULL);
			ppsPTInfoList[ui32PTIndex] = IMG_NULL;
		}
	}
	else
	{
		
		__DtTestPoint(__DtFunc__DeferredFreePageTable, __DtStep_16)
		pMMUHeap->ui32PTETotal -= pMMUHeap->ui32PTECount;
	}

	PDUMPCOMMENT("Finished free page table (page count == %08X)", pMMUHeap->ui32PageTableCount);
	__DtTestPoint(__DtFunc__DeferredFreePageTable, __DtStep_17)
}

static IMG_VOID
_DeferredFreePageTables (MMU_HEAP *pMMUHeap)
{
	IMG_UINT32 i;
#if defined(PDUMP)
	PDUMPCOMMENT("Free PTs (MMU Context ID == %u, PDBaseIndex == %u, PT count == 0x%x)",
			pMMUHeap->psMMUContext->ui32PDumpMMUContextID,
			pMMUHeap->ui32PDBaseIndex,
			pMMUHeap->ui32PageTableCount);
#endif
	for(i=0; i<pMMUHeap->ui32PageTableCount; i++)
	{
		__DtTestPoint(__DtFunc__DeferredFreePageTables, __DtStep_0)
		_DeferredFreePageTable(pMMUHeap, i, IMG_TRUE);
	}
	MMU_InvalidateDirectoryCache(pMMUHeap->psMMUContext->psDevInfo);
	__DtTestPoint(__DtFunc__DeferredFreePageTables, __DtStep_1)
}


static IMG_BOOL
_DeferredAllocPagetables(MMU_HEAP *pMMUHeap, IMG_DEV_VIRTADDR DevVAddr, IMG_UINT32 ui32Size)
{
	IMG_UINT32 ui32PageTableCount;
	IMG_UINT32 ui32PDIndex;
	IMG_UINT32 i;
	IMG_UINT32 *pui32PDEntry;
	MMU_PT_INFO **ppsPTInfoList;
	SYS_DATA *psSysData;
	IMG_DEV_VIRTADDR sHighDevVAddr;

	
#if SGX_FEATURE_ADDRESS_SPACE_SIZE < 32
	PVR_ASSERT(DevVAddr.uiAddr < (1<<SGX_FEATURE_ADDRESS_SPACE_SIZE));
#endif

	
	SysAcquireData(&psSysData);

	
	ui32PDIndex = DevVAddr.uiAddr >> pMMUHeap->ui32PDShift;

	
	
	if((UINT32_MAX_VALUE - DevVAddr.uiAddr)
		< (ui32Size + pMMUHeap->ui32DataPageMask + pMMUHeap->ui32PTMask))
	{
		
		__DtTestPoint(__DtFunc__DeferredAllocPagetables, __DtStep_0)
		sHighDevVAddr.uiAddr = UINT32_MAX_VALUE;
	}
	else
	{
		__DtTestPoint(__DtFunc__DeferredAllocPagetables, __DtStep_1)
		sHighDevVAddr.uiAddr = DevVAddr.uiAddr
								+ ui32Size
								+ pMMUHeap->ui32DataPageMask
								+ pMMUHeap->ui32PTMask;
	}

	ui32PageTableCount = sHighDevVAddr.uiAddr >> pMMUHeap->ui32PDShift;

	ui32PageTableCount -= ui32PDIndex;

	
	pui32PDEntry = (IMG_UINT32*)pMMUHeap->psMMUContext->pvPDCpuVAddr;
	pui32PDEntry += ui32PDIndex;

	
	ppsPTInfoList = &pMMUHeap->psMMUContext->apsPTInfoList[ui32PDIndex];

#if defined(PDUMP)
	PDUMPCOMMENT("Alloc PTs (MMU Context ID == %u, PDBaseIndex == %u, Size == 0x%x)",
			pMMUHeap->psMMUContext->ui32PDumpMMUContextID,
			pMMUHeap->ui32PDBaseIndex,
			ui32Size);
	PDUMPCOMMENT("Alloc page table (page count == %08X)", ui32PageTableCount);
	PDUMPCOMMENT("Page directory mods (page count == %08X)", ui32PageTableCount);
#endif
	
	for(i=0; i<ui32PageTableCount; i++)
	{
		__DtTestPoint(__DtFunc__DeferredAllocPagetables, __DtStep_2)
		if(ppsPTInfoList[i] == IMG_NULL)
		{
			__DtTestPoint(__DtFunc__DeferredAllocPagetables, __DtStep_3)
			OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP,
						 sizeof (MMU_PT_INFO),
						 (IMG_VOID **)&ppsPTInfoList[i], IMG_NULL,
						 "MMU Page Table Info");
			if (ppsPTInfoList[i] == IMG_NULL)
			{
				__DtTestPoint(__DtFunc__DeferredAllocPagetables, __DtStep_4)
				PVR_DPF((PVR_DBG_ERROR, "_DeferredAllocPagetables: ERROR call to OSAllocMem failed"));
				__DtTestPoint(__DtFunc__DeferredAllocPagetables, __DtStep_5)
				return IMG_FALSE;
			}
			OSMemSet (ppsPTInfoList[i], 0, sizeof(MMU_PT_INFO));
		}

		if(ppsPTInfoList[i]->hPTPageOSMemHandle == IMG_NULL
		&& ppsPTInfoList[i]->PTPageCpuVAddr == IMG_NULL)
		{
			IMG_DEV_PHYADDR	sDevPAddr;
#if defined(SUPPORT_SGX_MMU_DUMMY_PAGE)
			IMG_UINT32 *pui32Tmp;
			IMG_UINT32 j;
#else
			
			PVR_ASSERT(pui32PDEntry[i] == 0);
#endif

			if(_AllocPageTableMemory (pMMUHeap, ppsPTInfoList[i], &sDevPAddr) != IMG_TRUE)
			{
				__DtTestPoint(__DtFunc__DeferredAllocPagetables, __DtStep_6)
				PVR_DPF((PVR_DBG_ERROR, "_DeferredAllocPagetables: ERROR call to _AllocPageTableMemory failed"));
				__DtTestPoint(__DtFunc__DeferredAllocPagetables, __DtStep_7)
				return IMG_FALSE;
			}

			switch(pMMUHeap->psDevArena->DevMemHeapType)
			{
				case DEVICE_MEMORY_HEAP_SHARED :
				case DEVICE_MEMORY_HEAP_SHARED_EXPORTED :
				{
					
					MMU_CONTEXT *psMMUContext = (MMU_CONTEXT*)pMMUHeap->psMMUContext->psDevInfo->pvMMUContextList;

					__DtTestPoint(__DtFunc__DeferredAllocPagetables, __DtStep_8)
					while(psMMUContext)
					{
						
						__DtTestPoint(__DtFunc__DeferredAllocPagetables, __DtStep_9)
						pui32PDEntry = (IMG_UINT32*)psMMUContext->pvPDCpuVAddr;
						pui32PDEntry += ui32PDIndex;

						
						pui32PDEntry[i] = (sDevPAddr.uiAddr>>SGX_MMU_PDE_ADDR_ALIGNSHIFT)
										| pMMUHeap->ui32PDEPageSizeCtrl
										| SGX_MMU_PDE_VALID;

						#if defined(PDUMP)
						
						#if defined(SUPPORT_PDUMP_MULTI_PROCESS)
						if(psMMUContext->bPDumpActive)
						#endif
						{
							
							PDUMPPDENTRIES(&pMMUHeap->sMMUAttrib, psMMUContext->hPDOSMemHandle, (IMG_VOID*)&pui32PDEntry[i], sizeof(IMG_UINT32), 0, IMG_FALSE, PDUMP_PD_UNIQUETAG, PDUMP_PT_UNIQUETAG);
						}
						#endif 

						
						psMMUContext = psMMUContext->psNext;
					}
					break;
				}
				case DEVICE_MEMORY_HEAP_PERCONTEXT :
				case DEVICE_MEMORY_HEAP_KERNEL :
				{
					
					__DtTestPoint(__DtFunc__DeferredAllocPagetables, __DtStep_10)
					pui32PDEntry[i] = (sDevPAddr.uiAddr>>SGX_MMU_PDE_ADDR_ALIGNSHIFT)
									| pMMUHeap->ui32PDEPageSizeCtrl
									| SGX_MMU_PDE_VALID;

					
					PDUMPPDENTRIES(&pMMUHeap->sMMUAttrib, pMMUHeap->psMMUContext->hPDOSMemHandle, (IMG_VOID*)&pui32PDEntry[i], sizeof(IMG_UINT32), 0, IMG_FALSE, PDUMP_PD_UNIQUETAG, PDUMP_PT_UNIQUETAG);
					break;
				}
				default:
				{
					__DtTestPoint(__DtFunc__DeferredAllocPagetables, __DtStep_11)
					PVR_DPF((PVR_DBG_ERROR, "_DeferredAllocPagetables: ERROR invalid heap type"));
					__DtTestPoint(__DtFunc__DeferredAllocPagetables, __DtStep_12)
					return IMG_FALSE;
				}
			}

#if !defined(SGX_FEATURE_MULTIPLE_MEM_CONTEXTS)
			



			MMU_InvalidateDirectoryCache(pMMUHeap->psMMUContext->psDevInfo);
#endif
		}
		else
		{
			
			__DtTestPoint(__DtFunc__DeferredAllocPagetables, __DtStep_13)
			PVR_ASSERT(pui32PDEntry[i] != 0);
		}
	}

	#if defined(SGX_FEATURE_SYSTEM_CACHE)
	MMU_InvalidateSystemLevelCache(pMMUHeap->psMMUContext->psDevInfo);
	#endif 

	return IMG_TRUE;
}


#if defined(PDUMP)
IMG_UINT32 MMU_GetPDumpContextID(IMG_HANDLE hDevMemContext)
{
	BM_CONTEXT *pBMContext = hDevMemContext;
	__DtTestPoint(__DtFunc_MMU_GetPDumpContextID, __DtStep_0)
	PVR_ASSERT(pBMContext);
	__DtTestPoint(__DtFunc_MMU_GetPDumpContextID, __DtStep_1)
	return pBMContext->psMMUContext->ui32PDumpMMUContextID;
}

static IMG_VOID MMU_SetPDumpAttribs(PDUMP_MMU_ATTRIB *psMMUAttrib,
	PVRSRV_DEVICE_NODE *psDeviceNode,
	IMG_UINT32 ui32DataPageMask,
	IMG_UINT32 ui32PTSize)
{
	
	__DtTestPoint(__DtFunc_MMU_SetPDumpAttribs, __DtStep_0)
	psMMUAttrib->sDevId = psDeviceNode->sDevId;
	
	psMMUAttrib->pszPDRegRegion = IMG_NULL;
	psMMUAttrib->ui32DataPageMask = ui32DataPageMask;
	
	psMMUAttrib->ui32PTEValid = SGX_MMU_PTE_VALID;
	psMMUAttrib->ui32PTSize = ui32PTSize;
	psMMUAttrib->ui32PTEAlignShift = SGX_MMU_PTE_ADDR_ALIGNSHIFT;
	
	psMMUAttrib->ui32PDEMask = SGX_MMU_PDE_ADDR_MASK;
	psMMUAttrib->ui32PDEAlignShift = SGX_MMU_PDE_ADDR_ALIGNSHIFT;
	__DtTestPoint(__DtFunc_MMU_SetPDumpAttribs, __DtStep_1)
}
#endif 

PVRSRV_ERROR
MMU_Initialise (PVRSRV_DEVICE_NODE *psDeviceNode, MMU_CONTEXT **ppsMMUContext, IMG_DEV_PHYADDR *psPDDevPAddr)
{
	IMG_UINT32 *pui32Tmp;
	IMG_UINT32 i;
	IMG_CPU_VIRTADDR pvPDCpuVAddr;
	IMG_DEV_PHYADDR sPDDevPAddr;
	IMG_CPU_PHYADDR sCpuPAddr;
	MMU_CONTEXT *psMMUContext;
	IMG_HANDLE hPDOSMemHandle;
	SYS_DATA *psSysData;
	PVRSRV_SGXDEV_INFO *psDevInfo;
#if defined(PDUMP)
	PDUMP_MMU_ATTRIB sMMUAttrib;
#endif
	PVR_DPF ((PVR_DBG_MESSAGE, "MMU_Initialise"));

	SysAcquireData(&psSysData);
#if defined(PDUMP)
	
	
	MMU_SetPDumpAttribs(&sMMUAttrib, psDeviceNode,
						SGX_MMU_PAGE_MASK,
						SGX_MMU_PT_SIZE * sizeof(IMG_UINT32));
#endif

	OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP,
				 sizeof (MMU_CONTEXT),
				 (IMG_VOID **)&psMMUContext, IMG_NULL,
				 "MMU Context");
	if (psMMUContext == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_0)
		PVR_DPF((PVR_DBG_ERROR, "MMU_Initialise: ERROR call to OSAllocMem failed"));
		__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_1)
		return PVRSRV_ERROR_OUT_OF_MEMORY;
	}
	OSMemSet (psMMUContext, 0, sizeof(MMU_CONTEXT));

	
	psDevInfo = (PVRSRV_SGXDEV_INFO*)psDeviceNode->pvDevice;
	psMMUContext->psDevInfo = psDevInfo;

	
	psMMUContext->psDeviceNode = psDeviceNode;

	
	if(psDeviceNode->psLocalDevMemArena == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_2)
		if (OSAllocPages(PVRSRV_HAP_WRITECOMBINE | PVRSRV_HAP_KERNEL_ONLY,
							SGX_MMU_PAGE_SIZE,
							SGX_MMU_PAGE_SIZE,
							&pvPDCpuVAddr,
							&hPDOSMemHandle) != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_3)
			PVR_DPF((PVR_DBG_ERROR, "MMU_Initialise: ERROR call to OSAllocPages failed"));
			__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_4)
			return PVRSRV_ERROR_FAILED_TO_ALLOC_PAGES;
		}

		if(pvPDCpuVAddr)
		{
			__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_5)
			sCpuPAddr = OSMapLinToCPUPhys(hPDOSMemHandle,
										  pvPDCpuVAddr);
		}
		else
		{
			
			__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_6)
			sCpuPAddr = OSMemHandleToCpuPAddr(hPDOSMemHandle, 0);
		}
		sPDDevPAddr = SysCpuPAddrToDevPAddr (PVRSRV_DEVICE_TYPE_SGX, sCpuPAddr);

		#if PAGE_TEST
		PageTest(pvPDCpuVAddr, sPDDevPAddr);
		#endif

#if defined(SUPPORT_SGX_MMU_DUMMY_PAGE)
		
		if(!psDevInfo->pvMMUContextList)
		{
			
			__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_7)
			if (OSAllocPages(PVRSRV_HAP_WRITECOMBINE | PVRSRV_HAP_KERNEL_ONLY,
								SGX_MMU_PAGE_SIZE,
								SGX_MMU_PAGE_SIZE,
								&psDevInfo->pvDummyPTPageCpuVAddr,
								&psDevInfo->hDummyPTPageOSMemHandle) != PVRSRV_OK)
			{
				__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_8)
				PVR_DPF((PVR_DBG_ERROR, "MMU_Initialise: ERROR call to OSAllocPages failed"));
				__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_9)
				return PVRSRV_ERROR_FAILED_TO_ALLOC_PAGES;
			}

			if(psDevInfo->pvDummyPTPageCpuVAddr)
			{
				__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_10)
				sCpuPAddr = OSMapLinToCPUPhys(psDevInfo->hDummyPTPageOSMemHandle,
											  psDevInfo->pvDummyPTPageCpuVAddr);
			}
			else
			{
				
				__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_11)
				sCpuPAddr = OSMemHandleToCpuPAddr(psDevInfo->hDummyPTPageOSMemHandle, 0);
			}
			psDevInfo->sDummyPTDevPAddr = SysCpuPAddrToDevPAddr (PVRSRV_DEVICE_TYPE_SGX, sCpuPAddr);

			
			if (OSAllocPages(PVRSRV_HAP_WRITECOMBINE | PVRSRV_HAP_KERNEL_ONLY,
								SGX_MMU_PAGE_SIZE,
								SGX_MMU_PAGE_SIZE,
								&psDevInfo->pvDummyDataPageCpuVAddr,
								&psDevInfo->hDummyDataPageOSMemHandle) != PVRSRV_OK)
			{
				__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_12)
				PVR_DPF((PVR_DBG_ERROR, "MMU_Initialise: ERROR call to OSAllocPages failed"));
				__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_13)
				return PVRSRV_ERROR_FAILED_TO_ALLOC_PAGES;
			}

			if(psDevInfo->pvDummyDataPageCpuVAddr)
			{
				__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_14)
				sCpuPAddr = OSMapLinToCPUPhys(psDevInfo->hDummyPTPageOSMemHandle,
											  psDevInfo->pvDummyDataPageCpuVAddr);
			}
			else
			{
				__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_15)
				sCpuPAddr = OSMemHandleToCpuPAddr(psDevInfo->hDummyDataPageOSMemHandle, 0);
			}
			psDevInfo->sDummyDataDevPAddr = SysCpuPAddrToDevPAddr (PVRSRV_DEVICE_TYPE_SGX, sCpuPAddr);
		}
#endif 
	}
	else
	{
		IMG_SYS_PHYADDR sSysPAddr;

		
		__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_16)
		if(RA_Alloc(psDeviceNode->psLocalDevMemArena,
					SGX_MMU_PAGE_SIZE,
					IMG_NULL,
					IMG_NULL,
					0,
					SGX_MMU_PAGE_SIZE,
					0,
					&(sSysPAddr.uiAddr))!= IMG_TRUE)
		{
			__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_17)
			PVR_DPF((PVR_DBG_ERROR, "MMU_Initialise: ERROR call to RA_Alloc failed"));
			__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_18)
			return PVRSRV_ERROR_FAILED_TO_ALLOC_VIRT_MEMORY;
		}

		
		sCpuPAddr = SysSysPAddrToCpuPAddr(sSysPAddr);
		sPDDevPAddr = SysSysPAddrToDevPAddr(PVRSRV_DEVICE_TYPE_SGX, sSysPAddr);
		pvPDCpuVAddr = OSMapPhysToLin(sCpuPAddr,
										SGX_MMU_PAGE_SIZE,
										PVRSRV_HAP_WRITECOMBINE|PVRSRV_HAP_KERNEL_ONLY,
										&hPDOSMemHandle);
		if(!pvPDCpuVAddr)
		{
			__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_19)
			PVR_DPF((PVR_DBG_ERROR, "MMU_Initialise: ERROR failed to map page tables"));
			__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_20)
			return PVRSRV_ERROR_FAILED_TO_MAP_PAGE_TABLE;
		}

		#if PAGE_TEST
		PageTest(pvPDCpuVAddr, sPDDevPAddr);
		#endif

#if defined(SUPPORT_SGX_MMU_DUMMY_PAGE)
		
		if(!psDevInfo->pvMMUContextList)
		{
			
			__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_21)
			if(RA_Alloc(psDeviceNode->psLocalDevMemArena,
						SGX_MMU_PAGE_SIZE,
						IMG_NULL,
						IMG_NULL,
						0,
						SGX_MMU_PAGE_SIZE,
						0,
						&(sSysPAddr.uiAddr))!= IMG_TRUE)
			{
				__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_22)
				PVR_DPF((PVR_DBG_ERROR, "MMU_Initialise: ERROR call to RA_Alloc failed"));
				__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_23)
				return PVRSRV_ERROR_FAILED_TO_ALLOC_VIRT_MEMORY;
			}

			
			sCpuPAddr = SysSysPAddrToCpuPAddr(sSysPAddr);
			psDevInfo->sDummyPTDevPAddr = SysSysPAddrToDevPAddr(PVRSRV_DEVICE_TYPE_SGX, sSysPAddr);
			psDevInfo->pvDummyPTPageCpuVAddr = OSMapPhysToLin(sCpuPAddr,
																SGX_MMU_PAGE_SIZE,
																PVRSRV_HAP_WRITECOMBINE|PVRSRV_HAP_KERNEL_ONLY,
																&psDevInfo->hDummyPTPageOSMemHandle);
			if(!psDevInfo->pvDummyPTPageCpuVAddr)
			{
				__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_24)
				PVR_DPF((PVR_DBG_ERROR, "MMU_Initialise: ERROR failed to map page tables"));
				__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_25)
				return PVRSRV_ERROR_FAILED_TO_MAP_PAGE_TABLE;
			}

			
			if(RA_Alloc(psDeviceNode->psLocalDevMemArena,
						SGX_MMU_PAGE_SIZE,
						IMG_NULL,
						IMG_NULL,
						0,
						SGX_MMU_PAGE_SIZE,
						0,
						&(sSysPAddr.uiAddr))!= IMG_TRUE)
			{
				__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_26)
				PVR_DPF((PVR_DBG_ERROR, "MMU_Initialise: ERROR call to RA_Alloc failed"));
				__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_27)
				return PVRSRV_ERROR_FAILED_TO_ALLOC_VIRT_MEMORY;
			}

			
			sCpuPAddr = SysSysPAddrToCpuPAddr(sSysPAddr);
			psDevInfo->sDummyDataDevPAddr = SysSysPAddrToDevPAddr(PVRSRV_DEVICE_TYPE_SGX, sSysPAddr);
			psDevInfo->pvDummyDataPageCpuVAddr = OSMapPhysToLin(sCpuPAddr,
																SGX_MMU_PAGE_SIZE,
																PVRSRV_HAP_WRITECOMBINE|PVRSRV_HAP_KERNEL_ONLY,
																&psDevInfo->hDummyDataPageOSMemHandle);
			if(!psDevInfo->pvDummyDataPageCpuVAddr)
			{
				__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_28)
				PVR_DPF((PVR_DBG_ERROR, "MMU_Initialise: ERROR failed to map page tables"));
				__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_29)
				return PVRSRV_ERROR_FAILED_TO_MAP_PAGE_TABLE;
			}
		}
#endif 
	}

#if defined(PDUMP)
#if defined(SUPPORT_PDUMP_MULTI_PROCESS)
	
	{
		PVRSRV_PER_PROCESS_DATA* psPerProc = PVRSRVFindPerProcessData();
		if(psPerProc == IMG_NULL)
		{
			
			__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_30)
			psMMUContext->bPDumpActive = IMG_TRUE;
		}
		else
		{
			__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_31)
			psMMUContext->bPDumpActive = psPerProc->bPDumpActive;
		}
	}
#endif 
	
#if IMG_ADDRSPACE_PHYSADDR_BITS == 32
	PDUMPCOMMENT("Alloc page directory for new MMU context (PDDevPAddr == 0x%08x)",
			sPDDevPAddr.uiAddr);
#else
	PDUMPCOMMENT("Alloc page directory for new MMU context, 64-bit arch detected (PDDevPAddr == 0x%08x%08x)",
			sPDDevPAddr.uiHighAddr, sPDDevPAddr.uiAddr);
#endif
	PDUMPMALLOCPAGETABLE(&psDeviceNode->sDevId, hPDOSMemHandle, 0, pvPDCpuVAddr, SGX_MMU_PAGE_SIZE, 0, PDUMP_PD_UNIQUETAG);
#endif 
#ifdef SUPPORT_SGX_MMU_BYPASS
	EnableHostAccess(psMMUContext);
#endif

	if (pvPDCpuVAddr)
	{
		__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_32)
		pui32Tmp = (IMG_UINT32 *)pvPDCpuVAddr;
	}
	else
	{
		__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_33)
		PVR_DPF((PVR_DBG_ERROR, "MMU_Initialise: pvPDCpuVAddr invalid"));
		__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_34)
		return PVRSRV_ERROR_INVALID_CPU_ADDR;
	}

#if defined(SUPPORT_SGX_MMU_DUMMY_PAGE)
	
	for(i=0; i<SGX_MMU_PD_SIZE; i++)
	{
		__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_35)
		pui32Tmp[i] = (psDevInfo->sDummyPTDevPAddr.uiAddr>>SGX_MMU_PDE_ADDR_ALIGNSHIFT)
					| SGX_MMU_PDE_PAGE_SIZE_4K
					| SGX_MMU_PDE_VALID;
	}

	if(!psDevInfo->pvMMUContextList)
	{
		


		__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_36)
		pui32Tmp = (IMG_UINT32 *)psDevInfo->pvDummyPTPageCpuVAddr;
		for(i=0; i<SGX_MMU_PT_SIZE; i++)
		{
			__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_37)
			pui32Tmp[i] = (psDevInfo->sDummyDataDevPAddr.uiAddr>>SGX_MMU_PTE_ADDR_ALIGNSHIFT)
						| SGX_MMU_PTE_VALID;
		}
		
		PDUMPCOMMENT("Dummy Page table contents");
		PDUMPMEMPTENTRIES(&sMMUAttrib, psDevInfo->hDummyPTOSMemHandle, psDevInfo->pvDummyPTPageCpuVAddr, SGX_MMU_PAGE_SIZE, 0, IMG_TRUE, PDUMP_PD_UNIQUETAG, PDUMP_PT_UNIQUETAG);

		

		pui32Tmp = (IMG_UINT32 *)psDevInfo->pvDummyDataPageCpuVAddr;
		for(i=0; i<(SGX_MMU_PAGE_SIZE/4); i++)
		{
			__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_38)
			pui32Tmp[i] = DUMMY_DATA_PAGE_SIGNATURE;
		}
		
		PDUMPCOMMENT("Dummy Data Page contents");
		PDUMPMEMPTENTRIES(PVRSRV_DEVICE_TYPE_SGX, psDevInfo->hDummyDataPageOSMemHandle, psDevInfo->pvDummyDataPageCpuVAddr, SGX_MMU_PAGE_SIZE, 0, IMG_TRUE, PDUMP_PD_UNIQUETAG, PDUMP_PT_UNIQUETAG);
	}
#else 
	
	for(i=0; i<SGX_MMU_PD_SIZE; i++)
	{
		
		__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_39)
		pui32Tmp[i] = 0;
	}
#endif 

#if defined(PDUMP)
#if defined(SUPPORT_PDUMP_MULTI_PROCESS)
	if(psMMUContext->bPDumpActive)
#endif 
	{
		
		PDUMPCOMMENT("Page directory contents");
		PDUMPPDENTRIES(&sMMUAttrib, hPDOSMemHandle, pvPDCpuVAddr, SGX_MMU_PAGE_SIZE, 0, IMG_TRUE, PDUMP_PD_UNIQUETAG, PDUMP_PT_UNIQUETAG);
	}
	
	
	{
		PVRSRV_ERROR eError;
		
		IMG_UINT32 ui32MMUType = 1;

		#if defined(SGX_FEATURE_36BIT_MMU)
			ui32MMUType = 3;
		#else
			#if defined(SGX_FEATURE_VARIABLE_MMU_PAGE_SIZE)
				ui32MMUType = 2;
			#endif
		#endif

		eError = PDumpSetMMUContext(PVRSRV_DEVICE_TYPE_SGX,
									psDeviceNode->sDevId.pszPDumpDevName,
									&psMMUContext->ui32PDumpMMUContextID,
									ui32MMUType,
									PDUMP_PT_UNIQUETAG,
									hPDOSMemHandle,
									pvPDCpuVAddr);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_40)
			PVR_DPF((PVR_DBG_ERROR, "MMU_Initialise: ERROR call to PDumpSetMMUContext failed"));
			__DtTestPoint(__DtFunc_MMU_Initialise, __DtStep_41)
			return eError;
		}
	}

	
	PDUMPCOMMENT("Set MMU context complete (MMU Context ID == %u)", psMMUContext->ui32PDumpMMUContextID);
#endif

	
	psMMUContext->pvPDCpuVAddr = pvPDCpuVAddr;
	psMMUContext->sPDDevPAddr = sPDDevPAddr;
	psMMUContext->hPDOSMemHandle = hPDOSMemHandle;

	
	*ppsMMUContext = psMMUContext;

	
	*psPDDevPAddr = sPDDevPAddr;

	
	psMMUContext->psNext = (MMU_CONTEXT*)psDevInfo->pvMMUContextList;
	psDevInfo->pvMMUContextList = (IMG_VOID*)psMMUContext;

#ifdef SUPPORT_SGX_MMU_BYPASS
	DisableHostAccess(psMMUContext);
#endif

	return PVRSRV_OK;
}

IMG_VOID
MMU_Finalise (MMU_CONTEXT *psMMUContext)
{
	IMG_UINT32 *pui32Tmp, i;
	SYS_DATA *psSysData;
	MMU_CONTEXT **ppsMMUContext;
#if defined(SUPPORT_SGX_MMU_DUMMY_PAGE)
	PVRSRV_SGXDEV_INFO *psDevInfo = (PVRSRV_SGXDEV_INFO*)psMMUContext->psDevInfo;
	MMU_CONTEXT *psMMUContextList = (MMU_CONTEXT*)psDevInfo->pvMMUContextList;
#endif

	SysAcquireData(&psSysData);

#if defined(PDUMP)
	
	PDUMPCOMMENT("Clear MMU context (MMU Context ID == %u)", psMMUContext->ui32PDumpMMUContextID);
	PDUMPCLEARMMUCONTEXT(PVRSRV_DEVICE_TYPE_SGX, psMMUContext->psDeviceNode->sDevId.pszPDumpDevName, psMMUContext->ui32PDumpMMUContextID, 2);

	
#if IMG_ADDRSPACE_PHYSADDR_BITS == 32
	PDUMPCOMMENT("Free page directory (PDDevPAddr == 0x%08x)",
			psMMUContext->sPDDevPAddr.uiAddr);
#else
	PDUMPCOMMENT("Free page directory, 64-bit arch detected (PDDevPAddr == 0x%08x%08x)",
			psMMUContext->sPDDevPAddr.uiHighAddr, psMMUContext->sPDDevPAddr.uiAddr);
#endif
#endif 

	PDUMPFREEPAGETABLE(&psMMUContext->psDeviceNode->sDevId, psMMUContext->hPDOSMemHandle, psMMUContext->pvPDCpuVAddr, SGX_MMU_PAGE_SIZE, 0, PDUMP_PT_UNIQUETAG);
#if defined(SUPPORT_SGX_MMU_DUMMY_PAGE)
	PDUMPFREEPAGETABLE(&psMMUContext->psDeviceNode->sDevId, psDevInfo->hDummyPTPageOSMemHandle, psDevInfo->pvDummyPTPageCpuVAddr, SGX_MMU_PAGE_SIZE, 0, PDUMP_PT_UNIQUETAG);
	PDUMPFREEPAGETABLE(&psMMUContext->psDeviceNode->sDevId, psDevInfo->hDummyDataPageOSMemHandle, psDevInfo->pvDummyDataPageCpuVAddr, SGX_MMU_PAGE_SIZE, 0, PDUMP_PT_UNIQUETAG);
#endif

	pui32Tmp = (IMG_UINT32 *)psMMUContext->pvPDCpuVAddr;

	
	for(i=0; i<SGX_MMU_PD_SIZE; i++)
	{
		
		__DtTestPoint(__DtFunc_MMU_Finalise, __DtStep_0)
		pui32Tmp[i] = 0;
	}

	



	if(psMMUContext->psDeviceNode->psLocalDevMemArena == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_MMU_Finalise, __DtStep_1)
		OSFreePages(PVRSRV_HAP_WRITECOMBINE | PVRSRV_HAP_KERNEL_ONLY,
						SGX_MMU_PAGE_SIZE,
						psMMUContext->pvPDCpuVAddr,
						psMMUContext->hPDOSMemHandle);

#if defined(SUPPORT_SGX_MMU_DUMMY_PAGE)
		
		if(!psMMUContextList->psNext)
		{
			__DtTestPoint(__DtFunc_MMU_Finalise, __DtStep_2)
			OSFreePages(PVRSRV_HAP_WRITECOMBINE | PVRSRV_HAP_KERNEL_ONLY,
							SGX_MMU_PAGE_SIZE,
							psDevInfo->pvDummyPTPageCpuVAddr,
							psDevInfo->hDummyPTPageOSMemHandle);
			OSFreePages(PVRSRV_HAP_WRITECOMBINE | PVRSRV_HAP_KERNEL_ONLY,
							SGX_MMU_PAGE_SIZE,
							psDevInfo->pvDummyDataPageCpuVAddr,
							psDevInfo->hDummyDataPageOSMemHandle);
		}
#endif
	}
	else
	{
		IMG_SYS_PHYADDR sSysPAddr;
		IMG_CPU_PHYADDR sCpuPAddr;

		
		__DtTestPoint(__DtFunc_MMU_Finalise, __DtStep_3)
		sCpuPAddr = OSMapLinToCPUPhys(psMMUContext->hPDOSMemHandle,
									  psMMUContext->pvPDCpuVAddr);
		sSysPAddr = SysCpuPAddrToSysPAddr(sCpuPAddr);

		
		OSUnMapPhysToLin(psMMUContext->pvPDCpuVAddr,
							SGX_MMU_PAGE_SIZE,
                            PVRSRV_HAP_WRITECOMBINE|PVRSRV_HAP_KERNEL_ONLY,
							psMMUContext->hPDOSMemHandle);
		
		RA_Free (psMMUContext->psDeviceNode->psLocalDevMemArena, sSysPAddr.uiAddr, IMG_FALSE);

#if defined(SUPPORT_SGX_MMU_DUMMY_PAGE)
		
		if(!psMMUContextList->psNext)
		{
			
			__DtTestPoint(__DtFunc_MMU_Finalise, __DtStep_4)
			sCpuPAddr = OSMapLinToCPUPhys(psDevInfo->hDummyPTPageOSMemHandle,
										  psDevInfo->pvDummyPTPageCpuVAddr);
			sSysPAddr = SysCpuPAddrToSysPAddr(sCpuPAddr);

			
			OSUnMapPhysToLin(psDevInfo->pvDummyPTPageCpuVAddr,
								SGX_MMU_PAGE_SIZE,
                                PVRSRV_HAP_WRITECOMBINE|PVRSRV_HAP_KERNEL_ONLY,
								psDevInfo->hDummyPTPageOSMemHandle);
			
			RA_Free (psMMUContext->psDeviceNode->psLocalDevMemArena, sSysPAddr.uiAddr, IMG_FALSE);

			
			sCpuPAddr = OSMapLinToCPUPhys(psDevInfo->hDummyDataPageOSMemHandle,
										  psDevInfo->pvDummyDataPageCpuVAddr);
			sSysPAddr = SysCpuPAddrToSysPAddr(sCpuPAddr);

			
			OSUnMapPhysToLin(psDevInfo->pvDummyDataPageCpuVAddr,
								SGX_MMU_PAGE_SIZE,
                                PVRSRV_HAP_WRITECOMBINE|PVRSRV_HAP_KERNEL_ONLY,
								psDevInfo->hDummyDataPageOSMemHandle);
			
			RA_Free (psMMUContext->psDeviceNode->psLocalDevMemArena, sSysPAddr.uiAddr, IMG_FALSE);
		}
#endif
	}

	PVR_DPF ((PVR_DBG_MESSAGE, "MMU_Finalise"));

	
	ppsMMUContext = (MMU_CONTEXT**)&psMMUContext->psDevInfo->pvMMUContextList;
	while(*ppsMMUContext)
	{
		__DtTestPoint(__DtFunc_MMU_Finalise, __DtStep_5)
		if(*ppsMMUContext == psMMUContext)
		{
			
			__DtTestPoint(__DtFunc_MMU_Finalise, __DtStep_6)
			*ppsMMUContext = psMMUContext->psNext;
			break;
		}

		
		ppsMMUContext = &((*ppsMMUContext)->psNext);
	}

	
	OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(MMU_CONTEXT), psMMUContext, IMG_NULL);
	
	__DtTestPoint(__DtFunc_MMU_Finalise, __DtStep_7)
}


IMG_VOID
MMU_InsertHeap(MMU_CONTEXT *psMMUContext, MMU_HEAP *psMMUHeap)
{
	IMG_UINT32 *pui32PDCpuVAddr = (IMG_UINT32 *) psMMUContext->pvPDCpuVAddr;
	IMG_UINT32 *pui32KernelPDCpuVAddr = (IMG_UINT32 *) psMMUHeap->psMMUContext->pvPDCpuVAddr;
	IMG_UINT32 ui32PDEntry;
#if !defined(SGX_FEATURE_MULTIPLE_MEM_CONTEXTS)
	IMG_BOOL bInvalidateDirectoryCache = IMG_FALSE;
#endif

	
	pui32PDCpuVAddr += psMMUHeap->psDevArena->BaseDevVAddr.uiAddr >> psMMUHeap->ui32PDShift;
	pui32KernelPDCpuVAddr += psMMUHeap->psDevArena->BaseDevVAddr.uiAddr >> psMMUHeap->ui32PDShift;

	


#if defined(PDUMP)
	PDUMPCOMMENT("Page directory shared heap range copy");
	PDUMPCOMMENT("  (Source heap MMU Context ID == %u, PT count == 0x%x)",
			psMMUHeap->psMMUContext->ui32PDumpMMUContextID,
			psMMUHeap->ui32PageTableCount);
	PDUMPCOMMENT("  (Destination MMU Context ID == %u)", psMMUContext->ui32PDumpMMUContextID);
#endif 
#ifdef SUPPORT_SGX_MMU_BYPASS
	EnableHostAccess(psMMUContext);
#endif

	for (ui32PDEntry = 0; ui32PDEntry < psMMUHeap->ui32PageTableCount; ui32PDEntry++)
	{
#if !defined(SUPPORT_SGX_MMU_DUMMY_PAGE)
		
		PVR_ASSERT(pui32PDCpuVAddr[ui32PDEntry] == 0);
#endif

		
		pui32PDCpuVAddr[ui32PDEntry] = pui32KernelPDCpuVAddr[ui32PDEntry];
		if (pui32PDCpuVAddr[ui32PDEntry])
		{
			
		#if defined(PDUMP)
			
		#if defined(SUPPORT_PDUMP_MULTI_PROCESS)
			if(psMMUContext->bPDumpActive)
		#endif 
			{
				PDUMPPDENTRIES(&psMMUHeap->sMMUAttrib, psMMUContext->hPDOSMemHandle, (IMG_VOID *) &pui32PDCpuVAddr[ui32PDEntry], sizeof(IMG_UINT32), 0, IMG_FALSE, PDUMP_PD_UNIQUETAG, PDUMP_PT_UNIQUETAG);
			}
		#endif
#if !defined(SGX_FEATURE_MULTIPLE_MEM_CONTEXTS)
			bInvalidateDirectoryCache = IMG_TRUE;
#endif
		}
	}

#ifdef SUPPORT_SGX_MMU_BYPASS
	DisableHostAccess(psMMUContext);
#endif

#if !defined(SGX_FEATURE_MULTIPLE_MEM_CONTEXTS)
	if (bInvalidateDirectoryCache)
	{
		



		__DtTestPoint(__DtFunc_MMU_InsertHeap, __DtStep_0)
		MMU_InvalidateDirectoryCache(psMMUContext->psDevInfo);
	}
#endif
	__DtTestPoint(__DtFunc_MMU_InsertHeap, __DtStep_1)
}


static IMG_VOID
MMU_UnmapPagesAndFreePTs (MMU_HEAP *psMMUHeap,
						  IMG_DEV_VIRTADDR sDevVAddr,
						  IMG_UINT32 ui32PageCount,
						  IMG_HANDLE hUniqueTag)
{
	IMG_DEV_VIRTADDR	sTmpDevVAddr;
	IMG_UINT32			i;
	IMG_UINT32			ui32PDIndex;
	IMG_UINT32			ui32PTIndex;
	IMG_UINT32			*pui32Tmp;
	IMG_BOOL			bInvalidateDirectoryCache = IMG_FALSE;

#if !defined (PDUMP)
	PVR_UNREFERENCED_PARAMETER(hUniqueTag);
#endif
	
	sTmpDevVAddr = sDevVAddr;

	for(i=0; i<ui32PageCount; i++)
	{
		MMU_PT_INFO **ppsPTInfoList;

		
		__DtTestPoint(__DtFunc_MMU_UnmapPagesAndFreePTs, __DtStep_0)
		ui32PDIndex = sTmpDevVAddr.uiAddr >> psMMUHeap->ui32PDShift;

		
		ppsPTInfoList = &psMMUHeap->psMMUContext->apsPTInfoList[ui32PDIndex];

		{
			
			ui32PTIndex = (sTmpDevVAddr.uiAddr & psMMUHeap->ui32PTMask) >> psMMUHeap->ui32PTShift;

			
			if (!ppsPTInfoList[0])
			{
				__DtTestPoint(__DtFunc_MMU_UnmapPagesAndFreePTs, __DtStep_1)
				PVR_DPF((PVR_DBG_MESSAGE, "MMU_UnmapPagesAndFreePTs: Invalid PT for alloc at VAddr:0x%08X (VaddrIni:0x%08X AllocPage:%u) PDIdx:%u PTIdx:%u",sTmpDevVAddr.uiAddr, sDevVAddr.uiAddr,i, ui32PDIndex, ui32PTIndex ));

				
				sTmpDevVAddr.uiAddr += psMMUHeap->ui32DataPageSize;

				
				continue;
			}

			
			pui32Tmp = (IMG_UINT32*)ppsPTInfoList[0]->PTPageCpuVAddr;

			
			if (!pui32Tmp)
			{
				__DtTestPoint(__DtFunc_MMU_UnmapPagesAndFreePTs, __DtStep_2)
				continue;
			}

			CheckPT(ppsPTInfoList[0]);

			
			if (pui32Tmp[ui32PTIndex] & SGX_MMU_PTE_VALID)
			{
				__DtTestPoint(__DtFunc_MMU_UnmapPagesAndFreePTs, __DtStep_3)
				ppsPTInfoList[0]->ui32ValidPTECount--;
			}
			else
			{
				__DtTestPoint(__DtFunc_MMU_UnmapPagesAndFreePTs, __DtStep_4)
				PVR_DPF((PVR_DBG_MESSAGE, "MMU_UnmapPagesAndFreePTs: Page is already invalid for alloc at VAddr:0x%08X (VAddrIni:0x%08X AllocPage:%u) PDIdx:%u PTIdx:%u",sTmpDevVAddr.uiAddr, sDevVAddr.uiAddr,i, ui32PDIndex, ui32PTIndex ));
			}

			
			PVR_ASSERT((IMG_INT32)ppsPTInfoList[0]->ui32ValidPTECount >= 0);

#if defined(SUPPORT_SGX_MMU_DUMMY_PAGE)
			
			pui32Tmp[ui32PTIndex] = (psMMUHeap->psMMUContext->psDevInfo->sDummyDataDevPAddr.uiAddr>>SGX_MMU_PTE_ADDR_ALIGNSHIFT)
									| SGX_MMU_PTE_VALID;
#else
			
			pui32Tmp[ui32PTIndex] = 0;
#endif

			CheckPT(ppsPTInfoList[0]);
		}

		

		if (ppsPTInfoList[0] && ppsPTInfoList[0]->ui32ValidPTECount == 0)
		{
			__DtTestPoint(__DtFunc_MMU_UnmapPagesAndFreePTs, __DtStep_5)
			_DeferredFreePageTable(psMMUHeap, ui32PDIndex - psMMUHeap->ui32PDBaseIndex, IMG_TRUE);
			bInvalidateDirectoryCache = IMG_TRUE;
		}

		
		sTmpDevVAddr.uiAddr += psMMUHeap->ui32DataPageSize;
	}

	if(bInvalidateDirectoryCache)
	{
		__DtTestPoint(__DtFunc_MMU_UnmapPagesAndFreePTs, __DtStep_6)
		MMU_InvalidateDirectoryCache(psMMUHeap->psMMUContext->psDevInfo);
	}
	else
	{
		__DtTestPoint(__DtFunc_MMU_UnmapPagesAndFreePTs, __DtStep_7)
		MMU_InvalidatePageTableCache(psMMUHeap->psMMUContext->psDevInfo);
	}

#if defined(PDUMP)
	MMU_PDumpPageTables(psMMUHeap,
						sDevVAddr,
						psMMUHeap->ui32DataPageSize * ui32PageCount,
						IMG_TRUE,
						hUniqueTag);
#endif 
	__DtTestPoint(__DtFunc_MMU_UnmapPagesAndFreePTs, __DtStep_8)
}


static IMG_VOID MMU_FreePageTables(IMG_PVOID pvMMUHeap,
                                   IMG_SIZE_T ui32Start,
                                   IMG_SIZE_T ui32End,
                                   IMG_HANDLE hUniqueTag)
{
	MMU_HEAP *pMMUHeap = (MMU_HEAP*)pvMMUHeap;
	IMG_DEV_VIRTADDR Start;

	__DtTestPoint(__DtFunc_MMU_FreePageTables, __DtStep_0)
	Start.uiAddr = ui32Start;

	MMU_UnmapPagesAndFreePTs(pMMUHeap, Start, (ui32End - ui32Start) >> pMMUHeap->ui32PTShift, hUniqueTag);
	__DtTestPoint(__DtFunc_MMU_FreePageTables, __DtStep_1)
}

MMU_HEAP *
MMU_Create (MMU_CONTEXT *psMMUContext,
			DEV_ARENA_DESCRIPTOR *psDevArena,
			RA_ARENA **ppsVMArena,
			PDUMP_MMU_ATTRIB **ppsMMUAttrib)
{
	MMU_HEAP *pMMUHeap;
	IMG_UINT32 ui32ScaleSize;

	__DtTestPoint(__DtFunc_MMU_Create, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(ppsMMUAttrib);

	PVR_ASSERT (psDevArena != IMG_NULL);

	if (psDevArena == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_MMU_Create, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "MMU_Create: invalid parameter"));
		__DtTestPoint(__DtFunc_MMU_Create, __DtStep_2)
		return IMG_NULL;
	}

	OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP,
				 sizeof (MMU_HEAP),
				 (IMG_VOID **)&pMMUHeap, IMG_NULL,
				 "MMU Heap");
	if (pMMUHeap == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_MMU_Create, __DtStep_3)
		PVR_DPF((PVR_DBG_ERROR, "MMU_Create: ERROR call to OSAllocMem failed"));
		__DtTestPoint(__DtFunc_MMU_Create, __DtStep_4)
		return IMG_NULL;
	}

	pMMUHeap->psMMUContext = psMMUContext;
	pMMUHeap->psDevArena = psDevArena;

	


	switch(pMMUHeap->psDevArena->ui32DataPageSize)
	{
		case 0x1000:
			__DtTestPoint(__DtFunc_MMU_Create, __DtStep_5)
			ui32ScaleSize = 0;
			pMMUHeap->ui32PDEPageSizeCtrl = SGX_MMU_PDE_PAGE_SIZE_4K;
			break;
#if defined(SGX_FEATURE_VARIABLE_MMU_PAGE_SIZE)
		case 0x4000:
			__DtTestPoint(__DtFunc_MMU_Create, __DtStep_6)
			ui32ScaleSize = 2;
			pMMUHeap->ui32PDEPageSizeCtrl = SGX_MMU_PDE_PAGE_SIZE_16K;
			break;
		case 0x10000:
			__DtTestPoint(__DtFunc_MMU_Create, __DtStep_7)
			ui32ScaleSize = 4;
			pMMUHeap->ui32PDEPageSizeCtrl = SGX_MMU_PDE_PAGE_SIZE_64K;
			break;
		case 0x40000:
			__DtTestPoint(__DtFunc_MMU_Create, __DtStep_8)
			ui32ScaleSize = 6;
			pMMUHeap->ui32PDEPageSizeCtrl = SGX_MMU_PDE_PAGE_SIZE_256K;
			break;
		case 0x100000:
			__DtTestPoint(__DtFunc_MMU_Create, __DtStep_9)
			ui32ScaleSize = 8;
			pMMUHeap->ui32PDEPageSizeCtrl = SGX_MMU_PDE_PAGE_SIZE_1M;
			break;
		case 0x400000:
			__DtTestPoint(__DtFunc_MMU_Create, __DtStep_10)
			ui32ScaleSize = 10;
			pMMUHeap->ui32PDEPageSizeCtrl = SGX_MMU_PDE_PAGE_SIZE_4M;
			break;
#endif 
		default:
			__DtTestPoint(__DtFunc_MMU_Create, __DtStep_11)
			PVR_DPF((PVR_DBG_ERROR, "MMU_Create: invalid data page size"));
			goto ErrorFreeHeap;
	}

	
	pMMUHeap->ui32DataPageSize = psDevArena->ui32DataPageSize;
	pMMUHeap->ui32DataPageBitWidth = SGX_MMU_PAGE_SHIFT + ui32ScaleSize;
	pMMUHeap->ui32DataPageMask = pMMUHeap->ui32DataPageSize - 1;
	
	pMMUHeap->ui32PTShift = pMMUHeap->ui32DataPageBitWidth;
	pMMUHeap->ui32PTBitWidth = SGX_MMU_PT_SHIFT - ui32ScaleSize;
	pMMUHeap->ui32PTMask = SGX_MMU_PT_MASK & (SGX_MMU_PT_MASK<<ui32ScaleSize);
	pMMUHeap->ui32PTSize = (IMG_UINT32)(1UL<<pMMUHeap->ui32PTBitWidth) * sizeof(IMG_UINT32);
	
	if(pMMUHeap->ui32PTSize < 4 * sizeof(IMG_UINT32))
	{
		__DtTestPoint(__DtFunc_MMU_Create, __DtStep_12)
		pMMUHeap->ui32PTSize = 4 * sizeof(IMG_UINT32);
	}
	pMMUHeap->ui32PTECount = pMMUHeap->ui32PTSize >> 2;

	
	pMMUHeap->ui32PDShift = pMMUHeap->ui32PTBitWidth + pMMUHeap->ui32PTShift;
	pMMUHeap->ui32PDBitWidth = SGX_FEATURE_ADDRESS_SPACE_SIZE - pMMUHeap->ui32PTBitWidth - pMMUHeap->ui32DataPageBitWidth;
	pMMUHeap->ui32PDMask = SGX_MMU_PD_MASK & (SGX_MMU_PD_MASK>>(32-SGX_FEATURE_ADDRESS_SPACE_SIZE));

	



	if(psDevArena->BaseDevVAddr.uiAddr > (pMMUHeap->ui32DataPageMask | pMMUHeap->ui32PTMask))
	{
		


		__DtTestPoint(__DtFunc_MMU_Create, __DtStep_13)
		PVR_ASSERT ((psDevArena->BaseDevVAddr.uiAddr
						& (pMMUHeap->ui32DataPageMask
							| pMMUHeap->ui32PTMask)) == 0);
	}

	
	pMMUHeap->ui32PTETotal = pMMUHeap->psDevArena->ui32Size >> pMMUHeap->ui32PTShift;

	
	pMMUHeap->ui32PDBaseIndex = (pMMUHeap->psDevArena->BaseDevVAddr.uiAddr & pMMUHeap->ui32PDMask) >> pMMUHeap->ui32PDShift;

	


	pMMUHeap->ui32PageTableCount = (pMMUHeap->ui32PTETotal + pMMUHeap->ui32PTECount - 1)
										>> pMMUHeap->ui32PTBitWidth;

	
	pMMUHeap->psVMArena = RA_Create(psDevArena->pszName,
									psDevArena->BaseDevVAddr.uiAddr,
									psDevArena->ui32Size,
									IMG_NULL,
									MAX(HOST_PAGESIZE(), pMMUHeap->ui32DataPageSize),
									IMG_NULL,
									IMG_NULL,
									&MMU_FreePageTables,
									pMMUHeap);

	if (pMMUHeap->psVMArena == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_MMU_Create, __DtStep_14)
		PVR_DPF((PVR_DBG_ERROR, "MMU_Create: ERROR call to RA_Create failed"));
		goto ErrorFreePagetables;
	}

#if defined(PDUMP)
	
	MMU_SetPDumpAttribs(&pMMUHeap->sMMUAttrib,
						psMMUContext->psDeviceNode,
						pMMUHeap->ui32DataPageMask,
						pMMUHeap->ui32PTSize);
	*ppsMMUAttrib = &pMMUHeap->sMMUAttrib;

	PDUMPCOMMENT("Create MMU device from arena %s (Size == 0x%x, DataPageSize == 0x%x, BaseDevVAddr == 0x%x)",
			psDevArena->pszName,
			psDevArena->ui32Size,
			pMMUHeap->ui32DataPageSize,
			psDevArena->BaseDevVAddr.uiAddr);
#endif 

#if 0 
	
	if(psDevArena->ui32HeapID == SGX_TILED_HEAP_ID)
	{
		IMG_UINT32 ui32RegVal;
		IMG_UINT32 ui32XTileStride;

		




		__DtTestPoint(__DtFunc_MMU_Create, __DtStep_15)
		ui32XTileStride	= 2;

		ui32RegVal = (EUR_CR_BIF_TILE0_MIN_ADDRESS_MASK
						& ((psDevArena->BaseDevVAddr.uiAddr>>20)
						<< EUR_CR_BIF_TILE0_MIN_ADDRESS_SHIFT))
					|(EUR_CR_BIF_TILE0_MAX_ADDRESS_MASK
						& (((psDevArena->BaseDevVAddr.uiAddr+psDevArena->ui32Size)>>20)
						<< EUR_CR_BIF_TILE0_MAX_ADDRESS_SHIFT))
					|(EUR_CR_BIF_TILE0_CFG_MASK
						& (((ui32XTileStride<<1)|8) << EUR_CR_BIF_TILE0_CFG_SHIFT));
		PDUMPREG(SGX_PDUMPREG_NAME, EUR_CR_BIF_TILE0, ui32RegVal);
	}
#endif

	

	*ppsVMArena = pMMUHeap->psVMArena;

	__DtTestPoint(__DtFunc_MMU_Create, __DtStep_16)
	return pMMUHeap;

	
ErrorFreePagetables:
	_DeferredFreePageTables (pMMUHeap);

ErrorFreeHeap:
	OSFreeMem (PVRSRV_OS_PAGEABLE_HEAP, sizeof(MMU_HEAP), pMMUHeap, IMG_NULL);
	

	__DtTestPoint(__DtFunc_MMU_Create, __DtStep_17)
	return IMG_NULL;
}

IMG_VOID
MMU_Delete (MMU_HEAP *pMMUHeap)
{
	__DtTestPoint(__DtFunc_MMU_Delete, __DtStep_0)
	if (pMMUHeap != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_MMU_Delete, __DtStep_1)
		PVR_DPF ((PVR_DBG_MESSAGE, "MMU_Delete"));

		if(pMMUHeap->psVMArena)
		{
			__DtTestPoint(__DtFunc_MMU_Delete, __DtStep_2)
			RA_Delete (pMMUHeap->psVMArena);
		}

#if defined(PDUMP)
		PDUMPCOMMENT("Delete MMU device from arena %s (BaseDevVAddr == 0x%x, PT count for deferred free == 0x%x)",
				pMMUHeap->psDevArena->pszName,
				pMMUHeap->psDevArena->BaseDevVAddr.uiAddr,
				pMMUHeap->ui32PageTableCount);
#endif 

#ifdef SUPPORT_SGX_MMU_BYPASS
		EnableHostAccess(pMMUHeap->psMMUContext);
#endif
		_DeferredFreePageTables (pMMUHeap);
#ifdef SUPPORT_SGX_MMU_BYPASS
		DisableHostAccess(pMMUHeap->psMMUContext);
#endif

		OSFreeMem (PVRSRV_OS_PAGEABLE_HEAP, sizeof(MMU_HEAP), pMMUHeap, IMG_NULL);
		
	}
	__DtTestPoint(__DtFunc_MMU_Delete, __DtStep_3)
}

IMG_BOOL
MMU_Alloc (MMU_HEAP *pMMUHeap,
		   IMG_SIZE_T uSize,
		   IMG_SIZE_T *pActualSize,
		   IMG_UINT32 uFlags,
		   IMG_UINT32 uDevVAddrAlignment,
		   IMG_DEV_VIRTADDR *psDevVAddr)
{
	IMG_BOOL bStatus;

	__DtTestPoint(__DtFunc_MMU_Alloc, __DtStep_0)
	PVR_DPF ((PVR_DBG_MESSAGE,
		"MMU_Alloc: uSize=0x%x, flags=0x%x, align=0x%x",
		uSize, uFlags, uDevVAddrAlignment));

	

	if((uFlags & PVRSRV_MEM_USER_SUPPLIED_DEVVADDR) == 0)
	{
		IMG_UINTPTR_T uiAddr;

		__DtTestPoint(__DtFunc_MMU_Alloc, __DtStep_1)
		bStatus = RA_Alloc (pMMUHeap->psVMArena,
							uSize,
							pActualSize,
							IMG_NULL,
							0,
							uDevVAddrAlignment,
							0,
							&uiAddr);
		if(!bStatus)
		{
			__DtTestPoint(__DtFunc_MMU_Alloc, __DtStep_2)
			PVR_DPF((PVR_DBG_ERROR,"MMU_Alloc: RA_Alloc of VMArena failed"));
			PVR_DPF((PVR_DBG_ERROR,"MMU_Alloc: Alloc of DevVAddr failed from heap %s ID%d",
									pMMUHeap->psDevArena->pszName,
									pMMUHeap->psDevArena->ui32HeapID));
			__DtTestPoint(__DtFunc_MMU_Alloc, __DtStep_3)
			return bStatus;
		}

		psDevVAddr->uiAddr = IMG_CAST_TO_DEVVADDR_UINT(uiAddr);
	}

	#ifdef SUPPORT_SGX_MMU_BYPASS
	EnableHostAccess(pMMUHeap->psMMUContext);
	#endif

	
	bStatus = _DeferredAllocPagetables(pMMUHeap, *psDevVAddr, uSize);

	#ifdef SUPPORT_SGX_MMU_BYPASS
	DisableHostAccess(pMMUHeap->psMMUContext);
	#endif

	if (!bStatus)
	{
		__DtTestPoint(__DtFunc_MMU_Alloc, __DtStep_4)
		PVR_DPF((PVR_DBG_ERROR,"MMU_Alloc: _DeferredAllocPagetables failed"));
		PVR_DPF((PVR_DBG_ERROR,"MMU_Alloc: Failed to alloc pagetable(s) for DevVAddr 0x%8.8x from heap %s ID%d",
								psDevVAddr->uiAddr,
								pMMUHeap->psDevArena->pszName,
								pMMUHeap->psDevArena->ui32HeapID));
		if((uFlags & PVRSRV_MEM_USER_SUPPLIED_DEVVADDR) == 0)
		{
			
			__DtTestPoint(__DtFunc_MMU_Alloc, __DtStep_5)
			RA_Free (pMMUHeap->psVMArena, psDevVAddr->uiAddr, IMG_FALSE);
		}
	}

	__DtTestPoint(__DtFunc_MMU_Alloc, __DtStep_6)
	return bStatus;
}

IMG_VOID
MMU_Free (MMU_HEAP *pMMUHeap, IMG_DEV_VIRTADDR DevVAddr, IMG_UINT32 ui32Size)
{
	__DtTestPoint(__DtFunc_MMU_Free, __DtStep_0)
	PVR_ASSERT (pMMUHeap != IMG_NULL);

	if (pMMUHeap == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_MMU_Free, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "MMU_Free: invalid parameter"));
		__DtTestPoint(__DtFunc_MMU_Free, __DtStep_2)
		return;
	}

	PVR_DPF((PVR_DBG_MESSAGE, "MMU_Free: Freeing DevVAddr 0x%08X from heap %s ID%d",
								DevVAddr.uiAddr,
								pMMUHeap->psDevArena->pszName,
								pMMUHeap->psDevArena->ui32HeapID));

	if((DevVAddr.uiAddr >= pMMUHeap->psDevArena->BaseDevVAddr.uiAddr) &&
		(DevVAddr.uiAddr + ui32Size <= pMMUHeap->psDevArena->BaseDevVAddr.uiAddr + pMMUHeap->psDevArena->ui32Size))
	{
		__DtTestPoint(__DtFunc_MMU_Free, __DtStep_3)
		RA_Free (pMMUHeap->psVMArena, DevVAddr.uiAddr, IMG_TRUE);
		__DtTestPoint(__DtFunc_MMU_Free, __DtStep_4)
		return;
	}

	PVR_DPF((PVR_DBG_ERROR,"MMU_Free: Couldn't free DevVAddr %08X from heap %s ID%d (not in range of heap))",
							DevVAddr.uiAddr,
							pMMUHeap->psDevArena->pszName,
							pMMUHeap->psDevArena->ui32HeapID));
	__DtTestPoint(__DtFunc_MMU_Free, __DtStep_5)
}

IMG_VOID
MMU_Enable (MMU_HEAP *pMMUHeap)
{
	__DtTestPoint(__DtFunc_MMU_Enable, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(pMMUHeap);
	
	__DtTestPoint(__DtFunc_MMU_Enable, __DtStep_1)
}

IMG_VOID
MMU_Disable (MMU_HEAP *pMMUHeap)
{
	__DtTestPoint(__DtFunc_MMU_Disable, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(pMMUHeap);
	
	__DtTestPoint(__DtFunc_MMU_Disable, __DtStep_1)
}

#if defined(PDUMP)
static IMG_VOID
MMU_PDumpPageTables	(MMU_HEAP *pMMUHeap,
					 IMG_DEV_VIRTADDR DevVAddr,
					 IMG_SIZE_T uSize,
					 IMG_BOOL bForUnmap,
					 IMG_HANDLE hUniqueTag)
{
	IMG_UINT32	ui32NumPTEntries;
	IMG_UINT32	ui32PTIndex;
	IMG_UINT32	*pui32PTEntry;

	MMU_PT_INFO **ppsPTInfoList;
	IMG_UINT32 ui32PDIndex;
	IMG_UINT32 ui32PTDumpCount;

	
	__DtTestPoint(__DtFunc_MMU_PDumpPageTables, __DtStep_0)
	ui32NumPTEntries = (uSize + pMMUHeap->ui32DataPageMask) >> pMMUHeap->ui32PTShift;

	
	ui32PDIndex = DevVAddr.uiAddr >> pMMUHeap->ui32PDShift;

	
	ppsPTInfoList = &pMMUHeap->psMMUContext->apsPTInfoList[ui32PDIndex];

	
	ui32PTIndex = (DevVAddr.uiAddr & pMMUHeap->ui32PTMask) >> pMMUHeap->ui32PTShift;

	
	PDUMPCOMMENT("Page table mods (num entries == %08X) %s", ui32NumPTEntries, bForUnmap ? "(for unmap)" : "");

	
	while(ui32NumPTEntries > 0)
	{
		MMU_PT_INFO* psPTInfo = *ppsPTInfoList++;

		__DtTestPoint(__DtFunc_MMU_PDumpPageTables, __DtStep_1)
		if(ui32NumPTEntries <= pMMUHeap->ui32PTECount - ui32PTIndex)
		{
			__DtTestPoint(__DtFunc_MMU_PDumpPageTables, __DtStep_2)
			ui32PTDumpCount = ui32NumPTEntries;
		}
		else
		{
			__DtTestPoint(__DtFunc_MMU_PDumpPageTables, __DtStep_3)
			ui32PTDumpCount = pMMUHeap->ui32PTECount - ui32PTIndex;
		}

		if (psPTInfo)
		{
			IMG_UINT32 ui32Flags = 0;
#if defined(SUPPORT_PDUMP_MULTI_PROCESS)
			ui32Flags |= ( MMU_IsHeapShared(pMMUHeap) ) ? PDUMP_FLAGS_PERSISTENT : 0;
#endif
			pui32PTEntry = (IMG_UINT32*)psPTInfo->PTPageCpuVAddr;
			PDUMPMEMPTENTRIES(&pMMUHeap->sMMUAttrib, psPTInfo->hPTPageOSMemHandle, (IMG_VOID *) &pui32PTEntry[ui32PTIndex], ui32PTDumpCount * sizeof(IMG_UINT32), ui32Flags, IMG_FALSE, PDUMP_PT_UNIQUETAG, hUniqueTag);
		}

		
		ui32NumPTEntries -= ui32PTDumpCount;

		
		ui32PTIndex = 0;
	}

	PDUMPCOMMENT("Finished page table mods %s", bForUnmap ? "(for unmap)" : "");
	__DtTestPoint(__DtFunc_MMU_PDumpPageTables, __DtStep_4)
}
#endif 


static IMG_VOID
MMU_MapPage (MMU_HEAP *pMMUHeap,
			 IMG_DEV_VIRTADDR DevVAddr,
			 IMG_DEV_PHYADDR DevPAddr,
			 IMG_UINT32 ui32MemFlags)
{
	IMG_UINT32 ui32Index;
	IMG_UINT32 *pui32Tmp;
	IMG_UINT32 ui32MMUFlags = 0;
	MMU_PT_INFO **ppsPTInfoList;

	
	__DtTestPoint(__DtFunc_MMU_MapPage, __DtStep_0)
	PVR_ASSERT((DevPAddr.uiAddr & pMMUHeap->ui32DataPageMask) == 0);

	

	if(((PVRSRV_MEM_READ|PVRSRV_MEM_WRITE) & ui32MemFlags) == (PVRSRV_MEM_READ|PVRSRV_MEM_WRITE))
	{
		
		__DtTestPoint(__DtFunc_MMU_MapPage, __DtStep_1)
		ui32MMUFlags = 0;
	}
	else if(PVRSRV_MEM_READ & ui32MemFlags)
	{
		
		__DtTestPoint(__DtFunc_MMU_MapPage, __DtStep_2)
		ui32MMUFlags |= SGX_MMU_PTE_READONLY;
	}
	else if(PVRSRV_MEM_WRITE & ui32MemFlags)
	{
		
		__DtTestPoint(__DtFunc_MMU_MapPage, __DtStep_3)
		ui32MMUFlags |= SGX_MMU_PTE_WRITEONLY;
	}

	
	if(PVRSRV_MEM_CACHE_CONSISTENT & ui32MemFlags)
	{
		__DtTestPoint(__DtFunc_MMU_MapPage, __DtStep_4)
		ui32MMUFlags |= SGX_MMU_PTE_CACHECONSISTENT;
	}

#if !defined(FIX_HW_BRN_25503)
	
	if(PVRSRV_MEM_EDM_PROTECT & ui32MemFlags)
	{
		__DtTestPoint(__DtFunc_MMU_MapPage, __DtStep_5)
		ui32MMUFlags |= SGX_MMU_PTE_EDMPROTECT;
	}
#endif

	


	
	ui32Index = DevVAddr.uiAddr >> pMMUHeap->ui32PDShift;

	
	ppsPTInfoList = &pMMUHeap->psMMUContext->apsPTInfoList[ui32Index];

	CheckPT(ppsPTInfoList[0]);

	
	ui32Index = (DevVAddr.uiAddr & pMMUHeap->ui32PTMask) >> pMMUHeap->ui32PTShift;

	
	pui32Tmp = (IMG_UINT32*)ppsPTInfoList[0]->PTPageCpuVAddr;

#if !defined(SUPPORT_SGX_MMU_DUMMY_PAGE)
	{
		IMG_UINT32 uTmp = pui32Tmp[ui32Index];
		
		
		if (uTmp & SGX_MMU_PTE_VALID)
		{
			__DtTestPoint(__DtFunc_MMU_MapPage, __DtStep_6)
			PVR_DPF((PVR_DBG_ERROR, "MMU_MapPage: Page is already valid for alloc at VAddr:0x%08X PDIdx:%u PTIdx:%u",
									DevVAddr.uiAddr,
									DevVAddr.uiAddr >> pMMUHeap->ui32PDShift,
									ui32Index ));
			PVR_DPF((PVR_DBG_ERROR, "MMU_MapPage: Page table entry value: 0x%08X", uTmp));
			PVR_DPF((PVR_DBG_ERROR, "MMU_MapPage: Physical page to map: 0x%08X", DevPAddr.uiAddr));
		}
		
		PVR_ASSERT((uTmp & SGX_MMU_PTE_VALID) == 0);
	}
#endif

	
	ppsPTInfoList[0]->ui32ValidPTECount++;

	
	pui32Tmp[ui32Index] = ((DevPAddr.uiAddr>>SGX_MMU_PTE_ADDR_ALIGNSHIFT)
						& ((~pMMUHeap->ui32DataPageMask)>>SGX_MMU_PTE_ADDR_ALIGNSHIFT))
						| SGX_MMU_PTE_VALID
						| ui32MMUFlags;

	CheckPT(ppsPTInfoList[0]);
	__DtTestPoint(__DtFunc_MMU_MapPage, __DtStep_7)
}


IMG_VOID
MMU_MapScatter (MMU_HEAP *pMMUHeap,
				IMG_DEV_VIRTADDR DevVAddr,
				IMG_SYS_PHYADDR *psSysAddr,
				IMG_SIZE_T uSize,
				IMG_UINT32 ui32MemFlags,
				IMG_HANDLE hUniqueTag)
{
#if defined(PDUMP)
	IMG_DEV_VIRTADDR MapBaseDevVAddr;
#endif 
	IMG_UINT32 uCount, i;
	IMG_DEV_PHYADDR DevPAddr;

	PVR_ASSERT (pMMUHeap != IMG_NULL);

#if defined(PDUMP)
	MapBaseDevVAddr = DevVAddr;
#else
	PVR_UNREFERENCED_PARAMETER(hUniqueTag);
#endif 

	for (i=0, uCount=0; uCount<uSize; i++, uCount+=pMMUHeap->ui32DataPageSize)
	{
		IMG_SYS_PHYADDR sSysAddr;

		__DtTestPoint(__DtFunc_MMU_MapScatter, __DtStep_0)
		sSysAddr = psSysAddr[i];


		
		PVR_ASSERT((sSysAddr.uiAddr & pMMUHeap->ui32DataPageMask) == 0);

		DevPAddr = SysSysPAddrToDevPAddr(PVRSRV_DEVICE_TYPE_SGX, sSysAddr);

		MMU_MapPage (pMMUHeap, DevVAddr, DevPAddr, ui32MemFlags);
		DevVAddr.uiAddr += pMMUHeap->ui32DataPageSize;

		PVR_DPF ((PVR_DBG_MESSAGE,
				 "MMU_MapScatter: devVAddr=%08X, SysAddr=%08X, size=0x%x/0x%x",
				  DevVAddr.uiAddr, sSysAddr.uiAddr, uCount, uSize));
	}

#if defined(PDUMP)
	MMU_PDumpPageTables (pMMUHeap, MapBaseDevVAddr, uSize, IMG_FALSE, hUniqueTag);
#endif 
	__DtTestPoint(__DtFunc_MMU_MapScatter, __DtStep_1)
}

IMG_VOID
MMU_MapPages (MMU_HEAP *pMMUHeap,
			  IMG_DEV_VIRTADDR DevVAddr,
			  IMG_SYS_PHYADDR SysPAddr,
			  IMG_SIZE_T uSize,
			  IMG_UINT32 ui32MemFlags,
			  IMG_HANDLE hUniqueTag)
{
	IMG_DEV_PHYADDR DevPAddr;
#if defined(PDUMP)
	IMG_DEV_VIRTADDR MapBaseDevVAddr;
#endif 
	IMG_UINT32 uCount;
	IMG_UINT32 ui32VAdvance;
	IMG_UINT32 ui32PAdvance;

	PVR_ASSERT (pMMUHeap != IMG_NULL);

	PVR_DPF ((PVR_DBG_MESSAGE, "MMU_MapPages: heap:%s, heap_id:%d devVAddr=%08X, SysPAddr=%08X, size=0x%x",
								pMMUHeap->psDevArena->pszName,
								pMMUHeap->psDevArena->ui32HeapID,
								DevVAddr.uiAddr, 
								SysPAddr.uiAddr,
								uSize));

	
	ui32VAdvance = pMMUHeap->ui32DataPageSize;
	ui32PAdvance = pMMUHeap->ui32DataPageSize;

#if defined(PDUMP)
	MapBaseDevVAddr = DevVAddr;
#else
	PVR_UNREFERENCED_PARAMETER(hUniqueTag);
#endif 

	DevPAddr = SysSysPAddrToDevPAddr(PVRSRV_DEVICE_TYPE_SGX, SysPAddr);

	
	PVR_ASSERT((DevPAddr.uiAddr & pMMUHeap->ui32DataPageMask) == 0);

#if defined(FIX_HW_BRN_23281)
	if(ui32MemFlags & PVRSRV_MEM_INTERLEAVED)
	{
		__DtTestPoint(__DtFunc_MMU_MapPages, __DtStep_0)
		ui32VAdvance *= 2;
	}
#endif

	


	if(ui32MemFlags & PVRSRV_MEM_DUMMY)
	{
		__DtTestPoint(__DtFunc_MMU_MapPages, __DtStep_1)
		ui32PAdvance = 0;
	}

	for (uCount=0; uCount<uSize; uCount+=ui32VAdvance)
	{
		__DtTestPoint(__DtFunc_MMU_MapPages, __DtStep_2)
		MMU_MapPage (pMMUHeap, DevVAddr, DevPAddr, ui32MemFlags);
		DevVAddr.uiAddr += ui32VAdvance;
		DevPAddr.uiAddr += ui32PAdvance;
	}

#if defined(PDUMP)
	MMU_PDumpPageTables (pMMUHeap, MapBaseDevVAddr, uSize, IMG_FALSE, hUniqueTag);
#endif 
	__DtTestPoint(__DtFunc_MMU_MapPages, __DtStep_3)
}

IMG_VOID
MMU_MapShadow (MMU_HEAP          *pMMUHeap,
			   IMG_DEV_VIRTADDR   MapBaseDevVAddr,
			   IMG_SIZE_T         uByteSize,
			   IMG_CPU_VIRTADDR   CpuVAddr,
			   IMG_HANDLE         hOSMemHandle,
			   IMG_DEV_VIRTADDR  *pDevVAddr,
			   IMG_UINT32         ui32MemFlags,
			   IMG_HANDLE         hUniqueTag)
{
	IMG_UINT32			i;
	IMG_UINT32			uOffset = 0;
	IMG_DEV_VIRTADDR	MapDevVAddr;
	IMG_UINT32			ui32VAdvance;
	IMG_UINT32			ui32PAdvance;

#if !defined (PDUMP)
	PVR_UNREFERENCED_PARAMETER(hUniqueTag);
#endif

	PVR_DPF ((PVR_DBG_MESSAGE,
			"MMU_MapShadow: DevVAddr:%08X, Bytes:0x%x, CPUVAddr:%08X",
			MapBaseDevVAddr.uiAddr,
			uByteSize,
			(IMG_UINTPTR_T)CpuVAddr));

	
	ui32VAdvance = pMMUHeap->ui32DataPageSize;
	ui32PAdvance = pMMUHeap->ui32DataPageSize;

	
	PVR_ASSERT(((IMG_UINTPTR_T)CpuVAddr & (SGX_MMU_PAGE_SIZE - 1)) == 0);
	PVR_ASSERT(((IMG_UINT32)uByteSize & pMMUHeap->ui32DataPageMask) == 0);
	pDevVAddr->uiAddr = MapBaseDevVAddr.uiAddr;

#if defined(FIX_HW_BRN_23281)
	if(ui32MemFlags & PVRSRV_MEM_INTERLEAVED)
	{
		__DtTestPoint(__DtFunc_MMU_MapShadow, __DtStep_0)
		ui32VAdvance *= 2;
	}
#endif

	


	if(ui32MemFlags & PVRSRV_MEM_DUMMY)
	{
		__DtTestPoint(__DtFunc_MMU_MapShadow, __DtStep_1)
		ui32PAdvance = 0;
	}

	
	MapDevVAddr = MapBaseDevVAddr;
	for (i=0; i<uByteSize; i+=ui32VAdvance)
	{
		IMG_CPU_PHYADDR CpuPAddr;
		IMG_DEV_PHYADDR DevPAddr;

		__DtTestPoint(__DtFunc_MMU_MapShadow, __DtStep_2)
		if(CpuVAddr)
		{
			__DtTestPoint(__DtFunc_MMU_MapShadow, __DtStep_3)
			CpuPAddr = OSMapLinToCPUPhys (hOSMemHandle,
										  (IMG_VOID *)((IMG_UINTPTR_T)CpuVAddr + uOffset));
		}
		else
		{
			__DtTestPoint(__DtFunc_MMU_MapShadow, __DtStep_4)
			CpuPAddr = OSMemHandleToCpuPAddr(hOSMemHandle, uOffset);
		}
		DevPAddr = SysCpuPAddrToDevPAddr (PVRSRV_DEVICE_TYPE_SGX, CpuPAddr);

		
		PVR_ASSERT((DevPAddr.uiAddr & pMMUHeap->ui32DataPageMask) == 0);

		PVR_DPF ((PVR_DBG_MESSAGE,
				"Offset=0x%x: CpuVAddr=%08X, CpuPAddr=%08X, DevVAddr=%08X, DevPAddr=%08X",
				uOffset,
				(IMG_UINTPTR_T)CpuVAddr + uOffset,
				CpuPAddr.uiAddr,
				MapDevVAddr.uiAddr,
				DevPAddr.uiAddr));

		MMU_MapPage (pMMUHeap, MapDevVAddr, DevPAddr, ui32MemFlags);

		
		MapDevVAddr.uiAddr += ui32VAdvance;
		uOffset += ui32PAdvance;
	}

#if defined(PDUMP)
	MMU_PDumpPageTables (pMMUHeap, MapBaseDevVAddr, uByteSize, IMG_FALSE, hUniqueTag);
#endif 
	__DtTestPoint(__DtFunc_MMU_MapShadow, __DtStep_5)
}


IMG_VOID
MMU_UnmapPages (MMU_HEAP *psMMUHeap,
				IMG_DEV_VIRTADDR sDevVAddr,
				IMG_UINT32 ui32PageCount,
				IMG_HANDLE hUniqueTag)
{
	IMG_UINT32			uPageSize = psMMUHeap->ui32DataPageSize;
	IMG_DEV_VIRTADDR	sTmpDevVAddr;
	IMG_UINT32			i;
	IMG_UINT32			ui32PDIndex;
	IMG_UINT32			ui32PTIndex;
	IMG_UINT32			*pui32Tmp;

#if !defined (PDUMP)
	PVR_UNREFERENCED_PARAMETER(hUniqueTag);
#endif

	
	sTmpDevVAddr = sDevVAddr;

	for(i=0; i<ui32PageCount; i++)
	{
		MMU_PT_INFO **ppsPTInfoList;

		
		__DtTestPoint(__DtFunc_MMU_UnmapPages, __DtStep_0)
		ui32PDIndex = sTmpDevVAddr.uiAddr >> psMMUHeap->ui32PDShift;

		
		ppsPTInfoList = &psMMUHeap->psMMUContext->apsPTInfoList[ui32PDIndex];

		
		ui32PTIndex = (sTmpDevVAddr.uiAddr & psMMUHeap->ui32PTMask) >> psMMUHeap->ui32PTShift;

		
		if (!ppsPTInfoList[0])
		{
			__DtTestPoint(__DtFunc_MMU_UnmapPages, __DtStep_1)
			PVR_DPF((PVR_DBG_ERROR, "MMU_UnmapPages: ERROR Invalid PT for alloc at VAddr:0x%08X (VaddrIni:0x%08X AllocPage:%u) PDIdx:%u PTIdx:%u",
									sTmpDevVAddr.uiAddr,
									sDevVAddr.uiAddr,
									i,
									ui32PDIndex,
									ui32PTIndex));

			
			sTmpDevVAddr.uiAddr += uPageSize;

			
			continue;
		}

		CheckPT(ppsPTInfoList[0]);

		
		pui32Tmp = (IMG_UINT32*)ppsPTInfoList[0]->PTPageCpuVAddr;

		
		if (pui32Tmp[ui32PTIndex] & SGX_MMU_PTE_VALID)
		{
			__DtTestPoint(__DtFunc_MMU_UnmapPages, __DtStep_2)
			ppsPTInfoList[0]->ui32ValidPTECount--;
		}
		else
		{
			__DtTestPoint(__DtFunc_MMU_UnmapPages, __DtStep_3)
			PVR_DPF((PVR_DBG_ERROR, "MMU_UnmapPages: Page is already invalid for alloc at VAddr:0x%08X (VAddrIni:0x%08X AllocPage:%u) PDIdx:%u PTIdx:%u",
									sTmpDevVAddr.uiAddr,
									sDevVAddr.uiAddr,
									i,
									ui32PDIndex,
									ui32PTIndex));
			PVR_DPF((PVR_DBG_ERROR, "MMU_UnmapPages: Page table entry value: 0x%08X", pui32Tmp[ui32PTIndex]));
		}

		
		PVR_ASSERT((IMG_INT32)ppsPTInfoList[0]->ui32ValidPTECount >= 0);

#if defined(SUPPORT_SGX_MMU_DUMMY_PAGE)
		
		pui32Tmp[ui32PTIndex] = (psMMUHeap->psMMUContext->psDevInfo->sDummyDataDevPAddr.uiAddr>>SGX_MMU_PTE_ADDR_ALIGNSHIFT)
								| SGX_MMU_PTE_VALID;
#else
		
		pui32Tmp[ui32PTIndex] = 0;
#endif

		CheckPT(ppsPTInfoList[0]);

		
		sTmpDevVAddr.uiAddr += uPageSize;
	}

	MMU_InvalidatePageTableCache(psMMUHeap->psMMUContext->psDevInfo);

#if defined(PDUMP)
	MMU_PDumpPageTables (psMMUHeap, sDevVAddr, uPageSize*ui32PageCount, IMG_TRUE, hUniqueTag);
#endif 
	__DtTestPoint(__DtFunc_MMU_UnmapPages, __DtStep_4)
}


IMG_DEV_PHYADDR
MMU_GetPhysPageAddr(MMU_HEAP *pMMUHeap, IMG_DEV_VIRTADDR sDevVPageAddr)
{
	IMG_UINT32 *pui32PageTable;
	IMG_UINT32 ui32Index;
	IMG_DEV_PHYADDR sDevPAddr;
	MMU_PT_INFO **ppsPTInfoList;

	
	__DtTestPoint(__DtFunc_MMU_GetPhysPageAddr, __DtStep_0)
	ui32Index = sDevVPageAddr.uiAddr >> pMMUHeap->ui32PDShift;

	
	ppsPTInfoList = &pMMUHeap->psMMUContext->apsPTInfoList[ui32Index];
	if (!ppsPTInfoList[0])
	{
		__DtTestPoint(__DtFunc_MMU_GetPhysPageAddr, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR,"MMU_GetPhysPageAddr: Not mapped in at 0x%08x", sDevVPageAddr.uiAddr));
		sDevPAddr.uiAddr = 0;
		__DtTestPoint(__DtFunc_MMU_GetPhysPageAddr, __DtStep_2)
		return sDevPAddr;
	}

	
	ui32Index = (sDevVPageAddr.uiAddr & pMMUHeap->ui32PTMask) >> pMMUHeap->ui32PTShift;

	
	pui32PageTable = (IMG_UINT32*)ppsPTInfoList[0]->PTPageCpuVAddr;

	
	sDevPAddr.uiAddr = pui32PageTable[ui32Index];

	
	sDevPAddr.uiAddr &= ~(pMMUHeap->ui32DataPageMask>>SGX_MMU_PTE_ADDR_ALIGNSHIFT);

	
	sDevPAddr.uiAddr <<= SGX_MMU_PTE_ADDR_ALIGNSHIFT;

	__DtTestPoint(__DtFunc_MMU_GetPhysPageAddr, __DtStep_3)
	return sDevPAddr;
}


IMG_DEV_PHYADDR MMU_GetPDDevPAddr(MMU_CONTEXT *pMMUContext)
{
	__DtTestPoint(__DtFunc_MMU_GetPDDevPAddr, __DtStep_0)
	return (pMMUContext->sPDDevPAddr);
}


IMG_EXPORT
PVRSRV_ERROR SGXGetPhysPageAddrKM (IMG_HANDLE hDevMemHeap,
								   IMG_DEV_VIRTADDR sDevVAddr,
								   IMG_DEV_PHYADDR *pDevPAddr,
								   IMG_CPU_PHYADDR *pCpuPAddr)
{
	MMU_HEAP *pMMUHeap;
	IMG_DEV_PHYADDR DevPAddr;

	

	__DtTestPoint(__DtFunc_SGXGetPhysPageAddrKM, __DtStep_0)
	pMMUHeap = (MMU_HEAP*)BM_GetMMUHeap(hDevMemHeap);

	DevPAddr = MMU_GetPhysPageAddr(pMMUHeap, sDevVAddr);
	pCpuPAddr->uiAddr = DevPAddr.uiAddr; 
	pDevPAddr->uiAddr = DevPAddr.uiAddr;

	__DtTestPoint(__DtFunc_SGXGetPhysPageAddrKM, __DtStep_1)
	return (pDevPAddr->uiAddr != 0) ? PVRSRV_OK : PVRSRV_ERROR_INVALID_PARAMS;
}


PVRSRV_ERROR SGXGetMMUPDAddrKM(IMG_HANDLE		hDevCookie,
								IMG_HANDLE 		hDevMemContext,
								IMG_DEV_PHYADDR *psPDDevPAddr)
{
	__DtTestPoint(__DtFunc_SGXGetMMUPDAddrKM, __DtStep_0)
	if (!hDevCookie || !hDevMemContext || !psPDDevPAddr)
	{
		__DtTestPoint(__DtFunc_SGXGetMMUPDAddrKM, __DtStep_1)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	
	*psPDDevPAddr = ((BM_CONTEXT*)hDevMemContext)->psMMUContext->sPDDevPAddr;

	__DtTestPoint(__DtFunc_SGXGetMMUPDAddrKM, __DtStep_2)
	return PVRSRV_OK;
}

PVRSRV_ERROR MMU_BIFResetPDAlloc(PVRSRV_SGXDEV_INFO *psDevInfo)
{
	PVRSRV_ERROR eError;
	SYS_DATA *psSysData;
	RA_ARENA *psLocalDevMemArena;
	IMG_HANDLE hOSMemHandle = IMG_NULL;
	IMG_BYTE *pui8MemBlock = IMG_NULL;
	IMG_SYS_PHYADDR sMemBlockSysPAddr;
	IMG_CPU_PHYADDR sMemBlockCpuPAddr;

	__DtTestPoint(__DtFunc_MMU_BIFResetPDAlloc, __DtStep_0)
	SysAcquireData(&psSysData);

	psLocalDevMemArena = psSysData->apsLocalDevMemArena[0];

	
	if(psLocalDevMemArena == IMG_NULL)
	{
		
		__DtTestPoint(__DtFunc_MMU_BIFResetPDAlloc, __DtStep_1)
		eError = OSAllocPages(PVRSRV_HAP_WRITECOMBINE | PVRSRV_HAP_KERNEL_ONLY,
						      3 * SGX_MMU_PAGE_SIZE,
						      SGX_MMU_PAGE_SIZE,
						      (IMG_VOID **)&pui8MemBlock,
						      &hOSMemHandle);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_MMU_BIFResetPDAlloc, __DtStep_2)
			PVR_DPF((PVR_DBG_ERROR, "MMU_BIFResetPDAlloc: ERROR call to OSAllocPages failed"));
			__DtTestPoint(__DtFunc_MMU_BIFResetPDAlloc, __DtStep_3)
			return eError;
		}

		
		if(pui8MemBlock)
		{
			__DtTestPoint(__DtFunc_MMU_BIFResetPDAlloc, __DtStep_4)
			sMemBlockCpuPAddr = OSMapLinToCPUPhys(hOSMemHandle,
												  pui8MemBlock);
		}
		else
		{
			
			__DtTestPoint(__DtFunc_MMU_BIFResetPDAlloc, __DtStep_5)
			sMemBlockCpuPAddr = OSMemHandleToCpuPAddr(hOSMemHandle, 0);
		}
	}
	else
	{
		

		__DtTestPoint(__DtFunc_MMU_BIFResetPDAlloc, __DtStep_6)
		if(RA_Alloc(psLocalDevMemArena,
					3 * SGX_MMU_PAGE_SIZE,
					IMG_NULL,
					IMG_NULL,
					0,
					SGX_MMU_PAGE_SIZE,
					0,
					&(sMemBlockSysPAddr.uiAddr)) != IMG_TRUE)
		{
			__DtTestPoint(__DtFunc_MMU_BIFResetPDAlloc, __DtStep_7)
			PVR_DPF((PVR_DBG_ERROR, "MMU_BIFResetPDAlloc: ERROR call to RA_Alloc failed"));
			__DtTestPoint(__DtFunc_MMU_BIFResetPDAlloc, __DtStep_8)
			return PVRSRV_ERROR_OUT_OF_MEMORY;
		}

		
		sMemBlockCpuPAddr = SysSysPAddrToCpuPAddr(sMemBlockSysPAddr);
		pui8MemBlock = OSMapPhysToLin(sMemBlockCpuPAddr,
									  SGX_MMU_PAGE_SIZE * 3,
									  PVRSRV_HAP_WRITECOMBINE|PVRSRV_HAP_KERNEL_ONLY,
									  &hOSMemHandle);
		if(!pui8MemBlock)
		{
			__DtTestPoint(__DtFunc_MMU_BIFResetPDAlloc, __DtStep_9)
			PVR_DPF((PVR_DBG_ERROR, "MMU_BIFResetPDAlloc: ERROR failed to map page tables"));
			__DtTestPoint(__DtFunc_MMU_BIFResetPDAlloc, __DtStep_10)
			return PVRSRV_ERROR_BAD_MAPPING;
		}
	}

	psDevInfo->hBIFResetPDOSMemHandle = hOSMemHandle;
	psDevInfo->sBIFResetPDDevPAddr = SysCpuPAddrToDevPAddr(PVRSRV_DEVICE_TYPE_SGX, sMemBlockCpuPAddr);
	psDevInfo->sBIFResetPTDevPAddr.uiAddr = psDevInfo->sBIFResetPDDevPAddr.uiAddr + SGX_MMU_PAGE_SIZE;
	psDevInfo->sBIFResetPageDevPAddr.uiAddr = psDevInfo->sBIFResetPTDevPAddr.uiAddr + SGX_MMU_PAGE_SIZE;
	
	
	psDevInfo->pui32BIFResetPD = (IMG_UINT32 *)pui8MemBlock;
	psDevInfo->pui32BIFResetPT = (IMG_UINT32 *)(pui8MemBlock + SGX_MMU_PAGE_SIZE);

	
	OSMemSet(psDevInfo->pui32BIFResetPD, 0, SGX_MMU_PAGE_SIZE);
	OSMemSet(psDevInfo->pui32BIFResetPT, 0, SGX_MMU_PAGE_SIZE);
	
	OSMemSet(pui8MemBlock + (2 * SGX_MMU_PAGE_SIZE), 0xDB, SGX_MMU_PAGE_SIZE);

	__DtTestPoint(__DtFunc_MMU_BIFResetPDAlloc, __DtStep_11)
	return PVRSRV_OK;
}

IMG_VOID MMU_BIFResetPDFree(PVRSRV_SGXDEV_INFO *psDevInfo)
{
	SYS_DATA *psSysData;
	RA_ARENA *psLocalDevMemArena;
	IMG_SYS_PHYADDR sPDSysPAddr;

	__DtTestPoint(__DtFunc_MMU_BIFResetPDFree, __DtStep_0)
	SysAcquireData(&psSysData);

	psLocalDevMemArena = psSysData->apsLocalDevMemArena[0];

	
	if(psLocalDevMemArena == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_MMU_BIFResetPDFree, __DtStep_1)
		OSFreePages(PVRSRV_HAP_WRITECOMBINE | PVRSRV_HAP_KERNEL_ONLY,
					3 * SGX_MMU_PAGE_SIZE,
					psDevInfo->pui32BIFResetPD,
					psDevInfo->hBIFResetPDOSMemHandle);
	}
	else
	{
		__DtTestPoint(__DtFunc_MMU_BIFResetPDFree, __DtStep_2)
		OSUnMapPhysToLin(psDevInfo->pui32BIFResetPD,
                         3 * SGX_MMU_PAGE_SIZE,
                         PVRSRV_HAP_WRITECOMBINE|PVRSRV_HAP_KERNEL_ONLY,
                         psDevInfo->hBIFResetPDOSMemHandle);

		sPDSysPAddr = SysDevPAddrToSysPAddr(PVRSRV_DEVICE_TYPE_SGX, psDevInfo->sBIFResetPDDevPAddr);
		RA_Free(psLocalDevMemArena, sPDSysPAddr.uiAddr, IMG_FALSE);
	}
	__DtTestPoint(__DtFunc_MMU_BIFResetPDFree, __DtStep_3)
}


#if defined(FIX_HW_BRN_22997) && defined(FIX_HW_BRN_23030) && defined(SGX_FEATURE_HOST_PORT)
PVRSRV_ERROR WorkaroundBRN22997Alloc(PVRSRV_DEVICE_NODE	*psDeviceNode)
{
	PVRSRV_ERROR eError;
	SYS_DATA *psSysData;
	RA_ARENA *psLocalDevMemArena;
	IMG_HANDLE hPTPageOSMemHandle = IMG_NULL;
	IMG_HANDLE hPDPageOSMemHandle = IMG_NULL;
	IMG_UINT32 *pui32PD = IMG_NULL;
	IMG_UINT32 *pui32PT = IMG_NULL;
	IMG_CPU_PHYADDR sCpuPAddr;
	IMG_DEV_PHYADDR sPTDevPAddr;
	IMG_DEV_PHYADDR sPDDevPAddr;
	PVRSRV_SGXDEV_INFO *psDevInfo;
	IMG_UINT32 ui32PDOffset;
	IMG_UINT32 ui32PTOffset;

	__DtTestPoint(__DtFunc_WorkaroundBRN22997Alloc, __DtStep_0)
	psDevInfo = (PVRSRV_SGXDEV_INFO *)psDeviceNode->pvDevice;

	SysAcquireData(&psSysData);

	psLocalDevMemArena = psSysData->apsLocalDevMemArena[0];

	
	if(psLocalDevMemArena == IMG_NULL)
	{
		
		__DtTestPoint(__DtFunc_WorkaroundBRN22997Alloc, __DtStep_1)
		eError = OSAllocPages(PVRSRV_HAP_WRITECOMBINE | PVRSRV_HAP_KERNEL_ONLY,
						   SGX_MMU_PAGE_SIZE,
						   SGX_MMU_PAGE_SIZE,
						   (IMG_VOID **)&pui32PT,
						   &hPTPageOSMemHandle);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_WorkaroundBRN22997Alloc, __DtStep_2)
			PVR_DPF((PVR_DBG_ERROR, "WorkaroundBRN22997: ERROR call to OSAllocPages failed"));
			__DtTestPoint(__DtFunc_WorkaroundBRN22997Alloc, __DtStep_3)
			return eError;
		}
		ui32PTOffset = 0;

		eError = OSAllocPages(PVRSRV_HAP_WRITECOMBINE | PVRSRV_HAP_KERNEL_ONLY,
						   SGX_MMU_PAGE_SIZE,
						   SGX_MMU_PAGE_SIZE,
						   (IMG_VOID **)&pui32PD,
						   &hPDPageOSMemHandle);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_WorkaroundBRN22997Alloc, __DtStep_4)
			PVR_DPF((PVR_DBG_ERROR, "WorkaroundBRN22997: ERROR call to OSAllocPages failed"));
			__DtTestPoint(__DtFunc_WorkaroundBRN22997Alloc, __DtStep_5)
			return eError;
		}
		ui32PDOffset = 0;

		
		if(pui32PT)
        {
            __DtTestPoint(__DtFunc_WorkaroundBRN22997Alloc, __DtStep_6)
            sCpuPAddr = OSMapLinToCPUPhys(hPTPageOSMemHandle,
										  pui32PT);
        }
        else
        {
            
            __DtTestPoint(__DtFunc_WorkaroundBRN22997Alloc, __DtStep_7)
            sCpuPAddr = OSMemHandleToCpuPAddr(hPTPageOSMemHandle, 0);
        }
		sPTDevPAddr = SysCpuPAddrToDevPAddr(PVRSRV_DEVICE_TYPE_SGX, sCpuPAddr);

		if(pui32PD)
        {
            __DtTestPoint(__DtFunc_WorkaroundBRN22997Alloc, __DtStep_8)
            sCpuPAddr = OSMapLinToCPUPhys(hPDPageOSMemHandle,
										  pui32PD);
        }
        else
        {
            
            __DtTestPoint(__DtFunc_WorkaroundBRN22997Alloc, __DtStep_9)
            sCpuPAddr = OSMemHandleToCpuPAddr(hPDPageOSMemHandle, 0);
        }
		sPDDevPAddr = SysCpuPAddrToDevPAddr(PVRSRV_DEVICE_TYPE_SGX, sCpuPAddr);

	}
	else
	{
		

		__DtTestPoint(__DtFunc_WorkaroundBRN22997Alloc, __DtStep_10)
		if(RA_Alloc(psLocalDevMemArena,
					SGX_MMU_PAGE_SIZE * 2,
					IMG_NULL,
					IMG_NULL,
					0,
					SGX_MMU_PAGE_SIZE,
					0,
					&(psDevInfo->sBRN22997SysPAddr.uiAddr))!= IMG_TRUE)
		{
			__DtTestPoint(__DtFunc_WorkaroundBRN22997Alloc, __DtStep_11)
			PVR_DPF((PVR_DBG_ERROR, "WorkaroundBRN22997: ERROR call to RA_Alloc failed"));
			__DtTestPoint(__DtFunc_WorkaroundBRN22997Alloc, __DtStep_12)
			return PVRSRV_ERROR_OUT_OF_MEMORY;
		}

		
		sCpuPAddr = SysSysPAddrToCpuPAddr(psDevInfo->sBRN22997SysPAddr);
		pui32PT = OSMapPhysToLin(sCpuPAddr,
								SGX_MMU_PAGE_SIZE * 2,
                                PVRSRV_HAP_WRITECOMBINE|PVRSRV_HAP_KERNEL_ONLY,
								&hPTPageOSMemHandle);
		if(!pui32PT)
		{
			__DtTestPoint(__DtFunc_WorkaroundBRN22997Alloc, __DtStep_13)
			PVR_DPF((PVR_DBG_ERROR, "WorkaroundBRN22997: ERROR failed to map page tables"));
			__DtTestPoint(__DtFunc_WorkaroundBRN22997Alloc, __DtStep_14)
			return PVRSRV_ERROR_BAD_MAPPING;
		}
		ui32PTOffset = 0;

		
		sPTDevPAddr = SysCpuPAddrToDevPAddr(PVRSRV_DEVICE_TYPE_SGX, sCpuPAddr);
		
		pui32PD = pui32PT + SGX_MMU_PAGE_SIZE/sizeof(IMG_UINT32);
		ui32PDOffset = SGX_MMU_PAGE_SIZE;
		hPDPageOSMemHandle = hPTPageOSMemHandle;
		sPDDevPAddr.uiAddr = sPTDevPAddr.uiAddr + SGX_MMU_PAGE_SIZE;
	}

	OSMemSet(pui32PD, 0, SGX_MMU_PAGE_SIZE);
	OSMemSet(pui32PT, 0, SGX_MMU_PAGE_SIZE);

	
	PDUMPMALLOCPAGETABLE(&psDeviceNode->sDevId, hPDPageOSMemHandle, ui32PDOffset, pui32PD, SGX_MMU_PAGE_SIZE, 0, PDUMP_PD_UNIQUETAG);
	PDUMPMALLOCPAGETABLE(&psDeviceNode->sDevId, hPTPageOSMemHandle, ui32PTOffset, pui32PT, SGX_MMU_PAGE_SIZE, 0, PDUMP_PT_UNIQUETAG);
	PDUMPMEMPTENTRIES(&psDevInfo->sMMUAttrib, hPDPageOSMemHandle, pui32PD, SGX_MMU_PAGE_SIZE, 0, IMG_TRUE, PDUMP_PD_UNIQUETAG, PDUMP_PT_UNIQUETAG);
	PDUMPMEMPTENTRIES(&psDevInfo->sMMUAttrib, hPTPageOSMemHandle, pui32PT, SGX_MMU_PAGE_SIZE, 0, IMG_TRUE, PDUMP_PT_UNIQUETAG, PDUMP_PD_UNIQUETAG);

	psDevInfo->hBRN22997PTPageOSMemHandle = hPTPageOSMemHandle;
	psDevInfo->hBRN22997PDPageOSMemHandle = hPDPageOSMemHandle;
	psDevInfo->sBRN22997PTDevPAddr = sPTDevPAddr;
	psDevInfo->sBRN22997PDDevPAddr = sPDDevPAddr;
	psDevInfo->pui32BRN22997PD = pui32PD;
	psDevInfo->pui32BRN22997PT = pui32PT;

	__DtTestPoint(__DtFunc_WorkaroundBRN22997Alloc, __DtStep_15)
	return PVRSRV_OK;
}


IMG_VOID WorkaroundBRN22997ReadHostPort(PVRSRV_SGXDEV_INFO *psDevInfo)
{
	IMG_UINT32 *pui32PD = psDevInfo->pui32BRN22997PD;
	IMG_UINT32 *pui32PT = psDevInfo->pui32BRN22997PT;
	IMG_UINT32 ui32PDIndex;
	IMG_UINT32 ui32PTIndex;
	IMG_DEV_VIRTADDR sDevVAddr;
	volatile IMG_UINT32 *pui32HostPort;
	IMG_UINT32 ui32BIFCtrl;

	
	
	
	__DtTestPoint(__DtFunc_WorkaroundBRN22997ReadHostPort, __DtStep_0)
	pui32HostPort = (volatile IMG_UINT32*)(((IMG_UINT8*)psDevInfo->pvHostPortBaseKM) + SYS_SGX_HOSTPORT_BRN23030_OFFSET);

	
	sDevVAddr.uiAddr = SYS_SGX_HOSTPORT_BASE_DEVVADDR + SYS_SGX_HOSTPORT_BRN23030_OFFSET;

	ui32PDIndex = (sDevVAddr.uiAddr & SGX_MMU_PD_MASK) >> (SGX_MMU_PAGE_SHIFT + SGX_MMU_PT_SHIFT);
	ui32PTIndex = (sDevVAddr.uiAddr & SGX_MMU_PT_MASK) >> SGX_MMU_PAGE_SHIFT;

	
	pui32PD[ui32PDIndex] = (psDevInfo->sBRN22997PTDevPAddr.uiAddr>>SGX_MMU_PDE_ADDR_ALIGNSHIFT)
							| SGX_MMU_PDE_VALID;
	
	pui32PT[ui32PTIndex] = (psDevInfo->sBRN22997PTDevPAddr.uiAddr>>SGX_MMU_PTE_ADDR_ALIGNSHIFT)
							| SGX_MMU_PTE_VALID;

	PDUMPMEMPTENTRIES(&psDevInfo->sMMUAttrib, psDevInfo->hBRN22997PDPageOSMemHandle, pui32PD, SGX_MMU_PAGE_SIZE, 0, IMG_TRUE, PDUMP_PD_UNIQUETAG, PDUMP_PT_UNIQUETAG);
	PDUMPMEMPTENTRIES(&psDevInfo->sMMUAttrib, psDevInfo->hBRN22997PTPageOSMemHandle, pui32PT, SGX_MMU_PAGE_SIZE, 0, IMG_TRUE, PDUMP_PT_UNIQUETAG, PDUMP_PD_UNIQUETAG);

	
	OSWriteHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_BIF_DIR_LIST_BASE0,
				 psDevInfo->sBRN22997PDDevPAddr.uiAddr);
	PDUMPPDREG(&psDevInfo->sMMUAttrib, EUR_CR_BIF_DIR_LIST_BASE0, psDevInfo->sBRN22997PDDevPAddr.uiAddr, PDUMP_PD_UNIQUETAG);

	
	ui32BIFCtrl = OSReadHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_BIF_CTRL);
	OSWriteHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_BIF_CTRL, ui32BIFCtrl | EUR_CR_BIF_CTRL_INVALDC_MASK);
	PDUMPREG(SGX_PDUMPREG_NAME, EUR_CR_BIF_CTRL, ui32BIFCtrl | EUR_CR_BIF_CTRL_INVALDC_MASK);
	OSWriteHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_BIF_CTRL, ui32BIFCtrl);
	PDUMPREG(SGX_PDUMPREG_NAME, EUR_CR_BIF_CTRL, ui32BIFCtrl);

	
	if (pui32HostPort)
	{
		 
		IMG_UINT32 ui32Tmp;
		__DtTestPoint(__DtFunc_WorkaroundBRN22997ReadHostPort, __DtStep_1)
		ui32Tmp = *pui32HostPort;
	}
	else
	{
		__DtTestPoint(__DtFunc_WorkaroundBRN22997ReadHostPort, __DtStep_2)
		PVR_DPF((PVR_DBG_ERROR,"Host Port not present for BRN22997 workaround"));
	}

	




	
	PDUMPCOMMENT("RDW :SGXMEM:v4:%08X\r\n", sDevVAddr.uiAddr);
	
    PDUMPCOMMENT("SAB :SGXMEM:v4:%08X 4 0 hostport.bin", sDevVAddr.uiAddr);

	
	pui32PD[ui32PDIndex] = 0;
	pui32PT[ui32PTIndex] = 0;

	
	PDUMPMEMPTENTRIES(&psDevInfo->sMMUAttrib, psDevInfo->hBRN22997PDPageOSMemHandle, pui32PD, SGX_MMU_PAGE_SIZE, 0, IMG_TRUE, PDUMP_PD_UNIQUETAG, PDUMP_PT_UNIQUETAG);
	PDUMPMEMPTENTRIES(&psDevInfo->sMMUAttrib, psDevInfo->hBRN22997PTPageOSMemHandle, pui32PT, SGX_MMU_PAGE_SIZE, 0, IMG_TRUE, PDUMP_PT_UNIQUETAG, PDUMP_PD_UNIQUETAG);

	OSWriteHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_BIF_CTRL, ui32BIFCtrl | EUR_CR_BIF_CTRL_INVALDC_MASK);
	PDUMPREG(SGX_PDUMPREG_NAME, EUR_CR_BIF_CTRL, ui32BIFCtrl | EUR_CR_BIF_CTRL_INVALDC_MASK);
	OSWriteHWReg(psDevInfo->pvRegsBaseKM, EUR_CR_BIF_CTRL, ui32BIFCtrl);
	PDUMPREG(SGX_PDUMPREG_NAME, EUR_CR_BIF_CTRL, ui32BIFCtrl);
	__DtTestPoint(__DtFunc_WorkaroundBRN22997ReadHostPort, __DtStep_3)
}


IMG_VOID WorkaroundBRN22997Free(PVRSRV_DEVICE_NODE *psDeviceNode)
{
	SYS_DATA *psSysData;
	RA_ARENA *psLocalDevMemArena;
	PVRSRV_SGXDEV_INFO *psDevInfo = (PVRSRV_SGXDEV_INFO*)psDeviceNode->pvDevice;


	__DtTestPoint(__DtFunc_WorkaroundBRN22997Free, __DtStep_0)
	SysAcquireData(&psSysData);

	psLocalDevMemArena = psSysData->apsLocalDevMemArena[0];

	PDUMPFREEPAGETABLE(&psDeviceNode->sDevId, psDevInfo->hBRN22997PDPageOSMemHandle, psDevInfo->pui32BRN22997PD, SGX_MMU_PAGE_SIZE, 0, PDUMP_PD_UNIQUETAG);
	PDUMPFREEPAGETABLE(&psDeviceNode->sDevId, psDevInfo->hBRN22997PTPageOSMemHandle, psDevInfo->pui32BRN22997PT, SGX_MMU_PAGE_SIZE, 0, PDUMP_PT_UNIQUETAG);

	
	if(psLocalDevMemArena == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_WorkaroundBRN22997Free, __DtStep_1)
		if (psDevInfo->pui32BRN22997PD != IMG_NULL)
		{
			__DtTestPoint(__DtFunc_WorkaroundBRN22997Free, __DtStep_2)
			OSFreePages(PVRSRV_HAP_WRITECOMBINE | PVRSRV_HAP_KERNEL_ONLY,
						  SGX_MMU_PAGE_SIZE,
						  psDevInfo->pui32BRN22997PD,
						  psDevInfo->hBRN22997PDPageOSMemHandle);
		}

		if (psDevInfo->pui32BRN22997PT != IMG_NULL)
		{
			__DtTestPoint(__DtFunc_WorkaroundBRN22997Free, __DtStep_3)
			OSFreePages(PVRSRV_HAP_WRITECOMBINE | PVRSRV_HAP_KERNEL_ONLY,
						  SGX_MMU_PAGE_SIZE,
						  psDevInfo->pui32BRN22997PT,
						  psDevInfo->hBRN22997PTPageOSMemHandle);
		}
	}
	else
	{
		__DtTestPoint(__DtFunc_WorkaroundBRN22997Free, __DtStep_4)
		if (psDevInfo->pui32BRN22997PT != IMG_NULL)
		{
			__DtTestPoint(__DtFunc_WorkaroundBRN22997Free, __DtStep_5)
			OSUnMapPhysToLin(psDevInfo->pui32BRN22997PT,
				 SGX_MMU_PAGE_SIZE * 2,
				 PVRSRV_HAP_WRITECOMBINE|PVRSRV_HAP_KERNEL_ONLY,
				 psDevInfo->hBRN22997PTPageOSMemHandle);


			RA_Free(psLocalDevMemArena, psDevInfo->sBRN22997SysPAddr.uiAddr, IMG_FALSE);
		}
	}
	__DtTestPoint(__DtFunc_WorkaroundBRN22997Free, __DtStep_6)
}
#endif 


#if defined(SUPPORT_EXTERNAL_SYSTEM_CACHE)
PVRSRV_ERROR MMU_MapExtSystemCacheRegs(PVRSRV_DEVICE_NODE *psDeviceNode)
{
	PVRSRV_ERROR eError;
	SYS_DATA *psSysData;
	RA_ARENA *psLocalDevMemArena;
	IMG_HANDLE hPTPageOSMemHandle = IMG_NULL;
	IMG_UINT32 *pui32PD;
	IMG_UINT32 *pui32PT = IMG_NULL;
	IMG_CPU_PHYADDR sCpuPAddr;
	IMG_DEV_PHYADDR sPTDevPAddr;
	PVRSRV_SGXDEV_INFO *psDevInfo;
	IMG_UINT32 ui32PDIndex;
	IMG_UINT32 ui32PTIndex;

	__DtTestPoint(__DtFunc_MMU_MapExtSystemCacheRegs, __DtStep_0)
	psDevInfo = (PVRSRV_SGXDEV_INFO*)psDeviceNode->pvDevice;
	pui32PD = (IMG_UINT32*)psDeviceNode->sDevMemoryInfo.pBMKernelContext->psMMUContext->pvPDCpuVAddr;

	SysAcquireData(&psSysData);

	psLocalDevMemArena = psSysData->apsLocalDevMemArena[0];

	
	if(psLocalDevMemArena == IMG_NULL)
	{
		
		__DtTestPoint(__DtFunc_MMU_MapExtSystemCacheRegs, __DtStep_1)
		eError = OSAllocPages(PVRSRV_HAP_WRITECOMBINE | PVRSRV_HAP_KERNEL_ONLY,
						   SGX_MMU_PAGE_SIZE,
						   SGX_MMU_PAGE_SIZE,
						   (IMG_VOID **)&pui32PT,
						   &hPTPageOSMemHandle);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_MMU_MapExtSystemCacheRegs, __DtStep_2)
			PVR_DPF((PVR_DBG_ERROR, "MMU_MapExtSystemCacheRegs: ERROR call to OSAllocPages failed"));
			__DtTestPoint(__DtFunc_MMU_MapExtSystemCacheRegs, __DtStep_3)
			return eError;
		}

		
		if(pui32PT)
        {
            __DtTestPoint(__DtFunc_MMU_MapExtSystemCacheRegs, __DtStep_4)
            sCpuPAddr = OSMapLinToCPUPhys(hPTPageOSMemHandle,
										  pui32PT);
        }
        else
        {
            
            __DtTestPoint(__DtFunc_MMU_MapExtSystemCacheRegs, __DtStep_5)
            sCpuPAddr = OSMemHandleToCpuPAddr(hPTPageOSMemHandle, 0);
        }
		sPTDevPAddr = SysCpuPAddrToDevPAddr(PVRSRV_DEVICE_TYPE_SGX, sCpuPAddr);
	}
	else
	{
		IMG_SYS_PHYADDR sSysPAddr;

		
		__DtTestPoint(__DtFunc_MMU_MapExtSystemCacheRegs, __DtStep_6)
		if(RA_Alloc(psLocalDevMemArena,
					SGX_MMU_PAGE_SIZE,
					IMG_NULL,
					IMG_NULL,
					0,
					SGX_MMU_PAGE_SIZE,
					0,
					&(sSysPAddr.uiAddr))!= IMG_TRUE)
		{
			__DtTestPoint(__DtFunc_MMU_MapExtSystemCacheRegs, __DtStep_7)
			PVR_DPF((PVR_DBG_ERROR, "MMU_MapExtSystemCacheRegs: ERROR call to RA_Alloc failed"));
			__DtTestPoint(__DtFunc_MMU_MapExtSystemCacheRegs, __DtStep_8)
			return PVRSRV_ERROR_OUT_OF_MEMORY;
		}

		
		sCpuPAddr = SysSysPAddrToCpuPAddr(sSysPAddr);
		pui32PT = OSMapPhysToLin(sCpuPAddr,
								SGX_MMU_PAGE_SIZE,
                                PVRSRV_HAP_WRITECOMBINE|PVRSRV_HAP_KERNEL_ONLY,
								&hPTPageOSMemHandle);
		if(!pui32PT)
		{
			__DtTestPoint(__DtFunc_MMU_MapExtSystemCacheRegs, __DtStep_9)
			PVR_DPF((PVR_DBG_ERROR, "MMU_MapExtSystemCacheRegs: ERROR failed to map page tables"));
			__DtTestPoint(__DtFunc_MMU_MapExtSystemCacheRegs, __DtStep_10)
			return PVRSRV_ERROR_BAD_MAPPING;
		}

		
		sPTDevPAddr = SysCpuPAddrToDevPAddr(PVRSRV_DEVICE_TYPE_SGX, sCpuPAddr);

		
		psDevInfo->sExtSystemCacheRegsPTSysPAddr = sSysPAddr;
	}

	OSMemSet(pui32PT, 0, SGX_MMU_PAGE_SIZE);

	ui32PDIndex = (SGX_EXT_SYSTEM_CACHE_REGS_DEVVADDR_BASE & SGX_MMU_PD_MASK) >> (SGX_MMU_PAGE_SHIFT + SGX_MMU_PT_SHIFT);
	ui32PTIndex = (SGX_EXT_SYSTEM_CACHE_REGS_DEVVADDR_BASE & SGX_MMU_PT_MASK) >> SGX_MMU_PAGE_SHIFT;

	
	pui32PD[ui32PDIndex] = (sPTDevPAddr.uiAddr>>SGX_MMU_PDE_ADDR_ALIGNSHIFT)
							| SGX_MMU_PDE_VALID;
	
	pui32PT[ui32PTIndex] = (psDevInfo->sExtSysCacheRegsDevPBase.uiAddr>>SGX_MMU_PTE_ADDR_ALIGNSHIFT)
							| SGX_MMU_PTE_VALID;

	
	PDUMPMALLOCPAGETABLE(&psDeviceNode->sDevID, hPTPageOSMemHandle, 0, pui32PT, SGX_MMU_PAGE_SIZE, 0, PDUMP_PT_UNIQUETAG);
	PDUMPMEMPTENTRIES(PVRSRV_DEVICE_TYPE_SGX, hPDPageOSMemHandle, pui32PD, SGX_MMU_PAGE_SIZE, 0, IMG_TRUE, PDUMP_PD_UNIQUETAG, PDUMP_PT_UNIQUETAG);
	PDUMPMEMPTENTRIES(PVRSRV_DEVICE_TYPE_SGX, hPTPageOSMemHandle, pui32PT, SGX_MMU_PAGE_SIZE, 0, IMG_TRUE, PDUMP_PT_UNIQUETAG, PDUMP_PD_UNIQUETAG);

	
	psDevInfo->pui32ExtSystemCacheRegsPT = pui32PT;
	psDevInfo->hExtSystemCacheRegsPTPageOSMemHandle = hPTPageOSMemHandle;

	__DtTestPoint(__DtFunc_MMU_MapExtSystemCacheRegs, __DtStep_11)
	return PVRSRV_OK;
}


PVRSRV_ERROR MMU_UnmapExtSystemCacheRegs(PVRSRV_DEVICE_NODE *psDeviceNode)
{
	SYS_DATA *psSysData;
	RA_ARENA *psLocalDevMemArena;
	PVRSRV_SGXDEV_INFO *psDevInfo;
	IMG_UINT32 ui32PDIndex;
	IMG_UINT32 *pui32PD;

	__DtTestPoint(__DtFunc_MMU_UnmapExtSystemCacheRegs, __DtStep_0)
	psDevInfo = (PVRSRV_SGXDEV_INFO*)psDeviceNode->pvDevice;
	pui32PD = (IMG_UINT32*)psDeviceNode->sDevMemoryInfo.pBMKernelContext->psMMUContext->pvPDCpuVAddr;

	SysAcquireData(&psSysData);

	psLocalDevMemArena = psSysData->apsLocalDevMemArena[0];

	
	ui32PDIndex = (SGX_EXT_SYSTEM_CACHE_REGS_DEVVADDR_BASE & SGX_MMU_PD_MASK) >> (SGX_MMU_PAGE_SHIFT + SGX_MMU_PT_SHIFT);
	pui32PD[ui32PDIndex] = 0;

	PDUMPMEMPTENTRIES(PVRSRV_DEVICE_TYPE_SGX, psDeviceNode->sDevMemoryInfo.pBMKernelContext->psMMUContext->hPDOSMemHandle, pui32PD, SGX_MMU_PAGE_SIZE, 0, IMG_TRUE, PDUMP_PD_UNIQUETAG, PDUMP_PT_UNIQUETAG);
	PDUMPFREEPAGETABLE(&psDeviceNode->sDevId, psDevInfo->hExtSystemCacheRegsPTPageOSMemHandle, psDevInfo->pui32ExtSystemCacheRegsPT, SGX_MMU_PAGE_SIZE, 0, PDUMP_PT_UNIQUETAG);

	
	if(psLocalDevMemArena == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_MMU_UnmapExtSystemCacheRegs, __DtStep_1)
		if (psDevInfo->pui32ExtSystemCacheRegsPT != IMG_NULL)
		{
			__DtTestPoint(__DtFunc_MMU_UnmapExtSystemCacheRegs, __DtStep_2)
			OSFreePages(PVRSRV_HAP_WRITECOMBINE | PVRSRV_HAP_KERNEL_ONLY,
						  SGX_MMU_PAGE_SIZE,
						  psDevInfo->pui32ExtSystemCacheRegsPT,
						  psDevInfo->hExtSystemCacheRegsPTPageOSMemHandle);
		}
	}
	else
	{
		__DtTestPoint(__DtFunc_MMU_UnmapExtSystemCacheRegs, __DtStep_3)
		if (psDevInfo->pui32ExtSystemCacheRegsPT != IMG_NULL)
		{
			__DtTestPoint(__DtFunc_MMU_UnmapExtSystemCacheRegs, __DtStep_4)
			OSUnMapPhysToLin(psDevInfo->pui32ExtSystemCacheRegsPT,
				 SGX_MMU_PAGE_SIZE,
				 PVRSRV_HAP_WRITECOMBINE|PVRSRV_HAP_KERNEL_ONLY,
				 psDevInfo->hExtSystemCacheRegsPTPageOSMemHandle);

			RA_Free(psLocalDevMemArena, psDevInfo->sExtSystemCacheRegsPTSysPAddr.uiAddr, IMG_FALSE);
		}
	}

	__DtTestPoint(__DtFunc_MMU_UnmapExtSystemCacheRegs, __DtStep_5)
	return PVRSRV_OK;
}
#endif


#if PAGE_TEST
static IMG_VOID PageTest(IMG_VOID* pMem, IMG_DEV_PHYADDR sDevPAddr)
{
	volatile IMG_UINT32 ui32WriteData;
	volatile IMG_UINT32 ui32ReadData;
	volatile IMG_UINT32 *pMem32 = (volatile IMG_UINT32 *)pMem;
	IMG_INT n;
	IMG_BOOL bOK=IMG_TRUE;

	__DtTestPoint(__DtFunc_PageTest, __DtStep_0)
	ui32WriteData = 0xffffffff;

	for (n=0; n<1024; n++)
	{
		__DtTestPoint(__DtFunc_PageTest, __DtStep_1)
		pMem32[n] = ui32WriteData;
		ui32ReadData = pMem32[n];

		if (ui32WriteData != ui32ReadData)
		{
			
			__DtTestPoint(__DtFunc_PageTest, __DtStep_2)
			PVR_DPF ((PVR_DBG_ERROR, "Error - memory page test failed at device phys address 0x%08X", sDevPAddr.uiAddr + (n<<2) ));
			PVR_DBG_BREAK;
			bOK = IMG_FALSE;
		}
 	}

	ui32WriteData = 0;

	for (n=0; n<1024; n++)
	{
		__DtTestPoint(__DtFunc_PageTest, __DtStep_3)
		pMem32[n] = ui32WriteData;
		ui32ReadData = pMem32[n];

		if (ui32WriteData != ui32ReadData)
		{
			
			__DtTestPoint(__DtFunc_PageTest, __DtStep_4)
			PVR_DPF ((PVR_DBG_ERROR, "Error - memory page test failed at device phys address 0x%08X", sDevPAddr.uiAddr + (n<<2) ));
			PVR_DBG_BREAK;
			bOK = IMG_FALSE;
		}
 	}

	if (bOK)
	{
		__DtTestPoint(__DtFunc_PageTest, __DtStep_5)
		PVR_DPF ((PVR_DBG_VERBOSE, "MMU Page 0x%08X is OK", sDevPAddr.uiAddr));
	}
	else
	{
		__DtTestPoint(__DtFunc_PageTest, __DtStep_6)
		PVR_DPF ((PVR_DBG_VERBOSE, "MMU Page 0x%08X *** FAILED ***", sDevPAddr.uiAddr));
	}
	__DtTestPoint(__DtFunc_PageTest, __DtStep_7)
}
#endif

