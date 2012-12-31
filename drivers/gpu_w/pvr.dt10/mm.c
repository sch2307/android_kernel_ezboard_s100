#include	"DT_mm.h"	/* For DynamicTracer-TestPoint */
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

#include <linux/version.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <asm/io.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0))
#include <linux/wrapper.h>
#endif
#include <linux/slab.h>
#include <linux/highmem.h>
#include <linux/sched.h>

#include "img_defs.h"
#include "services.h"
#include "servicesint.h"
#include "syscommon.h"
#include "mutils.h"
#include "mm.h"
#include "pvrmmap.h"
#include "mmap.h"
#include "osfunc.h"
#include "pvr_debug.h"
#include "proc.h"
#include "mutex.h"
#include "lock.h"

#if defined(DEBUG_LINUX_MEM_AREAS) || defined(DEBUG_LINUX_MEMORY_ALLOCATIONS)
	#include "lists.h"
#endif

#if defined(DEBUG_LINUX_MEMORY_ALLOCATIONS)
typedef enum {
    DEBUG_MEM_ALLOC_TYPE_KMALLOC,
    DEBUG_MEM_ALLOC_TYPE_VMALLOC,
    DEBUG_MEM_ALLOC_TYPE_ALLOC_PAGES,
    DEBUG_MEM_ALLOC_TYPE_IOREMAP,
    DEBUG_MEM_ALLOC_TYPE_IO,
    DEBUG_MEM_ALLOC_TYPE_KMEM_CACHE,
    DEBUG_MEM_ALLOC_TYPE_COUNT
}DEBUG_MEM_ALLOC_TYPE;

typedef struct _DEBUG_MEM_ALLOC_REC
{
    DEBUG_MEM_ALLOC_TYPE    eAllocType;
	IMG_VOID				*pvKey; 
    IMG_VOID                *pvCpuVAddr;
    IMG_UINT32              ulCpuPAddr;
    IMG_VOID                *pvPrivateData;
	IMG_UINT32				ui32Bytes;
	pid_t					pid;
    IMG_CHAR                *pszFileName;
    IMG_UINT32              ui32Line;
    
    struct _DEBUG_MEM_ALLOC_REC   *psNext;
	struct _DEBUG_MEM_ALLOC_REC   **ppsThis;
}DEBUG_MEM_ALLOC_REC;

static IMPLEMENT_LIST_ANY_VA_2(DEBUG_MEM_ALLOC_REC, IMG_BOOL, IMG_FALSE)
static IMPLEMENT_LIST_ANY_VA(DEBUG_MEM_ALLOC_REC)
static IMPLEMENT_LIST_FOR_EACH(DEBUG_MEM_ALLOC_REC)
static IMPLEMENT_LIST_INSERT(DEBUG_MEM_ALLOC_REC)
static IMPLEMENT_LIST_REMOVE(DEBUG_MEM_ALLOC_REC)


static DEBUG_MEM_ALLOC_REC *g_MemoryRecords;

static IMG_UINT32 g_WaterMarkData[DEBUG_MEM_ALLOC_TYPE_COUNT];
static IMG_UINT32 g_HighWaterMarkData[DEBUG_MEM_ALLOC_TYPE_COUNT];

static IMG_UINT32 g_SysRAMWaterMark;
static IMG_UINT32 g_SysRAMHighWaterMark;

static IMG_UINT32 g_IOMemWaterMark;
static IMG_UINT32 g_IOMemHighWaterMark;

static IMG_VOID DebugMemAllocRecordAdd(DEBUG_MEM_ALLOC_TYPE eAllocType,
                                       IMG_VOID *pvKey,
                                       IMG_VOID *pvCpuVAddr,
                                       IMG_UINT32 ulCpuPAddr,
                                       IMG_VOID *pvPrivateData,
                                       IMG_UINT32 ui32Bytes,
                                       IMG_CHAR *pszFileName,
                                       IMG_UINT32 ui32Line);

static IMG_VOID DebugMemAllocRecordRemove(DEBUG_MEM_ALLOC_TYPE eAllocType, IMG_VOID *pvKey, IMG_CHAR *pszFileName, IMG_UINT32 ui32Line);

static IMG_CHAR *DebugMemAllocRecordTypeToString(DEBUG_MEM_ALLOC_TYPE eAllocType);


static struct proc_dir_entry *g_SeqFileMemoryRecords =0;
static void* ProcSeqNextMemoryRecords(struct seq_file *sfile,void* el,loff_t off);
static void ProcSeqShowMemoryRecords(struct seq_file *sfile,void* el);
static void* ProcSeqOff2ElementMemoryRecords(struct seq_file * sfile, loff_t off);

#endif


#if defined(DEBUG_LINUX_MEM_AREAS)
typedef struct _DEBUG_LINUX_MEM_AREA_REC
{
	LinuxMemArea                *psLinuxMemArea;
    IMG_UINT32                  ui32Flags;
	pid_t					    pid;

	struct _DEBUG_LINUX_MEM_AREA_REC  *psNext;
	struct _DEBUG_LINUX_MEM_AREA_REC  **ppsThis;
}DEBUG_LINUX_MEM_AREA_REC;


static IMPLEMENT_LIST_ANY_VA(DEBUG_LINUX_MEM_AREA_REC)
static IMPLEMENT_LIST_FOR_EACH(DEBUG_LINUX_MEM_AREA_REC)
static IMPLEMENT_LIST_INSERT(DEBUG_LINUX_MEM_AREA_REC)
static IMPLEMENT_LIST_REMOVE(DEBUG_LINUX_MEM_AREA_REC)




static DEBUG_LINUX_MEM_AREA_REC *g_LinuxMemAreaRecords;
static IMG_UINT32 g_LinuxMemAreaCount;
static IMG_UINT32 g_LinuxMemAreaWaterMark;
static IMG_UINT32 g_LinuxMemAreaHighWaterMark;


static struct proc_dir_entry *g_SeqFileMemArea=0;

static void* ProcSeqNextMemArea(struct seq_file *sfile,void* el,loff_t off);
static void ProcSeqShowMemArea(struct seq_file *sfile,void* el);
static void* ProcSeqOff2ElementMemArea(struct seq_file *sfile, loff_t off);

#endif

#if defined(DEBUG_LINUX_MEM_AREAS) || defined(DEBUG_LINUX_MEMORY_ALLOCATIONS)
static PVRSRV_LINUX_MUTEX g_sDebugMutex;
#endif

#if (defined(DEBUG_LINUX_MEM_AREAS) || defined(DEBUG_LINUX_MEMORY_ALLOCATIONS))
static void ProcSeqStartstopDebugMutex(struct seq_file *sfile,IMG_BOOL start);
#endif

static LinuxKMemCache *psLinuxMemAreaCache;


#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,15))
static IMG_VOID ReservePages(IMG_VOID *pvAddress, IMG_UINT32 ui32Length);
static IMG_VOID UnreservePages(IMG_VOID *pvAddress, IMG_UINT32 ui32Length);
#endif

static LinuxMemArea *LinuxMemAreaStructAlloc(IMG_VOID);
static IMG_VOID LinuxMemAreaStructFree(LinuxMemArea *psLinuxMemArea);
#if defined(DEBUG_LINUX_MEM_AREAS)
static IMG_VOID DebugLinuxMemAreaRecordAdd(LinuxMemArea *psLinuxMemArea, IMG_UINT32 ui32Flags);
static DEBUG_LINUX_MEM_AREA_REC *DebugLinuxMemAreaRecordFind(LinuxMemArea *psLinuxMemArea);
static IMG_VOID DebugLinuxMemAreaRecordRemove(LinuxMemArea *psLinuxMemArea);
#endif

PVRSRV_ERROR
LinuxMMInit(IMG_VOID)
{
#if defined(DEBUG_LINUX_MEM_AREAS) || defined(DEBUG_LINUX_MEMORY_ALLOCATIONS)
	LinuxInitMutex(&g_sDebugMutex);
#endif

#if defined(DEBUG_LINUX_MEM_AREAS)
    {
		g_SeqFileMemArea = CreateProcReadEntrySeq(
									"mem_areas", 
									NULL, 
									ProcSeqNextMemArea,
									ProcSeqShowMemArea,
									ProcSeqOff2ElementMemArea,
									ProcSeqStartstopDebugMutex
								   );
		if(!g_SeqFileMemArea)
        {
            __DtTestPoint(__DtFunc_LinuxMMInit, __DtStep_0)
            return PVRSRV_ERROR_OUT_OF_MEMORY;
        }
    }
#endif


#if defined(DEBUG_LINUX_MEMORY_ALLOCATIONS)
    {
		g_SeqFileMemoryRecords =CreateProcReadEntrySeq(
									"meminfo", 
									NULL, 
									ProcSeqNextMemoryRecords,
									ProcSeqShowMemoryRecords, 
									ProcSeqOff2ElementMemoryRecords,
									ProcSeqStartstopDebugMutex
								   );
        if(!g_SeqFileMemoryRecords)
        {
            __DtTestPoint(__DtFunc_LinuxMMInit, __DtStep_1)
            return PVRSRV_ERROR_OUT_OF_MEMORY;
        }
    }
#endif

    psLinuxMemAreaCache = KMemCacheCreateWrapper("img-mm", sizeof(LinuxMemArea), 0, 0);
    if(!psLinuxMemAreaCache)
    {
        __DtTestPoint(__DtFunc_LinuxMMInit, __DtStep_2)
        PVR_DPF((PVR_DBG_ERROR,"%s: failed to allocate kmem_cache", __FUNCTION__));
        __DtTestPoint(__DtFunc_LinuxMMInit, __DtStep_3)
        return PVRSRV_ERROR_OUT_OF_MEMORY;
    }

    __DtTestPoint(__DtFunc_LinuxMMInit, __DtStep_4)
    return PVRSRV_OK;
}

#if defined(DEBUG_LINUX_MEM_AREAS)
static IMG_VOID LinuxMMCleanup_MemAreas_ForEachCb(DEBUG_LINUX_MEM_AREA_REC *psCurrentRecord)
{
	LinuxMemArea *psLinuxMemArea;

	__DtTestPoint(__DtFunc_LinuxMMCleanup_MemAreas_ForEachCb, __DtStep_0)
	psLinuxMemArea = psCurrentRecord->psLinuxMemArea;
	PVR_DPF((PVR_DBG_ERROR, "%s: BUG!: Cleaning up Linux memory area (%p), type=%s, size=%d bytes",
				__FUNCTION__,
				psCurrentRecord->psLinuxMemArea,
				LinuxMemAreaTypeToString(psCurrentRecord->psLinuxMemArea->eAreaType),
				psCurrentRecord->psLinuxMemArea->ui32ByteSize));
	
	LinuxMemAreaDeepFree(psLinuxMemArea);
	__DtTestPoint(__DtFunc_LinuxMMCleanup_MemAreas_ForEachCb, __DtStep_1)
}
#endif

#if defined(DEBUG_LINUX_MEMORY_ALLOCATIONS)
static IMG_VOID LinuxMMCleanup_MemRecords_ForEachVa(DEBUG_MEM_ALLOC_REC *psCurrentRecord)

{
	
	__DtTestPoint(__DtFunc_LinuxMMCleanup_MemRecords_ForEachVa, __DtStep_0)
	PVR_DPF((PVR_DBG_ERROR, "%s: BUG!: Cleaning up memory: "
							"type=%s "
							"CpuVAddr=%p "
							"CpuPAddr=0x%08x, "
							"allocated @ file=%s,line=%d",
			__FUNCTION__,
			DebugMemAllocRecordTypeToString(psCurrentRecord->eAllocType),
			psCurrentRecord->pvCpuVAddr,
			psCurrentRecord->ulCpuPAddr,
			psCurrentRecord->pszFileName,
			psCurrentRecord->ui32Line));
	switch(psCurrentRecord->eAllocType)
	{
		case DEBUG_MEM_ALLOC_TYPE_KMALLOC:
			__DtTestPoint(__DtFunc_LinuxMMCleanup_MemRecords_ForEachVa, __DtStep_1)
			KFreeWrapper(psCurrentRecord->pvCpuVAddr);
			break;
		case DEBUG_MEM_ALLOC_TYPE_IOREMAP:
			__DtTestPoint(__DtFunc_LinuxMMCleanup_MemRecords_ForEachVa, __DtStep_2)
			IOUnmapWrapper(psCurrentRecord->pvCpuVAddr);
			break;
		case DEBUG_MEM_ALLOC_TYPE_IO:
			
			__DtTestPoint(__DtFunc_LinuxMMCleanup_MemRecords_ForEachVa, __DtStep_3)
			DebugMemAllocRecordRemove(DEBUG_MEM_ALLOC_TYPE_IO, psCurrentRecord->pvKey, __FILE__, __LINE__);
			break;
		case DEBUG_MEM_ALLOC_TYPE_VMALLOC:
			__DtTestPoint(__DtFunc_LinuxMMCleanup_MemRecords_ForEachVa, __DtStep_4)
			VFreeWrapper(psCurrentRecord->pvCpuVAddr);
			break;
		case DEBUG_MEM_ALLOC_TYPE_ALLOC_PAGES:
			
			__DtTestPoint(__DtFunc_LinuxMMCleanup_MemRecords_ForEachVa, __DtStep_5)
			DebugMemAllocRecordRemove(DEBUG_MEM_ALLOC_TYPE_ALLOC_PAGES, psCurrentRecord->pvKey, __FILE__, __LINE__);
			break;
		case DEBUG_MEM_ALLOC_TYPE_KMEM_CACHE:
			__DtTestPoint(__DtFunc_LinuxMMCleanup_MemRecords_ForEachVa, __DtStep_6)
			KMemCacheFreeWrapper(psCurrentRecord->pvPrivateData, psCurrentRecord->pvCpuVAddr);
			break;
		default:
			__DtTestPoint(__DtFunc_LinuxMMCleanup_MemRecords_ForEachVa, __DtStep_7)
			PVR_ASSERT(0);
	}
	__DtTestPoint(__DtFunc_LinuxMMCleanup_MemRecords_ForEachVa, __DtStep_8)
}
#endif


