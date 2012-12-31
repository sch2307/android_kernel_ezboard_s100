#include	"DT_mem_debug.h"	/* For DynamicTracer-TestPoint */
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

#ifndef MEM_DEBUG_C
#define MEM_DEBUG_C

#if defined(PVRSRV_DEBUG_OS_MEMORY)

#include "img_types.h"
#include "services_headers.h"

#if defined (__cplusplus)
extern "C"
{
#endif

#define STOP_ON_ERROR 0

	

	





	IMG_BOOL MemCheck(const IMG_PVOID pvAddr, const IMG_UINT8 ui8Pattern, IMG_SIZE_T uSize)
	{
		IMG_UINT8 *pui8Addr;
		__DtTestPoint(__DtFunc_MemCheck, __DtStep_0)
		for (pui8Addr = (IMG_UINT8*)pvAddr; uSize > 0; uSize--, pui8Addr++)
		{
			__DtTestPoint(__DtFunc_MemCheck, __DtStep_1)
			if (*pui8Addr != ui8Pattern)
			{
				__DtTestPoint(__DtFunc_MemCheck, __DtStep_2)
				return IMG_FALSE;
			}
		}
		__DtTestPoint(__DtFunc_MemCheck, __DtStep_3)
		return IMG_TRUE;
	}

	

	IMG_VOID OSCheckMemDebug(IMG_PVOID pvCpuVAddr, IMG_SIZE_T uSize, const IMG_CHAR *pszFileName, const IMG_UINT32 uLine)
	{
		OSMEM_DEBUG_INFO const *psInfo = (OSMEM_DEBUG_INFO *)((IMG_UINT32)pvCpuVAddr - TEST_BUFFER_PADDING_STATUS);

		
		__DtTestPoint(__DtFunc_OSCheckMemDebug, __DtStep_0)
		if (pvCpuVAddr == IMG_NULL)
		{
			__DtTestPoint(__DtFunc_OSCheckMemDebug, __DtStep_1)
			PVR_DPF((PVR_DBG_ERROR, "Pointer 0x%X : null pointer"
					 " - referenced %s:%d - allocated %s:%d",
					 pvCpuVAddr,
					 pszFileName, uLine,
					 psInfo->sFileName, psInfo->uLineNo));
			while (STOP_ON_ERROR);
		}

		
		if (((IMG_UINT32)pvCpuVAddr&3) != 0)
		{
			__DtTestPoint(__DtFunc_OSCheckMemDebug, __DtStep_2)
			PVR_DPF((PVR_DBG_ERROR, "Pointer 0x%X : invalid alignment"
					 " - referenced %s:%d - allocated %s:%d",
					 pvCpuVAddr,
					 pszFileName, uLine,
					 psInfo->sFileName, psInfo->uLineNo));
			while (STOP_ON_ERROR);
		}

		
		if (!MemCheck((IMG_PVOID)psInfo->sGuardRegionBefore, 0xB1, sizeof(psInfo->sGuardRegionBefore)))
		{
			__DtTestPoint(__DtFunc_OSCheckMemDebug, __DtStep_3)
			PVR_DPF((PVR_DBG_ERROR, "Pointer 0x%X : guard region before overwritten"
					 " - referenced %s:%d - allocated %s:%d",
					 pvCpuVAddr,
					 pszFileName, uLine,
					 psInfo->sFileName, psInfo->uLineNo));
			while (STOP_ON_ERROR);
		}

		
		if (uSize != psInfo->uSize)
		{
			__DtTestPoint(__DtFunc_OSCheckMemDebug, __DtStep_4)
			PVR_DPF((PVR_DBG_WARNING, "Pointer 0x%X : supplied size was different to stored size (0x%X != 0x%X)"
					 " - referenced %s:%d - allocated %s:%d",
					 pvCpuVAddr, uSize, psInfo->uSize,
					 pszFileName, uLine,
					 psInfo->sFileName, psInfo->uLineNo));
			while (STOP_ON_ERROR);
		}

		
		if ((0x01234567 ^ psInfo->uSizeParityCheck) != psInfo->uSize)
		{
			__DtTestPoint(__DtFunc_OSCheckMemDebug, __DtStep_5)
			PVR_DPF((PVR_DBG_WARNING, "Pointer 0x%X : stored size parity error (0x%X != 0x%X)"
					 " - referenced %s:%d - allocated %s:%d",
					 pvCpuVAddr, psInfo->uSize, 0x01234567 ^ psInfo->uSizeParityCheck,
					 pszFileName, uLine,
					 psInfo->sFileName, psInfo->uLineNo));
			while (STOP_ON_ERROR);
		}
		else
		{
			
			__DtTestPoint(__DtFunc_OSCheckMemDebug, __DtStep_6)
			uSize = psInfo->uSize;
		}

		
		if (uSize)
		{
			__DtTestPoint(__DtFunc_OSCheckMemDebug, __DtStep_7)
			if (!MemCheck((IMG_VOID*)((IMG_UINT32)pvCpuVAddr + uSize), 0xB2, TEST_BUFFER_PADDING_AFTER))
			{
				__DtTestPoint(__DtFunc_OSCheckMemDebug, __DtStep_8)
				PVR_DPF((PVR_DBG_ERROR, "Pointer 0x%X : guard region after overwritten"
						 " - referenced from %s:%d - allocated from %s:%d",
						 pvCpuVAddr,
						 pszFileName, uLine,
						 psInfo->sFileName, psInfo->uLineNo));
			}
		}

		
		if (psInfo->eValid != isAllocated)
		{
			__DtTestPoint(__DtFunc_OSCheckMemDebug, __DtStep_9)
			PVR_DPF((PVR_DBG_ERROR, "Pointer 0x%X : not allocated (freed? %d)"
					 " - referenced %s:%d - freed %s:%d",
					 pvCpuVAddr, psInfo->eValid == isFree,
					 pszFileName, uLine,
					 psInfo->sFileName, psInfo->uLineNo));
			while (STOP_ON_ERROR);
		}
		__DtTestPoint(__DtFunc_OSCheckMemDebug, __DtStep_10)
	}

	IMG_VOID debug_strcpy(IMG_CHAR *pDest, const IMG_CHAR *pSrc)
	{
		IMG_SIZE_T i = 0;

		__DtTestPoint(__DtFunc_debug_strcpy, __DtStep_0)
		for (; i < 128; i++) 
		{
			__DtTestPoint(__DtFunc_debug_strcpy, __DtStep_1)
			*pDest = *pSrc;
			if (*pSrc == '\0') break;
			pDest++;
			pSrc++;
		}
		__DtTestPoint(__DtFunc_debug_strcpy, __DtStep_2)
	}

	PVRSRV_ERROR OSAllocMem_Debug_Wrapper(IMG_UINT32 ui32Flags,
										  IMG_UINT32 ui32Size,
										  IMG_PVOID *ppvCpuVAddr,
										  IMG_HANDLE *phBlockAlloc,
										  IMG_CHAR *pszFilename,
										  IMG_UINT32 ui32Line)
	{
		OSMEM_DEBUG_INFO *psInfo;

		PVRSRV_ERROR eError;

		__DtTestPoint(__DtFunc_OSAllocMem_Debug_Wrapper, __DtStep_0)
		eError = OSAllocMem_Debug_Linux_Memory_Allocations(ui32Flags,
				 ui32Size + TEST_BUFFER_PADDING,
				 ppvCpuVAddr,
				 phBlockAlloc,
				 pszFilename,
				 ui32Line);

		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_OSAllocMem_Debug_Wrapper, __DtStep_1)
			return eError;
		}

		
		OSMemSet((IMG_CHAR *)(*ppvCpuVAddr) + TEST_BUFFER_PADDING_STATUS, 0xBB, ui32Size);
		OSMemSet((IMG_CHAR *)(*ppvCpuVAddr) + ui32Size + TEST_BUFFER_PADDING_STATUS, 0xB2, TEST_BUFFER_PADDING_AFTER);

		
		psInfo = (OSMEM_DEBUG_INFO *)(*ppvCpuVAddr);

		OSMemSet(psInfo->sGuardRegionBefore, 0xB1, sizeof(psInfo->sGuardRegionBefore));
		debug_strcpy(psInfo->sFileName, pszFilename);
		psInfo->uLineNo = ui32Line;
		psInfo->eValid = isAllocated;
		psInfo->uSize = ui32Size;
		psInfo->uSizeParityCheck = 0x01234567 ^ ui32Size;

		
		*ppvCpuVAddr = (IMG_PVOID) ((IMG_UINT32)*ppvCpuVAddr)+TEST_BUFFER_PADDING_STATUS;

