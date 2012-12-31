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

#include "falinux_debug.h"

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
	FA_ENTER();
	FA_EXIT();
	return (atomic_read(&gsPDumpSuspended) != 0) ? IMG_TRUE : IMG_FALSE;
}

PVRSRV_ERROR PDumpOSGetScriptString(IMG_HANDLE *phScript,
									IMG_UINT32 *pui32MaxLen)
{
	FA_ENTER();
	*phScript = (IMG_HANDLE)gsDBGPdumpState.pszScript;
	*pui32MaxLen = SZ_SCRIPT_SIZE_MAX;
	if ((!*phScript) || PDumpSuspended())
	{
		FA_EXIT();
		return PVRSRV_ERROR_PDUMP_NOT_ACTIVE;
	}
	FA_EXIT();
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpOSGetMessageString(IMG_CHAR **ppszMsg,
									 IMG_UINT32 *pui32MaxLen)
{
	FA_ENTER();
	*ppszMsg = gsDBGPdumpState.pszMsg;
	*pui32MaxLen = SZ_MSG_SIZE_MAX;
	if ((!*ppszMsg) || PDumpSuspended())
	{
		FA_EXIT();
		return PVRSRV_ERROR_PDUMP_NOT_ACTIVE;
	}
	FA_EXIT();
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpOSGetFilenameString(IMG_CHAR **ppszFile,
									 IMG_UINT32 *pui32MaxLen)
{
	FA_ENTER();
	*ppszFile = gsDBGPdumpState.pszFile;
	*pui32MaxLen = SZ_FILENAME_SIZE_MAX;
	if ((!*ppszFile) || PDumpSuspended())
	{
		FA_EXIT();
		return PVRSRV_ERROR_PDUMP_NOT_ACTIVE;
	}
	FA_EXIT();
	return PVRSRV_OK;
}

IMG_BOOL PDumpOSWriteString2(IMG_HANDLE hScript, IMG_UINT32 ui32Flags)
{
	FA_ENTER();
	FA_EXIT();
	return PDumpWriteString2(hScript, ui32Flags);
}

PVRSRV_ERROR PDumpOSBufprintf(IMG_HANDLE hBuf, IMG_UINT32 ui32ScriptSizeMax, IMG_CHAR* pszFormat, ...)
{
	IMG_CHAR* pszBuf = hBuf;
	IMG_INT32 n;
	va_list	vaArgs;

	FA_ENTER();

	va_start(vaArgs, pszFormat);

	n = vsnprintf(pszBuf, ui32ScriptSizeMax, pszFormat, vaArgs);

	va_end(vaArgs);

	if (n>=(IMG_INT32)ui32ScriptSizeMax || n==-1)	
	{
		PVR_DPF((PVR_DBG_ERROR, "Buffer overflow detected, pdump output may be incomplete."));

		FA_EXIT();
		return PVRSRV_ERROR_PDUMP_BUF_OVERFLOW;
	}

#if defined(PDUMP_DEBUG_OUTFILES)
	g_ui32EveryLineCounter++;
#endif
	FA_EXIT();
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpOSVSprintf(IMG_CHAR *pszComment, IMG_UINT32 ui32ScriptSizeMax, IMG_CHAR* pszFormat, PDUMP_va_list vaArgs)
{
	IMG_INT32 n;

	FA_ENTER();

	n = vsnprintf(pszComment, ui32ScriptSizeMax, pszFormat, vaArgs);

	if (n>=(IMG_INT32)ui32ScriptSizeMax || n==-1)	
	{
		PVR_DPF((PVR_DBG_ERROR, "Buffer overflow detected, pdump output may be incomplete."));

		FA_EXIT();
		return PVRSRV_ERROR_PDUMP_BUF_OVERFLOW;
	}

	FA_EXIT();
	return PVRSRV_OK;
}

IMG_VOID PDumpOSDebugPrintf(IMG_CHAR* pszFormat, ...)
{
	FA_ENTER();
	PVR_UNREFERENCED_PARAMETER(pszFormat);
	FA_EXIT();
	
}

PVRSRV_ERROR PDumpOSSprintf(IMG_CHAR *pszComment, IMG_UINT32 ui32ScriptSizeMax, IMG_CHAR *pszFormat, ...)
{
	IMG_INT32 n;
	va_list	vaArgs;

	FA_ENTER();

	va_start(vaArgs, pszFormat);

	n = vsnprintf(pszComment, ui32ScriptSizeMax, pszFormat, vaArgs);

	va_end(vaArgs);

	if (n>=(IMG_INT32)ui32ScriptSizeMax || n==-1)	
	{
		PVR_DPF((PVR_DBG_ERROR, "Buffer overflow detected, pdump output may be incomplete."));
	
		FA_EXIT();
		return PVRSRV_ERROR_PDUMP_BUF_OVERFLOW;
	}

	FA_EXIT();
	return PVRSRV_OK;
}

IMG_UINT32 PDumpOSBuflen(IMG_HANDLE hBuffer, IMG_UINT32 ui32BufferSizeMax)
{
	IMG_CHAR* pszBuf = hBuffer;
	IMG_UINT32 ui32Count = 0;

	FA_ENTER();

	while ((pszBuf[ui32Count]!=0) && (ui32Count<ui32BufferSizeMax) )
	{
		ui32Count++;
	}
	FA_EXIT();
	return(ui32Count);
}

IMG_VOID PDumpOSVerifyLineEnding(IMG_HANDLE hBuffer, IMG_UINT32 ui32BufferSizeMax)
{
	IMG_UINT32 ui32Count;
	IMG_CHAR* pszBuf = hBuffer;

	FA_ENTER();
	
	ui32Count = PDumpOSBuflen(hBuffer, ui32BufferSizeMax);

	
	if ((ui32Count >= 1) && (pszBuf[ui32Count-1] != '\n') && (ui32Count<ui32BufferSizeMax))
	{
		pszBuf[ui32Count] = '\n';
		ui32Count++;
		pszBuf[ui32Count] = '\0';
	}
	if ((ui32Count >= 2) && (pszBuf[ui32Count-2] != '\r') && (ui32Count<ui32BufferSizeMax))
	{
		pszBuf[ui32Count-1] = '\r';
		pszBuf[ui32Count] = '\n';
		ui32Count++;
		pszBuf[ui32Count] = '\0';
	}
	FA_EXIT();
}

IMG_HANDLE PDumpOSGetStream(IMG_UINT32 ePDumpStream)
{
	FA_ENTER();
	FA_EXIT();
	return (IMG_HANDLE)gsDBGPdumpState.psStream[ePDumpStream];
}

IMG_UINT32 PDumpOSGetStreamOffset(IMG_UINT32 ePDumpStream)
{
	FA_ENTER();
	PDBG_STREAM psStream = gsDBGPdumpState.psStream[ePDumpStream];
	FA_EXIT();
	return gpfnDbgDrv->pfnGetStreamOffset(psStream);
}

IMG_UINT32 PDumpOSGetParamFileNum(IMG_VOID)
{
	FA_ENTER();
	FA_EXIT();
	return gsDBGPdumpState.ui32ParamFileNum;
}

IMG_BOOL PDumpOSWriteString(IMG_HANDLE hStream,
		IMG_UINT8 *psui8Data,
		IMG_UINT32 ui32Size,
		IMG_UINT32 ui32Flags)
{
	PDBG_STREAM psStream = (PDBG_STREAM)hStream;
	FA_ENTER();
	FA_EXIT();
	return PDumpWriteILock(psStream,
					psui8Data,
					ui32Size,
					ui32Flags);
}

IMG_VOID PDumpOSCheckForSplitting(IMG_HANDLE hStream, IMG_UINT32 ui32Size, IMG_UINT32 ui32Flags)
{
	FA_ENTER();
	PVR_UNREFERENCED_PARAMETER(hStream);
	PVR_UNREFERENCED_PARAMETER(ui32Size);
	PVR_UNREFERENCED_PARAMETER(ui32Flags);
	FA_EXIT();
}

IMG_BOOL PDumpOSJTInitialised(IMG_VOID)
{
	FA_ENTER();
	if(gpfnDbgDrv)
	{
		FA_EXIT();
		return IMG_TRUE;
	}
	FA_EXIT();
	return IMG_FALSE;
}

inline IMG_BOOL PDumpOSIsSuspended(IMG_VOID)
{
	FA_ENTER();
	FA_EXIT();
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

	FA_ENTER();	

	PVR_UNREFERENCED_PARAMETER(pui8LinAddr);
	PVR_UNREFERENCED_PARAMETER(ui32PageSize);   

	
	   
	PVR_ASSERT (hOSMemHandle != IMG_NULL);
	
	sCpuPAddr = OSMemHandleToCpuPAddr(hOSMemHandle, ui32Offset);
	PVR_ASSERT((sCpuPAddr.uiAddr & (ui32PageSize - 1)) == 0);

	
	*psDevPAddr = SysCpuPAddrToDevPAddr(eDeviceType, sCpuPAddr);
	FA_EXIT();
}

IMG_VOID PDumpOSCPUVAddrToPhysPages(IMG_HANDLE hOSMemHandle,
		IMG_UINT32 ui32Offset,
		IMG_PUINT8 pui8LinAddr,
		IMG_UINT32 ui32DataPageMask,
		IMG_UINT32 *pui32PageOffset)
{
	FA_ENTER();
	if(hOSMemHandle)
	{
		
		IMG_CPU_PHYADDR     sCpuPAddr;

		PVR_UNREFERENCED_PARAMETER(pui8LinAddr);

		sCpuPAddr = OSMemHandleToCpuPAddr(hOSMemHandle, ui32Offset);
	    *pui32PageOffset = sCpuPAddr.uiAddr & ui32DataPageMask;
	}
	else
	{
		PVR_UNREFERENCED_PARAMETER(hOSMemHandle);
		PVR_UNREFERENCED_PARAMETER(ui32Offset);

		*pui32PageOffset = ((IMG_UINT32)pui8LinAddr & ui32DataPageMask);
	}
	FA_EXIT();
}

IMG_UINT32 PDumpOSDebugDriverWrite( PDBG_STREAM psStream,
									PDUMP_DDWMODE eDbgDrvWriteMode,
									IMG_UINT8 *pui8Data,
									IMG_UINT32 ui32BCount,
									IMG_UINT32 ui32Level,
									IMG_UINT32 ui32DbgDrvFlags)
{
	FA_ENTER();
	switch(eDbgDrvWriteMode)
	{
		case PDUMP_WRITE_MODE_CONTINUOUS:
			PVR_UNREFERENCED_PARAMETER(ui32DbgDrvFlags);
			FA_EXIT();
			return gpfnDbgDrv->pfnDBGDrivWrite2(psStream, pui8Data, ui32BCount, ui32Level);
		case PDUMP_WRITE_MODE_LASTFRAME:
			FA_EXIT();
			return gpfnDbgDrv->pfnWriteLF(psStream, pui8Data, ui32BCount, ui32Level, ui32DbgDrvFlags);
		case PDUMP_WRITE_MODE_BINCM:
			PVR_UNREFERENCED_PARAMETER(ui32DbgDrvFlags);
			FA_EXIT();
			return gpfnDbgDrv->pfnWriteBINCM(psStream, pui8Data, ui32BCount, ui32Level);
		case PDUMP_WRITE_MODE_PERSISTENT:
			PVR_UNREFERENCED_PARAMETER(ui32DbgDrvFlags);
			FA_EXIT();
			return gpfnDbgDrv->pfnWritePersist(psStream, pui8Data, ui32BCount, ui32Level);
		default:
			PVR_UNREFERENCED_PARAMETER(ui32DbgDrvFlags);
			break;
	}
	FA_EXIT();
	return 0xFFFFFFFFU;
}

IMG_VOID PDumpOSReleaseExecution(IMG_VOID)
{
	FA_ENTER();
	OSReleaseThreadQuanta();
	FA_EXIT();
}

IMG_VOID PDumpInit(IMG_VOID)
{
	IMG_UINT32 i;
	FA_ENTER();
	DBGKM_CONNECT_NOTIFIER sConnectNotifier;

	
	if (!gpfnDbgDrv)
	{
		DBGDrvGetServiceTable((IMG_VOID **)&gpfnDbgDrv);


		
		if (gpfnDbgDrv == IMG_NULL)
		{
			FA_EXIT();
			return;
		}
		
		
		sConnectNotifier.pfnConnectNotifier = &PDumpConnectionNotify;
		gpfnDbgDrv->pfnSetConnectNotifier(sConnectNotifier);

		if(!gsDBGPdumpState.pszFile)
		{
			if(OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP, SZ_FILENAME_SIZE_MAX, (IMG_PVOID *)&gsDBGPdumpState.pszFile, 0,
				"Filename string") != PVRSRV_OK)
			{
				goto init_failed;
			}
		}

		if(!gsDBGPdumpState.pszMsg)
		{
			if(OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP, SZ_MSG_SIZE_MAX, (IMG_PVOID *)&gsDBGPdumpState.pszMsg, 0,
				"Message string") != PVRSRV_OK)
			{
				goto init_failed;
			}
		}

		if(!gsDBGPdumpState.pszScript)
		{
			if(OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP, SZ_SCRIPT_SIZE_MAX, (IMG_PVOID *)&gsDBGPdumpState.pszScript, 0,
				"Script string") != PVRSRV_OK)
			{
				goto init_failed;
			}
		}

		for(i=0; i < PDUMP_NUM_STREAMS; i++)
		{
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
	FA_EXIT();
	return;

init_failed:

	if(gsDBGPdumpState.pszFile)
	{
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, SZ_FILENAME_SIZE_MAX, (IMG_PVOID) gsDBGPdumpState.pszFile, 0);
		gsDBGPdumpState.pszFile = IMG_NULL;
	}

	if(gsDBGPdumpState.pszScript)
	{
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, SZ_SCRIPT_SIZE_MAX, (IMG_PVOID) gsDBGPdumpState.pszScript, 0);
		gsDBGPdumpState.pszScript = IMG_NULL;
	}

	if(gsDBGPdumpState.pszMsg)
	{
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, SZ_MSG_SIZE_MAX, (IMG_PVOID) gsDBGPdumpState.pszMsg, 0);
		gsDBGPdumpState.pszMsg = IMG_NULL;
	}

	
	sConnectNotifier.pfnConnectNotifier = 0;
	gpfnDbgDrv->pfnSetConnectNotifier(sConnectNotifier);

	gpfnDbgDrv = IMG_NULL;
	FA_EXIT();
}


