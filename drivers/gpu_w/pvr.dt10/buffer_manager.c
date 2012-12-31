#include	"DT_buffer_manager.h"	/* For DynamicTracer-TestPoint */
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

#include "sysconfig.h"
#include "hash.h"
#include "ra.h"
#include "pdump_km.h"
#include "lists.h"

static IMG_BOOL
ZeroBuf(BM_BUF *pBuf, BM_MAPPING *pMapping, IMG_SIZE_T ui32Bytes, IMG_UINT32 ui32Flags);
static IMG_VOID
BM_FreeMemory (IMG_VOID *pH, IMG_UINTPTR_T base, BM_MAPPING *psMapping);
static IMG_BOOL
BM_ImportMemory(IMG_VOID *pH, IMG_SIZE_T uSize,
					IMG_SIZE_T *pActualSize, BM_MAPPING **ppsMapping,
					IMG_UINT32 uFlags, IMG_UINTPTR_T *pBase);

static IMG_BOOL
DevMemoryAlloc (BM_CONTEXT *pBMContext,
				BM_MAPPING *pMapping,
				IMG_SIZE_T *pActualSize,
				IMG_UINT32 uFlags,
				IMG_UINT32 dev_vaddr_alignment,
				IMG_DEV_VIRTADDR *pDevVAddr);
static IMG_VOID
DevMemoryFree (BM_MAPPING *pMapping);

static IMG_BOOL
AllocMemory (BM_CONTEXT				*pBMContext,
				BM_HEAP				*psBMHeap,
				IMG_DEV_VIRTADDR	*psDevVAddr,
				IMG_SIZE_T			uSize,
				IMG_UINT32			uFlags,
				IMG_UINT32			uDevVAddrAlignment,
				BM_BUF				*pBuf)
{
	BM_MAPPING			*pMapping;
	IMG_UINTPTR_T		uOffset;
	RA_ARENA			*pArena = IMG_NULL;

	__DtTestPoint(__DtFunc_AllocMemory, __DtStep_0)
	PVR_DPF ((PVR_DBG_MESSAGE,
			  "AllocMemory (uSize=0x%x, uFlags=0x%x, align=0x%x)",
			  uSize, uFlags, uDevVAddrAlignment));

	


	if(uFlags & PVRSRV_MEM_RAM_BACKED_ALLOCATION)
	{
		__DtTestPoint(__DtFunc_AllocMemory, __DtStep_1)
		if(uFlags & PVRSRV_MEM_USER_SUPPLIED_DEVVADDR)
		{
			
			__DtTestPoint(__DtFunc_AllocMemory, __DtStep_2)
			PVR_DPF ((PVR_DBG_ERROR, "AllocMemory: combination of DevVAddr management and RAM backing mode unsupported"));
			__DtTestPoint(__DtFunc_AllocMemory, __DtStep_3)
			return IMG_FALSE;
		}

		

		
		if(psBMHeap->ui32Attribs
		   &	(PVRSRV_BACKINGSTORE_SYSMEM_NONCONTIG
		   |PVRSRV_BACKINGSTORE_LOCALMEM_CONTIG))
		{
			
			__DtTestPoint(__DtFunc_AllocMemory, __DtStep_4)
			pArena = psBMHeap->pImportArena;
			PVR_ASSERT(psBMHeap->sDevArena.psDeviceMemoryHeapInfo->ui32Attribs & PVRSRV_MEM_RAM_BACKED_ALLOCATION);
		}
		else
		{
			__DtTestPoint(__DtFunc_AllocMemory, __DtStep_5)
			PVR_DPF ((PVR_DBG_ERROR, "AllocMemory: backing store type doesn't match heap"));
			__DtTestPoint(__DtFunc_AllocMemory, __DtStep_6)
			return IMG_FALSE;
		}

		
		if (!RA_Alloc(pArena,
					  uSize,
					  IMG_NULL,
					  (IMG_VOID*) &pMapping,
					  uFlags,
					  uDevVAddrAlignment,
					  0,
					  (IMG_UINTPTR_T *)&(pBuf->DevVAddr.uiAddr)))
		{
			__DtTestPoint(__DtFunc_AllocMemory, __DtStep_7)
			PVR_DPF((PVR_DBG_ERROR, "AllocMemory: RA_Alloc(0x%x) FAILED", uSize));
			__DtTestPoint(__DtFunc_AllocMemory, __DtStep_8)
			return IMG_FALSE;
		}

		uOffset = pBuf->DevVAddr.uiAddr - pMapping->DevVAddr.uiAddr;
		if(pMapping->CpuVAddr)
		{
			__DtTestPoint(__DtFunc_AllocMemory, __DtStep_9)
			pBuf->CpuVAddr = (IMG_VOID*) ((IMG_UINTPTR_T)pMapping->CpuVAddr + uOffset);
		}
		else
		{
			__DtTestPoint(__DtFunc_AllocMemory, __DtStep_10)
			pBuf->CpuVAddr = IMG_NULL;
		}

		if(uSize == pMapping->uSize)
		{
			__DtTestPoint(__DtFunc_AllocMemory, __DtStep_11)
			pBuf->hOSMemHandle = pMapping->hOSMemHandle;
		}
		else
		{
			__DtTestPoint(__DtFunc_AllocMemory, __DtStep_12)
			if(OSGetSubMemHandle(pMapping->hOSMemHandle,
								 uOffset,
								 uSize,
								 psBMHeap->ui32Attribs,
								 &pBuf->hOSMemHandle)!=PVRSRV_OK)
			{
				__DtTestPoint(__DtFunc_AllocMemory, __DtStep_13)
				PVR_DPF((PVR_DBG_ERROR, "AllocMemory: OSGetSubMemHandle FAILED"));
				__DtTestPoint(__DtFunc_AllocMemory, __DtStep_14)
				return IMG_FALSE;
			}
		}

		
		pBuf->CpuPAddr.uiAddr = pMapping->CpuPAddr.uiAddr + uOffset;

		if(uFlags & PVRSRV_MEM_ZERO)
		{
			__DtTestPoint(__DtFunc_AllocMemory, __DtStep_15)
			if(!ZeroBuf(pBuf, pMapping, uSize, psBMHeap->ui32Attribs | uFlags))
			{
				__DtTestPoint(__DtFunc_AllocMemory, __DtStep_16)
				return IMG_FALSE;
			}
		}
	}
	else
	{
		__DtTestPoint(__DtFunc_AllocMemory, __DtStep_17)
		if(uFlags & PVRSRV_MEM_USER_SUPPLIED_DEVVADDR)
		{
			
			__DtTestPoint(__DtFunc_AllocMemory, __DtStep_18)
			PVR_ASSERT(psDevVAddr != IMG_NULL);

			if (psDevVAddr == IMG_NULL)
			{
				__DtTestPoint(__DtFunc_AllocMemory, __DtStep_19)
				PVR_DPF((PVR_DBG_ERROR, "AllocMemory: invalid parameter - psDevVAddr"));
				__DtTestPoint(__DtFunc_AllocMemory, __DtStep_20)
				return IMG_FALSE;
			}

			
			pBMContext->psDeviceNode->pfnMMUAlloc (psBMHeap->pMMUHeap,
													uSize,
													IMG_NULL,
													PVRSRV_MEM_USER_SUPPLIED_DEVVADDR,
													uDevVAddrAlignment,
													psDevVAddr);

			
			pBuf->DevVAddr = *psDevVAddr;
		}
		else
		{
			

			
			__DtTestPoint(__DtFunc_AllocMemory, __DtStep_21)
			pBMContext->psDeviceNode->pfnMMUAlloc (psBMHeap->pMMUHeap,
													uSize,
													IMG_NULL,
													0,
													uDevVAddrAlignment,
													&pBuf->DevVAddr);
		}

		
		if (OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP,
							sizeof (struct _BM_MAPPING_),
							(IMG_PVOID *)&pMapping, IMG_NULL,
							"Buffer Manager Mapping") != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_AllocMemory, __DtStep_22)
			PVR_DPF((PVR_DBG_ERROR, "AllocMemory: OSAllocMem(0x%x) FAILED", sizeof(*pMapping)));
			__DtTestPoint(__DtFunc_AllocMemory, __DtStep_23)
			return IMG_FALSE;
		}

		
		pBuf->CpuVAddr = IMG_NULL;
		pBuf->hOSMemHandle = 0;
		pBuf->CpuPAddr.uiAddr = 0;

		
		pMapping->CpuVAddr = IMG_NULL;
		pMapping->CpuPAddr.uiAddr = 0;
		pMapping->DevVAddr = pBuf->DevVAddr;
		pMapping->psSysAddr = IMG_NULL;
		pMapping->uSize = uSize;
		pMapping->hOSMemHandle = 0;
	}

	
	pMapping->pArena = pArena;

	
	pMapping->pBMHeap = psBMHeap;
	pBuf->pMapping = pMapping;

	
	PVR_DPF ((PVR_DBG_MESSAGE,
				"AllocMemory: pMapping=%08x: DevV=%08X CpuV=%08x CpuP=%08X uSize=0x%x",
				(IMG_UINTPTR_T)pMapping,
				pMapping->DevVAddr.uiAddr,
				(IMG_UINTPTR_T)pMapping->CpuVAddr,
				pMapping->CpuPAddr.uiAddr,
				pMapping->uSize));

	PVR_DPF ((PVR_DBG_MESSAGE,
				"AllocMemory: pBuf=%08x: DevV=%08X CpuV=%08x CpuP=%08X uSize=0x%x",
				(IMG_UINTPTR_T)pBuf,
				pBuf->DevVAddr.uiAddr,
				(IMG_UINTPTR_T)pBuf->CpuVAddr,
				pBuf->CpuPAddr.uiAddr,
				uSize));

	
	PVR_ASSERT(((pBuf->DevVAddr.uiAddr) & (uDevVAddrAlignment - 1)) == 0);

	__DtTestPoint(__DtFunc_AllocMemory, __DtStep_24)
	return IMG_TRUE;
}


