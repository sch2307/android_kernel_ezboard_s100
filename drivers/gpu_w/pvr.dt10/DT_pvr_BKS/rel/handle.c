#include	"DT_handle.h"	/* For DynamicTracer-TestPoint */
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

#ifdef	PVR_SECURE_HANDLES
#include <stddef.h>

#include "services_headers.h"
#include "handle.h"

#ifdef	DEBUG
#define	HANDLE_BLOCK_SHIFT	2
#else
#define	HANDLE_BLOCK_SHIFT	8
#endif

#define	DIVIDE_BY_BLOCK_SIZE(i)		(((IMG_UINT32)(i)) >> HANDLE_BLOCK_SHIFT)
#define	MULTIPLY_BY_BLOCK_SIZE(i)	(((IMG_UINT32)(i)) << HANDLE_BLOCK_SHIFT)

#define HANDLE_BLOCK_SIZE       MULTIPLY_BY_BLOCK_SIZE(1)
#define	HANDLE_SUB_BLOCK_MASK	(HANDLE_BLOCK_SIZE - 1)
#define	HANDLE_BLOCK_MASK	(~(HANDLE_SUB_BLOCK_MASK))

#define	HANDLE_HASH_TAB_INIT_SIZE	32

#define	INDEX_IS_VALID(psBase, i) ((i) < (psBase)->ui32TotalHandCount)

#define	INDEX_TO_HANDLE(i) ((IMG_HANDLE)((i) + 1))
#define	HANDLE_TO_INDEX(h) ((IMG_UINT32)(h) - 1)

#define	INDEX_TO_BLOCK_INDEX(i)		DIVIDE_BY_BLOCK_SIZE(i)
#define BLOCK_INDEX_TO_INDEX(i)		MULTIPLY_BY_BLOCK_SIZE(i)
#define INDEX_TO_SUB_BLOCK_INDEX(i)	((i) & HANDLE_SUB_BLOCK_MASK)

#define INDEX_TO_INDEX_STRUCT_PTR(psArray, i) (&((psArray)[INDEX_TO_BLOCK_INDEX(i)]))
#define	BASE_AND_INDEX_TO_INDEX_STRUCT_PTR(psBase, i) INDEX_TO_INDEX_STRUCT_PTR((psBase)->psHandleArray, i)

#define	INDEX_TO_FREE_HAND_BLOCK_COUNT(psBase, i) (BASE_AND_INDEX_TO_INDEX_STRUCT_PTR(psBase, i)->ui32FreeHandBlockCount)

#define INDEX_TO_HANDLE_STRUCT_PTR(psBase, i) (BASE_AND_INDEX_TO_INDEX_STRUCT_PTR(psBase, i)->psHandle + INDEX_TO_SUB_BLOCK_INDEX(i))

#define	HANDLE_TO_HANDLE_STRUCT_PTR(psBase, h) (INDEX_TO_HANDLE_STRUCT_PTR(psBase, HANDLE_TO_INDEX(h)))

#define	HANDLE_PTR_TO_INDEX(psHandle) ((psHandle)->ui32Index)
#define	HANDLE_PTR_TO_HANDLE(psHandle) INDEX_TO_HANDLE(HANDLE_PTR_TO_INDEX(psHandle))

#define	ROUND_DOWN_TO_MULTIPLE_OF_BLOCK_SIZE(a) (HANDLE_BLOCK_MASK & (a))
#define	ROUND_UP_TO_MULTIPLE_OF_BLOCK_SIZE(a) ROUND_DOWN_TO_MULTIPLE_OF_BLOCK_SIZE((a) + HANDLE_BLOCK_SIZE - 1)

#define	DEFAULT_MAX_HANDLE		0x7fffffffu
#define	DEFAULT_MAX_INDEX_PLUS_ONE	ROUND_DOWN_TO_MULTIPLE_OF_BLOCK_SIZE(DEFAULT_MAX_HANDLE)

#define	HANDLES_BATCHED(psBase) ((psBase)->ui32HandBatchSize != 0)

#define HANDLE_ARRAY_SIZE(handleCount) DIVIDE_BY_BLOCK_SIZE(ROUND_UP_TO_MULTIPLE_OF_BLOCK_SIZE(handleCount))

#define	SET_FLAG(v, f) ((IMG_VOID)((v) |= (f)))
#define	CLEAR_FLAG(v, f) ((IMG_VOID)((v) &= ~(f)))
#define	TEST_FLAG(v, f) ((IMG_BOOL)(((v) & (f)) != 0))

#define	TEST_ALLOC_FLAG(psHandle, f) TEST_FLAG((psHandle)->eFlag, f)

#define	SET_INTERNAL_FLAG(psHandle, f) SET_FLAG((psHandle)->eInternalFlag, f)
#define	CLEAR_INTERNAL_FLAG(psHandle, f) CLEAR_FLAG((psHandle)->eInternalFlag, f)
#define	TEST_INTERNAL_FLAG(psHandle, f) TEST_FLAG((psHandle)->eInternalFlag, f)

#define	BATCHED_HANDLE(psHandle) TEST_INTERNAL_FLAG(psHandle, INTERNAL_HANDLE_FLAG_BATCHED)

#define	SET_BATCHED_HANDLE(psHandle) SET_INTERNAL_FLAG(psHandle, INTERNAL_HANDLE_FLAG_BATCHED)

#define	SET_UNBATCHED_HANDLE(psHandle) CLEAR_INTERNAL_FLAG(psHandle, INTERNAL_HANDLE_FLAG_BATCHED)

#define	BATCHED_HANDLE_PARTIALLY_FREE(psHandle) TEST_INTERNAL_FLAG(psHandle, INTERNAL_HANDLE_FLAG_BATCHED_PARTIALLY_FREE)

#define SET_BATCHED_HANDLE_PARTIALLY_FREE(psHandle) SET_INTERNAL_FLAG(psHandle, INTERNAL_HANDLE_FLAG_BATCHED_PARTIALLY_FREE)

#define	HANDLE_STRUCT_IS_FREE(psHandle) ((psHandle)->eType == PVRSRV_HANDLE_TYPE_NONE && (psHandle)->eInternalFlag == INTERNAL_HANDLE_FLAG_NONE)

#ifdef	MIN
#undef MIN
#endif

#define	MIN(x, y) (((x) < (y)) ? (x) : (y))

struct sHandleList
{
	IMG_UINT32 ui32Prev;
	IMG_UINT32 ui32Next;
	IMG_HANDLE hParent;
};

enum ePVRSRVInternalHandleFlag
{
	INTERNAL_HANDLE_FLAG_NONE = 0x00,
	INTERNAL_HANDLE_FLAG_BATCHED = 0x01,
	INTERNAL_HANDLE_FLAG_BATCHED_PARTIALLY_FREE = 0x02,
};

struct sHandle
{
	
	PVRSRV_HANDLE_TYPE eType;

	
	IMG_VOID *pvData;

	
	IMG_UINT32 ui32NextIndexPlusOne;

	
	enum ePVRSRVInternalHandleFlag eInternalFlag;

	
	PVRSRV_HANDLE_ALLOC_FLAG eFlag;

	
	IMG_UINT32 ui32Index;

	
	struct sHandleList sChildren;

	
	struct sHandleList sSiblings;
};

struct sHandleIndex
{
	
	struct sHandle *psHandle;

	
	IMG_HANDLE hBlockAlloc;

	
	IMG_UINT32 ui32FreeHandBlockCount;
};

struct _PVRSRV_HANDLE_BASE_
{
	
	IMG_HANDLE hBaseBlockAlloc;

	
	IMG_HANDLE hArrayBlockAlloc;

	
	struct sHandleIndex *psHandleArray;

	
	HASH_TABLE *psHashTab;

	
	IMG_UINT32 ui32FreeHandCount;

	
	IMG_UINT32 ui32FirstFreeIndex;

	
	IMG_UINT32 ui32MaxIndexPlusOne;

	
	IMG_UINT32 ui32TotalHandCount;

	
	IMG_UINT32 ui32LastFreeIndexPlusOne;

	
	IMG_UINT32 ui32HandBatchSize;

	
	IMG_UINT32 ui32TotalHandCountPreBatch;

	
	IMG_UINT32 ui32FirstBatchIndexPlusOne;

	
	IMG_UINT32 ui32BatchHandAllocFailures;

	
	IMG_BOOL bPurgingEnabled;
};

enum eHandKey {
	HAND_KEY_DATA = 0,
	HAND_KEY_TYPE,
	HAND_KEY_PARENT,
	HAND_KEY_LEN			
};

PVRSRV_HANDLE_BASE *gpsKernelHandleBase = IMG_NULL;

typedef IMG_UINTPTR_T HAND_KEY[HAND_KEY_LEN];

#ifdef INLINE_IS_PRAGMA
#pragma inline(HandleListInit)
#endif
static INLINE
IMG_VOID HandleListInit(IMG_UINT32 ui32Index, struct sHandleList *psList, IMG_HANDLE hParent)
{
	__DtTestPoint(__DtFunc_HandleListInit, __DtStep_0)
	psList->ui32Next = ui32Index;
	psList->ui32Prev = ui32Index;
	psList->hParent = hParent;
	__DtTestPoint(__DtFunc_HandleListInit, __DtStep_1)
}

#ifdef INLINE_IS_PRAGMA
#pragma inline(InitParentList)
#endif
static INLINE
IMG_VOID InitParentList(struct sHandle *psHandle)
{
	IMG_UINT32 ui32Parent = HANDLE_PTR_TO_INDEX(psHandle);

	__DtTestPoint(__DtFunc_InitParentList, __DtStep_0)
	HandleListInit(ui32Parent, &psHandle->sChildren, INDEX_TO_HANDLE(ui32Parent));
	__DtTestPoint(__DtFunc_InitParentList, __DtStep_1)
}

#ifdef INLINE_IS_PRAGMA
#pragma inline(InitChildEntry)
#endif
static INLINE
IMG_VOID InitChildEntry(struct sHandle *psHandle)
{
	__DtTestPoint(__DtFunc_InitChildEntry, __DtStep_0)
	HandleListInit(HANDLE_PTR_TO_INDEX(psHandle), &psHandle->sSiblings, IMG_NULL);
	__DtTestPoint(__DtFunc_InitChildEntry, __DtStep_1)
}

#ifdef INLINE_IS_PRAGMA
#pragma inline(HandleListIsEmpty)
#endif
static INLINE
IMG_BOOL HandleListIsEmpty(IMG_UINT32 ui32Index, struct sHandleList *psList)
{
	IMG_BOOL bIsEmpty;

	__DtTestPoint(__DtFunc_HandleListIsEmpty, __DtStep_0)
	bIsEmpty = (IMG_BOOL)(psList->ui32Next == ui32Index);

#ifdef	DEBUG
	{
		IMG_BOOL bIsEmpty2;

		bIsEmpty2 = (IMG_BOOL)(psList->ui32Prev == ui32Index);
		PVR_ASSERT(bIsEmpty == bIsEmpty2);
	}
#endif

	return bIsEmpty;
}