IMG_VOID PDumpDeInit(IMG_VOID)
{
	IMG_UINT32 i;
	DBGKM_CONNECT_NOTIFIER sConnectNotifier;

	FA_ENTER();

	for(i=0; i < PDUMP_NUM_STREAMS; i++)
	{
		gpfnDbgDrv->pfnDestroyStream(gsDBGPdumpState.psStream[i]);
	}

	if(gsDBGPdumpState.pszFile)
	{
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, SZ_FILENAME_SIZE_MAX, (IMG_PVOID) gsDBGPdumpState.pszFile, 0);
		gsDBGPdumpState.pszFile = IMG_NULL;
	}

	if(gsDBGPdumpState.pszScript)
	{
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, SZ_SCRIPT_SIZE_MAX, (IMG_PVOID) gsDBGPdumpState.pszScript, 0);
		gsDBGPdumpState.pszScript = IMG_NULL;
	}

	if(gsDBGPdumpState.pszMsg)
	{
		OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP, SZ_MSG_SIZE_MAX, (IMG_PVOID) gsDBGPdumpState.pszMsg, 0);
		gsDBGPdumpState.pszMsg = IMG_NULL;
	}

	
	sConnectNotifier.pfnConnectNotifier = 0;
	gpfnDbgDrv->pfnSetConnectNotifier(sConnectNotifier);

	gpfnDbgDrv = IMG_NULL;
	FA_EXIT();
}

