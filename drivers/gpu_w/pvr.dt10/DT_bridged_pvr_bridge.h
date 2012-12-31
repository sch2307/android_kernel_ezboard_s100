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

#ifndef	__DT_bridged_pvr_bridge_h__
#define	__DT_bridged_pvr_bridge_h__

#ifdef	__cplusplus
	#define	__DtEXTERN	extern "C"
#else
	#define	__DtEXTERN	extern
#endif

/* TestPoint MacroCode -----------------------------------------------------*/
#define		__DtBaseAddress		0x10
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
#define		__DtFunc_CopyFromUserWrapper		0
#define		__DtFunc_CopyToUserWrapper		1
#define		__DtFunc_PVRSRVEnumerateDevicesBW		2
#define		__DtFunc_PVRSRVAcquireDeviceDataBW		3
#define		__DtFunc_PVRSRVCreateDeviceMemContextBW		4
#define		__DtFunc_PVRSRVDestroyDeviceMemContextBW		5
#define		__DtFunc_PVRSRVGetDeviceMemHeapInfoBW		6
#define		__DtFunc_PVRSRVAllocDeviceMemBW		7
#define		__DtFunc_PVRSRVFreeDeviceMemBW		8
#define		__DtFunc_PVRSRVExportDeviceMemBW		9
#define		__DtFunc_PVRSRVMapDeviceMemoryBW		10
#define		__DtFunc_PVRSRVUnmapDeviceMemoryBW		11
#define		__DtFunc_PVRSRVMapDeviceClassMemoryBW		12
#define		__DtFunc_PVRSRVUnmapDeviceClassMemoryBW		13
#define		__DtFunc_PVRSRVWrapExtMemoryBW		14
#define		__DtFunc_PVRSRVUnwrapExtMemoryBW		15
#define		__DtFunc_PVRSRVGetFreeDeviceMemBW		16
#define		__DtFunc_PVRMMapOSMemHandleToMMapDataBW		17
#define		__DtFunc_PVRMMapReleaseMMapDataBW		18
#define		__DtFunc_PDumpIsCaptureFrameBW		19
#define		__DtFunc_PDumpCommentBW		20
#define		__DtFunc_PDumpSetFrameBW		21
#define		__DtFunc_PDumpRegWithFlagsBW		22
#define		__DtFunc_PDumpRegPolBW		23
#define		__DtFunc_PDumpMemPolBW		24
#define		__DtFunc_PDumpMemBW		25
#define		__DtFunc_PDumpBitmapBW		26
#define		__DtFunc_PDumpReadRegBW		27
#define		__DtFunc_PDumpDriverInfoBW		28
#define		__DtFunc_PDumpSyncDumpBW		29
#define		__DtFunc_PDumpSyncPolBW		30
#define		__DtFunc_PDumpCycleCountRegReadBW		31
#define		__DtFunc_PDumpPDDevPAddrBW		32
#define		__DtFunc_PDumpStartInitPhaseBW		33
#define		__DtFunc_PDumpStopInitPhaseBW		34
#define		__DtFunc_PVRSRVGetMiscInfoBW		35
#define		__DtFunc_PVRSRVConnectBW		36
#define		__DtFunc_PVRSRVDisconnectBW		37
#define		__DtFunc_PVRSRVEnumerateDCBW		38
#define		__DtFunc_PVRSRVOpenDCDeviceBW		39
#define		__DtFunc_PVRSRVCloseDCDeviceBW		40
#define		__DtFunc_PVRSRVEnumDCFormatsBW		41
#define		__DtFunc_PVRSRVEnumDCDimsBW		42
#define		__DtFunc_PVRSRVGetDCSystemBufferBW		43
#define		__DtFunc_PVRSRVGetDCInfoBW		44
#define		__DtFunc_PVRSRVCreateDCSwapChainBW		45
#define		__DtFunc_PVRSRVDestroyDCSwapChainBW		46
#define		__DtFunc_PVRSRVSetDCDstRectBW		47
#define		__DtFunc_PVRSRVSetDCSrcRectBW		48
#define		__DtFunc_PVRSRVSetDCDstColourKeyBW		49
#define		__DtFunc_PVRSRVSetDCSrcColourKeyBW		50
#define		__DtFunc_PVRSRVGetDCBuffersBW		51
#define		__DtFunc_PVRSRVSwapToDCBufferBW		52
#define		__DtFunc_PVRSRVSwapToDCSystemBW		53
#define		__DtFunc_PVRSRVOpenBCDeviceBW		54
#define		__DtFunc_PVRSRVCloseBCDeviceBW		55
#define		__DtFunc_PVRSRVGetBCInfoBW		56
#define		__DtFunc_PVRSRVGetBCBufferBW		57
#define		__DtFunc_PVRSRVAllocSharedSysMemoryBW		58
#define		__DtFunc_PVRSRVFreeSharedSysMemoryBW		59
#define		__DtFunc_PVRSRVMapMemInfoMemBW		60
#define		__DtFunc_MMU_GetPDDevPAddrBW		61
#define		__DtFunc_DummyBW		62
#define		__DtFunc__SetDispatchTableEntry		63
#define		__DtFunc_PVRSRVInitSrvConnectBW		64
#define		__DtFunc_PVRSRVInitSrvDisconnectBW		65
#define		__DtFunc_PVRSRVEventObjectWaitBW		66
#define		__DtFunc_PVRSRVEventObjectOpenBW		67
#define		__DtFunc_PVRSRVEventObjectCloseBW		68
#define		__DtFunc_DoQuerySyncOpsSatisfied		69
#define		__DtFunc_DoModifyCompleteSyncOps		70
#define		__DtFunc_ModifyCompleteSyncOpsCallBack		71
#define		__DtFunc_PVRSRVCreateSyncInfoModObjBW		72
#define		__DtFunc_PVRSRVDestroySyncInfoModObjBW		73
#define		__DtFunc_PVRSRVModifyPendingSyncOpsBW		74
#define		__DtFunc_PVRSRVModifyCompleteSyncOpsBW		75
#define		__DtFunc_PVRSRVSyncOpsFlushToModObjBW		76
#define		__DtFunc_PVRSRVSyncOpsFlushToDeltaBW		77
#define		__DtFunc_FreeSyncInfoCallback		78
#define		__DtFunc_PVRSRVAllocSyncInfoBW		79
#define		__DtFunc_PVRSRVFreeSyncInfoBW		80
#define		__DtFunc_CommonBridgeInit		81
#define		__DtFunc_BridgedDispatchKM		82

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

