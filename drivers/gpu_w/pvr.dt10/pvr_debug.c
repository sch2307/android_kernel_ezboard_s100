#include	"DT_pvr_debug.h"	/* For DynamicTracer-TestPoint */
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

#ifndef AUTOCONF_INCLUDED
 #include <linux/config.h>
#endif

#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <linux/hardirq.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/string.h>			
#include <stdarg.h>
#include "img_types.h"
#include "servicesext.h"
#include "pvr_debug.h"
#include "srvkm.h"
#include "proc.h"
#include "mutex.h"
#include "linkage.h"
#include "pvr_uaccess.h"

static IMG_BOOL VBAppend(IMG_CHAR *pszBuf, IMG_UINT32 ui32BufSiz,
						 const IMG_CHAR* pszFormat, va_list VArgs)
						 IMG_FORMAT_PRINTF(3, 0);


#if defined(PVRSRV_NEED_PVR_DPF)

#define PVR_MAX_FILEPATH_LEN 256

static IMG_BOOL BAppend(IMG_CHAR *pszBuf, IMG_UINT32 ui32BufSiz,
						const IMG_CHAR *pszFormat, ...)
						IMG_FORMAT_PRINTF(3, 4);

IMG_UINT32 gPVRDebugLevel =
	(DBGPRIV_FATAL | DBGPRIV_ERROR | DBGPRIV_WARNING);

#endif 

#define	PVR_MAX_MSG_LEN PVR_MAX_DEBUG_MESSAGE_LEN

static IMG_CHAR gszBufferNonIRQ[PVR_MAX_MSG_LEN + 1];

static IMG_CHAR gszBufferIRQ[PVR_MAX_MSG_LEN + 1];

static PVRSRV_LINUX_MUTEX gsDebugMutexNonIRQ;

 
static spinlock_t gsDebugLockIRQ = SPIN_LOCK_UNLOCKED;

#if !defined (USE_SPIN_LOCK)  
#define	USE_SPIN_LOCK (in_interrupt() || !preemptible())
#endif

static inline void GetBufferLock(unsigned long *pulLockFlags)
{
	__DtTestPoint(__DtFunc_GetBufferLock, __DtStep_0)
	if (USE_SPIN_LOCK)
	{
		__DtTestPoint(__DtFunc_GetBufferLock, __DtStep_1)
		spin_lock_irqsave(&gsDebugLockIRQ, *pulLockFlags);
	}
	else
	{
		__DtTestPoint(__DtFunc_GetBufferLock, __DtStep_2)
		LinuxLockMutex(&gsDebugMutexNonIRQ);
	}
	__DtTestPoint(__DtFunc_GetBufferLock, __DtStep_3)
}

static inline void ReleaseBufferLock(unsigned long ulLockFlags)
{
	__DtTestPoint(__DtFunc_ReleaseBufferLock, __DtStep_0)
	if (USE_SPIN_LOCK)
	{
		__DtTestPoint(__DtFunc_ReleaseBufferLock, __DtStep_1)
		spin_unlock_irqrestore(&gsDebugLockIRQ, ulLockFlags);
	}
	else
	{
		__DtTestPoint(__DtFunc_ReleaseBufferLock, __DtStep_2)
		LinuxUnLockMutex(&gsDebugMutexNonIRQ);
	}
	__DtTestPoint(__DtFunc_ReleaseBufferLock, __DtStep_3)
}

static inline void SelectBuffer(IMG_CHAR **ppszBuf, IMG_UINT32 *pui32BufSiz)
{
	__DtTestPoint(__DtFunc_SelectBuffer, __DtStep_0)
	if (USE_SPIN_LOCK)
	{
		__DtTestPoint(__DtFunc_SelectBuffer, __DtStep_1)
		*ppszBuf = gszBufferIRQ;
		*pui32BufSiz = sizeof(gszBufferIRQ);
	}
	else
	{
		__DtTestPoint(__DtFunc_SelectBuffer, __DtStep_2)
		*ppszBuf = gszBufferNonIRQ;
		*pui32BufSiz = sizeof(gszBufferNonIRQ);
	}
	__DtTestPoint(__DtFunc_SelectBuffer, __DtStep_3)
}