static IMG_BOOL
WrapMemory (BM_HEAP *psBMHeap,
			IMG_SIZE_T uSize,
			IMG_SIZE_T ui32BaseOffset,
			IMG_BOOL bPhysContig,
			IMG_SYS_PHYADDR *psAddr,
			IMG_VOID *pvCPUVAddr,
			IMG_UINT32 uFlags,
			BM_BUF *pBuf)
{
	IMG_DEV_VIRTADDR DevVAddr = {0};
	BM_MAPPING *pMapping;
	IMG_BOOL bResult;
	IMG_SIZE_T const ui32PageSize = HOST_PAGESIZE();

	__DtTestPoint(__DtFunc_WrapMemory, __DtStep_0)
	PVR_DPF ((PVR_DBG_MESSAGE,
			  "WrapMemory(psBMHeap=%08X, size=0x%x, offset=0x%x, bPhysContig=0x%x, pvCPUVAddr = 0x%08x, flags=0x%x)",
			  (IMG_UINTPTR_T)psBMHeap, uSize, ui32BaseOffset, bPhysContig, (IMG_UINTPTR_T)pvCPUVAddr, uFlags));

	PVR_ASSERT((psAddr->uiAddr & (ui32PageSize - 1)) == 0);
	
	PVR_ASSERT(((IMG_UINTPTR_T)pvCPUVAddr & (ui32PageSize - 1)) == 0);

	uSize += ui32BaseOffset;
	uSize = HOST_PAGEALIGN (uSize);

	
	if (OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP,
						sizeof(*pMapping),
						(IMG_PVOID *)&pMapping, IMG_NULL,
						"Mocked-up mapping") != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_WrapMemory, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "WrapMemory: OSAllocMem(0x%x) FAILED",sizeof(*pMapping)));
		__DtTestPoint(__DtFunc_WrapMemory, __DtStep_2)
		return IMG_FALSE;
	}

	OSMemSet(pMapping, 0, sizeof (*pMapping));

	pMapping->uSize = uSize;
	pMapping->pBMHeap = psBMHeap;

	if(pvCPUVAddr)
	{
		__DtTestPoint(__DtFunc_WrapMemory, __DtStep_3)
		pMapping->CpuVAddr = pvCPUVAddr;

		if (bPhysContig)
		{
			__DtTestPoint(__DtFunc_WrapMemory, __DtStep_4)
			pMapping->eCpuMemoryOrigin = hm_wrapped_virtaddr;
			pMapping->CpuPAddr = SysSysPAddrToCpuPAddr(psAddr[0]);

			if(OSRegisterMem(pMapping->CpuPAddr,
							pMapping->CpuVAddr,
							pMapping->uSize,
							uFlags,
							&pMapping->hOSMemHandle) != PVRSRV_OK)
			{
				__DtTestPoint(__DtFunc_WrapMemory, __DtStep_5)
				PVR_DPF((PVR_DBG_ERROR,	"WrapMemory: OSRegisterMem Phys=0x%08X, Size=%d) failed",
					pMapping->CpuPAddr.uiAddr, pMapping->uSize));
				goto fail_cleanup;
			}
		}
		else
		{
			__DtTestPoint(__DtFunc_WrapMemory, __DtStep_6)
			pMapping->eCpuMemoryOrigin = hm_wrapped_scatter_virtaddr;
			pMapping->psSysAddr = psAddr;

			if(OSRegisterDiscontigMem(pMapping->psSysAddr,
							pMapping->CpuVAddr,
							pMapping->uSize,
							uFlags,
							&pMapping->hOSMemHandle) != PVRSRV_OK)
			{
				__DtTestPoint(__DtFunc_WrapMemory, __DtStep_7)
				PVR_DPF((PVR_DBG_ERROR,	"WrapMemory: OSRegisterDiscontigMem Size=%d) failed",
					pMapping->uSize));
				goto fail_cleanup;
			}
		}
	}
	else
	{
		__DtTestPoint(__DtFunc_WrapMemory, __DtStep_8)
		if (bPhysContig)
		{
			__DtTestPoint(__DtFunc_WrapMemory, __DtStep_9)
			pMapping->eCpuMemoryOrigin = hm_wrapped;
			pMapping->CpuPAddr = SysSysPAddrToCpuPAddr(psAddr[0]);

			if(OSReservePhys(pMapping->CpuPAddr,
							 pMapping->uSize,
							 uFlags,
							 &pMapping->CpuVAddr,
							 &pMapping->hOSMemHandle) != PVRSRV_OK)
			{
				__DtTestPoint(__DtFunc_WrapMemory, __DtStep_10)
				PVR_DPF((PVR_DBG_ERROR,	"WrapMemory: OSReservePhys Phys=0x%08X, Size=%d) failed",
					pMapping->CpuPAddr.uiAddr, pMapping->uSize));
				goto fail_cleanup;
			}
		}
		else
		{
			__DtTestPoint(__DtFunc_WrapMemory, __DtStep_11)
			pMapping->eCpuMemoryOrigin = hm_wrapped_scatter;
			pMapping->psSysAddr = psAddr;

			if(OSReserveDiscontigPhys(pMapping->psSysAddr,
							 pMapping->uSize,
							 uFlags,
							 &pMapping->CpuVAddr,
							 &pMapping->hOSMemHandle) != PVRSRV_OK)
			{
				__DtTestPoint(__DtFunc_WrapMemory, __DtStep_12)
				PVR_DPF((PVR_DBG_ERROR,	"WrapMemory: OSReserveDiscontigPhys Size=%d) failed",
					pMapping->uSize));
				goto fail_cleanup;
			}
		}
	}

	
	bResult = DevMemoryAlloc(psBMHeap->pBMContext,
							 pMapping,
							 IMG_NULL,
							 uFlags | PVRSRV_MEM_READ | PVRSRV_MEM_WRITE,
							 IMG_CAST_TO_DEVVADDR_UINT(ui32PageSize),
							 &DevVAddr);
	if (!bResult)
	{
		__DtTestPoint(__DtFunc_WrapMemory, __DtStep_13)
		PVR_DPF((PVR_DBG_ERROR,
				"WrapMemory: DevMemoryAlloc(0x%x) failed",
				pMapping->uSize));
		goto fail_cleanup;
	}

	
	pBuf->CpuPAddr.uiAddr = pMapping->CpuPAddr.uiAddr + ui32BaseOffset;
	if(!ui32BaseOffset)
	{
		__DtTestPoint(__DtFunc_WrapMemory, __DtStep_14)
		pBuf->hOSMemHandle = pMapping->hOSMemHandle;
	}
	else
	{
		__DtTestPoint(__DtFunc_WrapMemory, __DtStep_15)
		if(OSGetSubMemHandle(pMapping->hOSMemHandle,
							 ui32BaseOffset,
							 (pMapping->uSize-ui32BaseOffset),
							 uFlags,
							 &pBuf->hOSMemHandle)!=PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_WrapMemory, __DtStep_16)
			PVR_DPF((PVR_DBG_ERROR, "WrapMemory: OSGetSubMemHandle failed"));
			goto fail_cleanup;
		}
	}
	if(pMapping->CpuVAddr)
	{
		__DtTestPoint(__DtFunc_WrapMemory, __DtStep_17)
		pBuf->CpuVAddr = (IMG_VOID*) ((IMG_UINTPTR_T)pMapping->CpuVAddr + ui32BaseOffset);
	}
	pBuf->DevVAddr.uiAddr = pMapping->DevVAddr.uiAddr + IMG_CAST_TO_DEVVADDR_UINT(ui32BaseOffset);

	if(uFlags & PVRSRV_MEM_ZERO)
	{
		__DtTestPoint(__DtFunc_WrapMemory, __DtStep_18)
		if(!ZeroBuf(pBuf, pMapping, uSize, uFlags))
		{
			__DtTestPoint(__DtFunc_WrapMemory, __DtStep_19)
			return IMG_FALSE;
		}
	}

	PVR_DPF ((PVR_DBG_MESSAGE, "DevVaddr.uiAddr=%08X", DevVAddr.uiAddr));
	PVR_DPF ((PVR_DBG_MESSAGE,
				"WrapMemory: DevV=%08X CpuP=%08X uSize=0x%x",
				pMapping->DevVAddr.uiAddr, pMapping->CpuPAddr.uiAddr, pMapping->uSize));
	PVR_DPF ((PVR_DBG_MESSAGE,
				"WrapMemory: DevV=%08X CpuP=%08X uSize=0x%x",
				pBuf->DevVAddr.uiAddr, pBuf->CpuPAddr.uiAddr, uSize));

	pBuf->pMapping = pMapping;
	__DtTestPoint(__DtFunc_WrapMemory, __DtStep_20)
	return IMG_TRUE;

fail_cleanup:
	if(ui32BaseOffset && pBuf->hOSMemHandle)
	{
		__DtTestPoint(__DtFunc_WrapMemory, __DtStep_21)
		OSReleaseSubMemHandle(pBuf->hOSMemHandle, uFlags);
	}

	if(pMapping && (pMapping->CpuVAddr || pMapping->hOSMemHandle))
	{
		__DtTestPoint(__DtFunc_WrapMemory, __DtStep_22)
		switch(pMapping->eCpuMemoryOrigin)
		{
			case hm_wrapped:
				__DtTestPoint(__DtFunc_WrapMemory, __DtStep_23)
				OSUnReservePhys(pMapping->CpuVAddr, pMapping->uSize, uFlags, pMapping->hOSMemHandle);
				break;
			case hm_wrapped_virtaddr:
				__DtTestPoint(__DtFunc_WrapMemory, __DtStep_24)
				OSUnRegisterMem(pMapping->CpuVAddr, pMapping->uSize, uFlags, pMapping->hOSMemHandle);
				break;
			case hm_wrapped_scatter:
				__DtTestPoint(__DtFunc_WrapMemory, __DtStep_25)
				OSUnReserveDiscontigPhys(pMapping->CpuVAddr, pMapping->uSize, uFlags, pMapping->hOSMemHandle);
				break;
			case hm_wrapped_scatter_virtaddr:
				__DtTestPoint(__DtFunc_WrapMemory, __DtStep_26)
				OSUnRegisterDiscontigMem(pMapping->CpuVAddr, pMapping->uSize, uFlags, pMapping->hOSMemHandle);
				break;
			default:
				__DtTestPoint(__DtFunc_WrapMemory, __DtStep_27)
				break;
		}

	}

	OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(BM_MAPPING), pMapping, IMG_NULL);
	

	__DtTestPoint(__DtFunc_WrapMemory, __DtStep_28)
	return IMG_FALSE;
}


static IMG_BOOL
ZeroBuf(BM_BUF *pBuf, BM_MAPPING *pMapping, IMG_SIZE_T ui32Bytes, IMG_UINT32 ui32Flags)
{
	IMG_VOID *pvCpuVAddr;

	__DtTestPoint(__DtFunc_ZeroBuf, __DtStep_0)
	if(pBuf->CpuVAddr)
	{
		__DtTestPoint(__DtFunc_ZeroBuf, __DtStep_1)
		OSMemSet(pBuf->CpuVAddr, 0, ui32Bytes);
	}
	else if(pMapping->eCpuMemoryOrigin == hm_contiguous
			|| pMapping->eCpuMemoryOrigin == hm_wrapped)
	{
		__DtTestPoint(__DtFunc_ZeroBuf, __DtStep_2)
		pvCpuVAddr = OSMapPhysToLin(pBuf->CpuPAddr,
									ui32Bytes,
									PVRSRV_HAP_KERNEL_ONLY
									| (ui32Flags & PVRSRV_HAP_CACHETYPE_MASK),
									IMG_NULL);
		if(!pvCpuVAddr)
		{
			__DtTestPoint(__DtFunc_ZeroBuf, __DtStep_3)
			PVR_DPF((PVR_DBG_ERROR, "ZeroBuf: OSMapPhysToLin for contiguous buffer failed"));
			__DtTestPoint(__DtFunc_ZeroBuf, __DtStep_4)
			return IMG_FALSE;
		}
		OSMemSet(pvCpuVAddr, 0, ui32Bytes);
		OSUnMapPhysToLin(pvCpuVAddr,
						 ui32Bytes,
						 PVRSRV_HAP_KERNEL_ONLY
						 | (ui32Flags & PVRSRV_HAP_CACHETYPE_MASK),
						 IMG_NULL);
	}
	else
	{
		IMG_SIZE_T ui32BytesRemaining = ui32Bytes;
		IMG_SIZE_T ui32CurrentOffset = 0;
		IMG_CPU_PHYADDR CpuPAddr;

		
		__DtTestPoint(__DtFunc_ZeroBuf, __DtStep_5)
		PVR_ASSERT(pBuf->hOSMemHandle);

		while(ui32BytesRemaining > 0)
		{
			IMG_SIZE_T ui32BlockBytes = MIN(ui32BytesRemaining, HOST_PAGESIZE());
			__DtTestPoint(__DtFunc_ZeroBuf, __DtStep_6)
			CpuPAddr = OSMemHandleToCpuPAddr(pBuf->hOSMemHandle, ui32CurrentOffset);
			
			if(CpuPAddr.uiAddr & (HOST_PAGESIZE() -1))
			{
				__DtTestPoint(__DtFunc_ZeroBuf, __DtStep_7)
				ui32BlockBytes =
					MIN(ui32BytesRemaining, (IMG_UINT32)(HOST_PAGEALIGN(CpuPAddr.uiAddr) - CpuPAddr.uiAddr));
			}

			pvCpuVAddr = OSMapPhysToLin(CpuPAddr,
										ui32BlockBytes,
										PVRSRV_HAP_KERNEL_ONLY
										| (ui32Flags & PVRSRV_HAP_CACHETYPE_MASK),
										IMG_NULL);
			if(!pvCpuVAddr)
			{
				__DtTestPoint(__DtFunc_ZeroBuf, __DtStep_8)
				PVR_DPF((PVR_DBG_ERROR, "ZeroBuf: OSMapPhysToLin while zeroing non-contiguous memory FAILED"));
				__DtTestPoint(__DtFunc_ZeroBuf, __DtStep_9)
				return IMG_FALSE;
			}
			OSMemSet(pvCpuVAddr, 0, ui32BlockBytes);
			OSUnMapPhysToLin(pvCpuVAddr,
							 ui32BlockBytes,
							 PVRSRV_HAP_KERNEL_ONLY
							 | (ui32Flags & PVRSRV_HAP_CACHETYPE_MASK),
							 IMG_NULL);

			ui32BytesRemaining -= ui32BlockBytes;
			ui32CurrentOffset += ui32BlockBytes;
		}
	}

	__DtTestPoint(__DtFunc_ZeroBuf, __DtStep_10)
	return IMG_TRUE;
}

