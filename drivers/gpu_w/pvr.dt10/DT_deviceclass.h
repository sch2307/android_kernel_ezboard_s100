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

#ifndef	__DT_deviceclass_h__
#define	__DT_deviceclass_h__

#ifdef	__cplusplus
	#define	__DtEXTERN	extern "C"
#else
	#define	__DtEXTERN	extern
#endif

/* TestPoint MacroCode -----------------------------------------------------*/
#define		__DtBaseAddress		0x40
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
#define		__DtFunc_DCDeviceHandleToDCInfo		0
#define		__DtFunc_BCDeviceHandleToBCInfo		1
#define		__DtFunc_PVRSRVEnumerateDCKM_ForEachVaCb		2
#define		__DtFunc_PVRSRVEnumerateDCKM		3
#define		__DtFunc_PVRSRVRegisterDCDeviceKM		4
#define		__DtFunc_PVRSRVRemoveDCDeviceKM		5
#define		__DtFunc_PVRSRVRegisterBCDeviceKM		6
#define		__DtFunc_PVRSRVRemoveBCDeviceKM		7
#define		__DtFunc_PVRSRVCloseDCDeviceKM		8
#define		__DtFunc_CloseDCDeviceCallBack		9
#define		__DtFunc_PVRSRVOpenDCDeviceKM		10
#define		__DtFunc_PVRSRVEnumDCFormatsKM		11
#define		__DtFunc_PVRSRVEnumDCDimsKM		12
#define		__DtFunc_PVRSRVGetDCSystemBufferKM		13
#define		__DtFunc_PVRSRVGetDCInfoKM		14
#define		__DtFunc_PVRSRVDestroyDCSwapChainKM		15
#define		__DtFunc_DestroyDCSwapChain		16
#define		__DtFunc_DestroyDCSwapChainRefCallBack		17
#define		__DtFunc_PVRSRVFindSharedDCSwapChainKM		18
#define		__DtFunc_PVRSRVCreateDCSwapChainRefKM		19
#define		__DtFunc_PVRSRVCreateDCSwapChainKM		20
#define		__DtFunc_PVRSRVSetDCDstRectKM		21
#define		__DtFunc_PVRSRVSetDCSrcRectKM		22
#define		__DtFunc_PVRSRVSetDCDstColourKeyKM		23
#define		__DtFunc_PVRSRVSetDCSrcColourKeyKM		24
#define		__DtFunc_PVRSRVGetDCBuffersKM		25
#define		__DtFunc_PVRSRVSwapToDCBufferKM		26
#define		__DtFunc_PVRSRVSwapToDCSystemKM		27
#define		__DtFunc_PVRSRVRegisterSystemISRHandler		28
#define		__DtFunc_PVRSRVSetDCState_ForEachVaCb		29
#define		__DtFunc_PVRSRVSetDCState		30
#define		__DtFunc_PVRGetDisplayClassJTable		31
#define		__DtFunc_PVRSRVCloseBCDeviceKM		32
#define		__DtFunc_CloseBCDeviceCallBack		33
#define		__DtFunc_PVRSRVOpenBCDeviceKM		34
#define		__DtFunc_PVRSRVGetBCInfoKM		35
#define		__DtFunc_PVRSRVGetBCBufferKM		36
#define		__DtFunc_PVRGetBufferClassJTable		37

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
#define		__DtStep_28		28
#define		__DtStep_29		29
#define		__DtStep_30		30