IMG_VOID
LinuxMMCleanup(IMG_VOID)
{

#if defined(DEBUG_LINUX_MEM_AREAS)
    {
        if(g_LinuxMemAreaCount)
        {
            __DtTestPoint(__DtFunc_LinuxMMCleanup, __DtStep_0)
            PVR_DPF((PVR_DBG_ERROR, "%s: BUG!: There are %d LinuxMemArea allocation unfreed (%d bytes)",
                    __FUNCTION__, g_LinuxMemAreaCount, g_LinuxMemAreaWaterMark));
        }
		
		List_DEBUG_LINUX_MEM_AREA_REC_ForEach(g_LinuxMemAreaRecords,
											LinuxMMCleanup_MemAreas_ForEachCb);

		RemoveProcEntrySeq( g_SeqFileMemArea );
    }
#endif


#if defined(DEBUG_LINUX_MEMORY_ALLOCATIONS)
    {
        
        
		List_DEBUG_MEM_ALLOC_REC_ForEach(g_MemoryRecords,
										LinuxMMCleanup_MemRecords_ForEachVa);

		RemoveProcEntrySeq( g_SeqFileMemoryRecords );
    }
#endif

    if(psLinuxMemAreaCache)
    {
        __DtTestPoint(__DtFunc_LinuxMMCleanup, __DtStep_1)
        KMemCacheDestroyWrapper(psLinuxMemAreaCache); 
        psLinuxMemAreaCache=NULL;
    }
	__DtTestPoint(__DtFunc_LinuxMMCleanup, __DtStep_2)
}


IMG_VOID *
_KMallocWrapper(IMG_UINT32 ui32ByteSize, IMG_CHAR *pszFileName, IMG_UINT32 ui32Line)
{
    IMG_VOID *pvRet;
    __DtTestPoint(__DtFunc__KMallocWrapper, __DtStep_0)
    pvRet = kmalloc(ui32ByteSize, GFP_KERNEL);
#if defined(DEBUG_LINUX_MEMORY_ALLOCATIONS)
    if(pvRet)
    {
        __DtTestPoint(__DtFunc__KMallocWrapper, __DtStep_1)
        DebugMemAllocRecordAdd(DEBUG_MEM_ALLOC_TYPE_KMALLOC,
                               pvRet,
                               pvRet,
                               0,
                               NULL,
                               ui32ByteSize,
                               pszFileName,
                               ui32Line
                               );
    }
#else
    PVR_UNREFERENCED_PARAMETER(pszFileName);
    PVR_UNREFERENCED_PARAMETER(ui32Line);
#endif
    return pvRet;
}


IMG_VOID
_KFreeWrapper(IMG_VOID *pvCpuVAddr, IMG_CHAR *pszFileName, IMG_UINT32 ui32Line)
{
#if defined(DEBUG_LINUX_MEMORY_ALLOCATIONS)
    DebugMemAllocRecordRemove(DEBUG_MEM_ALLOC_TYPE_KMALLOC, pvCpuVAddr, pszFileName,  ui32Line);
#else
    PVR_UNREFERENCED_PARAMETER(pszFileName);
    PVR_UNREFERENCED_PARAMETER(ui32Line);
#endif
    kfree(pvCpuVAddr);
	__DtTestPoint(__DtFunc__KFreeWrapper, __DtStep_0)
}


#if defined(DEBUG_LINUX_MEMORY_ALLOCATIONS)
static IMG_VOID
DebugMemAllocRecordAdd(DEBUG_MEM_ALLOC_TYPE eAllocType,
                       IMG_VOID *pvKey,
                       IMG_VOID *pvCpuVAddr,
                       IMG_UINT32 ulCpuPAddr,
                       IMG_VOID *pvPrivateData,
                       IMG_UINT32 ui32Bytes,
                       IMG_CHAR *pszFileName,
                       IMG_UINT32 ui32Line)
{
    DEBUG_MEM_ALLOC_REC *psRecord;

    __DtTestPoint(__DtFunc_DebugMemAllocRecordAdd, __DtStep_0)
    LinuxLockMutex(&g_sDebugMutex);

    psRecord = kmalloc(sizeof(DEBUG_MEM_ALLOC_REC), GFP_KERNEL);

    psRecord->eAllocType = eAllocType;
    psRecord->pvKey = pvKey;
    psRecord->pvCpuVAddr = pvCpuVAddr;
    psRecord->ulCpuPAddr = ulCpuPAddr;
    psRecord->pvPrivateData = pvPrivateData;
    psRecord->pid = current->pid;
    psRecord->ui32Bytes = ui32Bytes;
    psRecord->pszFileName = pszFileName;
    psRecord->ui32Line = ui32Line;
    
	List_DEBUG_MEM_ALLOC_REC_Insert(&g_MemoryRecords, psRecord);
    
    g_WaterMarkData[eAllocType] += ui32Bytes;
    if(g_WaterMarkData[eAllocType] > g_HighWaterMarkData[eAllocType])
    {
        __DtTestPoint(__DtFunc_DebugMemAllocRecordAdd, __DtStep_1)
        g_HighWaterMarkData[eAllocType] = g_WaterMarkData[eAllocType];
    }

    if(eAllocType == DEBUG_MEM_ALLOC_TYPE_KMALLOC
       || eAllocType == DEBUG_MEM_ALLOC_TYPE_VMALLOC
       || eAllocType == DEBUG_MEM_ALLOC_TYPE_ALLOC_PAGES
       || eAllocType == DEBUG_MEM_ALLOC_TYPE_KMEM_CACHE)
    {
        __DtTestPoint(__DtFunc_DebugMemAllocRecordAdd, __DtStep_2)
        g_SysRAMWaterMark += ui32Bytes;
        if(g_SysRAMWaterMark > g_SysRAMHighWaterMark)
        {
            __DtTestPoint(__DtFunc_DebugMemAllocRecordAdd, __DtStep_3)
            g_SysRAMHighWaterMark = g_SysRAMWaterMark;
        }
    }
    else if(eAllocType == DEBUG_MEM_ALLOC_TYPE_IOREMAP
            || eAllocType == DEBUG_MEM_ALLOC_TYPE_IO)
    {
        __DtTestPoint(__DtFunc_DebugMemAllocRecordAdd, __DtStep_4)
        g_IOMemWaterMark += ui32Bytes;
        if(g_IOMemWaterMark > g_IOMemHighWaterMark)
        {
            __DtTestPoint(__DtFunc_DebugMemAllocRecordAdd, __DtStep_5)
            g_IOMemHighWaterMark = g_IOMemWaterMark;
        }
    }

    LinuxUnLockMutex(&g_sDebugMutex);
	__DtTestPoint(__DtFunc_DebugMemAllocRecordAdd, __DtStep_6)
}


static IMG_BOOL DebugMemAllocRecordRemove_AnyVaCb(DEBUG_MEM_ALLOC_REC *psCurrentRecord, va_list va)
{
	DEBUG_MEM_ALLOC_TYPE eAllocType;
	IMG_VOID *pvKey;
	
	__DtTestPoint(__DtFunc_DebugMemAllocRecordRemove_AnyVaCb, __DtStep_0)
	eAllocType = va_arg(va, DEBUG_MEM_ALLOC_TYPE);
	pvKey = va_arg(va, IMG_VOID*);

	if(psCurrentRecord->eAllocType == eAllocType
		&& psCurrentRecord->pvKey == pvKey)
	{
		__DtTestPoint(__DtFunc_DebugMemAllocRecordRemove_AnyVaCb, __DtStep_1)
		eAllocType = psCurrentRecord->eAllocType;
		g_WaterMarkData[eAllocType] -= psCurrentRecord->ui32Bytes;
		
		if(eAllocType == DEBUG_MEM_ALLOC_TYPE_KMALLOC
		   || eAllocType == DEBUG_MEM_ALLOC_TYPE_VMALLOC
		   || eAllocType == DEBUG_MEM_ALLOC_TYPE_ALLOC_PAGES
		   || eAllocType == DEBUG_MEM_ALLOC_TYPE_KMEM_CACHE)
		{
			__DtTestPoint(__DtFunc_DebugMemAllocRecordRemove_AnyVaCb, __DtStep_2)
			g_SysRAMWaterMark -= psCurrentRecord->ui32Bytes;
		}
		else if(eAllocType == DEBUG_MEM_ALLOC_TYPE_IOREMAP
				|| eAllocType == DEBUG_MEM_ALLOC_TYPE_IO)
		{
			__DtTestPoint(__DtFunc_DebugMemAllocRecordRemove_AnyVaCb, __DtStep_3)
			g_IOMemWaterMark -= psCurrentRecord->ui32Bytes;
		}
		
		List_DEBUG_MEM_ALLOC_REC_Remove(psCurrentRecord);
		kfree(psCurrentRecord);

		__DtTestPoint(__DtFunc_DebugMemAllocRecordRemove_AnyVaCb, __DtStep_4)
		return IMG_TRUE;
	}
	else
	{
		__DtTestPoint(__DtFunc_DebugMemAllocRecordRemove_AnyVaCb, __DtStep_5)
		return IMG_FALSE;
	}
	__DtTestPoint(__DtFunc_DebugMemAllocRecordRemove_AnyVaCb, __DtStep_6)
}


static IMG_VOID
DebugMemAllocRecordRemove(DEBUG_MEM_ALLOC_TYPE eAllocType, IMG_VOID *pvKey, IMG_CHAR *pszFileName, IMG_UINT32 ui32Line)
{
    __DtTestPoint(__DtFunc_DebugMemAllocRecordRemove, __DtStep_0)
    LinuxLockMutex(&g_sDebugMutex);

    
	if(!List_DEBUG_MEM_ALLOC_REC_IMG_BOOL_Any_va(g_MemoryRecords,
												DebugMemAllocRecordRemove_AnyVaCb,
												eAllocType,
												pvKey))
	{
		__DtTestPoint(__DtFunc_DebugMemAllocRecordRemove, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "%s: couldn't find an entry for type=%s with pvKey=%p (called from %s, line %d\n",
		__FUNCTION__, DebugMemAllocRecordTypeToString(eAllocType), pvKey,
		pszFileName, ui32Line));
	}

    LinuxUnLockMutex(&g_sDebugMutex);
	__DtTestPoint(__DtFunc_DebugMemAllocRecordRemove, __DtStep_2)
}


static IMG_CHAR *
DebugMemAllocRecordTypeToString(DEBUG_MEM_ALLOC_TYPE eAllocType)
{
    IMG_CHAR *apszDebugMemoryRecordTypes[] = {
        "KMALLOC",
        "VMALLOC",
        "ALLOC_PAGES",
        "IOREMAP",
        "IO",
        "KMEM_CACHE_ALLOC"
    };
    __DtTestPoint(__DtFunc_DebugMemAllocRecordTypeToString, __DtStep_0)
    return apszDebugMemoryRecordTypes[eAllocType];
}
#endif



