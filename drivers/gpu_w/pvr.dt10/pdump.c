#include	"DT_pdump.h"	/* For DynamicTracer-TestPoint */
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

#if defined (SUPPORT_SGX) || defined (SUPPORT_VGX)
#if defined (PDUMP)

#include <asm/atomic.h>
#include <stdarg.h>
#if defined (SUPPORT_SGX)
#include "sgxdefs.h" 
#endif
#include "services_headers.h"

#include "pvrversion.h"
#include "pvr_debug.h"

#include "dbgdrvif.h"
#if defined (SUPPORT_SGX)
#include "sgxmmu.h"
#endif
#include "mm.h"
#include "pdump_km.h"
#include "pdump_int.h"

#include <linux/kernel.h> 
#include <linux/string.h> 

static IMG_BOOL PDumpWriteString2		(IMG_CHAR * pszString, IMG_UINT32 ui32Flags);
static IMG_BOOL PDumpWriteILock			(PDBG_STREAM psStream, IMG_UINT8 *pui8Data, IMG_UINT32 ui32Count, IMG_UINT32 ui32Flags);
static IMG_VOID DbgSetFrame				(PDBG_STREAM psStream, IMG_UINT32 ui32Frame);
static IMG_VOID DbgSetMarker			(PDBG_STREAM psStream, IMG_UINT32 ui32Marker);

#define PDUMP_DATAMASTER_PIXEL		(1)
#define PDUMP_DATAMASTER_EDM		(3)

#define MAX_FILE_SIZE	0x40000000

static atomic_t gsPDumpSuspended = ATOMIC_INIT(0);

static PDBGKM_SERVICE_TABLE gpfnDbgDrv = IMG_NULL;



IMG_CHAR *pszStreamName[PDUMP_NUM_STREAMS] = {	"ParamStream2",
												"ScriptStream2",
												"DriverInfoStream"};
typedef struct PDBG_PDUMP_STATE_TAG
{
	PDBG_STREAM psStream[PDUMP_NUM_STREAMS];
	IMG_UINT32 ui32ParamFileNum;

	IMG_CHAR *pszMsg;
	IMG_CHAR *pszScript;
	IMG_CHAR *pszFile;

} PDBG_PDUMP_STATE;

static PDBG_PDUMP_STATE gsDBGPdumpState = {{IMG_NULL}, 0, IMG_NULL, IMG_NULL, IMG_NULL};

#define SZ_MSG_SIZE_MAX			PVRSRV_PDUMP_MAX_COMMENT_SIZE-1
#define SZ_SCRIPT_SIZE_MAX		PVRSRV_PDUMP_MAX_COMMENT_SIZE-1
#define SZ_FILENAME_SIZE_MAX	PVRSRV_PDUMP_MAX_COMMENT_SIZE-1




IMG_VOID DBGDrvGetServiceTable(IMG_VOID **fn_table);

static inline IMG_BOOL PDumpSuspended(IMG_VOID)
{
	__DtTestPoint(__DtFunc_PDumpSuspended, __DtStep_0)
	return (atomic_read(&gsPDumpSuspended) != 0) ? IMG_TRUE : IMG_FALSE;
}