/* TestPoint DisableList ---------------------------------------------------*/
#define	__Dt__DtFunc_CopyFromUserWrapper__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0000 );
#define	__Dt__DtFunc_CopyFromUserWrapper__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0001 );
#define	__Dt__DtFunc_CopyToUserWrapper__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0002 );
#define	__Dt__DtFunc_CopyToUserWrapper__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0003 );
#define	__Dt__DtFunc_PVRSRVEnumerateDevicesBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0004 );
#define	__Dt__DtFunc_PVRSRVEnumerateDevicesBW__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0005 );
#define	__Dt__DtFunc_PVRSRVAcquireDeviceDataBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0006 );
#define	__Dt__DtFunc_PVRSRVAcquireDeviceDataBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0007 );
#define	__Dt__DtFunc_PVRSRVAcquireDeviceDataBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0008 );
#define	__Dt__DtFunc_PVRSRVCreateDeviceMemContextBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0009 );
#define	__Dt__DtFunc_PVRSRVCreateDeviceMemContextBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000A );
#define	__Dt__DtFunc_PVRSRVCreateDeviceMemContextBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000B );
#define	__Dt__DtFunc_PVRSRVCreateDeviceMemContextBW__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000C );
#define	__Dt__DtFunc_PVRSRVCreateDeviceMemContextBW__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000D );
#define	__Dt__DtFunc_PVRSRVCreateDeviceMemContextBW__DtStep_5	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000E );
#define	__Dt__DtFunc_PVRSRVCreateDeviceMemContextBW__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000F );
#define	__Dt__DtFunc_PVRSRVCreateDeviceMemContextBW__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0010 );
#define	__Dt__DtFunc_PVRSRVCreateDeviceMemContextBW__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0011 );
#define	__Dt__DtFunc_PVRSRVCreateDeviceMemContextBW__DtStep_9	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0012 );
#define	__Dt__DtFunc_PVRSRVCreateDeviceMemContextBW__DtStep_10	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0013 );
#define	__Dt__DtFunc_PVRSRVDestroyDeviceMemContextBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0014 );
#define	__Dt__DtFunc_PVRSRVDestroyDeviceMemContextBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0015 );
#define	__Dt__DtFunc_PVRSRVDestroyDeviceMemContextBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0016 );
#define	__Dt__DtFunc_PVRSRVDestroyDeviceMemContextBW__DtStep_3	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0017 );
#define	__Dt__DtFunc_PVRSRVDestroyDeviceMemContextBW__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0018 );
#define	__Dt__DtFunc_PVRSRVDestroyDeviceMemContextBW__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0019 );
#define	__Dt__DtFunc_PVRSRVGetDeviceMemHeapInfoBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x001A );
#define	__Dt__DtFunc_PVRSRVGetDeviceMemHeapInfoBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001B );
#define	__Dt__DtFunc_PVRSRVGetDeviceMemHeapInfoBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001C );
#define	__Dt__DtFunc_PVRSRVGetDeviceMemHeapInfoBW__DtStep_3	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001D );
#define	__Dt__DtFunc_PVRSRVGetDeviceMemHeapInfoBW__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001E );
#define	__Dt__DtFunc_PVRSRVGetDeviceMemHeapInfoBW__DtStep_5	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001F );
#define	__Dt__DtFunc_PVRSRVGetDeviceMemHeapInfoBW__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0020 );
#define	__Dt__DtFunc_PVRSRVAllocDeviceMemBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0021 );
#define	__Dt__DtFunc_PVRSRVAllocDeviceMemBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0022 );
#define	__Dt__DtFunc_PVRSRVAllocDeviceMemBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0023 );
#define	__Dt__DtFunc_PVRSRVAllocDeviceMemBW__DtStep_3	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0024 );
#define	__Dt__DtFunc_PVRSRVAllocDeviceMemBW__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0025 );
#define	__Dt__DtFunc_PVRSRVAllocDeviceMemBW__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0026 );
#define	__Dt__DtFunc_PVRSRVAllocDeviceMemBW__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0027 );
#define	__Dt__DtFunc_PVRSRVFreeDeviceMemBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0028 );
#define	__Dt__DtFunc_PVRSRVFreeDeviceMemBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002A );
#define	__Dt__DtFunc_PVRSRVFreeDeviceMemBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002B );
#define	__Dt__DtFunc_PVRSRVFreeDeviceMemBW__DtStep_3	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002C );
#define	__Dt__DtFunc_PVRSRVFreeDeviceMemBW__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002D );
#define	__Dt__DtFunc_PVRSRVExportDeviceMemBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x002E );
#define	__Dt__DtFunc_PVRSRVExportDeviceMemBW__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002F );
#define	__Dt__DtFunc_PVRSRVExportDeviceMemBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0031 );
#define	__Dt__DtFunc_PVRSRVExportDeviceMemBW__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0032 );
#define	__Dt__DtFunc_PVRSRVExportDeviceMemBW__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0033 );
#define	__Dt__DtFunc_PVRSRVExportDeviceMemBW__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0034 );
#define	__Dt__DtFunc_PVRSRVExportDeviceMemBW__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0035 );
#define	__Dt__DtFunc_PVRSRVExportDeviceMemBW__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0036 );
#define	__Dt__DtFunc_PVRSRVExportDeviceMemBW__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0037 );
#define	__Dt__DtFunc_PVRSRVExportDeviceMemBW__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0038 );
#define	__Dt__DtFunc_PVRSRVMapDeviceMemoryBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0039 );
#define	__Dt__DtFunc_PVRSRVMapDeviceMemoryBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003A );
#define	__Dt__DtFunc_PVRSRVMapDeviceMemoryBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003B );
#define	__Dt__DtFunc_PVRSRVMapDeviceMemoryBW__DtStep_3	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003C );
#define	__Dt__DtFunc_PVRSRVMapDeviceMemoryBW__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003D );
#define	__Dt__DtFunc_PVRSRVMapDeviceMemoryBW__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003E );
#define	__Dt__DtFunc_PVRSRVUnmapDeviceMemoryBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x003F );
#define	__Dt__DtFunc_PVRSRVUnmapDeviceMemoryBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0040 );
#define	__Dt__DtFunc_PVRSRVUnmapDeviceMemoryBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0041 );
#define	__Dt__DtFunc_PVRSRVUnmapDeviceMemoryBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0042 );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0043 );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0044 );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0045 );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryBW__DtStep_3	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0046 );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryBW__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0047 );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryBW__DtStep_5	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0048 );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryBW__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0049 );
#define	__Dt__DtFunc_PVRSRVMapDeviceClassMemoryBW__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004A );
#define	__Dt__DtFunc_PVRSRVUnmapDeviceClassMemoryBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x004B );
#define	__Dt__DtFunc_PVRSRVUnmapDeviceClassMemoryBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004C );
#define	__Dt__DtFunc_PVRSRVUnmapDeviceClassMemoryBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004D );
#define	__Dt__DtFunc_PVRSRVUnmapDeviceClassMemoryBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004E );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x004F );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0050 );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0051 );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryBW__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0052 );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryBW__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0053 );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryBW__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0054 );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryBW__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0055 );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryBW__DtStep_7	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0056 );
#define	__Dt__DtFunc_PVRSRVWrapExtMemoryBW__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0057 );
#define	__Dt__DtFunc_PVRSRVUnwrapExtMemoryBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0058 );
#define	__Dt__DtFunc_PVRSRVUnwrapExtMemoryBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0059 );
#define	__Dt__DtFunc_PVRSRVUnwrapExtMemoryBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005A );
#define	__Dt__DtFunc_PVRSRVUnwrapExtMemoryBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005B );
#define	__Dt__DtFunc_PVRSRVGetFreeDeviceMemBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x005C );
#define	__Dt__DtFunc_PVRSRVGetFreeDeviceMemBW__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005D );
#define	__Dt__DtFunc_PVRMMapOSMemHandleToMMapDataBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x005E );
#define	__Dt__DtFunc_PVRMMapReleaseMMapDataBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x005F );
#define	__Dt__DtFunc_PDumpIsCaptureFrameBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0060 );
#define	__Dt__DtFunc_PDumpIsCaptureFrameBW__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0061 );
#define	__Dt__DtFunc_PDumpCommentBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0062 );
#define	__Dt__DtFunc_PDumpCommentBW__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0063 );
#define	__Dt__DtFunc_PDumpSetFrameBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0064 );
#define	__Dt__DtFunc_PDumpSetFrameBW__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0065 );
#define	__Dt__DtFunc_PDumpRegWithFlagsBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0066 );
#define	__Dt__DtFunc_PDumpRegWithFlagsBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0067 );
#define	__Dt__DtFunc_PDumpRegWithFlagsBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0068 );
#define	__Dt__DtFunc_PDumpRegPolBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0069 );
#define	__Dt__DtFunc_PDumpRegPolBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006A );
#define	__Dt__DtFunc_PDumpRegPolBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006B );
#define	__Dt__DtFunc_PDumpMemPolBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x006C );
#define	__Dt__DtFunc_PDumpMemPolBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006D );
#define	__Dt__DtFunc_PDumpMemPolBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006E );
#define	__Dt__DtFunc_PDumpMemBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x006F );
#define	__Dt__DtFunc_PDumpMemBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0070 );
#define	__Dt__DtFunc_PDumpMemBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0071 );
#define	__Dt__DtFunc_PDumpBitmapBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0072 );
#define	__Dt__DtFunc_PDumpBitmapBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0073 );
#define	__Dt__DtFunc_PDumpBitmapBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0074 );
#define	__Dt__DtFunc_PDumpReadRegBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0075 );
#define	__Dt__DtFunc_PDumpReadRegBW__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0076 );
#define	__Dt__DtFunc_PDumpDriverInfoBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0077 );
#define	__Dt__DtFunc_PDumpDriverInfoBW__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0078 );
#define	__Dt__DtFunc_PDumpDriverInfoBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0079 );
#define	__Dt__DtFunc_PDumpSyncDumpBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x007A );
#define	__Dt__DtFunc_PDumpSyncDumpBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007B );
#define	__Dt__DtFunc_PDumpSyncDumpBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007C );
#define	__Dt__DtFunc_PDumpSyncPolBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x007D );
#define	__Dt__DtFunc_PDumpSyncPolBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007E );
#define	__Dt__DtFunc_PDumpSyncPolBW__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007F );
#define	__Dt__DtFunc_PDumpSyncPolBW__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0081 );
#define	__Dt__DtFunc_PDumpSyncPolBW__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0082 );
#define	__Dt__DtFunc_PDumpCycleCountRegReadBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0083 );
#define	__Dt__DtFunc_PDumpCycleCountRegReadBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0084 );
#define	__Dt__DtFunc_PDumpCycleCountRegReadBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0085 );
#define	__Dt__DtFunc_PDumpPDDevPAddrBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0086 );
#define	__Dt__DtFunc_PDumpPDDevPAddrBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0087 );
#define	__Dt__DtFunc_PDumpPDDevPAddrBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0088 );
#define	__Dt__DtFunc_PDumpStartInitPhaseBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0089 );
#define	__Dt__DtFunc_PDumpStartInitPhaseBW__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x008A );
#define	__Dt__DtFunc_PDumpStopInitPhaseBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x008B );
#define	__Dt__DtFunc_PDumpStopInitPhaseBW__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x008C );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x008D );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoBW__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008E );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x008F );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoBW__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0091 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoBW__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0092 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoBW__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0093 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoBW__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0094 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoBW__DtStep_7	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0095 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoBW__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0096 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoBW__DtStep_9	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0097 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoBW__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0098 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoBW__DtStep_11	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0099 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoBW__DtStep_12	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x009A );
#define	__Dt__DtFunc_PVRSRVConnectBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x009B );
#define	__Dt__DtFunc_PVRSRVConnectBW__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x009C );
#define	__Dt__DtFunc_PVRSRVDisconnectBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x009D );
#define	__Dt__DtFunc_PVRSRVDisconnectBW__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x009E );
#define	__Dt__DtFunc_PVRSRVEnumerateDCBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x009F );
#define	__Dt__DtFunc_PVRSRVEnumerateDCBW__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A1 );
#define	__Dt__DtFunc_PVRSRVOpenDCDeviceBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00A2 );
#define	__Dt__DtFunc_PVRSRVOpenDCDeviceBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A3 );
#define	__Dt__DtFunc_PVRSRVOpenDCDeviceBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A4 );
#define	__Dt__DtFunc_PVRSRVOpenDCDeviceBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A5 );
#define	__Dt__DtFunc_PVRSRVCloseDCDeviceBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00A6 );
#define	__Dt__DtFunc_PVRSRVCloseDCDeviceBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A7 );
#define	__Dt__DtFunc_PVRSRVCloseDCDeviceBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A8 );
#define	__Dt__DtFunc_PVRSRVCloseDCDeviceBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A9 );
#define	__Dt__DtFunc_PVRSRVEnumDCFormatsBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00AA );
#define	__Dt__DtFunc_PVRSRVEnumDCFormatsBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00AB );
#define	__Dt__DtFunc_PVRSRVEnumDCFormatsBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00AC );
#define	__Dt__DtFunc_PVRSRVEnumDCDimsBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00AD );
#define	__Dt__DtFunc_PVRSRVEnumDCDimsBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00AE );
#define	__Dt__DtFunc_PVRSRVEnumDCDimsBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00AF );
#define	__Dt__DtFunc_PVRSRVGetDCSystemBufferBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00B1 );
#define	__Dt__DtFunc_PVRSRVGetDCSystemBufferBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B2 );
#define	__Dt__DtFunc_PVRSRVGetDCSystemBufferBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B3 );
#define	__Dt__DtFunc_PVRSRVGetDCSystemBufferBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B4 );
#define	__Dt__DtFunc_PVRSRVGetDCInfoBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00B5 );
#define	__Dt__DtFunc_PVRSRVGetDCInfoBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B6 );
#define	__Dt__DtFunc_PVRSRVGetDCInfoBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B7 );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00B8 );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B9 );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BA );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BB );
#define	__Dt__DtFunc_PVRSRVDestroyDCSwapChainBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00BC );
#define	__Dt__DtFunc_PVRSRVDestroyDCSwapChainBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BD );
#define	__Dt__DtFunc_PVRSRVDestroyDCSwapChainBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BE );
#define	__Dt__DtFunc_PVRSRVDestroyDCSwapChainBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BF );
#define	__Dt__DtFunc_PVRSRVSetDCDstRectBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00C0 );
#define	__Dt__DtFunc_PVRSRVSetDCDstRectBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C1 );
#define	__Dt__DtFunc_PVRSRVSetDCDstRectBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C2 );
#define	__Dt__DtFunc_PVRSRVSetDCDstRectBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C3 );
#define	__Dt__DtFunc_PVRSRVSetDCSrcRectBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00C4 );
#define	__Dt__DtFunc_PVRSRVSetDCSrcRectBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C5 );
#define	__Dt__DtFunc_PVRSRVSetDCSrcRectBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C6 );
#define	__Dt__DtFunc_PVRSRVSetDCSrcRectBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C7 );
#define	__Dt__DtFunc_PVRSRVSetDCDstColourKeyBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00C8 );
#define	__Dt__DtFunc_PVRSRVSetDCDstColourKeyBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C9 );
#define	__Dt__DtFunc_PVRSRVSetDCDstColourKeyBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CA );
#define	__Dt__DtFunc_PVRSRVSetDCDstColourKeyBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CB );
#define	__Dt__DtFunc_PVRSRVSetDCSrcColourKeyBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00CC );
#define	__Dt__DtFunc_PVRSRVSetDCSrcColourKeyBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CD );
#define	__Dt__DtFunc_PVRSRVSetDCSrcColourKeyBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CE );
#define	__Dt__DtFunc_PVRSRVSetDCSrcColourKeyBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CF );
#define	__Dt__DtFunc_PVRSRVGetDCBuffersBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00D0 );
#define	__Dt__DtFunc_PVRSRVGetDCBuffersBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D1 );
#define	__Dt__DtFunc_PVRSRVGetDCBuffersBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D2 );
#define	__Dt__DtFunc_PVRSRVGetDCBuffersBW__DtStep_3	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D3 );
#define	__Dt__DtFunc_PVRSRVGetDCBuffersBW__DtStep_4	/*for*/	_TP_BusOut( __DtBaseAddress, 0x00D4 );
#define	__Dt__DtFunc_PVRSRVGetDCBuffersBW__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D5 );
#define	__Dt__DtFunc_PVRSRVSwapToDCBufferBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00D6 );
#define	__Dt__DtFunc_PVRSRVSwapToDCBufferBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D7 );
#define	__Dt__DtFunc_PVRSRVSwapToDCBufferBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D8 );
#define	__Dt__DtFunc_PVRSRVSwapToDCBufferBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D9 );
#define	__Dt__DtFunc_PVRSRVSwapToDCSystemBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00DA );
#define	__Dt__DtFunc_PVRSRVSwapToDCSystemBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DB );
#define	__Dt__DtFunc_PVRSRVSwapToDCSystemBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DC );
#define	__Dt__DtFunc_PVRSRVSwapToDCSystemBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DD );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00DE );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DF );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E0 );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E1 );
#define	__Dt__DtFunc_PVRSRVCloseBCDeviceBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00E2 );
#define	__Dt__DtFunc_PVRSRVCloseBCDeviceBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E3 );
#define	__Dt__DtFunc_PVRSRVCloseBCDeviceBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E4 );
#define	__Dt__DtFunc_PVRSRVCloseBCDeviceBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E5 );
#define	__Dt__DtFunc_PVRSRVGetBCInfoBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00E6 );
#define	__Dt__DtFunc_PVRSRVGetBCInfoBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E7 );
#define	__Dt__DtFunc_PVRSRVGetBCInfoBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E8 );
#define	__Dt__DtFunc_PVRSRVGetBCBufferBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00E9 );
#define	__Dt__DtFunc_PVRSRVGetBCBufferBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00EA );
#define	__Dt__DtFunc_PVRSRVGetBCBufferBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00EB );
#define	__Dt__DtFunc_PVRSRVGetBCBufferBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00EC );
#define	__Dt__DtFunc_PVRSRVAllocSharedSysMemoryBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00ED );
#define	__Dt__DtFunc_PVRSRVAllocSharedSysMemoryBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00EE );
#define	__Dt__DtFunc_PVRSRVAllocSharedSysMemoryBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00EF );
#define	__Dt__DtFunc_PVRSRVFreeSharedSysMemoryBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00F1 );
#define	__Dt__DtFunc_PVRSRVFreeSharedSysMemoryBW__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F2 );
#define	__Dt__DtFunc_PVRSRVMapMemInfoMemBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00F3 );
#define	__Dt__DtFunc_PVRSRVMapMemInfoMemBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F4 );
#define	__Dt__DtFunc_PVRSRVMapMemInfoMemBW__DtStep_2	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00F5 );
#define	__Dt__DtFunc_PVRSRVMapMemInfoMemBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F6 );
#define	__Dt__DtFunc_PVRSRVMapMemInfoMemBW__DtStep_4	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F7 );
#define	__Dt__DtFunc_PVRSRVMapMemInfoMemBW__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F8 );
#define	__Dt__DtFunc_PVRSRVMapMemInfoMemBW__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F9 );
#define	__Dt__DtFunc_PVRSRVMapMemInfoMemBW__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00FA );
#define	__Dt__DtFunc_PVRSRVMapMemInfoMemBW__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00FB );
#define	__Dt__DtFunc_MMU_GetPDDevPAddrBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00FC );
#define	__Dt__DtFunc_MMU_GetPDDevPAddrBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00FD );
#define	__Dt__DtFunc_MMU_GetPDDevPAddrBW__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00FE );
#define	__Dt__DtFunc_MMU_GetPDDevPAddrBW__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00FF );
#define	__Dt__DtFunc_MMU_GetPDDevPAddrBW__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0100 );
#define	__Dt__DtFunc__SetDispatchTableEntry__DtStep_0	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0101 );
#define	__Dt__DtFunc_PVRSRVInitSrvConnectBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0102 );
#define	__Dt__DtFunc_PVRSRVInitSrvConnectBW__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0103 );
#define	__Dt__DtFunc_PVRSRVInitSrvConnectBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0104 );
#define	__Dt__DtFunc_PVRSRVInitSrvConnectBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0105 );
#define	__Dt__DtFunc_PVRSRVInitSrvDisconnectBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0106 );
#define	__Dt__DtFunc_PVRSRVInitSrvDisconnectBW__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0107 );
#define	__Dt__DtFunc_PVRSRVInitSrvDisconnectBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0108 );
#define	__Dt__DtFunc_PVRSRVInitSrvDisconnectBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0109 );
#define	__Dt__DtFunc_PVRSRVEventObjectWaitBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x010A );
#define	__Dt__DtFunc_PVRSRVEventObjectWaitBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x010B );
#define	__Dt__DtFunc_PVRSRVEventObjectWaitBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x010C );
#define	__Dt__DtFunc_PVRSRVEventObjectOpenBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x010D );
#define	__Dt__DtFunc_PVRSRVEventObjectOpenBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x010E );
#define	__Dt__DtFunc_PVRSRVEventObjectOpenBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x010F );
#define	__Dt__DtFunc_PVRSRVEventObjectOpenBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0110 );
#define	__Dt__DtFunc_PVRSRVEventObjectCloseBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0111 );
#define	__Dt__DtFunc_PVRSRVEventObjectCloseBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0112 );
#define	__Dt__DtFunc_PVRSRVEventObjectCloseBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0113 );
#define	__Dt__DtFunc_PVRSRVEventObjectCloseBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0114 );
#define	__Dt__DtFunc_DoQuerySyncOpsSatisfied__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0115 );
#define	__Dt__DtFunc_DoQuerySyncOpsSatisfied__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0116 );
#define	__Dt__DtFunc_DoQuerySyncOpsSatisfied__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0117 );
#define	__Dt__DtFunc_DoQuerySyncOpsSatisfied__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0118 );
#define	__Dt__DtFunc_DoModifyCompleteSyncOps__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0119 );
#define	__Dt__DtFunc_DoModifyCompleteSyncOps__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x011A );
#define	__Dt__DtFunc_DoModifyCompleteSyncOps__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x011B );
#define	__Dt__DtFunc_DoModifyCompleteSyncOps__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x011C );
#define	__Dt__DtFunc_DoModifyCompleteSyncOps__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x011D );
#define	__Dt__DtFunc_DoModifyCompleteSyncOps__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x011E );
#define	__Dt__DtFunc_ModifyCompleteSyncOpsCallBack__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x011F );
#define	__Dt__DtFunc_ModifyCompleteSyncOpsCallBack__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0120 );
#define	__Dt__DtFunc_ModifyCompleteSyncOpsCallBack__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0121 );
#define	__Dt__DtFunc_ModifyCompleteSyncOpsCallBack__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0122 );
#define	__Dt__DtFunc_ModifyCompleteSyncOpsCallBack__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0123 );
#define	__Dt__DtFunc_ModifyCompleteSyncOpsCallBack__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0124 );
#define	__Dt__DtFunc_ModifyCompleteSyncOpsCallBack__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0125 );
#define	__Dt__DtFunc_PVRSRVCreateSyncInfoModObjBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0126 );
#define	__Dt__DtFunc_PVRSRVCreateSyncInfoModObjBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0127 );
#define	__Dt__DtFunc_PVRSRVCreateSyncInfoModObjBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0128 );
#define	__Dt__DtFunc_PVRSRVDestroySyncInfoModObjBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x012A );
#define	__Dt__DtFunc_PVRSRVDestroySyncInfoModObjBW__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x012B );
#define	__Dt__DtFunc_PVRSRVDestroySyncInfoModObjBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x012C );
#define	__Dt__DtFunc_PVRSRVDestroySyncInfoModObjBW__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x012D );
#define	__Dt__DtFunc_PVRSRVDestroySyncInfoModObjBW__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x012E );
#define	__Dt__DtFunc_PVRSRVDestroySyncInfoModObjBW__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x012F );
#define	__Dt__DtFunc_PVRSRVDestroySyncInfoModObjBW__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0131 );
#define	__Dt__DtFunc_PVRSRVDestroySyncInfoModObjBW__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0132 );
#define	__Dt__DtFunc_PVRSRVDestroySyncInfoModObjBW__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0133 );
#define	__Dt__DtFunc_PVRSRVDestroySyncInfoModObjBW__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0134 );
#define	__Dt__DtFunc_PVRSRVModifyPendingSyncOpsBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0135 );
#define	__Dt__DtFunc_PVRSRVModifyPendingSyncOpsBW__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0136 );
#define	__Dt__DtFunc_PVRSRVModifyPendingSyncOpsBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0137 );
#define	__Dt__DtFunc_PVRSRVModifyPendingSyncOpsBW__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0138 );
#define	__Dt__DtFunc_PVRSRVModifyPendingSyncOpsBW__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0139 );
#define	__Dt__DtFunc_PVRSRVModifyPendingSyncOpsBW__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x013A );
#define	__Dt__DtFunc_PVRSRVModifyPendingSyncOpsBW__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x013B );
#define	__Dt__DtFunc_PVRSRVModifyPendingSyncOpsBW__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x013C );
#define	__Dt__DtFunc_PVRSRVModifyPendingSyncOpsBW__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x013D );
#define	__Dt__DtFunc_PVRSRVModifyPendingSyncOpsBW__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x013E );
#define	__Dt__DtFunc_PVRSRVModifyPendingSyncOpsBW__DtStep_10	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x013F );
#define	__Dt__DtFunc_PVRSRVModifyPendingSyncOpsBW__DtStep_11	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0140 );
#define	__Dt__DtFunc_PVRSRVModifyCompleteSyncOpsBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0141 );
#define	__Dt__DtFunc_PVRSRVModifyCompleteSyncOpsBW__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0142 );
#define	__Dt__DtFunc_PVRSRVModifyCompleteSyncOpsBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0143 );
#define	__Dt__DtFunc_PVRSRVModifyCompleteSyncOpsBW__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0144 );
#define	__Dt__DtFunc_PVRSRVModifyCompleteSyncOpsBW__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0145 );
#define	__Dt__DtFunc_PVRSRVModifyCompleteSyncOpsBW__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0146 );
#define	__Dt__DtFunc_PVRSRVModifyCompleteSyncOpsBW__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0147 );
#define	__Dt__DtFunc_PVRSRVModifyCompleteSyncOpsBW__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0148 );
#define	__Dt__DtFunc_PVRSRVSyncOpsFlushToModObjBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0149 );
#define	__Dt__DtFunc_PVRSRVSyncOpsFlushToModObjBW__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x014A );
#define	__Dt__DtFunc_PVRSRVSyncOpsFlushToModObjBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x014B );
#define	__Dt__DtFunc_PVRSRVSyncOpsFlushToModObjBW__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x014C );
#define	__Dt__DtFunc_PVRSRVSyncOpsFlushToModObjBW__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x014D );
#define	__Dt__DtFunc_PVRSRVSyncOpsFlushToModObjBW__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x014E );
#define	__Dt__DtFunc_PVRSRVSyncOpsFlushToModObjBW__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x014F );
#define	__Dt__DtFunc_PVRSRVSyncOpsFlushToModObjBW__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0150 );
#define	__Dt__DtFunc_PVRSRVSyncOpsFlushToDeltaBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0151 );
#define	__Dt__DtFunc_PVRSRVSyncOpsFlushToDeltaBW__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0152 );
#define	__Dt__DtFunc_PVRSRVSyncOpsFlushToDeltaBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0153 );
#define	__Dt__DtFunc_PVRSRVSyncOpsFlushToDeltaBW__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0154 );
#define	__Dt__DtFunc_PVRSRVSyncOpsFlushToDeltaBW__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0155 );
#define	__Dt__DtFunc_PVRSRVSyncOpsFlushToDeltaBW__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0156 );
#define	__Dt__DtFunc_PVRSRVSyncOpsFlushToDeltaBW__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0157 );
#define	__Dt__DtFunc_FreeSyncInfoCallback__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0158 );
#define	__Dt__DtFunc_FreeSyncInfoCallback__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0159 );
#define	__Dt__DtFunc_FreeSyncInfoCallback__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x015A );
#define	__Dt__DtFunc_PVRSRVAllocSyncInfoBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x015B );
#define	__Dt__DtFunc_PVRSRVAllocSyncInfoBW__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x015C );
#define	__Dt__DtFunc_PVRSRVAllocSyncInfoBW__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x015D );
#define	__Dt__DtFunc_PVRSRVAllocSyncInfoBW__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x015E );
#define	__Dt__DtFunc_PVRSRVAllocSyncInfoBW__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x015F );
#define	__Dt__DtFunc_PVRSRVFreeSyncInfoBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0160 );
#define	__Dt__DtFunc_PVRSRVFreeSyncInfoBW__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0161 );
#define	__Dt__DtFunc_PVRSRVFreeSyncInfoBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0162 );
#define	__Dt__DtFunc_PVRSRVFreeSyncInfoBW__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0163 );
#define	__Dt__DtFunc_PVRSRVFreeSyncInfoBW__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0164 );
#define	__Dt__DtFunc_PVRSRVFreeSyncInfoBW__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0165 );
#define	__Dt__DtFunc_PVRSRVFreeSyncInfoBW__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0166 );
#define	__Dt__DtFunc_PVRSRVFreeSyncInfoBW__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0167 );
#define	__Dt__DtFunc_CommonBridgeInit__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0168 );
#define	__Dt__DtFunc_CommonBridgeInit__DtStep_1	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0169 );
#define	__Dt__DtFunc_CommonBridgeInit__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x016A );
#define	__Dt__DtFunc_CommonBridgeInit__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x016B );
#define	__Dt__DtFunc_BridgedDispatchKM__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x016C );
#define	__Dt__DtFunc_BridgedDispatchKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x016D );
#define	__Dt__DtFunc_BridgedDispatchKM__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x016E );
#define	__Dt__DtFunc_BridgedDispatchKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x016F );
#define	__Dt__DtFunc_BridgedDispatchKM__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0170 );
#define	__Dt__DtFunc_BridgedDispatchKM__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0171 );
#define	__Dt__DtFunc_BridgedDispatchKM__DtStep_6	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0172 );
#define	__Dt__DtFunc_BridgedDispatchKM__DtStep_7	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0173 );
#define	__Dt__DtFunc_BridgedDispatchKM__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0174 );
#define	__Dt__DtFunc_BridgedDispatchKM__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0175 );
#define	__Dt__DtFunc_BridgedDispatchKM__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0176 );
#define	__Dt__DtFunc_BridgedDispatchKM__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0177 );
#define	__Dt__DtFunc_BridgedDispatchKM__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0178 );
#define	__Dt__DtFunc_BridgedDispatchKM__DtStep_13	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0179 );
#define	__Dt__DtFunc_BridgedDispatchKM__DtStep_14	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x017A );

#endif	/* __DT_bridged_pvr_bridge_h__ */

