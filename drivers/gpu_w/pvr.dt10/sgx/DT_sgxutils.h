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

#ifndef	__DT_sgxutils_h__
#define	__DT_sgxutils_h__

#ifdef	__cplusplus
	#define	__DtEXTERN	extern "C"
#else
	#define	__DtEXTERN	extern
#endif

/* TestPoint MacroCode -----------------------------------------------------*/
#define		__DtBaseAddress		0x310
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
#define		__DtFunc_SGXPostActivePowerEvent		0
#define		__DtFunc_SGXTestActivePowerEvent		1
#define		__DtFunc_SGXAcquireKernelCCBSlot		2
#define		__DtFunc_SGXScheduleCCBCommand		3
#define		__DtFunc_SGXScheduleCCBCommandKM		4
#define		__DtFunc_SGXScheduleProcessQueuesKM		5
#define		__DtFunc_SGXIsDevicePowered		6
#define		__DtFunc_SGXGetInternalDevInfoKM		7
#define		__DtFunc_SGXCleanupRequest		8
#define		__DtFunc_SGXCleanupHWRenderContextCallback		9
#define		__DtFunc_SGXCleanupHWTransferContextCallback		10
#define		__DtFunc_SGXRegisterHWRenderContextKM		11
#define		__DtFunc_SGXUnregisterHWRenderContextKM		12
#define		__DtFunc_SGXRegisterHWTransferContextKM		13
#define		__DtFunc_SGXUnregisterHWTransferContextKM		14
#define		__DtFunc_SGXCleanupHW2DContextCallback		15
#define		__DtFunc_SGXRegisterHW2DContextKM		16
#define		__DtFunc_SGXUnregisterHW2DContextKM		17
#define		__DtFunc_SGX2DQuerySyncOpsComplete		18
#define		__DtFunc_SGX2DQueryBlitsCompleteKM		19
#define		__DtFunc_SGXFlushHWRenderTargetKM		20
#define		__DtFunc_SGXConvertTimeStamp		21

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

