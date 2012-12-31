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

#ifndef	__DT_sysconfig_h__
#define	__DT_sysconfig_h__

#ifdef	__cplusplus
	#define	__DtEXTERN	extern "C"
#else
	#define	__DtEXTERN	extern
#endif

/* TestPoint MacroCode -----------------------------------------------------*/
#define		__DtBaseAddress		0x260
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
#define		__DtFunc_EnableSGXClocks		0
#define		__DtFunc_DisableSGXClocks		1
#define		__DtFunc_SysLocateDevices		2
#define		__DtFunc_SysInitialise		3
#define		__DtFunc_SysFinalise		4
#define		__DtFunc_SysDeinitialise		5
#define		__DtFunc_SysGetDeviceMemoryMap		6
#define		__DtFunc_SysCpuPAddrToDevPAddr		7
#define		__DtFunc_SysSysPAddrToCpuPAddr		8
#define		__DtFunc_SysCpuPAddrToSysPAddr		9
#define		__DtFunc_SysSysPAddrToDevPAddr		10
#define		__DtFunc_SysDevPAddrToSysPAddr		11
#define		__DtFunc_SysRegisterExternalDevice		12
#define		__DtFunc_SysRemoveExternalDevice		13
#define		__DtFunc_SysGetInterruptSource		14
#define		__DtFunc_SysClearInterrupts		15
#define		__DtFunc_SysSystemPrePowerState		16
#define		__DtFunc_SysSystemPostPowerState		17
#define		__DtFunc_SysDevicePrePowerState		18
#define		__DtFunc_SysDevicePostPowerState		19
#define		__DtFunc_SysOEMFunction		20
#define		__DtFunc_SysPowerLockWrap		21
#define		__DtFunc_SysPowerLockUnwrap		22

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
#define		__DtStep_31		31
#define		__DtStep_32		32
#define		__DtStep_33		33
#define		__DtStep_34		34