PVRSRV_ERROR PDumpStartInitPhaseKM(IMG_VOID)
{
	IMG_UINT32 i;

	FA_ENTER();

	if (gpfnDbgDrv)
	{
		PDUMPCOMMENT("Start Init Phase");
		for(i=0; i < PDUMP_NUM_STREAMS; i++)
		{
			gpfnDbgDrv->pfnStartInitPhase(gsDBGPdumpState.psStream[i]);
		}
	}
	FA_EXIT();
	return PVRSRV_OK;
}

PVRSRV_ERROR PDumpStopInitPhaseKM(IMG_VOID)
{
	IMG_UINT32 i;

	FA_ENTER();

	if (gpfnDbgDrv)
	{
		PDUMPCOMMENT("Stop Init Phase");

		for(i=0; i < PDUMP_NUM_STREAMS; i++)
		{
			gpfnDbgDrv->pfnStopInitPhase(gsDBGPdumpState.psStream[i]);
		}
	}
	FA_EXIT();
	return PVRSRV_OK;
}

IMG_BOOL PDumpIsLastCaptureFrameKM(IMG_VOID)
{
	FA_ENTER();
	FA_EXIT();
	return gpfnDbgDrv->pfnIsLastCaptureFrame(gsDBGPdumpState.psStream[PDUMP_STREAM_SCRIPT2]);
}


