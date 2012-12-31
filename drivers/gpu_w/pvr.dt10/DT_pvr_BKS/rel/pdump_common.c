#include	"DT_pdump_common.h"	/* For DynamicTracer-TestPoint */
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

#if defined(PDUMP)
#include <stdarg.h>

#include "services_headers.h"
#if defined(SUPPORT_SGX)
#include "sgxdefs.h"
#endif 
#include "perproc.h"

#include "pdump_km.h"
#include "pdump_int.h"

#if !defined(PDUMP_TEMP_BUFFER_SIZE)
#define PDUMP_TEMP_BUFFER_SIZE (64 * 1024U)
#endif

#if 1
#define PDUMP_DBG(a)   PDumpOSDebugPrintf (a)
#else
#define PDUMP_DBG(a)
#endif

#define PDUMP_DATAMASTER_PIXEL		(1)
#define PDUMP_DATAMASTER_EDM		(3)

#define	PTR_PLUS(t, p, x) ((t)(((IMG_CHAR *)(p)) + (x)))
#define	VPTR_PLUS(p, x) PTR_PLUS(IMG_VOID *, p, x)
#define	VPTR_INC(p, x) ((p) = VPTR_PLUS(p, x))
#define MAX_PDUMP_MMU_CONTEXTS	(32)
static IMG_VOID *gpvTempBuffer = IMG_NULL;
static IMG_HANDLE ghTempBufferBlockAlloc;
static IMG_UINT16 gui16MMUContextUsage = 0;

#if defined(PDUMP_DEBUG_OUTFILES)
IMG_UINT32 g_ui32EveryLineCounter = 1U;
#endif

#ifdef INLINE_IS_PRAGMA
#pragma inline(_PDumpIsPersistent)
#endif
static INLINE
IMG_BOOL _PDumpIsPersistent(IMG_VOID)
{
	PVRSRV_PER_PROCESS_DATA* psPerProc = PVRSRVFindPerProcessData();
	__DtTestPoint(__DtFunc__PDumpIsPersistent, __DtStep_0)
	if(psPerProc == IMG_NULL)
	{
		
		__DtTestPoint(__DtFunc__PDumpIsPersistent, __DtStep_1)
		return IMG_FALSE;
	}
	__DtTestPoint(__DtFunc__PDumpIsPersistent, __DtStep_2)
	return psPerProc->bPDumpPersistent;
}

#if defined(SUPPORT_PDUMP_MULTI_PROCESS)
static INLINE
IMG_BOOL _PDumpIsProcessActive(IMG_VOID)
{
	PVRSRV_PER_PROCESS_DATA* psPerProc = PVRSRVFindPerProcessData();
	__DtTestPoint(__DtFunc__PDumpIsProcessActive, __DtStep_0)
	if(psPerProc == IMG_NULL)
	{
		
		__DtTestPoint(__DtFunc__PDumpIsProcessActive, __DtStep_1)
		return IMG_TRUE;
	}
	__DtTestPoint(__DtFunc__PDumpIsProcessActive, __DtStep_2)
	return psPerProc->bPDumpActive;
}
#endif 

#if defined(PDUMP_DEBUG_OUTFILES)
static INLINE
IMG_UINT32 _PDumpGetPID(IMG_VOID)
{
	PVRSRV_PER_PROCESS_DATA* psPerProc = PVRSRVFindPerProcessData();
	__DtTestPoint(__DtFunc__PDumpGetPID, __DtStep_0)
	if(psPerProc == IMG_NULL)
	{
		
		__DtTestPoint(__DtFunc__PDumpGetPID, __DtStep_1)
		return 0;
	}
	__DtTestPoint(__DtFunc__PDumpGetPID, __DtStep_2)
	return psPerProc->ui32PID;
}
#endif 

static IMG_VOID *GetTempBuffer(IMG_VOID)
{
	
	__DtTestPoint(__DtFunc_GetTempBuffer, __DtStep_0)
	if (gpvTempBuffer == IMG_NULL)
	{
		PVRSRV_ERROR eError = OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP,
					  PDUMP_TEMP_BUFFER_SIZE,
					  &gpvTempBuffer,
					  &ghTempBufferBlockAlloc,
					  "PDUMP Temporary Buffer");
		__DtTestPoint(__DtFunc_GetTempBuffer, __DtStep_1)
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_GetTempBuffer, __DtStep_2)
			PVR_DPF((PVR_DBG_ERROR, "GetTempBuffer: OSAllocMem failed: %d", eError));
		}
	}

	__DtTestPoint(__DtFunc_GetTempBuffer, __DtStep_3)
	return gpvTempBuffer;
}

static IMG_VOID FreeTempBuffer(IMG_VOID)
{

	__DtTestPoint(__DtFunc_FreeTempBuffer, __DtStep_0)
	if (gpvTempBuffer != IMG_NULL)
	{
		PVRSRV_ERROR eError = OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP,
					  PDUMP_TEMP_BUFFER_SIZE,
					  gpvTempBuffer,
					  ghTempBufferBlockAlloc);
		__DtTestPoint(__DtFunc_FreeTempBuffer, __DtStep_1)
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_FreeTempBuffer, __DtStep_2)
			PVR_DPF((PVR_DBG_ERROR, "FreeTempBuffer: OSFreeMem failed: %d", eError));
		}
		else
		{
			__DtTestPoint(__DtFunc_FreeTempBuffer, __DtStep_3)
			gpvTempBuffer = IMG_NULL;
		}
	}
	__DtTestPoint(__DtFunc_FreeTempBuffer, __DtStep_4)
}

IMG_VOID PDumpInitCommon(IMG_VOID)
{
	
	__DtTestPoint(__DtFunc_PDumpInitCommon, __DtStep_0)
	(IMG_VOID) GetTempBuffer();

	
	PDumpInit();
	__DtTestPoint(__DtFunc_PDumpInitCommon, __DtStep_1)
}

IMG_VOID PDumpDeInitCommon(IMG_VOID)
{
	
	__DtTestPoint(__DtFunc_PDumpDeInitCommon, __DtStep_0)
	FreeTempBuffer();

	
	PDumpDeInit();
	__DtTestPoint(__DtFunc_PDumpDeInitCommon, __DtStep_1)
}

IMG_BOOL PDumpIsSuspended(IMG_VOID)
{
	__DtTestPoint(__DtFunc_PDumpIsSuspended, __DtStep_0)
	return PDumpOSIsSuspended();
}

IMG_BOOL PDumpIsCaptureFrameKM(IMG_VOID)
{
#if defined(SUPPORT_PDUMP_MULTI_PROCESS)
	if( _PDumpIsProcessActive() )
	{
		__DtTestPoint(__DtFunc_PDumpIsCaptureFrameKM, __DtStep_0)
		return PDumpOSIsCaptureFrameKM();
	}
	__DtTestPoint(__DtFunc_PDumpIsCaptureFrameKM, __DtStep_1)
	return IMG_FALSE;
#else
	return PDumpOSIsCaptureFrameKM();
#endif
}

PVRSRV_ERROR PDumpSetFrameKM(IMG_UINT32 ui32Frame)
{
#if defined(SUPPORT_PDUMP_MULTI_PROCESS)
	if( _PDumpIsProcessActive() )
	{
		__DtTestPoint(__DtFunc_PDumpSetFrameKM, __DtStep_0)
		return PDumpOSSetFrameKM(ui32Frame);
	}
	__DtTestPoint(__DtFunc_PDumpSetFrameKM, __DtStep_1)
	return PVRSRV_OK;
#else
	return PDumpOSSetFrameKM(ui32Frame);
#endif
}


PVRSRV_ERROR PDumpRegWithFlagsKM(IMG_CHAR *pszPDumpRegName,
								IMG_UINT32 ui32Reg,
								IMG_UINT32 ui32Data,
								IMG_UINT32 ui32Flags)
{
	PVRSRV_ERROR eErr;
	__DtTestPoint(__DtFunc_PDumpRegWithFlagsKM, __DtStep_0)
	PDUMP_GET_SCRIPT_STRING()
	PDUMP_DBG(("PDumpRegWithFlagsKM"));

	eErr = PDumpOSBufprintf(hScript, ui32MaxLen, "WRW :%s:0x%08X 0x%08X\r\n",
								pszPDumpRegName, ui32Reg, ui32Data);
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpRegWithFlagsKM, __DtStep_1)
		return eErr;
	}
	PDumpOSWriteString2(hScript, ui32Flags);

	__DtTestPoint(__DtFunc_PDumpRegWithFlagsKM, __DtStep_2)
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpRegKM(IMG_CHAR *pszPDumpRegName,
						IMG_UINT32 ui32Reg,
						IMG_UINT32 ui32Data)
{
	__DtTestPoint(__DtFunc_PDumpRegKM, __DtStep_0)
	return PDumpRegWithFlagsKM(pszPDumpRegName, ui32Reg, ui32Data, PDUMP_FLAGS_CONTINUOUS);
}

PVRSRV_ERROR PDumpRegPolWithFlagsKM(IMG_CHAR *pszPDumpRegName,
									IMG_UINT32 ui32RegAddr, 
									IMG_UINT32 ui32RegValue, 
									IMG_UINT32 ui32Mask,
									IMG_UINT32 ui32Flags)
{
	
	#define POLL_DELAY			1000U
	#define POLL_COUNT_LONG		(2000000000U / POLL_DELAY)
	#define POLL_COUNT_SHORT	(1000000U / POLL_DELAY)

	PVRSRV_ERROR eErr;
	IMG_UINT32	ui32PollCount;

	PDUMP_GET_SCRIPT_STRING();
	PDUMP_DBG(("PDumpRegPolWithFlagsKM"));
	if ( _PDumpIsPersistent() )
	{
		
		__DtTestPoint(__DtFunc_PDumpRegPolWithFlagsKM, __DtStep_0)
		return PVRSRV_OK;
	}

#if 0
	if (((ui32RegAddr == EUR_CR_EVENT_STATUS) &&
		(ui32RegValue & ui32Mask & EUR_CR_EVENT_STATUS_TA_FINISHED_MASK) != 0) ||
	    ((ui32RegAddr == EUR_CR_EVENT_STATUS) &&
		(ui32RegValue & ui32Mask & EUR_CR_EVENT_STATUS_PIXELBE_END_RENDER_MASK) != 0) ||
	    ((ui32RegAddr == EUR_CR_EVENT_STATUS) &&
		(ui32RegValue & ui32Mask & EUR_CR_EVENT_STATUS_DPM_3D_MEM_FREE_MASK) != 0))
	{
		__DtTestPoint(__DtFunc_PDumpRegPolWithFlagsKM, __DtStep_1)
		ui32PollCount = POLL_COUNT_LONG;
	}
	else
#endif
	{
		ui32PollCount = POLL_COUNT_LONG;
	}

	eErr = PDumpOSBufprintf(hScript, ui32MaxLen, "POL :%s:0x%08X 0x%08X 0x%08X %d %u %d\r\n",
							pszPDumpRegName, ui32RegAddr, ui32RegValue,
							ui32Mask, 0, ui32PollCount, POLL_DELAY);
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpRegPolWithFlagsKM, __DtStep_2)
		return eErr;
	}
	PDumpOSWriteString2(hScript, ui32Flags);

	__DtTestPoint(__DtFunc_PDumpRegPolWithFlagsKM, __DtStep_3)
	return PVRSRV_OK;
}


PVRSRV_ERROR PDumpRegPolKM(IMG_CHAR *pszPDumpRegName, IMG_UINT32 ui32RegAddr, IMG_UINT32 ui32RegValue, IMG_UINT32 ui32Mask)
{
	__DtTestPoint(__DtFunc_PDumpRegPolKM, __DtStep_0)
	return PDumpRegPolWithFlagsKM(pszPDumpRegName, ui32RegAddr, ui32RegValue, ui32Mask, PDUMP_FLAGS_CONTINUOUS);
}