PVRSRV_ERROR PDumpOSGetScriptString(IMG_HANDLE *phScript,
									IMG_UINT32 *pui32MaxLen)
{
	__DtTestPoint(__DtFunc_PDumpOSGetScriptString, __DtStep_0)
	*phScript = (IMG_HANDLE)gsDBGPdumpState.pszScript;
	*pui32MaxLen = SZ_SCRIPT_SIZE_MAX;
	if ((!*phScript) || PDumpSuspended())
	{
		__DtTestPoint(__DtFunc_PDumpOSGetScriptString, __DtStep_1)
		return PVRSRV_ERROR_PDUMP_NOT_ACTIVE;
	}
	__DtTestPoint(__DtFunc_PDumpOSGetScriptString, __DtStep_2)
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpOSGetMessageString(IMG_CHAR **ppszMsg,
									 IMG_UINT32 *pui32MaxLen)
{
	__DtTestPoint(__DtFunc_PDumpOSGetMessageString, __DtStep_0)
	*ppszMsg = gsDBGPdumpState.pszMsg;
	*pui32MaxLen = SZ_MSG_SIZE_MAX;
	if ((!*ppszMsg) || PDumpSuspended())
	{
		__DtTestPoint(__DtFunc_PDumpOSGetMessageString, __DtStep_1)
		return PVRSRV_ERROR_PDUMP_NOT_ACTIVE;
	}
	__DtTestPoint(__DtFunc_PDumpOSGetMessageString, __DtStep_2)
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpOSGetFilenameString(IMG_CHAR **ppszFile,
									 IMG_UINT32 *pui32MaxLen)
{
	__DtTestPoint(__DtFunc_PDumpOSGetFilenameString, __DtStep_0)
	*ppszFile = gsDBGPdumpState.pszFile;
	*pui32MaxLen = SZ_FILENAME_SIZE_MAX;
	if ((!*ppszFile) || PDumpSuspended())
	{
		__DtTestPoint(__DtFunc_PDumpOSGetFilenameString, __DtStep_1)
		return PVRSRV_ERROR_PDUMP_NOT_ACTIVE;
	}
	__DtTestPoint(__DtFunc_PDumpOSGetFilenameString, __DtStep_2)
	return PVRSRV_OK;
}

IMG_BOOL PDumpOSWriteString2(IMG_HANDLE hScript, IMG_UINT32 ui32Flags)
{
	__DtTestPoint(__DtFunc_PDumpOSWriteString2, __DtStep_0)
	return PDumpWriteString2(hScript, ui32Flags);
}

PVRSRV_ERROR PDumpOSBufprintf(IMG_HANDLE hBuf, IMG_UINT32 ui32ScriptSizeMax, IMG_CHAR* pszFormat, ...)
{
	IMG_CHAR* pszBuf = hBuf;
	IMG_INT32 n;
	va_list	vaArgs;

	__DtTestPoint(__DtFunc_PDumpOSBufprintf, __DtStep_0)
	va_start(vaArgs, pszFormat);

	n = vsnprintf(pszBuf, ui32ScriptSizeMax, pszFormat, vaArgs);

	va_end(vaArgs);

	if (n>=(IMG_INT32)ui32ScriptSizeMax || n==-1)	
	{
		__DtTestPoint(__DtFunc_PDumpOSBufprintf, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "Buffer overflow detected, pdump output may be incomplete."));

		__DtTestPoint(__DtFunc_PDumpOSBufprintf, __DtStep_2)
		return PVRSRV_ERROR_PDUMP_BUF_OVERFLOW;
	}

#if defined(PDUMP_DEBUG_OUTFILES)
	g_ui32EveryLineCounter++;
#endif
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpOSVSprintf(IMG_CHAR *pszComment, IMG_UINT32 ui32ScriptSizeMax, IMG_CHAR* pszFormat, PDUMP_va_list vaArgs)
{
	IMG_INT32 n;

	__DtTestPoint(__DtFunc_PDumpOSVSprintf, __DtStep_0)
	n = vsnprintf(pszComment, ui32ScriptSizeMax, pszFormat, vaArgs);

	if (n>=(IMG_INT32)ui32ScriptSizeMax || n==-1)	
	{
		__DtTestPoint(__DtFunc_PDumpOSVSprintf, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "Buffer overflow detected, pdump output may be incomplete."));

		__DtTestPoint(__DtFunc_PDumpOSVSprintf, __DtStep_2)
		return PVRSRV_ERROR_PDUMP_BUF_OVERFLOW;
	}

	__DtTestPoint(__DtFunc_PDumpOSVSprintf, __DtStep_3)
	return PVRSRV_OK;
}

IMG_VOID PDumpOSDebugPrintf(IMG_CHAR* pszFormat, ...)
{
	__DtTestPoint(__DtFunc_PDumpOSDebugPrintf, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(pszFormat);

	
	__DtTestPoint(__DtFunc_PDumpOSDebugPrintf, __DtStep_1)
}

PVRSRV_ERROR PDumpOSSprintf(IMG_CHAR *pszComment, IMG_UINT32 ui32ScriptSizeMax, IMG_CHAR *pszFormat, ...)
{
	IMG_INT32 n;
	va_list	vaArgs;

	__DtTestPoint(__DtFunc_PDumpOSSprintf, __DtStep_0)
	va_start(vaArgs, pszFormat);

	n = vsnprintf(pszComment, ui32ScriptSizeMax, pszFormat, vaArgs);

	va_end(vaArgs);

	if (n>=(IMG_INT32)ui32ScriptSizeMax || n==-1)	
	{
		__DtTestPoint(__DtFunc_PDumpOSSprintf, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "Buffer overflow detected, pdump output may be incomplete."));

		__DtTestPoint(__DtFunc_PDumpOSSprintf, __DtStep_2)
		return PVRSRV_ERROR_PDUMP_BUF_OVERFLOW;
	}

	__DtTestPoint(__DtFunc_PDumpOSSprintf, __DtStep_3)
	return PVRSRV_OK;
}

IMG_UINT32 PDumpOSBuflen(IMG_HANDLE hBuffer, IMG_UINT32 ui32BufferSizeMax)
{
	IMG_CHAR* pszBuf = hBuffer;
	IMG_UINT32 ui32Count = 0;

	__DtTestPoint(__DtFunc_PDumpOSBuflen, __DtStep_0)
	while ((pszBuf[ui32Count]!=0) && (ui32Count<ui32BufferSizeMax) )
	{
		__DtTestPoint(__DtFunc_PDumpOSBuflen, __DtStep_1)
		ui32Count++;
	}
	__DtTestPoint(__DtFunc_PDumpOSBuflen, __DtStep_2)
	return(ui32Count);
}

IMG_VOID PDumpOSVerifyLineEnding(IMG_HANDLE hBuffer, IMG_UINT32 ui32BufferSizeMax)
{
	IMG_UINT32 ui32Count;
	IMG_CHAR* pszBuf = hBuffer;

	
	__DtTestPoint(__DtFunc_PDumpOSVerifyLineEnding, __DtStep_0)
	ui32Count = PDumpOSBuflen(hBuffer, ui32BufferSizeMax);

	
	if ((ui32Count >= 1) && (pszBuf[ui32Count-1] != '\n') && (ui32Count<ui32BufferSizeMax))
	{
		__DtTestPoint(__DtFunc_PDumpOSVerifyLineEnding, __DtStep_1)
		pszBuf[ui32Count] = '\n';
		ui32Count++;
		pszBuf[ui32Count] = '\0';
	}
	if ((ui32Count >= 2) && (pszBuf[ui32Count-2] != '\r') && (ui32Count<ui32BufferSizeMax))
	{
		__DtTestPoint(__DtFunc_PDumpOSVerifyLineEnding, __DtStep_2)
		pszBuf[ui32Count-1] = '\r';
		pszBuf[ui32Count] = '\n';
		ui32Count++;
		pszBuf[ui32Count] = '\0';
	}
	__DtTestPoint(__DtFunc_PDumpOSVerifyLineEnding, __DtStep_3)
}

IMG_HANDLE PDumpOSGetStream(IMG_UINT32 ePDumpStream)
{
	__DtTestPoint(__DtFunc_PDumpOSGetStream, __DtStep_0)
	return (IMG_HANDLE)gsDBGPdumpState.psStream[ePDumpStream];
}

IMG_UINT32 PDumpOSGetStreamOffset(IMG_UINT32 ePDumpStream)
{
	PDBG_STREAM psStream = gsDBGPdumpState.psStream[ePDumpStream];
	__DtTestPoint(__DtFunc_PDumpOSGetStreamOffset, __DtStep_0)
	return gpfnDbgDrv->pfnGetStreamOffset(psStream);
}

IMG_UINT32 PDumpOSGetParamFileNum(IMG_VOID)
{
	__DtTestPoint(__DtFunc_PDumpOSGetParamFileNum, __DtStep_0)
	return gsDBGPdumpState.ui32ParamFileNum;
}

IMG_BOOL PDumpOSWriteString(IMG_HANDLE hStream,
		IMG_UINT8 *psui8Data,
		IMG_UINT32 ui32Size,
		IMG_UINT32 ui32Flags)
{
	PDBG_STREAM psStream = (PDBG_STREAM)hStream;
	__DtTestPoint(__DtFunc_PDumpOSWriteString, __DtStep_0)
	return PDumpWriteILock(psStream,
					psui8Data,
					ui32Size,
					ui32Flags);
}

IMG_VOID PDumpOSCheckForSplitting(IMG_HANDLE hStream, IMG_UINT32 ui32Size, IMG_UINT32 ui32Flags)
{
	
	__DtTestPoint(__DtFunc_PDumpOSCheckForSplitting, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(hStream);
	PVR_UNREFERENCED_PARAMETER(ui32Size);
	PVR_UNREFERENCED_PARAMETER(ui32Flags);
	__DtTestPoint(__DtFunc_PDumpOSCheckForSplitting, __DtStep_1)
}

IMG_BOOL PDumpOSJTInitialised(IMG_VOID)
{
	__DtTestPoint(__DtFunc_PDumpOSJTInitialised, __DtStep_0)
	if(gpfnDbgDrv)
	{
		__DtTestPoint(__DtFunc_PDumpOSJTInitialised, __DtStep_1)
		return IMG_TRUE;
	}
	__DtTestPoint(__DtFunc_PDumpOSJTInitialised, __DtStep_2)
	return IMG_FALSE;
}

inline IMG_BOOL PDumpOSIsSuspended(IMG_VOID)
{
	__DtTestPoint(__DtFunc_PDumpOSIsSuspended, __DtStep_0)
	return (atomic_read(&gsPDumpSuspended) != 0) ? IMG_TRUE : IMG_FALSE;
}

IMG_VOID PDumpOSCPUVAddrToDevPAddr(PVRSRV_DEVICE_TYPE eDeviceType,
        IMG_HANDLE hOSMemHandle,
		IMG_UINT32 ui32Offset,
		IMG_UINT8 *pui8LinAddr,
		IMG_UINT32 ui32PageSize,
		IMG_DEV_PHYADDR *psDevPAddr)
{
	IMG_CPU_PHYADDR	sCpuPAddr;

	__DtTestPoint(__DtFunc_PDumpOSCPUVAddrToDevPAddr, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(pui8LinAddr);
	PVR_UNREFERENCED_PARAMETER(ui32PageSize);   

	
	   
	PVR_ASSERT (hOSMemHandle != IMG_NULL);
	
	sCpuPAddr = OSMemHandleToCpuPAddr(hOSMemHandle, ui32Offset);
	PVR_ASSERT((sCpuPAddr.uiAddr & (ui32PageSize - 1)) == 0);

	
	*psDevPAddr = SysCpuPAddrToDevPAddr(eDeviceType, sCpuPAddr);
	__DtTestPoint(__DtFunc_PDumpOSCPUVAddrToDevPAddr, __DtStep_1)
}

IMG_VOID PDumpOSCPUVAddrToPhysPages(IMG_HANDLE hOSMemHandle,
		IMG_UINT32 ui32Offset,
		IMG_PUINT8 pui8LinAddr,
		IMG_UINT32 ui32DataPageMask,
		IMG_UINT32 *pui32PageOffset)
{
	__DtTestPoint(__DtFunc_PDumpOSCPUVAddrToPhysPages, __DtStep_0)
	if(hOSMemHandle)
	{
		
		IMG_CPU_PHYADDR     sCpuPAddr;

		__DtTestPoint(__DtFunc_PDumpOSCPUVAddrToPhysPages, __DtStep_1)
		PVR_UNREFERENCED_PARAMETER(pui8LinAddr);

		sCpuPAddr = OSMemHandleToCpuPAddr(hOSMemHandle, ui32Offset);
	    *pui32PageOffset = sCpuPAddr.uiAddr & ui32DataPageMask;
	}
	else
	{
		__DtTestPoint(__DtFunc_PDumpOSCPUVAddrToPhysPages, __DtStep_2)
		PVR_UNREFERENCED_PARAMETER(hOSMemHandle);
		PVR_UNREFERENCED_PARAMETER(ui32Offset);

		*pui32PageOffset = ((IMG_UINT32)pui8LinAddr & ui32DataPageMask);
	}
	__DtTestPoint(__DtFunc_PDumpOSCPUVAddrToPhysPages, __DtStep_3)
}

IMG_UINT32 PDumpOSDebugDriverWrite( PDBG_STREAM psStream,
									PDUMP_DDWMODE eDbgDrvWriteMode,
									IMG_UINT8 *pui8Data,
									IMG_UINT32 ui32BCount,
									IMG_UINT32 ui32Level,
									IMG_UINT32 ui32DbgDrvFlags)
{
	__DtTestPoint(__DtFunc_PDumpOSDebugDriverWrite, __DtStep_0)
	switch(eDbgDrvWriteMode)
	{
		case PDUMP_WRITE_MODE_CONTINUOUS:
			__DtTestPoint(__DtFunc_PDumpOSDebugDriverWrite, __DtStep_1)
			PVR_UNREFERENCED_PARAMETER(ui32DbgDrvFlags);
			__DtTestPoint(__DtFunc_PDumpOSDebugDriverWrite, __DtStep_2)
			return gpfnDbgDrv->pfnDBGDrivWrite2(psStream, pui8Data, ui32BCount, ui32Level);
		case PDUMP_WRITE_MODE_LASTFRAME:
			__DtTestPoint(__DtFunc_PDumpOSDebugDriverWrite, __DtStep_3)
			return gpfnDbgDrv->pfnWriteLF(psStream, pui8Data, ui32BCount, ui32Level, ui32DbgDrvFlags);
		case PDUMP_WRITE_MODE_BINCM:
			__DtTestPoint(__DtFunc_PDumpOSDebugDriverWrite, __DtStep_4)
			PVR_UNREFERENCED_PARAMETER(ui32DbgDrvFlags);
			__DtTestPoint(__DtFunc_PDumpOSDebugDriverWrite, __DtStep_5)
			return gpfnDbgDrv->pfnWriteBINCM(psStream, pui8Data, ui32BCount, ui32Level);
		case PDUMP_WRITE_MODE_PERSISTENT:
			__DtTestPoint(__DtFunc_PDumpOSDebugDriverWrite, __DtStep_6)
			PVR_UNREFERENCED_PARAMETER(ui32DbgDrvFlags);
			__DtTestPoint(__DtFunc_PDumpOSDebugDriverWrite, __DtStep_7)
			return gpfnDbgDrv->pfnWritePersist(psStream, pui8Data, ui32BCount, ui32Level);
		default:
			__DtTestPoint(__DtFunc_PDumpOSDebugDriverWrite, __DtStep_8)
			PVR_UNREFERENCED_PARAMETER(ui32DbgDrvFlags);
			break;
	}
	__DtTestPoint(__DtFunc_PDumpOSDebugDriverWrite, __DtStep_9)
	return 0xFFFFFFFFU;
}

IMG_VOID PDumpOSReleaseExecution(IMG_VOID)
{
	__DtTestPoint(__DtFunc_PDumpOSReleaseExecution, __DtStep_0)
	OSReleaseThreadQuanta();
	__DtTestPoint(__DtFunc_PDumpOSReleaseExecution, __DtStep_1)
}

IMG_VOID PDumpInit(IMG_VOID)
{
	IMG_UINT32 i;
	DBGKM_CONNECT_NOTIFIER sConnectNotifier;

	
	__DtTestPoint(__DtFunc_PDumpInit, __DtStep_0)
	if (!gpfnDbgDrv)
	{
		__DtTestPoint(__DtFunc_PDumpInit, __DtStep_1)
		DBGDrvGetServiceTable((IMG_VOID **)&gpfnDbgDrv);


		
		if (gpfnDbgDrv == IMG_NULL)
		{
			__DtTestPoint(__DtFunc_PDumpInit, __DtStep_2)
			return;
		}
		
		
		sConnectNotifier.pfnConnectNotifier = &PDumpConnectionNotify;
		gpfnDbgDrv->pfnSetConnectNotifier(sConnectNotifier);

		if(!gsDBGPdumpState.pszFile)
		{
			__DtTestPoint(__DtFunc_PDumpInit, __DtStep_3)
			if(OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP, SZ_FILENAME_SIZE_MAX, (IMG_PVOID *)&gsDBGPdumpState.pszFile, 0,
				"Filename string") != PVRSRV_OK)
			{
				__DtTestPoint(__DtFunc_PDumpInit, __DtStep_4)
				goto init_failed;
			}
		}

		if(!gsDBGPdumpState.pszMsg)
		{
			__DtTestPoint(__DtFunc_PDumpInit, __DtStep_5)
			if(OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP, SZ_MSG_SIZE_MAX, (IMG_PVOID *)&gsDBGPdumpState.pszMsg, 0,
				"Message string") != PVRSRV_OK)
			{
				__DtTestPoint(__DtFunc_PDumpInit, __DtStep_6)
				goto init_failed;
			}
		}

		if(!gsDBGPdumpState.pszScript)
		{
			__DtTestPoint(__DtFunc_PDumpInit, __DtStep_7)
			if(OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP, SZ_SCRIPT_SIZE_MAX, (IMG_PVOID *)&gsDBGPdumpState.pszScript, 0,
				"Script string") != PVRSRV_OK)
			{
				__DtTestPoint(__DtFunc_PDumpInit, __DtStep_8)
				goto init_failed;
			}
		}

		for(i=0; i < PDUMP_NUM_STREAMS; i++)
		{
			__DtTestPoint(__DtFunc_PDumpInit, __DtStep_9)
			gsDBGPdumpState.psStream[i] = gpfnDbgDrv->pfnCreateStream(pszStreamName[i],
														DEBUG_CAPMODE_FRAMED,
														DEBUG_OUTMODE_STREAMENABLE,
														0,
														10);

			gpfnDbgDrv->pfnSetCaptureMode(gsDBGPdumpState.psStream[i],DEBUG_CAPMODE_FRAMED,0xFFFFFFFF, 0xFFFFFFFF, 1);
			gpfnDbgDrv->pfnSetFrame(gsDBGPdumpState.psStream[i],0);
		}

		PDUMPCOMMENT("Driver Product Name: %s", VS_PRODUCT_NAME);
		PDUMPCOMMENT("Driver Product Version: %s (%s)", PVRVERSION_STRING, PVRVERSION_FILE);
		PDUMPCOMMENT("Start of Init Phase");
	}

	__DtTestPoint(__DtFunc_PDumpInit, __DtStep_10)
	return;

init_failed:

	if(gsDBGPdumpState.pszFile)
	{
		__DtTestPoint(__DtFunc_PDumpInit, __DtStep_11)
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, SZ_FILENAME_SIZE_MAX, (IMG_PVOID) gsDBGPdumpState.pszFile, 0);
		gsDBGPdumpState.pszFile = IMG_NULL;
	}

	if(gsDBGPdumpState.pszScript)
	{
		__DtTestPoint(__DtFunc_PDumpInit, __DtStep_12)
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, SZ_SCRIPT_SIZE_MAX, (IMG_PVOID) gsDBGPdumpState.pszScript, 0);
		gsDBGPdumpState.pszScript = IMG_NULL;
	}

	if(gsDBGPdumpState.pszMsg)
	{
		__DtTestPoint(__DtFunc_PDumpInit, __DtStep_13)
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, SZ_MSG_SIZE_MAX, (IMG_PVOID) gsDBGPdumpState.pszMsg, 0);
		gsDBGPdumpState.pszMsg = IMG_NULL;
	}

	
	sConnectNotifier.pfnConnectNotifier = 0;
	gpfnDbgDrv->pfnSetConnectNotifier(sConnectNotifier);

	gpfnDbgDrv = IMG_NULL;
}