static IMG_VOID
FreeBuf (BM_BUF *pBuf, IMG_UINT32 ui32Flags, IMG_BOOL bFromAllocator)
{
	BM_MAPPING *pMapping;

	__DtTestPoint(__DtFunc_FreeBuf, __DtStep_0)
	PVR_DPF ((PVR_DBG_MESSAGE,
			"FreeBuf: pBuf=0x%x: DevVAddr=%08X CpuVAddr=0x%x CpuPAddr=%08X",
			(IMG_UINTPTR_T)pBuf, pBuf->DevVAddr.uiAddr,
			(IMG_UINTPTR_T)pBuf->CpuVAddr, pBuf->CpuPAddr.uiAddr));

	
	pMapping = pBuf->pMapping;

	if(ui32Flags & PVRSRV_MEM_USER_SUPPLIED_DEVVADDR)
	{
		
		__DtTestPoint(__DtFunc_FreeBuf, __DtStep_1)
		if ((pBuf->ui32ExportCount == 0) && (pBuf->ui32RefCount == 0))
		{
			
			__DtTestPoint(__DtFunc_FreeBuf, __DtStep_2)
			if(ui32Flags & PVRSRV_MEM_RAM_BACKED_ALLOCATION)
			{
				
				__DtTestPoint(__DtFunc_FreeBuf, __DtStep_3)
				PVR_DPF ((PVR_DBG_ERROR, "FreeBuf: combination of DevVAddr management and RAM backing mode unsupported"));
			}
			else
			{
				
				__DtTestPoint(__DtFunc_FreeBuf, __DtStep_4)
				OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(BM_MAPPING), pMapping, IMG_NULL);
				pBuf->pMapping = IMG_NULL; 
			}
		}
	}
	else
	{
		
		__DtTestPoint(__DtFunc_FreeBuf, __DtStep_5)
		if(pBuf->hOSMemHandle != pMapping->hOSMemHandle)
		{
            
			__DtTestPoint(__DtFunc_FreeBuf, __DtStep_6)
			if ((pBuf->ui32ExportCount == 0) && (pBuf->ui32RefCount == 0))
			{
				
				__DtTestPoint(__DtFunc_FreeBuf, __DtStep_7)
				OSReleaseSubMemHandle(pBuf->hOSMemHandle, ui32Flags);
			}
		}
		if(ui32Flags & PVRSRV_MEM_RAM_BACKED_ALLOCATION)
		{
			
            
			__DtTestPoint(__DtFunc_FreeBuf, __DtStep_8)
			if ((pBuf->ui32ExportCount == 0) && (pBuf->ui32RefCount == 0))
			{
				


				__DtTestPoint(__DtFunc_FreeBuf, __DtStep_9)
				PVR_ASSERT(pBuf->ui32ExportCount == 0)
				RA_Free (pBuf->pMapping->pArena, pBuf->DevVAddr.uiAddr, IMG_FALSE);
			}
		}
		else
		{
			__DtTestPoint(__DtFunc_FreeBuf, __DtStep_10)
			if ((pBuf->ui32ExportCount == 0) && (pBuf->ui32RefCount == 0))
			{
				__DtTestPoint(__DtFunc_FreeBuf, __DtStep_11)
				switch (pMapping->eCpuMemoryOrigin)
				{
					case hm_wrapped:
						__DtTestPoint(__DtFunc_FreeBuf, __DtStep_12)
						OSUnReservePhys(pMapping->CpuVAddr, pMapping->uSize, ui32Flags, pMapping->hOSMemHandle);
						break;
					case hm_wrapped_virtaddr:
						__DtTestPoint(__DtFunc_FreeBuf, __DtStep_13)
						OSUnRegisterMem(pMapping->CpuVAddr, pMapping->uSize, ui32Flags, pMapping->hOSMemHandle);
						break;
					case hm_wrapped_scatter:
						__DtTestPoint(__DtFunc_FreeBuf, __DtStep_14)
						OSUnReserveDiscontigPhys(pMapping->CpuVAddr, pMapping->uSize, ui32Flags, pMapping->hOSMemHandle);
						break;
					case hm_wrapped_scatter_virtaddr:
						__DtTestPoint(__DtFunc_FreeBuf, __DtStep_15)
						OSUnRegisterDiscontigMem(pMapping->CpuVAddr, pMapping->uSize, ui32Flags, pMapping->hOSMemHandle);
						break;
					default:
						__DtTestPoint(__DtFunc_FreeBuf, __DtStep_16)
						break;
				}
			}
			if (bFromAllocator)
				DevMemoryFree (pMapping);

			if ((pBuf->ui32ExportCount == 0) && (pBuf->ui32RefCount == 0))
			{
				
				__DtTestPoint(__DtFunc_FreeBuf, __DtStep_17)
				OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(BM_MAPPING), pMapping, IMG_NULL);
				pBuf->pMapping = IMG_NULL; 
			}
		}
	}


	if ((pBuf->ui32ExportCount == 0) && (pBuf->ui32RefCount == 0))
	{
		__DtTestPoint(__DtFunc_FreeBuf, __DtStep_18)
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(BM_BUF), pBuf, IMG_NULL);
		
	}
	__DtTestPoint(__DtFunc_FreeBuf, __DtStep_19)
}

static PVRSRV_ERROR BM_DestroyContext_AnyCb(BM_HEAP *psBMHeap)
{
	__DtTestPoint(__DtFunc_BM_DestroyContext_AnyCb, __DtStep_0)
	if(psBMHeap->ui32Attribs
	& 	(PVRSRV_BACKINGSTORE_SYSMEM_NONCONTIG
		|PVRSRV_BACKINGSTORE_LOCALMEM_CONTIG))
	{
		__DtTestPoint(__DtFunc_BM_DestroyContext_AnyCb, __DtStep_1)
		if (psBMHeap->pImportArena)
		{
			IMG_BOOL bTestDelete = RA_TestDelete(psBMHeap->pImportArena);
			__DtTestPoint(__DtFunc_BM_DestroyContext_AnyCb, __DtStep_2)
			if (!bTestDelete)
			{
				__DtTestPoint(__DtFunc_BM_DestroyContext_AnyCb, __DtStep_3)
				PVR_DPF ((PVR_DBG_ERROR, "BM_DestroyContext_AnyCb: RA_TestDelete failed"));
				__DtTestPoint(__DtFunc_BM_DestroyContext_AnyCb, __DtStep_4)
				return PVRSRV_ERROR_UNABLE_TO_DESTROY_BM_HEAP;
			}
		}
	}
	__DtTestPoint(__DtFunc_BM_DestroyContext_AnyCb, __DtStep_5)
	return PVRSRV_OK;
}


PVRSRV_ERROR
BM_DestroyContext(IMG_HANDLE	hBMContext,
				  IMG_BOOL		*pbDestroyed)
{
	PVRSRV_ERROR eError;
	BM_CONTEXT *pBMContext = (BM_CONTEXT*)hBMContext;

	__DtTestPoint(__DtFunc_BM_DestroyContext, __DtStep_0)
	PVR_DPF ((PVR_DBG_MESSAGE, "BM_DestroyContext"));

	if (pbDestroyed != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_BM_DestroyContext, __DtStep_1)
		*pbDestroyed = IMG_FALSE;
	}

	

	if (pBMContext == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_BM_DestroyContext, __DtStep_2)
		PVR_DPF ((PVR_DBG_ERROR, "BM_DestroyContext: Invalid handle"));
		__DtTestPoint(__DtFunc_BM_DestroyContext, __DtStep_3)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	pBMContext->ui32RefCount--;

	if (pBMContext->ui32RefCount > 0)
	{
		
		__DtTestPoint(__DtFunc_BM_DestroyContext, __DtStep_4)
		return PVRSRV_OK;
	}

	


	eError = List_BM_HEAP_PVRSRV_ERROR_Any(pBMContext->psBMHeap, &BM_DestroyContext_AnyCb);
	if(eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_BM_DestroyContext, __DtStep_5)
		PVR_DPF ((PVR_DBG_ERROR, "BM_DestroyContext: List_BM_HEAP_PVRSRV_ERROR_Any failed"));
#if 0
		
		
		
		
		PVR_DPF ((PVR_DBG_ERROR, "BM_DestroyContext: Cleaning up with ResManFreeSpecial"));
		if(ResManFreeSpecial() != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_BM_DestroyContext, __DtStep_6)
			PVR_DPF ((PVR_DBG_ERROR, "BM_DestroyContext: ResManFreeSpecial failed %d",eError));
		}
		
#endif
		return eError;
	}
	else
	{
		
		__DtTestPoint(__DtFunc_BM_DestroyContext, __DtStep_7)
		eError = ResManFreeResByPtr(pBMContext->hResItem);
		if(eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_BM_DestroyContext, __DtStep_8)
			PVR_DPF ((PVR_DBG_ERROR, "BM_DestroyContext: ResManFreeResByPtr failed %d",eError));
			__DtTestPoint(__DtFunc_BM_DestroyContext, __DtStep_9)
			return eError;
		}

		
		if (pbDestroyed != IMG_NULL)
		{
			__DtTestPoint(__DtFunc_BM_DestroyContext, __DtStep_10)
			*pbDestroyed = IMG_TRUE;
		}
	}

	__DtTestPoint(__DtFunc_BM_DestroyContext, __DtStep_11)
	return PVRSRV_OK;
}


static PVRSRV_ERROR BM_DestroyContextCallBack_AnyVaCb(BM_HEAP *psBMHeap, va_list va)
{
	PVRSRV_DEVICE_NODE *psDeviceNode;
	__DtTestPoint(__DtFunc_BM_DestroyContextCallBack_AnyVaCb, __DtStep_0)
	psDeviceNode = va_arg(va, PVRSRV_DEVICE_NODE*);

	
	if(psBMHeap->ui32Attribs
	& 	(PVRSRV_BACKINGSTORE_SYSMEM_NONCONTIG
		|PVRSRV_BACKINGSTORE_LOCALMEM_CONTIG))
	{
		__DtTestPoint(__DtFunc_BM_DestroyContextCallBack_AnyVaCb, __DtStep_1)
		if (psBMHeap->pImportArena)
		{
			__DtTestPoint(__DtFunc_BM_DestroyContextCallBack_AnyVaCb, __DtStep_2)
			RA_Delete (psBMHeap->pImportArena);
		}
	}
	else
	{
		__DtTestPoint(__DtFunc_BM_DestroyContextCallBack_AnyVaCb, __DtStep_3)
		PVR_DPF((PVR_DBG_ERROR, "BM_DestroyContext: backing store type unsupported"));
		__DtTestPoint(__DtFunc_BM_DestroyContextCallBack_AnyVaCb, __DtStep_4)
		return PVRSRV_ERROR_UNSUPPORTED_BACKING_STORE;
	}

	
	psDeviceNode->pfnMMUDelete(psBMHeap->pMMUHeap);

	
	OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(BM_HEAP), psBMHeap, IMG_NULL);
	

	__DtTestPoint(__DtFunc_BM_DestroyContextCallBack_AnyVaCb, __DtStep_5)
	return PVRSRV_OK;
}


