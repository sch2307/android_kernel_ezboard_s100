#include	"DT_lists.h"	/* For DynamicTracer-TestPoint */
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

#include "lists.h"
#include "services_headers.h"

IMPLEMENT_LIST_ANY_VA(BM_HEAP)
IMPLEMENT_LIST_ANY_2(BM_HEAP, PVRSRV_ERROR, PVRSRV_OK)
IMPLEMENT_LIST_ANY_VA_2(BM_HEAP, PVRSRV_ERROR, PVRSRV_OK)
IMPLEMENT_LIST_FOR_EACH_VA(BM_HEAP)
IMPLEMENT_LIST_REMOVE(BM_HEAP)
IMPLEMENT_LIST_INSERT(BM_HEAP)

IMPLEMENT_LIST_ANY_VA(BM_CONTEXT)
IMPLEMENT_LIST_ANY_VA_2(BM_CONTEXT, IMG_HANDLE, IMG_NULL)
IMPLEMENT_LIST_ANY_VA_2(BM_CONTEXT, PVRSRV_ERROR, PVRSRV_OK)
IMPLEMENT_LIST_FOR_EACH(BM_CONTEXT)
IMPLEMENT_LIST_REMOVE(BM_CONTEXT)
IMPLEMENT_LIST_INSERT(BM_CONTEXT)

IMPLEMENT_LIST_ANY_2(PVRSRV_DEVICE_NODE, PVRSRV_ERROR, PVRSRV_OK)
IMPLEMENT_LIST_ANY_VA(PVRSRV_DEVICE_NODE)
IMPLEMENT_LIST_ANY_VA_2(PVRSRV_DEVICE_NODE, PVRSRV_ERROR, PVRSRV_OK)
IMPLEMENT_LIST_FOR_EACH(PVRSRV_DEVICE_NODE)
IMPLEMENT_LIST_FOR_EACH_VA(PVRSRV_DEVICE_NODE)
IMPLEMENT_LIST_INSERT(PVRSRV_DEVICE_NODE)
IMPLEMENT_LIST_REMOVE(PVRSRV_DEVICE_NODE)

IMPLEMENT_LIST_ANY_VA(PVRSRV_POWER_DEV)
IMPLEMENT_LIST_ANY_VA_2(PVRSRV_POWER_DEV, PVRSRV_ERROR, PVRSRV_OK)
IMPLEMENT_LIST_INSERT(PVRSRV_POWER_DEV)
IMPLEMENT_LIST_REMOVE(PVRSRV_POWER_DEV)


IMG_VOID* MatchDeviceKM_AnyVaCb(PVRSRV_DEVICE_NODE* psDeviceNode, va_list va)
{
	IMG_UINT32 ui32DevIndex;
	IMG_BOOL bIgnoreClass;
	PVRSRV_DEVICE_CLASS eDevClass;

	__DtTestPoint(__DtFunc_MatchDeviceKM_AnyVaCb, __DtStep_0)
	ui32DevIndex = va_arg(va, IMG_UINT32);
	bIgnoreClass = va_arg(va, IMG_BOOL);
	if (!bIgnoreClass)
	{
		__DtTestPoint(__DtFunc_MatchDeviceKM_AnyVaCb, __DtStep_1)
		eDevClass = va_arg(va, PVRSRV_DEVICE_CLASS);
	}
	else
	{
		

		__DtTestPoint(__DtFunc_MatchDeviceKM_AnyVaCb, __DtStep_2)
		eDevClass = PVRSRV_DEVICE_CLASS_FORCE_I32;
	}

	if ((bIgnoreClass || psDeviceNode->sDevId.eDeviceClass == eDevClass) &&
		psDeviceNode->sDevId.ui32DeviceIndex == ui32DevIndex)
	{
		__DtTestPoint(__DtFunc_MatchDeviceKM_AnyVaCb, __DtStep_3)
		return psDeviceNode;
	}
	__DtTestPoint(__DtFunc_MatchDeviceKM_AnyVaCb, __DtStep_4)
	return IMG_NULL;
}

IMG_VOID* MatchPowerDeviceIndex_AnyVaCb(PVRSRV_POWER_DEV *psPowerDev, va_list va)
{
	IMG_UINT32 ui32DeviceIndex;

	__DtTestPoint(__DtFunc_MatchPowerDeviceIndex_AnyVaCb, __DtStep_0)
	ui32DeviceIndex = va_arg(va, IMG_UINT32);

	if (psPowerDev->ui32DeviceIndex == ui32DeviceIndex)
	{
		__DtTestPoint(__DtFunc_MatchPowerDeviceIndex_AnyVaCb, __DtStep_1)
		return psPowerDev;
	}
	else
	{
		__DtTestPoint(__DtFunc_MatchPowerDeviceIndex_AnyVaCb, __DtStep_2)
		return IMG_NULL;
	}
	__DtTestPoint(__DtFunc_MatchPowerDeviceIndex_AnyVaCb, __DtStep_3)
}