#ifdef DEBUG
#ifdef INLINE_IS_PRAGMA
#pragma inline(NoChildren)
#endif
static INLINE
IMG_BOOL NoChildren(struct sHandle *psHandle)
{
	__DtTestPoint(__DtFunc_NoChildren, __DtStep_0)
	PVR_ASSERT(psHandle->sChildren.hParent == HANDLE_PTR_TO_HANDLE(psHandle));

	__DtTestPoint(__DtFunc_NoChildren, __DtStep_1)
	return HandleListIsEmpty(HANDLE_PTR_TO_INDEX(psHandle), &psHandle->sChildren);
}

#ifdef INLINE_IS_PRAGMA
#pragma inline(NoParent)
#endif
static INLINE
IMG_BOOL NoParent(struct sHandle *psHandle)
{
	__DtTestPoint(__DtFunc_NoParent, __DtStep_0)
	if (HandleListIsEmpty(HANDLE_PTR_TO_INDEX(psHandle), &psHandle->sSiblings))
	{
		__DtTestPoint(__DtFunc_NoParent, __DtStep_1)
		PVR_ASSERT(psHandle->sSiblings.hParent == IMG_NULL);

		__DtTestPoint(__DtFunc_NoParent, __DtStep_2)
		return IMG_TRUE;
	}
	else
	{
		__DtTestPoint(__DtFunc_NoParent, __DtStep_3)
		PVR_ASSERT(psHandle->sSiblings.hParent != IMG_NULL);
	}
	__DtTestPoint(__DtFunc_NoParent, __DtStep_4)
	return IMG_FALSE;
}
#endif 
#ifdef INLINE_IS_PRAGMA
#pragma inline(ParentHandle)
#endif
static INLINE
IMG_HANDLE ParentHandle(struct sHandle *psHandle)
{
	__DtTestPoint(__DtFunc_ParentHandle, __DtStep_0)
	return psHandle->sSiblings.hParent;
}

#define	LIST_PTR_FROM_INDEX_AND_OFFSET(psBase, i, p, po, eo) \
		((struct sHandleList *)((IMG_CHAR *)(INDEX_TO_HANDLE_STRUCT_PTR(psBase, i)) + (((i) == (p)) ? (po) : (eo))))

#ifdef INLINE_IS_PRAGMA
#pragma inline(HandleListInsertBefore)
#endif
static INLINE
IMG_VOID HandleListInsertBefore(PVRSRV_HANDLE_BASE *psBase, IMG_UINT32 ui32InsIndex, struct sHandleList *psIns, IMG_SIZE_T uiParentOffset, IMG_UINT32 ui32EntryIndex, struct sHandleList *psEntry, IMG_SIZE_T uiEntryOffset, IMG_UINT32 ui32ParentIndex)
{
	 
	struct sHandleList *psPrevIns = LIST_PTR_FROM_INDEX_AND_OFFSET(psBase, psIns->ui32Prev, ui32ParentIndex, uiParentOffset, uiEntryOffset);

	__DtTestPoint(__DtFunc_HandleListInsertBefore, __DtStep_0)
	PVR_ASSERT(psEntry->hParent == IMG_NULL);
	PVR_ASSERT(ui32InsIndex == psPrevIns->ui32Next);
	PVR_ASSERT(LIST_PTR_FROM_INDEX_AND_OFFSET(psBase, ui32ParentIndex, ui32ParentIndex, uiParentOffset, uiParentOffset)->hParent == INDEX_TO_HANDLE(ui32ParentIndex));

	psEntry->ui32Prev = psIns->ui32Prev;
	psIns->ui32Prev = ui32EntryIndex;
	psEntry->ui32Next = ui32InsIndex;
	psPrevIns->ui32Next = ui32EntryIndex;

	psEntry->hParent = INDEX_TO_HANDLE(ui32ParentIndex);
	__DtTestPoint(__DtFunc_HandleListInsertBefore, __DtStep_1)
}

#ifdef INLINE_IS_PRAGMA
#pragma inline(AdoptChild)
#endif
static INLINE
IMG_VOID AdoptChild(PVRSRV_HANDLE_BASE *psBase, struct sHandle *psParent, struct sHandle *psChild)
{
	IMG_UINT32 ui32Parent = HANDLE_TO_INDEX(psParent->sChildren.hParent);

	__DtTestPoint(__DtFunc_AdoptChild, __DtStep_0)
	PVR_ASSERT(ui32Parent == HANDLE_PTR_TO_INDEX(psParent));

	HandleListInsertBefore(psBase, ui32Parent, &psParent->sChildren, offsetof(struct sHandle, sChildren), HANDLE_PTR_TO_INDEX(psChild), &psChild->sSiblings, offsetof(struct sHandle, sSiblings), ui32Parent);

	__DtTestPoint(__DtFunc_AdoptChild, __DtStep_1)
}

#ifdef INLINE_IS_PRAGMA
#pragma inline(HandleListRemove)
#endif
static INLINE
IMG_VOID HandleListRemove(PVRSRV_HANDLE_BASE *psBase, IMG_UINT32 ui32EntryIndex, struct sHandleList *psEntry, IMG_SIZE_T uiEntryOffset, IMG_SIZE_T uiParentOffset)
{
	__DtTestPoint(__DtFunc_HandleListRemove, __DtStep_0)
	if (!HandleListIsEmpty(ui32EntryIndex, psEntry))
	{
		 
		struct sHandleList *psPrev = LIST_PTR_FROM_INDEX_AND_OFFSET(psBase, psEntry->ui32Prev, HANDLE_TO_INDEX(psEntry->hParent), uiParentOffset, uiEntryOffset);
		struct sHandleList *psNext = LIST_PTR_FROM_INDEX_AND_OFFSET(psBase, psEntry->ui32Next, HANDLE_TO_INDEX(psEntry->hParent), uiParentOffset, uiEntryOffset);

		
		__DtTestPoint(__DtFunc_HandleListRemove, __DtStep_1)
		PVR_ASSERT(psEntry->hParent != IMG_NULL);

		psPrev->ui32Next = psEntry->ui32Next;
		psNext->ui32Prev = psEntry->ui32Prev;

		HandleListInit(ui32EntryIndex, psEntry, IMG_NULL);
	}
	__DtTestPoint(__DtFunc_HandleListRemove, __DtStep_2)
}

#ifdef INLINE_IS_PRAGMA
#pragma inline(UnlinkFromParent)
#endif
static INLINE
IMG_VOID UnlinkFromParent(PVRSRV_HANDLE_BASE *psBase, struct sHandle *psHandle)
{
	HandleListRemove(psBase, HANDLE_PTR_TO_INDEX(psHandle), &psHandle->sSiblings, offsetof(struct sHandle, sSiblings), offsetof(struct sHandle, sChildren));
	__DtTestPoint(__DtFunc_UnlinkFromParent, __DtStep_0)
}

#ifdef INLINE_IS_PRAGMA
#pragma inline(HandleListIterate)
#endif
static INLINE
PVRSRV_ERROR HandleListIterate(PVRSRV_HANDLE_BASE *psBase, struct sHandleList *psHead, IMG_SIZE_T uiParentOffset, IMG_SIZE_T uiEntryOffset, PVRSRV_ERROR (*pfnIterFunc)(PVRSRV_HANDLE_BASE *, struct sHandle *))
{
	IMG_UINT32 ui32Index;
	IMG_UINT32 ui32Parent = HANDLE_TO_INDEX(psHead->hParent);

	__DtTestPoint(__DtFunc_HandleListIterate, __DtStep_0)
	PVR_ASSERT(psHead->hParent != IMG_NULL);

	
	for(ui32Index = psHead->ui32Next; ui32Index != ui32Parent; )
	{
		struct sHandle *psHandle = INDEX_TO_HANDLE_STRUCT_PTR(psBase, ui32Index);
		 
		struct sHandleList *psEntry = LIST_PTR_FROM_INDEX_AND_OFFSET(psBase, ui32Index, ui32Parent, uiParentOffset, uiEntryOffset);
		PVRSRV_ERROR eError;

		__DtTestPoint(__DtFunc_HandleListIterate, __DtStep_1)
		PVR_ASSERT(psEntry->hParent == psHead->hParent);
		
		ui32Index = psEntry->ui32Next;

		eError = (*pfnIterFunc)(psBase, psHandle);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_HandleListIterate, __DtStep_2)
			return eError;
		}
	}

	__DtTestPoint(__DtFunc_HandleListIterate, __DtStep_3)
	return PVRSRV_OK;
}

#ifdef INLINE_IS_PRAGMA
#pragma inline(IterateOverChildren)
#endif
static INLINE
PVRSRV_ERROR IterateOverChildren(PVRSRV_HANDLE_BASE *psBase, struct sHandle *psParent, PVRSRV_ERROR (*pfnIterFunc)(PVRSRV_HANDLE_BASE *, struct sHandle *))
{
	 __DtTestPoint(__DtFunc_IterateOverChildren, __DtStep_0)
	 return HandleListIterate(psBase, &psParent->sChildren, offsetof(struct sHandle, sChildren), offsetof(struct sHandle, sSiblings), pfnIterFunc);
}

#ifdef INLINE_IS_PRAGMA
#pragma inline(GetHandleStructure)
#endif
static INLINE
PVRSRV_ERROR GetHandleStructure(PVRSRV_HANDLE_BASE *psBase, struct sHandle **ppsHandle, IMG_HANDLE hHandle, PVRSRV_HANDLE_TYPE eType)
{
	IMG_UINT32 ui32Index = HANDLE_TO_INDEX(hHandle);
	struct sHandle *psHandle;

	
	__DtTestPoint(__DtFunc_GetHandleStructure, __DtStep_0)
	if (!INDEX_IS_VALID(psBase, ui32Index))
	{
		__DtTestPoint(__DtFunc_GetHandleStructure, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "GetHandleStructure: Handle index out of range (%u >= %u)", ui32Index, psBase->ui32TotalHandCount));
		__DtTestPoint(__DtFunc_GetHandleStructure, __DtStep_2)
		return PVRSRV_ERROR_HANDLE_INDEX_OUT_OF_RANGE;
	}

	psHandle =  INDEX_TO_HANDLE_STRUCT_PTR(psBase, ui32Index);
	if (psHandle->eType == PVRSRV_HANDLE_TYPE_NONE)
	{
		__DtTestPoint(__DtFunc_GetHandleStructure, __DtStep_3)
		PVR_DPF((PVR_DBG_ERROR, "GetHandleStructure: Handle not allocated (index: %u)", ui32Index));
		__DtTestPoint(__DtFunc_GetHandleStructure, __DtStep_4)
		return PVRSRV_ERROR_HANDLE_NOT_ALLOCATED;
	}

	
	if (eType != PVRSRV_HANDLE_TYPE_NONE && eType != psHandle->eType)
	{
		__DtTestPoint(__DtFunc_GetHandleStructure, __DtStep_5)
		PVR_DPF((PVR_DBG_ERROR, "GetHandleStructure: Handle type mismatch (%d != %d)", eType, psHandle->eType));
		__DtTestPoint(__DtFunc_GetHandleStructure, __DtStep_6)
		return PVRSRV_ERROR_HANDLE_TYPE_MISMATCH;
	}

	
	*ppsHandle = psHandle;

	__DtTestPoint(__DtFunc_GetHandleStructure, __DtStep_7)
	return PVRSRV_OK;
}