IMG_VOID *
_VMallocWrapper(IMG_UINT32 ui32Bytes,
                IMG_UINT32 ui32AllocFlags,
                IMG_CHAR *pszFileName,
                IMG_UINT32 ui32Line)
{
    pgprot_t PGProtFlags;
    IMG_VOID *pvRet;
    
    __DtTestPoint(__DtFunc__VMallocWrapper, __DtStep_0)
    switch(ui32AllocFlags & PVRSRV_HAP_CACHETYPE_MASK)
    {
        case PVRSRV_HAP_CACHED:
            __DtTestPoint(__DtFunc__VMallocWrapper, __DtStep_1)
            PGProtFlags = PAGE_KERNEL;
            break;
        case PVRSRV_HAP_WRITECOMBINE:
            __DtTestPoint(__DtFunc__VMallocWrapper, __DtStep_2)
            PGProtFlags = PGPROT_WC(PAGE_KERNEL);
            break;
        case PVRSRV_HAP_UNCACHED:
            __DtTestPoint(__DtFunc__VMallocWrapper, __DtStep_3)
            PGProtFlags = PGPROT_UC(PAGE_KERNEL);
            break;
        default:
            __DtTestPoint(__DtFunc__VMallocWrapper, __DtStep_4)
            PVR_DPF((PVR_DBG_ERROR,
                     "VMAllocWrapper: unknown mapping flags=0x%08x",
                     ui32AllocFlags));
            dump_stack();
            __DtTestPoint(__DtFunc__VMallocWrapper, __DtStep_5)
            return NULL;
    }

	
    pvRet = __vmalloc(ui32Bytes, GFP_KERNEL | __GFP_HIGHMEM, PGProtFlags);
    
#if defined(DEBUG_LINUX_MEMORY_ALLOCATIONS)
    if(pvRet)
    {
        __DtTestPoint(__DtFunc__VMallocWrapper, __DtStep_6)
        DebugMemAllocRecordAdd(DEBUG_MEM_ALLOC_TYPE_VMALLOC,
                               pvRet,
                               pvRet,
                               0,
                               NULL,
                               PAGE_ALIGN(ui32Bytes),
                               pszFileName,
                               ui32Line
                               );
    }
#else
    PVR_UNREFERENCED_PARAMETER(pszFileName);
    PVR_UNREFERENCED_PARAMETER(ui32Line);
#endif

    return pvRet;
}


IMG_VOID
_VFreeWrapper(IMG_VOID *pvCpuVAddr, IMG_CHAR *pszFileName, IMG_UINT32 ui32Line)
{
#if defined(DEBUG_LINUX_MEMORY_ALLOCATIONS)
    DebugMemAllocRecordRemove(DEBUG_MEM_ALLOC_TYPE_VMALLOC, pvCpuVAddr, pszFileName, ui32Line);
#else
    PVR_UNREFERENCED_PARAMETER(pszFileName);
    PVR_UNREFERENCED_PARAMETER(ui32Line);
#endif
    vfree(pvCpuVAddr);
	__DtTestPoint(__DtFunc__VFreeWrapper, __DtStep_0)
}


LinuxMemArea *
NewVMallocLinuxMemArea(IMG_UINT32 ui32Bytes, IMG_UINT32 ui32AreaFlags)
{
    LinuxMemArea *psLinuxMemArea;
    IMG_VOID *pvCpuVAddr;

    __DtTestPoint(__DtFunc_NewVMallocLinuxMemArea, __DtStep_0)
    psLinuxMemArea = LinuxMemAreaStructAlloc();
    if(!psLinuxMemArea)
    {
        __DtTestPoint(__DtFunc_NewVMallocLinuxMemArea, __DtStep_1)
        goto failed;
    }

    pvCpuVAddr = VMallocWrapper(ui32Bytes, ui32AreaFlags);
    if(!pvCpuVAddr)
    {
        __DtTestPoint(__DtFunc_NewVMallocLinuxMemArea, __DtStep_2)
        goto failed;
    }
    
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,15))
    
    ReservePages(pvCpuVAddr, ui32Bytes);
#endif

    psLinuxMemArea->eAreaType = LINUX_MEM_AREA_VMALLOC;
    psLinuxMemArea->uData.sVmalloc.pvVmallocAddress = pvCpuVAddr;
    psLinuxMemArea->ui32ByteSize = ui32Bytes;
    psLinuxMemArea->ui32AreaFlags = ui32AreaFlags;
    INIT_LIST_HEAD(&psLinuxMemArea->sMMapOffsetStructList);

#if defined(DEBUG_LINUX_MEM_AREAS)
    DebugLinuxMemAreaRecordAdd(psLinuxMemArea, ui32AreaFlags);
#endif

    
    if(ui32AreaFlags & (PVRSRV_HAP_WRITECOMBINE | PVRSRV_HAP_UNCACHED))
        OSInvalidateCPUCacheRangeKM(psLinuxMemArea, pvCpuVAddr, ui32Bytes);

    __DtTestPoint(__DtFunc_NewVMallocLinuxMemArea, __DtStep_3)
    return psLinuxMemArea;

failed:
    PVR_DPF((PVR_DBG_ERROR, "%s: failed!", __FUNCTION__));
    if(psLinuxMemArea)
        LinuxMemAreaStructFree(psLinuxMemArea);
    __DtTestPoint(__DtFunc_NewVMallocLinuxMemArea, __DtStep_4)
    return NULL;
}


IMG_VOID
FreeVMallocLinuxMemArea(LinuxMemArea *psLinuxMemArea)
{
    __DtTestPoint(__DtFunc_FreeVMallocLinuxMemArea, __DtStep_0)
    PVR_ASSERT(psLinuxMemArea);
    PVR_ASSERT(psLinuxMemArea->eAreaType == LINUX_MEM_AREA_VMALLOC);
    PVR_ASSERT(psLinuxMemArea->uData.sVmalloc.pvVmallocAddress);

#if defined(DEBUG_LINUX_MEM_AREAS)
    DebugLinuxMemAreaRecordRemove(psLinuxMemArea);
#endif

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,15))
	UnreservePages(psLinuxMemArea->uData.sVmalloc.pvVmallocAddress,
                    psLinuxMemArea->ui32ByteSize);
#endif

    PVR_DPF((PVR_DBG_MESSAGE,"%s: pvCpuVAddr: %p",
             __FUNCTION__, psLinuxMemArea->uData.sVmalloc.pvVmallocAddress));
    VFreeWrapper(psLinuxMemArea->uData.sVmalloc.pvVmallocAddress);

    LinuxMemAreaStructFree(psLinuxMemArea);
	__DtTestPoint(__DtFunc_FreeVMallocLinuxMemArea, __DtStep_1)
}


#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,15))
static IMG_VOID
ReservePages(IMG_VOID *pvAddress, IMG_UINT32 ui32Length)
{
	IMG_VOID *pvPage;
	IMG_VOID *pvEnd = pvAddress + ui32Length;

	__DtTestPoint(__DtFunc_ReservePages, __DtStep_0)
	for(pvPage = pvAddress; pvPage < pvEnd;  pvPage += PAGE_SIZE)
	{
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0))
		SetPageReserved(vmalloc_to_page(pvPage));
#else
		mem_map_reserve(vmalloc_to_page(pvPage));
#endif
	}
	__DtTestPoint(__DtFunc_ReservePages, __DtStep_1)
}


static IMG_VOID
UnreservePages(IMG_VOID *pvAddress, IMG_UINT32 ui32Length)
{
	IMG_VOID *pvPage;
	IMG_VOID *pvEnd = pvAddress + ui32Length;

	__DtTestPoint(__DtFunc_UnreservePages, __DtStep_0)
	for(pvPage = pvAddress; pvPage < pvEnd;  pvPage += PAGE_SIZE)
	{
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0))
		ClearPageReserved(vmalloc_to_page(pvPage));
#else
		mem_map_unreserve(vmalloc_to_page(pvPage));
#endif
	}
	__DtTestPoint(__DtFunc_UnreservePages, __DtStep_1)
}
#endif 


IMG_VOID *
_IORemapWrapper(IMG_CPU_PHYADDR BasePAddr,
               IMG_UINT32 ui32Bytes,
               IMG_UINT32 ui32MappingFlags,
               IMG_CHAR *pszFileName,
               IMG_UINT32 ui32Line)
{
    IMG_VOID *pvIORemapCookie;
    
    __DtTestPoint(__DtFunc__IORemapWrapper, __DtStep_0)
    switch(ui32MappingFlags & PVRSRV_HAP_CACHETYPE_MASK)
    {
        case PVRSRV_HAP_CACHED:
	    __DtTestPoint(__DtFunc__IORemapWrapper, __DtStep_1)
	    pvIORemapCookie = (IMG_VOID *)IOREMAP(BasePAddr.uiAddr, ui32Bytes);
            break;
        case PVRSRV_HAP_WRITECOMBINE:
	    __DtTestPoint(__DtFunc__IORemapWrapper, __DtStep_2)
	    pvIORemapCookie = (IMG_VOID *)IOREMAP_WC(BasePAddr.uiAddr, ui32Bytes);
            break;
        case PVRSRV_HAP_UNCACHED:
            __DtTestPoint(__DtFunc__IORemapWrapper, __DtStep_3)
            pvIORemapCookie = (IMG_VOID *)IOREMAP_UC(BasePAddr.uiAddr, ui32Bytes);
            break;
        default:
            __DtTestPoint(__DtFunc__IORemapWrapper, __DtStep_4)
            PVR_DPF((PVR_DBG_ERROR, "IORemapWrapper: unknown mapping flags"));
            __DtTestPoint(__DtFunc__IORemapWrapper, __DtStep_5)
            return NULL;
    }
    
#if defined(DEBUG_LINUX_MEMORY_ALLOCATIONS)
    if(pvIORemapCookie)
    {
        __DtTestPoint(__DtFunc__IORemapWrapper, __DtStep_6)
        DebugMemAllocRecordAdd(DEBUG_MEM_ALLOC_TYPE_IOREMAP,
                               pvIORemapCookie,
                               pvIORemapCookie,
                               BasePAddr.uiAddr,
                               NULL,
                               ui32Bytes,
                               pszFileName,
                               ui32Line
                               );
    }
#else
    PVR_UNREFERENCED_PARAMETER(pszFileName);
    PVR_UNREFERENCED_PARAMETER(ui32Line);
#endif

    return pvIORemapCookie;
}


IMG_VOID
_IOUnmapWrapper(IMG_VOID *pvIORemapCookie, IMG_CHAR *pszFileName, IMG_UINT32 ui32Line)
{
#if defined(DEBUG_LINUX_MEMORY_ALLOCATIONS)
    DebugMemAllocRecordRemove(DEBUG_MEM_ALLOC_TYPE_IOREMAP, pvIORemapCookie, pszFileName, ui32Line);
#else
    PVR_UNREFERENCED_PARAMETER(pszFileName);
    PVR_UNREFERENCED_PARAMETER(ui32Line);
#endif
    iounmap(pvIORemapCookie);
	__DtTestPoint(__DtFunc__IOUnmapWrapper, __DtStep_0)
}


LinuxMemArea *
NewIORemapLinuxMemArea(IMG_CPU_PHYADDR BasePAddr,
                       IMG_UINT32 ui32Bytes,
                       IMG_UINT32 ui32AreaFlags)
{
    LinuxMemArea *psLinuxMemArea;
    IMG_VOID *pvIORemapCookie;

    __DtTestPoint(__DtFunc_NewIORemapLinuxMemArea, __DtStep_0)
    psLinuxMemArea = LinuxMemAreaStructAlloc();
    if(!psLinuxMemArea)
    {
        __DtTestPoint(__DtFunc_NewIORemapLinuxMemArea, __DtStep_1)
        return NULL;
    }

    pvIORemapCookie = IORemapWrapper(BasePAddr, ui32Bytes, ui32AreaFlags);
    if(!pvIORemapCookie)
    {
        __DtTestPoint(__DtFunc_NewIORemapLinuxMemArea, __DtStep_2)
        LinuxMemAreaStructFree(psLinuxMemArea);
        __DtTestPoint(__DtFunc_NewIORemapLinuxMemArea, __DtStep_3)
        return NULL;
    }

    psLinuxMemArea->eAreaType = LINUX_MEM_AREA_IOREMAP;
    psLinuxMemArea->uData.sIORemap.pvIORemapCookie = pvIORemapCookie;
    psLinuxMemArea->uData.sIORemap.CPUPhysAddr = BasePAddr;
    psLinuxMemArea->ui32ByteSize = ui32Bytes;
    psLinuxMemArea->ui32AreaFlags = ui32AreaFlags;
    INIT_LIST_HEAD(&psLinuxMemArea->sMMapOffsetStructList);

#if defined(DEBUG_LINUX_MEM_AREAS)
    DebugLinuxMemAreaRecordAdd(psLinuxMemArea, ui32AreaFlags);
#endif

    return psLinuxMemArea;
}


IMG_VOID
FreeIORemapLinuxMemArea(LinuxMemArea *psLinuxMemArea)
{
    __DtTestPoint(__DtFunc_FreeIORemapLinuxMemArea, __DtStep_0)
    PVR_ASSERT(psLinuxMemArea->eAreaType == LINUX_MEM_AREA_IOREMAP);

#if defined(DEBUG_LINUX_MEM_AREAS)
    DebugLinuxMemAreaRecordRemove(psLinuxMemArea);
#endif
    
    IOUnmapWrapper(psLinuxMemArea->uData.sIORemap.pvIORemapCookie);

    LinuxMemAreaStructFree(psLinuxMemArea);
	__DtTestPoint(__DtFunc_FreeIORemapLinuxMemArea, __DtStep_1)
}