#ifdef PVRSRV_LOG_MEMORY_ALLOCS
		
		PVR_TRACE(("Allocated pointer (after debug info): 0x%X from %s:%d", *ppvCpuVAddr, pszFilename, ui32Line));
#endif

		return PVRSRV_OK;
	}

	PVRSRV_ERROR OSFreeMem_Debug_Wrapper(IMG_UINT32 ui32Flags,
										 IMG_UINT32 ui32Size,
										 IMG_PVOID pvCpuVAddr,
										 IMG_HANDLE hBlockAlloc,
										 IMG_CHAR *pszFilename,
										 IMG_UINT32 ui32Line)
	{
		OSMEM_DEBUG_INFO *psInfo;

		
		__DtTestPoint(__DtFunc_OSFreeMem_Debug_Wrapper, __DtStep_0)
		OSCheckMemDebug(pvCpuVAddr, ui32Size, pszFilename, ui32Line);

		
		OSMemSet(pvCpuVAddr, 0xBF, ui32Size + TEST_BUFFER_PADDING_AFTER);  

		
		psInfo = (OSMEM_DEBUG_INFO *)((IMG_UINT32) pvCpuVAddr - TEST_BUFFER_PADDING_STATUS);

		
		psInfo->uSize = 0;
		psInfo->uSizeParityCheck = 0;
		psInfo->eValid = isFree;
		psInfo->uLineNo = ui32Line;
		debug_strcpy(psInfo->sFileName, pszFilename);

		__DtTestPoint(__DtFunc_OSFreeMem_Debug_Wrapper, __DtStep_1)
		return OSFreeMem_Debug_Linux_Memory_Allocations(ui32Flags, ui32Size + TEST_BUFFER_PADDING, psInfo, hBlockAlloc, pszFilename, ui32Line);
	}

#if defined (__cplusplus)

}
#endif

#endif 

#endif        
