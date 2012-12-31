/*
 Copyright (C) 2009-2012, Heartland. Data Co.,Ltd.  All Rights Reserved.  
                                                                          
 This software is furnished under a license and may be used and copied    
 only in accordance with the terms of such license and with the inclusion 
 of the above copyright notice. No title to and ownership of the software 
 is transferred.                                                          
 Heartland. Data Co.,Ltd. makes no representation or warranties with      
 respect to the performance of this computer program, and specifically    
 disclaims any responsibility for any damages, special or consequential,  
 connected with the use of this program.                                  
*/

#ifndef	__DT_devicemem_h__
#define	__DT_devicemem_h__

#ifdef	__cplusplus
	#define	__DtEXTERN	extern "C"
#else
	#define	__DtEXTERN	extern
#endif

/* TestPoint MacroCode -----------------------------------------------------*/
#define		__DtBaseAddress		0x60
#define		__DtAllEnable		1
#if ( __DtAllEnable == 1 )
#define		__DtTestPoint(func, step)		__Dt##func##step
#define		__DtTestPointValue(func, step, value, size)		__Dt##func##step(value,size)
#define		__DtTestPointWrite(func, step, value, size)		__Dt##func##step(value,size)
#define		__DtTestPointEventTrigger(func, step, data)		__Dt##func##step(data)
#else
#define		__DtTestPoint(func, step)		
#define		__DtTestPointValue(func, step, value, size)		
#define		__DtTestPointWrite(func, step, value, size)		
#define		__DtTestPointEventTrigger(func, step, data)		
#endif
__DtEXTERN		void	_TP_BusOut( unsigned int addr, unsigned int dat );
__DtEXTERN		void	_TP_MemoryOutput( unsigned int addr, unsigned int dat, void *value, unsigned int size );
__DtEXTERN		void	_TP_WritePoint( unsigned int addr, unsigned int dat, void *value, unsigned int size );
__DtEXTERN		void	_TP_EventTrigger( unsigned int addr, unsigned int dat );

/* TestPoint FuncList ------------------------------------------------------*/
#define		__DtFunc_PVRSRVGetDeviceMemHeapsKM		0
#define		__DtFunc_PVRSRVCreateDeviceMemContextKM		1
#define		__DtFunc_PVRSRVDestroyDeviceMemContextKM		2
#define		__DtFunc_PVRSRVGetDeviceMemHeapInfoKM		3
#define		__DtFunc_AllocDeviceMem		4
#define		__DtFunc_FreeDeviceMem2		5
#define		__DtFunc_FreeDeviceMem		6
#define		__DtFunc_PVRSRVAllocSyncInfoKM		7
#define		__DtFunc_PVRSRVFreeSyncInfoKM		8
#define		__DtFunc_freeWrapped		9
#define		__DtFunc_FreeMemCallBackCommon		10
#define		__DtFunc_FreeDeviceMemCallBack		11
#define		__DtFunc_PVRSRVFreeDeviceMemKM		12
#define		__DtFunc__PVRSRVAllocDeviceMemKM		13
#define		__DtFunc_PVRSRVDissociateDeviceMemKM		14
#define		__DtFunc_PVRSRVGetFreeDeviceMemKM		15
#define		__DtFunc_PVRSRVUnwrapExtMemoryKM		16
#define		__DtFunc_UnwrapExtMemoryCallBack		17
#define		__DtFunc_PVRSRVWrapExtMemoryKM		18
#define		__DtFunc_PVRSRVUnmapDeviceMemoryKM		19
#define		__DtFunc_UnmapDeviceMemoryCallBack		20
#define		__DtFunc_PVRSRVMapDeviceMemoryKM		21
#define		__DtFunc_PVRSRVUnmapDeviceClassMemoryKM		22
#define		__DtFunc_UnmapDeviceClassMemoryCallBack		23
#define		__DtFunc_PVRSRVMapDeviceClassMemoryKM		24

/* TestPoint StepList ------------------------------------------------------*/
#define		__DtStep_0		0
#define		__DtStep_1		1
#define		__DtStep_2		2
#define		__DtStep_3		3
#define		__DtStep_4		4
#define		__DtStep_5		5
#define		__DtStep_6		6
#define		__DtStep_7		7
#define		__DtStep_8		8
#define		__DtStep_9		9
#define		__DtStep_10		10
#define		__DtStep_11		11
#define		__DtStep_12		12
#define		__DtStep_13		13
#define		__DtStep_14		14
#define		__DtStep_15		15
#define		__DtStep_16		16
#define		__DtStep_17		17
#define		__DtStep_18		18
#define		__DtStep_19		19
#define		__DtStep_20		20
#define		__DtStep_21		21