IMG_VOID PDumpDeInit(IMG_VOID)
{
	IMG_UINT32 i;
	DBGKM_CONNECT_NOTIFIER sConnectNotifier;

	__DtTestPoint(__DtFunc_PDumpDeInit, __DtStep_0)
	for(i=0; i < PDUMP_NUM_STREAMS; i++)
	{
		__DtTestPoint(__DtFunc_PDumpDeInit, __DtStep_1)
		gpfnDbgDrv->pfnDestroyStream(gsDBGPdumpState.psStream[i]);
	}

	if(gsDBGPdumpState.pszFile)
	{
		__DtTestPoint(__DtFunc_PDumpDeInit, __DtStep_2)
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, SZ_FILENAME_SIZE_MAX, (IMG_PVOID) gsDBGPdumpState.pszFile, 0);
		gsDBGPdumpState.pszFile = IMG_NULL;
	}

	if(gsDBGPdumpState.pszScript)
	{
		__DtTestPoint(__DtFunc_PDumpDeInit, __DtStep_3)
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, SZ_SCRIPT_SIZE_MAX, (IMG_PVOID) gsDBGPdumpState.pszScript, 0);
		gsDBGPdumpState.pszScript = IMG_NULL;
	}

	if(gsDBGPdumpState.pszMsg)
	{
		__DtTestPoint(__DtFunc_PDumpDeInit, __DtStep_4)
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, SZ_MSG_SIZE_MAX, (IMG_PVOID) gsDBGPdumpState.pszMsg, 0);
		gsDBGPdumpState.pszMsg = IMG_NULL;
	}

	
	sConnectNotifier.pfnConnectNotifier = 0;
	gpfnDbgDrv->pfnSetConnectNotifier(sConnectNotifier);

	gpfnDbgDrv = IMG_NULL;
	__DtTestPoint(__DtFunc_PDumpDeInit, __DtStep_5)
}