static PVRSRV_ERROR BM_DestroyContextCallBack(IMG_PVOID		pvParam,
											  IMG_UINT32	ui32Param)
{
	BM_CONTEXT *pBMContext = pvParam;
	PVRSRV_DEVICE_NODE *psDeviceNode;
	PVRSRV_ERROR eError;
	__DtTestPoint(__DtFunc_BM_DestroyContextCallBack, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(ui32Param);

	

	psDeviceNode = pBMContext->psDeviceNode;

	

	eError = List_BM_HEAP_PVRSRV_ERROR_Any_va(pBMContext->psBMHeap,
										&BM_DestroyContextCallBack_AnyVaCb,
										psDeviceNode);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_BM_DestroyContextCallBack, __DtStep_1)
		return eError;
	}
	

	if (pBMContext->psMMUContext)
	{
		__DtTestPoint(__DtFunc_BM_DestroyContextCallBack, __DtStep_2)
		psDeviceNode->pfnMMUFinalise(pBMContext->psMMUContext);
	}

	

	if (pBMContext->pBufferHash)
	{
		__DtTestPoint(__DtFunc_BM_DestroyContextCallBack, __DtStep_3)
		HASH_Delete(pBMContext->pBufferHash);
	}

	if (pBMContext == psDeviceNode->sDevMemoryInfo.pBMKernelContext)
	{
		
		__DtTestPoint(__DtFunc_BM_DestroyContextCallBack, __DtStep_4)
		psDeviceNode->sDevMemoryInfo.pBMKernelContext = IMG_NULL;
	}
	else
	{
		
		__DtTestPoint(__DtFunc_BM_DestroyContextCallBack, __DtStep_5)
		List_BM_CONTEXT_Remove(pBMContext);
	}

	OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(BM_CONTEXT), pBMContext, IMG_NULL);
	

	__DtTestPoint(__DtFunc_BM_DestroyContextCallBack, __DtStep_6)
	return PVRSRV_OK;
}


static IMG_HANDLE BM_CreateContext_IncRefCount_AnyVaCb(BM_CONTEXT *pBMContext, va_list va)
{
	PRESMAN_CONTEXT	hResManContext;
	__DtTestPoint(__DtFunc_BM_CreateContext_IncRefCount_AnyVaCb, __DtStep_0)
	hResManContext = va_arg(va, PRESMAN_CONTEXT);
	if(ResManFindResourceByPtr(hResManContext, pBMContext->hResItem) == PVRSRV_OK)
	{
		
		__DtTestPoint(__DtFunc_BM_CreateContext_IncRefCount_AnyVaCb, __DtStep_1)
		pBMContext->ui32RefCount++;
		__DtTestPoint(__DtFunc_BM_CreateContext_IncRefCount_AnyVaCb, __DtStep_2)
		return pBMContext;
	}
	__DtTestPoint(__DtFunc_BM_CreateContext_IncRefCount_AnyVaCb, __DtStep_3)
	return IMG_NULL;
}

static IMG_VOID BM_CreateContext_InsertHeap_ForEachVaCb(BM_HEAP *psBMHeap, va_list va)
{
	PVRSRV_DEVICE_NODE *psDeviceNode;
	BM_CONTEXT *pBMContext;
	__DtTestPoint(__DtFunc_BM_CreateContext_InsertHeap_ForEachVaCb, __DtStep_0)
	psDeviceNode = va_arg(va, PVRSRV_DEVICE_NODE*);
	pBMContext = va_arg(va, BM_CONTEXT*);
	switch(psBMHeap->sDevArena.DevMemHeapType)
	{
		case DEVICE_MEMORY_HEAP_SHARED:
		case DEVICE_MEMORY_HEAP_SHARED_EXPORTED:
		{
			
			__DtTestPoint(__DtFunc_BM_CreateContext_InsertHeap_ForEachVaCb, __DtStep_1)
			psDeviceNode->pfnMMUInsertHeap(pBMContext->psMMUContext, psBMHeap->pMMUHeap);
			break;
		}
	}
	__DtTestPoint(__DtFunc_BM_CreateContext_InsertHeap_ForEachVaCb, __DtStep_2)
}

IMG_HANDLE
BM_CreateContext(PVRSRV_DEVICE_NODE			*psDeviceNode,
				 IMG_DEV_PHYADDR			*psPDDevPAddr,
				 PVRSRV_PER_PROCESS_DATA	*psPerProc,
				 IMG_BOOL					*pbCreated)
{
	BM_CONTEXT			*pBMContext;
	DEVICE_MEMORY_INFO	*psDevMemoryInfo;
	IMG_BOOL			bKernelContext;
	PRESMAN_CONTEXT		hResManContext;

	__DtTestPoint(__DtFunc_BM_CreateContext, __DtStep_0)
	PVR_DPF((PVR_DBG_MESSAGE, "BM_CreateContext"));

	if (psPerProc == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_BM_CreateContext, __DtStep_1)
		bKernelContext = IMG_TRUE;
		hResManContext = psDeviceNode->hResManContext;
	}
	else
	{
		__DtTestPoint(__DtFunc_BM_CreateContext, __DtStep_2)
		bKernelContext = IMG_FALSE;
		hResManContext = psPerProc->hResManContext;
	}

	if (pbCreated != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_BM_CreateContext, __DtStep_3)
		*pbCreated = IMG_FALSE;
	}

	
	psDevMemoryInfo = &psDeviceNode->sDevMemoryInfo;

	if (bKernelContext == IMG_FALSE)
	{
		IMG_HANDLE res = (IMG_HANDLE) List_BM_CONTEXT_Any_va(psDevMemoryInfo->pBMContext,
															&BM_CreateContext_IncRefCount_AnyVaCb,
															hResManContext);
		__DtTestPoint(__DtFunc_BM_CreateContext, __DtStep_4)
		if (res)
		{
			__DtTestPoint(__DtFunc_BM_CreateContext, __DtStep_5)
			return res;
		}
	}

	
	if (OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP,
					 sizeof (struct _BM_CONTEXT_),
					 (IMG_PVOID *)&pBMContext, IMG_NULL,
					 "Buffer Manager Context") != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_BM_CreateContext, __DtStep_6)
		PVR_DPF ((PVR_DBG_ERROR, "BM_CreateContext: Alloc failed"));
		__DtTestPoint(__DtFunc_BM_CreateContext, __DtStep_7)
		return IMG_NULL;
	}
	OSMemSet(pBMContext, 0, sizeof (BM_CONTEXT));

	
	pBMContext->psDeviceNode = psDeviceNode;

	
	
	pBMContext->pBufferHash = HASH_Create(32);
	if (pBMContext->pBufferHash==IMG_NULL)
	{
		__DtTestPoint(__DtFunc_BM_CreateContext, __DtStep_8)
		PVR_DPF ((PVR_DBG_ERROR, "BM_CreateContext: HASH_Create failed"));
		goto cleanup;
	}

	if(psDeviceNode->pfnMMUInitialise(psDeviceNode,
										&pBMContext->psMMUContext,
										psPDDevPAddr) != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_BM_CreateContext, __DtStep_9)
		PVR_DPF((PVR_DBG_ERROR, "BM_CreateContext: MMUInitialise failed"));
		goto cleanup;
	}

	if(bKernelContext)
	{
		
		__DtTestPoint(__DtFunc_BM_CreateContext, __DtStep_10)
		PVR_ASSERT(psDevMemoryInfo->pBMKernelContext == IMG_NULL);
		psDevMemoryInfo->pBMKernelContext = pBMContext;
	}
	else
	{
		




		__DtTestPoint(__DtFunc_BM_CreateContext, __DtStep_11)
		PVR_ASSERT(psDevMemoryInfo->pBMKernelContext);

		if (psDevMemoryInfo->pBMKernelContext == IMG_NULL)
		{
			__DtTestPoint(__DtFunc_BM_CreateContext, __DtStep_12)
			PVR_DPF((PVR_DBG_ERROR, "BM_CreateContext: psDevMemoryInfo->pBMKernelContext invalid"));
			goto cleanup;
		}

		PVR_ASSERT(psDevMemoryInfo->pBMKernelContext->psBMHeap);

		



		pBMContext->psBMSharedHeap = psDevMemoryInfo->pBMKernelContext->psBMHeap;

		


		List_BM_HEAP_ForEach_va(pBMContext->psBMSharedHeap,
								&BM_CreateContext_InsertHeap_ForEachVaCb,
								psDeviceNode,
								pBMContext);

		
		List_BM_CONTEXT_Insert(&psDevMemoryInfo->pBMContext, pBMContext);
	}

	
	pBMContext->ui32RefCount++;

	
	pBMContext->hResItem = ResManRegisterRes(hResManContext,
											RESMAN_TYPE_DEVICEMEM_CONTEXT,
											pBMContext,
											0,
											&BM_DestroyContextCallBack);
	if (pBMContext->hResItem == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_BM_CreateContext, __DtStep_13)
		PVR_DPF ((PVR_DBG_ERROR, "BM_CreateContext: ResManRegisterRes failed"));
		goto cleanup;
	}

	if (pbCreated != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_BM_CreateContext, __DtStep_14)
		*pbCreated = IMG_TRUE;
	}
	__DtTestPoint(__DtFunc_BM_CreateContext, __DtStep_15)
	return (IMG_HANDLE)pBMContext;

cleanup:
	(IMG_VOID)BM_DestroyContextCallBack(pBMContext, 0);

	__DtTestPoint(__DtFunc_BM_CreateContext, __DtStep_16)
	return IMG_NULL;
}


static IMG_VOID *BM_CreateHeap_AnyVaCb(BM_HEAP *psBMHeap, va_list va)
{
	DEVICE_MEMORY_HEAP_INFO *psDevMemHeapInfo;
	__DtTestPoint(__DtFunc_BM_CreateHeap_AnyVaCb, __DtStep_0)
	psDevMemHeapInfo = va_arg(va, DEVICE_MEMORY_HEAP_INFO*);
	if (psBMHeap->sDevArena.ui32HeapID ==  psDevMemHeapInfo->ui32HeapID)
	{
		
		__DtTestPoint(__DtFunc_BM_CreateHeap_AnyVaCb, __DtStep_1)
		return psBMHeap;
	}
	else
	{
		__DtTestPoint(__DtFunc_BM_CreateHeap_AnyVaCb, __DtStep_2)
		return IMG_NULL;
	}
	__DtTestPoint(__DtFunc_BM_CreateHeap_AnyVaCb, __DtStep_3)
}

IMG_HANDLE
BM_CreateHeap (IMG_HANDLE hBMContext,
			   DEVICE_MEMORY_HEAP_INFO *psDevMemHeapInfo)
{
	BM_CONTEXT *pBMContext = (BM_CONTEXT*)hBMContext;
	PVRSRV_DEVICE_NODE *psDeviceNode;
	BM_HEAP *psBMHeap;

	__DtTestPoint(__DtFunc_BM_CreateHeap, __DtStep_0)
	PVR_DPF((PVR_DBG_MESSAGE, "BM_CreateHeap"));

	if(!pBMContext)
	{
		__DtTestPoint(__DtFunc_BM_CreateHeap, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "BM_CreateHeap: BM_CONTEXT null"));
		__DtTestPoint(__DtFunc_BM_CreateHeap, __DtStep_2)
		return IMG_NULL;
	}

	psDeviceNode = pBMContext->psDeviceNode;

	




	if(pBMContext->ui32RefCount > 0)
	{
		__DtTestPoint(__DtFunc_BM_CreateHeap, __DtStep_3)
		psBMHeap = (BM_HEAP*)List_BM_HEAP_Any_va(pBMContext->psBMHeap,
												 &BM_CreateHeap_AnyVaCb,
												 psDevMemHeapInfo);

		if (psBMHeap)
		{
			__DtTestPoint(__DtFunc_BM_CreateHeap, __DtStep_4)
			return psBMHeap;
		}
	}


	if (OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP,
						sizeof (BM_HEAP),
						(IMG_PVOID *)&psBMHeap, IMG_NULL,
						"Buffer Manager Heap") != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_BM_CreateHeap, __DtStep_5)
		PVR_DPF((PVR_DBG_ERROR, "BM_CreateHeap: Alloc failed"));
		__DtTestPoint(__DtFunc_BM_CreateHeap, __DtStep_6)
		return IMG_NULL;
	}

	OSMemSet (psBMHeap, 0, sizeof (BM_HEAP));

	psBMHeap->sDevArena.ui32HeapID = psDevMemHeapInfo->ui32HeapID;
	psBMHeap->sDevArena.pszName = psDevMemHeapInfo->pszName;
	psBMHeap->sDevArena.BaseDevVAddr = psDevMemHeapInfo->sDevVAddrBase;
	psBMHeap->sDevArena.ui32Size = psDevMemHeapInfo->ui32HeapSize;
	psBMHeap->sDevArena.DevMemHeapType = psDevMemHeapInfo->DevMemHeapType;
	psBMHeap->sDevArena.ui32DataPageSize = psDevMemHeapInfo->ui32DataPageSize;
	psBMHeap->sDevArena.psDeviceMemoryHeapInfo = psDevMemHeapInfo;
	psBMHeap->ui32Attribs = psDevMemHeapInfo->ui32Attribs;

	
	psBMHeap->pBMContext = pBMContext;

	psBMHeap->pMMUHeap = psDeviceNode->pfnMMUCreate (pBMContext->psMMUContext,
													&psBMHeap->sDevArena,
													&psBMHeap->pVMArena,
													&psBMHeap->psMMUAttrib);
	if (!psBMHeap->pMMUHeap)
	{
		__DtTestPoint(__DtFunc_BM_CreateHeap, __DtStep_7)
		PVR_DPF((PVR_DBG_ERROR, "BM_CreateHeap: MMUCreate failed"));
		goto ErrorExit;
	}

	
	psBMHeap->pImportArena = RA_Create (psDevMemHeapInfo->pszBSName,
										0, 0, IMG_NULL,
										MAX(HOST_PAGESIZE(), psBMHeap->sDevArena.ui32DataPageSize),
										&BM_ImportMemory,
										&BM_FreeMemory,
										IMG_NULL,
										psBMHeap);
	if(psBMHeap->pImportArena == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_BM_CreateHeap, __DtStep_8)
		PVR_DPF((PVR_DBG_ERROR, "BM_CreateHeap: RA_Create failed"));
		goto ErrorExit;
	}

	if(psBMHeap->ui32Attribs & PVRSRV_BACKINGSTORE_LOCALMEM_CONTIG)
	{
		



		__DtTestPoint(__DtFunc_BM_CreateHeap, __DtStep_9)
		psBMHeap->pLocalDevMemArena = psDevMemHeapInfo->psLocalDevMemArena;
		if(psBMHeap->pLocalDevMemArena == IMG_NULL)
		{
			__DtTestPoint(__DtFunc_BM_CreateHeap, __DtStep_10)
			PVR_DPF((PVR_DBG_ERROR, "BM_CreateHeap: LocalDevMemArena null"));
			goto ErrorExit;
		}
	}

	
	List_BM_HEAP_Insert(&pBMContext->psBMHeap, psBMHeap);

	__DtTestPoint(__DtFunc_BM_CreateHeap, __DtStep_11)
	return (IMG_HANDLE)psBMHeap;

	