PVRSRV_ERROR PDumpMallocPages (PVRSRV_DEVICE_IDENTIFIER	*psDevID,
                           IMG_UINT32         ui32DevVAddr,
                           IMG_CPU_VIRTADDR   pvLinAddr,
                           IMG_HANDLE         hOSMemHandle,
                           IMG_UINT32         ui32NumBytes,
                           IMG_UINT32         ui32PageSize,
                           IMG_BOOL			  bShared,
                           IMG_HANDLE         hUniqueTag)
{
	PVRSRV_ERROR eErr;
	IMG_PUINT8		pui8LinAddr;
    IMG_UINT32      ui32Offset;
	IMG_UINT32		ui32NumPages;
	IMG_DEV_PHYADDR	sDevPAddr;
	IMG_UINT32		ui32Page;
	IMG_UINT32		ui32Flags = PDUMP_FLAGS_CONTINUOUS;

	__DtTestPoint(__DtFunc_PDumpMallocPages, __DtStep_0)
	PDUMP_GET_SCRIPT_STRING();
#if defined(SUPPORT_PDUMP_MULTI_PROCESS)
	
	ui32Flags |= ( _PDumpIsPersistent() || bShared ) ? PDUMP_FLAGS_PERSISTENT : 0;
#else
	PVR_UNREFERENCED_PARAMETER(bShared);
	ui32Flags |= ( _PDumpIsPersistent() ) ? PDUMP_FLAGS_PERSISTENT : 0;
#endif

	
#if !defined(LINUX)
	PVR_ASSERT(((IMG_UINTPTR_T)pvLinAddr & HOST_PAGEMASK) == 0);
#endif

	PVR_ASSERT(((IMG_UINT32) ui32DevVAddr & HOST_PAGEMASK) == 0);
	PVR_ASSERT(((IMG_UINT32) ui32NumBytes & HOST_PAGEMASK) == 0);

	

	eErr = PDumpOSBufprintf(hScript, ui32MaxLen, "-- MALLOC :%s:VA_%08X 0x%08X %u\r\n",
			psDevID->pszPDumpDevName, ui32DevVAddr, ui32NumBytes, ui32PageSize);
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpMallocPages, __DtStep_1)
		return eErr;
	}
	PDumpOSWriteString2(hScript, ui32Flags);

	

	pui8LinAddr = (IMG_PUINT8) pvLinAddr;
	ui32Offset = 0;
	ui32NumPages = ui32NumBytes / ui32PageSize;
	while (ui32NumPages)
	{ 
		__DtTestPoint(__DtFunc_PDumpMallocPages, __DtStep_2)
		ui32NumPages--;

		






		
		
 		
		PDumpOSCPUVAddrToDevPAddr(psDevID->eDeviceType,
				hOSMemHandle,
				ui32Offset,
				pui8LinAddr,
				ui32PageSize,
				&sDevPAddr);
		ui32Page = (IMG_UINT32)(sDevPAddr.uiAddr / ui32PageSize);
		
		pui8LinAddr	+= ui32PageSize;
		ui32Offset += ui32PageSize;

		eErr = PDumpOSBufprintf(hScript, ui32MaxLen, "MALLOC :%s:PA_%08X%08X %u %u 0x%08X\r\n",
												psDevID->pszPDumpDevName,
												(IMG_UINT32)(IMG_UINTPTR_T)hUniqueTag,
												ui32Page * ui32PageSize,
												ui32PageSize,
												ui32PageSize,
												ui32Page * ui32PageSize);
		if(eErr != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PDumpMallocPages, __DtStep_3)
			return eErr;
		}
		PDumpOSWriteString2(hScript, ui32Flags);
	}
	__DtTestPoint(__DtFunc_PDumpMallocPages, __DtStep_4)
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpMallocPageTable (PVRSRV_DEVICE_IDENTIFIER	*psDevId,
								   IMG_HANDLE hOSMemHandle,
								   IMG_UINT32 ui32Offset,
                          		   IMG_CPU_VIRTADDR pvLinAddr,
								   IMG_UINT32 ui32PTSize,
								   IMG_UINT32 ui32Flags,
                                   IMG_HANDLE hUniqueTag)
{
	PVRSRV_ERROR eErr;
	IMG_DEV_PHYADDR	sDevPAddr;

	__DtTestPoint(__DtFunc_PDumpMallocPageTable, __DtStep_0)
	PDUMP_GET_SCRIPT_STRING();

	PVR_ASSERT(((IMG_UINTPTR_T)pvLinAddr & (ui32PTSize - 1)) == 0);	
	ui32Flags |= PDUMP_FLAGS_CONTINUOUS;
	ui32Flags |= ( _PDumpIsPersistent() ) ? PDUMP_FLAGS_PERSISTENT : 0;

	

	eErr = PDumpOSBufprintf(hScript,
							ui32MaxLen,
							"-- MALLOC :%s:PAGE_TABLE 0x%08X %u\r\n",
							psDevId->pszPDumpDevName,
							ui32PTSize,
							ui32PTSize);
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpMallocPageTable, __DtStep_1)
		return eErr;
	}
	PDumpOSWriteString2(hScript, ui32Flags);

	

	
	
	
	

	PDumpOSCPUVAddrToDevPAddr(psDevId->eDeviceType,
			hOSMemHandle, 
			ui32Offset,
			(IMG_PUINT8) pvLinAddr,
			ui32PTSize,
			&sDevPAddr);

	eErr = PDumpOSBufprintf(hScript, ui32MaxLen, "MALLOC :%s:PA_%08X%08X 0x%X %u 0x%08X\r\n",
											psDevId->pszPDumpDevName,
											(IMG_UINT32)(IMG_UINTPTR_T)hUniqueTag,
											sDevPAddr.uiAddr,
											ui32PTSize,
											ui32PTSize,
											sDevPAddr.uiAddr);
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpMallocPageTable, __DtStep_2)
		return eErr;
	}
	PDumpOSWriteString2(hScript, ui32Flags);

	__DtTestPoint(__DtFunc_PDumpMallocPageTable, __DtStep_3)
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpFreePages	(BM_HEAP 			*psBMHeap,
                         IMG_DEV_VIRTADDR  sDevVAddr,
                         IMG_UINT32        ui32NumBytes,
                         IMG_UINT32        ui32PageSize,
                         IMG_HANDLE        hUniqueTag,
						 IMG_BOOL		   bInterleaved)
{
	PVRSRV_ERROR eErr;
	IMG_UINT32 ui32NumPages, ui32PageCounter;
	IMG_DEV_PHYADDR	sDevPAddr;
	IMG_UINT32 ui32Flags = PDUMP_FLAGS_CONTINUOUS;
	PVRSRV_DEVICE_NODE *psDeviceNode;

	__DtTestPoint(__DtFunc_PDumpFreePages, __DtStep_0)
	PDUMP_GET_SCRIPT_STRING();

	PVR_ASSERT(((IMG_UINT32) sDevVAddr.uiAddr & (ui32PageSize - 1)) == 0);
	PVR_ASSERT(((IMG_UINT32) ui32NumBytes & (ui32PageSize - 1)) == 0);

	psDeviceNode = psBMHeap->pBMContext->psDeviceNode;
	ui32Flags |= ( _PDumpIsPersistent() ) ? PDUMP_FLAGS_PERSISTENT : 0;

	

	eErr = PDumpOSBufprintf(hScript, ui32MaxLen, "-- FREE :%s:VA_%08X\r\n", 
							psDeviceNode->sDevId.pszPDumpDevName, sDevVAddr.uiAddr);
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpFreePages, __DtStep_1)
		return eErr;
	}

#if defined(SUPPORT_PDUMP_MULTI_PROCESS)
	
	{
		PVRSRV_DEVICE_NODE *psDeviceNode = psBMHeap->pBMContext->psDeviceNode;
		
		if( psDeviceNode->pfnMMUIsHeapShared(psBMHeap->pMMUHeap) )
		{
			__DtTestPoint(__DtFunc_PDumpFreePages, __DtStep_2)
			ui32Flags |= PDUMP_FLAGS_PERSISTENT;
		}
	}
#endif
	PDumpOSWriteString2(hScript, ui32Flags);

	

	ui32NumPages = ui32NumBytes / ui32PageSize;
	for (ui32PageCounter = 0; ui32PageCounter < ui32NumPages; ui32PageCounter++)
	{
		__DtTestPoint(__DtFunc_PDumpFreePages, __DtStep_3)
		if (!bInterleaved || (ui32PageCounter % 2) == 0)
		{
			__DtTestPoint(__DtFunc_PDumpFreePages, __DtStep_4)
			sDevPAddr = psDeviceNode->pfnMMUGetPhysPageAddr(psBMHeap->pMMUHeap, sDevVAddr);
			{
				eErr = PDumpOSBufprintf(hScript, ui32MaxLen, "FREE :%s:PA_%08X%08X\r\n",
										psDeviceNode->sDevId.pszPDumpDevName, (IMG_UINT32)(IMG_UINTPTR_T)hUniqueTag, sDevPAddr.uiAddr);
				if(eErr != PVRSRV_OK)
				{
					__DtTestPoint(__DtFunc_PDumpFreePages, __DtStep_5)
					return eErr;
				}
				PDumpOSWriteString2(hScript, ui32Flags);
			}
		}
		else
		{
			
			__DtTestPoint(__DtFunc_PDumpFreePages, __DtStep_6)
		}

		sDevVAddr.uiAddr += ui32PageSize;
	}
	__DtTestPoint(__DtFunc_PDumpFreePages, __DtStep_7)
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpFreePageTable	(PVRSRV_DEVICE_IDENTIFIER *psDevID,
								 IMG_HANDLE hOSMemHandle,
								 IMG_CPU_VIRTADDR   pvLinAddr,
								 IMG_UINT32         ui32PTSize,
								 IMG_UINT32			ui32Flags,
								 IMG_HANDLE         hUniqueTag)
{
	PVRSRV_ERROR eErr;
	IMG_DEV_PHYADDR	sDevPAddr;

	__DtTestPoint(__DtFunc_PDumpFreePageTable, __DtStep_0)
	PDUMP_GET_SCRIPT_STRING();

	PVR_UNREFERENCED_PARAMETER(ui32PTSize);
	ui32Flags |= PDUMP_FLAGS_CONTINUOUS;
	ui32Flags |= ( _PDumpIsPersistent() ) ? PDUMP_FLAGS_PERSISTENT : 0;

	
	PVR_ASSERT(((IMG_UINTPTR_T)pvLinAddr & (ui32PTSize-1UL)) == 0);	

	

	eErr = PDumpOSBufprintf(hScript, ui32MaxLen, "-- FREE :%s:PAGE_TABLE\r\n", psDevID->pszPDumpDevName);
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpFreePageTable, __DtStep_1)
		return eErr;
	}
	PDumpOSWriteString2(hScript, ui32Flags);

	

	
	
	
	

	PDumpOSCPUVAddrToDevPAddr(psDevID->eDeviceType,
							  hOSMemHandle, 
			0,
			(IMG_PUINT8) pvLinAddr,
			ui32PTSize,
			&sDevPAddr);

	{
		eErr = PDumpOSBufprintf(hScript, ui32MaxLen, "FREE :%s:PA_%08X%08X\r\n",
								psDevID->pszPDumpDevName,
								(IMG_UINT32)(IMG_UINTPTR_T)hUniqueTag,
								sDevPAddr.uiAddr);
		if(eErr != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PDumpFreePageTable, __DtStep_2)
			return eErr;
		}
		PDumpOSWriteString2(hScript, ui32Flags);
	}

	__DtTestPoint(__DtFunc_PDumpFreePageTable, __DtStep_3)
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpPDRegWithFlags(PDUMP_MMU_ATTRIB *psMMUAttrib,
							IMG_UINT32 ui32Reg,
							 IMG_UINT32 ui32Data,
							 IMG_UINT32	ui32Flags,
							 IMG_HANDLE hUniqueTag)
{
	PVRSRV_ERROR eErr;
	IMG_CHAR *pszRegString;
	__DtTestPoint(__DtFunc_PDumpPDRegWithFlags, __DtStep_0)
	PDUMP_GET_SCRIPT_STRING()
	
	if(psMMUAttrib->pszPDRegRegion != IMG_NULL)
	{	
		__DtTestPoint(__DtFunc_PDumpPDRegWithFlags, __DtStep_1)
		pszRegString = psMMUAttrib->pszPDRegRegion;
	}
	else
	{
		__DtTestPoint(__DtFunc_PDumpPDRegWithFlags, __DtStep_2)
		pszRegString = psMMUAttrib->sDevId.pszPDumpRegName;
	}

	

#if defined(SGX_FEATURE_36BIT_MMU)
	eErr = PDumpOSBufprintf(hScript, ui32MaxLen,
			 "WRW :%s:$1 :%s:PA_%08X%08X:0x0\r\n",
			 psMMUAttrib->sDevId.pszPDumpDevName,
			 psMMUAttrib->sDevId.pszPDumpDevName,
			 (IMG_UINT32)hUniqueTag,
			 (ui32Data & psMMUAttrib->ui32PDEMask) << psMMUAttrib->ui32PDEAlignShift);
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpPDRegWithFlags, __DtStep_3)
		return eErr;
	}
	PDumpOSWriteString2(hScript, ui32Flags);
	eErr = PDumpOSBufprintf(hScript, ui32MaxLen, "SHR :%s:$1 :%s:$1 0x4\r\n", 
			psMMUAttrib->sDevId.pszPDumpDevName,
			psMMUAttrib->sDevId.pszPDumpDevName);
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpPDRegWithFlags, __DtStep_4)
		return eErr;
	}
	PDumpOSWriteString2(hScript, ui32Flags);
	eErr = PDumpOSBufprintf(hScript, ui32MaxLen,
			 "WRW :%s:0x%08X: %s:$1\r\n",
			 pszRegString,
			 ui32Reg,
			 psMMUAttrib->sDevId.pszPDumpDevName);
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpPDRegWithFlags, __DtStep_5)
		return eErr;
	}
	PDumpOSWriteString2(hScript, ui32Flags);
#else
	eErr = PDumpOSBufprintf(hScript,
				ui32MaxLen,
				"WRW :%s:0x%08X :%s:PA_%08X%08X:0x%08X\r\n",
				pszRegString,
				ui32Reg,
				psMMUAttrib->sDevId.pszPDumpDevName,
				(IMG_UINT32)(IMG_UINTPTR_T)hUniqueTag,
				(ui32Data & psMMUAttrib->ui32PDEMask) << psMMUAttrib->ui32PDEAlignShift,
				ui32Data & ~psMMUAttrib->ui32PDEMask);
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpPDRegWithFlags, __DtStep_6)
		return eErr;
	}
	PDumpOSWriteString2(hScript, ui32Flags);