PVRSRV_ERROR PDumpStartInitPhaseKM(IMG_VOID)
{
	IMG_UINT32 i;

	__DtTestPoint(__DtFunc_PDumpStartInitPhaseKM, __DtStep_0)
	if (gpfnDbgDrv)
	{
		__DtTestPoint(__DtFunc_PDumpStartInitPhaseKM, __DtStep_1)
		PDUMPCOMMENT("Start Init Phase");
		for(i=0; i < PDUMP_NUM_STREAMS; i++)
		{
			__DtTestPoint(__DtFunc_PDumpStartInitPhaseKM, __DtStep_2)
			gpfnDbgDrv->pfnStartInitPhase(gsDBGPdumpState.psStream[i]);
		}
	}
	__DtTestPoint(__DtFunc_PDumpStartInitPhaseKM, __DtStep_3)
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpStopInitPhaseKM(IMG_VOID)
{
	IMG_UINT32 i;

	__DtTestPoint(__DtFunc_PDumpStopInitPhaseKM, __DtStep_0)
	if (gpfnDbgDrv)
	{
		__DtTestPoint(__DtFunc_PDumpStopInitPhaseKM, __DtStep_1)
		PDUMPCOMMENT("Stop Init Phase");

		for(i=0; i < PDUMP_NUM_STREAMS; i++)
		{
			__DtTestPoint(__DtFunc_PDumpStopInitPhaseKM, __DtStep_2)
			gpfnDbgDrv->pfnStopInitPhase(gsDBGPdumpState.psStream[i]);
		}
	}
	__DtTestPoint(__DtFunc_PDumpStopInitPhaseKM, __DtStep_3)
	return PVRSRV_OK;
}

IMG_BOOL PDumpIsLastCaptureFrameKM(IMG_VOID)
{
	__DtTestPoint(__DtFunc_PDumpIsLastCaptureFrameKM, __DtStep_0)
	return gpfnDbgDrv->pfnIsLastCaptureFrame(gsDBGPdumpState.psStream[PDUMP_STREAM_SCRIPT2]);
}


IMG_BOOL PDumpOSIsCaptureFrameKM(IMG_VOID)
{
	__DtTestPoint(__DtFunc_PDumpOSIsCaptureFrameKM, __DtStep_0)
	if (PDumpSuspended())
	{
		__DtTestPoint(__DtFunc_PDumpOSIsCaptureFrameKM, __DtStep_1)
		return IMG_FALSE;
	}
	__DtTestPoint(__DtFunc_PDumpOSIsCaptureFrameKM, __DtStep_2)
	return gpfnDbgDrv->pfnIsCaptureFrame(gsDBGPdumpState.psStream[PDUMP_STREAM_SCRIPT2], IMG_FALSE);
}

PVRSRV_ERROR PDumpOSSetFrameKM(IMG_UINT32 ui32Frame)
{
	IMG_UINT32	ui32Stream;

	__DtTestPoint(__DtFunc_PDumpOSSetFrameKM, __DtStep_0)
	for	(ui32Stream = 0; ui32Stream < PDUMP_NUM_STREAMS; ui32Stream++)
	{
		__DtTestPoint(__DtFunc_PDumpOSSetFrameKM, __DtStep_1)
		if	(gsDBGPdumpState.psStream[ui32Stream])
		{
			__DtTestPoint(__DtFunc_PDumpOSSetFrameKM, __DtStep_2)
			DbgSetFrame(gsDBGPdumpState.psStream[ui32Stream], ui32Frame);
		}
	}

	__DtTestPoint(__DtFunc_PDumpOSSetFrameKM, __DtStep_3)
	return PVRSRV_OK;
}


static IMG_BOOL PDumpWriteString2(IMG_CHAR * pszString, IMG_UINT32 ui32Flags)
{
	__DtTestPoint(__DtFunc_PDumpWriteString2, __DtStep_0)
	return PDumpWriteILock(gsDBGPdumpState.psStream[PDUMP_STREAM_SCRIPT2], (IMG_UINT8 *) pszString, strlen(pszString), ui32Flags);
}


static IMG_BOOL PDumpWriteILock(PDBG_STREAM psStream, IMG_UINT8 *pui8Data, IMG_UINT32 ui32Count, IMG_UINT32 ui32Flags)
{
	IMG_UINT32 ui32Written = 0;
	__DtTestPoint(__DtFunc_PDumpWriteILock, __DtStep_0)
	if ((psStream == IMG_NULL) || PDumpSuspended() || ((ui32Flags & PDUMP_FLAGS_NEVER) != 0))
	{
		__DtTestPoint(__DtFunc_PDumpWriteILock, __DtStep_1)
		PVR_DPF((PVR_DBG_MESSAGE, "PDumpWriteILock: Failed to write 0x%x bytes to stream 0x%x", ui32Count, (IMG_UINT32)psStream));
		__DtTestPoint(__DtFunc_PDumpWriteILock, __DtStep_2)
		return IMG_TRUE;
	}


	

	if (psStream == gsDBGPdumpState.psStream[PDUMP_STREAM_PARAM2])
	{
		IMG_UINT32 ui32ParamOutPos = gpfnDbgDrv->pfnGetStreamOffset(gsDBGPdumpState.psStream[PDUMP_STREAM_PARAM2]);

		__DtTestPoint(__DtFunc_PDumpWriteILock, __DtStep_3)
		if (ui32ParamOutPos + ui32Count > MAX_FILE_SIZE)
		{
			__DtTestPoint(__DtFunc_PDumpWriteILock, __DtStep_4)
			if ((gsDBGPdumpState.psStream[PDUMP_STREAM_SCRIPT2] && PDumpWriteString2("\r\n-- Splitting pdump output file\r\n\r\n", ui32Flags)))
			{
				__DtTestPoint(__DtFunc_PDumpWriteILock, __DtStep_5)
				DbgSetMarker(gsDBGPdumpState.psStream[PDUMP_STREAM_PARAM2], ui32ParamOutPos);
				gsDBGPdumpState.ui32ParamFileNum++;
			}
		}
	}

	ui32Written = DbgWrite(psStream, pui8Data, ui32Count, ui32Flags);

	if (ui32Written == 0xFFFFFFFF)
	{
		__DtTestPoint(__DtFunc_PDumpWriteILock, __DtStep_6)
		return IMG_FALSE;
	}

	__DtTestPoint(__DtFunc_PDumpWriteILock, __DtStep_7)
	return IMG_TRUE;
}

static IMG_VOID DbgSetFrame(PDBG_STREAM psStream, IMG_UINT32 ui32Frame)
{
	__DtTestPoint(__DtFunc_DbgSetFrame, __DtStep_0)
	gpfnDbgDrv->pfnSetFrame(psStream, ui32Frame);
	__DtTestPoint(__DtFunc_DbgSetFrame, __DtStep_1)
}

static IMG_VOID DbgSetMarker(PDBG_STREAM psStream, IMG_UINT32 ui32Marker)
{
	__DtTestPoint(__DtFunc_DbgSetMarker, __DtStep_0)
	gpfnDbgDrv->pfnSetMarker(psStream, ui32Marker);
	__DtTestPoint(__DtFunc_DbgSetMarker, __DtStep_1)
}

IMG_VOID PDumpSuspendKM(IMG_VOID)
{
	__DtTestPoint(__DtFunc_PDumpSuspendKM, __DtStep_0)
	atomic_inc(&gsPDumpSuspended);
	__DtTestPoint(__DtFunc_PDumpSuspendKM, __DtStep_1)
}

IMG_VOID PDumpResumeKM(IMG_VOID)
{
	__DtTestPoint(__DtFunc_PDumpResumeKM, __DtStep_0)
	atomic_dec(&gsPDumpSuspended);
	__DtTestPoint(__DtFunc_PDumpResumeKM, __DtStep_1)
}

#endif 
#endif 
