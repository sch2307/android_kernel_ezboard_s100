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

#ifndef	__DT_pvrsrv_h__
#define	__DT_pvrsrv_h__

#ifdef	__cplusplus
	#define	__DtEXTERN	extern "C"
#else
	#define	__DtEXTERN	extern
#endif

/* TestPoint MacroCode -----------------------------------------------------*/
#define		__DtBaseAddress		0x210
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
#define		__DtFunc_AllocateDeviceID		0
#define		__DtFunc_FreeDeviceID		1
#define		__DtFunc_ReadHWReg		2
#define		__DtFunc_WriteHWReg		3
#define		__DtFunc_WriteHWRegs		4
#define		__DtFunc_PVRSRVEnumerateDevicesKM_ForEachVaCb		5
#define		__DtFunc_PVRSRVEnumerateDevicesKM		6
#define		__DtFunc_PVRSRVInit		7
#define		__DtFunc_PVRSRVDeInit		8
#define		__DtFunc_PVRSRVRegisterDevice		9
#define		__DtFunc_PVRSRVInitialiseDevice		10
#define		__DtFunc_PVRSRVFinaliseSystem_SetPowerState_AnyCb		11
#define		__DtFunc_PVRSRVFinaliseSystem_CompatCheck_AnyCb		12
#define		__DtFunc_PVRSRVFinaliseSystem		13
#define		__DtFunc_PVRSRVDevInitCompatCheck		14
#define		__DtFunc_PVRSRVAcquireDeviceDataKM_Match_AnyVaCb		15
#define		__DtFunc_PVRSRVAcquireDeviceDataKM		16
#define		__DtFunc_PVRSRVDeinitialiseDevice		17
#define		__DtFunc_PollForValueKM		18
#define		__DtFunc_PVRSRVGetMiscInfoKM_RA_GetStats_ForEachVaCb		19
#define		__DtFunc_PVRSRVGetMiscInfoKM_BMContext_AnyVaCb		20
#define		__DtFunc_PVRSRVGetMiscInfoKM_Device_AnyVaCb		21
#define		__DtFunc_PVRSRVGetMiscInfoKM		22
#define		__DtFunc_PVRSRVDeviceLISR		23
#define		__DtFunc_PVRSRVSystemLISR_ForEachVaCb		24
#define		__DtFunc_PVRSRVSystemLISR		25
#define		__DtFunc_PVRSRVMISR_ForEachCb		26
#define		__DtFunc_PVRSRVMISR		27
#define		__DtFunc_PVRSRVProcessConnect		28
#define		__DtFunc_PVRSRVProcessDisconnect		29
#define		__DtFunc_PVRSRVSaveRestoreLiveSegments		30
#define		__DtFunc_PVRSRVGetErrorStringKM		31
#define		__DtFunc_PVRSRVCommandCompleteCallbacks_ForEachCb		32
#define		__DtFunc_PVRSRVScheduleDeviceCallbacks		33
#define		__DtFunc_PVRSRVScheduleDevicesKM		34

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