/* TestPoint DisableList ---------------------------------------------------*/
#define	__Dt__DtFunc_EnableSGXClocks__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0000 );
#define	__Dt__DtFunc_EnableSGXClocks__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0001 );
#define	__Dt__DtFunc_DisableSGXClocks__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0002 );
#define	__Dt__DtFunc_DisableSGXClocks__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0003 );
#define	__Dt__DtFunc_SysLocateDevices__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0004 );
#define	__Dt__DtFunc_SysLocateDevices__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0005 );
#define	__Dt__DtFunc_SysInitialise__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0006 );
#define	__Dt__DtFunc_SysInitialise__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0007 );
#define	__Dt__DtFunc_SysInitialise__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0008 );
#define	__Dt__DtFunc_SysInitialise__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0009 );
#define	__Dt__DtFunc_SysInitialise__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000A );
#define	__Dt__DtFunc_SysInitialise__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000B );
#define	__Dt__DtFunc_SysInitialise__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000C );
#define	__Dt__DtFunc_SysInitialise__DtStep_7	/*for*/	_TP_BusOut( __DtBaseAddress, 0x000D );
#define	__Dt__DtFunc_SysInitialise__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000E );
#define	__Dt__DtFunc_SysInitialise__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000F );
#define	__Dt__DtFunc_SysInitialise__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0010 );
#define	__Dt__DtFunc_SysInitialise__DtStep_11	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0011 );
#define	__Dt__DtFunc_SysInitialise__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0012 );
#define	__Dt__DtFunc_SysInitialise__DtStep_13	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0013 );
#define	__Dt__DtFunc_SysInitialise__DtStep_14	/*while*/	_TP_BusOut( __DtBaseAddress, 0x0014 );
#define	__Dt__DtFunc_SysInitialise__DtStep_15	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0015 );
#define	__Dt__DtFunc_SysInitialise__DtStep_16	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0016 );
#define	__Dt__DtFunc_SysInitialise__DtStep_17	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0017 );
#define	__Dt__DtFunc_SysInitialise__DtStep_18	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0018 );
#define	__Dt__DtFunc_SysInitialise__DtStep_19	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0019 );
#define	__Dt__DtFunc_SysInitialise__DtStep_20	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x001A );
#define	__Dt__DtFunc_SysInitialise__DtStep_21	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x001B );
#define	__Dt__DtFunc_SysInitialise__DtStep_22	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x001C );
#define	__Dt__DtFunc_SysInitialise__DtStep_23	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x001D );
#define	__Dt__DtFunc_SysInitialise__DtStep_24	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x001E );
#define	__Dt__DtFunc_SysInitialise__DtStep_25	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x001F );
#define	__Dt__DtFunc_SysInitialise__DtStep_26	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0020 );
#define	__Dt__DtFunc_SysInitialise__DtStep_27	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0021 );
#define	__Dt__DtFunc_SysInitialise__DtStep_28	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0022 );
#define	__Dt__DtFunc_SysInitialise__DtStep_29	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0023 );
#define	__Dt__DtFunc_SysInitialise__DtStep_30	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0024 );
#define	__Dt__DtFunc_SysInitialise__DtStep_31	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0025 );
#define	__Dt__DtFunc_SysInitialise__DtStep_32	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0026 );
#define	__Dt__DtFunc_SysInitialise__DtStep_33	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0027 );
#define	__Dt__DtFunc_SysInitialise__DtStep_34	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0028 );
#define	__Dt__DtFunc_SysFinalise__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002A );
#define	__Dt__DtFunc_SysFinalise__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002B );
#define	__Dt__DtFunc_SysFinalise__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002C );
#define	__Dt__DtFunc_SysFinalise__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002D );
#define	__Dt__DtFunc_SysFinalise__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002E );
#define	__Dt__DtFunc_SysFinalise__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002F );
#define	__Dt__DtFunc_SysFinalise__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0031 );
#define	__Dt__DtFunc_SysFinalise__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0032 );
#define	__Dt__DtFunc_SysDeinitialise__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0033 );
#define	__Dt__DtFunc_SysDeinitialise__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0034 );
#define	__Dt__DtFunc_SysDeinitialise__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0035 );
#define	__Dt__DtFunc_SysDeinitialise__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0036 );
#define	__Dt__DtFunc_SysDeinitialise__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0037 );
#define	__Dt__DtFunc_SysDeinitialise__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0038 );
#define	__Dt__DtFunc_SysDeinitialise__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0039 );
#define	__Dt__DtFunc_SysDeinitialise__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003A );
#define	__Dt__DtFunc_SysDeinitialise__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003B );
#define	__Dt__DtFunc_SysDeinitialise__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003C );
#define	__Dt__DtFunc_SysDeinitialise__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003D );
#define	__Dt__DtFunc_SysDeinitialise__DtStep_11	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003E );
#define	__Dt__DtFunc_SysDeinitialise__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003F );
#define	__Dt__DtFunc_SysDeinitialise__DtStep_13	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0040 );
#define	__Dt__DtFunc_SysDeinitialise__DtStep_14	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0041 );
#define	__Dt__DtFunc_SysGetDeviceMemoryMap__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0042 );
#define	__Dt__DtFunc_SysGetDeviceMemoryMap__DtStep_1	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0043 );
#define	__Dt__DtFunc_SysGetDeviceMemoryMap__DtStep_2	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0044 );
#define	__Dt__DtFunc_SysGetDeviceMemoryMap__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0045 );
#define	__Dt__DtFunc_SysCpuPAddrToDevPAddr__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0046 );
#define	__Dt__DtFunc_SysCpuPAddrToDevPAddr__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0047 );
#define	__Dt__DtFunc_SysSysPAddrToCpuPAddr__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0048 );
#define	__Dt__DtFunc_SysSysPAddrToCpuPAddr__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0049 );
#define	__Dt__DtFunc_SysCpuPAddrToSysPAddr__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x004A );
#define	__Dt__DtFunc_SysCpuPAddrToSysPAddr__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004B );
#define	__Dt__DtFunc_SysSysPAddrToDevPAddr__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x004C );
#define	__Dt__DtFunc_SysSysPAddrToDevPAddr__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004D );
#define	__Dt__DtFunc_SysDevPAddrToSysPAddr__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x004E );
#define	__Dt__DtFunc_SysDevPAddrToSysPAddr__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004F );
#define	__Dt__DtFunc_SysRegisterExternalDevice__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0050 );
#define	__Dt__DtFunc_SysRegisterExternalDevice__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0051 );
#define	__Dt__DtFunc_SysRemoveExternalDevice__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0052 );
#define	__Dt__DtFunc_SysRemoveExternalDevice__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0053 );
#define	__Dt__DtFunc_SysGetInterruptSource__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0054 );
#define	__Dt__DtFunc_SysClearInterrupts__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0055 );
#define	__Dt__DtFunc_SysClearInterrupts__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0056 );
#define	__Dt__DtFunc_SysSystemPrePowerState__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0057 );
#define	__Dt__DtFunc_SysSystemPostPowerState__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0058 );
#define	__Dt__DtFunc_SysDevicePrePowerState__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0059 );
#define	__Dt__DtFunc_SysDevicePrePowerState__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005A );
#define	__Dt__DtFunc_SysDevicePrePowerState__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005B );
#define	__Dt__DtFunc_SysDevicePostPowerState__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x005C );
#define	__Dt__DtFunc_SysDevicePostPowerState__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005D );
#define	__Dt__DtFunc_SysDevicePostPowerState__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005E );
#define	__Dt__DtFunc_SysOEMFunction__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x005F );
#define	__Dt__DtFunc_SysOEMFunction__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0060 );
#define	__Dt__DtFunc_SysOEMFunction__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0061 );
#define	__Dt__DtFunc_SysOEMFunction__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0062 );
#define	__Dt__DtFunc_SysPowerLockWrap__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0063 );
#define	__Dt__DtFunc_SysPowerLockUnwrap__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0064 );

#endif	/* __DT_sysconfig_h__ */