#ifdef INLINE_IS_PRAGMA
#pragma inline(ParentIfPrivate)
#endif
static INLINE
IMG_HANDLE ParentIfPrivate(struct sHandle *psHandle)
{
	__DtTestPoint(__DtFunc_ParentIfPrivate, __DtStep_0)
	return TEST_ALLOC_FLAG(psHandle, PVRSRV_HANDLE_ALLOC_FLAG_PRIVATE) ?
			ParentHandle(psHandle) : IMG_NULL;
}

#ifdef INLINE_IS_PRAGMA
#pragma inline(InitKey)
#endif
static INLINE
IMG_VOID InitKey(HAND_KEY aKey, PVRSRV_HANDLE_BASE *psBase, IMG_VOID *pvData, PVRSRV_HANDLE_TYPE eType, IMG_HANDLE hParent)
{
	__DtTestPoint(__DtFunc_InitKey, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(psBase);

	aKey[HAND_KEY_DATA] = (IMG_UINTPTR_T)pvData;
	aKey[HAND_KEY_TYPE] = (IMG_UINTPTR_T)eType;
	aKey[HAND_KEY_PARENT] = (IMG_UINTPTR_T)hParent;
	__DtTestPoint(__DtFunc_InitKey, __DtStep_1)
}

static
PVRSRV_ERROR ReallocHandleArray(PVRSRV_HANDLE_BASE *psBase, IMG_UINT32 ui32NewCount)
{
	struct sHandleIndex *psOldArray = psBase->psHandleArray;
	IMG_HANDLE hOldArrayBlockAlloc = psBase->hArrayBlockAlloc;
	IMG_UINT32 ui32OldCount = psBase->ui32TotalHandCount;
	struct sHandleIndex *psNewArray = IMG_NULL;
	IMG_HANDLE hNewArrayBlockAlloc = IMG_NULL;
	PVRSRV_ERROR eError;
	PVRSRV_ERROR eReturn = PVRSRV_OK;
	IMG_UINT32 ui32Index;

	__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_0)
	if (ui32NewCount == ui32OldCount)
	{
		__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_1)
		return PVRSRV_OK;
	}

	if (ui32NewCount != 0 && !psBase->bPurgingEnabled &&
		 ui32NewCount < ui32OldCount)
	{
		__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_2)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	if (((ui32OldCount % HANDLE_BLOCK_SIZE) != 0) ||
		((ui32NewCount % HANDLE_BLOCK_SIZE) != 0))
	{
		__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_3)
		PVR_ASSERT((ui32OldCount % HANDLE_BLOCK_SIZE) == 0);
		PVR_ASSERT((ui32NewCount % HANDLE_BLOCK_SIZE) == 0);

		__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_4)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	if (ui32NewCount != 0)
	{
		
		__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_5)
		eError = OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP,
			HANDLE_ARRAY_SIZE(ui32NewCount) * sizeof(struct sHandleIndex),
			(IMG_VOID **)&psNewArray,
			&hNewArrayBlockAlloc,
			"Memory Area");
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_6)
			PVR_DPF((PVR_DBG_ERROR, "ReallocHandleArray: Couldn't allocate new handle array (%d)", eError));
			eReturn = eError;
			goto error;
		}

		if (ui32OldCount != 0)
		{
			__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_7)
			OSMemCopy(psNewArray, psOldArray, HANDLE_ARRAY_SIZE(MIN(ui32NewCount, ui32OldCount)) * sizeof(struct sHandleIndex));
		}
	}

	
	for(ui32Index = ui32NewCount; ui32Index < ui32OldCount; ui32Index += HANDLE_BLOCK_SIZE)
	{
		struct sHandleIndex *psIndex = INDEX_TO_INDEX_STRUCT_PTR(psOldArray, ui32Index);

		__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_8)
		eError = OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP,
				sizeof(struct sHandle) * HANDLE_BLOCK_SIZE,
				psIndex->psHandle,
				psIndex->hBlockAlloc);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_9)
			PVR_DPF((PVR_DBG_ERROR, "ReallocHandleArray: Couldn't free handle structures (%d)", eError));
		}
	}

	
	for(ui32Index = ui32OldCount; ui32Index < ui32NewCount; ui32Index += HANDLE_BLOCK_SIZE)
	{
		 
		struct sHandleIndex *psIndex = INDEX_TO_INDEX_STRUCT_PTR(psNewArray, ui32Index);

		__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_10)
		eError = OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP,
				sizeof(struct sHandle) * HANDLE_BLOCK_SIZE,
				(IMG_VOID **)&psIndex->psHandle,
				&psIndex->hBlockAlloc,
				"Memory Area");
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_11)
			psIndex->psHandle = IMG_NULL;
			PVR_DPF((PVR_DBG_ERROR, "ReallocHandleArray: Couldn't allocate handle structures (%d)", eError));
			eReturn = eError;
		}
		else
		{
			IMG_UINT32 ui32SubIndex;

			__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_12)
			psIndex->ui32FreeHandBlockCount = HANDLE_BLOCK_SIZE;

			for(ui32SubIndex = 0; ui32SubIndex < HANDLE_BLOCK_SIZE; ui32SubIndex++)
			{
				struct sHandle *psHandle = psIndex->psHandle + ui32SubIndex;


				__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_13)
				psHandle->ui32Index = ui32SubIndex + ui32Index;
				psHandle->eType = PVRSRV_HANDLE_TYPE_NONE;
				psHandle->eInternalFlag = INTERNAL_HANDLE_FLAG_NONE;
				psHandle->ui32NextIndexPlusOne  = 0;
			}
		}
	}
	if (eReturn != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_14)
		goto error;
	}

#ifdef	DEBUG_MAX_HANDLE_COUNT
	
	if (ui32NewCount > DEBUG_MAX_HANDLE_COUNT)
	{
		__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_15)
		PVR_DPF((PVR_DBG_ERROR, "ReallocHandleArray: Max handle count (%u) reached", DEBUG_MAX_HANDLE_COUNT));
		eReturn = PVRSRV_ERROR_OUT_OF_MEMORY;
		goto error;
	}
#endif

	if (psOldArray != IMG_NULL)
	{
		
		__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_16)
		eError = OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP,
			HANDLE_ARRAY_SIZE(ui32OldCount) * sizeof(struct sHandleIndex),
			psOldArray,
			hOldArrayBlockAlloc);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_17)
			PVR_DPF((PVR_DBG_ERROR, "ReallocHandleArray: Couldn't free old handle array (%d)", eError));
		}
	}

	psBase->psHandleArray = psNewArray;
	psBase->hArrayBlockAlloc = hNewArrayBlockAlloc;
	psBase->ui32TotalHandCount = ui32NewCount;

	if (ui32NewCount > ui32OldCount)
	{
		
		__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_18)
		PVR_ASSERT(psBase->ui32FreeHandCount + (ui32NewCount - ui32OldCount) > psBase->ui32FreeHandCount)

		 
		psBase->ui32FreeHandCount += (ui32NewCount - ui32OldCount);

		
		if (psBase->ui32FirstFreeIndex == 0)
		{
			__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_19)
			PVR_ASSERT(psBase->ui32LastFreeIndexPlusOne == 0)

			psBase->ui32FirstFreeIndex = ui32OldCount;
		}
		else
		{
			__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_20)
			if (!psBase->bPurgingEnabled)
			{
				__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_21)
				PVR_ASSERT(psBase->ui32LastFreeIndexPlusOne != 0)
				PVR_ASSERT(INDEX_TO_HANDLE_STRUCT_PTR(psBase, psBase->ui32LastFreeIndexPlusOne - 1)->ui32NextIndexPlusOne == 0)

				INDEX_TO_HANDLE_STRUCT_PTR(psBase, psBase->ui32LastFreeIndexPlusOne - 1)->ui32NextIndexPlusOne = ui32OldCount + 1;
			}
		}

		if (!psBase->bPurgingEnabled)
		{
			__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_22)
			psBase->ui32LastFreeIndexPlusOne = ui32NewCount;
		}
	}
	else
	{
		__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_23)
		PVR_ASSERT(ui32NewCount == 0 || psBase->bPurgingEnabled)
		PVR_ASSERT(ui32NewCount == 0 || psBase->ui32FirstFreeIndex <= ui32NewCount)
		PVR_ASSERT(psBase->ui32FreeHandCount - (ui32OldCount - ui32NewCount) < psBase->ui32FreeHandCount)

		 
		psBase->ui32FreeHandCount -= (ui32OldCount - ui32NewCount);

		if (ui32NewCount == 0)
		{
			__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_24)
			psBase->ui32FirstFreeIndex = 0;
			psBase->ui32LastFreeIndexPlusOne = 0;
		}
	}

	PVR_ASSERT(psBase->ui32FirstFreeIndex <= psBase->ui32TotalHandCount);

	__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_25)
	return PVRSRV_OK;

error:
	PVR_ASSERT(eReturn != PVRSRV_OK);

	if (psNewArray != IMG_NULL)
	{
		
		__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_26)
		for(ui32Index = ui32OldCount; ui32Index < ui32NewCount; ui32Index += HANDLE_BLOCK_SIZE)
		{
			struct sHandleIndex *psIndex = INDEX_TO_INDEX_STRUCT_PTR(psNewArray, ui32Index);
			__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_27)
			if (psIndex->psHandle != IMG_NULL)
			{
				__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_28)
				eError = OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP,
						sizeof(struct sHandle) * HANDLE_BLOCK_SIZE,
						psIndex->psHandle,
						psIndex->hBlockAlloc);
				if (eError != PVRSRV_OK)
				{
					__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_29)
					PVR_DPF((PVR_DBG_ERROR, "ReallocHandleArray: Couldn't free handle structures (%d)", eError));
				}
			}
		}

		
		eError = OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP,
			HANDLE_ARRAY_SIZE(ui32NewCount) * sizeof(struct sHandleIndex),
			psNewArray,
			hNewArrayBlockAlloc);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_30)
			PVR_DPF((PVR_DBG_ERROR, "ReallocHandleArray: Couldn't free new handle array (%d)", eError));
		}
	}

	__DtTestPoint(__DtFunc_ReallocHandleArray, __DtStep_31)
	return eReturn;
}

