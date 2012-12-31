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

#ifndef	__DT_osfunc_h__
#define	__DT_osfunc_h__

#ifdef	__cplusplus
	#define	__DtEXTERN	extern "C"
#else
	#define	__DtEXTERN	extern
#endif

/* TestPoint MacroCode -----------------------------------------------------*/
#define		__DtBaseAddress		0x160
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
#define		__DtFunc_OSAllocMem_Impl		0
#define		__DtFunc_is_vmalloc_addr		1
#define		__DtFunc_OSFreeMem_Impl		2
#define		__DtFunc_OSAllocPages_Impl		3
#define		__DtFunc_OSFreePages		4
#define		__DtFunc_OSGetSubMemHandle		5
#define		__DtFunc_OSReleaseSubMemHandle		6
#define		__DtFunc_OSMemHandleToCpuPAddr		7
#define		__DtFunc_OSMemCopy		8
#define		__DtFunc_OSMemSet		9
#define		__DtFunc_OSStringCopy		10
#define		__DtFunc_OSSNPrintf		11
#define		__DtFunc_OSBreakResourceLock		12
#define		__DtFunc_OSCreateResource		13
#define		__DtFunc_OSDestroyResource		14
#define		__DtFunc_OSInitEnvData		15
#define		__DtFunc_OSDeInitEnvData		16
#define		__DtFunc_OSReleaseThreadQuanta		17
#define		__DtFunc_OSClockus		18
#define		__DtFunc_OSWaitus		19
#define		__DtFunc_OSSleepms		20
#define		__DtFunc_OSGetCurrentProcessIDKM		21
#define		__DtFunc_OSGetPageSize		22
#define		__DtFunc_DeviceISRWrapper		23
#define		__DtFunc_SystemISRWrapper		24
#define		__DtFunc_OSInstallDeviceLISR		25
#define		__DtFunc_OSUninstallDeviceLISR		26
#define		__DtFunc_OSInstallSystemLISR		27
#define		__DtFunc_OSUninstallSystemLISR		28
#define		__DtFunc_MISRWrapper		29
#define		__DtFunc_OSInstallMISR		30
#define		__DtFunc_OSUninstallMISR		31
#define		__DtFunc_OSScheduleMISR		32
#define		__DtFunc_OSPanic		41
#define		__DtFunc_OSLockResource		42
#define		__DtFunc_OSUnlockResource		43
#define		__DtFunc_OSIsResourceLocked		44
#define		__DtFunc_OSMapLinToCPUPhys		45
#define		__DtFunc_OSMapPhysToLin		46
#define		__DtFunc_OSUnMapPhysToLin		47
#define		__DtFunc_RegisterExternalMem		48
#define		__DtFunc_OSRegisterMem		49
#define		__DtFunc_OSRegisterDiscontigMem		50
#define		__DtFunc_OSUnRegisterMem		51
#define		__DtFunc_OSUnRegisterDiscontigMem		52
#define		__DtFunc_OSReservePhys		53
#define		__DtFunc_OSUnReservePhys		54
#define		__DtFunc_OSBaseAllocContigMemory		55
#define		__DtFunc_OSBaseFreeContigMemory		56
#define		__DtFunc_OSReadHWReg		57
#define		__DtFunc_OSWriteHWReg		58
#define		__DtFunc_OSPCISetDev		59
#define		__DtFunc_OSPCIAcquireDev		60
#define		__DtFunc_OSPCIIRQ		61
#define		__DtFunc_OSPCIAddrRangeFunc		62
#define		__DtFunc_OSPCIAddrRangeLen		63
#define		__DtFunc_OSPCIAddrRangeStart		64
#define		__DtFunc_OSPCIAddrRangeEnd		65
#define		__DtFunc_OSPCIRequestAddrRange		66
#define		__DtFunc_OSPCIReleaseAddrRange		67
#define		__DtFunc_OSPCIReleaseDev		68
#define		__DtFunc_OSPCISuspendDev		69
#define		__DtFunc_OSPCIResumeDev		70
#define		__DtFunc_OSTimerCallbackBody		71
#define		__DtFunc_OSTimerCallbackWrapper		72
#define		__DtFunc_OSTimerWorkQueueCallBack		73
#define		__DtFunc_OSAddTimer		74
#define		__DtFunc_GetTimerStructure		75
#define		__DtFunc_OSRemoveTimer		76
#define		__DtFunc_OSEnableTimer		77
#define		__DtFunc_OSDisableTimer		78
#define		__DtFunc_OSEventObjectCreate		79
#define		__DtFunc_OSEventObjectDestroy		80
#define		__DtFunc_OSEventObjectWait		81
#define		__DtFunc_OSEventObjectOpen		82
#define		__DtFunc_OSEventObjectClose		83
#define		__DtFunc_OSEventObjectSignal		84
#define		__DtFunc_OSProcHasPrivSrvInit		85
#define		__DtFunc_OSCopyToUser		86
#define		__DtFunc_OSCopyFromUser		87
#define		__DtFunc_OSAccessOK		88
#define		__DtFunc_CPUVAddrToPFN		89
#define		__DtFunc_OSReleasePhysPageAddr		90
#define		__DtFunc_OSAcquirePhysPageAddr		91
#define		__DtFunc_VMallocAreaToPhys		92
#define		__DtFunc_ExternalKVAreaToPhys		93
#define		__DtFunc_AllocPagesAreaToPhys		94
#define		__DtFunc_FindMMapBaseVAddr		95
#define		__DtFunc_CheckExecuteCacheOp		96
#define		__DtFunc_per_cpu_cache_flush		97
#define		__DtFunc_x86_flush_cache_range		98
#define		__DtFunc_OSCleanCPUCacheKM		99
#define		__DtFunc_OSFlushCPUCacheKM		100
#define		__DtFunc_OSFlushCPUCacheRangeKM		101
#define		__DtFunc_OSCleanCPUCacheRangeKM		102
#define		__DtFunc_OSInvalidateCPUCacheRangeKM		103
#define		__DtFunc__dmac_inv_range		107
#define		__DtFunc__dmac_clean_range		108
#define		__DtFunc_PVROSFuncInit		117
#define		__DtFunc_PVROSFuncDeInit		118

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
#define		__DtStep_22		22
#define		__DtStep_23		23
#define		__DtStep_24		24
#define		__DtStep_25		25
#define		__DtStep_26		26
#define		__DtStep_27		27

