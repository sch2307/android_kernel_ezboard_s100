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

#ifndef	__DT_power_h__
#define	__DT_power_h__

#ifdef	__cplusplus
	#define	__DtEXTERN	extern "C"
#else
	#define	__DtEXTERN	extern
#endif

/* TestPoint MacroCode -----------------------------------------------------*/
#define		__DtBaseAddress		0x1D0
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
#define		__DtFunc_PVRSRVSetInitServerState		0
#define		__DtFunc_PVRSRVGetInitServerState		1
#define		__DtFunc__IsSystemStatePowered		2
#define		__DtFunc_PVRSRVPowerLock		3
#define		__DtFunc_PVRSRVPowerUnlock		4
#define		__DtFunc_PVRSRVDevicePrePowerStateKM_AnyVaCb		5
#define		__DtFunc_PVRSRVDevicePrePowerStateKM		6
#define		__DtFunc_PVRSRVDevicePostPowerStateKM_AnyVaCb		7
#define		__DtFunc_PVRSRVDevicePostPowerStateKM		8
#define		__DtFunc_PVRSRVSetDevicePowerStateKM		9
#define		__DtFunc_PVRSRVSystemPrePowerStateKM		10
#define		__DtFunc_PVRSRVSystemPostPowerStateKM		11
#define		__DtFunc_PVRSRVSetPowerStateKM		12
#define		__DtFunc_zero_pvr_suspend		13
#define		__DtFunc_zero_pvr_resume		14
#define		__DtFunc_PVRSRVRegisterPowerDevice		15
#define		__DtFunc_PVRSRVRemovePowerDevice		16
#define		__DtFunc_PVRSRVIsDevicePowered		17
#define		__DtFunc_PVRSRVDevicePreClockSpeedChange		18
#define		__DtFunc_PVRSRVDevicePostClockSpeedChange		19

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