ErrorExit:

	
	if (psBMHeap->pMMUHeap != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_BM_CreateHeap, __DtStep_12)
		psDeviceNode->pfnMMUDelete (psBMHeap->pMMUHeap);
		psDeviceNode->pfnMMUFinalise (pBMContext->psMMUContext);
	}

	
	OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(BM_HEAP), psBMHeap, IMG_NULL);
	

	__DtTestPoint(__DtFunc_BM_CreateHeap, __DtStep_13)
	return IMG_NULL;
}

IMG_VOID
BM_DestroyHeap (IMG_HANDLE hDevMemHeap)
{
	BM_HEAP* psBMHeap = (BM_HEAP*)hDevMemHeap;
	PVRSRV_DEVICE_NODE *psDeviceNode = psBMHeap->pBMContext->psDeviceNode;

	__DtTestPoint(__DtFunc_BM_DestroyHeap, __DtStep_0)
	PVR_DPF((PVR_DBG_MESSAGE, "BM_DestroyHeap"));

	if(psBMHeap)
	{
		
		__DtTestPoint(__DtFunc_BM_DestroyHeap, __DtStep_1)
		if(psBMHeap->ui32Attribs
		&	(PVRSRV_BACKINGSTORE_SYSMEM_NONCONTIG
			|PVRSRV_BACKINGSTORE_LOCALMEM_CONTIG))
		{
			__DtTestPoint(__DtFunc_BM_DestroyHeap, __DtStep_2)
			if (psBMHeap->pImportArena)
			{
				__DtTestPoint(__DtFunc_BM_DestroyHeap, __DtStep_3)
				RA_Delete (psBMHeap->pImportArena);
			}
		}
		else
		{
			__DtTestPoint(__DtFunc_BM_DestroyHeap, __DtStep_4)
			PVR_DPF((PVR_DBG_ERROR, "BM_DestroyHeap: backing store type unsupported"));
			__DtTestPoint(__DtFunc_BM_DestroyHeap, __DtStep_5)
			return;
		}

		
		psDeviceNode->pfnMMUDelete (psBMHeap->pMMUHeap);

		
		List_BM_HEAP_Remove(psBMHeap);
		
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(BM_HEAP), psBMHeap, IMG_NULL);
		
	}
	else
	{
		__DtTestPoint(__DtFunc_BM_DestroyHeap, __DtStep_6)
		PVR_DPF ((PVR_DBG_ERROR, "BM_DestroyHeap: invalid heap handle"));
	}
	__DtTestPoint(__DtFunc_BM_DestroyHeap, __DtStep_7)
}


IMG_BOOL
BM_Reinitialise (PVRSRV_DEVICE_NODE *psDeviceNode)
{

	__DtTestPoint(__DtFunc_BM_Reinitialise, __DtStep_0)
	PVR_DPF((PVR_DBG_MESSAGE, "BM_Reinitialise"));
	PVR_UNREFERENCED_PARAMETER(psDeviceNode);


	__DtTestPoint(__DtFunc_BM_Reinitialise, __DtStep_1)
	return IMG_TRUE;
}

IMG_BOOL
BM_Alloc (  IMG_HANDLE			hDevMemHeap,
			IMG_DEV_VIRTADDR	*psDevVAddr,
			IMG_SIZE_T			uSize,
			IMG_UINT32			*pui32Flags,
			IMG_UINT32			uDevVAddrAlignment,
			BM_HANDLE			*phBuf)
{
	BM_BUF *pBuf;
	BM_CONTEXT *pBMContext;
	BM_HEAP *psBMHeap;
	SYS_DATA *psSysData;
	IMG_UINT32 uFlags;

	__DtTestPoint(__DtFunc_BM_Alloc, __DtStep_0)
	if (pui32Flags == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_BM_Alloc, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "BM_Alloc: invalid parameter"));
		PVR_DBG_BREAK;
		__DtTestPoint(__DtFunc_BM_Alloc, __DtStep_2)
		return IMG_FALSE;
	}

	uFlags = *pui32Flags;

	PVR_DPF ((PVR_DBG_MESSAGE,
		  "BM_Alloc (uSize=0x%x, uFlags=0x%x, uDevVAddrAlignment=0x%x)",
			uSize, uFlags, uDevVAddrAlignment));

	SysAcquireData(&psSysData);

	psBMHeap = (BM_HEAP*)hDevMemHeap;
	pBMContext = psBMHeap->pBMContext;

	if(uDevVAddrAlignment == 0)
	{
		__DtTestPoint(__DtFunc_BM_Alloc, __DtStep_3)
		uDevVAddrAlignment = 1;
	}

	
	if (OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP,
				   sizeof (BM_BUF),
				   (IMG_PVOID *)&pBuf, IMG_NULL,
				   "Buffer Manager buffer") != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_BM_Alloc, __DtStep_4)
		PVR_DPF((PVR_DBG_ERROR, "BM_Alloc: BM_Buf alloc FAILED"));
		__DtTestPoint(__DtFunc_BM_Alloc, __DtStep_5)
		return IMG_FALSE;
	}
	OSMemSet(pBuf, 0, sizeof (BM_BUF));

	
	if (AllocMemory(pBMContext,
					psBMHeap,
					psDevVAddr,
					uSize,
					uFlags,
					uDevVAddrAlignment,
					pBuf) != IMG_TRUE)
	{
		__DtTestPoint(__DtFunc_BM_Alloc, __DtStep_6)
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof (BM_BUF), pBuf, IMG_NULL);
		
		PVR_DPF((PVR_DBG_ERROR, "BM_Alloc: AllocMemory FAILED"));
		__DtTestPoint(__DtFunc_BM_Alloc, __DtStep_7)
		return IMG_FALSE;
	}

	PVR_DPF ((PVR_DBG_MESSAGE,
		  "BM_Alloc (uSize=0x%x, uFlags=0x%x)",
		  uSize, uFlags));

	
	pBuf->ui32RefCount = 1;
	*phBuf = (BM_HANDLE)pBuf;
	*pui32Flags = uFlags | psBMHeap->ui32Attribs;

	
	if(uFlags & PVRSRV_HAP_CACHETYPE_MASK)
	{
		__DtTestPoint(__DtFunc_BM_Alloc, __DtStep_8)
		*pui32Flags &= ~PVRSRV_HAP_CACHETYPE_MASK;
		*pui32Flags |= (uFlags & PVRSRV_HAP_CACHETYPE_MASK);
	}

	__DtTestPoint(__DtFunc_BM_Alloc, __DtStep_9)
	return IMG_TRUE;
}



#if defined(PVR_LMA)
static IMG_BOOL
ValidSysPAddrArrayForDev(PVRSRV_DEVICE_NODE *psDeviceNode, IMG_SYS_PHYADDR *psSysPAddr, IMG_UINT32 ui32PageCount, IMG_SIZE_T ui32PageSize)
{
	IMG_UINT32 i;

	__DtTestPoint(__DtFunc_ValidSysPAddrArrayForDev, __DtStep_0)
	for (i = 0; i < ui32PageCount; i++)
	{
		IMG_SYS_PHYADDR sStartSysPAddr = psSysPAddr[i];
		IMG_SYS_PHYADDR sEndSysPAddr;

		__DtTestPoint(__DtFunc_ValidSysPAddrArrayForDev, __DtStep_1)
		if (!SysVerifySysPAddrToDevPAddr(psDeviceNode->sDevId.eDeviceType, sStartSysPAddr))
		{
			__DtTestPoint(__DtFunc_ValidSysPAddrArrayForDev, __DtStep_2)
			return IMG_FALSE;
		}

		sEndSysPAddr.uiAddr = sStartSysPAddr.uiAddr + ui32PageSize;

		if (!SysVerifySysPAddrToDevPAddr(psDeviceNode->sDevId.eDeviceType, sEndSysPAddr))
		{
			__DtTestPoint(__DtFunc_ValidSysPAddrArrayForDev, __DtStep_3)
			return IMG_FALSE;
		}
	}

	__DtTestPoint(__DtFunc_ValidSysPAddrArrayForDev, __DtStep_4)
	return IMG_TRUE;
}

static IMG_BOOL
ValidSysPAddrRangeForDev(PVRSRV_DEVICE_NODE *psDeviceNode, IMG_SYS_PHYADDR sStartSysPAddr, IMG_SIZE_T ui32Range)
{
	IMG_SYS_PHYADDR sEndSysPAddr;

	__DtTestPoint(__DtFunc_ValidSysPAddrRangeForDev, __DtStep_0)
	if (!SysVerifySysPAddrToDevPAddr(psDeviceNode->sDevId.eDeviceType, sStartSysPAddr))
	{
		__DtTestPoint(__DtFunc_ValidSysPAddrRangeForDev, __DtStep_1)
		return IMG_FALSE;
	}

	sEndSysPAddr.uiAddr = sStartSysPAddr.uiAddr + ui32Range;

	if (!SysVerifySysPAddrToDevPAddr(psDeviceNode->sDevId.eDeviceType, sEndSysPAddr))
	{
		__DtTestPoint(__DtFunc_ValidSysPAddrRangeForDev, __DtStep_2)
		return IMG_FALSE;
	}

	__DtTestPoint(__DtFunc_ValidSysPAddrRangeForDev, __DtStep_3)
	return IMG_TRUE;
}

#define	WRAP_MAPPING_SIZE(ui32ByteSize, ui32PageOffset) HOST_PAGEALIGN((ui32ByteSize) + (ui32PageOffset))

#define	WRAP_PAGE_COUNT(ui32ByteSize, ui32PageOffset, ui32HostPageSize)	(WRAP_MAPPING_SIZE(ui32ByteSize, ui32PageOffset) / (ui32HostPageSize))

#endif