static IMG_BOOL VBAppend(IMG_CHAR *pszBuf, IMG_UINT32 ui32BufSiz, const IMG_CHAR* pszFormat, va_list VArgs)
{
	IMG_UINT32 ui32Used;
	IMG_UINT32 ui32Space;
	IMG_INT32 i32Len;

	__DtTestPoint(__DtFunc_VBAppend, __DtStep_0)
	ui32Used = strlen(pszBuf);
	BUG_ON(ui32Used >= ui32BufSiz);
	ui32Space = ui32BufSiz - ui32Used;

	i32Len = vsnprintf(&pszBuf[ui32Used], ui32Space, pszFormat, VArgs);
	pszBuf[ui32BufSiz - 1] = 0;

	
	__DtTestPoint(__DtFunc_VBAppend, __DtStep_1)
	return (i32Len < 0 || i32Len >= (IMG_INT32)ui32Space) ? IMG_TRUE : IMG_FALSE;
}

IMG_VOID PVRDPFInit(IMG_VOID)
{
    __DtTestPoint(__DtFunc_PVRDPFInit, __DtStep_0)
    LinuxInitMutex(&gsDebugMutexNonIRQ);
	__DtTestPoint(__DtFunc_PVRDPFInit, __DtStep_1)
}

IMG_VOID PVRSRVReleasePrintf(const IMG_CHAR *pszFormat, ...)
{
	va_list vaArgs;
	unsigned long ulLockFlags = 0;
	IMG_CHAR *pszBuf;
	IMG_UINT32 ui32BufSiz;

	__DtTestPoint(__DtFunc_PVRSRVReleasePrintf, __DtStep_0)
	SelectBuffer(&pszBuf, &ui32BufSiz);

	va_start(vaArgs, pszFormat);

	GetBufferLock(&ulLockFlags);
	strncpy (pszBuf, "PVR_K: ", (ui32BufSiz -1));

	if (VBAppend(pszBuf, ui32BufSiz, pszFormat, vaArgs))
	{
		__DtTestPoint(__DtFunc_PVRSRVReleasePrintf, __DtStep_1)
		printk(KERN_INFO "PVR_K:(Message Truncated): %s\n", pszBuf);
	}
	else
	{
		// remvoe [FG]
		// printk(KERN_INFO "%s\n", pszBuf);
		__DtTestPoint(__DtFunc_PVRSRVReleasePrintf, __DtStep_2)
	}

	ReleaseBufferLock(ulLockFlags);
	va_end(vaArgs);

	__DtTestPoint(__DtFunc_PVRSRVReleasePrintf, __DtStep_3)
}

#if defined(PVRSRV_NEED_PVR_ASSERT)

IMG_VOID PVRSRVDebugAssertFail(const IMG_CHAR* pszFile, IMG_UINT32 uLine)
{
	__DtTestPoint(__DtFunc_PVRSRVDebugAssertFail, __DtStep_0)
	PVRSRVDebugPrintf(DBGPRIV_FATAL, pszFile, uLine, "Debug assertion failed!");
#if !defined(SLSI_S5PC110)
	BUG();
#endif
	__DtTestPoint(__DtFunc_PVRSRVDebugAssertFail, __DtStep_1)
}

#endif 

#if defined(PVRSRV_NEED_PVR_TRACE)

IMG_VOID PVRSRVTrace(const IMG_CHAR* pszFormat, ...)
{
	va_list VArgs;
	unsigned long ulLockFlags = 0;
	IMG_CHAR *pszBuf;
	IMG_UINT32 ui32BufSiz;

	__DtTestPoint(__DtFunc_PVRSRVTrace, __DtStep_0)
	SelectBuffer(&pszBuf, &ui32BufSiz);

	va_start(VArgs, pszFormat);

	GetBufferLock(&ulLockFlags);

	strncpy(pszBuf, "PVR: ", (ui32BufSiz -1));

	if (VBAppend(pszBuf, ui32BufSiz, pszFormat, VArgs))
	{
		__DtTestPoint(__DtFunc_PVRSRVTrace, __DtStep_1)
		printk(KERN_INFO "PVR_K:(Message Truncated): %s\n", pszBuf);
	}
	else
	{
		__DtTestPoint(__DtFunc_PVRSRVTrace, __DtStep_2)
		printk(KERN_INFO "%s\n", pszBuf);
	}

	ReleaseBufferLock(ulLockFlags);

	va_end(VArgs);
	__DtTestPoint(__DtFunc_PVRSRVTrace, __DtStep_3)
}

#endif 

#if defined(PVRSRV_NEED_PVR_DPF)