static PVRSRV_ERROR FreeHandleArray(PVRSRV_HANDLE_BASE *psBase)
{
	__DtTestPoint(__DtFunc_FreeHandleArray, __DtStep_0)
	return ReallocHandleArray(psBase, 0);
}

static PVRSRV_ERROR FreeHandle(PVRSRV_HANDLE_BASE *psBase, struct sHandle *psHandle)
{
	HAND_KEY aKey;
	IMG_UINT32 ui32Index = HANDLE_PTR_TO_INDEX(psHandle);
	PVRSRV_ERROR eError;

	
	__DtTestPoint(__DtFunc_FreeHandle, __DtStep_0)
	InitKey(aKey, psBase, psHandle->pvData, psHandle->eType, ParentIfPrivate(psHandle));

	if (!TEST_ALLOC_FLAG(psHandle, PVRSRV_HANDLE_ALLOC_FLAG_MULTI) && !BATCHED_HANDLE_PARTIALLY_FREE(psHandle))
	{
		IMG_HANDLE hHandle;
		__DtTestPoint(__DtFunc_FreeHandle, __DtStep_1)
		hHandle = (IMG_HANDLE) HASH_Remove_Extended(psBase->psHashTab, aKey);

		PVR_ASSERT(hHandle != IMG_NULL);
		PVR_ASSERT(hHandle == INDEX_TO_HANDLE(ui32Index));
		PVR_UNREFERENCED_PARAMETER(hHandle);
	}

	
	UnlinkFromParent(psBase, psHandle);

	
	eError = IterateOverChildren(psBase, psHandle, FreeHandle);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_FreeHandle, __DtStep_2)
		PVR_DPF((PVR_DBG_ERROR, "FreeHandle: Error whilst freeing subhandles (%d)", eError));
		__DtTestPoint(__DtFunc_FreeHandle, __DtStep_3)
		return eError;
	}

	
	psHandle->eType = PVRSRV_HANDLE_TYPE_NONE;

	if (BATCHED_HANDLE(psHandle) && !BATCHED_HANDLE_PARTIALLY_FREE(psHandle))
	{
		 
        __DtTestPoint(__DtFunc_FreeHandle, __DtStep_4)
        SET_BATCHED_HANDLE_PARTIALLY_FREE(psHandle);
		
		__DtTestPoint(__DtFunc_FreeHandle, __DtStep_5)
		return PVRSRV_OK;
	}

	
	if (!psBase->bPurgingEnabled)
	{
		__DtTestPoint(__DtFunc_FreeHandle, __DtStep_6)
		if (psBase->ui32FreeHandCount == 0)
		{
			__DtTestPoint(__DtFunc_FreeHandle, __DtStep_7)
			PVR_ASSERT(psBase->ui32FirstFreeIndex == 0);
			PVR_ASSERT(psBase->ui32LastFreeIndexPlusOne == 0);

			psBase->ui32FirstFreeIndex =  ui32Index;
		}
		else
		{
			
			__DtTestPoint(__DtFunc_FreeHandle, __DtStep_8)
			PVR_ASSERT(psBase->ui32LastFreeIndexPlusOne != 0);
			PVR_ASSERT(INDEX_TO_HANDLE_STRUCT_PTR(psBase, psBase->ui32LastFreeIndexPlusOne - 1)->ui32NextIndexPlusOne == 0);
			INDEX_TO_HANDLE_STRUCT_PTR(psBase, psBase->ui32LastFreeIndexPlusOne - 1)->ui32NextIndexPlusOne =  ui32Index + 1;
		}

		PVR_ASSERT(psHandle->ui32NextIndexPlusOne == 0);

		
		psBase->ui32LastFreeIndexPlusOne = ui32Index + 1;
	}

	psBase->ui32FreeHandCount++;
	INDEX_TO_FREE_HAND_BLOCK_COUNT(psBase, ui32Index)++;

	PVR_ASSERT(INDEX_TO_FREE_HAND_BLOCK_COUNT(psBase, ui32Index)<= HANDLE_BLOCK_SIZE);

#ifdef DEBUG
	{
		IMG_UINT32 ui32BlockedIndex;
		IMG_UINT32 ui32FreeHandCount = 0;

		for (ui32BlockedIndex = 0; ui32BlockedIndex < psBase->ui32TotalHandCount; ui32BlockedIndex += HANDLE_BLOCK_SIZE)
		{
			__DtTestPoint(__DtFunc_FreeHandle, __DtStep_9)
			ui32FreeHandCount += INDEX_TO_FREE_HAND_BLOCK_COUNT(psBase, ui32BlockedIndex);
		}

		PVR_ASSERT(ui32FreeHandCount == psBase->ui32FreeHandCount);
	}
#endif

	return PVRSRV_OK;
}

static PVRSRV_ERROR FreeAllHandles(PVRSRV_HANDLE_BASE *psBase)
{
	IMG_UINT32 i;
	PVRSRV_ERROR eError = PVRSRV_OK;

	__DtTestPoint(__DtFunc_FreeAllHandles, __DtStep_0)
	if (psBase->ui32FreeHandCount == psBase->ui32TotalHandCount)
	{
		__DtTestPoint(__DtFunc_FreeAllHandles, __DtStep_1)
		return eError;
	}

	for (i = 0; i < psBase->ui32TotalHandCount; i++)
	{
		struct sHandle *psHandle;

		__DtTestPoint(__DtFunc_FreeAllHandles, __DtStep_2)
		psHandle = INDEX_TO_HANDLE_STRUCT_PTR(psBase, i);

		if (psHandle->eType != PVRSRV_HANDLE_TYPE_NONE)
		{
			__DtTestPoint(__DtFunc_FreeAllHandles, __DtStep_3)
			eError = FreeHandle(psBase, psHandle);
			if (eError != PVRSRV_OK)
			{
				__DtTestPoint(__DtFunc_FreeAllHandles, __DtStep_4)
				PVR_DPF((PVR_DBG_ERROR, "FreeAllHandles: FreeHandle failed (%d)", eError));
				break;
			}

			
			if (psBase->ui32FreeHandCount == psBase->ui32TotalHandCount)
			{
				__DtTestPoint(__DtFunc_FreeAllHandles, __DtStep_5)
				break;
			}
		}
	}

	__DtTestPoint(__DtFunc_FreeAllHandles, __DtStep_6)
	return eError;
}

static PVRSRV_ERROR FreeHandleBase(PVRSRV_HANDLE_BASE *psBase)
{
	PVRSRV_ERROR eError;

	__DtTestPoint(__DtFunc_FreeHandleBase, __DtStep_0)
	if (HANDLES_BATCHED(psBase))
	{
		__DtTestPoint(__DtFunc_FreeHandleBase, __DtStep_1)
		PVR_DPF((PVR_DBG_WARNING, "FreeHandleBase: Uncommitted/Unreleased handle batch"));
		PVRSRVReleaseHandleBatch(psBase);
	}

	
	eError = FreeAllHandles(psBase);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_FreeHandleBase, __DtStep_2)
		PVR_DPF((PVR_DBG_ERROR, "FreeHandleBase: Couldn't free handles (%d)", eError));
		__DtTestPoint(__DtFunc_FreeHandleBase, __DtStep_3)
		return eError;
	}

	
	eError = FreeHandleArray(psBase);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_FreeHandleBase, __DtStep_4)
		PVR_DPF((PVR_DBG_ERROR, "FreeHandleBase: Couldn't free handle array (%d)", eError));
		__DtTestPoint(__DtFunc_FreeHandleBase, __DtStep_5)
		return eError;
	}

	if (psBase->psHashTab != IMG_NULL)
	{
		
		__DtTestPoint(__DtFunc_FreeHandleBase, __DtStep_6)
		HASH_Delete(psBase->psHashTab);
	}

	eError = OSFreeMem(PVRSRV_OS_PAGEABLE_HEAP,
		sizeof(*psBase),
		psBase,
		psBase->hBaseBlockAlloc);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_FreeHandleBase, __DtStep_7)
		PVR_DPF((PVR_DBG_ERROR, "FreeHandleBase: Couldn't free handle base (%d)", eError));
		__DtTestPoint(__DtFunc_FreeHandleBase, __DtStep_8)
		return eError;
	}

	__DtTestPoint(__DtFunc_FreeHandleBase, __DtStep_9)
	return PVRSRV_OK;
}

#ifdef INLINE_IS_PRAGMA
#pragma inline(FindHandle)
#endif
static INLINE
IMG_HANDLE FindHandle(PVRSRV_HANDLE_BASE *psBase, IMG_VOID *pvData, PVRSRV_HANDLE_TYPE eType, IMG_HANDLE hParent)
{
	HAND_KEY aKey;

	__DtTestPoint(__DtFunc_FindHandle, __DtStep_0)
	PVR_ASSERT(eType != PVRSRV_HANDLE_TYPE_NONE);

	InitKey(aKey, psBase, pvData, eType, hParent);

	__DtTestPoint(__DtFunc_FindHandle, __DtStep_1)
	return (IMG_HANDLE) HASH_Retrieve_Extended(psBase->psHashTab, aKey);
}

static PVRSRV_ERROR IncreaseHandleArraySize(PVRSRV_HANDLE_BASE *psBase, IMG_UINT32 ui32Delta)
{
	PVRSRV_ERROR eError;
	IMG_UINT32 ui32DeltaAdjusted = ROUND_UP_TO_MULTIPLE_OF_BLOCK_SIZE(ui32Delta);
	IMG_UINT32 ui32NewTotalHandCount = psBase->ui32TotalHandCount + ui32DeltaAdjusted;
__DtTestPoint(__DtFunc_IncreaseHandleArraySize, __DtStep_0)
;

	PVR_ASSERT(ui32Delta != 0);

	
	if (ui32NewTotalHandCount > psBase->ui32MaxIndexPlusOne || ui32NewTotalHandCount <= psBase->ui32TotalHandCount)
	{
		__DtTestPoint(__DtFunc_IncreaseHandleArraySize, __DtStep_1)
		ui32NewTotalHandCount = psBase->ui32MaxIndexPlusOne;

		ui32DeltaAdjusted = ui32NewTotalHandCount - psBase->ui32TotalHandCount;

		if (ui32DeltaAdjusted < ui32Delta)
		{
			__DtTestPoint(__DtFunc_IncreaseHandleArraySize, __DtStep_2)
			PVR_DPF((PVR_DBG_ERROR, "IncreaseHandleArraySize: Maximum handle limit reached (%d)", psBase->ui32MaxIndexPlusOne));
			__DtTestPoint(__DtFunc_IncreaseHandleArraySize, __DtStep_3)
			return PVRSRV_ERROR_OUT_OF_MEMORY;
		}
	}

	PVR_ASSERT(ui32DeltaAdjusted >= ui32Delta);

	
	eError = ReallocHandleArray(psBase, ui32NewTotalHandCount);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_IncreaseHandleArraySize, __DtStep_4)
		PVR_DPF((PVR_DBG_ERROR, "IncreaseHandleArraySize: ReallocHandleArray failed (%d)", eError));
		__DtTestPoint(__DtFunc_IncreaseHandleArraySize, __DtStep_5)
		return eError;
	}

	__DtTestPoint(__DtFunc_IncreaseHandleArraySize, __DtStep_6)
	return PVRSRV_OK;
}