#endif
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpPDReg	(PDUMP_MMU_ATTRIB *psMMUAttrib, 
					 IMG_UINT32 ui32Reg,
					 IMG_UINT32 ui32Data,
					 IMG_HANDLE hUniqueTag)
{
	__DtTestPoint(__DtFunc_PDumpPDReg, __DtStep_0)
	return PDumpPDRegWithFlags(psMMUAttrib, ui32Reg, ui32Data, PDUMP_FLAGS_CONTINUOUS, hUniqueTag);
}

PVRSRV_ERROR PDumpMemPolKM(PVRSRV_KERNEL_MEM_INFO		*psMemInfo,
						   IMG_UINT32			ui32Offset,
						   IMG_UINT32			ui32Value,
						   IMG_UINT32			ui32Mask,
						   PDUMP_POLL_OPERATOR	eOperator,
						   IMG_UINT32			ui32Flags,
						   IMG_HANDLE			hUniqueTag)
{
	#define MEMPOLL_DELAY		(1000)
	#define MEMPOLL_COUNT		(2000000000 / MEMPOLL_DELAY)

	PVRSRV_ERROR eErr;
	IMG_UINT32			ui32PageOffset;
	IMG_UINT8			*pui8LinAddr;
	IMG_DEV_PHYADDR		sDevPAddr;
	IMG_DEV_VIRTADDR	sDevVPageAddr;
	PDUMP_MMU_ATTRIB	*psMMUAttrib;

	PDUMP_GET_SCRIPT_STRING();
	if ( _PDumpIsPersistent() )
	{
		
		__DtTestPoint(__DtFunc_PDumpMemPolKM, __DtStep_0)
		return PVRSRV_OK;
	}

	
	PVR_ASSERT((ui32Offset + sizeof(IMG_UINT32)) <= psMemInfo->ui32AllocSize);

	psMMUAttrib = ((BM_BUF*)psMemInfo->sMemBlk.hBuffer)->pMapping->pBMHeap->psMMUAttrib;

	

	eErr = PDumpOSBufprintf(hScript,
			 ui32MaxLen,
			 "-- POL :%s:VA_%08X 0x%08X 0x%08X %d %d %d\r\n",
			 psMMUAttrib->sDevId.pszPDumpDevName,
			 psMemInfo->sDevVAddr.uiAddr + ui32Offset,
			 ui32Value,
			 ui32Mask,
			 eOperator,
			 MEMPOLL_COUNT,
			 MEMPOLL_DELAY);
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpMemPolKM, __DtStep_1)
		return eErr;
	}
	PDumpOSWriteString2(hScript, ui32Flags);


	pui8LinAddr = psMemInfo->pvLinAddrKM;

	
	pui8LinAddr += ui32Offset;

	


	PDumpOSCPUVAddrToPhysPages(psMemInfo->sMemBlk.hOSMemHandle,
			ui32Offset,
			pui8LinAddr,
			psMMUAttrib->ui32DataPageMask,
			&ui32PageOffset);

	
	sDevVPageAddr.uiAddr = psMemInfo->sDevVAddr.uiAddr + ui32Offset - ui32PageOffset;

	PVR_ASSERT((sDevVPageAddr.uiAddr & psMMUAttrib->ui32DataPageMask) == 0);

	
	BM_GetPhysPageAddr(psMemInfo, sDevVPageAddr, &sDevPAddr);

	
	sDevPAddr.uiAddr += ui32PageOffset;

	eErr = PDumpOSBufprintf(hScript,
			 ui32MaxLen,
			 "POL :%s:PA_%08X%08X:0x%08X 0x%08X 0x%08X %d %d %d\r\n",
			 psMMUAttrib->sDevId.pszPDumpDevName,
			 (IMG_UINT32)(IMG_UINTPTR_T)hUniqueTag,
			 sDevPAddr.uiAddr & ~(psMMUAttrib->ui32DataPageMask),
			 sDevPAddr.uiAddr & (psMMUAttrib->ui32DataPageMask),
			 ui32Value,
			 ui32Mask,
			 eOperator,
			 MEMPOLL_COUNT,
			 MEMPOLL_DELAY);
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpMemPolKM, __DtStep_2)
		return eErr;
	}
	PDumpOSWriteString2(hScript, ui32Flags);

	__DtTestPoint(__DtFunc_PDumpMemPolKM, __DtStep_3)
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpMemKM(IMG_PVOID pvAltLinAddr,
						PVRSRV_KERNEL_MEM_INFO *psMemInfo,
						IMG_UINT32 ui32Offset,
						IMG_UINT32 ui32Bytes,
						IMG_UINT32 ui32Flags,
						IMG_HANDLE hUniqueTag)
{
	PVRSRV_ERROR eErr;
	IMG_UINT32 ui32NumPages;
	IMG_UINT32 ui32PageByteOffset;
	IMG_UINT32 ui32BlockBytes;
	IMG_UINT8* pui8LinAddr;
	IMG_UINT8* pui8DataLinAddr = IMG_NULL;
	IMG_DEV_VIRTADDR sDevVPageAddr;
	IMG_DEV_VIRTADDR sDevVAddr;
	IMG_DEV_PHYADDR sDevPAddr;
	IMG_UINT32 ui32ParamOutPos;
	PDUMP_MMU_ATTRIB *psMMUAttrib;
	IMG_UINT32 ui32DataPageSize;

	__DtTestPoint(__DtFunc_PDumpMemKM, __DtStep_0)
	PDUMP_GET_SCRIPT_AND_FILE_STRING();
	
	psMMUAttrib = ((BM_BUF*)psMemInfo->sMemBlk.hBuffer)->pMapping->pBMHeap->psMMUAttrib;
	
	

	PVR_ASSERT((ui32Offset + ui32Bytes) <= psMemInfo->ui32AllocSize);

	if (!PDumpOSJTInitialised())
	{
		__DtTestPoint(__DtFunc_PDumpMemKM, __DtStep_1)
		return PVRSRV_ERROR_PDUMP_NOT_AVAILABLE;
	}

	 
	if (ui32Bytes == 0 || PDumpOSIsSuspended())
	{
		__DtTestPoint(__DtFunc_PDumpMemKM, __DtStep_2)
		return PVRSRV_OK;
	}

#if defined(SUPPORT_PDUMP_MULTI_PROCESS)
	
	{
		BM_HEAP *pHeap = ((BM_BUF*)psMemInfo->sMemBlk.hBuffer)->pMapping->pBMHeap;
		PVRSRV_DEVICE_NODE *psDeviceNode = pHeap->pBMContext->psDeviceNode;
		
		if( psDeviceNode->pfnMMUIsHeapShared(pHeap->pMMUHeap) )
		{
			__DtTestPoint(__DtFunc_PDumpMemKM, __DtStep_3)
			ui32Flags |= PDUMP_FLAGS_PERSISTENT;
		}
	}
#endif

	
	if(pvAltLinAddr)
	{
		__DtTestPoint(__DtFunc_PDumpMemKM, __DtStep_4)
		pui8DataLinAddr = pvAltLinAddr;
	}
	else if(psMemInfo->pvLinAddrKM)
	{
		__DtTestPoint(__DtFunc_PDumpMemKM, __DtStep_5)
		pui8DataLinAddr = (IMG_UINT8 *)psMemInfo->pvLinAddrKM + ui32Offset;
	}
	pui8LinAddr = (IMG_UINT8 *)psMemInfo->pvLinAddrKM;
	sDevVAddr = psMemInfo->sDevVAddr;

	
	sDevVAddr.uiAddr += ui32Offset;
	pui8LinAddr += ui32Offset;

	PVR_ASSERT(pui8DataLinAddr);

	PDumpOSCheckForSplitting(PDumpOSGetStream(PDUMP_STREAM_PARAM2), ui32Bytes, ui32Flags);

	ui32ParamOutPos = PDumpOSGetStreamOffset(PDUMP_STREAM_PARAM2);

	

	if(!PDumpOSWriteString(PDumpOSGetStream(PDUMP_STREAM_PARAM2),
						pui8DataLinAddr,
						ui32Bytes,
						ui32Flags))
	{
		__DtTestPoint(__DtFunc_PDumpMemKM, __DtStep_6)
		return PVRSRV_ERROR_PDUMP_BUFFER_FULL;
	}

	if (PDumpOSGetParamFileNum() == 0)
	{
		__DtTestPoint(__DtFunc_PDumpMemKM, __DtStep_7)
		eErr = PDumpOSSprintf(pszFileName, ui32MaxLenFileName, "%%0%%.prm");
	}
	else
	{
		__DtTestPoint(__DtFunc_PDumpMemKM, __DtStep_8)
		eErr = PDumpOSSprintf(pszFileName, ui32MaxLenFileName, "%%0%%_%u.prm", PDumpOSGetParamFileNum());
	}
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpMemKM, __DtStep_9)
		return eErr;
	}

	

	eErr = PDumpOSBufprintf(hScript,
			 ui32MaxLenScript,
			 "-- LDB :%s:VA_%08X%08X:0x%08X 0x%08X 0x%08X %s\r\n",
			 psMMUAttrib->sDevId.pszPDumpDevName,
			 (IMG_UINT32)(IMG_UINTPTR_T)hUniqueTag,
			 psMemInfo->sDevVAddr.uiAddr,
			 ui32Offset,
			 ui32Bytes,
			 ui32ParamOutPos,
			 pszFileName);
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpMemKM, __DtStep_10)
		return eErr;
	}
	PDumpOSWriteString2(hScript, ui32Flags);

	


	PDumpOSCPUVAddrToPhysPages(psMemInfo->sMemBlk.hOSMemHandle,
			ui32Offset,
			pui8LinAddr,
			psMMUAttrib->ui32DataPageMask,
			&ui32PageByteOffset);
	ui32DataPageSize = psMMUAttrib->ui32DataPageMask + 1;
	ui32NumPages = (ui32PageByteOffset + ui32Bytes + psMMUAttrib->ui32DataPageMask) / ui32DataPageSize;

	while(ui32NumPages)
	{
		__DtTestPoint(__DtFunc_PDumpMemKM, __DtStep_11)
		ui32NumPages--;
	
		
		sDevVPageAddr.uiAddr = sDevVAddr.uiAddr - ui32PageByteOffset;

		if (ui32DataPageSize <= PDUMP_TEMP_BUFFER_SIZE)
		{
			
			__DtTestPoint(__DtFunc_PDumpMemKM, __DtStep_12)
			PVR_ASSERT((sDevVPageAddr.uiAddr & psMMUAttrib->ui32DataPageMask) == 0);
		}

		
		BM_GetPhysPageAddr(psMemInfo, sDevVPageAddr, &sDevPAddr);

		
		sDevPAddr.uiAddr += ui32PageByteOffset;

		
		if (ui32PageByteOffset + ui32Bytes > ui32DataPageSize)
		{
			
			__DtTestPoint(__DtFunc_PDumpMemKM, __DtStep_13)
			ui32BlockBytes = ui32DataPageSize - ui32PageByteOffset;
		}
		else
		{
			
			__DtTestPoint(__DtFunc_PDumpMemKM, __DtStep_14)
			ui32BlockBytes = ui32Bytes;
		}

		eErr = PDumpOSBufprintf(hScript,
					 ui32MaxLenScript,
					 "LDB :%s:PA_%08X%08X:0x%08X 0x%08X 0x%08X %s\r\n",
					 psMMUAttrib->sDevId.pszPDumpDevName,
					 (IMG_UINT32)(IMG_UINTPTR_T)hUniqueTag,
					 sDevPAddr.uiAddr & ~(psMMUAttrib->ui32DataPageMask),
					 sDevPAddr.uiAddr & (psMMUAttrib->ui32DataPageMask),
					 ui32BlockBytes,
					 ui32ParamOutPos,
					 pszFileName);
		if(eErr != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PDumpMemKM, __DtStep_15)
			return eErr;
		}
		PDumpOSWriteString2(hScript, ui32Flags);

		

#if defined(SGX_FEATURE_VARIABLE_MMU_PAGE_SIZE)
		
		ui32PageByteOffset = (ui32PageByteOffset + ui32BlockBytes) % ui32DataPageSize;
#else
		
		ui32PageByteOffset = 0;
#endif
		
		ui32Bytes -= ui32BlockBytes;	 
		
		sDevVAddr.uiAddr += ui32BlockBytes;
		
		pui8LinAddr += ui32BlockBytes;
		
		ui32ParamOutPos += ui32BlockBytes;
	}

	__DtTestPoint(__DtFunc_PDumpMemKM, __DtStep_16)
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpMemPDEntriesKM(PDUMP_MMU_ATTRIB *psMMUAttrib,
								 IMG_HANDLE hOSMemHandle,
								 IMG_CPU_VIRTADDR pvLinAddr,
								 IMG_UINT32 ui32Bytes,
								 IMG_UINT32 ui32Flags,
								 IMG_BOOL bInitialisePages,
								 IMG_HANDLE hUniqueTag1,
								 IMG_HANDLE hUniqueTag2)
{
	PDUMP_MMU_ATTRIB sMMUAttrib;
	
	
	__DtTestPoint(__DtFunc_PDumpMemPDEntriesKM, __DtStep_0)
	sMMUAttrib = *psMMUAttrib;
	sMMUAttrib.ui32PTSize = HOST_PAGESIZE();
	__DtTestPoint(__DtFunc_PDumpMemPDEntriesKM, __DtStep_1)
	return PDumpMemPTEntriesKM(	&sMMUAttrib,
								hOSMemHandle,
								pvLinAddr,
								ui32Bytes,
								ui32Flags,
								bInitialisePages,
								hUniqueTag1,
								hUniqueTag2);
}