IMG_BOOL PDumpOSIsCaptureFrameKM(IMG_VOID)
{
	FA_ENTER();
	if (PDumpSuspended())
	{
		FA_EXIT();
		return IMG_FALSE;
	}
	FA_EXIT();
	return gpfnDbgDrv->pfnIsCaptureFrame(gsDBGPdumpState.psStream[PDUMP_STREAM_SCRIPT2], IMG_FALSE);
}

PVRSRV_ERROR PDumpOSSetFrameKM(IMG_UINT32 ui32Frame)
{
	IMG_UINT32	ui32Stream;

	FA_ENTER();

	for	(ui32Stream = 0; ui32Stream < PDUMP_NUM_STREAMS; ui32Stream++)
	{
		if	(gsDBGPdumpState.psStream[ui32Stream])
		{
			DbgSetFrame(gsDBGPdumpState.psStream[ui32Stream], ui32Frame);
		}
	}
	FA_EXIT();
	return PVRSRV_OK;
}


static IMG_BOOL PDumpWriteString2(IMG_CHAR * pszString, IMG_UINT32 ui32Flags)
{
	FA_ENTER();
	FA_EXIT();
	return PDumpWriteILock(gsDBGPdumpState.psStream[PDUMP_STREAM_SCRIPT2], (IMG_UINT8 *) pszString, strlen(pszString), ui32Flags);
}