static PVRSRV_ERROR EnsureFreeHandles(PVRSRV_HANDLE_BASE *psBase, IMG_UINT32 ui32Free)
{
	PVRSRV_ERROR eError;

	__DtTestPoint(__DtFunc_EnsureFreeHandles, __DtStep_0)
	if (ui32Free > psBase->ui32FreeHandCount)
	{
		IMG_UINT32 ui32FreeHandDelta = ui32Free - psBase->ui32FreeHandCount;
		__DtTestPoint(__DtFunc_EnsureFreeHandles, __DtStep_1)
		eError = IncreaseHandleArraySize(psBase, ui32FreeHandDelta);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_EnsureFreeHandles, __DtStep_2)
			PVR_DPF((PVR_DBG_ERROR, "EnsureFreeHandles: Couldn't allocate %u handles to ensure %u free handles (IncreaseHandleArraySize failed with error %d)", ui32FreeHandDelta, ui32Free, eError));

			__DtTestPoint(__DtFunc_EnsureFreeHandles, __DtStep_3)
			return eError;
		}
	}

	__DtTestPoint(__DtFunc_EnsureFreeHandles, __DtStep_4)
	return PVRSRV_OK;
}

static PVRSRV_ERROR AllocHandle(PVRSRV_HANDLE_BASE *psBase, IMG_HANDLE *phHandle, IMG_VOID *pvData, PVRSRV_HANDLE_TYPE eType, PVRSRV_HANDLE_ALLOC_FLAG eFlag, IMG_HANDLE hParent)
{
	IMG_UINT32 ui32NewIndex = DEFAULT_MAX_INDEX_PLUS_ONE;
	struct sHandle *psNewHandle = IMG_NULL;
	IMG_HANDLE hHandle;
	HAND_KEY aKey;
	PVRSRV_ERROR eError;

	
	__DtTestPoint(__DtFunc_AllocHandle, __DtStep_0)
	PVR_ASSERT(eType != PVRSRV_HANDLE_TYPE_NONE);
	PVR_ASSERT(psBase != IMG_NULL);
	PVR_ASSERT(psBase->psHashTab != IMG_NULL);

	if (!TEST_FLAG(eFlag, PVRSRV_HANDLE_ALLOC_FLAG_MULTI))
	{
		
		__DtTestPoint(__DtFunc_AllocHandle, __DtStep_1)
		PVR_ASSERT(FindHandle(psBase, pvData, eType, hParent) == IMG_NULL);
	}

	if (psBase->ui32FreeHandCount == 0 && HANDLES_BATCHED(psBase))
	{
		 __DtTestPoint(__DtFunc_AllocHandle, __DtStep_2)
		 PVR_DPF((PVR_DBG_WARNING, "AllocHandle: Handle batch size (%u) was too small, allocating additional space", psBase->ui32HandBatchSize));
	}

	
	eError = EnsureFreeHandles(psBase, 1);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_AllocHandle, __DtStep_3)
		PVR_DPF((PVR_DBG_ERROR, "AllocHandle: EnsureFreeHandles failed (%d)", eError));
		__DtTestPoint(__DtFunc_AllocHandle, __DtStep_4)
		return eError;
	}
	PVR_ASSERT(psBase->ui32FreeHandCount != 0)

	if (!psBase->bPurgingEnabled)
	{
		
		__DtTestPoint(__DtFunc_AllocHandle, __DtStep_5)
		ui32NewIndex = psBase->ui32FirstFreeIndex;

		
		psNewHandle = INDEX_TO_HANDLE_STRUCT_PTR(psBase, ui32NewIndex);
	}
	else
	{
		IMG_UINT32 ui32BlockedIndex;

		
		
		__DtTestPoint(__DtFunc_AllocHandle, __DtStep_6)
		PVR_ASSERT((psBase->ui32FirstFreeIndex % HANDLE_BLOCK_SIZE) == 0);

		for (ui32BlockedIndex = ROUND_DOWN_TO_MULTIPLE_OF_BLOCK_SIZE(psBase->ui32FirstFreeIndex); ui32BlockedIndex < psBase->ui32TotalHandCount; ui32BlockedIndex += HANDLE_BLOCK_SIZE)
		{
			struct sHandleIndex *psIndex = BASE_AND_INDEX_TO_INDEX_STRUCT_PTR(psBase, ui32BlockedIndex);

			__DtTestPoint(__DtFunc_AllocHandle, __DtStep_7)
			if (psIndex->ui32FreeHandBlockCount == 0)
			{
				__DtTestPoint(__DtFunc_AllocHandle, __DtStep_8)
				continue;
			}

			for (ui32NewIndex = ui32BlockedIndex; ui32NewIndex < ui32BlockedIndex + HANDLE_BLOCK_SIZE; ui32NewIndex++)
			{
				__DtTestPoint(__DtFunc_AllocHandle, __DtStep_9)
				psNewHandle = INDEX_TO_HANDLE_STRUCT_PTR(psBase, ui32NewIndex);
				if (HANDLE_STRUCT_IS_FREE(psNewHandle))
				{
					__DtTestPoint(__DtFunc_AllocHandle, __DtStep_10)
					break;
				}
			}
		}
		psBase->ui32FirstFreeIndex = 0;
		PVR_ASSERT(ui32NewIndex < psBase->ui32TotalHandCount);
	}
	PVR_ASSERT(psNewHandle != IMG_NULL);

	
	hHandle = INDEX_TO_HANDLE(ui32NewIndex);

	
	if (!TEST_FLAG(eFlag, PVRSRV_HANDLE_ALLOC_FLAG_MULTI))
	{
		
		__DtTestPoint(__DtFunc_AllocHandle, __DtStep_11)
		InitKey(aKey, psBase, pvData, eType, hParent);

		
		if (!HASH_Insert_Extended(psBase->psHashTab, aKey, (IMG_UINTPTR_T)hHandle))
		{
			__DtTestPoint(__DtFunc_AllocHandle, __DtStep_12)
			PVR_DPF((PVR_DBG_ERROR, "AllocHandle: Couldn't add handle to hash table"));

			__DtTestPoint(__DtFunc_AllocHandle, __DtStep_13)
			return PVRSRV_ERROR_UNABLE_TO_ADD_HANDLE;
		}
	}

	psBase->ui32FreeHandCount--;

	PVR_ASSERT(INDEX_TO_FREE_HAND_BLOCK_COUNT(psBase, ui32NewIndex) <= HANDLE_BLOCK_SIZE);
	PVR_ASSERT(INDEX_TO_FREE_HAND_BLOCK_COUNT(psBase, ui32NewIndex) > 0);

	INDEX_TO_FREE_HAND_BLOCK_COUNT(psBase, ui32NewIndex)--;

	
	if (!psBase->bPurgingEnabled)
	{
		
		__DtTestPoint(__DtFunc_AllocHandle, __DtStep_14)
		if (psBase->ui32FreeHandCount == 0)
		{
			__DtTestPoint(__DtFunc_AllocHandle, __DtStep_15)
			PVR_ASSERT(psBase->ui32FirstFreeIndex == ui32NewIndex);
			PVR_ASSERT(psBase->ui32LastFreeIndexPlusOne == (ui32NewIndex + 1));

			psBase->ui32LastFreeIndexPlusOne = 0;
			psBase->ui32FirstFreeIndex = 0;
		}
		else
		{
			
			__DtTestPoint(__DtFunc_AllocHandle, __DtStep_16)
			psBase->ui32FirstFreeIndex = (psNewHandle->ui32NextIndexPlusOne == 0) ?
				ui32NewIndex + 1 :
				psNewHandle->ui32NextIndexPlusOne - 1;
		}
	}

	
	PVR_ASSERT(psNewHandle->ui32Index == ui32NewIndex);

	 
	psNewHandle->eType = eType;
	psNewHandle->pvData = pvData;
	psNewHandle->eInternalFlag = INTERNAL_HANDLE_FLAG_NONE;
	psNewHandle->eFlag = eFlag;

	InitParentList(psNewHandle);
#if defined(DEBUG)
	PVR_ASSERT(NoChildren(psNewHandle));
#endif

	InitChildEntry(psNewHandle);
#if defined(DEBUG)
	PVR_ASSERT(NoParent(psNewHandle));
#endif

	if (HANDLES_BATCHED(psBase))
	{
		
		__DtTestPoint(__DtFunc_AllocHandle, __DtStep_17)
		psNewHandle->ui32NextIndexPlusOne = psBase->ui32FirstBatchIndexPlusOne;

		psBase->ui32FirstBatchIndexPlusOne = ui32NewIndex + 1;

		 
		SET_BATCHED_HANDLE(psNewHandle);
	}
	else
	{
		__DtTestPoint(__DtFunc_AllocHandle, __DtStep_18)
		psNewHandle->ui32NextIndexPlusOne = 0;
	}

	
	*phHandle = hHandle;

	__DtTestPoint(__DtFunc_AllocHandle, __DtStep_19)
	return PVRSRV_OK;
}

PVRSRV_ERROR PVRSRVAllocHandle(PVRSRV_HANDLE_BASE *psBase, IMG_HANDLE *phHandle, IMG_VOID *pvData, PVRSRV_HANDLE_TYPE eType, PVRSRV_HANDLE_ALLOC_FLAG eFlag)
{
	IMG_HANDLE hHandle;
	PVRSRV_ERROR eError;

	__DtTestPoint(__DtFunc_PVRSRVAllocHandle, __DtStep_0)
	*phHandle = IMG_NULL;

	if (HANDLES_BATCHED(psBase))
	{
		
		__DtTestPoint(__DtFunc_PVRSRVAllocHandle, __DtStep_1)
		psBase->ui32BatchHandAllocFailures++;
	}

	
	PVR_ASSERT(eType != PVRSRV_HANDLE_TYPE_NONE);

	if (!TEST_FLAG(eFlag, PVRSRV_HANDLE_ALLOC_FLAG_MULTI))
	{
		
		__DtTestPoint(__DtFunc_PVRSRVAllocHandle, __DtStep_2)
		hHandle = FindHandle(psBase, pvData, eType, IMG_NULL);
		if (hHandle != IMG_NULL)
		{
			struct sHandle *psHandle;

			__DtTestPoint(__DtFunc_PVRSRVAllocHandle, __DtStep_3)
			eError = GetHandleStructure(psBase, &psHandle, hHandle, eType);
			if (eError != PVRSRV_OK)
			{
				__DtTestPoint(__DtFunc_PVRSRVAllocHandle, __DtStep_4)
				PVR_DPF((PVR_DBG_ERROR, "PVRSRVAllocHandle: Lookup of existing handle failed"));
				__DtTestPoint(__DtFunc_PVRSRVAllocHandle, __DtStep_5)
				return eError;
			}

			
			if (TEST_FLAG(psHandle->eFlag & eFlag, PVRSRV_HANDLE_ALLOC_FLAG_SHARED))
			{
				__DtTestPoint(__DtFunc_PVRSRVAllocHandle, __DtStep_6)
				*phHandle = hHandle;
				eError = PVRSRV_OK;
				goto exit_ok;
			}
			__DtTestPoint(__DtFunc_PVRSRVAllocHandle, __DtStep_7)
			return PVRSRV_ERROR_HANDLE_NOT_SHAREABLE;
		}
	}

	eError = AllocHandle(psBase, phHandle, pvData, eType, eFlag, IMG_NULL);

exit_ok:
	if (HANDLES_BATCHED(psBase) && (eError == PVRSRV_OK))
	{
		__DtTestPoint(__DtFunc_PVRSRVAllocHandle, __DtStep_8)
		psBase->ui32BatchHandAllocFailures--;
	}

	__DtTestPoint(__DtFunc_PVRSRVAllocHandle, __DtStep_9)
	return eError;
}

