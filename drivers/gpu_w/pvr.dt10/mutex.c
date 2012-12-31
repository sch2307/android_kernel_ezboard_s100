#include	"DT_mutex.h"	/* For DynamicTracer-TestPoint */
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

#include <linux/version.h>
#include <linux/errno.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,15))
#include <linux/mutex.h>
#else
#include <asm/semaphore.h>
#endif
#include <linux/module.h>

#include <img_defs.h>
#include <services.h>

#include "mutex.h"


#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,15))

IMG_VOID LinuxInitMutex(PVRSRV_LINUX_MUTEX *psPVRSRVMutex)
{
    __DtTestPoint(__DtFunc_LinuxInitMutex, __DtStep_0)
    mutex_init(psPVRSRVMutex);
	__DtTestPoint(__DtFunc_LinuxInitMutex, __DtStep_1)
}

IMG_VOID LinuxLockMutex(PVRSRV_LINUX_MUTEX *psPVRSRVMutex)
{
    __DtTestPoint(__DtFunc_LinuxLockMutex, __DtStep_0)
    mutex_lock(psPVRSRVMutex);
	__DtTestPoint(__DtFunc_LinuxLockMutex, __DtStep_1)
}

PVRSRV_ERROR LinuxLockMutexInterruptible(PVRSRV_LINUX_MUTEX *psPVRSRVMutex)
{
    __DtTestPoint(__DtFunc_LinuxLockMutexInterruptible, __DtStep_0)
    if(mutex_lock_interruptible(psPVRSRVMutex) == -EINTR)
    {
        __DtTestPoint(__DtFunc_LinuxLockMutexInterruptible, __DtStep_1)
        return PVRSRV_ERROR_MUTEX_INTERRUPTIBLE_ERROR;
    }
    else
    {
        __DtTestPoint(__DtFunc_LinuxLockMutexInterruptible, __DtStep_2)
        return PVRSRV_OK;
    }
	__DtTestPoint(__DtFunc_LinuxLockMutexInterruptible, __DtStep_3)
}

IMG_INT32 LinuxTryLockMutex(PVRSRV_LINUX_MUTEX *psPVRSRVMutex)
{
    __DtTestPoint(__DtFunc_LinuxTryLockMutex, __DtStep_0)
    return mutex_trylock(psPVRSRVMutex);
}

IMG_VOID LinuxUnLockMutex(PVRSRV_LINUX_MUTEX *psPVRSRVMutex)
{
    __DtTestPoint(__DtFunc_LinuxUnLockMutex, __DtStep_0)
    mutex_unlock(psPVRSRVMutex);
	__DtTestPoint(__DtFunc_LinuxUnLockMutex, __DtStep_1)
}

IMG_BOOL LinuxIsLockedMutex(PVRSRV_LINUX_MUTEX *psPVRSRVMutex)
{
    __DtTestPoint(__DtFunc_LinuxIsLockedMutex, __DtStep_0)
    return (mutex_is_locked(psPVRSRVMutex)) ? IMG_TRUE : IMG_FALSE;
}


#else 


IMG_VOID LinuxInitMutex(PVRSRV_LINUX_MUTEX *psPVRSRVMutex)
{
    __DtTestPoint(__DtFunc_LinuxInitMutex, __DtStep_2)
    init_MUTEX(&psPVRSRVMutex->sSemaphore);
    atomic_set(&psPVRSRVMutex->Count, 0);
	__DtTestPoint(__DtFunc_LinuxInitMutex, __DtStep_3)
}

IMG_VOID LinuxLockMutex(PVRSRV_LINUX_MUTEX *psPVRSRVMutex)
{
    __DtTestPoint(__DtFunc_LinuxLockMutex, __DtStep_2)
    down(&psPVRSRVMutex->sSemaphore);
    atomic_dec(&psPVRSRVMutex->Count);
	__DtTestPoint(__DtFunc_LinuxLockMutex, __DtStep_3)
}

PVRSRV_ERROR LinuxLockMutexInterruptible(PVRSRV_LINUX_MUTEX *psPVRSRVMutex)
{
    __DtTestPoint(__DtFunc_LinuxLockMutexInterruptible, __DtStep_4)
    if(down_interruptible(&psPVRSRVMutex->sSemaphore) == -EINTR)
    {
        
        __DtTestPoint(__DtFunc_LinuxLockMutexInterruptible, __DtStep_5)
        return PVRSRV_ERROR_MUTEX_INTERRUPTIBLE_ERROR;
    }else{
        __DtTestPoint(__DtFunc_LinuxLockMutexInterruptible, __DtStep_6)
        atomic_dec(&psPVRSRVMutex->Count);
        __DtTestPoint(__DtFunc_LinuxLockMutexInterruptible, __DtStep_7)
        return PVRSRV_OK;
    }
	__DtTestPoint(__DtFunc_LinuxLockMutexInterruptible, __DtStep_8)
}

IMG_INT32 LinuxTryLockMutex(PVRSRV_LINUX_MUTEX *psPVRSRVMutex)
{
    IMG_INT32 Status = down_trylock(&psPVRSRVMutex->sSemaphore);
    __DtTestPoint(__DtFunc_LinuxTryLockMutex, __DtStep_1)
    if(Status == 0)
    {
        __DtTestPoint(__DtFunc_LinuxTryLockMutex, __DtStep_2)
        atomic_dec(&psPVRSRVMutex->Count);
    }

    __DtTestPoint(__DtFunc_LinuxTryLockMutex, __DtStep_3)
    return Status;
}

IMG_VOID LinuxUnLockMutex(PVRSRV_LINUX_MUTEX *psPVRSRVMutex)
{
    __DtTestPoint(__DtFunc_LinuxUnLockMutex, __DtStep_2)
    atomic_inc(&psPVRSRVMutex->Count);
    up(&psPVRSRVMutex->sSemaphore);
	__DtTestPoint(__DtFunc_LinuxUnLockMutex, __DtStep_3)
}

IMG_BOOL LinuxIsLockedMutex(PVRSRV_LINUX_MUTEX *psPVRSRVMutex)
{
    IMG_INT32 iCount;
    
    __DtTestPoint(__DtFunc_LinuxIsLockedMutex, __DtStep_1)
    iCount = atomic_read(&psPVRSRVMutex->Count);

    __DtTestPoint(__DtFunc_LinuxIsLockedMutex, __DtStep_2)
    return (IMG_BOOL)iCount;
}

#endif 