static IMG_BOOL BAppend(IMG_CHAR *pszBuf, IMG_UINT32 ui32BufSiz, const IMG_CHAR *pszFormat, ...)
{
	va_list VArgs;
	IMG_BOOL bTrunc;

	__DtTestPoint(__DtFunc_BAppend, __DtStep_0)
	va_start (VArgs, pszFormat);

	bTrunc = VBAppend(pszBuf, ui32BufSiz, pszFormat, VArgs);

	va_end (VArgs);

	__DtTestPoint(__DtFunc_BAppend, __DtStep_1)
	return bTrunc;
}

IMG_VOID PVRSRVDebugPrintf	(
						IMG_UINT32	ui32DebugLevel,
						const IMG_CHAR*	pszFullFileName,
						IMG_UINT32	ui32Line,
						const IMG_CHAR*	pszFormat,
						...
					)
{
	IMG_BOOL bTrace;
	const IMG_CHAR *pszFileName = pszFullFileName;
	IMG_CHAR *pszLeafName;


	__DtTestPoint(__DtFunc_PVRSRVDebugPrintf, __DtStep_0)
	bTrace = (IMG_BOOL)(ui32DebugLevel & DBGPRIV_CALLTRACE) ? IMG_TRUE : IMG_FALSE;

	if (gPVRDebugLevel & ui32DebugLevel)
	{
		va_list vaArgs;
		unsigned long ulLockFlags = 0;
		IMG_CHAR *pszBuf;
		IMG_UINT32 ui32BufSiz;

		__DtTestPoint(__DtFunc_PVRSRVDebugPrintf, __DtStep_1)
		SelectBuffer(&pszBuf, &ui32BufSiz);

		va_start(vaArgs, pszFormat);

		GetBufferLock(&ulLockFlags);

		
		if (bTrace == IMG_FALSE)
		{
			__DtTestPoint(__DtFunc_PVRSRVDebugPrintf, __DtStep_2)
			switch(ui32DebugLevel)
			{
				case DBGPRIV_FATAL:
				{
					__DtTestPoint(__DtFunc_PVRSRVDebugPrintf, __DtStep_3)
					strncpy (pszBuf, "PVR_K:(Fatal): ", (ui32BufSiz -1));
					break;
				}
				case DBGPRIV_ERROR:
				{
					__DtTestPoint(__DtFunc_PVRSRVDebugPrintf, __DtStep_4)
					strncpy (pszBuf, "PVR_K:(Error): ", (ui32BufSiz -1));
					break;
				}
				case DBGPRIV_WARNING:
				{
					__DtTestPoint(__DtFunc_PVRSRVDebugPrintf, __DtStep_5)
					strncpy (pszBuf, "PVR_K:(Warning): ", (ui32BufSiz -1));
					break;
				}
				case DBGPRIV_MESSAGE:
				{
					__DtTestPoint(__DtFunc_PVRSRVDebugPrintf, __DtStep_6)
					strncpy (pszBuf, "PVR_K:(Message): ", (ui32BufSiz -1));
					break;
				}
				case DBGPRIV_VERBOSE:
				{
					__DtTestPoint(__DtFunc_PVRSRVDebugPrintf, __DtStep_7)
					strncpy (pszBuf, "PVR_K:(Verbose): ", (ui32BufSiz -1));
					break;
				}
				default:
				{
					__DtTestPoint(__DtFunc_PVRSRVDebugPrintf, __DtStep_8)
					strncpy (pszBuf, "PVR_K:(Unknown message level)", (ui32BufSiz -1));
					break;
				}
			}
		}
		else
		{
			__DtTestPoint(__DtFunc_PVRSRVDebugPrintf, __DtStep_9)
			strncpy (pszBuf, "PVR_K: ", (ui32BufSiz -1));
		}

		if (VBAppend(pszBuf, ui32BufSiz, pszFormat, vaArgs))
		{
			__DtTestPoint(__DtFunc_PVRSRVDebugPrintf, __DtStep_10)
			printk(KERN_INFO "PVR_K:(Message Truncated): %s\n", pszBuf);
		}
		else
		{
			
			__DtTestPoint(__DtFunc_PVRSRVDebugPrintf, __DtStep_11)
			if (bTrace == IMG_FALSE)
			{
#ifdef DEBUG_LOG_PATH_TRUNCATE
				
				static IMG_CHAR szFileNameRewrite[PVR_MAX_FILEPATH_LEN];

   				IMG_CHAR* pszTruncIter;
				IMG_CHAR* pszTruncBackInter;

				
				pszFileName = pszFullFileName + strlen(DEBUG_LOG_PATH_TRUNCATE)+1;

				
				strncpy(szFileNameRewrite, pszFileName,PVR_MAX_FILEPATH_LEN);

				if(strlen(szFileNameRewrite) == PVR_MAX_FILEPATH_LEN-1) {
					IMG_CHAR szTruncateMassage[] = "FILENAME TRUNCATED";
					__DtTestPoint(__DtFunc_PVRSRVDebugPrintf, __DtStep_12)
					strcpy(szFileNameRewrite + (PVR_MAX_FILEPATH_LEN - 1 - strlen(szTruncateMassage)), szTruncateMassage);
				}

				pszTruncIter = szFileNameRewrite;
				while(*pszTruncIter++ != 0)
				{
					IMG_CHAR* pszNextStartPoint;
					
					__DtTestPoint(__DtFunc_PVRSRVDebugPrintf, __DtStep_13)
					if(
					   !( ( *pszTruncIter == '/' && (pszTruncIter-4 >= szFileNameRewrite) ) &&
						 ( *(pszTruncIter-1) == '.') &&
						 ( *(pszTruncIter-2) == '.') &&
						 ( *(pszTruncIter-3) == '/') )
					   ) continue;

					
					pszTruncBackInter = pszTruncIter - 3;
					while(*(--pszTruncBackInter) != '/')
					{
						__DtTestPoint(__DtFunc_PVRSRVDebugPrintf, __DtStep_14)
						if(pszTruncBackInter <= szFileNameRewrite) break;
					}
					pszNextStartPoint = pszTruncBackInter;

					
					while(*pszTruncIter != 0)
					{
						__DtTestPoint(__DtFunc_PVRSRVDebugPrintf, __DtStep_15)
						*pszTruncBackInter++ = *pszTruncIter++;
					}
					*pszTruncBackInter = 0;

					
					pszTruncIter = pszNextStartPoint;
				}

				pszFileName = szFileNameRewrite;
				
				if(*pszFileName == '/') pszFileName++;
#endif

#if !defined(__sh__)
				pszLeafName = (IMG_CHAR *)strrchr (pszFileName, '\\');

				if (pszLeafName)
				{
					__DtTestPoint(__DtFunc_PVRSRVDebugPrintf, __DtStep_16)
					pszFileName = pszLeafName;
		       	}
#endif 

				if (BAppend(pszBuf, ui32BufSiz, " [%u, %s]", ui32Line, pszFileName))
				{
					__DtTestPoint(__DtFunc_PVRSRVDebugPrintf, __DtStep_17)
					printk(KERN_INFO "PVR_K:(Message Truncated): %s\n", pszBuf);
				}
				else
				{
					__DtTestPoint(__DtFunc_PVRSRVDebugPrintf, __DtStep_18)
					printk(KERN_INFO "%s\n", pszBuf);
				}
			}
			else
			{
				__DtTestPoint(__DtFunc_PVRSRVDebugPrintf, __DtStep_19)
				printk(KERN_INFO "%s\n", pszBuf);
			}
		}

		ReleaseBufferLock(ulLockFlags);

		va_end (vaArgs);
	}
	__DtTestPoint(__DtFunc_PVRSRVDebugPrintf, __DtStep_20)
}