PVRSRV_ERROR PVRSRVAllocSubHandle(PVRSRV_HANDLE_BASE *psBase, IMG_HANDLE *phHandle, IMG_VOID *pvData, PVRSRV_HANDLE_TYPE eType, PVRSRV_HANDLE_ALLOC_FLAG eFlag, IMG_HANDLE hParent)
{
	struct sHandle *psPHand;
	struct sHandle *psCHand;
	PVRSRV_ERROR eError;
	IMG_HANDLE hParentKey;
	IMG_HANDLE hHandle;

	__DtTestPoint(__DtFunc_PVRSRVAllocSubHandle, __DtStep_0)
	*phHandle = IMG_NULL;

	if (HANDLES_BATCHED(psBase))
	{
		
		__DtTestPoint(__DtFunc_PVRSRVAllocSubHandle, __DtStep_1)
		psBase->ui32BatchHandAllocFailures++;
	}

	
	PVR_ASSERT(eType != PVRSRV_HANDLE_TYPE_NONE);

	hParentKey = TEST_FLAG(eFlag, PVRSRV_HANDLE_ALLOC_FLAG_PRIVATE) ?
			hParent : IMG_NULL;

	
	eError = GetHandleStructure(psBase, &psPHand, hParent, PVRSRV_HANDLE_TYPE_NONE);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVAllocSubHandle, __DtStep_2)
		return eError;
	}

	if (!TEST_FLAG(eFlag, PVRSRV_HANDLE_ALLOC_FLAG_MULTI))
	{
		
		__DtTestPoint(__DtFunc_PVRSRVAllocSubHandle, __DtStep_3)
		hHandle = FindHandle(psBase, pvData, eType, hParentKey);
		if (hHandle != IMG_NULL)
		{
			struct sHandle *psCHandle;
			PVRSRV_ERROR eErr;

			__DtTestPoint(__DtFunc_PVRSRVAllocSubHandle, __DtStep_4)
			eErr = GetHandleStructure(psBase, &psCHandle, hHandle, eType);
			if (eErr != PVRSRV_OK)
			{
				__DtTestPoint(__DtFunc_PVRSRVAllocSubHandle, __DtStep_5)
				PVR_DPF((PVR_DBG_ERROR, "PVRSRVAllocSubHandle: Lookup of existing handle failed"));
				__DtTestPoint(__DtFunc_PVRSRVAllocSubHandle, __DtStep_6)
				return eErr;
			}

			PVR_ASSERT(hParentKey != IMG_NULL && ParentHandle(HANDLE_TO_HANDLE_STRUCT_PTR(psBase, hHandle)) == hParent);

			
			if (TEST_FLAG(psCHandle->eFlag & eFlag, PVRSRV_HANDLE_ALLOC_FLAG_SHARED) && ParentHandle(HANDLE_TO_HANDLE_STRUCT_PTR(psBase, hHandle)) == hParent)
			{
				__DtTestPoint(__DtFunc_PVRSRVAllocSubHandle, __DtStep_7)
				*phHandle = hHandle;
				goto exit_ok;
			}
			__DtTestPoint(__DtFunc_PVRSRVAllocSubHandle, __DtStep_8)
			return PVRSRV_ERROR_HANDLE_NOT_SHAREABLE;
		}
	}

	eError = AllocHandle(psBase, &hHandle, pvData, eType, eFlag, hParentKey);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVAllocSubHandle, __DtStep_9)
		return eError;
	}

	
	psPHand = HANDLE_TO_HANDLE_STRUCT_PTR(psBase, hParent);

	psCHand = HANDLE_TO_HANDLE_STRUCT_PTR(psBase, hHandle);

	AdoptChild(psBase, psPHand, psCHand);

	*phHandle = hHandle;

exit_ok:
	if (HANDLES_BATCHED(psBase))
	{
		__DtTestPoint(__DtFunc_PVRSRVAllocSubHandle, __DtStep_10)
		psBase->ui32BatchHandAllocFailures--;
	}

	__DtTestPoint(__DtFunc_PVRSRVAllocSubHandle, __DtStep_11)
	return PVRSRV_OK;
}

PVRSRV_ERROR PVRSRVFindHandle(PVRSRV_HANDLE_BASE *psBase, IMG_HANDLE *phHandle, IMG_VOID *pvData, PVRSRV_HANDLE_TYPE eType)
{
	IMG_HANDLE hHandle;

	__DtTestPoint(__DtFunc_PVRSRVFindHandle, __DtStep_0)
	PVR_ASSERT(eType != PVRSRV_HANDLE_TYPE_NONE);

	
	hHandle = (IMG_HANDLE) FindHandle(psBase, pvData, eType, IMG_NULL);
	if (hHandle == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_PVRSRVFindHandle, __DtStep_1)
		return PVRSRV_ERROR_HANDLE_NOT_FOUND;
	}

	*phHandle = hHandle;

	__DtTestPoint(__DtFunc_PVRSRVFindHandle, __DtStep_2)
	return PVRSRV_OK;
}

PVRSRV_ERROR PVRSRVLookupHandleAnyType(PVRSRV_HANDLE_BASE *psBase, IMG_PVOID *ppvData, PVRSRV_HANDLE_TYPE *peType, IMG_HANDLE hHandle)
{
	struct sHandle *psHandle;
	PVRSRV_ERROR eError;

	__DtTestPoint(__DtFunc_PVRSRVLookupHandleAnyType, __DtStep_0)
	eError = GetHandleStructure(psBase, &psHandle, hHandle, PVRSRV_HANDLE_TYPE_NONE);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVLookupHandleAnyType, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "PVRSRVLookupHandleAnyType: Error looking up handle (%d)", eError));
		__DtTestPoint(__DtFunc_PVRSRVLookupHandleAnyType, __DtStep_2)
		return eError;
	}

	*ppvData = psHandle->pvData;
	*peType = psHandle->eType;

	__DtTestPoint(__DtFunc_PVRSRVLookupHandleAnyType, __DtStep_3)
	return PVRSRV_OK;
}

PVRSRV_ERROR PVRSRVLookupHandle(PVRSRV_HANDLE_BASE *psBase, IMG_PVOID *ppvData, IMG_HANDLE hHandle, PVRSRV_HANDLE_TYPE eType)
{
	struct sHandle *psHandle;
	PVRSRV_ERROR eError;

	__DtTestPoint(__DtFunc_PVRSRVLookupHandle, __DtStep_0)
	PVR_ASSERT(eType != PVRSRV_HANDLE_TYPE_NONE);

	eError = GetHandleStructure(psBase, &psHandle, hHandle, eType);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVLookupHandle, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "PVRSRVLookupHandle: Error looking up handle (%d)", eError));
		__DtTestPoint(__DtFunc_PVRSRVLookupHandle, __DtStep_2)
		return eError;
	}

	*ppvData = psHandle->pvData;

	__DtTestPoint(__DtFunc_PVRSRVLookupHandle, __DtStep_3)
	return PVRSRV_OK;
}

PVRSRV_ERROR PVRSRVLookupSubHandle(PVRSRV_HANDLE_BASE *psBase, IMG_PVOID *ppvData, IMG_HANDLE hHandle, PVRSRV_HANDLE_TYPE eType, IMG_HANDLE hAncestor)
{
	struct sHandle *psPHand;
	struct sHandle *psCHand;
	PVRSRV_ERROR eError;

	__DtTestPoint(__DtFunc_PVRSRVLookupSubHandle, __DtStep_0)
	PVR_ASSERT(eType != PVRSRV_HANDLE_TYPE_NONE);

	eError = GetHandleStructure(psBase, &psCHand, hHandle, eType);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVLookupSubHandle, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "PVRSRVLookupSubHandle: Error looking up subhandle (%d)", eError));
		__DtTestPoint(__DtFunc_PVRSRVLookupSubHandle, __DtStep_2)
		return eError;
	}

	
	for (psPHand = psCHand; ParentHandle(psPHand) != hAncestor; )
	{
		__DtTestPoint(__DtFunc_PVRSRVLookupSubHandle, __DtStep_3)
		eError = GetHandleStructure(psBase, &psPHand, ParentHandle(psPHand), PVRSRV_HANDLE_TYPE_NONE);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PVRSRVLookupSubHandle, __DtStep_4)
			PVR_DPF((PVR_DBG_ERROR, "PVRSRVLookupSubHandle: Subhandle doesn't belong to given ancestor"));
			__DtTestPoint(__DtFunc_PVRSRVLookupSubHandle, __DtStep_5)
			return PVRSRV_ERROR_INVALID_SUBHANDLE;
		}
	}

	*ppvData = psCHand->pvData;

	__DtTestPoint(__DtFunc_PVRSRVLookupSubHandle, __DtStep_6)
	return PVRSRV_OK;
}

PVRSRV_ERROR PVRSRVGetParentHandle(PVRSRV_HANDLE_BASE *psBase, IMG_PVOID *phParent, IMG_HANDLE hHandle, PVRSRV_HANDLE_TYPE eType)
{
	struct sHandle *psHandle;
	PVRSRV_ERROR eError;

	__DtTestPoint(__DtFunc_PVRSRVGetParentHandle, __DtStep_0)
	PVR_ASSERT(eType != PVRSRV_HANDLE_TYPE_NONE);

	eError = GetHandleStructure(psBase, &psHandle, hHandle, eType);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVGetParentHandle, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "PVRSRVGetParentHandle: Error looking up subhandle (%d)", eError));
		__DtTestPoint(__DtFunc_PVRSRVGetParentHandle, __DtStep_2)
		return eError;
	}

	*phParent = ParentHandle(psHandle);

	__DtTestPoint(__DtFunc_PVRSRVGetParentHandle, __DtStep_3)
	return PVRSRV_OK;
}