#if !defined(PVR_MAKE_ALL_PFNS_SPECIAL)
static IMG_BOOL
TreatExternalPagesAsContiguous(IMG_SYS_PHYADDR *psSysPhysAddr, IMG_UINT32 ui32Bytes, IMG_BOOL bPhysContig)
{
	IMG_UINT32 ui32;
	IMG_UINT32 ui32AddrChk;
	IMG_UINT32 ui32NumPages = RANGE_TO_PAGES(ui32Bytes);

	
	__DtTestPoint(__DtFunc_TreatExternalPagesAsContiguous, __DtStep_0)
	for (ui32 = 0, ui32AddrChk = psSysPhysAddr[0].uiAddr;
		ui32 < ui32NumPages;
		ui32++, ui32AddrChk = (bPhysContig) ? (ui32AddrChk + PAGE_SIZE) : psSysPhysAddr[ui32].uiAddr)
	{
		__DtTestPoint(__DtFunc_TreatExternalPagesAsContiguous, __DtStep_1)
		if (!pfn_valid(PHYS_TO_PFN(ui32AddrChk)))
		{
			__DtTestPoint(__DtFunc_TreatExternalPagesAsContiguous, __DtStep_2)
			break;
		}
	}
	if (ui32 == ui32NumPages)
	{
		__DtTestPoint(__DtFunc_TreatExternalPagesAsContiguous, __DtStep_3)
		return IMG_FALSE;
	}

	if (!bPhysContig)
	{
		__DtTestPoint(__DtFunc_TreatExternalPagesAsContiguous, __DtStep_4)
		for (ui32 = 0, ui32AddrChk = psSysPhysAddr[0].uiAddr;
			ui32 < ui32NumPages;
			ui32++, ui32AddrChk += PAGE_SIZE)
		{
			__DtTestPoint(__DtFunc_TreatExternalPagesAsContiguous, __DtStep_5)
			if (psSysPhysAddr[ui32].uiAddr != ui32AddrChk)
			{
				__DtTestPoint(__DtFunc_TreatExternalPagesAsContiguous, __DtStep_6)
				return IMG_FALSE;
			}
		}
	}

	__DtTestPoint(__DtFunc_TreatExternalPagesAsContiguous, __DtStep_7)
	return IMG_TRUE;
}
#endif

LinuxMemArea *NewExternalKVLinuxMemArea(IMG_SYS_PHYADDR *pBasePAddr, IMG_VOID *pvCPUVAddr, IMG_UINT32 ui32Bytes, IMG_BOOL bPhysContig, IMG_UINT32 ui32AreaFlags)
{
    LinuxMemArea *psLinuxMemArea;

    __DtTestPoint(__DtFunc_NewExternalKVLinuxMemArea, __DtStep_0)
    psLinuxMemArea = LinuxMemAreaStructAlloc();
    if(!psLinuxMemArea)
    {
        __DtTestPoint(__DtFunc_NewExternalKVLinuxMemArea, __DtStep_1)
        return NULL;
    }

    psLinuxMemArea->eAreaType = LINUX_MEM_AREA_EXTERNAL_KV;
    psLinuxMemArea->uData.sExternalKV.pvExternalKV = pvCPUVAddr;
    psLinuxMemArea->uData.sExternalKV.bPhysContig =
#if !defined(PVR_MAKE_ALL_PFNS_SPECIAL)
	(bPhysContig || TreatExternalPagesAsContiguous(pBasePAddr, ui32Bytes, bPhysContig))
                                                    ? IMG_TRUE : IMG_FALSE;
#else
	bPhysContig;
#endif
    if (psLinuxMemArea->uData.sExternalKV.bPhysContig)
    {
	__DtTestPoint(__DtFunc_NewExternalKVLinuxMemArea, __DtStep_2)
	psLinuxMemArea->uData.sExternalKV.uPhysAddr.SysPhysAddr = *pBasePAddr;
    }
    else
    {
	__DtTestPoint(__DtFunc_NewExternalKVLinuxMemArea, __DtStep_3)
	psLinuxMemArea->uData.sExternalKV.uPhysAddr.pSysPhysAddr = pBasePAddr;
    }
    psLinuxMemArea->ui32ByteSize = ui32Bytes;
    psLinuxMemArea->ui32AreaFlags = ui32AreaFlags;
    INIT_LIST_HEAD(&psLinuxMemArea->sMMapOffsetStructList);

#if defined(DEBUG_LINUX_MEM_AREAS)
    DebugLinuxMemAreaRecordAdd(psLinuxMemArea, ui32AreaFlags);
#endif

    return psLinuxMemArea;
}


IMG_VOID
FreeExternalKVLinuxMemArea(LinuxMemArea *psLinuxMemArea)
{
    __DtTestPoint(__DtFunc_FreeExternalKVLinuxMemArea, __DtStep_0)
    PVR_ASSERT(psLinuxMemArea->eAreaType == LINUX_MEM_AREA_EXTERNAL_KV);

#if defined(DEBUG_LINUX_MEM_AREAS)
    DebugLinuxMemAreaRecordRemove(psLinuxMemArea);
#endif
    
    LinuxMemAreaStructFree(psLinuxMemArea);
	__DtTestPoint(__DtFunc_FreeExternalKVLinuxMemArea, __DtStep_1)
}


LinuxMemArea *
NewIOLinuxMemArea(IMG_CPU_PHYADDR BasePAddr,
                  IMG_UINT32 ui32Bytes,
                  IMG_UINT32 ui32AreaFlags)
{
    LinuxMemArea *psLinuxMemArea = LinuxMemAreaStructAlloc();
    __DtTestPoint(__DtFunc_NewIOLinuxMemArea, __DtStep_0)
    if(!psLinuxMemArea)
    {
        __DtTestPoint(__DtFunc_NewIOLinuxMemArea, __DtStep_1)
        return NULL;
    }

    
    psLinuxMemArea->eAreaType = LINUX_MEM_AREA_IO;
    psLinuxMemArea->uData.sIO.CPUPhysAddr.uiAddr = BasePAddr.uiAddr;
    psLinuxMemArea->ui32ByteSize = ui32Bytes;
    psLinuxMemArea->ui32AreaFlags = ui32AreaFlags;
    INIT_LIST_HEAD(&psLinuxMemArea->sMMapOffsetStructList);

#if defined(DEBUG_LINUX_MEMORY_ALLOCATIONS)
    DebugMemAllocRecordAdd(DEBUG_MEM_ALLOC_TYPE_IO,
                           (IMG_VOID *)BasePAddr.uiAddr,
                           0,
                           BasePAddr.uiAddr,
                           NULL,
                           ui32Bytes,
                           "unknown",
                           0
                           );
#endif
   
#if defined(DEBUG_LINUX_MEM_AREAS)
    DebugLinuxMemAreaRecordAdd(psLinuxMemArea, ui32AreaFlags);
#endif

    return psLinuxMemArea;
}


IMG_VOID
FreeIOLinuxMemArea(LinuxMemArea *psLinuxMemArea)
{
    __DtTestPoint(__DtFunc_FreeIOLinuxMemArea, __DtStep_0)
    PVR_ASSERT(psLinuxMemArea->eAreaType == LINUX_MEM_AREA_IO);
    
#if defined(DEBUG_LINUX_MEM_AREAS)
    DebugLinuxMemAreaRecordRemove(psLinuxMemArea);
#endif

#if defined(DEBUG_LINUX_MEMORY_ALLOCATIONS)
    DebugMemAllocRecordRemove(DEBUG_MEM_ALLOC_TYPE_IO,
                              (IMG_VOID *)psLinuxMemArea->uData.sIO.CPUPhysAddr.uiAddr, __FILE__, __LINE__);
#endif

    

    LinuxMemAreaStructFree(psLinuxMemArea);
	__DtTestPoint(__DtFunc_FreeIOLinuxMemArea, __DtStep_1)
}


LinuxMemArea *
NewAllocPagesLinuxMemArea(IMG_UINT32 ui32Bytes, IMG_UINT32 ui32AreaFlags)
{
    LinuxMemArea *psLinuxMemArea;
    IMG_UINT32 ui32PageCount;
    struct page **pvPageList;
    IMG_HANDLE hBlockPageList;
    IMG_INT32 i;		
    PVRSRV_ERROR eError;
    
    __DtTestPoint(__DtFunc_NewAllocPagesLinuxMemArea, __DtStep_0)
    psLinuxMemArea = LinuxMemAreaStructAlloc();
    if(!psLinuxMemArea)
    {
        __DtTestPoint(__DtFunc_NewAllocPagesLinuxMemArea, __DtStep_1)
        goto failed_area_alloc;
    }
    
    ui32PageCount = RANGE_TO_PAGES(ui32Bytes);
    eError = OSAllocMem(0, sizeof(*pvPageList) * ui32PageCount, (IMG_VOID **)&pvPageList, &hBlockPageList,
							"Array of pages");
    if(eError != PVRSRV_OK)
    {
        __DtTestPoint(__DtFunc_NewAllocPagesLinuxMemArea, __DtStep_2)
        goto failed_page_list_alloc;
    }
    
    for(i=0; i<(IMG_INT32)ui32PageCount; i++)
    {
        __DtTestPoint(__DtFunc_NewAllocPagesLinuxMemArea, __DtStep_3)
        pvPageList[i] = alloc_pages(GFP_KERNEL | __GFP_HIGHMEM, 0);
        if(!pvPageList[i])
        {
            __DtTestPoint(__DtFunc_NewAllocPagesLinuxMemArea, __DtStep_4)
            goto failed_alloc_pages;
        }
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,15))
    	
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0))		
    	SetPageReserved(pvPageList[i]);
#else
        mem_map_reserve(pvPageList[i]);
#endif
#endif

    }

#if defined(DEBUG_LINUX_MEMORY_ALLOCATIONS)
    DebugMemAllocRecordAdd(DEBUG_MEM_ALLOC_TYPE_ALLOC_PAGES,
                           pvPageList,
                           0,
                           0,
                           NULL,
                           PAGE_ALIGN(ui32Bytes),
                           "unknown",
                           0
                           );
#endif

    psLinuxMemArea->eAreaType = LINUX_MEM_AREA_ALLOC_PAGES;
    psLinuxMemArea->uData.sPageList.pvPageList = pvPageList;
    psLinuxMemArea->uData.sPageList.hBlockPageList = hBlockPageList;
    psLinuxMemArea->ui32ByteSize = ui32Bytes;
    psLinuxMemArea->ui32AreaFlags = ui32AreaFlags;
    INIT_LIST_HEAD(&psLinuxMemArea->sMMapOffsetStructList);

    
    if(ui32AreaFlags & (PVRSRV_HAP_WRITECOMBINE | PVRSRV_HAP_UNCACHED))
    {
        __DtTestPoint(__DtFunc_NewAllocPagesLinuxMemArea, __DtStep_5)
        psLinuxMemArea->bNeedsCacheInvalidate = IMG_TRUE;
    }

#if defined(DEBUG_LINUX_MEM_AREAS)
    DebugLinuxMemAreaRecordAdd(psLinuxMemArea, ui32AreaFlags);
#endif

    return psLinuxMemArea;
    
failed_alloc_pages:
    for(i--; i >= 0; i--)
    {
        __DtTestPoint(__DtFunc_NewAllocPagesLinuxMemArea, __DtStep_6)
        __free_pages(pvPageList[i], 0);
    }
    (IMG_VOID) OSFreeMem(0, sizeof(*pvPageList) * ui32PageCount, pvPageList, hBlockPageList);
	psLinuxMemArea->uData.sPageList.pvPageList = IMG_NULL; 
failed_page_list_alloc:
    LinuxMemAreaStructFree(psLinuxMemArea);
failed_area_alloc:
    PVR_DPF((PVR_DBG_ERROR, "%s: failed", __FUNCTION__));
    
    __DtTestPoint(__DtFunc_NewAllocPagesLinuxMemArea, __DtStep_7)
    return NULL;
}


IMG_VOID
FreeAllocPagesLinuxMemArea(LinuxMemArea *psLinuxMemArea)
{
    IMG_UINT32 ui32PageCount;
    struct page **pvPageList;
    IMG_HANDLE hBlockPageList;
    IMG_INT32 i;

    __DtTestPoint(__DtFunc_FreeAllocPagesLinuxMemArea, __DtStep_0)
    PVR_ASSERT(psLinuxMemArea);
    PVR_ASSERT(psLinuxMemArea->eAreaType == LINUX_MEM_AREA_ALLOC_PAGES);

#if defined(DEBUG_LINUX_MEM_AREAS)
    DebugLinuxMemAreaRecordRemove(psLinuxMemArea);
#endif
    
    ui32PageCount = RANGE_TO_PAGES(psLinuxMemArea->ui32ByteSize);
    pvPageList = psLinuxMemArea->uData.sPageList.pvPageList;
    hBlockPageList = psLinuxMemArea->uData.sPageList.hBlockPageList;
    
#if defined(DEBUG_LINUX_MEMORY_ALLOCATIONS)
    DebugMemAllocRecordRemove(DEBUG_MEM_ALLOC_TYPE_ALLOC_PAGES, pvPageList, __FILE__, __LINE__);
#endif

    for(i=0;i<(IMG_INT32)ui32PageCount;i++)
    {
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,15))		
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,0))		
        ClearPageReserved(pvPageList[i]);
#else
        mem_map_reserve(pvPageList[i]);