PVRSRV_ERROR PDumpMemPTEntriesKM(PDUMP_MMU_ATTRIB *psMMUAttrib,
								 IMG_HANDLE hOSMemHandle,
								 IMG_CPU_VIRTADDR pvLinAddr,
								 IMG_UINT32 ui32Bytes,
								 IMG_UINT32 ui32Flags,
								 IMG_BOOL bInitialisePages,
								 IMG_HANDLE hUniqueTag1,
								 IMG_HANDLE hUniqueTag2)
{
	PVRSRV_ERROR eErr;
	IMG_UINT32 ui32NumPages;
	IMG_UINT32 ui32PageOffset;
	IMG_UINT32 ui32BlockBytes;
	IMG_UINT8* pui8LinAddr;
	IMG_DEV_PHYADDR sDevPAddr;
	IMG_CPU_PHYADDR sCpuPAddr;
	IMG_UINT32 ui32Offset;
	IMG_UINT32 ui32ParamOutPos;
	IMG_UINT32 ui32PageMask; 

	__DtTestPoint(__DtFunc_PDumpMemPTEntriesKM, __DtStep_0)
	PDUMP_GET_SCRIPT_AND_FILE_STRING();
	ui32Flags |= ( _PDumpIsPersistent() ) ? PDUMP_FLAGS_PERSISTENT : 0;

	if (!PDumpOSJTInitialised())
	{
		__DtTestPoint(__DtFunc_PDumpMemPTEntriesKM, __DtStep_1)
		return PVRSRV_ERROR_PDUMP_NOT_AVAILABLE;
	}

	if (!pvLinAddr)
	{
		__DtTestPoint(__DtFunc_PDumpMemPTEntriesKM, __DtStep_2)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	if (PDumpOSIsSuspended())
	{
		__DtTestPoint(__DtFunc_PDumpMemPTEntriesKM, __DtStep_3)
		return PVRSRV_OK;
	}

	PDumpOSCheckForSplitting(PDumpOSGetStream(PDUMP_STREAM_PARAM2), ui32Bytes, ui32Flags);

	ui32ParamOutPos = PDumpOSGetStreamOffset(PDUMP_STREAM_PARAM2);

	if (bInitialisePages)
	{
		


		__DtTestPoint(__DtFunc_PDumpMemPTEntriesKM, __DtStep_4)
		if (!PDumpOSWriteString(PDumpOSGetStream(PDUMP_STREAM_PARAM2),
							pvLinAddr,
							ui32Bytes,
							ui32Flags | PDUMP_FLAGS_CONTINUOUS))
		{
			__DtTestPoint(__DtFunc_PDumpMemPTEntriesKM, __DtStep_5)
			return PVRSRV_ERROR_PDUMP_BUFFER_FULL;
		}

		if (PDumpOSGetParamFileNum() == 0)
		{
			__DtTestPoint(__DtFunc_PDumpMemPTEntriesKM, __DtStep_6)
			eErr = PDumpOSSprintf(pszFileName, ui32MaxLenFileName, "%%0%%.prm");
		}
		else
		{
			__DtTestPoint(__DtFunc_PDumpMemPTEntriesKM, __DtStep_7)
			eErr = PDumpOSSprintf(pszFileName, ui32MaxLenFileName, "%%0%%_%u.prm", PDumpOSGetParamFileNum());
		}
		if(eErr != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PDumpMemPTEntriesKM, __DtStep_8)
			return eErr;
		}
	}

	




	ui32PageMask = psMMUAttrib->ui32PTSize - 1;

	

	
 	ui32PageOffset	= (IMG_UINT32)((IMG_UINTPTR_T)pvLinAddr & (psMMUAttrib->ui32PTSize - 1));
 	ui32NumPages	= (ui32PageOffset + ui32Bytes + psMMUAttrib->ui32PTSize - 1) / psMMUAttrib->ui32PTSize;
	pui8LinAddr		= (IMG_UINT8*) pvLinAddr;

	while (ui32NumPages)
	{
		__DtTestPoint(__DtFunc_PDumpMemPTEntriesKM, __DtStep_9)
		ui32NumPages--;
		





		sCpuPAddr = OSMapLinToCPUPhys(hOSMemHandle, pui8LinAddr);
		sDevPAddr = SysCpuPAddrToDevPAddr(psMMUAttrib->sDevId.eDeviceType, sCpuPAddr);

		
		if (ui32PageOffset + ui32Bytes > psMMUAttrib->ui32PTSize)
		{
			
			__DtTestPoint(__DtFunc_PDumpMemPTEntriesKM, __DtStep_10)
			ui32BlockBytes = psMMUAttrib->ui32PTSize - ui32PageOffset;
		}
		else
		{
			
			__DtTestPoint(__DtFunc_PDumpMemPTEntriesKM, __DtStep_11)
			ui32BlockBytes = ui32Bytes;
		}

		

		
		if (bInitialisePages)
		{
			__DtTestPoint(__DtFunc_PDumpMemPTEntriesKM, __DtStep_12)
			eErr = PDumpOSBufprintf(hScript,
					 ui32MaxLenScript,
					 "LDB :%s:PA_%08X%08X:0x%08X 0x%08X 0x%08X %s\r\n",
					 psMMUAttrib->sDevId.pszPDumpDevName,
					 (IMG_UINT32)(IMG_UINTPTR_T)hUniqueTag1,
					 sDevPAddr.uiAddr & ~ui32PageMask,
					 sDevPAddr.uiAddr & ui32PageMask,
					 ui32BlockBytes,
					 ui32ParamOutPos,
					 pszFileName);
			if(eErr != PVRSRV_OK)
			{
				__DtTestPoint(__DtFunc_PDumpMemPTEntriesKM, __DtStep_13)
				return eErr;
			}
			PDumpOSWriteString2(hScript, ui32Flags | PDUMP_FLAGS_CONTINUOUS);
		}
		else
		{
			__DtTestPoint(__DtFunc_PDumpMemPTEntriesKM, __DtStep_14)
			for (ui32Offset = 0; ui32Offset < ui32BlockBytes; ui32Offset += sizeof(IMG_UINT32))
			{
				IMG_UINT32 ui32PTE = *((IMG_UINT32 *) (pui8LinAddr + ui32Offset));  

				__DtTestPoint(__DtFunc_PDumpMemPTEntriesKM, __DtStep_15)
				if ((ui32PTE & psMMUAttrib->ui32PDEMask) != 0)
				{
					
#if defined(SGX_FEATURE_36BIT_MMU)
					eErr = PDumpOSBufprintf(hScript,
							ui32MaxLenScript,
							 "WRW :%s:$1 :%s:PA_%08X%08X:0x0\r\n",
							 psMMUAttrib->sDevId.pszPDumpDevName,
							 psMMUAttrib->sDevId.pszPDumpDevName,
							 (IMG_UINT32)hUniqueTag2,
							 (ui32PTE & psMMUAttrib->ui32PDEMask) << psMMUAttrib->ui32PTEAlignShift);
					if(eErr != PVRSRV_OK)
					{
						__DtTestPoint(__DtFunc_PDumpMemPTEntriesKM, __DtStep_16)
						return eErr;
					}
					PDumpOSWriteString2(hScript, ui32Flags | PDUMP_FLAGS_CONTINUOUS);
					eErr = PDumpOSBufprintf(hScript, ui32MaxLenScript, "SHR :%s:$1 :%s:$1 0x4\r\n",
								psMMUAttrib->sDevId.pszPDumpDevName,
								psMMUAttrib->sDevId.pszPDumpDevName);
					if(eErr != PVRSRV_OK)
					{
						__DtTestPoint(__DtFunc_PDumpMemPTEntriesKM, __DtStep_17)
						return eErr;
					}
					PDumpOSWriteString2(hScript, ui32Flags | PDUMP_FLAGS_CONTINUOUS);
					eErr = PDumpOSBufprintf(hScript, ui32MaxLenScript, "OR :%s:$1 :%s:$1 0x%08X\r\n",
								psMMUAttrib->sDevId.pszPDumpDevName,
								psMMUAttrib->sDevId.pszPDumpDevName,
								ui32PTE & ~psMMUAttrib->ui32PDEMask);
					if(eErr != PVRSRV_OK)
					{
						__DtTestPoint(__DtFunc_PDumpMemPTEntriesKM, __DtStep_18)
						return eErr;
					}
					PDumpOSWriteString2(hScript, ui32Flags | PDUMP_FLAGS_CONTINUOUS);
					eErr = PDumpOSBufprintf(hScript,
							ui32MaxLenScript,
							 "WRW :%s:PA_%08X%08X:0x%08X :%s:$1\r\n",
							 psMMUAttrib->sDevId.pszPDumpDevName,
							 (IMG_UINT32)hUniqueTag1,
							 (sDevPAddr.uiAddr + ui32Offset) & ~ui32PageMask,
							 (sDevPAddr.uiAddr + ui32Offset) & ui32PageMask,
							 psMMUAttrib->sDevId.pszPDumpDevName);
					if(eErr != PVRSRV_OK)
					{
						__DtTestPoint(__DtFunc_PDumpMemPTEntriesKM, __DtStep_19)
						return eErr;
					}
					PDumpOSWriteString2(hScript, ui32Flags | PDUMP_FLAGS_CONTINUOUS);
#else
					eErr = PDumpOSBufprintf(hScript,
							ui32MaxLenScript,
							 "WRW :%s:PA_%08X%08X:0x%08X :%s:PA_%08X%08X:0x%08X\r\n",
							 psMMUAttrib->sDevId.pszPDumpDevName,
							 (IMG_UINT32)(IMG_UINTPTR_T)hUniqueTag1,
							 (sDevPAddr.uiAddr + ui32Offset) & ~ui32PageMask,
							 (sDevPAddr.uiAddr + ui32Offset) & ui32PageMask,
							 psMMUAttrib->sDevId.pszPDumpDevName,
							 (IMG_UINT32)(IMG_UINTPTR_T)hUniqueTag2,
							 (ui32PTE & psMMUAttrib->ui32PDEMask) << psMMUAttrib->ui32PTEAlignShift,
							 ui32PTE & ~psMMUAttrib->ui32PDEMask);
					if(eErr != PVRSRV_OK)
					{
						__DtTestPoint(__DtFunc_PDumpMemPTEntriesKM, __DtStep_20)
						return eErr;
					}
					PDumpOSWriteString2(hScript, ui32Flags | PDUMP_FLAGS_CONTINUOUS);
#endif
				}
				else
				{
					__DtTestPoint(__DtFunc_PDumpMemPTEntriesKM, __DtStep_21)
					PVR_ASSERT((ui32PTE & psMMUAttrib->ui32PTEValid) == 0UL);
					eErr = PDumpOSBufprintf(hScript,
							ui32MaxLenScript,
							 "WRW :%s:PA_%08X%08X:0x%08X 0x%08X%08X\r\n",
							 psMMUAttrib->sDevId.pszPDumpDevName,
							 (IMG_UINT32)(IMG_UINTPTR_T)hUniqueTag1,
							 (sDevPAddr.uiAddr + ui32Offset) & ~ui32PageMask,
							 (sDevPAddr.uiAddr + ui32Offset) & ui32PageMask,
							 (ui32PTE << psMMUAttrib->ui32PTEAlignShift),
							 (IMG_UINT32)(IMG_UINTPTR_T)hUniqueTag2);
					if(eErr != PVRSRV_OK)
					{
						__DtTestPoint(__DtFunc_PDumpMemPTEntriesKM, __DtStep_22)
						return eErr;
					}
					PDumpOSWriteString2(hScript, ui32Flags | PDUMP_FLAGS_CONTINUOUS);
				}
			}
		}

		

		
		ui32PageOffset = 0;
		
		ui32Bytes -= ui32BlockBytes;
		
		pui8LinAddr += ui32BlockBytes;
		
		ui32ParamOutPos += ui32BlockBytes;
	}

	__DtTestPoint(__DtFunc_PDumpMemPTEntriesKM, __DtStep_23)
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpPDDevPAddrKM(PVRSRV_KERNEL_MEM_INFO *psMemInfo,
							   IMG_UINT32 ui32Offset,
							   IMG_DEV_PHYADDR sPDDevPAddr,
							   IMG_HANDLE hUniqueTag1,
							   IMG_HANDLE hUniqueTag2)
{
	PVRSRV_ERROR eErr;
	IMG_UINT32 ui32PageByteOffset;
	IMG_DEV_VIRTADDR sDevVAddr;
	IMG_DEV_VIRTADDR sDevVPageAddr;
	IMG_DEV_PHYADDR sDevPAddr;
	IMG_UINT32 ui32Flags = PDUMP_FLAGS_CONTINUOUS;
	IMG_UINT32 ui32ParamOutPos;
	PDUMP_MMU_ATTRIB *psMMUAttrib;
	IMG_UINT32 ui32PageMask; 

	__DtTestPoint(__DtFunc_PDumpPDDevPAddrKM, __DtStep_0)
	PDUMP_GET_SCRIPT_AND_FILE_STRING();

	if (!PDumpOSJTInitialised())
	{
		__DtTestPoint(__DtFunc_PDumpPDDevPAddrKM, __DtStep_1)
		return PVRSRV_ERROR_PDUMP_NOT_AVAILABLE;
	}

	psMMUAttrib = ((BM_BUF*)psMemInfo->sMemBlk.hBuffer)->pMapping->pBMHeap->psMMUAttrib;
	ui32PageMask = psMMUAttrib->ui32PTSize - 1;

	ui32ParamOutPos = PDumpOSGetStreamOffset(PDUMP_STREAM_PARAM2);

	
	if(!PDumpOSWriteString(PDumpOSGetStream(PDUMP_STREAM_PARAM2),
						(IMG_UINT8 *)&sPDDevPAddr,
						sizeof(IMG_DEV_PHYADDR),
						ui32Flags))
	{
		__DtTestPoint(__DtFunc_PDumpPDDevPAddrKM, __DtStep_2)
		return PVRSRV_ERROR_PDUMP_BUFFER_FULL;
	}

	if (PDumpOSGetParamFileNum() == 0)
	{
		__DtTestPoint(__DtFunc_PDumpPDDevPAddrKM, __DtStep_3)
		eErr = PDumpOSSprintf(pszFileName, ui32MaxLenFileName, "%%0%%.prm");
	}
	else
	{
		__DtTestPoint(__DtFunc_PDumpPDDevPAddrKM, __DtStep_4)
		eErr = PDumpOSSprintf(pszFileName, ui32MaxLenFileName, "%%0%%_%u.prm", PDumpOSGetParamFileNum());
	}
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpPDDevPAddrKM, __DtStep_5)
		return eErr;
	}

	
	eErr = PDumpOSBufprintf(hScript,
			ui32MaxLenScript,
			"-- LDB :%s:PA_0x%08X%08X:0x%08X 0x%08X 0x%08X %s\r\n",
			psMMUAttrib->sDevId.pszPDumpDevName,
			(IMG_UINT32)(IMG_UINTPTR_T)hUniqueTag1,
			sPDDevPAddr.uiAddr & ~ui32PageMask,
			sPDDevPAddr.uiAddr & ui32PageMask,
			sizeof(IMG_DEV_PHYADDR),
			ui32ParamOutPos,
			pszFileName);
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpPDDevPAddrKM, __DtStep_6)
		return eErr;
	}
	PDumpOSWriteString2(hScript, ui32Flags);

	
	sDevVAddr = psMemInfo->sDevVAddr;
	ui32PageByteOffset = sDevVAddr.uiAddr & ui32PageMask;

	sDevVPageAddr.uiAddr = sDevVAddr.uiAddr - ui32PageByteOffset;
	PVR_ASSERT((sDevVPageAddr.uiAddr & 0xFFF) == 0);

	BM_GetPhysPageAddr(psMemInfo, sDevVPageAddr, &sDevPAddr);
	sDevPAddr.uiAddr += ui32PageByteOffset + ui32Offset;

	if ((sPDDevPAddr.uiAddr & psMMUAttrib->ui32PDEMask) != 0UL)
	{
#if defined(SGX_FEATURE_36BIT_MMU)
		eErr = PDumpOSBufprintf(hScript,
				ui32MaxLenScript,
				 "WRW :%s:$1 :%s:PA_%08X%08X:0x0\r\n",
				 psMMUAttrib->sDevId.pszPDumpDevName,
				 psMMUAttrib->sDevId.pszPDumpDevName,
				 (IMG_UINT32)hUniqueTag2,
				 sPDDevPAddr.uiAddr);
		if(eErr != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PDumpPDDevPAddrKM, __DtStep_7)
			return eErr;
		}
		PDumpOSWriteString2(hScript, ui32Flags);

		eErr = PDumpOSBufprintf(hScript, ui32MaxLenScript, "AND  :%s:$2 :%s:$1 0xFFFFFFFF\r\n",
					psMMUAttrib->sDevId.pszPDumpDevName,
					psMMUAttrib->sDevId.pszPDumpDevName);
		if(eErr != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PDumpPDDevPAddrKM, __DtStep_8)
			return eErr;
		}
		PDumpOSWriteString2(hScript, ui32Flags);

		eErr = PDumpOSBufprintf(hScript,
				ui32MaxLenScript,
				 "WRW :%s:PA_%08X%08X:0x%08X :%s:$2\r\n",
				 psMMUAttrib->sDevId.pszPDumpDevName,
				 (IMG_UINT32)hUniqueTag1,
				 (sDevPAddr.uiAddr) & ~(psMMUAttrib->ui32DataPageMask),
				 (sDevPAddr.uiAddr) & (psMMUAttrib->ui32DataPageMask),
				 psMMUAttrib->sDevId.pszPDumpDevName);
		if(eErr != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PDumpPDDevPAddrKM, __DtStep_9)
			return eErr;
		}
		PDumpOSWriteString2(hScript, ui32Flags);

		eErr = PDumpOSBufprintf(hScript, ui32MaxLenScript, "SHR :%s:$2 :%s:$1 0x20\r\n",
				psMMUAttrib->sDevId.pszPDumpDevName,
				psMMUAttrib->sDevId.pszPDumpDevName);
		if(eErr != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PDumpPDDevPAddrKM, __DtStep_10)
			return eErr;
		}
		PDumpOSWriteString2(hScript, ui32Flags);

		eErr = PDumpOSBufprintf(hScript,
				ui32MaxLenScript,
				 "WRW :%s:PA_%08X%08X:0x%08X :%s:$2\r\n",
				 psMMUAttrib->sDevId.pszPDumpDevName,
				 (IMG_UINT32)hUniqueTag1,
				 (sDevPAddr.uiAddr + 4) & ~(psMMUAttrib->ui32DataPageMask),
				 (sDevPAddr.uiAddr + 4) & (psMMUAttrib->ui32DataPageMask),
				 psMMUAttrib->sDevId.pszPDumpDevName);
		if(eErr != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PDumpPDDevPAddrKM, __DtStep_11)
			return eErr;
		}
		PDumpOSWriteString2(hScript, ui32Flags);