IMG_BOOL
BM_Wrap (	IMG_HANDLE hDevMemHeap,
			IMG_SIZE_T ui32Size,
			IMG_SIZE_T ui32Offset,
			IMG_BOOL bPhysContig,
			IMG_SYS_PHYADDR *psSysAddr,
			IMG_VOID *pvCPUVAddr,
			IMG_UINT32 *pui32Flags,
			BM_HANDLE *phBuf)
{
	BM_BUF *pBuf;
	BM_CONTEXT *psBMContext;
	BM_HEAP *psBMHeap;
	SYS_DATA *psSysData;
	IMG_SYS_PHYADDR sHashAddress;
	IMG_UINT32 uFlags;

	__DtTestPoint(__DtFunc_BM_Wrap, __DtStep_0)
	psBMHeap = (BM_HEAP*)hDevMemHeap;
	psBMContext = psBMHeap->pBMContext;

	uFlags = psBMHeap->ui32Attribs & (PVRSRV_HAP_CACHETYPE_MASK | PVRSRV_HAP_MAPTYPE_MASK);

	if ((pui32Flags != IMG_NULL) && ((*pui32Flags & PVRSRV_HAP_CACHETYPE_MASK) != 0))
	{
		__DtTestPoint(__DtFunc_BM_Wrap, __DtStep_1)
		uFlags &= ~PVRSRV_HAP_CACHETYPE_MASK;
		uFlags |= *pui32Flags & PVRSRV_HAP_CACHETYPE_MASK;
	}

	PVR_DPF ((PVR_DBG_MESSAGE,
		  "BM_Wrap (uSize=0x%x, uOffset=0x%x, bPhysContig=0x%x, pvCPUVAddr=0x%x, uFlags=0x%x)",
			ui32Size, ui32Offset, bPhysContig, (IMG_UINTPTR_T)pvCPUVAddr, uFlags));

	SysAcquireData(&psSysData);

#if defined(PVR_LMA)
	if (bPhysContig)
	{
		__DtTestPoint(__DtFunc_BM_Wrap, __DtStep_2)
		if (!ValidSysPAddrRangeForDev(psBMContext->psDeviceNode, *psSysAddr, WRAP_MAPPING_SIZE(ui32Size, ui32Offset)))
		{
			__DtTestPoint(__DtFunc_BM_Wrap, __DtStep_3)
			PVR_DPF((PVR_DBG_ERROR, "BM_Wrap: System address range invalid for device"));
			__DtTestPoint(__DtFunc_BM_Wrap, __DtStep_4)
			return IMG_FALSE;
		}
	}
	else
	{
		IMG_SIZE_T ui32HostPageSize = HOST_PAGESIZE();

		__DtTestPoint(__DtFunc_BM_Wrap, __DtStep_5)
		if (!ValidSysPAddrArrayForDev(psBMContext->psDeviceNode, psSysAddr, WRAP_PAGE_COUNT(ui32Size, ui32Offset, ui32HostPageSize), ui32HostPageSize))
		{
			__DtTestPoint(__DtFunc_BM_Wrap, __DtStep_6)
			PVR_DPF((PVR_DBG_ERROR, "BM_Wrap: Array of system addresses invalid for device"));
			__DtTestPoint(__DtFunc_BM_Wrap, __DtStep_7)
			return IMG_FALSE;
		}
	}
#endif
	
	sHashAddress = psSysAddr[0];

	
	sHashAddress.uiAddr += ui32Offset;

	
	pBuf = (BM_BUF *)HASH_Retrieve(psBMContext->pBufferHash, sHashAddress.uiAddr);

	if(pBuf)
	{
		IMG_SIZE_T ui32MappingSize = HOST_PAGEALIGN (ui32Size + ui32Offset);

		
		__DtTestPoint(__DtFunc_BM_Wrap, __DtStep_8)
		if(pBuf->pMapping->uSize == ui32MappingSize && (pBuf->pMapping->eCpuMemoryOrigin == hm_wrapped ||
														pBuf->pMapping->eCpuMemoryOrigin == hm_wrapped_virtaddr))
		{
			__DtTestPoint(__DtFunc_BM_Wrap, __DtStep_9)
			PVR_DPF((PVR_DBG_MESSAGE,
					"BM_Wrap (Matched previous Wrap! uSize=0x%x, uOffset=0x%x, SysAddr=%08X)",
					ui32Size, ui32Offset, sHashAddress.uiAddr));

			pBuf->ui32RefCount++;
			*phBuf = (BM_HANDLE)pBuf;
			if(pui32Flags)
				*pui32Flags = uFlags;

			__DtTestPoint(__DtFunc_BM_Wrap, __DtStep_10)
			return IMG_TRUE;
		}
	}

	
	if (OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP,
						sizeof (BM_BUF),
						(IMG_PVOID *)&pBuf, IMG_NULL,
						"Buffer Manager buffer") != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_BM_Wrap, __DtStep_11)
		PVR_DPF((PVR_DBG_ERROR, "BM_Wrap: BM_Buf alloc FAILED"));
		__DtTestPoint(__DtFunc_BM_Wrap, __DtStep_12)
		return IMG_FALSE;
	}
	OSMemSet(pBuf, 0, sizeof (BM_BUF));

	
	if (WrapMemory (psBMHeap, ui32Size, ui32Offset, bPhysContig, psSysAddr, pvCPUVAddr, uFlags, pBuf) != IMG_TRUE)
	{
		__DtTestPoint(__DtFunc_BM_Wrap, __DtStep_13)
		PVR_DPF((PVR_DBG_ERROR, "BM_Wrap: WrapMemory FAILED"));
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof (BM_BUF), pBuf, IMG_NULL);
		
		__DtTestPoint(__DtFunc_BM_Wrap, __DtStep_14)
		return IMG_FALSE;
	}

	
	if(pBuf->pMapping->eCpuMemoryOrigin == hm_wrapped || pBuf->pMapping->eCpuMemoryOrigin == hm_wrapped_virtaddr)
	{
		
		__DtTestPoint(__DtFunc_BM_Wrap, __DtStep_15)
		PVR_ASSERT(SysSysPAddrToCpuPAddr(sHashAddress).uiAddr == pBuf->CpuPAddr.uiAddr);

		if (!HASH_Insert (psBMContext->pBufferHash, sHashAddress.uiAddr, (IMG_UINTPTR_T)pBuf))
		{
			__DtTestPoint(__DtFunc_BM_Wrap, __DtStep_16)
			FreeBuf (pBuf, uFlags, IMG_TRUE);
			PVR_DPF((PVR_DBG_ERROR, "BM_Wrap: HASH_Insert FAILED"));
			__DtTestPoint(__DtFunc_BM_Wrap, __DtStep_17)
			return IMG_FALSE;
		}
	}

	PVR_DPF ((PVR_DBG_MESSAGE,
			"BM_Wrap (uSize=0x%x, uFlags=0x%x, devVAddr=%08X)",
			ui32Size, uFlags, pBuf->DevVAddr.uiAddr));

	
	pBuf->ui32RefCount = 1;
	*phBuf = (BM_HANDLE)pBuf;
	if(pui32Flags)
	{
		
		__DtTestPoint(__DtFunc_BM_Wrap, __DtStep_18)
		*pui32Flags = (uFlags & ~PVRSRV_HAP_MAPTYPE_MASK) | PVRSRV_HAP_MULTI_PROCESS;
	}

	__DtTestPoint(__DtFunc_BM_Wrap, __DtStep_19)
	return IMG_TRUE;
}

IMG_VOID
BM_Export (BM_HANDLE hBuf)
{
	BM_BUF *pBuf = (BM_BUF *)hBuf;

	__DtTestPoint(__DtFunc_BM_Export, __DtStep_0)
	pBuf->ui32ExportCount++;
	__DtTestPoint(__DtFunc_BM_Export, __DtStep_1)
}

IMG_VOID
BM_FreeExport(BM_HANDLE hBuf,
		IMG_UINT32 ui32Flags)
{
	BM_BUF *pBuf = (BM_BUF *)hBuf;

	__DtTestPoint(__DtFunc_BM_FreeExport, __DtStep_0)
	pBuf->ui32ExportCount--;
	FreeBuf (pBuf, ui32Flags, IMG_FALSE);
	__DtTestPoint(__DtFunc_BM_FreeExport, __DtStep_1)
}

IMG_VOID
BM_Free (BM_HANDLE hBuf,
		IMG_UINT32 ui32Flags)
{
	BM_BUF *pBuf = (BM_BUF *)hBuf;
	SYS_DATA *psSysData;
	IMG_SYS_PHYADDR sHashAddr;

	__DtTestPoint(__DtFunc_BM_Free, __DtStep_0)
	PVR_DPF ((PVR_DBG_MESSAGE, "BM_Free (h=0x%x)", (IMG_UINTPTR_T)hBuf));
	PVR_ASSERT (pBuf!=IMG_NULL);

	if (pBuf == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_BM_Free, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "BM_Free: invalid parameter"));
		__DtTestPoint(__DtFunc_BM_Free, __DtStep_2)
		return;
	}

	SysAcquireData(&psSysData);

	pBuf->ui32RefCount--;

	if(pBuf->ui32RefCount == 0)
	{
		__DtTestPoint(__DtFunc_BM_Free, __DtStep_3)
		if(pBuf->pMapping->eCpuMemoryOrigin == hm_wrapped || pBuf->pMapping->eCpuMemoryOrigin == hm_wrapped_virtaddr)
		{
			__DtTestPoint(__DtFunc_BM_Free, __DtStep_4)
			sHashAddr = SysCpuPAddrToSysPAddr(pBuf->CpuPAddr);

			HASH_Remove (pBuf->pMapping->pBMHeap->pBMContext->pBufferHash,	(IMG_UINTPTR_T)sHashAddr.uiAddr);
		}
		FreeBuf (pBuf, ui32Flags, IMG_TRUE);
	}
	__DtTestPoint(__DtFunc_BM_Free, __DtStep_5)
}


IMG_CPU_VIRTADDR
BM_HandleToCpuVaddr (BM_HANDLE hBuf)
{
	BM_BUF *pBuf = (BM_BUF *)hBuf;

	__DtTestPoint(__DtFunc_BM_HandleToCpuVaddr, __DtStep_0)
	PVR_ASSERT (pBuf != IMG_NULL);
	if (pBuf == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_BM_HandleToCpuVaddr, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "BM_HandleToCpuVaddr: invalid parameter"));
		__DtTestPoint(__DtFunc_BM_HandleToCpuVaddr, __DtStep_2)
		return IMG_NULL;
	}

	PVR_DPF ((PVR_DBG_MESSAGE,
				"BM_HandleToCpuVaddr(h=0x%x)=0x%x",
				(IMG_UINTPTR_T)hBuf, (IMG_UINTPTR_T)pBuf->CpuVAddr));
	__DtTestPoint(__DtFunc_BM_HandleToCpuVaddr, __DtStep_3)
	return pBuf->CpuVAddr;
}


IMG_DEV_VIRTADDR
BM_HandleToDevVaddr (BM_HANDLE hBuf)
{
	BM_BUF *pBuf = (BM_BUF *)hBuf;

	__DtTestPoint(__DtFunc_BM_HandleToDevVaddr, __DtStep_0)
	PVR_ASSERT (pBuf != IMG_NULL);
	if (pBuf == IMG_NULL)
	{
		IMG_DEV_VIRTADDR	DevVAddr = {0};
		__DtTestPoint(__DtFunc_BM_HandleToDevVaddr, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "BM_HandleToDevVaddr: invalid parameter"));
		__DtTestPoint(__DtFunc_BM_HandleToDevVaddr, __DtStep_2)
		return DevVAddr;
	}

	PVR_DPF ((PVR_DBG_MESSAGE, "BM_HandleToDevVaddr(h=0x%x)=%08X", (IMG_UINTPTR_T)hBuf, pBuf->DevVAddr.uiAddr));
	__DtTestPoint(__DtFunc_BM_HandleToDevVaddr, __DtStep_3)
	return pBuf->DevVAddr;
}