/* TestPoint DisableList ---------------------------------------------------*/
#define	__Dt__DtFunc_SGXPostActivePowerEvent__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0000 );
#define	__Dt__DtFunc_SGXPostActivePowerEvent__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0001 );
#define	__Dt__DtFunc_SGXPostActivePowerEvent__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0002 );
#define	__Dt__DtFunc_SGXPostActivePowerEvent__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0003 );
#define	__Dt__DtFunc_SGXPostActivePowerEvent__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0004 );
#define	__Dt__DtFunc_SGXTestActivePowerEvent__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0005 );
#define	__Dt__DtFunc_SGXTestActivePowerEvent__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0006 );
#define	__Dt__DtFunc_SGXTestActivePowerEvent__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0007 );
#define	__Dt__DtFunc_SGXTestActivePowerEvent__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0008 );
#define	__Dt__DtFunc_SGXTestActivePowerEvent__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0009 );
#define	__Dt__DtFunc_SGXTestActivePowerEvent__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000A );
#define	__Dt__DtFunc_SGXAcquireKernelCCBSlot__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x000B );
#define	__Dt__DtFunc_SGXAcquireKernelCCBSlot__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000C );
#define	__Dt__DtFunc_SGXAcquireKernelCCBSlot__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000D );
#define	__Dt__DtFunc_SGXScheduleCCBCommand__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000E );
#define	__Dt__DtFunc_SGXScheduleCCBCommand__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000F );
#define	__Dt__DtFunc_SGXScheduleCCBCommand__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0010 );
#define	__Dt__DtFunc_SGXScheduleCCBCommand__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0011 );
#define	__Dt__DtFunc_SGXScheduleCCBCommand__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0012 );
#define	__Dt__DtFunc_SGXScheduleCCBCommand__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0013 );
#define	__Dt__DtFunc_SGXScheduleCCBCommand__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0014 );
#define	__Dt__DtFunc_SGXScheduleCCBCommand__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0015 );
#define	__Dt__DtFunc_SGXScheduleCCBCommand__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0016 );
#define	__Dt__DtFunc_SGXScheduleCCBCommand__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0017 );
#define	__Dt__DtFunc_SGXScheduleCCBCommand__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0018 );
#define	__Dt__DtFunc_SGXScheduleCCBCommand__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0019 );
#define	__Dt__DtFunc_SGXScheduleCCBCommand__DtStep_12	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001A );
#define	__Dt__DtFunc_SGXScheduleCCBCommandKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x001B );
#define	__Dt__DtFunc_SGXScheduleCCBCommandKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001C );
#define	__Dt__DtFunc_SGXScheduleCCBCommandKM__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001D );
#define	__Dt__DtFunc_SGXScheduleCCBCommandKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001E );
#define	__Dt__DtFunc_SGXScheduleCCBCommandKM__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001F );
#define	__Dt__DtFunc_SGXScheduleCCBCommandKM__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0020 );
#define	__Dt__DtFunc_SGXScheduleCCBCommandKM__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0021 );
#define	__Dt__DtFunc_SGXScheduleCCBCommandKM__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0022 );
#define	__Dt__DtFunc_SGXScheduleCCBCommandKM__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0023 );
#define	__Dt__DtFunc_SGXScheduleCCBCommandKM__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0024 );
#define	__Dt__DtFunc_SGXScheduleProcessQueuesKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0025 );
#define	__Dt__DtFunc_SGXScheduleProcessQueuesKM__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0026 );
#define	__Dt__DtFunc_SGXScheduleProcessQueuesKM__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0027 );
#define	__Dt__DtFunc_SGXScheduleProcessQueuesKM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0028 );
#define	__Dt__DtFunc_SGXScheduleProcessQueuesKM__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002A );
#define	__Dt__DtFunc_SGXIsDevicePowered__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002B );
#define	__Dt__DtFunc_SGXGetInternalDevInfoKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x002C );
#define	__Dt__DtFunc_SGXGetInternalDevInfoKM__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002D );
#define	__Dt__DtFunc_SGXCleanupRequest__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x002E );
#define	__Dt__DtFunc_SGXCleanupRequest__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002F );
#define	__Dt__DtFunc_SGXCleanupRequest__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0031 );
#define	__Dt__DtFunc_SGXCleanupRequest__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0032 );
#define	__Dt__DtFunc_SGXCleanupRequest__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0033 );
#define	__Dt__DtFunc_SGXCleanupRequest__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0034 );
#define	__Dt__DtFunc_SGXCleanupHWRenderContextCallback__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0035 );
#define	__Dt__DtFunc_SGXCleanupHWRenderContextCallback__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0036 );
#define	__Dt__DtFunc_SGXCleanupHWTransferContextCallback__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0037 );
#define	__Dt__DtFunc_SGXCleanupHWTransferContextCallback__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0038 );
#define	__Dt__DtFunc_SGXRegisterHWRenderContextKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0039 );
#define	__Dt__DtFunc_SGXRegisterHWRenderContextKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003A );
#define	__Dt__DtFunc_SGXRegisterHWRenderContextKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003B );
#define	__Dt__DtFunc_SGXRegisterHWRenderContextKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003C );
#define	__Dt__DtFunc_SGXRegisterHWRenderContextKM__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003D );
#define	__Dt__DtFunc_SGXRegisterHWRenderContextKM__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003E );
#define	__Dt__DtFunc_SGXUnregisterHWRenderContextKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x003F );
#define	__Dt__DtFunc_SGXUnregisterHWRenderContextKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0040 );
#define	__Dt__DtFunc_SGXUnregisterHWRenderContextKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0041 );
#define	__Dt__DtFunc_SGXUnregisterHWRenderContextKM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0042 );
#define	__Dt__DtFunc_SGXRegisterHWTransferContextKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0043 );
#define	__Dt__DtFunc_SGXRegisterHWTransferContextKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0044 );
#define	__Dt__DtFunc_SGXRegisterHWTransferContextKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0045 );
#define	__Dt__DtFunc_SGXRegisterHWTransferContextKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0046 );
#define	__Dt__DtFunc_SGXRegisterHWTransferContextKM__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0047 );
#define	__Dt__DtFunc_SGXRegisterHWTransferContextKM__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0048 );
#define	__Dt__DtFunc_SGXUnregisterHWTransferContextKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0049 );
#define	__Dt__DtFunc_SGXUnregisterHWTransferContextKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004A );
#define	__Dt__DtFunc_SGXUnregisterHWTransferContextKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004B );
#define	__Dt__DtFunc_SGXUnregisterHWTransferContextKM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004C );
#define	__Dt__DtFunc_SGXCleanupHW2DContextCallback__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x004D );
#define	__Dt__DtFunc_SGXCleanupHW2DContextCallback__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004E );
#define	__Dt__DtFunc_SGXRegisterHW2DContextKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x004F );
#define	__Dt__DtFunc_SGXRegisterHW2DContextKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0050 );
#define	__Dt__DtFunc_SGXRegisterHW2DContextKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0051 );
#define	__Dt__DtFunc_SGXRegisterHW2DContextKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0052 );
#define	__Dt__DtFunc_SGXRegisterHW2DContextKM__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0053 );
#define	__Dt__DtFunc_SGXRegisterHW2DContextKM__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0054 );
#define	__Dt__DtFunc_SGXUnregisterHW2DContextKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0055 );
#define	__Dt__DtFunc_SGXUnregisterHW2DContextKM__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0056 );
#define	__Dt__DtFunc_SGXUnregisterHW2DContextKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0057 );
#define	__Dt__DtFunc_SGX2DQuerySyncOpsComplete__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0058 );
#define	__Dt__DtFunc_SGX2DQueryBlitsCompleteKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0059 );
#define	__Dt__DtFunc_SGX2DQueryBlitsCompleteKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005A );
#define	__Dt__DtFunc_SGX2DQueryBlitsCompleteKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005B );
#define	__Dt__DtFunc_SGX2DQueryBlitsCompleteKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005C );
#define	__Dt__DtFunc_SGX2DQueryBlitsCompleteKM__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005D );
#define	__Dt__DtFunc_SGX2DQueryBlitsCompleteKM__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005E );
#define	__Dt__DtFunc_SGX2DQueryBlitsCompleteKM__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005F );
#define	__Dt__DtFunc_SGXFlushHWRenderTargetKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0060 );
#define	__Dt__DtFunc_SGXFlushHWRenderTargetKM__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0061 );
#define	__Dt__DtFunc_SGXConvertTimeStamp__DtStep_0	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0062 );
#define	__Dt__DtFunc_SGXConvertTimeStamp__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0063 );

#endif	/* __DT_sgxutils_h__ */