/* TestPoint DisableList ---------------------------------------------------*/
#define	__Dt__DtFunc_AllocateDeviceID__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0000 );
#define	__Dt__DtFunc_AllocateDeviceID__DtStep_1	/*while*/	_TP_BusOut( __DtBaseAddress, 0x0001 );
#define	__Dt__DtFunc_AllocateDeviceID__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0002 );
#define	__Dt__DtFunc_AllocateDeviceID__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0003 );
#define	__Dt__DtFunc_AllocateDeviceID__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0004 );
#define	__Dt__DtFunc_FreeDeviceID__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0005 );
#define	__Dt__DtFunc_FreeDeviceID__DtStep_1	/*while*/	_TP_BusOut( __DtBaseAddress, 0x0006 );
#define	__Dt__DtFunc_FreeDeviceID__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0007 );
#define	__Dt__DtFunc_FreeDeviceID__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0008 );
#define	__Dt__DtFunc_FreeDeviceID__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0009 );
#define	__Dt__DtFunc_ReadHWReg__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000A );
#define	__Dt__DtFunc_WriteHWReg__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x000B );
#define	__Dt__DtFunc_WriteHWReg__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000C );
#define	__Dt__DtFunc_WriteHWRegs__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x000D );
#define	__Dt__DtFunc_WriteHWRegs__DtStep_1	/*while*/	_TP_BusOut( __DtBaseAddress, 0x000E );
#define	__Dt__DtFunc_WriteHWRegs__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000F );
#define	__Dt__DtFunc_PVRSRVEnumerateDevicesKM_ForEachVaCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0010 );
#define	__Dt__DtFunc_PVRSRVEnumerateDevicesKM_ForEachVaCb__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0011 );
#define	__Dt__DtFunc_PVRSRVEnumerateDevicesKM_ForEachVaCb__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0012 );
#define	__Dt__DtFunc_PVRSRVEnumerateDevicesKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0013 );
#define	__Dt__DtFunc_PVRSRVEnumerateDevicesKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0014 );
#define	__Dt__DtFunc_PVRSRVEnumerateDevicesKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0015 );
#define	__Dt__DtFunc_PVRSRVEnumerateDevicesKM__DtStep_3	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0016 );
#define	__Dt__DtFunc_PVRSRVEnumerateDevicesKM__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0017 );
#define	__Dt__DtFunc_PVRSRVInit__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0018 );
#define	__Dt__DtFunc_PVRSRVInit__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0019 );
#define	__Dt__DtFunc_PVRSRVInit__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001A );
#define	__Dt__DtFunc_PVRSRVInit__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001B );
#define	__Dt__DtFunc_PVRSRVInit__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001C );
#define	__Dt__DtFunc_PVRSRVInit__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001D );
#define	__Dt__DtFunc_PVRSRVInit__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001E );
#define	__Dt__DtFunc_PVRSRVInit__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001F );
#define	__Dt__DtFunc_PVRSRVInit__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0020 );
#define	__Dt__DtFunc_PVRSRVDeInit__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0021 );
#define	__Dt__DtFunc_PVRSRVDeInit__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0022 );
#define	__Dt__DtFunc_PVRSRVDeInit__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0023 );
#define	__Dt__DtFunc_PVRSRVDeInit__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0024 );
#define	__Dt__DtFunc_PVRSRVDeInit__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0025 );
#define	__Dt__DtFunc_PVRSRVDeInit__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0026 );
#define	__Dt__DtFunc_PVRSRVDeInit__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0027 );
#define	__Dt__DtFunc_PVRSRVDeInit__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0028 );
#define	__Dt__DtFunc_PVRSRVRegisterDevice__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x002A );
#define	__Dt__DtFunc_PVRSRVRegisterDevice__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002B );
#define	__Dt__DtFunc_PVRSRVRegisterDevice__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002C );
#define	__Dt__DtFunc_PVRSRVRegisterDevice__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002D );
#define	__Dt__DtFunc_PVRSRVRegisterDevice__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002E );
#define	__Dt__DtFunc_PVRSRVRegisterDevice__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002F );
#define	__Dt__DtFunc_PVRSRVInitialiseDevice__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0031 );
#define	__Dt__DtFunc_PVRSRVInitialiseDevice__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0032 );
#define	__Dt__DtFunc_PVRSRVInitialiseDevice__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0033 );
#define	__Dt__DtFunc_PVRSRVInitialiseDevice__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0034 );
#define	__Dt__DtFunc_PVRSRVInitialiseDevice__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0035 );
#define	__Dt__DtFunc_PVRSRVInitialiseDevice__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0036 );
#define	__Dt__DtFunc_PVRSRVInitialiseDevice__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0037 );
#define	__Dt__DtFunc_PVRSRVInitialiseDevice__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0038 );
#define	__Dt__DtFunc_PVRSRVInitialiseDevice__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0039 );
#define	__Dt__DtFunc_PVRSRVFinaliseSystem_SetPowerState_AnyCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x003A );
#define	__Dt__DtFunc_PVRSRVFinaliseSystem_SetPowerState_AnyCb__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003B );
#define	__Dt__DtFunc_PVRSRVFinaliseSystem_SetPowerState_AnyCb__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003C );
#define	__Dt__DtFunc_PVRSRVFinaliseSystem_CompatCheck_AnyCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x003D );
#define	__Dt__DtFunc_PVRSRVFinaliseSystem_CompatCheck_AnyCb__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003E );
#define	__Dt__DtFunc_PVRSRVFinaliseSystem_CompatCheck_AnyCb__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003F );
#define	__Dt__DtFunc_PVRSRVFinaliseSystem__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0040 );
#define	__Dt__DtFunc_PVRSRVFinaliseSystem__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0041 );
#define	__Dt__DtFunc_PVRSRVFinaliseSystem__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0042 );
#define	__Dt__DtFunc_PVRSRVFinaliseSystem__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0043 );
#define	__Dt__DtFunc_PVRSRVFinaliseSystem__DtStep_4	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0044 );
#define	__Dt__DtFunc_PVRSRVFinaliseSystem__DtStep_5	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0045 );
#define	__Dt__DtFunc_PVRSRVFinaliseSystem__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0046 );
#define	__Dt__DtFunc_PVRSRVDevInitCompatCheck__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0047 );
#define	__Dt__DtFunc_PVRSRVDevInitCompatCheck__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0048 );
#define	__Dt__DtFunc_PVRSRVAcquireDeviceDataKM_Match_AnyVaCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0049 );
#define	__Dt__DtFunc_PVRSRVAcquireDeviceDataKM_Match_AnyVaCb__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004A );
#define	__Dt__DtFunc_PVRSRVAcquireDeviceDataKM_Match_AnyVaCb__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004B );
#define	__Dt__DtFunc_PVRSRVAcquireDeviceDataKM_Match_AnyVaCb__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004C );
#define	__Dt__DtFunc_PVRSRVAcquireDeviceDataKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x004D );
#define	__Dt__DtFunc_PVRSRVAcquireDeviceDataKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004E );
#define	__Dt__DtFunc_PVRSRVAcquireDeviceDataKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004F );
#define	__Dt__DtFunc_PVRSRVAcquireDeviceDataKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0050 );
#define	__Dt__DtFunc_PVRSRVAcquireDeviceDataKM__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0051 );
#define	__Dt__DtFunc_PVRSRVDeinitialiseDevice__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0052 );
#define	__Dt__DtFunc_PVRSRVDeinitialiseDevice__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0053 );
#define	__Dt__DtFunc_PVRSRVDeinitialiseDevice__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0054 );
#define	__Dt__DtFunc_PVRSRVDeinitialiseDevice__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0055 );
#define	__Dt__DtFunc_PVRSRVDeinitialiseDevice__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0056 );
#define	__Dt__DtFunc_PVRSRVDeinitialiseDevice__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0057 );
#define	__Dt__DtFunc_PVRSRVDeinitialiseDevice__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0058 );
#define	__Dt__DtFunc_PVRSRVDeinitialiseDevice__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0059 );
#define	__Dt__DtFunc_PVRSRVDeinitialiseDevice__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005A );
#define	__Dt__DtFunc_PVRSRVDeinitialiseDevice__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005B );
#define	__Dt__DtFunc_PVRSRVDeinitialiseDevice__DtStep_10	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005C );
#define	__Dt__DtFunc_PollForValueKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x005D );
#define	__Dt__DtFunc_PollForValueKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005E );
#define	__Dt__DtFunc_PollForValueKM__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005F );
#define	__Dt__DtFunc_PollForValueKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0060 );
#define	__Dt__DtFunc_PollForValueKM__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0061 );
#define	__Dt__DtFunc_PollForValueKM__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0062 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM_RA_GetStats_ForEachVaCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0063 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM_RA_GetStats_ForEachVaCb__DtStep_1	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0064 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM_RA_GetStats_ForEachVaCb__DtStep_2	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0065 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM_RA_GetStats_ForEachVaCb__DtStep_3	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0066 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM_RA_GetStats_ForEachVaCb__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0067 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM_RA_GetStats_ForEachVaCb__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0068 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM_RA_GetStats_ForEachVaCb__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0069 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM_BMContext_AnyVaCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x006A );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM_BMContext_AnyVaCb__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006B );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM_Device_AnyVaCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x006C );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM_Device_AnyVaCb__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006D );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM_Device_AnyVaCb__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006E );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x006F );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0070 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0071 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0072 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0073 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0074 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0075 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0076 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0077 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_9	/*while*/	_TP_BusOut( __DtBaseAddress, 0x0078 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0079 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007A );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007B );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_13	/*for*/	_TP_BusOut( __DtBaseAddress, 0x007C );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_14	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007D );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_15	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007E );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_16	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007F );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_17	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0081 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_18	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0082 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_19	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0083 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_20	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0084 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_21	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0085 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_22	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0086 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_23	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0087 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_24	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0088 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_25	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0089 );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_26	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008A );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_27	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x008B );
#define	__Dt__DtFunc_PVRSRVGetMiscInfoKM__DtStep_28	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008C );
#define	__Dt__DtFunc_PVRSRVDeviceLISR__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x008D );
#define	__Dt__DtFunc_PVRSRVDeviceLISR__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008E );
#define	__Dt__DtFunc_PVRSRVDeviceLISR__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008F );
#define	__Dt__DtFunc_PVRSRVDeviceLISR__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0091 );
#define	__Dt__DtFunc_PVRSRVDeviceLISR__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0092 );
#define	__Dt__DtFunc_PVRSRVSystemLISR_ForEachVaCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0093 );
#define	__Dt__DtFunc_PVRSRVSystemLISR_ForEachVaCb__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0094 );
#define	__Dt__DtFunc_PVRSRVSystemLISR_ForEachVaCb__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0095 );
#define	__Dt__DtFunc_PVRSRVSystemLISR_ForEachVaCb__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0096 );
#define	__Dt__DtFunc_PVRSRVSystemLISR_ForEachVaCb__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0097 );
#define	__Dt__DtFunc_PVRSRVSystemLISR__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0098 );
#define	__Dt__DtFunc_PVRSRVSystemLISR__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0099 );
#define	__Dt__DtFunc_PVRSRVSystemLISR__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009A );
#define	__Dt__DtFunc_PVRSRVSystemLISR__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009B );
#define	__Dt__DtFunc_PVRSRVSystemLISR__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x009C );
#define	__Dt__DtFunc_PVRSRVMISR_ForEachCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x009D );
#define	__Dt__DtFunc_PVRSRVMISR_ForEachCb__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009E );
#define	__Dt__DtFunc_PVRSRVMISR_ForEachCb__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x009F );
#define	__Dt__DtFunc_PVRSRVMISR__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00A1 );
#define	__Dt__DtFunc_PVRSRVMISR__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A2 );
#define	__Dt__DtFunc_PVRSRVMISR__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A3 );
#define	__Dt__DtFunc_PVRSRVMISR__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A4 );
#define	__Dt__DtFunc_PVRSRVMISR__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A5 );
#define	__Dt__DtFunc_PVRSRVMISR__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A6 );
#define	__Dt__DtFunc_PVRSRVMISR__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A7 );
#define	__Dt__DtFunc_PVRSRVProcessConnect__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A8 );
#define	__Dt__DtFunc_PVRSRVProcessDisconnect__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00A9 );
#define	__Dt__DtFunc_PVRSRVProcessDisconnect__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00AA );
#define	__Dt__DtFunc_PVRSRVSaveRestoreLiveSegments__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00AB );
#define	__Dt__DtFunc_PVRSRVSaveRestoreLiveSegments__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00AC );
#define	__Dt__DtFunc_PVRSRVSaveRestoreLiveSegments__DtStep_2	/*while*/	_TP_BusOut( __DtBaseAddress, 0x00AD );
#define	__Dt__DtFunc_PVRSRVSaveRestoreLiveSegments__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00AE );
#define	__Dt__DtFunc_PVRSRVSaveRestoreLiveSegments__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00AF );
#define	__Dt__DtFunc_PVRSRVSaveRestoreLiveSegments__DtStep_5	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B1 );
#define	__Dt__DtFunc_PVRSRVSaveRestoreLiveSegments__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B2 );
#define	__Dt__DtFunc_PVRSRVSaveRestoreLiveSegments__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B3 );
#define	__Dt__DtFunc_PVRSRVSaveRestoreLiveSegments__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B4 );
#define	__Dt__DtFunc_PVRSRVSaveRestoreLiveSegments__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B5 );
#define	__Dt__DtFunc_PVRSRVSaveRestoreLiveSegments__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B6 );
#define	__Dt__DtFunc_PVRSRVSaveRestoreLiveSegments__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B7 );
#define	__Dt__DtFunc_PVRSRVSaveRestoreLiveSegments__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B8 );
#define	__Dt__DtFunc_PVRSRVSaveRestoreLiveSegments__DtStep_13	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B9 );
#define	__Dt__DtFunc_PVRSRVGetErrorStringKM__DtStep_0	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BA );
#define	__Dt__DtFunc_PVRSRVCommandCompleteCallbacks_ForEachCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00BB );
#define	__Dt__DtFunc_PVRSRVCommandCompleteCallbacks_ForEachCb__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00BC );
#define	__Dt__DtFunc_PVRSRVCommandCompleteCallbacks_ForEachCb__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BD );
#define	__Dt__DtFunc_PVRSRVScheduleDeviceCallbacks__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00BE );
#define	__Dt__DtFunc_PVRSRVScheduleDeviceCallbacks__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BF );
#define	__Dt__DtFunc_PVRSRVScheduleDevicesKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00C0 );
#define	__Dt__DtFunc_PVRSRVScheduleDevicesKM__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C1 );

#endif	/* __DT_pvrsrv_h__ */