IMG_SYS_PHYADDR
BM_HandleToSysPaddr (BM_HANDLE hBuf)
{
	BM_BUF *pBuf = (BM_BUF *)hBuf;

	__DtTestPoint(__DtFunc_BM_HandleToSysPaddr, __DtStep_0)
	PVR_ASSERT (pBuf != IMG_NULL);

	if (pBuf == IMG_NULL)
	{
		IMG_SYS_PHYADDR	PhysAddr = {0};
		__DtTestPoint(__DtFunc_BM_HandleToSysPaddr, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "BM_HandleToSysPaddr: invalid parameter"));
		__DtTestPoint(__DtFunc_BM_HandleToSysPaddr, __DtStep_2)
		return PhysAddr;
	}

	PVR_DPF ((PVR_DBG_MESSAGE, "BM_HandleToSysPaddr(h=0x%x)=%08X", (IMG_UINTPTR_T)hBuf, pBuf->CpuPAddr.uiAddr));
	__DtTestPoint(__DtFunc_BM_HandleToSysPaddr, __DtStep_3)
	return SysCpuPAddrToSysPAddr (pBuf->CpuPAddr);
}

IMG_HANDLE
BM_HandleToOSMemHandle(BM_HANDLE hBuf)
{
	BM_BUF *pBuf = (BM_BUF *)hBuf;

	__DtTestPoint(__DtFunc_BM_HandleToOSMemHandle, __DtStep_0)
	PVR_ASSERT (pBuf != IMG_NULL);

	if (pBuf == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_BM_HandleToOSMemHandle, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "BM_HandleToOSMemHandle: invalid parameter"));
		__DtTestPoint(__DtFunc_BM_HandleToOSMemHandle, __DtStep_2)
		return IMG_NULL;
	}

	PVR_DPF ((PVR_DBG_MESSAGE,
				"BM_HandleToOSMemHandle(h=0x%x)=0x%x",
				(IMG_UINTPTR_T)hBuf, (IMG_UINTPTR_T)pBuf->hOSMemHandle));
	__DtTestPoint(__DtFunc_BM_HandleToOSMemHandle, __DtStep_3)
	return pBuf->hOSMemHandle;
}

static IMG_BOOL
DevMemoryAlloc (BM_CONTEXT *pBMContext,
				BM_MAPPING *pMapping,
				IMG_SIZE_T *pActualSize,
				IMG_UINT32 uFlags,
				IMG_UINT32 dev_vaddr_alignment,
				IMG_DEV_VIRTADDR *pDevVAddr)
{
	PVRSRV_DEVICE_NODE *psDeviceNode;
#ifdef PDUMP
	IMG_UINT32 ui32PDumpSize = pMapping->uSize;
#endif

	psDeviceNode = pBMContext->psDeviceNode;

	if(uFlags & PVRSRV_MEM_INTERLEAVED)
	{
		
		__DtTestPoint(__DtFunc_DevMemoryAlloc, __DtStep_0)
		pMapping->uSize *= 2;
	}

#ifdef PDUMP
	if(uFlags & PVRSRV_MEM_DUMMY)
	{
		
		__DtTestPoint(__DtFunc_DevMemoryAlloc, __DtStep_1)
		ui32PDumpSize = pMapping->pBMHeap->sDevArena.ui32DataPageSize;
	}
#endif

	
	if (!psDeviceNode->pfnMMUAlloc (pMapping->pBMHeap->pMMUHeap,
									pMapping->uSize,
									pActualSize,
									0,
									dev_vaddr_alignment,
									&(pMapping->DevVAddr)))
	{
		__DtTestPoint(__DtFunc_DevMemoryAlloc, __DtStep_2)
		PVR_DPF((PVR_DBG_ERROR, "DevMemoryAlloc ERROR MMU_Alloc"));
		__DtTestPoint(__DtFunc_DevMemoryAlloc, __DtStep_3)
		return IMG_FALSE;
	}

#ifdef SUPPORT_SGX_MMU_BYPASS
	EnableHostAccess(pBMContext->psMMUContext);
#endif

#if defined(PDUMP)
	
	PDUMPMALLOCPAGES(&psDeviceNode->sDevId,
					 pMapping->DevVAddr.uiAddr,
					 pMapping->CpuVAddr,
					 pMapping->hOSMemHandle,
					 ui32PDumpSize,
					 pMapping->pBMHeap->sDevArena.ui32DataPageSize,
#if defined(SUPPORT_PDUMP_MULTI_PROCESS)
					 psDeviceNode->pfnMMUIsHeapShared(pMapping->pBMHeap->pMMUHeap),
#else
					 IMG_FALSE, 
#endif 
					 (IMG_HANDLE)pMapping);
#endif

	switch (pMapping->eCpuMemoryOrigin)
	{
		case hm_wrapped:
		case hm_wrapped_virtaddr:
		case hm_contiguous:
		{
			__DtTestPoint(__DtFunc_DevMemoryAlloc, __DtStep_4)
			psDeviceNode->pfnMMUMapPages (	pMapping->pBMHeap->pMMUHeap,
							pMapping->DevVAddr,
							SysCpuPAddrToSysPAddr (pMapping->CpuPAddr),
							pMapping->uSize,
							uFlags,
							(IMG_HANDLE)pMapping);

			*pDevVAddr = pMapping->DevVAddr;
			break;
		}
		case hm_env:
		{
			__DtTestPoint(__DtFunc_DevMemoryAlloc, __DtStep_5)
			psDeviceNode->pfnMMUMapShadow (	pMapping->pBMHeap->pMMUHeap,
							pMapping->DevVAddr,
							pMapping->uSize,
							pMapping->CpuVAddr,
							pMapping->hOSMemHandle,
							pDevVAddr,
							uFlags,
							(IMG_HANDLE)pMapping);
			break;
		}
		case hm_wrapped_scatter:
		case hm_wrapped_scatter_virtaddr:
		{
			__DtTestPoint(__DtFunc_DevMemoryAlloc, __DtStep_6)
			psDeviceNode->pfnMMUMapScatter (pMapping->pBMHeap->pMMUHeap,
							pMapping->DevVAddr,
							pMapping->psSysAddr,
							pMapping->uSize,
							uFlags,
							(IMG_HANDLE)pMapping);

			*pDevVAddr = pMapping->DevVAddr;
			break;
		}
		default:
			__DtTestPoint(__DtFunc_DevMemoryAlloc, __DtStep_7)
			PVR_DPF((PVR_DBG_ERROR,
				"Illegal value %d for pMapping->eCpuMemoryOrigin",
				pMapping->eCpuMemoryOrigin));
			__DtTestPoint(__DtFunc_DevMemoryAlloc, __DtStep_8)
			return IMG_FALSE;
	}

#ifdef SUPPORT_SGX_MMU_BYPASS
	DisableHostAccess(pBMContext->psMMUContext);
#endif

	return IMG_TRUE;
}

static IMG_VOID
DevMemoryFree (BM_MAPPING *pMapping)
{
	PVRSRV_DEVICE_NODE *psDeviceNode;
#ifdef PDUMP
	IMG_UINT32 ui32PSize;
#endif

#ifdef PDUMP
	
	if(pMapping->ui32Flags & PVRSRV_MEM_DUMMY)
	{
		
		__DtTestPoint(__DtFunc_DevMemoryFree, __DtStep_0)
		ui32PSize = pMapping->pBMHeap->sDevArena.ui32DataPageSize;
	}
	else
	{
		__DtTestPoint(__DtFunc_DevMemoryFree, __DtStep_1)
		ui32PSize = pMapping->uSize;
	}

	PDUMPFREEPAGES(pMapping->pBMHeap,
                    pMapping->DevVAddr,
                    ui32PSize,
                    pMapping->pBMHeap->sDevArena.ui32DataPageSize,
                    (IMG_HANDLE)pMapping,
                    (pMapping->ui32Flags & PVRSRV_MEM_INTERLEAVED) ? IMG_TRUE : IMG_FALSE);
#endif

	psDeviceNode = pMapping->pBMHeap->pBMContext->psDeviceNode;

	psDeviceNode->pfnMMUFree (pMapping->pBMHeap->pMMUHeap, pMapping->DevVAddr, IMG_CAST_TO_DEVVADDR_UINT(pMapping->uSize));
	__DtTestPoint(__DtFunc_DevMemoryFree, __DtStep_2)
}

static IMG_BOOL
BM_ImportMemory (IMG_VOID *pH,
			  IMG_SIZE_T uRequestSize,
			  IMG_SIZE_T *pActualSize,
			  BM_MAPPING **ppsMapping,
			  IMG_UINT32 uFlags,
			  IMG_UINTPTR_T *pBase)
{
	BM_MAPPING *pMapping;
	BM_HEAP *pBMHeap = pH;
	BM_CONTEXT *pBMContext = pBMHeap->pBMContext;
	IMG_BOOL bResult;
	IMG_SIZE_T uSize;
	IMG_SIZE_T uPSize;
	IMG_UINT32 uDevVAddrAlignment = 0;

	__DtTestPoint(__DtFunc_BM_ImportMemory, __DtStep_0)
	PVR_DPF ((PVR_DBG_MESSAGE,
			  "BM_ImportMemory (pBMContext=0x%x, uRequestSize=0x%x, uFlags=0x%x, uAlign=0x%x)",
			  (IMG_UINTPTR_T)pBMContext, uRequestSize, uFlags, uDevVAddrAlignment));

	PVR_ASSERT (ppsMapping != IMG_NULL);
	PVR_ASSERT (pBMContext != IMG_NULL);

	if (ppsMapping == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_BM_ImportMemory, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "BM_ImportMemory: invalid parameter"));
		goto fail_exit;
	}

	uSize = HOST_PAGEALIGN (uRequestSize);
	PVR_ASSERT (uSize >= uRequestSize);

	if (OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP,
						sizeof (BM_MAPPING),
						(IMG_PVOID *)&pMapping, IMG_NULL,
						"Buffer Manager Mapping") != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_BM_ImportMemory, __DtStep_2)
		PVR_DPF ((PVR_DBG_ERROR, "BM_ImportMemory: failed BM_MAPPING alloc"));
		goto fail_exit;
	}

	pMapping->hOSMemHandle = 0;
	pMapping->CpuVAddr = 0;
	pMapping->DevVAddr.uiAddr = 0;
	pMapping->CpuPAddr.uiAddr = 0;
	pMapping->uSize = uSize;
	pMapping->pBMHeap = pBMHeap;
	pMapping->ui32Flags = uFlags;

	
	if (pActualSize)
	{
		__DtTestPoint(__DtFunc_BM_ImportMemory, __DtStep_3)
		*pActualSize = uSize;
	}

	
	if(pMapping->ui32Flags & PVRSRV_MEM_DUMMY)
	{
		__DtTestPoint(__DtFunc_BM_ImportMemory, __DtStep_4)
		uPSize = pBMHeap->sDevArena.ui32DataPageSize;
	}
	else
	{
		__DtTestPoint(__DtFunc_BM_ImportMemory, __DtStep_5)
		uPSize = pMapping->uSize;
	}

	

	if(pBMHeap->ui32Attribs & PVRSRV_BACKINGSTORE_SYSMEM_NONCONTIG)
	{
		IMG_UINT32 ui32Attribs = pBMHeap->ui32Attribs;

		
		__DtTestPoint(__DtFunc_BM_ImportMemory, __DtStep_6)
		if (pMapping->ui32Flags & PVRSRV_HAP_CACHETYPE_MASK)
		{
			__DtTestPoint(__DtFunc_BM_ImportMemory, __DtStep_7)
			ui32Attribs &= ~PVRSRV_HAP_CACHETYPE_MASK;
			ui32Attribs |= (pMapping->ui32Flags & PVRSRV_HAP_CACHETYPE_MASK);
		}

		
		if (OSAllocPages(ui32Attribs,
						 uPSize,
						 pBMHeap->sDevArena.ui32DataPageSize,
						 (IMG_VOID **)&pMapping->CpuVAddr,
						 &pMapping->hOSMemHandle) != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_BM_ImportMemory, __DtStep_8)
			PVR_DPF((PVR_DBG_ERROR,
					"BM_ImportMemory: OSAllocPages(0x%x) failed",
					uPSize));
			goto fail_mapping_alloc;
		}

		
		pMapping->eCpuMemoryOrigin = hm_env;
	}
	else if(pBMHeap->ui32Attribs & PVRSRV_BACKINGSTORE_LOCALMEM_CONTIG)
	{
		IMG_SYS_PHYADDR sSysPAddr;
		IMG_UINT32 ui32Attribs = pBMHeap->ui32Attribs;

		
		__DtTestPoint(__DtFunc_BM_ImportMemory, __DtStep_9)
		PVR_ASSERT(pBMHeap->pLocalDevMemArena != IMG_NULL);

		
		if (pMapping->ui32Flags & PVRSRV_HAP_CACHETYPE_MASK)
		{
			__DtTestPoint(__DtFunc_BM_ImportMemory, __DtStep_10)
			ui32Attribs &= ~PVRSRV_HAP_CACHETYPE_MASK;
			ui32Attribs |= (pMapping->ui32Flags & PVRSRV_HAP_CACHETYPE_MASK);
		}

		if (!RA_Alloc (pBMHeap->pLocalDevMemArena,
					   uPSize,
					   IMG_NULL,
					   IMG_NULL,
					   0,
					   pBMHeap->sDevArena.ui32DataPageSize,
					   0,
					   (IMG_UINTPTR_T *)&sSysPAddr.uiAddr))
		{
			__DtTestPoint(__DtFunc_BM_ImportMemory, __DtStep_11)
			PVR_DPF((PVR_DBG_ERROR, "BM_ImportMemory: RA_Alloc(0x%x) FAILED", uPSize));
			goto fail_mapping_alloc;
		}

		
		pMapping->CpuPAddr = SysSysPAddrToCpuPAddr(sSysPAddr);
		if(OSReservePhys(pMapping->CpuPAddr,
						 uPSize,
						 ui32Attribs,
						 &pMapping->CpuVAddr,
						 &pMapping->hOSMemHandle) != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_BM_ImportMemory, __DtStep_12)
			PVR_DPF((PVR_DBG_ERROR,	"BM_ImportMemory: OSReservePhys failed"));
			goto fail_dev_mem_alloc;
		}

		
		pMapping->eCpuMemoryOrigin = hm_contiguous;
	}
	else
	{
		__DtTestPoint(__DtFunc_BM_ImportMemory, __DtStep_13)
		PVR_DPF((PVR_DBG_ERROR,	"BM_ImportMemory: Invalid backing store type"));
		goto fail_mapping_alloc;
	}

	
	bResult = DevMemoryAlloc (pBMContext,
								pMapping,
								IMG_NULL,
								uFlags,
								uDevVAddrAlignment,
								&pMapping->DevVAddr);
	if (!bResult)
	{
		__DtTestPoint(__DtFunc_BM_ImportMemory, __DtStep_14)
		PVR_DPF((PVR_DBG_ERROR,
				"BM_ImportMemory: DevMemoryAlloc(0x%x) failed",
				pMapping->uSize));
		goto fail_dev_mem_alloc;
	}

	
	
	PVR_ASSERT (uDevVAddrAlignment>1?(pMapping->DevVAddr.uiAddr%uDevVAddrAlignment)==0:1);

	*pBase = pMapping->DevVAddr.uiAddr;
	*ppsMapping = pMapping;

	PVR_DPF ((PVR_DBG_MESSAGE, "BM_ImportMemory: IMG_TRUE"));
	__DtTestPoint(__DtFunc_BM_ImportMemory, __DtStep_15)
	return IMG_TRUE;