#else
		eErr = PDumpOSBufprintf(hScript,
				 ui32MaxLenScript,
				 "WRW :%s:PA_%08X%08X:0x%08X :%s:PA_%08X%08X:0x%08X\r\n",
				 psMMUAttrib->sDevId.pszPDumpDevName,
				 (IMG_UINT32)(IMG_UINTPTR_T)hUniqueTag1,
				 sDevPAddr.uiAddr & ~ui32PageMask,
				 sDevPAddr.uiAddr & ui32PageMask,
				 psMMUAttrib->sDevId.pszPDumpDevName,
				 (IMG_UINT32)(IMG_UINTPTR_T)hUniqueTag2,
				 sPDDevPAddr.uiAddr & psMMUAttrib->ui32PDEMask,
				 sPDDevPAddr.uiAddr & ~psMMUAttrib->ui32PDEMask);
		if(eErr != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PDumpPDDevPAddrKM, __DtStep_12)
			return eErr;
		}
#endif
	}
	else
	{
		__DtTestPoint(__DtFunc_PDumpPDDevPAddrKM, __DtStep_13)
		PVR_ASSERT(!(sDevPAddr.uiAddr & psMMUAttrib->ui32PTEValid));
		eErr = PDumpOSBufprintf(hScript,
				 ui32MaxLenScript,
				 "WRW :%s:PA_%08X%08X:0x%08X 0x%08X\r\n",
				 psMMUAttrib->sDevId.pszPDumpDevName,
				 (IMG_UINT32)(IMG_UINTPTR_T)hUniqueTag1,
				 sDevPAddr.uiAddr & ~ui32PageMask,
				 sDevPAddr.uiAddr & ui32PageMask,
				 sPDDevPAddr.uiAddr);
		if(eErr != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PDumpPDDevPAddrKM, __DtStep_14)
			return eErr;
		}
	}
	PDumpOSWriteString2(hScript, ui32Flags);

	__DtTestPoint(__DtFunc_PDumpPDDevPAddrKM, __DtStep_15)
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpCommentKM(IMG_CHAR *pszComment, IMG_UINT32 ui32Flags)
{
	PVRSRV_ERROR eErr;
	IMG_CHAR pszCommentPrefix[] = "-- "; 
#if defined(PDUMP_DEBUG_OUTFILES)
	IMG_CHAR pszTemp[256];
#endif
	IMG_UINT32 ui32LenCommentPrefix;
	PDUMP_GET_SCRIPT_STRING();
	PDUMP_DBG(("PDumpCommentKM"));
#if defined(PDUMP_DEBUG_OUTFILES)
	
	ui32Flags |= ( _PDumpIsPersistent() ) ? PDUMP_FLAGS_PERSISTENT : 0;
#endif
	
	PDumpOSVerifyLineEnding(pszComment, ui32MaxLen);

	
	ui32LenCommentPrefix = PDumpOSBuflen(pszCommentPrefix, sizeof(pszCommentPrefix));

	
	if (!PDumpOSWriteString(PDumpOSGetStream(PDUMP_STREAM_SCRIPT2),
			  (IMG_UINT8*)pszCommentPrefix,
			  ui32LenCommentPrefix,
			  ui32Flags))
	{
		__DtTestPoint(__DtFunc_PDumpCommentKM, __DtStep_0)
		if(ui32Flags & PDUMP_FLAGS_CONTINUOUS)
		{
			__DtTestPoint(__DtFunc_PDumpCommentKM, __DtStep_1)
			return PVRSRV_ERROR_PDUMP_BUFFER_FULL;
		}
		else
		{
			__DtTestPoint(__DtFunc_PDumpCommentKM, __DtStep_2)
			return PVRSRV_ERROR_CMD_NOT_PROCESSED;
		}
	}
#if defined(PDUMP_DEBUG_OUTFILES)
	
	eErr = PDumpOSSprintf(pszTemp, 256, "%d-%d %s",
		_PDumpGetPID(),
		g_ui32EveryLineCounter,
		pszComment);

	
	eErr = PDumpOSBufprintf(hScript, ui32MaxLen, "%s",
		pszTemp);
#else
	eErr = PDumpOSBufprintf(hScript, ui32MaxLen, "%s",
		pszComment);
#endif
	if( (eErr != PVRSRV_OK) &&
		(eErr != PVRSRV_ERROR_PDUMP_BUF_OVERFLOW))
	{
		__DtTestPoint(__DtFunc_PDumpCommentKM, __DtStep_3)
		return eErr;
	}
	PDumpOSWriteString2(hScript, ui32Flags);
	__DtTestPoint(__DtFunc_PDumpCommentKM, __DtStep_4)
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpCommentWithFlags(IMG_UINT32 ui32Flags, IMG_CHAR * pszFormat, ...)
{
	PVRSRV_ERROR eErr;
	PDUMP_va_list ap;
	__DtTestPoint(__DtFunc_PDumpCommentWithFlags, __DtStep_0)
	PDUMP_GET_MSG_STRING();

	
	PDUMP_va_start(ap, pszFormat);
	eErr = PDumpOSVSprintf(pszMsg, ui32MaxLen, pszFormat, ap);
	PDUMP_va_end(ap);

	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpCommentWithFlags, __DtStep_1)
		return eErr;
	}
	__DtTestPoint(__DtFunc_PDumpCommentWithFlags, __DtStep_2)
	return PDumpCommentKM(pszMsg, ui32Flags);
}

PVRSRV_ERROR PDumpComment(IMG_CHAR *pszFormat, ...)
{
	PVRSRV_ERROR eErr;
	PDUMP_va_list ap;
	__DtTestPoint(__DtFunc_PDumpComment, __DtStep_0)
	PDUMP_GET_MSG_STRING();

	
	PDUMP_va_start(ap, pszFormat);
	eErr = PDumpOSVSprintf(pszMsg, ui32MaxLen, pszFormat, ap);
	PDUMP_va_end(ap);

	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpComment, __DtStep_1)
		return eErr;
	}
	__DtTestPoint(__DtFunc_PDumpComment, __DtStep_2)
	return PDumpCommentKM(pszMsg, PDUMP_FLAGS_CONTINUOUS);
}