#endif		
#endif	
        __free_pages(pvPageList[i], 0);
    }

    (IMG_VOID) OSFreeMem(0, sizeof(*pvPageList) * ui32PageCount, pvPageList, hBlockPageList);
	psLinuxMemArea->uData.sPageList.pvPageList = IMG_NULL; 

    LinuxMemAreaStructFree(psLinuxMemArea);
	__DtTestPoint(__DtFunc_FreeAllocPagesLinuxMemArea, __DtStep_1)
}


struct page*
LinuxMemAreaOffsetToPage(LinuxMemArea *psLinuxMemArea,
                         IMG_UINT32 ui32ByteOffset)
{
    IMG_UINT32 ui32PageIndex;
    IMG_CHAR *pui8Addr;

    __DtTestPoint(__DtFunc_LinuxMemAreaOffsetToPage, __DtStep_0)
    switch(psLinuxMemArea->eAreaType)
    {
        case LINUX_MEM_AREA_ALLOC_PAGES:
            __DtTestPoint(__DtFunc_LinuxMemAreaOffsetToPage, __DtStep_1)
            ui32PageIndex = PHYS_TO_PFN(ui32ByteOffset);
            __DtTestPoint(__DtFunc_LinuxMemAreaOffsetToPage, __DtStep_2)
            return psLinuxMemArea->uData.sPageList.pvPageList[ui32PageIndex];
 
        case LINUX_MEM_AREA_VMALLOC:
            __DtTestPoint(__DtFunc_LinuxMemAreaOffsetToPage, __DtStep_3)
            pui8Addr = psLinuxMemArea->uData.sVmalloc.pvVmallocAddress;
            pui8Addr += ui32ByteOffset;
            __DtTestPoint(__DtFunc_LinuxMemAreaOffsetToPage, __DtStep_4)
            return vmalloc_to_page(pui8Addr);
 
        case LINUX_MEM_AREA_SUB_ALLOC:
             
            __DtTestPoint(__DtFunc_LinuxMemAreaOffsetToPage, __DtStep_5)
            return LinuxMemAreaOffsetToPage(psLinuxMemArea->uData.sSubAlloc.psParentLinuxMemArea,
                                            psLinuxMemArea->uData.sSubAlloc.ui32ByteOffset
                                             + ui32ByteOffset);
        default:
            __DtTestPoint(__DtFunc_LinuxMemAreaOffsetToPage, __DtStep_6)
            PVR_DPF((PVR_DBG_ERROR,
                    "%s: Unsupported request for struct page from LinuxMemArea with type=%s",
                    __FUNCTION__, LinuxMemAreaTypeToString(psLinuxMemArea->eAreaType)));
            __DtTestPoint(__DtFunc_LinuxMemAreaOffsetToPage, __DtStep_7)
            return NULL;
    }
	__DtTestPoint(__DtFunc_LinuxMemAreaOffsetToPage, __DtStep_8)
}


LinuxKMemCache *
KMemCacheCreateWrapper(IMG_CHAR *pszName,
                       size_t Size,
                       size_t Align,
                       IMG_UINT32 ui32Flags)
{
#if defined(DEBUG_LINUX_SLAB_ALLOCATIONS)
    ui32Flags |= SLAB_POISON|SLAB_RED_ZONE;
#endif
    return kmem_cache_create(pszName, Size, Align, ui32Flags, NULL
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,22))
				, NULL
#endif	
			    );
}


IMG_VOID
KMemCacheDestroyWrapper(LinuxKMemCache *psCache)
{
    __DtTestPoint(__DtFunc_KMemCacheDestroyWrapper, __DtStep_0)
    kmem_cache_destroy(psCache);
	__DtTestPoint(__DtFunc_KMemCacheDestroyWrapper, __DtStep_1)
}


IMG_VOID *
_KMemCacheAllocWrapper(LinuxKMemCache *psCache,
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,14))
                      gfp_t Flags,
#else
                      IMG_INT Flags,
#endif
                      IMG_CHAR *pszFileName,
                      IMG_UINT32 ui32Line)
{
    IMG_VOID *pvRet;
    
    __DtTestPoint(__DtFunc__KMemCacheAllocWrapper, __DtStep_0)
    pvRet = kmem_cache_zalloc(psCache, Flags);

#if defined(DEBUG_LINUX_MEMORY_ALLOCATIONS)
    DebugMemAllocRecordAdd(DEBUG_MEM_ALLOC_TYPE_KMEM_CACHE,
                           pvRet,
                           pvRet,
                           0,
                           psCache,
                           kmem_cache_size(psCache),
                           pszFileName,
                           ui32Line
                           );
#else
    PVR_UNREFERENCED_PARAMETER(pszFileName);
    PVR_UNREFERENCED_PARAMETER(ui32Line);
#endif
    
    return pvRet;
}


IMG_VOID
_KMemCacheFreeWrapper(LinuxKMemCache *psCache, IMG_VOID *pvObject, IMG_CHAR *pszFileName, IMG_UINT32 ui32Line)
{
#if defined(DEBUG_LINUX_MEMORY_ALLOCATIONS)
    DebugMemAllocRecordRemove(DEBUG_MEM_ALLOC_TYPE_KMEM_CACHE, pvObject, pszFileName, ui32Line);
#else
    PVR_UNREFERENCED_PARAMETER(pszFileName);
    PVR_UNREFERENCED_PARAMETER(ui32Line);
#endif

    kmem_cache_free(psCache, pvObject);
	__DtTestPoint(__DtFunc__KMemCacheFreeWrapper, __DtStep_0)
}


const IMG_CHAR *
KMemCacheNameWrapper(LinuxKMemCache *psCache)
{
    __DtTestPoint(__DtFunc_KMemCacheNameWrapper, __DtStep_0)
    PVR_UNREFERENCED_PARAMETER(psCache);

    
    __DtTestPoint(__DtFunc_KMemCacheNameWrapper, __DtStep_1)
    return "";
}


LinuxMemArea *
NewSubLinuxMemArea(LinuxMemArea *psParentLinuxMemArea,
                   IMG_UINT32 ui32ByteOffset,
                   IMG_UINT32 ui32Bytes)
{
    LinuxMemArea *psLinuxMemArea;
    
    __DtTestPoint(__DtFunc_NewSubLinuxMemArea, __DtStep_0)
    PVR_ASSERT((ui32ByteOffset+ui32Bytes) <= psParentLinuxMemArea->ui32ByteSize);
    
    psLinuxMemArea = LinuxMemAreaStructAlloc();
    if(!psLinuxMemArea)
    {
        __DtTestPoint(__DtFunc_NewSubLinuxMemArea, __DtStep_1)
        return NULL;
    }
    
    psLinuxMemArea->eAreaType = LINUX_MEM_AREA_SUB_ALLOC;
    psLinuxMemArea->uData.sSubAlloc.psParentLinuxMemArea = psParentLinuxMemArea;
    psLinuxMemArea->uData.sSubAlloc.ui32ByteOffset = ui32ByteOffset;
    psLinuxMemArea->ui32ByteSize = ui32Bytes;
    psLinuxMemArea->ui32AreaFlags = psParentLinuxMemArea->ui32AreaFlags;
    psLinuxMemArea->bNeedsCacheInvalidate = psParentLinuxMemArea->bNeedsCacheInvalidate;
    INIT_LIST_HEAD(&psLinuxMemArea->sMMapOffsetStructList);
    
#if defined(DEBUG_LINUX_MEM_AREAS)
    {
        DEBUG_LINUX_MEM_AREA_REC *psParentRecord;
        psParentRecord = DebugLinuxMemAreaRecordFind(psParentLinuxMemArea);
        DebugLinuxMemAreaRecordAdd(psLinuxMemArea, psParentRecord->ui32Flags);
    }
#endif
    
    return psLinuxMemArea;
}


static IMG_VOID
FreeSubLinuxMemArea(LinuxMemArea *psLinuxMemArea)
{
    __DtTestPoint(__DtFunc_FreeSubLinuxMemArea, __DtStep_0)
    PVR_ASSERT(psLinuxMemArea->eAreaType == LINUX_MEM_AREA_SUB_ALLOC);

#if defined(DEBUG_LINUX_MEM_AREAS)
    DebugLinuxMemAreaRecordRemove(psLinuxMemArea);
#endif
    
    

    LinuxMemAreaStructFree(psLinuxMemArea);
	__DtTestPoint(__DtFunc_FreeSubLinuxMemArea, __DtStep_1)
}


static LinuxMemArea *
LinuxMemAreaStructAlloc(IMG_VOID)
{
#if 0
    LinuxMemArea *psLinuxMemArea;
    psLinuxMemArea = kmem_cache_alloc(psLinuxMemAreaCache, GFP_KERNEL);
    printk(KERN_ERR "%s: psLinuxMemArea=%p\n", __FUNCTION__, psLinuxMemArea);
    dump_stack();
    __DtTestPoint(__DtFunc_LinuxMemAreaStructAlloc, __DtStep_0)
    return psLinuxMemArea;
#else
    return KMemCacheAllocWrapper(psLinuxMemAreaCache, GFP_KERNEL);
#endif
}


static IMG_VOID
LinuxMemAreaStructFree(LinuxMemArea *psLinuxMemArea)
{
    __DtTestPoint(__DtFunc_LinuxMemAreaStructFree, __DtStep_0)
    KMemCacheFreeWrapper(psLinuxMemAreaCache, psLinuxMemArea);
    
    
	__DtTestPoint(__DtFunc_LinuxMemAreaStructFree, __DtStep_1)
}


IMG_VOID
LinuxMemAreaDeepFree(LinuxMemArea *psLinuxMemArea)
{
    __DtTestPoint(__DtFunc_LinuxMemAreaDeepFree, __DtStep_0)
    switch(psLinuxMemArea->eAreaType)
    {
        case LINUX_MEM_AREA_VMALLOC:
            __DtTestPoint(__DtFunc_LinuxMemAreaDeepFree, __DtStep_1)
            FreeVMallocLinuxMemArea(psLinuxMemArea);
            break;
        case LINUX_MEM_AREA_ALLOC_PAGES:
            __DtTestPoint(__DtFunc_LinuxMemAreaDeepFree, __DtStep_2)
            FreeAllocPagesLinuxMemArea(psLinuxMemArea);
            break;
        case LINUX_MEM_AREA_IOREMAP:
            __DtTestPoint(__DtFunc_LinuxMemAreaDeepFree, __DtStep_3)
            FreeIORemapLinuxMemArea(psLinuxMemArea);
            break;
	case LINUX_MEM_AREA_EXTERNAL_KV:
	    __DtTestPoint(__DtFunc_LinuxMemAreaDeepFree, __DtStep_4)
	    FreeExternalKVLinuxMemArea(psLinuxMemArea);
	    break;
        case LINUX_MEM_AREA_IO:
            __DtTestPoint(__DtFunc_LinuxMemAreaDeepFree, __DtStep_5)
            FreeIOLinuxMemArea(psLinuxMemArea);
            break;
        case LINUX_MEM_AREA_SUB_ALLOC:
            __DtTestPoint(__DtFunc_LinuxMemAreaDeepFree, __DtStep_6)
            FreeSubLinuxMemArea(psLinuxMemArea);
            break;
        default:
            __DtTestPoint(__DtFunc_LinuxMemAreaDeepFree, __DtStep_7)
            PVR_DPF((PVR_DBG_ERROR, "%s: Unknown are type (%d)\n",
                     __FUNCTION__, psLinuxMemArea->eAreaType));
            break;
    }
	__DtTestPoint(__DtFunc_LinuxMemAreaDeepFree, __DtStep_8)
}


#if defined(DEBUG_LINUX_MEM_AREAS)
static IMG_VOID
DebugLinuxMemAreaRecordAdd(LinuxMemArea *psLinuxMemArea, IMG_UINT32 ui32Flags)
{
    DEBUG_LINUX_MEM_AREA_REC *psNewRecord;
    const IMG_CHAR *pi8FlagsString;
    
    __DtTestPoint(__DtFunc_DebugLinuxMemAreaRecordAdd, __DtStep_0)
    LinuxLockMutex(&g_sDebugMutex);

    if(psLinuxMemArea->eAreaType != LINUX_MEM_AREA_SUB_ALLOC)
    {
        __DtTestPoint(__DtFunc_DebugLinuxMemAreaRecordAdd, __DtStep_1)
        g_LinuxMemAreaWaterMark += psLinuxMemArea->ui32ByteSize;
        if(g_LinuxMemAreaWaterMark > g_LinuxMemAreaHighWaterMark)
        {
            __DtTestPoint(__DtFunc_DebugLinuxMemAreaRecordAdd, __DtStep_2)
            g_LinuxMemAreaHighWaterMark = g_LinuxMemAreaWaterMark;
        }
    }
    g_LinuxMemAreaCount++;
    
    
    psNewRecord = kmalloc(sizeof(DEBUG_LINUX_MEM_AREA_REC), GFP_KERNEL);
    if(psNewRecord)
    {
        
        __DtTestPoint(__DtFunc_DebugLinuxMemAreaRecordAdd, __DtStep_3)
        psNewRecord->psLinuxMemArea = psLinuxMemArea;
        psNewRecord->ui32Flags = ui32Flags;
        psNewRecord->pid = current->pid;
		
		List_DEBUG_LINUX_MEM_AREA_REC_Insert(&g_LinuxMemAreaRecords, psNewRecord);
    }
    else
    {
        __DtTestPoint(__DtFunc_DebugLinuxMemAreaRecordAdd, __DtStep_4)
        PVR_DPF((PVR_DBG_ERROR,
                 "%s: failed to allocate linux memory area record.",
                 __FUNCTION__));
    }
    
    
    pi8FlagsString = HAPFlagsToString(ui32Flags);
    if(strstr(pi8FlagsString, "UNKNOWN"))
    {
        __DtTestPoint(__DtFunc_DebugLinuxMemAreaRecordAdd, __DtStep_5)
        PVR_DPF((PVR_DBG_ERROR,
                 "%s: Unexpected flags (0x%08x) associated with psLinuxMemArea @ %p",
                 __FUNCTION__,
                 ui32Flags,
                 psLinuxMemArea));
        
    }

    LinuxUnLockMutex(&g_sDebugMutex);
	__DtTestPoint(__DtFunc_DebugLinuxMemAreaRecordAdd, __DtStep_6)
}