fail_dev_mem_alloc:
	if (pMapping && (pMapping->CpuVAddr || pMapping->hOSMemHandle))
	{
		
		__DtTestPoint(__DtFunc_BM_ImportMemory, __DtStep_16)
		if(pMapping->ui32Flags & PVRSRV_MEM_INTERLEAVED)
		{
			__DtTestPoint(__DtFunc_BM_ImportMemory, __DtStep_17)
			pMapping->uSize /= 2;
		}

		if(pMapping->ui32Flags & PVRSRV_MEM_DUMMY)
		{
			__DtTestPoint(__DtFunc_BM_ImportMemory, __DtStep_18)
			uPSize = pBMHeap->sDevArena.ui32DataPageSize;
		}
		else
		{
			__DtTestPoint(__DtFunc_BM_ImportMemory, __DtStep_19)
			uPSize = pMapping->uSize;
		}

		if(pBMHeap->ui32Attribs & PVRSRV_BACKINGSTORE_SYSMEM_NONCONTIG)
		{
			__DtTestPoint(__DtFunc_BM_ImportMemory, __DtStep_20)
			OSFreePages(pBMHeap->ui32Attribs,
						  uPSize,
						  (IMG_VOID *)pMapping->CpuVAddr,
						  pMapping->hOSMemHandle);
		}
		else
		{
			IMG_SYS_PHYADDR sSysPAddr;

			__DtTestPoint(__DtFunc_BM_ImportMemory, __DtStep_21)
			if(pMapping->CpuVAddr)
			{
				__DtTestPoint(__DtFunc_BM_ImportMemory, __DtStep_22)
				OSUnReservePhys(pMapping->CpuVAddr,
								uPSize,
								pBMHeap->ui32Attribs,
								pMapping->hOSMemHandle);
			}
			sSysPAddr = SysCpuPAddrToSysPAddr(pMapping->CpuPAddr);
			RA_Free (pBMHeap->pLocalDevMemArena, sSysPAddr.uiAddr, IMG_FALSE);
		}
	}
fail_mapping_alloc:
	OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(BM_MAPPING), pMapping, IMG_NULL);
	
fail_exit:
	__DtTestPoint(__DtFunc_BM_ImportMemory, __DtStep_23)
	return IMG_FALSE;
}


static IMG_VOID
BM_FreeMemory (IMG_VOID *h, IMG_UINTPTR_T _base, BM_MAPPING *psMapping)
{
	BM_HEAP *pBMHeap = h;
	IMG_SIZE_T uPSize;

	__DtTestPoint(__DtFunc_BM_FreeMemory, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER (_base);

	PVR_DPF ((PVR_DBG_MESSAGE,
			  "BM_FreeMemory (h=0x%x, base=0x%x, psMapping=0x%x)",
			  (IMG_UINTPTR_T)h, _base, (IMG_UINTPTR_T)psMapping));

	PVR_ASSERT (psMapping != IMG_NULL);

	if (psMapping == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_BM_FreeMemory, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "BM_FreeMemory: invalid parameter"));
		__DtTestPoint(__DtFunc_BM_FreeMemory, __DtStep_2)
		return;
	}

	DevMemoryFree (psMapping);

	
	if((psMapping->ui32Flags & PVRSRV_MEM_INTERLEAVED) != 0)
	{
		__DtTestPoint(__DtFunc_BM_FreeMemory, __DtStep_3)
		psMapping->uSize /= 2;
	}

	if(psMapping->ui32Flags & PVRSRV_MEM_DUMMY)
	{
		__DtTestPoint(__DtFunc_BM_FreeMemory, __DtStep_4)
		uPSize = psMapping->pBMHeap->sDevArena.ui32DataPageSize;
	}
	else
	{
		__DtTestPoint(__DtFunc_BM_FreeMemory, __DtStep_5)
		uPSize = psMapping->uSize;
	}

	if(pBMHeap->ui32Attribs & PVRSRV_BACKINGSTORE_SYSMEM_NONCONTIG)
	{
		__DtTestPoint(__DtFunc_BM_FreeMemory, __DtStep_6)
		OSFreePages(pBMHeap->ui32Attribs,
						uPSize,
						(IMG_VOID *) psMapping->CpuVAddr,
						psMapping->hOSMemHandle);
	}
	else if(pBMHeap->ui32Attribs & PVRSRV_BACKINGSTORE_LOCALMEM_CONTIG)
	{
		IMG_SYS_PHYADDR sSysPAddr;

		__DtTestPoint(__DtFunc_BM_FreeMemory, __DtStep_7)
		OSUnReservePhys(psMapping->CpuVAddr, uPSize, pBMHeap->ui32Attribs, psMapping->hOSMemHandle);

		sSysPAddr = SysCpuPAddrToSysPAddr(psMapping->CpuPAddr);

		RA_Free (pBMHeap->pLocalDevMemArena, sSysPAddr.uiAddr, IMG_FALSE);
	}
	else
	{
		__DtTestPoint(__DtFunc_BM_FreeMemory, __DtStep_8)
		PVR_DPF((PVR_DBG_ERROR,	"BM_FreeMemory: Invalid backing store type"));
	}

	OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, sizeof(BM_MAPPING), psMapping, IMG_NULL);
	

	PVR_DPF((PVR_DBG_MESSAGE,
			"..BM_FreeMemory (h=0x%x, base=0x%x)",
			(IMG_UINTPTR_T)h, _base));
	__DtTestPoint(__DtFunc_BM_FreeMemory, __DtStep_9)
}

IMG_VOID BM_GetPhysPageAddr(PVRSRV_KERNEL_MEM_INFO *psMemInfo,
								IMG_DEV_VIRTADDR sDevVPageAddr,
								IMG_DEV_PHYADDR *psDevPAddr)
{
	PVRSRV_DEVICE_NODE *psDeviceNode;

	__DtTestPoint(__DtFunc_BM_GetPhysPageAddr, __DtStep_0)
	PVR_DPF((PVR_DBG_MESSAGE, "BM_GetPhysPageAddr"));

	PVR_ASSERT (psMemInfo && psDevPAddr)

	
	PVR_ASSERT((sDevVPageAddr.uiAddr & 0xFFF) == 0);

	psDeviceNode = ((BM_BUF*)psMemInfo->sMemBlk.hBuffer)->pMapping->pBMHeap->pBMContext->psDeviceNode;

	*psDevPAddr = psDeviceNode->pfnMMUGetPhysPageAddr(((BM_BUF*)psMemInfo->sMemBlk.hBuffer)->pMapping->pBMHeap->pMMUHeap,
												sDevVPageAddr);
	__DtTestPoint(__DtFunc_BM_GetPhysPageAddr, __DtStep_1)
}


MMU_CONTEXT* BM_GetMMUContext(IMG_HANDLE hDevMemHeap)
{
	BM_HEAP *pBMHeap = (BM_HEAP*)hDevMemHeap;

	__DtTestPoint(__DtFunc_BM_GetMMUContext, __DtStep_0)
	PVR_DPF((PVR_DBG_VERBOSE, "BM_GetMMUContext"));

	__DtTestPoint(__DtFunc_BM_GetMMUContext, __DtStep_1)
	return pBMHeap->pBMContext->psMMUContext;
}

MMU_CONTEXT* BM_GetMMUContextFromMemContext(IMG_HANDLE hDevMemContext)
{
	BM_CONTEXT *pBMContext = (BM_CONTEXT*)hDevMemContext;

	__DtTestPoint(__DtFunc_BM_GetMMUContextFromMemContext, __DtStep_0)
	PVR_DPF ((PVR_DBG_VERBOSE, "BM_GetMMUContextFromMemContext"));

	__DtTestPoint(__DtFunc_BM_GetMMUContextFromMemContext, __DtStep_1)
	return pBMContext->psMMUContext;
}

IMG_HANDLE BM_GetMMUHeap(IMG_HANDLE hDevMemHeap)
{
	__DtTestPoint(__DtFunc_BM_GetMMUHeap, __DtStep_0)
	PVR_DPF((PVR_DBG_VERBOSE, "BM_GetMMUHeap"));

	__DtTestPoint(__DtFunc_BM_GetMMUHeap, __DtStep_1)
	return (IMG_HANDLE)((BM_HEAP*)hDevMemHeap)->pMMUHeap;
}


PVRSRV_DEVICE_NODE* BM_GetDeviceNode(IMG_HANDLE hDevMemContext)
{
	__DtTestPoint(__DtFunc_BM_GetDeviceNode, __DtStep_0)
	PVR_DPF((PVR_DBG_VERBOSE, "BM_GetDeviceNode"));

	__DtTestPoint(__DtFunc_BM_GetDeviceNode, __DtStep_1)
	return ((BM_CONTEXT*)hDevMemContext)->psDeviceNode;
}


IMG_HANDLE BM_GetMappingHandle(PVRSRV_KERNEL_MEM_INFO *psMemInfo)
{
	__DtTestPoint(__DtFunc_BM_GetMappingHandle, __DtStep_0)
	PVR_DPF((PVR_DBG_VERBOSE, "BM_GetMappingHandle"));

	__DtTestPoint(__DtFunc_BM_GetMappingHandle, __DtStep_1)
	return ((BM_BUF*)psMemInfo->sMemBlk.hBuffer)->pMapping->hOSMemHandle;
}