PVRSRV_ERROR PDumpDriverInfoKM(IMG_CHAR *pszString, IMG_UINT32 ui32Flags)
{
	PVRSRV_ERROR eErr;
	IMG_UINT32	ui32MsgLen;
	__DtTestPoint(__DtFunc_PDumpDriverInfoKM, __DtStep_0)
	PDUMP_GET_MSG_STRING();

	
	eErr = PDumpOSSprintf(pszMsg, ui32MaxLen, "%s", pszString);
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpDriverInfoKM, __DtStep_1)
		return eErr;
	}

	
	PDumpOSVerifyLineEnding(pszMsg, ui32MaxLen);
	ui32MsgLen = PDumpOSBuflen(pszMsg, ui32MaxLen);

	if	(!PDumpOSWriteString(PDumpOSGetStream(PDUMP_STREAM_DRIVERINFO),
						  (IMG_UINT8*)pszMsg,
						  ui32MsgLen,
						  ui32Flags))
	{
		__DtTestPoint(__DtFunc_PDumpDriverInfoKM, __DtStep_2)
		if	(ui32Flags & PDUMP_FLAGS_CONTINUOUS)
		{
			__DtTestPoint(__DtFunc_PDumpDriverInfoKM, __DtStep_3)
			return PVRSRV_ERROR_PDUMP_BUFFER_FULL;
		}
		else
		{
			__DtTestPoint(__DtFunc_PDumpDriverInfoKM, __DtStep_4)
			return PVRSRV_ERROR_CMD_NOT_PROCESSED;
		}
	}
	__DtTestPoint(__DtFunc_PDumpDriverInfoKM, __DtStep_5)
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpBitmapKM(	PVRSRV_DEVICE_NODE *psDeviceNode,
							IMG_CHAR *pszFileName,
							IMG_UINT32 ui32FileOffset,
							IMG_UINT32 ui32Width,
							IMG_UINT32 ui32Height,
							IMG_UINT32 ui32StrideInBytes,
							IMG_DEV_VIRTADDR sDevBaseAddr,
							IMG_HANDLE hDevMemContext,
							IMG_UINT32 ui32Size,
							PDUMP_PIXEL_FORMAT ePixelFormat,
							PDUMP_MEM_FORMAT eMemFormat,
							IMG_UINT32 ui32PDumpFlags)
{
	PVRSRV_DEVICE_IDENTIFIER *psDevId = &psDeviceNode->sDevId;
	
	PVRSRV_ERROR eErr;
	__DtTestPoint(__DtFunc_PDumpBitmapKM, __DtStep_0)
	PDUMP_GET_SCRIPT_STRING();

	if ( _PDumpIsPersistent() )
	{
		__DtTestPoint(__DtFunc_PDumpBitmapKM, __DtStep_1)
		return PVRSRV_OK;
	}

	PDumpCommentWithFlags(ui32PDumpFlags, "\r\n-- Dump bitmap of render\r\n");

	
	
	
	PVR_UNREFERENCED_PARAMETER(hDevMemContext);
	
#if defined(SGX_FEATURE_MULTIPLE_MEM_CONTEXTS)
	
	eErr = PDumpOSBufprintf(hScript,
				ui32MaxLen,
				"SII %s %s.bin :%s:v%x:0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X\r\n",
				pszFileName,
				pszFileName,
				psDevId->pszPDumpDevName,
				PDUMP_DATAMASTER_PIXEL,
				sDevBaseAddr.uiAddr,
				ui32Size,
				ui32FileOffset,
				ePixelFormat,
				ui32Width,
				ui32Height,
				ui32StrideInBytes,
				eMemFormat);
#else
	eErr = PDumpOSBufprintf(hScript,
				ui32MaxLen,
				"SII %s %s.bin :%s:v:0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X\r\n",
				pszFileName,
				pszFileName,
				psDevId->pszPDumpDevName,
				sDevBaseAddr.uiAddr,
				ui32Size,
				ui32FileOffset,
				ePixelFormat,
				ui32Width,
				ui32Height,
				ui32StrideInBytes,
				eMemFormat);
#endif
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpBitmapKM, __DtStep_2)
		return eErr;
	}

	PDumpOSWriteString2( hScript, ui32PDumpFlags);
	__DtTestPoint(__DtFunc_PDumpBitmapKM, __DtStep_3)
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpReadRegKM		(	IMG_CHAR *pszPDumpRegName,
									IMG_CHAR *pszFileName,
									IMG_UINT32 ui32FileOffset,
									IMG_UINT32 ui32Address,
									IMG_UINT32 ui32Size,
									IMG_UINT32 ui32PDumpFlags)
{
	PVRSRV_ERROR eErr;
	__DtTestPoint(__DtFunc_PDumpReadRegKM, __DtStep_0)
	PDUMP_GET_SCRIPT_STRING();

	PVR_UNREFERENCED_PARAMETER(ui32Size);

	eErr = PDumpOSBufprintf(hScript,
			ui32MaxLen,
			"SAB :%s:0x%08X 0x%08X %s\r\n",
			pszPDumpRegName,
			ui32Address,
			ui32FileOffset,
			pszFileName);
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpReadRegKM, __DtStep_1)
		return eErr;
	}

	PDumpOSWriteString2( hScript, ui32PDumpFlags);

	__DtTestPoint(__DtFunc_PDumpReadRegKM, __DtStep_2)
	return PVRSRV_OK;
}

IMG_BOOL PDumpTestNextFrame(IMG_UINT32 ui32CurrentFrame)
{
	IMG_BOOL	bFrameDumped;

	

	__DtTestPoint(__DtFunc_PDumpTestNextFrame, __DtStep_0)
	(IMG_VOID) PDumpSetFrameKM(ui32CurrentFrame + 1);
	bFrameDumped = PDumpIsCaptureFrameKM();
	(IMG_VOID) PDumpSetFrameKM(ui32CurrentFrame);

	__DtTestPoint(__DtFunc_PDumpTestNextFrame, __DtStep_1)
	return bFrameDumped;
}

static PVRSRV_ERROR PDumpSignatureRegister	(PVRSRV_DEVICE_IDENTIFIER *psDevId,
									 IMG_CHAR	*pszFileName,
									 IMG_UINT32		ui32Address,
									 IMG_UINT32		ui32Size,
									 IMG_UINT32		*pui32FileOffset,
									 IMG_UINT32		ui32Flags)
{
	PVRSRV_ERROR eErr;
	__DtTestPoint(__DtFunc_PDumpSignatureRegister, __DtStep_0)
	PDUMP_GET_SCRIPT_STRING();

	eErr = PDumpOSBufprintf(hScript,
			ui32MaxLen,
			"SAB :%s:0x%08X 0x%08X %s\r\n",
			psDevId->pszPDumpRegName,
			ui32Address,
			*pui32FileOffset,
			pszFileName);
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpSignatureRegister, __DtStep_1)
		return eErr;
	}

	PDumpOSWriteString2(hScript, ui32Flags);
	*pui32FileOffset += ui32Size;
	__DtTestPoint(__DtFunc_PDumpSignatureRegister, __DtStep_2)
	return PVRSRV_OK;
}

static IMG_VOID PDumpRegisterRange(PVRSRV_DEVICE_IDENTIFIER *psDevId,
									IMG_CHAR *pszFileName,
									IMG_UINT32 *pui32Registers,
									IMG_UINT32  ui32NumRegisters,
									IMG_UINT32 *pui32FileOffset,
									IMG_UINT32	ui32Size,
									IMG_UINT32	ui32Flags)
{
	IMG_UINT32 i;
	__DtTestPoint(__DtFunc_PDumpRegisterRange, __DtStep_0)
	for (i = 0; i < ui32NumRegisters; i++)
	{
		__DtTestPoint(__DtFunc_PDumpRegisterRange, __DtStep_1)
		PDumpSignatureRegister(psDevId, pszFileName, pui32Registers[i], ui32Size, pui32FileOffset, ui32Flags);
	}
	__DtTestPoint(__DtFunc_PDumpRegisterRange, __DtStep_2)
}

PVRSRV_ERROR PDump3DSignatureRegisters(PVRSRV_DEVICE_IDENTIFIER *psDevId,
										IMG_UINT32 ui32DumpFrameNum,
										IMG_BOOL bLastFrame,
										IMG_UINT32 *pui32Registers,
										IMG_UINT32 ui32NumRegisters)
{
	PVRSRV_ERROR eErr;
	IMG_UINT32	ui32FileOffset, ui32Flags;

	__DtTestPoint(__DtFunc_PDump3DSignatureRegisters, __DtStep_0)
	PDUMP_GET_FILE_STRING();

	ui32Flags = bLastFrame ? PDUMP_FLAGS_LASTFRAME : 0;
	ui32FileOffset = 0;

	PDumpCommentWithFlags(ui32Flags, "\r\n-- Dump 3D signature registers\r\n");
	eErr = PDumpOSSprintf(pszFileName, ui32MaxLen, "out%u_3d.sig", ui32DumpFrameNum);
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDump3DSignatureRegisters, __DtStep_1)
		return eErr;
	}

	PDumpRegisterRange(psDevId,
						pszFileName,
						pui32Registers,
						ui32NumRegisters,
						&ui32FileOffset,
						sizeof(IMG_UINT32),
						ui32Flags);

	__DtTestPoint(__DtFunc_PDump3DSignatureRegisters, __DtStep_2)
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpTASignatureRegisters	(PVRSRV_DEVICE_IDENTIFIER *psDevId,
			 IMG_UINT32 ui32DumpFrameNum,
			 IMG_UINT32	ui32TAKickCount,
			 IMG_BOOL	bLastFrame,
			 IMG_UINT32 *pui32Registers,
			 IMG_UINT32 ui32NumRegisters)
{
	PVRSRV_ERROR eErr;
	IMG_UINT32	ui32FileOffset, ui32Flags;

	__DtTestPoint(__DtFunc_PDumpTASignatureRegisters, __DtStep_0)
	PDUMP_GET_FILE_STRING();

	ui32Flags = bLastFrame ? PDUMP_FLAGS_LASTFRAME : 0;
	ui32FileOffset = ui32TAKickCount * ui32NumRegisters * sizeof(IMG_UINT32);

	PDumpCommentWithFlags(ui32Flags, "\r\n-- Dump TA signature registers\r\n");
	eErr = PDumpOSSprintf(pszFileName, ui32MaxLen, "out%u_ta.sig", ui32DumpFrameNum);
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpTASignatureRegisters, __DtStep_1)
		return eErr;
	}

	PDumpRegisterRange(psDevId,
						pszFileName, 
						pui32Registers, 
						ui32NumRegisters, 
						&ui32FileOffset, 
						sizeof(IMG_UINT32), 
						ui32Flags);
	__DtTestPoint(__DtFunc_PDumpTASignatureRegisters, __DtStep_2)
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpCounterRegisters (PVRSRV_DEVICE_IDENTIFIER *psDevId,
								IMG_UINT32 ui32DumpFrameNum,
								IMG_BOOL	bLastFrame,
								IMG_UINT32 *pui32Registers,
								IMG_UINT32 ui32NumRegisters)
{
	PVRSRV_ERROR eErr;
	IMG_UINT32	ui32FileOffset, ui32Flags;

	__DtTestPoint(__DtFunc_PDumpCounterRegisters, __DtStep_0)
	PDUMP_GET_FILE_STRING();

	ui32Flags = bLastFrame ? PDUMP_FLAGS_LASTFRAME : 0UL;
	ui32FileOffset = 0UL;

	PDumpCommentWithFlags(ui32Flags, "\r\n-- Dump counter registers\r\n");
	eErr = PDumpOSSprintf(pszFileName, ui32MaxLen, "out%u.perf", ui32DumpFrameNum);
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpCounterRegisters, __DtStep_1)
		return eErr;
	}

	PDumpRegisterRange(psDevId,
						pszFileName,
						pui32Registers,
						ui32NumRegisters,
						&ui32FileOffset,
						sizeof(IMG_UINT32),
						ui32Flags);

	__DtTestPoint(__DtFunc_PDumpCounterRegisters, __DtStep_2)
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpRegRead(IMG_CHAR *pszPDumpRegName,
							const IMG_UINT32 ui32RegOffset,
							IMG_UINT32 ui32Flags)
{
	PVRSRV_ERROR eErr;
	__DtTestPoint(__DtFunc_PDumpRegRead, __DtStep_0)
	PDUMP_GET_SCRIPT_STRING();

	eErr = PDumpOSBufprintf(hScript, ui32MaxLen, "RDW :%s:0x%X\r\n",
							pszPDumpRegName, 
							ui32RegOffset);
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpRegRead, __DtStep_1)
		return eErr;
	}
	PDumpOSWriteString2(hScript, ui32Flags);
	__DtTestPoint(__DtFunc_PDumpRegRead, __DtStep_2)
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpSaveMemKM (PVRSRV_DEVICE_IDENTIFIER *psDevId,
							 IMG_CHAR			*pszFileName,
							 IMG_UINT32			ui32FileOffset,
							 IMG_DEV_VIRTADDR	sDevBaseAddr,
							 IMG_UINT32 		ui32Size,
							 IMG_UINT32 		ui32DataMaster,
							 IMG_UINT32 		ui32PDumpFlags)
{
	PVRSRV_ERROR eErr;
	__DtTestPoint(__DtFunc_PDumpSaveMemKM, __DtStep_0)
	PDUMP_GET_SCRIPT_STRING();
	
#if !defined(SGX_FEATURE_MULTIPLE_MEM_CONTEXTS)
	PVR_UNREFERENCED_PARAMETER(ui32DataMaster);
#endif 

	eErr = PDumpOSBufprintf(hScript,
							ui32MaxLen,
#if defined(SGX_FEATURE_MULTIPLE_MEM_CONTEXTS)
							"SAB :%s:v%x:0x%08X 0x%08X 0x%08X %s.bin\r\n",
							psDevId->pszPDumpDevName,
							ui32DataMaster,
#else
							"SAB :%s:v:0x%08X 0x%08X 0x%08X %s.bin\r\n",
							psDevId->pszPDumpDevName,
#endif 
							sDevBaseAddr.uiAddr,
							ui32Size,
							ui32FileOffset,
							pszFileName);
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpSaveMemKM, __DtStep_1)
		return eErr;
	}

	PDumpOSWriteString2(hScript, ui32PDumpFlags);
	__DtTestPoint(__DtFunc_PDumpSaveMemKM, __DtStep_2)
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpCycleCountRegRead(PVRSRV_DEVICE_IDENTIFIER *psDevId,
									const IMG_UINT32 ui32RegOffset,
									IMG_BOOL bLastFrame)
{
	PVRSRV_ERROR eErr;
	__DtTestPoint(__DtFunc_PDumpCycleCountRegRead, __DtStep_0)
	PDUMP_GET_SCRIPT_STRING();

	eErr = PDumpOSBufprintf(hScript, ui32MaxLen, "RDW :%s:0x%X\r\n", 
							psDevId->pszPDumpRegName,
							ui32RegOffset);
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpCycleCountRegRead, __DtStep_1)
		return eErr;
	}
	PDumpOSWriteString2(hScript, bLastFrame ? PDUMP_FLAGS_LASTFRAME : 0);
	__DtTestPoint(__DtFunc_PDumpCycleCountRegRead, __DtStep_2)
	return PVRSRV_OK;
}