static IMG_VOID* MatchLinuxMemArea_AnyVaCb(DEBUG_LINUX_MEM_AREA_REC *psCurrentRecord,
										   va_list va)
{
	LinuxMemArea *psLinuxMemArea;
	
	__DtTestPoint(__DtFunc_MatchLinuxMemArea_AnyVaCb, __DtStep_0)
	psLinuxMemArea = va_arg(va, LinuxMemArea*);
	if(psCurrentRecord->psLinuxMemArea == psLinuxMemArea)
	{
		__DtTestPoint(__DtFunc_MatchLinuxMemArea_AnyVaCb, __DtStep_1)
		return psCurrentRecord;
	}
	else
	{
		__DtTestPoint(__DtFunc_MatchLinuxMemArea_AnyVaCb, __DtStep_2)
		return IMG_NULL;
	}
	__DtTestPoint(__DtFunc_MatchLinuxMemArea_AnyVaCb, __DtStep_3)
}


static DEBUG_LINUX_MEM_AREA_REC *
DebugLinuxMemAreaRecordFind(LinuxMemArea *psLinuxMemArea)
{
    DEBUG_LINUX_MEM_AREA_REC *psCurrentRecord;

    __DtTestPoint(__DtFunc_DebugLinuxMemAreaRecordFind, __DtStep_0)
    LinuxLockMutex(&g_sDebugMutex);
	psCurrentRecord = List_DEBUG_LINUX_MEM_AREA_REC_Any_va(g_LinuxMemAreaRecords,
														MatchLinuxMemArea_AnyVaCb,
														psLinuxMemArea);
	
    LinuxUnLockMutex(&g_sDebugMutex);

    __DtTestPoint(__DtFunc_DebugLinuxMemAreaRecordFind, __DtStep_1)
    return psCurrentRecord;
}


static IMG_VOID
DebugLinuxMemAreaRecordRemove(LinuxMemArea *psLinuxMemArea)
{
    DEBUG_LINUX_MEM_AREA_REC *psCurrentRecord;

    __DtTestPoint(__DtFunc_DebugLinuxMemAreaRecordRemove, __DtStep_0)
    LinuxLockMutex(&g_sDebugMutex);

    if(psLinuxMemArea->eAreaType != LINUX_MEM_AREA_SUB_ALLOC)
    {
        __DtTestPoint(__DtFunc_DebugLinuxMemAreaRecordRemove, __DtStep_1)
        g_LinuxMemAreaWaterMark -= psLinuxMemArea->ui32ByteSize;
    }
    g_LinuxMemAreaCount--;

    
	psCurrentRecord = List_DEBUG_LINUX_MEM_AREA_REC_Any_va(g_LinuxMemAreaRecords,
														MatchLinuxMemArea_AnyVaCb,
														psLinuxMemArea);
	if(psCurrentRecord)
	{
		
		__DtTestPoint(__DtFunc_DebugLinuxMemAreaRecordRemove, __DtStep_2)
		List_DEBUG_LINUX_MEM_AREA_REC_Remove(psCurrentRecord);
		kfree(psCurrentRecord);
	}
	else
	{
		__DtTestPoint(__DtFunc_DebugLinuxMemAreaRecordRemove, __DtStep_3)
		PVR_DPF((PVR_DBG_ERROR, "%s: couldn't find an entry for psLinuxMemArea=%p\n",
        	     __FUNCTION__, psLinuxMemArea));
	}

    LinuxUnLockMutex(&g_sDebugMutex);
	__DtTestPoint(__DtFunc_DebugLinuxMemAreaRecordRemove, __DtStep_4)
}
#endif


IMG_VOID *
LinuxMemAreaToCpuVAddr(LinuxMemArea *psLinuxMemArea)
{
    __DtTestPoint(__DtFunc_LinuxMemAreaToCpuVAddr, __DtStep_0)
    switch(psLinuxMemArea->eAreaType)
    {
        case LINUX_MEM_AREA_VMALLOC:
            __DtTestPoint(__DtFunc_LinuxMemAreaToCpuVAddr, __DtStep_1)
            return psLinuxMemArea->uData.sVmalloc.pvVmallocAddress;
        case LINUX_MEM_AREA_IOREMAP:
            __DtTestPoint(__DtFunc_LinuxMemAreaToCpuVAddr, __DtStep_2)
            return psLinuxMemArea->uData.sIORemap.pvIORemapCookie;
	case LINUX_MEM_AREA_EXTERNAL_KV:
	    __DtTestPoint(__DtFunc_LinuxMemAreaToCpuVAddr, __DtStep_3)
	    return psLinuxMemArea->uData.sExternalKV.pvExternalKV;
        case LINUX_MEM_AREA_SUB_ALLOC:
        {
            IMG_CHAR *pAddr =
                LinuxMemAreaToCpuVAddr(psLinuxMemArea->uData.sSubAlloc.psParentLinuxMemArea);  
            __DtTestPoint(__DtFunc_LinuxMemAreaToCpuVAddr, __DtStep_4)
            if(!pAddr)
            {
                __DtTestPoint(__DtFunc_LinuxMemAreaToCpuVAddr, __DtStep_5)
                return NULL;
            }
            __DtTestPoint(__DtFunc_LinuxMemAreaToCpuVAddr, __DtStep_6)
            return pAddr + psLinuxMemArea->uData.sSubAlloc.ui32ByteOffset;
        }
        default:
            __DtTestPoint(__DtFunc_LinuxMemAreaToCpuVAddr, __DtStep_7)
            return NULL;
    }
	__DtTestPoint(__DtFunc_LinuxMemAreaToCpuVAddr, __DtStep_8)
}


IMG_CPU_PHYADDR
LinuxMemAreaToCpuPAddr(LinuxMemArea *psLinuxMemArea, IMG_UINT32 ui32ByteOffset)
{
    IMG_CPU_PHYADDR CpuPAddr;
    
    __DtTestPoint(__DtFunc_LinuxMemAreaToCpuPAddr, __DtStep_0)
    CpuPAddr.uiAddr = 0;

    switch(psLinuxMemArea->eAreaType)
    {
        case LINUX_MEM_AREA_IOREMAP:
        {
            __DtTestPoint(__DtFunc_LinuxMemAreaToCpuPAddr, __DtStep_1)
            CpuPAddr = psLinuxMemArea->uData.sIORemap.CPUPhysAddr;
            CpuPAddr.uiAddr += ui32ByteOffset;
            break;
        }
	case LINUX_MEM_AREA_EXTERNAL_KV:
	{
	    __DtTestPoint(__DtFunc_LinuxMemAreaToCpuPAddr, __DtStep_2)
	    if (psLinuxMemArea->uData.sExternalKV.bPhysContig)
	    {
		__DtTestPoint(__DtFunc_LinuxMemAreaToCpuPAddr, __DtStep_3)
		CpuPAddr = SysSysPAddrToCpuPAddr(psLinuxMemArea->uData.sExternalKV.uPhysAddr.SysPhysAddr);
		CpuPAddr.uiAddr += ui32ByteOffset;
	    }
	    else
	    {
		IMG_UINT32 ui32PageIndex = PHYS_TO_PFN(ui32ByteOffset);
		IMG_SYS_PHYADDR SysPAddr = psLinuxMemArea->uData.sExternalKV.uPhysAddr.pSysPhysAddr[ui32PageIndex];

		__DtTestPoint(__DtFunc_LinuxMemAreaToCpuPAddr, __DtStep_4)
		CpuPAddr = SysSysPAddrToCpuPAddr(SysPAddr);
                CpuPAddr.uiAddr += ADDR_TO_PAGE_OFFSET(ui32ByteOffset);
	    }
            break;
	}
        case LINUX_MEM_AREA_IO:
        {
            __DtTestPoint(__DtFunc_LinuxMemAreaToCpuPAddr, __DtStep_5)
            CpuPAddr = psLinuxMemArea->uData.sIO.CPUPhysAddr;
            CpuPAddr.uiAddr += ui32ByteOffset;
            break;
        }
        case LINUX_MEM_AREA_VMALLOC:
        {
            IMG_CHAR *pCpuVAddr;
            __DtTestPoint(__DtFunc_LinuxMemAreaToCpuPAddr, __DtStep_6)
            pCpuVAddr =
                (IMG_CHAR *)psLinuxMemArea->uData.sVmalloc.pvVmallocAddress;
            pCpuVAddr += ui32ByteOffset;
            CpuPAddr.uiAddr = VMallocToPhys(pCpuVAddr);
            break;
        }
        case LINUX_MEM_AREA_ALLOC_PAGES:
        {
            struct page *page;
            IMG_UINT32 ui32PageIndex = PHYS_TO_PFN(ui32ByteOffset);
            __DtTestPoint(__DtFunc_LinuxMemAreaToCpuPAddr, __DtStep_7)
            page = psLinuxMemArea->uData.sPageList.pvPageList[ui32PageIndex];
            CpuPAddr.uiAddr = page_to_phys(page);
            CpuPAddr.uiAddr += ADDR_TO_PAGE_OFFSET(ui32ByteOffset);
            break;
        }
        case LINUX_MEM_AREA_SUB_ALLOC:
        {
            __DtTestPoint(__DtFunc_LinuxMemAreaToCpuPAddr, __DtStep_8)
            CpuPAddr =
                OSMemHandleToCpuPAddr(psLinuxMemArea->uData.sSubAlloc.psParentLinuxMemArea,
                                      psLinuxMemArea->uData.sSubAlloc.ui32ByteOffset
                                        + ui32ByteOffset);
            break;
        }
        default:
        {
            __DtTestPoint(__DtFunc_LinuxMemAreaToCpuPAddr, __DtStep_9)
            PVR_DPF((PVR_DBG_ERROR, "%s: Unknown LinuxMemArea type (%d)\n",
                     __FUNCTION__, psLinuxMemArea->eAreaType));
            PVR_ASSERT(CpuPAddr.uiAddr);
           break;
        }
   }
    
    __DtTestPoint(__DtFunc_LinuxMemAreaToCpuPAddr, __DtStep_10)
    return CpuPAddr;
}


IMG_BOOL
LinuxMemAreaPhysIsContig(LinuxMemArea *psLinuxMemArea)
{
    __DtTestPoint(__DtFunc_LinuxMemAreaPhysIsContig, __DtStep_0)
    switch(psLinuxMemArea->eAreaType)
    {
        case LINUX_MEM_AREA_IOREMAP:
        case LINUX_MEM_AREA_IO:
            __DtTestPoint(__DtFunc_LinuxMemAreaPhysIsContig, __DtStep_1)
            return IMG_TRUE;

	case LINUX_MEM_AREA_EXTERNAL_KV:
	    __DtTestPoint(__DtFunc_LinuxMemAreaPhysIsContig, __DtStep_2)
	    return psLinuxMemArea->uData.sExternalKV.bPhysContig;

        case LINUX_MEM_AREA_VMALLOC:
        case LINUX_MEM_AREA_ALLOC_PAGES:
	    __DtTestPoint(__DtFunc_LinuxMemAreaPhysIsContig, __DtStep_3)
	    return IMG_FALSE;

        case LINUX_MEM_AREA_SUB_ALLOC:
             
	    __DtTestPoint(__DtFunc_LinuxMemAreaPhysIsContig, __DtStep_4)
	    return LinuxMemAreaPhysIsContig(psLinuxMemArea->uData.sSubAlloc.psParentLinuxMemArea);

        default:
            __DtTestPoint(__DtFunc_LinuxMemAreaPhysIsContig, __DtStep_5)
            PVR_DPF((PVR_DBG_ERROR, "%s: Unknown LinuxMemArea type (%d)\n",
                     __FUNCTION__, psLinuxMemArea->eAreaType));
	    break;
    }
    __DtTestPoint(__DtFunc_LinuxMemAreaPhysIsContig, __DtStep_6)
    return IMG_FALSE;
}