static IMG_BOOL PDumpWriteILock(PDBG_STREAM psStream, IMG_UINT8 *pui8Data, IMG_UINT32 ui32Count, IMG_UINT32 ui32Flags)
{
	IMG_UINT32 ui32Written = 0;
	
	FA_ENTER();
	
	if ((psStream == IMG_NULL) || PDumpSuspended() || ((ui32Flags & PDUMP_FLAGS_NEVER) != 0))
	{
		PVR_DPF((PVR_DBG_MESSAGE, "PDumpWriteILock: Failed to write 0x%x bytes to stream 0x%x", ui32Count, (IMG_UINT32)psStream));
		FA_EXIT();
		return IMG_TRUE;
	}


	

	if (psStream == gsDBGPdumpState.psStream[PDUMP_STREAM_PARAM2])
	{
		IMG_UINT32 ui32ParamOutPos = gpfnDbgDrv->pfnGetStreamOffset(gsDBGPdumpState.psStream[PDUMP_STREAM_PARAM2]);

		if (ui32ParamOutPos + ui32Count > MAX_FILE_SIZE)
		{
			if ((gsDBGPdumpState.psStream[PDUMP_STREAM_SCRIPT2] && PDumpWriteString2("\r\n-- Splitting pdump output file\r\n\r\n", ui32Flags)))
			{
				DbgSetMarker(gsDBGPdumpState.psStream[PDUMP_STREAM_PARAM2], ui32ParamOutPos);
				gsDBGPdumpState.ui32ParamFileNum++;
			}
		}
	}

	ui32Written = DbgWrite(psStream, pui8Data, ui32Count, ui32Flags);

	if (ui32Written == 0xFFFFFFFF)
	{
		FA_EXIT();
		return IMG_FALSE;
	}

	FA_EXIT();
	return IMG_TRUE;
}

static IMG_VOID DbgSetFrame(PDBG_STREAM psStream, IMG_UINT32 ui32Frame)
{
	FA_ENTER();
	gpfnDbgDrv->pfnSetFrame(psStream, ui32Frame);
	FA_EXIT();
}

static IMG_VOID DbgSetMarker(PDBG_STREAM psStream, IMG_UINT32 ui32Marker)
{
	FA_ENTER();
	gpfnDbgDrv->pfnSetMarker(psStream, ui32Marker);
	FA_EXIT();
}

IMG_VOID PDumpSuspendKM(IMG_VOID)
{
	FA_ENTER();
	atomic_inc(&gsPDumpSuspended);
	FA_EXIT();
}

IMG_VOID PDumpResumeKM(IMG_VOID)
{
	FA_ENTER();
	atomic_dec(&gsPDumpSuspended);
	FA_EXIT();
}

#endif 
#endif 