/* TestPoint DisableList ---------------------------------------------------*/
#define	__Dt__DtFunc_PVRSRVSetInitServerState__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0000 );
#define	__Dt__DtFunc_PVRSRVSetInitServerState__DtStep_1	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0001 );
#define	__Dt__DtFunc_PVRSRVSetInitServerState__DtStep_2	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0002 );
#define	__Dt__DtFunc_PVRSRVSetInitServerState__DtStep_3	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0003 );
#define	__Dt__DtFunc_PVRSRVSetInitServerState__DtStep_4	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0004 );
#define	__Dt__DtFunc_PVRSRVSetInitServerState__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0005 );
#define	__Dt__DtFunc_PVRSRVSetInitServerState__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0006 );
#define	__Dt__DtFunc_PVRSRVGetInitServerState__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0007 );
#define	__Dt__DtFunc_PVRSRVGetInitServerState__DtStep_1	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0008 );
#define	__Dt__DtFunc_PVRSRVGetInitServerState__DtStep_2	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0009 );
#define	__Dt__DtFunc_PVRSRVGetInitServerState__DtStep_3	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x000A );
#define	__Dt__DtFunc_PVRSRVGetInitServerState__DtStep_4	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x000B );
#define	__Dt__DtFunc_PVRSRVGetInitServerState__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000C );
#define	__Dt__DtFunc__IsSystemStatePowered__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000D );
#define	__Dt__DtFunc_PVRSRVPowerLock__DtStep_0	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000E );
#define	__Dt__DtFunc_PVRSRVPowerLock__DtStep_1	/*do*/	_TP_BusOut( __DtBaseAddress, 0x000F );
#define	__Dt__DtFunc_PVRSRVPowerLock__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0010 );
#define	__Dt__DtFunc_PVRSRVPowerLock__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0011 );
#define	__Dt__DtFunc_PVRSRVPowerLock__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0012 );
#define	__Dt__DtFunc_PVRSRVPowerLock__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0013 );
#define	__Dt__DtFunc_PVRSRVPowerLock__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0014 );
#define	__Dt__DtFunc_PVRSRVPowerUnlock__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0015 );
#define	__Dt__DtFunc_PVRSRVPowerUnlock__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0016 );
#define	__Dt__DtFunc_PVRSRVDevicePrePowerStateKM_AnyVaCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0017 );
#define	__Dt__DtFunc_PVRSRVDevicePrePowerStateKM_AnyVaCb__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0018 );
#define	__Dt__DtFunc_PVRSRVDevicePrePowerStateKM_AnyVaCb__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0019 );
#define	__Dt__DtFunc_PVRSRVDevicePrePowerStateKM_AnyVaCb__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001A );
#define	__Dt__DtFunc_PVRSRVDevicePrePowerStateKM_AnyVaCb__DtStep_4	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001B );
#define	__Dt__DtFunc_PVRSRVDevicePrePowerStateKM_AnyVaCb__DtStep_5	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001C );
#define	__Dt__DtFunc_PVRSRVDevicePrePowerStateKM_AnyVaCb__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001D );
#define	__Dt__DtFunc_PVRSRVDevicePrePowerStateKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x001E );
#define	__Dt__DtFunc_PVRSRVDevicePrePowerStateKM__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001F );
#define	__Dt__DtFunc_PVRSRVDevicePostPowerStateKM_AnyVaCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0020 );
#define	__Dt__DtFunc_PVRSRVDevicePostPowerStateKM_AnyVaCb__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0021 );
#define	__Dt__DtFunc_PVRSRVDevicePostPowerStateKM_AnyVaCb__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0022 );
#define	__Dt__DtFunc_PVRSRVDevicePostPowerStateKM_AnyVaCb__DtStep_3	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0023 );
#define	__Dt__DtFunc_PVRSRVDevicePostPowerStateKM_AnyVaCb__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0024 );
#define	__Dt__DtFunc_PVRSRVDevicePostPowerStateKM_AnyVaCb__DtStep_5	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0025 );
#define	__Dt__DtFunc_PVRSRVDevicePostPowerStateKM_AnyVaCb__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0026 );
#define	__Dt__DtFunc_PVRSRVDevicePostPowerStateKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0027 );
#define	__Dt__DtFunc_PVRSRVDevicePostPowerStateKM__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0028 );
#define	__Dt__DtFunc_PVRSRVSetDevicePowerStateKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x002A );
#define	__Dt__DtFunc_PVRSRVSetDevicePowerStateKM__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002B );
#define	__Dt__DtFunc_PVRSRVSetDevicePowerStateKM__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002C );
#define	__Dt__DtFunc_PVRSRVSetDevicePowerStateKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002D );
#define	__Dt__DtFunc_PVRSRVSetDevicePowerStateKM__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002E );
#define	__Dt__DtFunc_PVRSRVSetDevicePowerStateKM__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002F );
#define	__Dt__DtFunc_PVRSRVSetDevicePowerStateKM__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0031 );
#define	__Dt__DtFunc_PVRSRVSetDevicePowerStateKM__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0032 );
#define	__Dt__DtFunc_PVRSRVSetDevicePowerStateKM__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0033 );
#define	__Dt__DtFunc_PVRSRVSetDevicePowerStateKM__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0034 );
#define	__Dt__DtFunc_PVRSRVSetDevicePowerStateKM__DtStep_10	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0035 );
#define	__Dt__DtFunc_PVRSRVSystemPrePowerStateKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0036 );
#define	__Dt__DtFunc_PVRSRVSystemPrePowerStateKM__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0037 );
#define	__Dt__DtFunc_PVRSRVSystemPrePowerStateKM__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0038 );
#define	__Dt__DtFunc_PVRSRVSystemPrePowerStateKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0039 );
#define	__Dt__DtFunc_PVRSRVSystemPrePowerStateKM__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003A );
#define	__Dt__DtFunc_PVRSRVSystemPrePowerStateKM__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003B );
#define	__Dt__DtFunc_PVRSRVSystemPrePowerStateKM__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003C );
#define	__Dt__DtFunc_PVRSRVSystemPrePowerStateKM__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003D );
#define	__Dt__DtFunc_PVRSRVSystemPrePowerStateKM__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003E );
#define	__Dt__DtFunc_PVRSRVSystemPrePowerStateKM__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003F );
#define	__Dt__DtFunc_PVRSRVSystemPostPowerStateKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0040 );
#define	__Dt__DtFunc_PVRSRVSystemPostPowerStateKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0041 );
#define	__Dt__DtFunc_PVRSRVSystemPostPowerStateKM__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0042 );
#define	__Dt__DtFunc_PVRSRVSystemPostPowerStateKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0043 );
#define	__Dt__DtFunc_PVRSRVSystemPostPowerStateKM__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0044 );
#define	__Dt__DtFunc_PVRSRVSystemPostPowerStateKM__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0045 );
#define	__Dt__DtFunc_PVRSRVSystemPostPowerStateKM__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0046 );
#define	__Dt__DtFunc_PVRSRVSystemPostPowerStateKM__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0047 );
#define	__Dt__DtFunc_PVRSRVSystemPostPowerStateKM__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0048 );
#define	__Dt__DtFunc_PVRSRVSetPowerStateKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0049 );
#define	__Dt__DtFunc_PVRSRVSetPowerStateKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004A );
#define	__Dt__DtFunc_PVRSRVSetPowerStateKM__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004B );
#define	__Dt__DtFunc_PVRSRVSetPowerStateKM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004C );
#define	__Dt__DtFunc_PVRSRVSetPowerStateKM__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004D );
#define	__Dt__DtFunc_zero_pvr_suspend__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x004E );
#define	__Dt__DtFunc_zero_pvr_suspend__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004F );
#define	__Dt__DtFunc_zero_pvr_resume__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0050 );
#define	__Dt__DtFunc_zero_pvr_resume__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0051 );
#define	__Dt__DtFunc_PVRSRVRegisterPowerDevice__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0052 );
#define	__Dt__DtFunc_PVRSRVRegisterPowerDevice__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0053 );
#define	__Dt__DtFunc_PVRSRVRegisterPowerDevice__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0054 );
#define	__Dt__DtFunc_PVRSRVRegisterPowerDevice__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0055 );
#define	__Dt__DtFunc_PVRSRVRegisterPowerDevice__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0056 );
#define	__Dt__DtFunc_PVRSRVRemovePowerDevice__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0057 );
#define	__Dt__DtFunc_PVRSRVRemovePowerDevice__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0058 );
#define	__Dt__DtFunc_PVRSRVRemovePowerDevice__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0059 );
#define	__Dt__DtFunc_PVRSRVIsDevicePowered__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x005A );
#define	__Dt__DtFunc_PVRSRVIsDevicePowered__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005B );
#define	__Dt__DtFunc_PVRSRVIsDevicePowered__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005C );
#define	__Dt__DtFunc_PVRSRVDevicePreClockSpeedChange__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x005D );
#define	__Dt__DtFunc_PVRSRVDevicePreClockSpeedChange__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005E );
#define	__Dt__DtFunc_PVRSRVDevicePreClockSpeedChange__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005F );
#define	__Dt__DtFunc_PVRSRVDevicePreClockSpeedChange__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0060 );
#define	__Dt__DtFunc_PVRSRVDevicePreClockSpeedChange__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0061 );
#define	__Dt__DtFunc_PVRSRVDevicePreClockSpeedChange__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0062 );
#define	__Dt__DtFunc_PVRSRVDevicePreClockSpeedChange__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0063 );
#define	__Dt__DtFunc_PVRSRVDevicePreClockSpeedChange__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0064 );
#define	__Dt__DtFunc_PVRSRVDevicePostClockSpeedChange__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0065 );
#define	__Dt__DtFunc_PVRSRVDevicePostClockSpeedChange__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0066 );
#define	__Dt__DtFunc_PVRSRVDevicePostClockSpeedChange__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0067 );
#define	__Dt__DtFunc_PVRSRVDevicePostClockSpeedChange__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0068 );
#define	__Dt__DtFunc_PVRSRVDevicePostClockSpeedChange__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0069 );

#endif	/* __DT_power_h__ */