/* TestPoint DisableList ---------------------------------------------------*/
#define	__Dt__DtFunc_DCDeviceHandleToDCInfo__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0000 );
#define	__Dt__DtFunc_DCDeviceHandleToDCInfo__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0001 );
#define	__Dt__DtFunc_BCDeviceHandleToBCInfo__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0002 );
#define	__Dt__DtFunc_BCDeviceHandleToBCInfo__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0003 );
#define	__Dt__DtFunc_PVRSRVEnumerateDCKM_ForEachVaCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0004 );
#define	__Dt__DtFunc_PVRSRVEnumerateDCKM_ForEachVaCb__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0005 );
#define	__Dt__DtFunc_PVRSRVEnumerateDCKM_ForEachVaCb__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0006 );
#define	__Dt__DtFunc_PVRSRVEnumerateDCKM_ForEachVaCb__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0007 );
#define	__Dt__DtFunc_PVRSRVEnumerateDCKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0008 );
#define	__Dt__DtFunc_PVRSRVEnumerateDCKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0009 );
#define	__Dt__DtFunc_PVRSRVEnumerateDCKM__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000A );
#define	__Dt__DtFunc_PVRSRVEnumerateDCKM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000B );
#define	__Dt__DtFunc_PVRSRVEnumerateDCKM__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000C );
#define	__Dt__DtFunc_PVRSRVRegisterDCDeviceKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x000D );
#define	__Dt__DtFunc_PVRSRVRegisterDCDeviceKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000E );
#define	__Dt__DtFunc_PVRSRVRegisterDCDeviceKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000F );
#define	__Dt__DtFunc_PVRSRVRegisterDCDeviceKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0010 );
#define	__Dt__DtFunc_PVRSRVRegisterDCDeviceKM__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0011 );
#define	__Dt__DtFunc_PVRSRVRegisterDCDeviceKM__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0012 );
#define	__Dt__DtFunc_PVRSRVRegisterDCDeviceKM__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0013 );
#define	__Dt__DtFunc_PVRSRVRegisterDCDeviceKM__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0014 );
#define	__Dt__DtFunc_PVRSRVRegisterDCDeviceKM__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0015 );
#define	__Dt__DtFunc_PVRSRVRegisterDCDeviceKM__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0016 );
#define	__Dt__DtFunc_PVRSRVRemoveDCDeviceKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0017 );
#define	__Dt__DtFunc_PVRSRVRemoveDCDeviceKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0018 );
#define	__Dt__DtFunc_PVRSRVRemoveDCDeviceKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0019 );
#define	__Dt__DtFunc_PVRSRVRemoveDCDeviceKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001A );
#define	__Dt__DtFunc_PVRSRVRemoveDCDeviceKM__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001B );
#define	__Dt__DtFunc_PVRSRVRemoveDCDeviceKM__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001C );
#define	__Dt__DtFunc_PVRSRVRemoveDCDeviceKM__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001D );
#define	__Dt__DtFunc_PVRSRVRegisterBCDeviceKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x001E );
#define	__Dt__DtFunc_PVRSRVRegisterBCDeviceKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001F );
#define	__Dt__DtFunc_PVRSRVRegisterBCDeviceKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0020 );
#define	__Dt__DtFunc_PVRSRVRegisterBCDeviceKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0021 );
#define	__Dt__DtFunc_PVRSRVRegisterBCDeviceKM__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0022 );
#define	__Dt__DtFunc_PVRSRVRegisterBCDeviceKM__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0023 );
#define	__Dt__DtFunc_PVRSRVRegisterBCDeviceKM__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0024 );
#define	__Dt__DtFunc_PVRSRVRegisterBCDeviceKM__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0025 );
#define	__Dt__DtFunc_PVRSRVRegisterBCDeviceKM__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0026 );
#define	__Dt__DtFunc_PVRSRVRegisterBCDeviceKM__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0027 );
#define	__Dt__DtFunc_PVRSRVRemoveBCDeviceKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0028 );
#define	__Dt__DtFunc_PVRSRVRemoveBCDeviceKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002A );
#define	__Dt__DtFunc_PVRSRVRemoveBCDeviceKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002B );
#define	__Dt__DtFunc_PVRSRVRemoveBCDeviceKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002C );
#define	__Dt__DtFunc_PVRSRVRemoveBCDeviceKM__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002D );
#define	__Dt__DtFunc_PVRSRVRemoveBCDeviceKM__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002E );
#define	__Dt__DtFunc_PVRSRVRemoveBCDeviceKM__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002F );
#define	__Dt__DtFunc_PVRSRVCloseDCDeviceKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0031 );
#define	__Dt__DtFunc_PVRSRVCloseDCDeviceKM__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0032 );
#define	__Dt__DtFunc_CloseDCDeviceCallBack__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0033 );
#define	__Dt__DtFunc_CloseDCDeviceCallBack__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0034 );
#define	__Dt__DtFunc_CloseDCDeviceCallBack__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0035 );
#define	__Dt__DtFunc_CloseDCDeviceCallBack__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0036 );
#define	__Dt__DtFunc_PVRSRVOpenDCDeviceKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0037 );
#define	__Dt__DtFunc_PVRSRVOpenDCDeviceKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0038 );
#define	__Dt__DtFunc_PVRSRVOpenDCDeviceKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0039 );
#define	__Dt__DtFunc_PVRSRVOpenDCDeviceKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003A );
#define	__Dt__DtFunc_PVRSRVOpenDCDeviceKM__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003B );
#define	__Dt__DtFunc_PVRSRVOpenDCDeviceKM__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003C );
#define	__Dt__DtFunc_PVRSRVOpenDCDeviceKM__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003D );
#define	__Dt__DtFunc_PVRSRVOpenDCDeviceKM__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003E );
#define	__Dt__DtFunc_PVRSRVOpenDCDeviceKM__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003F );
#define	__Dt__DtFunc_PVRSRVOpenDCDeviceKM__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0040 );
#define	__Dt__DtFunc_PVRSRVOpenDCDeviceKM__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0041 );
#define	__Dt__DtFunc_PVRSRVOpenDCDeviceKM__DtStep_11	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0042 );
#define	__Dt__DtFunc_PVRSRVOpenDCDeviceKM__DtStep_12	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0043 );
#define	__Dt__DtFunc_PVRSRVEnumDCFormatsKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0044 );
#define	__Dt__DtFunc_PVRSRVEnumDCFormatsKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0045 );
#define	__Dt__DtFunc_PVRSRVEnumDCFormatsKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0046 );
#define	__Dt__DtFunc_PVRSRVEnumDCFormatsKM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0047 );
#define	__Dt__DtFunc_PVRSRVEnumDCDimsKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0048 );
#define	__Dt__DtFunc_PVRSRVEnumDCDimsKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0049 );
#define	__Dt__DtFunc_PVRSRVEnumDCDimsKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004A );
#define	__Dt__DtFunc_PVRSRVEnumDCDimsKM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004B );
#define	__Dt__DtFunc_PVRSRVGetDCSystemBufferKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x004C );
#define	__Dt__DtFunc_PVRSRVGetDCSystemBufferKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004D );
#define	__Dt__DtFunc_PVRSRVGetDCSystemBufferKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004E );
#define	__Dt__DtFunc_PVRSRVGetDCSystemBufferKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004F );
#define	__Dt__DtFunc_PVRSRVGetDCSystemBufferKM__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0050 );
#define	__Dt__DtFunc_PVRSRVGetDCSystemBufferKM__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0051 );
#define	__Dt__DtFunc_PVRSRVGetDCInfoKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0052 );
#define	__Dt__DtFunc_PVRSRVGetDCInfoKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0053 );
#define	__Dt__DtFunc_PVRSRVGetDCInfoKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0054 );
#define	__Dt__DtFunc_PVRSRVGetDCInfoKM__DtStep_3	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0055 );
#define	__Dt__DtFunc_PVRSRVGetDCInfoKM__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0056 );
#define	__Dt__DtFunc_PVRSRVGetDCInfoKM__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0057 );
#define	__Dt__DtFunc_PVRSRVDestroyDCSwapChainKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0058 );
#define	__Dt__DtFunc_PVRSRVDestroyDCSwapChainKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0059 );
#define	__Dt__DtFunc_PVRSRVDestroyDCSwapChainKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005A );
#define	__Dt__DtFunc_PVRSRVDestroyDCSwapChainKM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005B );
#define	__Dt__DtFunc_DestroyDCSwapChain__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x005C );
#define	__Dt__DtFunc_DestroyDCSwapChain__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005D );
#define	__Dt__DtFunc_DestroyDCSwapChain__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005E );
#define	__Dt__DtFunc_DestroyDCSwapChain__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005F );
#define	__Dt__DtFunc_DestroyDCSwapChain__DtStep_4	/*while*/	_TP_BusOut( __DtBaseAddress, 0x0060 );
#define	__Dt__DtFunc_DestroyDCSwapChain__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0061 );
#define	__Dt__DtFunc_DestroyDCSwapChain__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0062 );
#define	__Dt__DtFunc_DestroyDCSwapChain__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0063 );
#define	__Dt__DtFunc_DestroyDCSwapChain__DtStep_8	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0064 );
#define	__Dt__DtFunc_DestroyDCSwapChain__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0065 );
#define	__Dt__DtFunc_DestroyDCSwapChain__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0066 );
#define	__Dt__DtFunc_DestroyDCSwapChain__DtStep_11	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0067 );
#define	__Dt__DtFunc_DestroyDCSwapChainRefCallBack__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0068 );
#define	__Dt__DtFunc_DestroyDCSwapChainRefCallBack__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0069 );
#define	__Dt__DtFunc_DestroyDCSwapChainRefCallBack__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006A );
#define	__Dt__DtFunc_PVRSRVFindSharedDCSwapChainKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x006B );
#define	__Dt__DtFunc_PVRSRVFindSharedDCSwapChainKM__DtStep_1	/*for*/	_TP_BusOut( __DtBaseAddress, 0x006C );
#define	__Dt__DtFunc_PVRSRVFindSharedDCSwapChainKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006D );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainRefKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x006E );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainRefKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006F );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainRefKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0070 );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainRefKM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0071 );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0072 );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0073 );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0074 );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0075 );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0076 );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0077 );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0078 );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0079 );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007A );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007B );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_10	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007C );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_11	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007D );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_12	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007E );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_13	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007F );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_14	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0081 );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_15	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0082 );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_16	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0083 );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_17	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0084 );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_18	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0085 );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_19	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0086 );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_20	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0087 );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_21	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0088 );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_22	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0089 );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_23	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008A );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_24	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x008B );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_25	/*for*/	_TP_BusOut( __DtBaseAddress, 0x008C );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_26	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008D );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_27	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008E );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_28	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008F );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_29	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0091 );
#define	__Dt__DtFunc_PVRSRVCreateDCSwapChainKM__DtStep_30	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0092 );
#define	__Dt__DtFunc_PVRSRVSetDCDstRectKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0093 );
#define	__Dt__DtFunc_PVRSRVSetDCDstRectKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0094 );
#define	__Dt__DtFunc_PVRSRVSetDCDstRectKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0095 );
#define	__Dt__DtFunc_PVRSRVSetDCDstRectKM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0096 );
#define	__Dt__DtFunc_PVRSRVSetDCSrcRectKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0097 );
#define	__Dt__DtFunc_PVRSRVSetDCSrcRectKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0098 );
#define	__Dt__DtFunc_PVRSRVSetDCSrcRectKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0099 );
#define	__Dt__DtFunc_PVRSRVSetDCSrcRectKM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x009A );
#define	__Dt__DtFunc_PVRSRVSetDCDstColourKeyKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x009B );
#define	__Dt__DtFunc_PVRSRVSetDCDstColourKeyKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009C );
#define	__Dt__DtFunc_PVRSRVSetDCDstColourKeyKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x009D );
#define	__Dt__DtFunc_PVRSRVSetDCDstColourKeyKM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x009E );
#define	__Dt__DtFunc_PVRSRVSetDCSrcColourKeyKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x009F );
#define	__Dt__DtFunc_PVRSRVSetDCSrcColourKeyKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A1 );
#define	__Dt__DtFunc_PVRSRVSetDCSrcColourKeyKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A2 );
#define	__Dt__DtFunc_PVRSRVSetDCSrcColourKeyKM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A3 );
#define	__Dt__DtFunc_PVRSRVGetDCBuffersKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00A4 );
#define	__Dt__DtFunc_PVRSRVGetDCBuffersKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A5 );
#define	__Dt__DtFunc_PVRSRVGetDCBuffersKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A6 );
#define	__Dt__DtFunc_PVRSRVGetDCBuffersKM__DtStep_3	/*for*/	_TP_BusOut( __DtBaseAddress, 0x00A7 );
#define	__Dt__DtFunc_PVRSRVGetDCBuffersKM__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A8 );
#define	__Dt__DtFunc_PVRSRVSwapToDCBufferKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00A9 );
#define	__Dt__DtFunc_PVRSRVSwapToDCBufferKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00AA );
#define	__Dt__DtFunc_PVRSRVSwapToDCBufferKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00AB );
#define	__Dt__DtFunc_PVRSRVSwapToDCBufferKM__DtStep_3	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00AC );
#define	__Dt__DtFunc_PVRSRVSwapToDCBufferKM__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00AD );
#define	__Dt__DtFunc_PVRSRVSwapToDCBufferKM__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00AE );
#define	__Dt__DtFunc_PVRSRVSwapToDCBufferKM__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00AF );
#define	__Dt__DtFunc_PVRSRVSwapToDCBufferKM__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B1 );
#define	__Dt__DtFunc_PVRSRVSwapToDCBufferKM__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B2 );
#define	__Dt__DtFunc_PVRSRVSwapToDCBufferKM__DtStep_9	/*for*/	_TP_BusOut( __DtBaseAddress, 0x00B3 );
#define	__Dt__DtFunc_PVRSRVSwapToDCBufferKM__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B4 );
#define	__Dt__DtFunc_PVRSRVSwapToDCBufferKM__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B5 );
#define	__Dt__DtFunc_PVRSRVSwapToDCBufferKM__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B6 );
#define	__Dt__DtFunc_PVRSRVSwapToDCSystemKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00B7 );
#define	__Dt__DtFunc_PVRSRVSwapToDCSystemKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B8 );
#define	__Dt__DtFunc_PVRSRVSwapToDCSystemKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B9 );
#define	__Dt__DtFunc_PVRSRVSwapToDCSystemKM__DtStep_3	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BA );
#define	__Dt__DtFunc_PVRSRVSwapToDCSystemKM__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00BB );
#define	__Dt__DtFunc_PVRSRVSwapToDCSystemKM__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00BC );
#define	__Dt__DtFunc_PVRSRVSwapToDCSystemKM__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00BD );
#define	__Dt__DtFunc_PVRSRVSwapToDCSystemKM__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00BE );
#define	__Dt__DtFunc_PVRSRVSwapToDCSystemKM__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00BF );
#define	__Dt__DtFunc_PVRSRVSwapToDCSystemKM__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C0 );
#define	__Dt__DtFunc_PVRSRVSwapToDCSystemKM__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C1 );
#define	__Dt__DtFunc_PVRSRVRegisterSystemISRHandler__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00C2 );
#define	__Dt__DtFunc_PVRSRVRegisterSystemISRHandler__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C3 );
#define	__Dt__DtFunc_PVRSRVRegisterSystemISRHandler__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C4 );
#define	__Dt__DtFunc_PVRSRVRegisterSystemISRHandler__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C5 );
#define	__Dt__DtFunc_PVRSRVSetDCState_ForEachVaCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00C6 );
#define	__Dt__DtFunc_PVRSRVSetDCState_ForEachVaCb__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C7 );
#define	__Dt__DtFunc_PVRSRVSetDCState_ForEachVaCb__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C8 );
#define	__Dt__DtFunc_PVRSRVSetDCState_ForEachVaCb__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C9 );
#define	__Dt__DtFunc_PVRSRVSetDCState__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00CA );
#define	__Dt__DtFunc_PVRSRVSetDCState__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CB );
#define	__Dt__DtFunc_PVRGetDisplayClassJTable__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00CC );
#define	__Dt__DtFunc_PVRSRVCloseBCDeviceKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00CD );
#define	__Dt__DtFunc_PVRSRVCloseBCDeviceKM__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CE );
#define	__Dt__DtFunc_CloseBCDeviceCallBack__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00CF );
#define	__Dt__DtFunc_CloseBCDeviceCallBack__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D0 );
#define	__Dt__DtFunc_CloseBCDeviceCallBack__DtStep_2	/*for*/	_TP_BusOut( __DtBaseAddress, 0x00D1 );
#define	__Dt__DtFunc_CloseBCDeviceCallBack__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D2 );
#define	__Dt__DtFunc_CloseBCDeviceCallBack__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D3 );
#define	__Dt__DtFunc_CloseBCDeviceCallBack__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D4 );
#define	__Dt__DtFunc_CloseBCDeviceCallBack__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D5 );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00D6 );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D7 );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D8 );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D9 );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceKM__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DA );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceKM__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00DB );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceKM__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DC );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceKM__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00DD );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceKM__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00DE );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceKM__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DF );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceKM__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E0 );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceKM__DtStep_11	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E1 );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceKM__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E2 );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceKM__DtStep_13	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E3 );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceKM__DtStep_14	/*for*/	_TP_BusOut( __DtBaseAddress, 0x00E4 );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceKM__DtStep_15	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E5 );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceKM__DtStep_16	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E6 );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceKM__DtStep_17	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E7 );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceKM__DtStep_18	/*for*/	_TP_BusOut( __DtBaseAddress, 0x00E8 );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceKM__DtStep_19	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E9 );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceKM__DtStep_20	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00EA );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceKM__DtStep_21	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00EB );
#define	__Dt__DtFunc_PVRSRVOpenBCDeviceKM__DtStep_22	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00EC );
#define	__Dt__DtFunc_PVRSRVGetBCInfoKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00ED );
#define	__Dt__DtFunc_PVRSRVGetBCInfoKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00EE );
#define	__Dt__DtFunc_PVRSRVGetBCInfoKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00EF );
#define	__Dt__DtFunc_PVRSRVGetBCInfoKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F1 );
#define	__Dt__DtFunc_PVRSRVGetBCInfoKM__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F2 );
#define	__Dt__DtFunc_PVRSRVGetBCInfoKM__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F3 );
#define	__Dt__DtFunc_PVRSRVGetBCBufferKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00F4 );
#define	__Dt__DtFunc_PVRSRVGetBCBufferKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F5 );
#define	__Dt__DtFunc_PVRSRVGetBCBufferKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F6 );
#define	__Dt__DtFunc_PVRSRVGetBCBufferKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F7 );
#define	__Dt__DtFunc_PVRSRVGetBCBufferKM__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F8 );
#define	__Dt__DtFunc_PVRSRVGetBCBufferKM__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F9 );
#define	__Dt__DtFunc_PVRSRVGetBCBufferKM__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00FA );
#define	__Dt__DtFunc_PVRGetBufferClassJTable__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00FB );
#define	__Dt__DtFunc_PVRGetBufferClassJTable__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00FC );

#endif	/* __DT_deviceclass_h__ */