/* TestPoint DisableList ---------------------------------------------------*/
#define	__Dt__DtFunc_PVRSRVGetDeviceMemHeapsKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0000 );
#define	__Dt__DtFunc_PVRSRVGetDeviceMemHeapsKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0001 );
#define	__Dt__DtFunc_PVRSRVGetDeviceMemHeapsKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0002 );
#define	__Dt__DtFunc_PVRSRVGetDeviceMemHeapsKM__DtStep_3	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0003 );
#define	__Dt__DtFunc_PVRSRVGetDeviceMemHeapsKM__DtStep_4	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0004 );
#define	__Dt__DtFunc_PVRSRVGetDeviceMemHeapsKM__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0005 );
#define	__Dt__DtFunc_PVRSRVCreateDeviceMemContextKM__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0006 );
#define	__Dt__DtFunc_PVRSRVCreateDeviceMemContextKM__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0007 );
#define	__Dt__DtFunc_PVRSRVCreateDeviceMemContextKM__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0008 );
#define	__Dt__DtFunc_PVRSRVCreateDeviceMemContextKM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0009 );
#define	__Dt__DtFunc_PVRSRVCreateDeviceMemContextKM__DtStep_4	/*for*/	_TP_BusOut( __DtBaseAddress, 0x000A );
#define	__Dt__DtFunc_PVRSRVCreateDeviceMemContextKM__DtStep_5	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x000B );
#define	__Dt__DtFunc_PVRSRVCreateDeviceMemContextKM__DtStep_6	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x000C );
#define	__Dt__DtFunc_PVRSRVCreateDeviceMemContextKM__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000D );
#define	__Dt__DtFunc_PVRSRVDestroyDeviceMemContextKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x000E );
#define	__Dt__DtFunc_PVRSRVDestroyDeviceMemContextKM__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000F );
#define	__Dt__DtFunc_PVRSRVGetDeviceMemHeapInfoKM__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0010 );
#define	__Dt__DtFunc_PVRSRVGetDeviceMemHeapInfoKM__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0011 );
#define	__Dt__DtFunc_PVRSRVGetDeviceMemHeapInfoKM__DtStep_2	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0012 );
#define	__Dt__DtFunc_PVRSRVGetDeviceMemHeapInfoKM__DtStep_3	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0013 );
#define	__Dt__DtFunc_PVRSRVGetDeviceMemHeapInfoKM__DtStep_4	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0014 );
#define	__Dt__DtFunc_PVRSRVGetDeviceMemHeapInfoKM__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0015 );
#define	__Dt__DtFunc_AllocDeviceMem__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0016 );
#define	__Dt__DtFunc_AllocDeviceMem__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0017 );
#define	__Dt__DtFunc_AllocDeviceMem__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0018 );
#define	__Dt__DtFunc_AllocDeviceMem__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0019 );
#define	__Dt__DtFunc_AllocDeviceMem__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001A );
#define	__Dt__DtFunc_AllocDeviceMem__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001B );
#define	__Dt__DtFunc_FreeDeviceMem2__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x001C );
#define	__Dt__DtFunc_FreeDeviceMem2__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001D );
#define	__Dt__DtFunc_FreeDeviceMem2__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001E );
#define	__Dt__DtFunc_FreeDeviceMem2__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001F );
#define	__Dt__DtFunc_FreeDeviceMem__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0020 );
#define	__Dt__DtFunc_FreeDeviceMem__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0021 );
#define	__Dt__DtFunc_FreeDeviceMem__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0022 );
#define	__Dt__DtFunc_FreeDeviceMem__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0023 );
#define	__Dt__DtFunc_PVRSRVAllocSyncInfoKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0024 );
#define	__Dt__DtFunc_PVRSRVAllocSyncInfoKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0025 );
#define	__Dt__DtFunc_PVRSRVAllocSyncInfoKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0026 );
#define	__Dt__DtFunc_PVRSRVAllocSyncInfoKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0027 );
#define	__Dt__DtFunc_PVRSRVAllocSyncInfoKM__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0028 );
#define	__Dt__DtFunc_PVRSRVAllocSyncInfoKM__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002A );
#define	__Dt__DtFunc_PVRSRVFreeSyncInfoKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x002B );
#define	__Dt__DtFunc_PVRSRVFreeSyncInfoKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002C );
#define	__Dt__DtFunc_PVRSRVFreeSyncInfoKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002D );
#define	__Dt__DtFunc_PVRSRVFreeSyncInfoKM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002E );
#define	__Dt__DtFunc_freeWrapped__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x002F );
#define	__Dt__DtFunc_freeWrapped__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0031 );
#define	__Dt__DtFunc_freeWrapped__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0032 );
#define	__Dt__DtFunc_freeWrapped__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0033 );
#define	__Dt__DtFunc_FreeMemCallBackCommon__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0034 );
#define	__Dt__DtFunc_FreeMemCallBackCommon__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0035 );
#define	__Dt__DtFunc_FreeMemCallBackCommon__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0036 );
#define	__Dt__DtFunc_FreeMemCallBackCommon__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0037 );
#define	__Dt__DtFunc_FreeMemCallBackCommon__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0038 );
#define	__Dt__DtFunc_FreeMemCallBackCommon__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0039 );
#define	__Dt__DtFunc_FreeMemCallBackCommon__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003A );
#define	__Dt__DtFunc_FreeMemCallBackCommon__DtStep_7	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x003B );
#define	__Dt__DtFunc_FreeMemCallBackCommon__DtStep_8	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x003C );
#define	__Dt__DtFunc_FreeMemCallBackCommon__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003D );
#define	__Dt__DtFunc_FreeMemCallBackCommon__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003E );
#define	__Dt__DtFunc_FreeMemCallBackCommon__DtStep_11	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x003F );
#define	__Dt__DtFunc_FreeMemCallBackCommon__DtStep_12	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0040 );
#define	__Dt__DtFunc_FreeMemCallBackCommon__DtStep_13	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0041 );
#define	__Dt__DtFunc_FreeDeviceMemCallBack__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0042 );
#define	__Dt__DtFunc_PVRSRVFreeDeviceMemKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0043 );
#define	__Dt__DtFunc_PVRSRVFreeDeviceMemKM__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0044 );
#define	__Dt__DtFunc_PVRSRVFreeDeviceMemKM__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0045 );
#define	__Dt__DtFunc_PVRSRVFreeDeviceMemKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0046 );
#define	__Dt__DtFunc_PVRSRVFreeDeviceMemKM__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0047 );
#define	__Dt__DtFunc__PVRSRVAllocDeviceMemKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0048 );
#define	__Dt__DtFunc__PVRSRVAllocDeviceMemKM__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0049 );
#define	__Dt__DtFunc__PVRSRVAllocDeviceMemKM__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004A );
#define	__Dt__DtFunc__PVRSRVAllocDeviceMemKM__DtStep_3	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004B );
#define	__Dt__DtFunc__PVRSRVAllocDeviceMemKM__DtStep_4	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004C );
#define	__Dt__DtFunc__PVRSRVAllocDeviceMemKM__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004D );
#define	__Dt__DtFunc__PVRSRVAllocDeviceMemKM__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004E );
#define	__Dt__DtFunc__PVRSRVAllocDeviceMemKM__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004F );
#define	__Dt__DtFunc__PVRSRVAllocDeviceMemKM__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0050 );
#define	__Dt__DtFunc__PVRSRVAllocDeviceMemKM__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0051 );
#define	__Dt__DtFunc__PVRSRVAllocDeviceMemKM__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0052 );
#define	__Dt__DtFunc__PVRSRVAllocDeviceMemKM__DtStep_11	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0053 );
#define	__Dt__DtFunc__PVRSRVAllocDeviceMemKM__DtStep_12	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0054 );
#define	__Dt__DtFunc_PVRSRVDissociateDeviceMemKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0055 );
#define	__Dt__DtFunc_PVRSRVDissociateDeviceMemKM__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0056 );
#define	__Dt__DtFunc_PVRSRVDissociateDeviceMemKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0057 );
#define	__Dt__DtFunc_PVRSRVGetFreeDeviceMemKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0058 );
#define	__Dt__DtFunc_PVRSRVGetFreeDeviceMemKM__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0059 );
#define	__Dt__DtFunc_PVRSRVUnwrapExtMemoryKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x005A );
#define	__Dt__DtFunc_PVRSRVUnwrapExtMemoryKM__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005B );
#define	__Dt__DtFunc_PVRSRVUnwrapExtMemoryKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005C );
#define	__Dt__DtFunc_UnwrapExtMemoryCallBack__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005D );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x005E );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005F );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0060 );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0061 );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryKM__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0062 );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryKM__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0063 );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryKM__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0064 );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryKM__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0065 );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryKM__DtStep_8	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0066 );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryKM__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0067 );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryKM__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0068 );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryKM__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0069 );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryKM__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006A );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryKM__DtStep_13	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006B );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryKM__DtStep_14	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006C );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryKM__DtStep_15	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006D );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryKM__DtStep_16	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006E );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryKM__DtStep_17	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006F );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryKM__DtStep_18	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0070 );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryKM__DtStep_19	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0071 );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryKM__DtStep_20	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0072 );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryKM__DtStep_21	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0073 );
#define	__Dt__DtFunc_PVRSRVUnmapDeviceMemoryKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0074 );
#define	__Dt__DtFunc_PVRSRVUnmapDeviceMemoryKM__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0075 );
#define	__Dt__DtFunc_PVRSRVUnmapDeviceMemoryKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0076 );
#define	__Dt__DtFunc_UnmapDeviceMemoryCallBack__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0077 );
#define	__Dt__DtFunc_UnmapDeviceMemoryCallBack__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0078 );
#define	__Dt__DtFunc_UnmapDeviceMemoryCallBack__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0079 );
#define	__Dt__DtFunc_UnmapDeviceMemoryCallBack__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007A );
#define	__Dt__DtFunc_UnmapDeviceMemoryCallBack__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007B );
#define	__Dt__DtFunc_UnmapDeviceMemoryCallBack__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007C );
#define	__Dt__DtFunc_UnmapDeviceMemoryCallBack__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007D );
#define	__Dt__DtFunc_UnmapDeviceMemoryCallBack__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007E );
#define	__Dt__DtFunc_UnmapDeviceMemoryCallBack__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007F );
#define	__Dt__DtFunc_PVRSRVMapDeviceMemoryKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0081 );
#define	__Dt__DtFunc_PVRSRVMapDeviceMemoryKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0082 );
#define	__Dt__DtFunc_PVRSRVMapDeviceMemoryKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0083 );
#define	__Dt__DtFunc_PVRSRVMapDeviceMemoryKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0084 );
#define	__Dt__DtFunc_PVRSRVMapDeviceMemoryKM__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0085 );
#define	__Dt__DtFunc_PVRSRVMapDeviceMemoryKM__DtStep_5	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0086 );
#define	__Dt__DtFunc_PVRSRVMapDeviceMemoryKM__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0087 );
#define	__Dt__DtFunc_PVRSRVMapDeviceMemoryKM__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0088 );
#define	__Dt__DtFunc_PVRSRVMapDeviceMemoryKM__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0089 );
#define	__Dt__DtFunc_PVRSRVMapDeviceMemoryKM__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x008A );
#define	__Dt__DtFunc_PVRSRVMapDeviceMemoryKM__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008B );
#define	__Dt__DtFunc_PVRSRVMapDeviceMemoryKM__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008C );
#define	__Dt__DtFunc_PVRSRVMapDeviceMemoryKM__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008D );
#define	__Dt__DtFunc_PVRSRVMapDeviceMemoryKM__DtStep_13	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x008E );
#define	__Dt__DtFunc_PVRSRVUnmapDeviceClassMemoryKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x008F );
#define	__Dt__DtFunc_PVRSRVUnmapDeviceClassMemoryKM__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0091 );
#define	__Dt__DtFunc_PVRSRVUnmapDeviceClassMemoryKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0092 );
#define	__Dt__DtFunc_UnmapDeviceClassMemoryCallBack__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0093 );
#define	__Dt__DtFunc_UnmapDeviceClassMemoryCallBack__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0094 );
#define	__Dt__DtFunc_UnmapDeviceClassMemoryCallBack__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0095 );
#define	__Dt__DtFunc_UnmapDeviceClassMemoryCallBack__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0096 );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0097 );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0098 );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0099 );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009A );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryKM__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x009B );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryKM__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009C );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryKM__DtStep_6	/*for*/	_TP_BusOut( __DtBaseAddress, 0x009D );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryKM__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009E );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryKM__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009F );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryKM__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A1 );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryKM__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A2 );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryKM__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A3 );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryKM__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A4 );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryKM__DtStep_13	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A5 );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryKM__DtStep_14	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A6 );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryKM__DtStep_15	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A7 );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryKM__DtStep_16	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A8 );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryKM__DtStep_17	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A9 );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryKM__DtStep_18	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00AA );

#endif	/* __DT_devicemem_h__ */