PVRSRV_ERROR PVRSRVLookupAndReleaseHandle(PVRSRV_HANDLE_BASE *psBase, IMG_PVOID *ppvData, IMG_HANDLE hHandle, PVRSRV_HANDLE_TYPE eType)
{
	struct sHandle *psHandle;
	PVRSRV_ERROR eError;

	__DtTestPoint(__DtFunc_PVRSRVLookupAndReleaseHandle, __DtStep_0)
	PVR_ASSERT(eType != PVRSRV_HANDLE_TYPE_NONE);

	eError = GetHandleStructure(psBase, &psHandle, hHandle, eType);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVLookupAndReleaseHandle, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "PVRSRVLookupAndReleaseHandle: Error looking up handle (%d)", eError));
		__DtTestPoint(__DtFunc_PVRSRVLookupAndReleaseHandle, __DtStep_2)
		return eError;
	}

	*ppvData = psHandle->pvData;

	eError = FreeHandle(psBase, psHandle);

	__DtTestPoint(__DtFunc_PVRSRVLookupAndReleaseHandle, __DtStep_3)
	return eError;
}

PVRSRV_ERROR PVRSRVReleaseHandle(PVRSRV_HANDLE_BASE *psBase, IMG_HANDLE hHandle, PVRSRV_HANDLE_TYPE eType)
{
	struct sHandle *psHandle;
	PVRSRV_ERROR eError;

	__DtTestPoint(__DtFunc_PVRSRVReleaseHandle, __DtStep_0)
	PVR_ASSERT(eType != PVRSRV_HANDLE_TYPE_NONE);

	eError = GetHandleStructure(psBase, &psHandle, hHandle, eType);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVReleaseHandle, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "PVRSRVReleaseHandle: Error looking up handle (%d)", eError));
		__DtTestPoint(__DtFunc_PVRSRVReleaseHandle, __DtStep_2)
		return eError;
	}

	eError = FreeHandle(psBase, psHandle);

	__DtTestPoint(__DtFunc_PVRSRVReleaseHandle, __DtStep_3)
	return eError;
}

PVRSRV_ERROR PVRSRVNewHandleBatch(PVRSRV_HANDLE_BASE *psBase, IMG_UINT32 ui32BatchSize)
{
	PVRSRV_ERROR eError;

	__DtTestPoint(__DtFunc_PVRSRVNewHandleBatch, __DtStep_0)
	if (HANDLES_BATCHED(psBase))
	{
		__DtTestPoint(__DtFunc_PVRSRVNewHandleBatch, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "PVRSRVNewHandleBatch: There is a handle batch already in use (size %u)", psBase->ui32HandBatchSize));
		__DtTestPoint(__DtFunc_PVRSRVNewHandleBatch, __DtStep_2)
		return  PVRSRV_ERROR_HANDLE_BATCH_IN_USE;
	}

	if (ui32BatchSize == 0)
	{
		__DtTestPoint(__DtFunc_PVRSRVNewHandleBatch, __DtStep_3)
		PVR_DPF((PVR_DBG_ERROR, "PVRSRVNewHandleBatch: Invalid batch size (%u)", ui32BatchSize));
		__DtTestPoint(__DtFunc_PVRSRVNewHandleBatch, __DtStep_4)
		return  PVRSRV_ERROR_INVALID_PARAMS;
	}

	eError = EnsureFreeHandles(psBase, ui32BatchSize);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVNewHandleBatch, __DtStep_5)
		PVR_DPF((PVR_DBG_ERROR, "PVRSRVNewHandleBatch: EnsureFreeHandles failed (error %d)", eError));
		__DtTestPoint(__DtFunc_PVRSRVNewHandleBatch, __DtStep_6)
		return eError;
	}

	psBase->ui32HandBatchSize = ui32BatchSize;

	
	psBase->ui32TotalHandCountPreBatch = psBase->ui32TotalHandCount;

	PVR_ASSERT(psBase->ui32BatchHandAllocFailures == 0);

	PVR_ASSERT(psBase->ui32FirstBatchIndexPlusOne == 0);

	PVR_ASSERT(HANDLES_BATCHED(psBase));

	__DtTestPoint(__DtFunc_PVRSRVNewHandleBatch, __DtStep_7)
	return PVRSRV_OK;
}

static PVRSRV_ERROR PVRSRVHandleBatchCommitOrRelease(PVRSRV_HANDLE_BASE *psBase, IMG_BOOL bCommit)
{

	IMG_UINT32 ui32IndexPlusOne;
	IMG_BOOL bCommitBatch = bCommit;

	__DtTestPoint(__DtFunc_PVRSRVHandleBatchCommitOrRelease, __DtStep_0)
	if (!HANDLES_BATCHED(psBase))
	{
		__DtTestPoint(__DtFunc_PVRSRVHandleBatchCommitOrRelease, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "PVRSRVHandleBatchCommitOrRelease: There is no handle batch"));
		__DtTestPoint(__DtFunc_PVRSRVHandleBatchCommitOrRelease, __DtStep_2)
		return PVRSRV_ERROR_INVALID_PARAMS;

	}

	if (psBase->ui32BatchHandAllocFailures != 0)
	{
		__DtTestPoint(__DtFunc_PVRSRVHandleBatchCommitOrRelease, __DtStep_3)
		if (bCommit)
		{
			__DtTestPoint(__DtFunc_PVRSRVHandleBatchCommitOrRelease, __DtStep_4)
			PVR_DPF((PVR_DBG_ERROR, "PVRSRVHandleBatchCommitOrRelease: Attempting to commit batch with handle allocation failures."));
		}
		bCommitBatch = IMG_FALSE;
	}
	
	PVR_ASSERT(psBase->ui32BatchHandAllocFailures == 0 || !bCommit);

	ui32IndexPlusOne = psBase->ui32FirstBatchIndexPlusOne;
	while(ui32IndexPlusOne != 0)
	{
		struct sHandle *psHandle = INDEX_TO_HANDLE_STRUCT_PTR(psBase, ui32IndexPlusOne - 1);
		IMG_UINT32 ui32NextIndexPlusOne = psHandle->ui32NextIndexPlusOne;
		__DtTestPoint(__DtFunc_PVRSRVHandleBatchCommitOrRelease, __DtStep_5)
		PVR_ASSERT(BATCHED_HANDLE(psHandle));

		psHandle->ui32NextIndexPlusOne = 0;

		if (!bCommitBatch || BATCHED_HANDLE_PARTIALLY_FREE(psHandle))
		{
			PVRSRV_ERROR eError;

			
			__DtTestPoint(__DtFunc_PVRSRVHandleBatchCommitOrRelease, __DtStep_6)
			if (!BATCHED_HANDLE_PARTIALLY_FREE(psHandle))
			{
				 
				__DtTestPoint(__DtFunc_PVRSRVHandleBatchCommitOrRelease, __DtStep_7)
				SET_UNBATCHED_HANDLE(psHandle);  
			}

			eError = FreeHandle(psBase, psHandle);
			if (eError != PVRSRV_OK)
			{
				 __DtTestPoint(__DtFunc_PVRSRVHandleBatchCommitOrRelease, __DtStep_8)
				 PVR_DPF((PVR_DBG_ERROR, "PVRSRVHandleBatchCommitOrRelease: Error freeing handle (%d)", eError));
			}
			PVR_ASSERT(eError == PVRSRV_OK);
		}
		else
		{
			 
			__DtTestPoint(__DtFunc_PVRSRVHandleBatchCommitOrRelease, __DtStep_9)
			SET_UNBATCHED_HANDLE(psHandle);
		}

		ui32IndexPlusOne = ui32NextIndexPlusOne;
	}

#ifdef DEBUG
	if (psBase->ui32TotalHandCountPreBatch != psBase->ui32TotalHandCount)
	{
		IMG_UINT32 ui32Delta = psBase->ui32TotalHandCount - psBase->ui32TotalHandCountPreBatch;

		__DtTestPoint(__DtFunc_PVRSRVHandleBatchCommitOrRelease, __DtStep_10)
		PVR_ASSERT(psBase->ui32TotalHandCount > psBase->ui32TotalHandCountPreBatch);

		PVR_DPF((PVR_DBG_WARNING, "PVRSRVHandleBatchCommitOrRelease: The batch size was too small.  Batch size was %u, but needs to be %u", psBase->ui32HandBatchSize,  psBase->ui32HandBatchSize + ui32Delta));

	}
#endif

	psBase->ui32HandBatchSize = 0;
	psBase->ui32FirstBatchIndexPlusOne = 0;
	psBase->ui32TotalHandCountPreBatch = 0;
	psBase->ui32BatchHandAllocFailures = 0;

	if (psBase->ui32BatchHandAllocFailures != 0 && bCommit)
	{
		__DtTestPoint(__DtFunc_PVRSRVHandleBatchCommitOrRelease, __DtStep_11)
		PVR_ASSERT(!bCommitBatch);

		__DtTestPoint(__DtFunc_PVRSRVHandleBatchCommitOrRelease, __DtStep_12)
		return PVRSRV_ERROR_HANDLE_BATCH_COMMIT_FAILURE;
	}

	__DtTestPoint(__DtFunc_PVRSRVHandleBatchCommitOrRelease, __DtStep_13)
	return PVRSRV_OK;
}

PVRSRV_ERROR PVRSRVCommitHandleBatch(PVRSRV_HANDLE_BASE *psBase)
{
	__DtTestPoint(__DtFunc_PVRSRVCommitHandleBatch, __DtStep_0)
	return PVRSRVHandleBatchCommitOrRelease(psBase, IMG_TRUE);
}

IMG_VOID PVRSRVReleaseHandleBatch(PVRSRV_HANDLE_BASE *psBase)
{
	__DtTestPoint(__DtFunc_PVRSRVReleaseHandleBatch, __DtStep_0)
	(IMG_VOID) PVRSRVHandleBatchCommitOrRelease(psBase, IMG_FALSE);
	__DtTestPoint(__DtFunc_PVRSRVReleaseHandleBatch, __DtStep_1)
}