const IMG_CHAR *
LinuxMemAreaTypeToString(LINUX_MEM_AREA_TYPE eMemAreaType)
{
    
    __DtTestPoint(__DtFunc_LinuxMemAreaTypeToString, __DtStep_0)
    switch(eMemAreaType)
    {
        case LINUX_MEM_AREA_IOREMAP:
            __DtTestPoint(__DtFunc_LinuxMemAreaTypeToString, __DtStep_1)
            return "LINUX_MEM_AREA_IOREMAP";
	case LINUX_MEM_AREA_EXTERNAL_KV:
	    __DtTestPoint(__DtFunc_LinuxMemAreaTypeToString, __DtStep_2)
	    return "LINUX_MEM_AREA_EXTERNAL_KV";
        case LINUX_MEM_AREA_IO:
            __DtTestPoint(__DtFunc_LinuxMemAreaTypeToString, __DtStep_3)
            return "LINUX_MEM_AREA_IO";
        case LINUX_MEM_AREA_VMALLOC:
            __DtTestPoint(__DtFunc_LinuxMemAreaTypeToString, __DtStep_4)
            return "LINUX_MEM_AREA_VMALLOC";
        case LINUX_MEM_AREA_SUB_ALLOC:
            __DtTestPoint(__DtFunc_LinuxMemAreaTypeToString, __DtStep_5)
            return "LINUX_MEM_AREA_SUB_ALLOC";
        case LINUX_MEM_AREA_ALLOC_PAGES:
            __DtTestPoint(__DtFunc_LinuxMemAreaTypeToString, __DtStep_6)
            return "LINUX_MEM_AREA_ALLOC_PAGES";
        default:
            __DtTestPoint(__DtFunc_LinuxMemAreaTypeToString, __DtStep_7)
            PVR_ASSERT(0);
    }

    __DtTestPoint(__DtFunc_LinuxMemAreaTypeToString, __DtStep_8)
    return "";
}


#if defined(DEBUG_LINUX_MEM_AREAS) || defined(DEBUG_LINUX_MEMORY_ALLOCATIONS)
static void ProcSeqStartstopDebugMutex(struct seq_file *sfile, IMG_BOOL start) 
{
	__DtTestPoint(__DtFunc_ProcSeqStartstopDebugMutex, __DtStep_0)
	if(start) 
	{
	    __DtTestPoint(__DtFunc_ProcSeqStartstopDebugMutex, __DtStep_1)
	    LinuxLockMutex(&g_sDebugMutex);		
	}
	else
	{
	    __DtTestPoint(__DtFunc_ProcSeqStartstopDebugMutex, __DtStep_2)
	    LinuxUnLockMutex(&g_sDebugMutex);
	}
	__DtTestPoint(__DtFunc_ProcSeqStartstopDebugMutex, __DtStep_3)
}
#endif 

#if defined(DEBUG_LINUX_MEM_AREAS)

static IMG_VOID* DecOffMemAreaRec_AnyVaCb(DEBUG_LINUX_MEM_AREA_REC *psNode, va_list va)
{
	off_t *pOff = va_arg(va, off_t*);
	__DtTestPoint(__DtFunc_DecOffMemAreaRec_AnyVaCb, __DtStep_0)
	if (--(*pOff))
	{
		__DtTestPoint(__DtFunc_DecOffMemAreaRec_AnyVaCb, __DtStep_1)
		return IMG_NULL;
	}
	else
	{
		__DtTestPoint(__DtFunc_DecOffMemAreaRec_AnyVaCb, __DtStep_2)
		return psNode;
	}
	__DtTestPoint(__DtFunc_DecOffMemAreaRec_AnyVaCb, __DtStep_3)
}

 
static void* ProcSeqNextMemArea(struct seq_file *sfile,void* el,loff_t off) 
{
    DEBUG_LINUX_MEM_AREA_REC *psRecord;
	__DtTestPoint(__DtFunc_ProcSeqNextMemArea, __DtStep_0)
	psRecord = (DEBUG_LINUX_MEM_AREA_REC*)
				List_DEBUG_LINUX_MEM_AREA_REC_Any_va(g_LinuxMemAreaRecords,
													DecOffMemAreaRec_AnyVaCb,
													&off);
	__DtTestPoint(__DtFunc_ProcSeqNextMemArea, __DtStep_1)
	return (void*)psRecord;
}

static void* ProcSeqOff2ElementMemArea(struct seq_file * sfile, loff_t off)
{
    DEBUG_LINUX_MEM_AREA_REC *psRecord;
	__DtTestPoint(__DtFunc_ProcSeqOff2ElementMemArea, __DtStep_0)
	if(!off) 
	{
		__DtTestPoint(__DtFunc_ProcSeqOff2ElementMemArea, __DtStep_1)
		return PVR_PROC_SEQ_START_TOKEN;
	}

	psRecord = (DEBUG_LINUX_MEM_AREA_REC*)
				List_DEBUG_LINUX_MEM_AREA_REC_Any_va(g_LinuxMemAreaRecords,
													DecOffMemAreaRec_AnyVaCb,
													&off);
	__DtTestPoint(__DtFunc_ProcSeqOff2ElementMemArea, __DtStep_2)
	return (void*)psRecord;
}


static void ProcSeqShowMemArea(struct seq_file *sfile,void* el)
{
    DEBUG_LINUX_MEM_AREA_REC *psRecord = (DEBUG_LINUX_MEM_AREA_REC*)el; 
	__DtTestPoint(__DtFunc_ProcSeqShowMemArea, __DtStep_0)
	if(el == PVR_PROC_SEQ_START_TOKEN) 
	{

#if !defined(DEBUG_LINUX_XML_PROC_FILES)
        seq_printf( sfile,
              			  "Number of Linux Memory Areas: %u\n"
                          "At the current water mark these areas correspond to %u bytes (excluding SUB areas)\n"
                          "At the highest water mark these areas corresponded to %u bytes (excluding SUB areas)\n"
                          "\nDetails for all Linux Memory Areas:\n"
                          "%s %-24s %s %s %-8s %-5s %s\n",
                          g_LinuxMemAreaCount,
                          g_LinuxMemAreaWaterMark,
                          g_LinuxMemAreaHighWaterMark,
                          "psLinuxMemArea",
                          "LinuxMemType",
                          "CpuVAddr",
                          "CpuPAddr",
                          "Bytes",
                          "Pid",
                          "Flags"
                         );
#else
        seq_printf( sfile,
                          "<mem_areas_header>\n"
                          "\t<count>%u</count>\n"
                          "\t<watermark key=\"mar0\" description=\"current\" bytes=\"%u\"/>\n" 
                          "\t<watermark key=\"mar1\" description=\"high\" bytes=\"%u\"/>\n" 
                          "</mem_areas_header>\n",
                          g_LinuxMemAreaCount,
                          g_LinuxMemAreaWaterMark,
                          g_LinuxMemAreaHighWaterMark
                         );
#endif
		return;
	}

        seq_printf( sfile,
#if !defined(DEBUG_LINUX_XML_PROC_FILES)
                       "%8p       %-24s %8p %08x %-8d %-5u %08x=(%s)\n",
#else
                       "<linux_mem_area>\n"
                       "\t<pointer>%8p</pointer>\n"
                       "\t<type>%s</type>\n"
                       "\t<cpu_virtual>%8p</cpu_virtual>\n"
                       "\t<cpu_physical>%08x</cpu_physical>\n"
                       "\t<bytes>%ld</bytes>\n"
                       "\t<pid>%u</pid>\n"
                       "\t<flags>%08lx</flags>\n"
                       "\t<flags_string>%s</flags_string>\n"
                       "</linux_mem_area>\n",
#endif
                       psRecord->psLinuxMemArea,
                       LinuxMemAreaTypeToString(psRecord->psLinuxMemArea->eAreaType),
                       LinuxMemAreaToCpuVAddr(psRecord->psLinuxMemArea),
                       LinuxMemAreaToCpuPAddr(psRecord->psLinuxMemArea,0).uiAddr,
                       psRecord->psLinuxMemArea->ui32ByteSize,
                       psRecord->pid,
                       psRecord->ui32Flags,
                       HAPFlagsToString(psRecord->ui32Flags)
                      );

	__DtTestPoint(__DtFunc_ProcSeqShowMemArea, __DtStep_1)
}

#endif 


#if defined(DEBUG_LINUX_MEMORY_ALLOCATIONS)

static IMG_VOID* DecOffMemAllocRec_AnyVaCb(DEBUG_MEM_ALLOC_REC *psNode, va_list va)
{
	off_t *pOff = va_arg(va, off_t*);
	__DtTestPoint(__DtFunc_DecOffMemAllocRec_AnyVaCb, __DtStep_0)
	if (--(*pOff))
	{
		__DtTestPoint(__DtFunc_DecOffMemAllocRec_AnyVaCb, __DtStep_1)
		return IMG_NULL;
	}
	else
	{
		__DtTestPoint(__DtFunc_DecOffMemAllocRec_AnyVaCb, __DtStep_2)
		return psNode;
	}
	__DtTestPoint(__DtFunc_DecOffMemAllocRec_AnyVaCb, __DtStep_3)
}


 
static void* ProcSeqNextMemoryRecords(struct seq_file *sfile,void* el,loff_t off) 
{
    DEBUG_MEM_ALLOC_REC *psRecord;
	__DtTestPoint(__DtFunc_ProcSeqNextMemoryRecords, __DtStep_0)
	psRecord = (DEBUG_MEM_ALLOC_REC*)
		List_DEBUG_MEM_ALLOC_REC_Any_va(g_MemoryRecords,
										DecOffMemAllocRec_AnyVaCb,
										&off);
#if defined(DEBUG_LINUX_XML_PROC_FILES)
	if(!psRecord) 
	{
		__DtTestPoint(__DtFunc_ProcSeqNextMemoryRecords, __DtStep_1)
		seq_printf( sfile, "</meminfo>\n");
	}
#endif

	return (void*)psRecord;
}

static void* ProcSeqOff2ElementMemoryRecords(struct seq_file *sfile, loff_t off)
{
    DEBUG_MEM_ALLOC_REC *psRecord;
	__DtTestPoint(__DtFunc_ProcSeqOff2ElementMemoryRecords, __DtStep_0)
	if(!off) 
	{
		__DtTestPoint(__DtFunc_ProcSeqOff2ElementMemoryRecords, __DtStep_1)
		return PVR_PROC_SEQ_START_TOKEN;
	}

	psRecord = (DEBUG_MEM_ALLOC_REC*)
		List_DEBUG_MEM_ALLOC_REC_Any_va(g_MemoryRecords,
										DecOffMemAllocRec_AnyVaCb,
										&off);

#if defined(DEBUG_LINUX_XML_PROC_FILES)
	if(!psRecord) 
	{
		__DtTestPoint(__DtFunc_ProcSeqOff2ElementMemoryRecords, __DtStep_2)
		seq_printf( sfile, "</meminfo>\n");
	}
#endif

	return (void*)psRecord;
}