#endif 

#if defined(DEBUG)

IMG_INT PVRDebugProcSetLevel(struct file *file, const IMG_CHAR *buffer, IMG_UINT32 count, IMG_VOID *data)
{
#define	_PROC_SET_BUFFER_SZ		2
	IMG_CHAR data_buffer[_PROC_SET_BUFFER_SZ];

	if (count != _PROC_SET_BUFFER_SZ)
	{
		__DtTestPoint(__DtFunc_PVRDebugProcSetLevel, __DtStep_0)
		return -EINVAL;
	}
	else
	{
		__DtTestPoint(__DtFunc_PVRDebugProcSetLevel, __DtStep_1)
		if (pvr_copy_from_user(data_buffer, buffer, count))
			return -EINVAL;
		if (data_buffer[count - 1] != '\n')
			return -EINVAL;
		gPVRDebugLevel = data_buffer[0] - '0';
	}
	__DtTestPoint(__DtFunc_PVRDebugProcSetLevel, __DtStep_2)
	return (count);
}

void ProcSeqShowDebugLevel(struct seq_file *sfile,void* el)
{
	__DtTestPoint(__DtFunc_ProcSeqShowDebugLevel, __DtStep_0)
	seq_printf(sfile, "%u\n", gPVRDebugLevel);
	__DtTestPoint(__DtFunc_ProcSeqShowDebugLevel, __DtStep_1)
}

#endif 