/* TestPoint DisableList ---------------------------------------------------*/
#define	__Dt__DtFunc_OSAllocMem_Impl__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0000 );
#define	__Dt__DtFunc_OSAllocMem_Impl__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0001 );
#define	__Dt__DtFunc_OSAllocMem_Impl__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0002 );
#define	__Dt__DtFunc_OSAllocMem_Impl__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0003 );
#define	__Dt__DtFunc_OSAllocMem_Impl__DtStep_4	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0004 );
#define	__Dt__DtFunc_OSAllocMem_Impl__DtStep_5	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0005 );
#define	__Dt__DtFunc_OSAllocMem_Impl__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0006 );
#define	__Dt__DtFunc_is_vmalloc_addr__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0007 );
#define	__Dt__DtFunc_OSFreeMem_Impl__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0008 );
#define	__Dt__DtFunc_OSFreeMem_Impl__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0009 );
#define	__Dt__DtFunc_OSAllocPages_Impl__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x000A );
#define	__Dt__DtFunc_OSAllocPages_Impl__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000B );
#define	__Dt__DtFunc_OSAllocPages_Impl__DtStep_2	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x000C );
#define	__Dt__DtFunc_OSAllocPages_Impl__DtStep_3	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000D );
#define	__Dt__DtFunc_OSAllocPages_Impl__DtStep_4	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x000E );
#define	__Dt__DtFunc_OSAllocPages_Impl__DtStep_5	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000F );
#define	__Dt__DtFunc_OSAllocPages_Impl__DtStep_6	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0010 );
#define	__Dt__DtFunc_OSAllocPages_Impl__DtStep_7	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0011 );
#define	__Dt__DtFunc_OSAllocPages_Impl__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0012 );
#define	__Dt__DtFunc_OSAllocPages_Impl__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0013 );
#define	__Dt__DtFunc_OSFreePages__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0014 );
#define	__Dt__DtFunc_OSFreePages__DtStep_1	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0015 );
#define	__Dt__DtFunc_OSFreePages__DtStep_2	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0016 );
#define	__Dt__DtFunc_OSFreePages__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0017 );
#define	__Dt__DtFunc_OSFreePages__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0018 );
#define	__Dt__DtFunc_OSFreePages__DtStep_5	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0019 );
#define	__Dt__DtFunc_OSFreePages__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001A );
#define	__Dt__DtFunc_OSFreePages__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001B );
#define	__Dt__DtFunc_OSGetSubMemHandle__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x001C );
#define	__Dt__DtFunc_OSGetSubMemHandle__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001D );
#define	__Dt__DtFunc_OSGetSubMemHandle__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001E );
#define	__Dt__DtFunc_OSGetSubMemHandle__DtStep_3	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001F );
#define	__Dt__DtFunc_OSGetSubMemHandle__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0020 );
#define	__Dt__DtFunc_OSGetSubMemHandle__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0021 );
#define	__Dt__DtFunc_OSGetSubMemHandle__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0022 );
#define	__Dt__DtFunc_OSReleaseSubMemHandle__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0023 );
#define	__Dt__DtFunc_OSReleaseSubMemHandle__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0024 );
#define	__Dt__DtFunc_OSReleaseSubMemHandle__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0025 );
#define	__Dt__DtFunc_OSReleaseSubMemHandle__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0026 );
#define	__Dt__DtFunc_OSMemHandleToCpuPAddr__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0027 );
#define	__Dt__DtFunc_OSMemHandleToCpuPAddr__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0028 );
#define	__Dt__DtFunc_OSMemCopy__DtStep_0	/*for*/	_TP_BusOut( __DtBaseAddress, 0x002A );
#define	__Dt__DtFunc_OSMemCopy__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002B );
#define	__Dt__DtFunc_OSMemSet__DtStep_0	/*for*/	_TP_BusOut( __DtBaseAddress, 0x002C );
#define	__Dt__DtFunc_OSMemSet__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002D );
#define	__Dt__DtFunc_OSStringCopy__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002E );
#define	__Dt__DtFunc_OSSNPrintf__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x002F );
#define	__Dt__DtFunc_OSSNPrintf__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0031 );
#define	__Dt__DtFunc_OSBreakResourceLock__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0032 );
#define	__Dt__DtFunc_OSBreakResourceLock__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0033 );
#define	__Dt__DtFunc_OSBreakResourceLock__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0034 );
#define	__Dt__DtFunc_OSBreakResourceLock__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0035 );
#define	__Dt__DtFunc_OSBreakResourceLock__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0036 );
#define	__Dt__DtFunc_OSBreakResourceLock__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0037 );
#define	__Dt__DtFunc_OSCreateResource__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0038 );
#define	__Dt__DtFunc_OSCreateResource__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0039 );
#define	__Dt__DtFunc_OSDestroyResource__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x003A );
#define	__Dt__DtFunc_OSDestroyResource__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003B );
#define	__Dt__DtFunc_OSInitEnvData__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x003C );
#define	__Dt__DtFunc_OSInitEnvData__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003D );
#define	__Dt__DtFunc_OSInitEnvData__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003E );
#define	__Dt__DtFunc_OSInitEnvData__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003F );
#define	__Dt__DtFunc_OSInitEnvData__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0040 );
#define	__Dt__DtFunc_OSDeInitEnvData__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0041 );
#define	__Dt__DtFunc_OSDeInitEnvData__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0042 );
#define	__Dt__DtFunc_OSReleaseThreadQuanta__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0043 );
#define	__Dt__DtFunc_OSReleaseThreadQuanta__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0044 );
#define	__Dt__DtFunc_OSClockus__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0045 );
#define	__Dt__DtFunc_OSClockus__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0046 );
#define	__Dt__DtFunc_OSWaitus__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0047 );
#define	__Dt__DtFunc_OSWaitus__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0048 );
#define	__Dt__DtFunc_OSSleepms__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0049 );
#define	__Dt__DtFunc_OSSleepms__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004A );
#define	__Dt__DtFunc_OSGetCurrentProcessIDKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x004B );
#define	__Dt__DtFunc_OSGetCurrentProcessIDKM__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004C );
#define	__Dt__DtFunc_OSGetPageSize__DtStep_0	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004D );
#define	__Dt__DtFunc_DeviceISRWrapper__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x004E );
#define	__Dt__DtFunc_DeviceISRWrapper__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004F );
#define	__Dt__DtFunc_DeviceISRWrapper__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0050 );
#define	__Dt__DtFunc_SystemISRWrapper__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0051 );
#define	__Dt__DtFunc_SystemISRWrapper__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0052 );
#define	__Dt__DtFunc_SystemISRWrapper__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0053 );
#define	__Dt__DtFunc_OSInstallDeviceLISR__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0054 );
#define	__Dt__DtFunc_OSInstallDeviceLISR__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0055 );
#define	__Dt__DtFunc_OSInstallDeviceLISR__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0056 );
#define	__Dt__DtFunc_OSInstallDeviceLISR__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0057 );
#define	__Dt__DtFunc_OSInstallDeviceLISR__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0058 );
#define	__Dt__DtFunc_OSUninstallDeviceLISR__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0059 );
#define	__Dt__DtFunc_OSUninstallDeviceLISR__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005A );
#define	__Dt__DtFunc_OSUninstallDeviceLISR__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005B );
#define	__Dt__DtFunc_OSUninstallDeviceLISR__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005C );
#define	__Dt__DtFunc_OSInstallSystemLISR__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x005D );
#define	__Dt__DtFunc_OSInstallSystemLISR__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005E );
#define	__Dt__DtFunc_OSInstallSystemLISR__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005F );
#define	__Dt__DtFunc_OSInstallSystemLISR__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0060 );
#define	__Dt__DtFunc_OSInstallSystemLISR__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0061 );
#define	__Dt__DtFunc_OSUninstallSystemLISR__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0062 );
#define	__Dt__DtFunc_OSUninstallSystemLISR__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0063 );
#define	__Dt__DtFunc_OSUninstallSystemLISR__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0064 );
#define	__Dt__DtFunc_OSUninstallSystemLISR__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0065 );
#define	__Dt__DtFunc_MISRWrapper__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0066 );
#define	__Dt__DtFunc_MISRWrapper__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0067 );
#define	__Dt__DtFunc_OSInstallMISR__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0068 );
#define	__Dt__DtFunc_OSInstallMISR__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0069 );
#define	__Dt__DtFunc_OSInstallMISR__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006A );
#define	__Dt__DtFunc_OSInstallMISR__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006B );
#define	__Dt__DtFunc_OSInstallMISR__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006C );
#define	__Dt__DtFunc_OSInstallMISR__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006D );
#define	__Dt__DtFunc_OSUninstallMISR__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x006E );
#define	__Dt__DtFunc_OSUninstallMISR__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006F );
#define	__Dt__DtFunc_OSUninstallMISR__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0070 );
#define	__Dt__DtFunc_OSUninstallMISR__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0071 );
#define	__Dt__DtFunc_OSScheduleMISR__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0072 );
#define	__Dt__DtFunc_OSScheduleMISR__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0073 );
#define	__Dt__DtFunc_OSScheduleMISR__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0074 );
#define	__Dt__DtFunc_MISRWrapper__DtStep_2	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0075 );
#define	__Dt__DtFunc_MISRWrapper__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0076 );
#define	__Dt__DtFunc_OSInstallMISR__DtStep_6	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0077 );
#define	__Dt__DtFunc_OSInstallMISR__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0078 );
#define	__Dt__DtFunc_OSInstallMISR__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0079 );
#define	__Dt__DtFunc_OSInstallMISR__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007A );
#define	__Dt__DtFunc_OSUninstallMISR__DtStep_4	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x007B );
#define	__Dt__DtFunc_OSUninstallMISR__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007C );
#define	__Dt__DtFunc_OSUninstallMISR__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007D );
#define	__Dt__DtFunc_OSUninstallMISR__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007E );
#define	__Dt__DtFunc_OSScheduleMISR__DtStep_3	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x007F );
#define	__Dt__DtFunc_OSScheduleMISR__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0081 );
#define	__Dt__DtFunc_OSScheduleMISR__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0082 );
#define	__Dt__DtFunc_MISRWrapper__DtStep_4	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0083 );
#define	__Dt__DtFunc_MISRWrapper__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0084 );
#define	__Dt__DtFunc_OSInstallMISR__DtStep_10	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0085 );
#define	__Dt__DtFunc_OSInstallMISR__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0086 );
#define	__Dt__DtFunc_OSInstallMISR__DtStep_12	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0087 );
#define	__Dt__DtFunc_OSInstallMISR__DtStep_13	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0088 );
#define	__Dt__DtFunc_OSUninstallMISR__DtStep_8	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0089 );
#define	__Dt__DtFunc_OSUninstallMISR__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008A );
#define	__Dt__DtFunc_OSUninstallMISR__DtStep_10	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x008B );
#define	__Dt__DtFunc_OSUninstallMISR__DtStep_11	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x008C );
#define	__Dt__DtFunc_OSScheduleMISR__DtStep_6	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x008D );
#define	__Dt__DtFunc_OSScheduleMISR__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008E );
#define	__Dt__DtFunc_OSScheduleMISR__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x008F );
#define	__Dt__DtFunc_OSPanic__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0091 );
#define	__Dt__DtFunc_OSPanic__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0092 );
#define	__Dt__DtFunc_OSLockResource__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0093 );
#define	__Dt__DtFunc_OSLockResource__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0094 );
#define	__Dt__DtFunc_OSUnlockResource__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0095 );
#define	__Dt__DtFunc_OSUnlockResource__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0096 );
#define	__Dt__DtFunc_OSUnlockResource__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0097 );
#define	__Dt__DtFunc_OSUnlockResource__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0098 );
#define	__Dt__DtFunc_OSUnlockResource__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0099 );
#define	__Dt__DtFunc_OSUnlockResource__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x009A );
#define	__Dt__DtFunc_OSIsResourceLocked__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x009B );
#define	__Dt__DtFunc_OSMapLinToCPUPhys__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x009C );
#define	__Dt__DtFunc_OSMapLinToCPUPhys__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x009D );
#define	__Dt__DtFunc_OSMapPhysToLin__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x009E );
#define	__Dt__DtFunc_OSMapPhysToLin__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009F );
#define	__Dt__DtFunc_OSMapPhysToLin__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A1 );
#define	__Dt__DtFunc_OSMapPhysToLin__DtStep_3	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A2 );
#define	__Dt__DtFunc_OSMapPhysToLin__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A3 );
#define	__Dt__DtFunc_OSMapPhysToLin__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A4 );
#define	__Dt__DtFunc_OSMapPhysToLin__DtStep_6	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A5 );
#define	__Dt__DtFunc_OSMapPhysToLin__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A6 );
#define	__Dt__DtFunc_OSMapPhysToLin__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A7 );
#define	__Dt__DtFunc_OSUnMapPhysToLin__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00A8 );
#define	__Dt__DtFunc_OSUnMapPhysToLin__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A9 );
#define	__Dt__DtFunc_OSUnMapPhysToLin__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00AA );
#define	__Dt__DtFunc_OSUnMapPhysToLin__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00AB );
#define	__Dt__DtFunc_OSUnMapPhysToLin__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00AC );
#define	__Dt__DtFunc_OSUnMapPhysToLin__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00AD );
#define	__Dt__DtFunc_RegisterExternalMem__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00AE );
#define	__Dt__DtFunc_RegisterExternalMem__DtStep_1	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00AF );
#define	__Dt__DtFunc_RegisterExternalMem__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B1 );
#define	__Dt__DtFunc_RegisterExternalMem__DtStep_3	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00B2 );
#define	__Dt__DtFunc_RegisterExternalMem__DtStep_4	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B3 );
#define	__Dt__DtFunc_RegisterExternalMem__DtStep_5	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B4 );
#define	__Dt__DtFunc_RegisterExternalMem__DtStep_6	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00B5 );
#define	__Dt__DtFunc_RegisterExternalMem__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B6 );
#define	__Dt__DtFunc_RegisterExternalMem__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B7 );
#define	__Dt__DtFunc_OSRegisterMem__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B8 );
#define	__Dt__DtFunc_OSRegisterDiscontigMem__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B9 );
#define	__Dt__DtFunc_OSUnRegisterMem__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00BA );
#define	__Dt__DtFunc_OSUnRegisterMem__DtStep_1	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00BB );
#define	__Dt__DtFunc_OSUnRegisterMem__DtStep_2	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00BC );
#define	__Dt__DtFunc_OSUnRegisterMem__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00BD );
#define	__Dt__DtFunc_OSUnRegisterMem__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BE );
#define	__Dt__DtFunc_OSUnRegisterMem__DtStep_5	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00BF );
#define	__Dt__DtFunc_OSUnRegisterMem__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C0 );
#define	__Dt__DtFunc_OSUnRegisterMem__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C1 );
#define	__Dt__DtFunc_OSUnRegisterDiscontigMem__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C2 );
#define	__Dt__DtFunc_OSReservePhys__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C3 );
#define	__Dt__DtFunc_OSReservePhys__DtStep_1	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00C4 );
#define	__Dt__DtFunc_OSReservePhys__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C5 );
#define	__Dt__DtFunc_OSReservePhys__DtStep_3	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00C6 );
#define	__Dt__DtFunc_OSReservePhys__DtStep_4	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C7 );
#define	__Dt__DtFunc_OSReservePhys__DtStep_5	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C8 );
#define	__Dt__DtFunc_OSReservePhys__DtStep_6	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00C9 );
#define	__Dt__DtFunc_OSReservePhys__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CA );
#define	__Dt__DtFunc_OSReservePhys__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CB );
#define	__Dt__DtFunc_OSUnReservePhys__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00CC );
#define	__Dt__DtFunc_OSUnReservePhys__DtStep_1	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00CD );
#define	__Dt__DtFunc_OSUnReservePhys__DtStep_2	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00CE );
#define	__Dt__DtFunc_OSUnReservePhys__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00CF );
#define	__Dt__DtFunc_OSUnReservePhys__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D0 );
#define	__Dt__DtFunc_OSUnReservePhys__DtStep_5	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00D1 );
#define	__Dt__DtFunc_OSUnReservePhys__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D2 );
#define	__Dt__DtFunc_OSUnReservePhys__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D3 );
#define	__Dt__DtFunc_OSBaseAllocContigMemory__DtStep_0	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D4 );
#define	__Dt__DtFunc_OSBaseAllocContigMemory__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D5 );
#define	__Dt__DtFunc_OSBaseAllocContigMemory__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D6 );
#define	__Dt__DtFunc_OSWriteHWReg__DtStep_0	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D7 );
#define	__Dt__DtFunc_OSPCISetDev__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00D8 );
#define	__Dt__DtFunc_OSPCISetDev__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D9 );
#define	__Dt__DtFunc_OSPCISetDev__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DA );
#define	__Dt__DtFunc_OSPCISetDev__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00DB );
#define	__Dt__DtFunc_OSPCISetDev__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DC );
#define	__Dt__DtFunc_OSPCISetDev__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00DD );
#define	__Dt__DtFunc_OSPCISetDev__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00DE );
#define	__Dt__DtFunc_OSPCISetDev__DtStep_7	/*for*/	_TP_BusOut( __DtBaseAddress, 0x00DF );
#define	__Dt__DtFunc_OSPCISetDev__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E0 );
#define	__Dt__DtFunc_OSPCIAcquireDev__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00E1 );
#define	__Dt__DtFunc_OSPCIAcquireDev__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E2 );
#define	__Dt__DtFunc_OSPCIAcquireDev__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E3 );
#define	__Dt__DtFunc_OSPCIAcquireDev__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E4 );
#define	__Dt__DtFunc_OSPCIIRQ__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00E5 );
#define	__Dt__DtFunc_OSPCIIRQ__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E6 );
#define	__Dt__DtFunc_OSPCIAddrRangeFunc__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00E7 );
#define	__Dt__DtFunc_OSPCIAddrRangeFunc__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E8 );
#define	__Dt__DtFunc_OSPCIAddrRangeFunc__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E9 );
#define	__Dt__DtFunc_OSPCIAddrRangeFunc__DtStep_3	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00EA );
#define	__Dt__DtFunc_OSPCIAddrRangeFunc__DtStep_4	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00EB );
#define	__Dt__DtFunc_OSPCIAddrRangeFunc__DtStep_5	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00EC );
#define	__Dt__DtFunc_OSPCIAddrRangeFunc__DtStep_6	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00ED );
#define	__Dt__DtFunc_OSPCIAddrRangeFunc__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00EE );
#define	__Dt__DtFunc_OSPCIAddrRangeFunc__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00EF );
#define	__Dt__DtFunc_OSPCIAddrRangeFunc__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F1 );
#define	__Dt__DtFunc_OSPCIAddrRangeFunc__DtStep_10	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00F2 );
#define	__Dt__DtFunc_OSPCIAddrRangeFunc__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F3 );
#define	__Dt__DtFunc_OSPCIAddrRangeFunc__DtStep_12	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F4 );
#define	__Dt__DtFunc_OSPCIAddrRangeFunc__DtStep_13	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00F5 );
#define	__Dt__DtFunc_OSPCIAddrRangeFunc__DtStep_14	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F6 );
#define	__Dt__DtFunc_OSPCIAddrRangeLen__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F7 );
#define	__Dt__DtFunc_OSPCIAddrRangeStart__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F8 );
#define	__Dt__DtFunc_OSPCIAddrRangeEnd__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F9 );
#define	__Dt__DtFunc_OSPCIRequestAddrRange__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00FA );
#define	__Dt__DtFunc_OSPCIReleaseAddrRange__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00FB );
#define	__Dt__DtFunc_OSPCIReleaseDev__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00FC );
#define	__Dt__DtFunc_OSPCIReleaseDev__DtStep_1	/*for*/	_TP_BusOut( __DtBaseAddress, 0x00FD );
#define	__Dt__DtFunc_OSPCIReleaseDev__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00FE );
#define	__Dt__DtFunc_OSPCIReleaseDev__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00FF );
#define	__Dt__DtFunc_OSPCIReleaseDev__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0100 );
#define	__Dt__DtFunc_OSPCIReleaseDev__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0101 );
#define	__Dt__DtFunc_OSPCISuspendDev__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0102 );
#define	__Dt__DtFunc_OSPCISuspendDev__DtStep_1	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0103 );
#define	__Dt__DtFunc_OSPCISuspendDev__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0104 );
#define	__Dt__DtFunc_OSPCISuspendDev__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0105 );
#define	__Dt__DtFunc_OSPCISuspendDev__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0106 );
#define	__Dt__DtFunc_OSPCISuspendDev__DtStep_5	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0107 );
#define	__Dt__DtFunc_OSPCISuspendDev__DtStep_6	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0108 );
#define	__Dt__DtFunc_OSPCISuspendDev__DtStep_7	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0109 );
#define	__Dt__DtFunc_OSPCISuspendDev__DtStep_8	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x010A );
#define	__Dt__DtFunc_OSPCISuspendDev__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x010B );
#define	__Dt__DtFunc_OSPCIResumeDev__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x010C );
#define	__Dt__DtFunc_OSPCIResumeDev__DtStep_1	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x010D );
#define	__Dt__DtFunc_OSPCIResumeDev__DtStep_2	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x010E );
#define	__Dt__DtFunc_OSPCIResumeDev__DtStep_3	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x010F );
#define	__Dt__DtFunc_OSPCIResumeDev__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0110 );
#define	__Dt__DtFunc_OSPCIResumeDev__DtStep_5	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0111 );
#define	__Dt__DtFunc_OSPCIResumeDev__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0112 );
#define	__Dt__DtFunc_OSPCIResumeDev__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0113 );
#define	__Dt__DtFunc_OSPCIResumeDev__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0114 );
#define	__Dt__DtFunc_OSPCIResumeDev__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0115 );
#define	__Dt__DtFunc_OSPCIResumeDev__DtStep_10	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0116 );
#define	__Dt__DtFunc_OSPCIResumeDev__DtStep_11	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0117 );
#define	__Dt__DtFunc_OSPCIResumeDev__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0118 );
#define	__Dt__DtFunc_OSPCIResumeDev__DtStep_13	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0119 );
#define	__Dt__DtFunc_OSPCIResumeDev__DtStep_14	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x011A );
#define	__Dt__DtFunc_OSTimerCallbackBody__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x011B );
#define	__Dt__DtFunc_OSTimerCallbackBody__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x011C );
#define	__Dt__DtFunc_OSTimerCallbackWrapper__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x011D );
#define	__Dt__DtFunc_OSTimerCallbackWrapper__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x011E );
#define	__Dt__DtFunc_OSTimerWorkQueueCallBack__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x011F );
#define	__Dt__DtFunc_OSTimerWorkQueueCallBack__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0120 );
#define	__Dt__DtFunc_OSAddTimer__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0121 );
#define	__Dt__DtFunc_OSAddTimer__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0122 );
#define	__Dt__DtFunc_OSAddTimer__DtStep_2	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0123 );
#define	__Dt__DtFunc_OSAddTimer__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0124 );
#define	__Dt__DtFunc_OSAddTimer__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0125 );
#define	__Dt__DtFunc_OSAddTimer__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0126 );
#define	__Dt__DtFunc_OSAddTimer__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0127 );
#define	__Dt__DtFunc_GetTimerStructure__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0128 );
#define	__Dt__DtFunc_GetTimerStructure__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x012A );
#define	__Dt__DtFunc_OSRemoveTimer__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x012B );
#define	__Dt__DtFunc_OSRemoveTimer__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x012C );
#define	__Dt__DtFunc_OSEnableTimer__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x012D );
#define	__Dt__DtFunc_OSEnableTimer__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x012E );
#define	__Dt__DtFunc_OSDisableTimer__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x012F );
#define	__Dt__DtFunc_OSEventObjectCreate__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0131 );
#define	__Dt__DtFunc_OSEventObjectCreate__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0132 );
#define	__Dt__DtFunc_OSEventObjectCreate__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0133 );
#define	__Dt__DtFunc_OSEventObjectCreate__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0134 );
#define	__Dt__DtFunc_OSEventObjectCreate__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0135 );
#define	__Dt__DtFunc_OSEventObjectCreate__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0136 );
#define	__Dt__DtFunc_OSEventObjectCreate__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0137 );
#define	__Dt__DtFunc_OSEventObjectDestroy__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0138 );
#define	__Dt__DtFunc_OSEventObjectDestroy__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0139 );
#define	__Dt__DtFunc_OSEventObjectDestroy__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x013A );
#define	__Dt__DtFunc_OSEventObjectDestroy__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x013B );
#define	__Dt__DtFunc_OSEventObjectDestroy__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x013C );
#define	__Dt__DtFunc_OSEventObjectDestroy__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x013D );
#define	__Dt__DtFunc_OSEventObjectWait__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x013E );
#define	__Dt__DtFunc_OSEventObjectWait__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x013F );
#define	__Dt__DtFunc_OSEventObjectWait__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0140 );
#define	__Dt__DtFunc_OSEventObjectWait__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0141 );
#define	__Dt__DtFunc_OSEventObjectOpen__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0142 );
#define	__Dt__DtFunc_OSEventObjectOpen__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0143 );
#define	__Dt__DtFunc_OSEventObjectOpen__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0144 );
#define	__Dt__DtFunc_OSEventObjectOpen__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0145 );
#define	__Dt__DtFunc_OSEventObjectOpen__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0146 );
#define	__Dt__DtFunc_OSEventObjectClose__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0147 );
#define	__Dt__DtFunc_OSEventObjectClose__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0148 );
#define	__Dt__DtFunc_OSEventObjectClose__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0149 );
#define	__Dt__DtFunc_OSEventObjectClose__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x014A );
#define	__Dt__DtFunc_OSEventObjectClose__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x014B );
#define	__Dt__DtFunc_OSEventObjectSignal__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x014C );
#define	__Dt__DtFunc_OSEventObjectSignal__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x014D );
#define	__Dt__DtFunc_OSEventObjectSignal__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x014E );
#define	__Dt__DtFunc_OSEventObjectSignal__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x014F );
#define	__Dt__DtFunc_OSProcHasPrivSrvInit__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0150 );
#define	__Dt__DtFunc_OSCopyToUser__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0151 );
#define	__Dt__DtFunc_OSCopyToUser__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0152 );
#define	__Dt__DtFunc_OSCopyFromUser__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0153 );
#define	__Dt__DtFunc_OSCopyFromUser__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0154 );
#define	__Dt__DtFunc_OSAccessOK__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0155 );
#define	__Dt__DtFunc_OSAccessOK__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0156 );
#define	__Dt__DtFunc_OSAccessOK__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0157 );
#define	__Dt__DtFunc_OSAccessOK__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0158 );
#define	__Dt__DtFunc_CPUVAddrToPFN__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0159 );
#define	__Dt__DtFunc_CPUVAddrToPFN__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x015A );
#define	__Dt__DtFunc_CPUVAddrToPFN__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x015B );
#define	__Dt__DtFunc_OSReleasePhysPageAddr__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x015C );
#define	__Dt__DtFunc_OSReleasePhysPageAddr__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x015D );
#define	__Dt__DtFunc_OSReleasePhysPageAddr__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x015E );
#define	__Dt__DtFunc_OSReleasePhysPageAddr__DtStep_3	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x015F );
#define	__Dt__DtFunc_OSReleasePhysPageAddr__DtStep_4	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0160 );
#define	__Dt__DtFunc_OSReleasePhysPageAddr__DtStep_5	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0161 );
#define	__Dt__DtFunc_OSReleasePhysPageAddr__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0162 );
#define	__Dt__DtFunc_OSReleasePhysPageAddr__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0163 );
#define	__Dt__DtFunc_OSReleasePhysPageAddr__DtStep_8	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0164 );
#define	__Dt__DtFunc_OSReleasePhysPageAddr__DtStep_9	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0165 );
#define	__Dt__DtFunc_OSReleasePhysPageAddr__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0166 );
#define	__Dt__DtFunc_OSReleasePhysPageAddr__DtStep_11	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0167 );
#define	__Dt__DtFunc_OSReleasePhysPageAddr__DtStep_12	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0168 );
#define	__Dt__DtFunc_OSReleasePhysPageAddr__DtStep_13	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0169 );
#define	__Dt__DtFunc_OSReleasePhysPageAddr__DtStep_14	/*if*/	_TP_BusOut( __DtBaseAddress, 0x016A );
#define	__Dt__DtFunc_OSReleasePhysPageAddr__DtStep_15	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x016B );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x016C );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x016D );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x016E );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x016F );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0170 );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0171 );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0172 );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_7	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0173 );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0174 );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0175 );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0176 );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0177 );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0178 );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_13	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0179 );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_14	/*for*/	_TP_BusOut( __DtBaseAddress, 0x017A );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_15	/*if*/	_TP_BusOut( __DtBaseAddress, 0x017B );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_16	/*if*/	_TP_BusOut( __DtBaseAddress, 0x017C );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_17	/*if*/	_TP_BusOut( __DtBaseAddress, 0x017D );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_18	/*if*/	_TP_BusOut( __DtBaseAddress, 0x017E );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_19	/*if*/	_TP_BusOut( __DtBaseAddress, 0x017F );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_20	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0181 );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_21	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0182 );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_22	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0183 );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_23	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0184 );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_24	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0185 );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_25	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0186 );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_26	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0187 );
#define	__Dt__DtFunc_OSAcquirePhysPageAddr__DtStep_27	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0188 );
#define	__Dt__DtFunc_VMallocAreaToPhys__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0189 );
#define	__Dt__DtFunc_ExternalKVAreaToPhys__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x018A );
#define	__Dt__DtFunc_ExternalKVAreaToPhys__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x018B );
#define	__Dt__DtFunc_AllocPagesAreaToPhys__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x018C );
#define	__Dt__DtFunc_AllocPagesAreaToPhys__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x018D );
#define	__Dt__DtFunc_FindMMapBaseVAddr__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x018E );
#define	__Dt__DtFunc_FindMMapBaseVAddr__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x018F );
#define	__Dt__DtFunc_CheckExecuteCacheOp__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0191 );
#define	__Dt__DtFunc_CheckExecuteCacheOp__DtStep_1	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0192 );
#define	__Dt__DtFunc_CheckExecuteCacheOp__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0193 );
#define	__Dt__DtFunc_CheckExecuteCacheOp__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0194 );
#define	__Dt__DtFunc_CheckExecuteCacheOp__DtStep_4	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0195 );
#define	__Dt__DtFunc_CheckExecuteCacheOp__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0196 );
#define	__Dt__DtFunc_CheckExecuteCacheOp__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0197 );
#define	__Dt__DtFunc_CheckExecuteCacheOp__DtStep_7	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0198 );
#define	__Dt__DtFunc_CheckExecuteCacheOp__DtStep_8	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0199 );
#define	__Dt__DtFunc_CheckExecuteCacheOp__DtStep_9	/*for*/	_TP_BusOut( __DtBaseAddress, 0x019A );
#define	__Dt__DtFunc_CheckExecuteCacheOp__DtStep_10	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x019B );
#define	__Dt__DtFunc_per_cpu_cache_flush__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x019C );
#define	__Dt__DtFunc_per_cpu_cache_flush__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x019D );
#define	__Dt__DtFunc_x86_flush_cache_range__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x019E );
#define	__Dt__DtFunc_x86_flush_cache_range__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x019F );
#define	__Dt__DtFunc_OSCleanCPUCacheKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x01A1 );
#define	__Dt__DtFunc_OSCleanCPUCacheKM__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x01A2 );
#define	__Dt__DtFunc_OSFlushCPUCacheKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x01A3 );
#define	__Dt__DtFunc_OSFlushCPUCacheKM__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x01A4 );
#define	__Dt__DtFunc_OSFlushCPUCacheRangeKM__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x01A5 );
#define	__Dt__DtFunc_OSCleanCPUCacheRangeKM__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x01A6 );
#define	__Dt__DtFunc_OSInvalidateCPUCacheRangeKM__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x01A7 );
#define	__Dt__DtFunc_per_cpu_cache_flush__DtStep_2	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x01A8 );
#define	__Dt__DtFunc_per_cpu_cache_flush__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x01A9 );
#define	__Dt__DtFunc_OSCleanCPUCacheKM__DtStep_2	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x01AA );
#define	__Dt__DtFunc_OSCleanCPUCacheKM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x01AB );
#define	__Dt__DtFunc_OSFlushCPUCacheKM__DtStep_2	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x01AC );
#define	__Dt__DtFunc_OSFlushCPUCacheKM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x01AD );
#define	__Dt__DtFunc__dmac_inv_range__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x01AE );
#define	__Dt__DtFunc__dmac_inv_range__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x01AF );
#define	__Dt__DtFunc__dmac_clean_range__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x01B1 );
#define	__Dt__DtFunc__dmac_clean_range__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x01B2 );
#define	__Dt__DtFunc_OSFlushCPUCacheRangeKM__DtStep_1	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x01B3 );
#define	__Dt__DtFunc_OSCleanCPUCacheRangeKM__DtStep_1	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x01B4 );
#define	__Dt__DtFunc_OSInvalidateCPUCacheRangeKM__DtStep_1	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x01B5 );
#define	__Dt__DtFunc_OSCleanCPUCacheKM__DtStep_4	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x01B6 );
#define	__Dt__DtFunc_OSCleanCPUCacheKM__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x01B7 );
#define	__Dt__DtFunc_OSFlushCPUCacheKM__DtStep_4	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x01B8 );
#define	__Dt__DtFunc_OSFlushCPUCacheKM__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x01B9 );
#define	__Dt__DtFunc_OSFlushCPUCacheRangeKM__DtStep_2	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x01BA );
#define	__Dt__DtFunc_OSFlushCPUCacheRangeKM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x01BB );
#define	__Dt__DtFunc_OSCleanCPUCacheRangeKM__DtStep_2	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x01BC );
#define	__Dt__DtFunc_OSCleanCPUCacheRangeKM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x01BD );
#define	__Dt__DtFunc_OSInvalidateCPUCacheRangeKM__DtStep_2	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x01BE );
#define	__Dt__DtFunc_OSInvalidateCPUCacheRangeKM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x01BF );
#define	__Dt__DtFunc_PVROSFuncInit__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x01C0 );
#define	__Dt__DtFunc_PVROSFuncInit__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x01C1 );
#define	__Dt__DtFunc_PVROSFuncInit__DtStep_2	/*for*/	_TP_BusOut( __DtBaseAddress, 0x01C2 );
#define	__Dt__DtFunc_PVROSFuncDeInit__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x01C3 );
#define	__Dt__DtFunc_PVROSFuncDeInit__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x01C4 );

#endif	/* __DT_osfunc_h__ */