PVRSRV_ERROR PVRSRVSetMaxHandle(PVRSRV_HANDLE_BASE *psBase, IMG_UINT32 ui32MaxHandle)
{
	IMG_UINT32 ui32MaxHandleRounded;

	__DtTestPoint(__DtFunc_PVRSRVSetMaxHandle, __DtStep_0)
	if (HANDLES_BATCHED(psBase))
	{
		__DtTestPoint(__DtFunc_PVRSRVSetMaxHandle, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "PVRSRVSetMaxHandle: Limit cannot be set whilst in batch mode"));
		__DtTestPoint(__DtFunc_PVRSRVSetMaxHandle, __DtStep_2)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	
	if (ui32MaxHandle  == 0 || ui32MaxHandle > DEFAULT_MAX_HANDLE)
	{
		__DtTestPoint(__DtFunc_PVRSRVSetMaxHandle, __DtStep_3)
		PVR_DPF((PVR_DBG_ERROR, "PVRSRVSetMaxHandle: Limit must be between %u and %u, inclusive", 0, DEFAULT_MAX_HANDLE));

		__DtTestPoint(__DtFunc_PVRSRVSetMaxHandle, __DtStep_4)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	
	if (psBase->ui32TotalHandCount != 0)
	{
		__DtTestPoint(__DtFunc_PVRSRVSetMaxHandle, __DtStep_5)
		PVR_DPF((PVR_DBG_ERROR, "PVRSRVSetMaxHandle: Limit cannot be set because handles have already been allocated"));

		__DtTestPoint(__DtFunc_PVRSRVSetMaxHandle, __DtStep_6)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	ui32MaxHandleRounded = ROUND_DOWN_TO_MULTIPLE_OF_BLOCK_SIZE(ui32MaxHandle);

	
	if (ui32MaxHandleRounded != 0 && ui32MaxHandleRounded < psBase->ui32MaxIndexPlusOne)
	{
		__DtTestPoint(__DtFunc_PVRSRVSetMaxHandle, __DtStep_7)
		psBase->ui32MaxIndexPlusOne = ui32MaxHandleRounded;
	}

	PVR_ASSERT(psBase->ui32MaxIndexPlusOne != 0);
	PVR_ASSERT(psBase->ui32MaxIndexPlusOne <= DEFAULT_MAX_INDEX_PLUS_ONE);
	PVR_ASSERT((psBase->ui32MaxIndexPlusOne % HANDLE_BLOCK_SIZE) == 0);

	__DtTestPoint(__DtFunc_PVRSRVSetMaxHandle, __DtStep_8)
	return PVRSRV_OK;
}

IMG_UINT32 PVRSRVGetMaxHandle(PVRSRV_HANDLE_BASE *psBase)
{
	__DtTestPoint(__DtFunc_PVRSRVGetMaxHandle, __DtStep_0)
	return psBase->ui32MaxIndexPlusOne;
}

PVRSRV_ERROR PVRSRVEnableHandlePurging(PVRSRV_HANDLE_BASE *psBase)
{
	__DtTestPoint(__DtFunc_PVRSRVEnableHandlePurging, __DtStep_0)
	if (psBase->bPurgingEnabled)
	{
		__DtTestPoint(__DtFunc_PVRSRVEnableHandlePurging, __DtStep_1)
		PVR_DPF((PVR_DBG_WARNING, "PVRSRVEnableHandlePurging: Purging already enabled"));
		__DtTestPoint(__DtFunc_PVRSRVEnableHandlePurging, __DtStep_2)
		return PVRSRV_OK;
	}

	
	if (psBase->ui32TotalHandCount != 0)
	{
		__DtTestPoint(__DtFunc_PVRSRVEnableHandlePurging, __DtStep_3)
		PVR_DPF((PVR_DBG_ERROR, "PVRSRVEnableHandlePurging: Handles have already been allocated"));
		__DtTestPoint(__DtFunc_PVRSRVEnableHandlePurging, __DtStep_4)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	psBase->bPurgingEnabled = IMG_TRUE;

	__DtTestPoint(__DtFunc_PVRSRVEnableHandlePurging, __DtStep_5)
	return PVRSRV_OK;
}

PVRSRV_ERROR PVRSRVPurgeHandles(PVRSRV_HANDLE_BASE *psBase)
{
	IMG_UINT32 ui32BlockIndex;
	IMG_UINT32 ui32NewHandCount;

	__DtTestPoint(__DtFunc_PVRSRVPurgeHandles, __DtStep_0)
	if (!psBase->bPurgingEnabled)
	{
		__DtTestPoint(__DtFunc_PVRSRVPurgeHandles, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "PVRSRVPurgeHandles: Purging not enabled for this handle base"));
		__DtTestPoint(__DtFunc_PVRSRVPurgeHandles, __DtStep_2)
		return PVRSRV_ERROR_NOT_SUPPORTED;
	}

	if (HANDLES_BATCHED(psBase))
	{
		__DtTestPoint(__DtFunc_PVRSRVPurgeHandles, __DtStep_3)
		PVR_DPF((PVR_DBG_ERROR, "PVRSRVPurgeHandles: Purging not allowed whilst in batch mode"));
		__DtTestPoint(__DtFunc_PVRSRVPurgeHandles, __DtStep_4)
		return PVRSRV_ERROR_INVALID_PARAMS;
	}

	PVR_ASSERT((psBase->ui32TotalHandCount % HANDLE_BLOCK_SIZE) == 0);

	for (ui32BlockIndex = INDEX_TO_BLOCK_INDEX(psBase->ui32TotalHandCount); ui32BlockIndex != 0; ui32BlockIndex--)
	{
		__DtTestPoint(__DtFunc_PVRSRVPurgeHandles, __DtStep_5)
		if (psBase->psHandleArray[ui32BlockIndex - 1].ui32FreeHandBlockCount != HANDLE_BLOCK_SIZE)
		{
			__DtTestPoint(__DtFunc_PVRSRVPurgeHandles, __DtStep_6)
			break;
		}
	}
	ui32NewHandCount = BLOCK_INDEX_TO_INDEX(ui32BlockIndex);

	
	if (ui32NewHandCount <= (psBase->ui32TotalHandCount/2))
	{
		PVRSRV_ERROR eError;

		

		__DtTestPoint(__DtFunc_PVRSRVPurgeHandles, __DtStep_7)
		eError = ReallocHandleArray(psBase, ui32NewHandCount);
		if (eError != PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PVRSRVPurgeHandles, __DtStep_8)
			return eError;
		}
	}

	__DtTestPoint(__DtFunc_PVRSRVPurgeHandles, __DtStep_9)
	return PVRSRV_OK;
}

PVRSRV_ERROR PVRSRVAllocHandleBase(PVRSRV_HANDLE_BASE **ppsBase)
{
	PVRSRV_HANDLE_BASE *psBase;
	IMG_HANDLE hBlockAlloc;
	PVRSRV_ERROR eError;

	__DtTestPoint(__DtFunc_PVRSRVAllocHandleBase, __DtStep_0)
	eError = OSAllocMem(PVRSRV_OS_PAGEABLE_HEAP,
		sizeof(*psBase),
		(IMG_PVOID *)&psBase,
		&hBlockAlloc,
		"Handle Base");
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVAllocHandleBase, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "PVRSRVAllocHandleBase: Couldn't allocate handle base (%d)", eError));
		__DtTestPoint(__DtFunc_PVRSRVAllocHandleBase, __DtStep_2)
		return eError;
	}
	OSMemSet(psBase, 0, sizeof(*psBase));

	
	psBase->psHashTab = HASH_Create_Extended(HANDLE_HASH_TAB_INIT_SIZE, sizeof(HAND_KEY), HASH_Func_Default, HASH_Key_Comp_Default);
	if (psBase->psHashTab == IMG_NULL)
	{
		__DtTestPoint(__DtFunc_PVRSRVAllocHandleBase, __DtStep_3)
		PVR_DPF((PVR_DBG_ERROR, "PVRSRVAllocHandleBase: Couldn't create data pointer hash table\n"));
		(IMG_VOID)PVRSRVFreeHandleBase(psBase);
		__DtTestPoint(__DtFunc_PVRSRVAllocHandleBase, __DtStep_4)
		return PVRSRV_ERROR_UNABLE_TO_CREATE_HASH_TABLE;
	}

	psBase->hBaseBlockAlloc = hBlockAlloc;

	psBase->ui32MaxIndexPlusOne = DEFAULT_MAX_INDEX_PLUS_ONE;

	*ppsBase = psBase;

	__DtTestPoint(__DtFunc_PVRSRVAllocHandleBase, __DtStep_5)
	return PVRSRV_OK;
}

PVRSRV_ERROR PVRSRVFreeHandleBase(PVRSRV_HANDLE_BASE *psBase)
{
	PVRSRV_ERROR eError;

	__DtTestPoint(__DtFunc_PVRSRVFreeHandleBase, __DtStep_0)
	PVR_ASSERT(psBase != gpsKernelHandleBase);

	eError = FreeHandleBase(psBase);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVFreeHandleBase, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "PVRSRVFreeHandleBase: FreeHandleBase failed (%d)", eError));
	}

	__DtTestPoint(__DtFunc_PVRSRVFreeHandleBase, __DtStep_2)
	return eError;
}

PVRSRV_ERROR PVRSRVHandleInit(IMG_VOID)
{
	PVRSRV_ERROR eError;

	__DtTestPoint(__DtFunc_PVRSRVHandleInit, __DtStep_0)
	PVR_ASSERT(gpsKernelHandleBase == IMG_NULL);

	eError = PVRSRVAllocHandleBase(&gpsKernelHandleBase);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVHandleInit, __DtStep_1)
		PVR_DPF((PVR_DBG_ERROR, "PVRSRVHandleInit: PVRSRVAllocHandleBase failed (%d)", eError));
		goto error;
	}

	eError = PVRSRVEnableHandlePurging(gpsKernelHandleBase);
	if (eError != PVRSRV_OK)
	{
		__DtTestPoint(__DtFunc_PVRSRVHandleInit, __DtStep_2)
		PVR_DPF((PVR_DBG_ERROR, "PVRSRVHandleInit: PVRSRVEnableHandlePurging failed (%d)", eError));
		goto error;
	}

	__DtTestPoint(__DtFunc_PVRSRVHandleInit, __DtStep_3)
	return PVRSRV_OK;
error:
	(IMG_VOID) PVRSRVHandleDeInit();
	__DtTestPoint(__DtFunc_PVRSRVHandleInit, __DtStep_4)
	return eError;
}

PVRSRV_ERROR PVRSRVHandleDeInit(IMG_VOID)
{
	PVRSRV_ERROR eError = PVRSRV_OK;

	__DtTestPoint(__DtFunc_PVRSRVHandleDeInit, __DtStep_0)
	if (gpsKernelHandleBase != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_PVRSRVHandleDeInit, __DtStep_1)
		eError = FreeHandleBase(gpsKernelHandleBase);
		if (eError == PVRSRV_OK)
		{
			__DtTestPoint(__DtFunc_PVRSRVHandleDeInit, __DtStep_2)
			gpsKernelHandleBase = IMG_NULL;
		}
		else
		{
			__DtTestPoint(__DtFunc_PVRSRVHandleDeInit, __DtStep_3)
			PVR_DPF((PVR_DBG_ERROR, "PVRSRVHandleDeInit: FreeHandleBase failed (%d)", eError));
		}
	}

	__DtTestPoint(__DtFunc_PVRSRVHandleDeInit, __DtStep_4)
	return eError;
}
#else
#endif	