PVRSRV_ERROR PDumpSignatureBuffer (PVRSRV_DEVICE_IDENTIFIER *psDevId,
								   IMG_CHAR			*pszFileName,
								   IMG_CHAR			*pszBufferType,
								   IMG_UINT32		ui32FileOffset,
								   IMG_DEV_VIRTADDR	sDevBaseAddr,
								   IMG_UINT32 		ui32Size,
								   IMG_UINT32 		ui32PDumpFlags)
{
	__DtTestPoint(__DtFunc_PDumpSignatureBuffer, __DtStep_0)
	PDumpCommentWithFlags(ui32PDumpFlags, "\r\n-- Dump microkernel %s signature Buffer\r\n",
						  pszBufferType);
	PDumpCommentWithFlags(ui32PDumpFlags, "Buffer format (sizes in 32-bit words):\r\n");
	PDumpCommentWithFlags(ui32PDumpFlags, "\tNumber of signatures per sample (1)\r\n");
	PDumpCommentWithFlags(ui32PDumpFlags, "\tNumber of samples (1)\r\n");
	PDumpCommentWithFlags(ui32PDumpFlags, "\tSignature register offsets (1 * number of signatures)\r\n");
	PDumpCommentWithFlags(ui32PDumpFlags, "\tSignature sample values (number of samples * number of signatures)\r\n");
	PDumpCommentWithFlags(ui32PDumpFlags, "Note: If buffer is full, last sample is final state after test completed\r\n");
	__DtTestPoint(__DtFunc_PDumpSignatureBuffer, __DtStep_1)
	return PDumpSaveMemKM(psDevId, pszFileName, ui32FileOffset, sDevBaseAddr, ui32Size,
						  PDUMP_DATAMASTER_EDM, ui32PDumpFlags);
}


PVRSRV_ERROR PDumpHWPerfCBKM (PVRSRV_DEVICE_IDENTIFIER *psDevId,
							  IMG_CHAR			*pszFileName,
							  IMG_UINT32		ui32FileOffset,
							  IMG_DEV_VIRTADDR	sDevBaseAddr,
							  IMG_UINT32 		ui32Size,
							  IMG_UINT32 		ui32PDumpFlags)
{
	__DtTestPoint(__DtFunc_PDumpHWPerfCBKM, __DtStep_0)
	PDumpCommentWithFlags(ui32PDumpFlags, "\r\n-- Dump Hardware Performance Circular Buffer\r\n");
	__DtTestPoint(__DtFunc_PDumpHWPerfCBKM, __DtStep_1)
	return PDumpSaveMemKM(psDevId, pszFileName, ui32FileOffset, sDevBaseAddr, ui32Size,
						  PDUMP_DATAMASTER_EDM, ui32PDumpFlags);
}


PVRSRV_ERROR PDumpCBP(PPVRSRV_KERNEL_MEM_INFO		psROffMemInfo,
			  IMG_UINT32					ui32ROffOffset,
			  IMG_UINT32					ui32WPosVal,
			  IMG_UINT32					ui32PacketSize,
			  IMG_UINT32					ui32BufferSize,
			  IMG_UINT32					ui32Flags,
			  IMG_HANDLE					hUniqueTag)
{
	PVRSRV_ERROR eErr;
	IMG_UINT32			ui32PageOffset;
	IMG_UINT8			*pui8LinAddr;
	IMG_DEV_VIRTADDR	sDevVAddr;
	IMG_DEV_PHYADDR		sDevPAddr;
	IMG_DEV_VIRTADDR 	sDevVPageAddr;
    
	PDUMP_MMU_ATTRIB *psMMUAttrib;

	__DtTestPoint(__DtFunc_PDumpCBP, __DtStep_0)
	PDUMP_GET_SCRIPT_STRING();

	psMMUAttrib = ((BM_BUF*)psROffMemInfo->sMemBlk.hBuffer)->pMapping->pBMHeap->psMMUAttrib;

	
	PVR_ASSERT((ui32ROffOffset + sizeof(IMG_UINT32)) <= psROffMemInfo->ui32AllocSize);

	pui8LinAddr = psROffMemInfo->pvLinAddrKM;
	sDevVAddr = psROffMemInfo->sDevVAddr;

	
	pui8LinAddr += ui32ROffOffset;
	sDevVAddr.uiAddr += ui32ROffOffset;

	


	PDumpOSCPUVAddrToPhysPages(psROffMemInfo->sMemBlk.hOSMemHandle,
			ui32ROffOffset,
			pui8LinAddr,
			psMMUAttrib->ui32DataPageMask,
			&ui32PageOffset);

	
	sDevVPageAddr.uiAddr = sDevVAddr.uiAddr - ui32PageOffset;

	PVR_ASSERT((sDevVPageAddr.uiAddr & 0xFFF) == 0);

	
	BM_GetPhysPageAddr(psROffMemInfo, sDevVPageAddr, &sDevPAddr);

	
	sDevPAddr.uiAddr += ui32PageOffset;

	eErr = PDumpOSBufprintf(hScript,
			 ui32MaxLen,
			 "CBP :%s:PA_%08X%08X:0x%08X 0x%08X 0x%08X 0x%08X\r\n",
			 psMMUAttrib->sDevId.pszPDumpDevName,
			 (IMG_UINT32)(IMG_UINTPTR_T)hUniqueTag,
			 sDevPAddr.uiAddr & ~(psMMUAttrib->ui32DataPageMask),
			 sDevPAddr.uiAddr & (psMMUAttrib->ui32DataPageMask),
			 ui32WPosVal,
			 ui32PacketSize,
			 ui32BufferSize);
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpCBP, __DtStep_1)
		return eErr;
	}
	PDumpOSWriteString2(hScript, ui32Flags);
	__DtTestPoint(__DtFunc_PDumpCBP, __DtStep_2)
	return PVRSRV_OK;
}


PVRSRV_ERROR PDumpIDLWithFlags(IMG_UINT32 ui32Clocks, IMG_UINT32 ui32Flags)
{
	PVRSRV_ERROR eErr;
	__DtTestPoint(__DtFunc_PDumpIDLWithFlags, __DtStep_0)
	PDUMP_GET_SCRIPT_STRING();
	PDUMP_DBG(("PDumpIDLWithFlags"));

	eErr = PDumpOSBufprintf(hScript, ui32MaxLen, "IDL %u\r\n", ui32Clocks);
	if(eErr != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpIDLWithFlags, __DtStep_1)
		return eErr;
	}
	PDumpOSWriteString2(hScript, ui32Flags);
	__DtTestPoint(__DtFunc_PDumpIDLWithFlags, __DtStep_2)
	return PVRSRV_OK;
}


PVRSRV_ERROR PDumpIDL(IMG_UINT32 ui32Clocks)
{
	__DtTestPoint(__DtFunc_PDumpIDL, __DtStep_0)
	return PDumpIDLWithFlags(ui32Clocks, PDUMP_FLAGS_CONTINUOUS);
}

PVRSRV_ERROR PDumpMemUM(PVRSRV_PER_PROCESS_DATA *psPerProc,
						IMG_PVOID pvAltLinAddrUM,
						IMG_PVOID pvLinAddrUM,
						PVRSRV_KERNEL_MEM_INFO *psMemInfo,
						IMG_UINT32 ui32Offset,
						IMG_UINT32 ui32Bytes,
						IMG_UINT32 ui32Flags,
						IMG_HANDLE hUniqueTag)
{
	IMG_VOID *pvAddrUM;
	IMG_VOID *pvAddrKM;
	IMG_UINT32 ui32BytesDumped;
	IMG_UINT32 ui32CurrentOffset;

	__DtTestPoint(__DtFunc_PDumpMemUM, __DtStep_0)
	if (psMemInfo->pvLinAddrKM != IMG_NULL && pvAltLinAddrUM == IMG_NULL)
	{
		
		__DtTestPoint(__DtFunc_PDumpMemUM, __DtStep_1)
		return PDumpMemKM(IMG_NULL,
					   psMemInfo,
					   ui32Offset,
					   ui32Bytes,
					   ui32Flags,
					   hUniqueTag);
	}

	pvAddrUM = (pvAltLinAddrUM != IMG_NULL) ? pvAltLinAddrUM : ((pvLinAddrUM != IMG_NULL) ? VPTR_PLUS(pvLinAddrUM, ui32Offset) : IMG_NULL);

	pvAddrKM = GetTempBuffer();

	
	PVR_ASSERT(pvAddrUM != IMG_NULL && pvAddrKM != IMG_NULL);
	if (pvAddrUM == IMG_NULL || pvAddrKM == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_PDumpMemUM, __DtStep_2)
		PVR_DPF((PVR_DBG_ERROR, "PDumpMemUM: Nothing to dump"));
		__DtTestPoint(__DtFunc_PDumpMemUM, __DtStep_3)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	if (ui32Bytes > PDUMP_TEMP_BUFFER_SIZE)
	{
		__DtTestPoint(__DtFunc_PDumpMemUM, __DtStep_4)
		PDumpCommentWithFlags(ui32Flags, "Dumping 0x%08x bytes of memory, in blocks of 0x%08x bytes", ui32Bytes, (IMG_UINT32)PDUMP_TEMP_BUFFER_SIZE);
	}

	ui32CurrentOffset = ui32Offset;
	for (ui32BytesDumped = 0; ui32BytesDumped < ui32Bytes;)
	{
		PVRSRV_ERROR eError;
		IMG_UINT32 ui32BytesToDump = MIN(PDUMP_TEMP_BUFFER_SIZE, ui32Bytes - ui32BytesDumped);

		__DtTestPoint(__DtFunc_PDumpMemUM, __DtStep_5)
		eError = OSCopyFromUser(psPerProc,
					   pvAddrKM,
					   pvAddrUM,
					   ui32BytesToDump);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PDumpMemUM, __DtStep_6)
			PVR_DPF((PVR_DBG_ERROR, "PDumpMemUM: OSCopyFromUser failed (%d)", eError));
			__DtTestPoint(__DtFunc_PDumpMemUM, __DtStep_7)
			return eError;
		}

		eError = PDumpMemKM(pvAddrKM,
					   psMemInfo,
					   ui32CurrentOffset,
					   ui32BytesToDump,
					   ui32Flags,
					   hUniqueTag);

		if (eError != PVRSRV_OK)
		{
			
			__DtTestPoint(__DtFunc_PDumpMemUM, __DtStep_8)
			if (ui32BytesDumped != 0)
			{
				__DtTestPoint(__DtFunc_PDumpMemUM, __DtStep_9)
				PVR_DPF((PVR_DBG_ERROR, "PDumpMemUM: PDumpMemKM failed (%d)", eError));
			}
			PVR_ASSERT(ui32BytesDumped == 0);
			__DtTestPoint(__DtFunc_PDumpMemUM, __DtStep_10)
			return eError;
		}

		VPTR_INC(pvAddrUM, ui32BytesToDump);
		ui32CurrentOffset += ui32BytesToDump;
		ui32BytesDumped += ui32BytesToDump;
	}

	__DtTestPoint(__DtFunc_PDumpMemUM, __DtStep_11)
	return PVRSRV_OK;
}


static PVRSRV_ERROR _PdumpAllocMMUContext(IMG_UINT32 *pui32MMUContextID)
{
	IMG_UINT32 i;

	
	__DtTestPoint(__DtFunc__PdumpAllocMMUContext, __DtStep_0)
	for(i=0; i<MAX_PDUMP_MMU_CONTEXTS; i++)
	{
		__DtTestPoint(__DtFunc__PdumpAllocMMUContext, __DtStep_1)
		if((gui16MMUContextUsage & (1U << i)) == 0)
		{
			
			__DtTestPoint(__DtFunc__PdumpAllocMMUContext, __DtStep_2)
			gui16MMUContextUsage |= 1U << i;
			*pui32MMUContextID = i;
			__DtTestPoint(__DtFunc__PdumpAllocMMUContext, __DtStep_3)
			return PVRSRV_OK;
		}
	}

	PVR_DPF((PVR_DBG_ERROR, "_PdumpAllocMMUContext: no free MMU context ids"));

	__DtTestPoint(__DtFunc__PdumpAllocMMUContext, __DtStep_4)
	return PVRSRV_ERROR_MMU_CONTEXT_NOT_FOUND;
}


static PVRSRV_ERROR _PdumpFreeMMUContext(IMG_UINT32 ui32MMUContextID)
{
	__DtTestPoint(__DtFunc__PdumpFreeMMUContext, __DtStep_0)
	if(ui32MMUContextID < MAX_PDUMP_MMU_CONTEXTS)
	{
		
		__DtTestPoint(__DtFunc__PdumpFreeMMUContext, __DtStep_1)
		gui16MMUContextUsage &= ~(1U << ui32MMUContextID);
		__DtTestPoint(__DtFunc__PdumpFreeMMUContext, __DtStep_2)
		return PVRSRV_OK;
	}

	PVR_DPF((PVR_DBG_ERROR, "_PdumpFreeMMUContext: MMU context ids invalid"));

	__DtTestPoint(__DtFunc__PdumpFreeMMUContext, __DtStep_3)
	return PVRSRV_ERROR_MMU_CONTEXT_NOT_FOUND;
}