static void ProcSeqShowMemoryRecords(struct seq_file *sfile,void* el)
{
    DEBUG_MEM_ALLOC_REC *psRecord = (DEBUG_MEM_ALLOC_REC*)el;
	__DtTestPoint(__DtFunc_ProcSeqShowMemoryRecords, __DtStep_0)
	if(el == PVR_PROC_SEQ_START_TOKEN) 
	{
#if !defined(DEBUG_LINUX_XML_PROC_FILES)
        
        seq_printf( sfile, "%-60s: %d bytes\n",
                           "Current Water Mark of bytes allocated via kmalloc",
                           g_WaterMarkData[DEBUG_MEM_ALLOC_TYPE_KMALLOC]);
        seq_printf( sfile, "%-60s: %d bytes\n",
                           "Highest Water Mark of bytes allocated via kmalloc",
                           g_HighWaterMarkData[DEBUG_MEM_ALLOC_TYPE_KMALLOC]);
        seq_printf( sfile, "%-60s: %d bytes\n",
                           "Current Water Mark of bytes allocated via vmalloc",
                           g_WaterMarkData[DEBUG_MEM_ALLOC_TYPE_VMALLOC]);
        seq_printf( sfile, "%-60s: %d bytes\n",
                           "Highest Water Mark of bytes allocated via vmalloc",
                           g_HighWaterMarkData[DEBUG_MEM_ALLOC_TYPE_VMALLOC]);
        seq_printf( sfile, "%-60s: %d bytes\n",
                           "Current Water Mark of bytes allocated via alloc_pages",
                           g_WaterMarkData[DEBUG_MEM_ALLOC_TYPE_ALLOC_PAGES]);
        seq_printf( sfile, "%-60s: %d bytes\n",
                           "Highest Water Mark of bytes allocated via alloc_pages",
                           g_HighWaterMarkData[DEBUG_MEM_ALLOC_TYPE_ALLOC_PAGES]);
        seq_printf( sfile, "%-60s: %d bytes\n",
                           "Current Water Mark of bytes allocated via ioremap",
                           g_WaterMarkData[DEBUG_MEM_ALLOC_TYPE_IOREMAP]);
        seq_printf( sfile, "%-60s: %d bytes\n",
                           "Highest Water Mark of bytes allocated via ioremap",
                           g_HighWaterMarkData[DEBUG_MEM_ALLOC_TYPE_IOREMAP]);
        seq_printf( sfile, "%-60s: %d bytes\n",
                           "Current Water Mark of bytes reserved for \"IO\" memory areas",
                           g_WaterMarkData[DEBUG_MEM_ALLOC_TYPE_IO]);
        seq_printf( sfile, "%-60s: %d bytes\n",
                           "Highest Water Mark of bytes allocated for \"IO\" memory areas",
                           g_HighWaterMarkData[DEBUG_MEM_ALLOC_TYPE_IO]);
        seq_printf( sfile, "%-60s: %d bytes\n",
                           "Current Water Mark of bytes allocated via kmem_cache_alloc",
                           g_WaterMarkData[DEBUG_MEM_ALLOC_TYPE_KMEM_CACHE]);
        seq_printf( sfile, "%-60s: %d bytes\n",
                           "Highest Water Mark of bytes allocated via kmem_cache_alloc",
                           g_HighWaterMarkData[DEBUG_MEM_ALLOC_TYPE_KMEM_CACHE]);
        seq_printf( sfile, "\n");

        seq_printf( sfile, "%-60s: %d bytes\n",
                           "The Current Water Mark for memory allocated from system RAM",
                           g_SysRAMWaterMark);
        seq_printf( sfile, "%-60s: %d bytes\n",
                           "The Highest Water Mark for memory allocated from system RAM",
                           g_SysRAMHighWaterMark);
        seq_printf( sfile, "%-60s: %d bytes\n",
                           "The Current Water Mark for memory allocated from IO memory",
                           g_IOMemWaterMark);
        seq_printf( sfile, "%-60s: %d bytes\n",
                           "The Highest Water Mark for memory allocated from IO memory",
                           g_IOMemHighWaterMark);

        seq_printf( sfile, "\n");

		seq_printf( sfile, "Details for all known allocations:\n"
                           "%-16s %-8s %-8s %-10s %-5s %-10s %s\n",
                           "Type",
                           "CpuVAddr",
                           "CpuPAddr",
                           "Bytes",
                           "PID",
                           "PrivateData",
                           "Filename:Line");

#else 
		
		
		seq_printf( sfile, "<meminfo>\n<meminfo_header>\n");
		seq_printf( sfile,
                           "<watermark key=\"mr0\" description=\"kmalloc_current\" bytes=\"%d\"/>\n",
                           g_WaterMarkData[DEBUG_MEM_ALLOC_TYPE_KMALLOC]);
		seq_printf( sfile,
                           "<watermark key=\"mr1\" description=\"kmalloc_high\" bytes=\"%d\"/>\n",
                           g_HighWaterMarkData[DEBUG_MEM_ALLOC_TYPE_KMALLOC]);
		seq_printf( sfile,
                           "<watermark key=\"mr2\" description=\"vmalloc_current\" bytes=\"%d\"/>\n",
                           g_WaterMarkData[DEBUG_MEM_ALLOC_TYPE_VMALLOC]);
		seq_printf( sfile,
                           "<watermark key=\"mr3\" description=\"vmalloc_high\" bytes=\"%d\"/>\n",
                           g_HighWaterMarkData[DEBUG_MEM_ALLOC_TYPE_VMALLOC]);
		seq_printf( sfile,
                           "<watermark key=\"mr4\" description=\"alloc_pages_current\" bytes=\"%d\"/>\n",
                           g_WaterMarkData[DEBUG_MEM_ALLOC_TYPE_ALLOC_PAGES]);
		seq_printf( sfile,
                           "<watermark key=\"mr5\" description=\"alloc_pages_high\" bytes=\"%d\"/>\n",
                           g_HighWaterMarkData[DEBUG_MEM_ALLOC_TYPE_ALLOC_PAGES]);
		seq_printf( sfile,
                           "<watermark key=\"mr6\" description=\"ioremap_current\" bytes=\"%d\"/>\n",
                           g_WaterMarkData[DEBUG_MEM_ALLOC_TYPE_IOREMAP]);
		seq_printf( sfile,
                           "<watermark key=\"mr7\" description=\"ioremap_high\" bytes=\"%d\"/>\n",
                           g_HighWaterMarkData[DEBUG_MEM_ALLOC_TYPE_IOREMAP]);
		seq_printf( sfile,
                           "<watermark key=\"mr8\" description=\"io_current\" bytes=\"%d\"/>\n",
                           g_WaterMarkData[DEBUG_MEM_ALLOC_TYPE_IO]);
		seq_printf( sfile,
                           "<watermark key=\"mr9\" description=\"io_high\" bytes=\"%d\"/>\n",
                           g_HighWaterMarkData[DEBUG_MEM_ALLOC_TYPE_IO]);
		seq_printf( sfile,
                           "<watermark key=\"mr10\" description=\"kmem_cache_current\" bytes=\"%d\"/>\n",
                           g_WaterMarkData[DEBUG_MEM_ALLOC_TYPE_KMEM_CACHE]);
		seq_printf( sfile,
                           "<watermark key=\"mr11\" description=\"kmem_cache_high\" bytes=\"%d\"/>\n",
                           g_HighWaterMarkData[DEBUG_MEM_ALLOC_TYPE_KMEM_CACHE]);
		seq_printf( sfile,"\n" );

		seq_printf( sfile,
                           "<watermark key=\"mr14\" description=\"system_ram_current\" bytes=\"%d\"/>\n",
                           g_SysRAMWaterMark);
		seq_printf( sfile,
                           "<watermark key=\"mr15\" description=\"system_ram_high\" bytes=\"%d\"/>\n",
                           g_SysRAMHighWaterMark);
		seq_printf( sfile,
                           "<watermark key=\"mr16\" description=\"system_io_current\" bytes=\"%d\"/>\n",
                           g_IOMemWaterMark);
		seq_printf( sfile,
                           "<watermark key=\"mr17\" description=\"system_io_high\" bytes=\"%d\"/>\n",
                           g_IOMemHighWaterMark);

		seq_printf( sfile, "</meminfo_header>\n");

#endif 
		return;
	}

    if(psRecord->eAllocType != DEBUG_MEM_ALLOC_TYPE_KMEM_CACHE)
    {
		__DtTestPoint(__DtFunc_ProcSeqShowMemoryRecords, __DtStep_1)
		seq_printf( sfile,
#if !defined(DEBUG_LINUX_XML_PROC_FILES)
                           "%-16s %-8p %08x %-10d %-5d %-10s %s:%d\n",
#else
                           "<allocation>\n"
                           "\t<type>%s</type>\n"
                           "\t<cpu_virtual>%-8p</cpu_virtual>\n"
                           "\t<cpu_physical>%08x</cpu_physical>\n"
                           "\t<bytes>%d</bytes>\n"
                           "\t<pid>%d</pid>\n"
                           "\t<private>%s</private>\n"
                           "\t<filename>%s</filename>\n"
                           "\t<line>%d</line>\n"
                           "</allocation>\n",
#endif
                           DebugMemAllocRecordTypeToString(psRecord->eAllocType),
                           psRecord->pvCpuVAddr,
                           psRecord->ulCpuPAddr,
                           psRecord->ui32Bytes,
                           psRecord->pid,
                           "NULL",
                           psRecord->pszFileName,
                           psRecord->ui32Line);
    }
    else
    {
		__DtTestPoint(__DtFunc_ProcSeqShowMemoryRecords, __DtStep_2)
		seq_printf( sfile,
#if !defined(DEBUG_LINUX_XML_PROC_FILES)
                           "%-16s %-8p %08x %-10d %-5d %-10s %s:%d\n",
#else
                           "<allocation>\n"
                           "\t<type>%s</type>\n"
                           "\t<cpu_virtual>%-8p</cpu_virtual>\n"
                           "\t<cpu_physical>%08x</cpu_physical>\n"
                           "\t<bytes>%d</bytes>\n"
                           "\t<pid>%d</pid>\n"
                           "\t<private>%s</private>\n"
                           "\t<filename>%s</filename>\n"
                           "\t<line>%d</line>\n"
                           "</allocation>\n",
#endif
                           DebugMemAllocRecordTypeToString(psRecord->eAllocType),
                           psRecord->pvCpuVAddr,
                           psRecord->ulCpuPAddr,
                           psRecord->ui32Bytes,
                           psRecord->pid,
                           KMemCacheNameWrapper(psRecord->pvPrivateData),
                           psRecord->pszFileName,
                           psRecord->ui32Line);
    }
	__DtTestPoint(__DtFunc_ProcSeqShowMemoryRecords, __DtStep_3)
}

#endif 


#if defined(DEBUG_LINUX_MEM_AREAS) || defined(DEBUG_LINUX_MMAP_AREAS)
const IMG_CHAR *
HAPFlagsToString(IMG_UINT32 ui32Flags)
{
    static IMG_CHAR szFlags[50];
    IMG_INT32 i32Pos = 0;
    IMG_UINT32 ui32CacheTypeIndex, ui32MapTypeIndex;
    IMG_CHAR *apszCacheTypes[] = {
        "UNCACHED",
        "CACHED",
        "WRITECOMBINE",
        "UNKNOWN"
    };
    IMG_CHAR *apszMapType[] = {
        "KERNEL_ONLY",
        "SINGLE_PROCESS",
        "MULTI_PROCESS",
        "FROM_EXISTING_PROCESS",
        "NO_CPU_VIRTUAL",
        "UNKNOWN"
    };
    
    
    __DtTestPoint(__DtFunc_HAPFlagsToString, __DtStep_0)
    if(ui32Flags & PVRSRV_HAP_UNCACHED){
        __DtTestPoint(__DtFunc_HAPFlagsToString, __DtStep_1)
        ui32CacheTypeIndex=0;
    }else if(ui32Flags & PVRSRV_HAP_CACHED){
        __DtTestPoint(__DtFunc_HAPFlagsToString, __DtStep_2)
        ui32CacheTypeIndex=1;
    }else if(ui32Flags & PVRSRV_HAP_WRITECOMBINE){
        __DtTestPoint(__DtFunc_HAPFlagsToString, __DtStep_3)
        ui32CacheTypeIndex=2;
    }else{
        __DtTestPoint(__DtFunc_HAPFlagsToString, __DtStep_4)
        ui32CacheTypeIndex=3;
        PVR_DPF((PVR_DBG_ERROR, "%s: unknown cache type (%u)",
                 __FUNCTION__, (ui32Flags & PVRSRV_HAP_CACHETYPE_MASK)));
    }

    
    if(ui32Flags & PVRSRV_HAP_KERNEL_ONLY){
        __DtTestPoint(__DtFunc_HAPFlagsToString, __DtStep_5)
        ui32MapTypeIndex = 0;
    }else if(ui32Flags & PVRSRV_HAP_SINGLE_PROCESS){
        __DtTestPoint(__DtFunc_HAPFlagsToString, __DtStep_6)
        ui32MapTypeIndex = 1;
    }else if(ui32Flags & PVRSRV_HAP_MULTI_PROCESS){
        __DtTestPoint(__DtFunc_HAPFlagsToString, __DtStep_7)
        ui32MapTypeIndex = 2;
    }else if(ui32Flags & PVRSRV_HAP_FROM_EXISTING_PROCESS){
        __DtTestPoint(__DtFunc_HAPFlagsToString, __DtStep_8)
        ui32MapTypeIndex = 3;
    }else if(ui32Flags & PVRSRV_HAP_NO_CPU_VIRTUAL){
        __DtTestPoint(__DtFunc_HAPFlagsToString, __DtStep_9)
        ui32MapTypeIndex = 4;
    }else{
        __DtTestPoint(__DtFunc_HAPFlagsToString, __DtStep_10)
        ui32MapTypeIndex = 5;
        PVR_DPF((PVR_DBG_ERROR, "%s: unknown map type (%u)",
                 __FUNCTION__, (ui32Flags & PVRSRV_HAP_MAPTYPE_MASK)));
    }

    i32Pos = sprintf(szFlags, "%s|", apszCacheTypes[ui32CacheTypeIndex]);
    if (i32Pos <= 0)
    {
	__DtTestPoint(__DtFunc_HAPFlagsToString, __DtStep_11)
	PVR_DPF((PVR_DBG_ERROR, "%s: sprintf for cache type %u failed (%d)",
		__FUNCTION__, ui32CacheTypeIndex, i32Pos));
	szFlags[0] = 0;
    }
    else
    {
        __DtTestPoint(__DtFunc_HAPFlagsToString, __DtStep_12)
        sprintf(szFlags + i32Pos, "%s", apszMapType[ui32MapTypeIndex]);
    }

    __DtTestPoint(__DtFunc_HAPFlagsToString, __DtStep_13)
    return szFlags;
}
#endif