PVRSRV_ERROR PDumpSetMMUContext(PVRSRV_DEVICE_TYPE eDeviceType,
								IMG_CHAR *pszMemSpace,
								IMG_UINT32 *pui32MMUContextID,
								IMG_UINT32 ui32MMUType,
								IMG_HANDLE hUniqueTag1,
								IMG_HANDLE hOSMemHandle, 
								IMG_VOID *pvPDCPUAddr)
{
	IMG_UINT8 *pui8LinAddr = (IMG_UINT8 *)pvPDCPUAddr;
	IMG_CPU_PHYADDR sCpuPAddr;
	IMG_DEV_PHYADDR sDevPAddr;
	IMG_UINT32 ui32MMUContextID;
	PVRSRV_ERROR eError;

	__DtTestPoint(__DtFunc_PDumpSetMMUContext, __DtStep_0)
	eError = _PdumpAllocMMUContext(&ui32MMUContextID);
	if(eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpSetMMUContext, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "PDumpSetMMUContext: _PdumpAllocMMUContext failed: %d", eError));
		__DtTestPoint(__DtFunc_PDumpSetMMUContext, __DtStep_2)
		return eError;
	}

	
	
	sCpuPAddr = OSMapLinToCPUPhys(hOSMemHandle, pui8LinAddr);
	sDevPAddr = SysCpuPAddrToDevPAddr(eDeviceType, sCpuPAddr);
	
	sDevPAddr.uiAddr &= ~((PVRSRV_4K_PAGE_SIZE) -1);

	PDumpComment("Set MMU Context\r\n");
	
	PDumpComment("MMU :%s:v%d %d :%s:PA_%08X%08X\r\n",
						pszMemSpace,
						ui32MMUContextID,
						ui32MMUType,
						pszMemSpace,
						(IMG_UINT32)(IMG_UINTPTR_T)hUniqueTag1,
						sDevPAddr.uiAddr);

	
	*pui32MMUContextID = ui32MMUContextID;

	__DtTestPoint(__DtFunc_PDumpSetMMUContext, __DtStep_3)
	return PVRSRV_OK;
}


PVRSRV_ERROR PDumpClearMMUContext(PVRSRV_DEVICE_TYPE eDeviceType,
								IMG_CHAR *pszMemSpace,
								IMG_UINT32 ui32MMUContextID,
								IMG_UINT32 ui32MMUType)
{
	PVRSRV_ERROR eError;

	__DtTestPoint(__DtFunc_PDumpClearMMUContext, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(eDeviceType);

	
	PDumpComment("Clear MMU Context for memory space %s\r\n", pszMemSpace);
	
	PDumpComment("MMU :%s:v%d %d\r\n",
						pszMemSpace,
						ui32MMUContextID,
						ui32MMUType);

	eError = _PdumpFreeMMUContext(ui32MMUContextID);
	if(eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PDumpClearMMUContext, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "PDumpClearMMUContext: _PdumpFreeMMUContext failed: %d", eError));
		__DtTestPoint(__DtFunc_PDumpClearMMUContext, __DtStep_2)
		return eError;
	}

	__DtTestPoint(__DtFunc_PDumpClearMMUContext, __DtStep_3)
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpStoreMemToFile(PDUMP_MMU_ATTRIB *psMMUAttrib,
						         IMG_CHAR *pszFileName,
								 IMG_UINT32 ui32FileOffset, 
								 PVRSRV_KERNEL_MEM_INFO *psMemInfo,
								 IMG_UINT32 uiAddr, 
								 IMG_UINT32 ui32Size,
								 IMG_UINT32 ui32PDumpFlags,
								 IMG_HANDLE hUniqueTag)
{
	IMG_DEV_PHYADDR		sDevPAddr;
	IMG_DEV_VIRTADDR	sDevVPageAddr;
	IMG_UINT32			ui32PageOffset;

	__DtTestPoint(__DtFunc_PDumpStoreMemToFile, __DtStep_0)
	PDUMP_GET_SCRIPT_STRING();

	


	ui32PageOffset = (IMG_UINT32)psMemInfo->pvLinAddrKM & psMMUAttrib->ui32DataPageMask;
	
	
	sDevVPageAddr.uiAddr = uiAddr - ui32PageOffset;
	
	
	BM_GetPhysPageAddr(psMemInfo, sDevVPageAddr, &sDevPAddr);
	
	
	sDevPAddr.uiAddr += ui32PageOffset;

	PDumpOSBufprintf(hScript,
			 ui32MaxLen,
			 "SAB :%s:PA_%08X%08X:0x%08X 0x%08X 0x%08X %s\r\n",
			 psMMUAttrib->sDevId.pszPDumpDevName,
			 (IMG_UINT32)(IMG_UINTPTR_T)hUniqueTag,
			 sDevPAddr.uiAddr & ~psMMUAttrib->ui32DataPageMask,
			 sDevPAddr.uiAddr & psMMUAttrib->ui32DataPageMask,
			 ui32Size,
			 ui32FileOffset,
			 pszFileName);

	PDumpOSWriteString2(hScript, ui32PDumpFlags);
	
	__DtTestPoint(__DtFunc_PDumpStoreMemToFile, __DtStep_1)
	return PVRSRV_OK;	
}

PVRSRV_ERROR PDumpRegBasedCBP(IMG_CHAR		*pszPDumpRegName,
							  IMG_UINT32	ui32RegOffset,
							  IMG_UINT32	ui32WPosVal,
							  IMG_UINT32	ui32PacketSize,
							  IMG_UINT32	ui32BufferSize,
							  IMG_UINT32	ui32Flags)
{
	__DtTestPoint(__DtFunc_PDumpRegBasedCBP, __DtStep_0)
	PDUMP_GET_SCRIPT_STRING();

	PDumpOSBufprintf(hScript,
			 ui32MaxLen,
			 "CBP :%s:0x%08X 0x%08X 0x%08X 0x%08X\r\n",
			 pszPDumpRegName,
			 ui32RegOffset,
			 ui32WPosVal,
			 ui32PacketSize,
			 ui32BufferSize);
	PDumpOSWriteString2(hScript, ui32Flags);
	
	__DtTestPoint(__DtFunc_PDumpRegBasedCBP, __DtStep_1)
	return PVRSRV_OK;		
}


#include "syscommon.h"

IMG_EXPORT IMG_VOID PDumpConnectionNotify(IMG_VOID)
{
	SYS_DATA			*psSysData;
	PVRSRV_DEVICE_NODE	*psThis;
	__DtTestPoint(__DtFunc_PDumpConnectionNotify, __DtStep_0)
	PVR_DPF((PVR_DBG_WARNING, "PDump has connected."));
	
	
	SysAcquireData(&psSysData);
	
	psThis = psSysData->psDeviceNodeList;
	while (psThis)
	{
		__DtTestPoint(__DtFunc_PDumpConnectionNotify, __DtStep_1)
		if (psThis->pfnPDumpInitDevice)
		{
			
			__DtTestPoint(__DtFunc_PDumpConnectionNotify, __DtStep_2)
			psThis->pfnPDumpInitDevice(psThis);
		}
		psThis = psThis->psNext;
	}
	__DtTestPoint(__DtFunc_PDumpConnectionNotify, __DtStep_3)
}

IMG_UINT32 DbgWrite(PDBG_STREAM psStream, IMG_UINT8 *pui8Data, IMG_UINT32 ui32BCount, IMG_UINT32 ui32Flags)
{
	IMG_UINT32	ui32BytesWritten = 0;
	IMG_UINT32	ui32Off = 0;
	PDBG_STREAM_CONTROL psCtrl = psStream->psCtrl;

	
	__DtTestPoint(__DtFunc_DbgWrite, __DtStep_0)
	if ((ui32Flags & PDUMP_FLAGS_NEVER) != 0)
	{
		__DtTestPoint(__DtFunc_DbgWrite, __DtStep_1)
		return ui32BCount;
	}

#if defined(SUPPORT_PDUMP_MULTI_PROCESS)
	
	if ( (_PDumpIsProcessActive() == IMG_FALSE ) &&
		 ((ui32Flags & PDUMP_FLAGS_PERSISTENT) == 0) )
	{
		__DtTestPoint(__DtFunc_DbgWrite, __DtStep_2)
		return ui32BCount;
	}
#endif

	
	if ( ((ui32Flags & PDUMP_FLAGS_PERSISTENT) != 0) && (psCtrl->bInitPhaseComplete) )
	{
		__DtTestPoint(__DtFunc_DbgWrite, __DtStep_3)
		while (((IMG_UINT32) ui32BCount > 0) && (ui32BytesWritten != 0xFFFFFFFFU))
		{
			


			__DtTestPoint(__DtFunc_DbgWrite, __DtStep_4)
			ui32BytesWritten = PDumpOSDebugDriverWrite(	psStream,
														PDUMP_WRITE_MODE_PERSISTENT,
														&pui8Data[ui32Off], ui32BCount, 1, 0);

			if (ui32BytesWritten == 0)
			{
				__DtTestPoint(__DtFunc_DbgWrite, __DtStep_5)
				PDumpOSReleaseExecution();
			}

			if (ui32BytesWritten != 0xFFFFFFFFU)
			{
				__DtTestPoint(__DtFunc_DbgWrite, __DtStep_6)
				ui32Off += ui32BytesWritten;
				ui32BCount -= ui32BytesWritten;
			}
			else
			{
				__DtTestPoint(__DtFunc_DbgWrite, __DtStep_7)
				PVR_DPF((PVR_DBG_ERROR, "DbgWrite: Failed to send persistent data"));
				if( (psCtrl->ui32Flags & DEBUG_FLAGS_READONLY) != 0)
				{
					
					__DtTestPoint(__DtFunc_DbgWrite, __DtStep_8)
					PDumpSuspendKM();
				}
				__DtTestPoint(__DtFunc_DbgWrite, __DtStep_9)
				return 0xFFFFFFFFU;
			}
		}
		
		
		ui32BCount = ui32Off; ui32Off = 0; ui32BytesWritten = 0;
	}

	while (((IMG_UINT32) ui32BCount > 0) && (ui32BytesWritten != 0xFFFFFFFFU))
	{
		__DtTestPoint(__DtFunc_DbgWrite, __DtStep_10)
		if ((ui32Flags & PDUMP_FLAGS_CONTINUOUS) != 0)
		{
			

			__DtTestPoint(__DtFunc_DbgWrite, __DtStep_11)
			if (((psCtrl->ui32CapMode & DEBUG_CAPMODE_FRAMED) != 0) &&
				 (psCtrl->ui32Start == 0xFFFFFFFFU) &&
				 (psCtrl->ui32End == 0xFFFFFFFFU) &&
				  psCtrl->bInitPhaseComplete)
			{
				__DtTestPoint(__DtFunc_DbgWrite, __DtStep_12)
				ui32BytesWritten = ui32BCount;
			}
			else
			{
				__DtTestPoint(__DtFunc_DbgWrite, __DtStep_13)
				ui32BytesWritten = PDumpOSDebugDriverWrite(	psStream, 
															PDUMP_WRITE_MODE_CONTINUOUS,
															&pui8Data[ui32Off], ui32BCount, 1, 0);
			}
		}
		else
		{
			__DtTestPoint(__DtFunc_DbgWrite, __DtStep_14)
			if (ui32Flags & PDUMP_FLAGS_LASTFRAME)
			{
				IMG_UINT32	ui32DbgFlags;
	
				__DtTestPoint(__DtFunc_DbgWrite, __DtStep_15)
				ui32DbgFlags = 0;
				if (ui32Flags & PDUMP_FLAGS_RESETLFBUFFER)
				{
					__DtTestPoint(__DtFunc_DbgWrite, __DtStep_16)
					ui32DbgFlags |= WRITELF_FLAGS_RESETBUF;
				}
	
				ui32BytesWritten = PDumpOSDebugDriverWrite(	psStream,
															PDUMP_WRITE_MODE_LASTFRAME,
															&pui8Data[ui32Off], ui32BCount, 1, ui32DbgFlags);
			}
			else
			{
				__DtTestPoint(__DtFunc_DbgWrite, __DtStep_17)
				ui32BytesWritten = PDumpOSDebugDriverWrite(	psStream, 
															PDUMP_WRITE_MODE_BINCM,
															&pui8Data[ui32Off], ui32BCount, 1, 0);
			}
		}

		


		if (ui32BytesWritten == 0)
		{
			__DtTestPoint(__DtFunc_DbgWrite, __DtStep_18)
			PDumpOSReleaseExecution();
		}

		if (ui32BytesWritten != 0xFFFFFFFFU)
		{
			__DtTestPoint(__DtFunc_DbgWrite, __DtStep_19)
			ui32Off += ui32BytesWritten;
			ui32BCount -= ui32BytesWritten;
		}

		
	}


	
	__DtTestPoint(__DtFunc_DbgWrite, __DtStep_20)
	return ui32BytesWritten;
}



#else	
#endif	
