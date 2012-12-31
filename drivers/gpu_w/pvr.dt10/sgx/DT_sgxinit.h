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

#ifndef	__DT_sgxinit_h__
#define	__DT_sgxinit_h__

#ifdef	__cplusplus
	#define	__DtEXTERN	extern "C"
#else
	#define	__DtEXTERN	extern
#endif

/* TestPoint MacroCode -----------------------------------------------------*/
#define		__DtBaseAddress		0x2C0
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
#define		__DtFunc_SGXCommandComplete		0
#define		__DtFunc_DeinitDevInfo		1
#define		__DtFunc_InitDevInfo		2
#define		__DtFunc_SGXRunScript		3
#define		__DtFunc_SGXInitialise		4
#define		__DtFunc_SGXDeinitialise		5
#define		__DtFunc_DevInitSGXPart1		6
#define		__DtFunc_SGXGetInfoForSrvinitKM		7
#define		__DtFunc_DevInitSGXPart2KM		8
#define		__DtFunc_DevDeInitSGX		9
#define		__DtFunc_SGXDumpDebugReg		10
#define		__DtFunc_SGXDumpDebugInfo		11
#define		__DtFunc_HWRecoveryResetSGX		12
#define		__DtFunc_SGXOSTimer		13
#define		__DtFunc_SGX_ISRHandler		14
#define		__DtFunc_SGX_MISRHandler		15
#define		__DtFunc_SGX_AllocMemTilingRange		16
#define		__DtFunc_SGX_FreeMemTilingRange		17
#define		__DtFunc_SGXRegisterDevice		18
#define		__DtFunc_SGXResetPDump		19
#define		__DtFunc_SGXGetClientInfoKM		20
#define		__DtFunc_SGXPanic		21
#define		__DtFunc_SGXDevInitCompatCheck		22
#define		__DtFunc_SGXGetMiscInfoUkernel		23
#define		__DtFunc_SGXGetMiscInfoKM		24
#define		__DtFunc_SGXReadHWPerfCBKM		25

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
#define		__DtStep_35		35
#define		__DtStep_36		36
#define		__DtStep_37		37
#define		__DtStep_38		38
#define		__DtStep_39		39
#define		__DtStep_40		40
#define		__DtStep_41		41
#define		__DtStep_42		42
#define		__DtStep_43		43
#define		__DtStep_44		44
#define		__DtStep_45		45
#define		__DtStep_46		46
#define		__DtStep_47		47
#define		__DtStep_48		48

/* TestPoint DisableList ---------------------------------------------------*/
#define	__Dt__DtFunc_SGXCommandComplete__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0000 );
#define	__Dt__DtFunc_SGXCommandComplete__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0001 );
#define	__Dt__DtFunc_SGXCommandComplete__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0002 );
#define	__Dt__DtFunc_DeinitDevInfo__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0003 );
#define	__Dt__DtFunc_DeinitDevInfo__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0004 );
#define	__Dt__DtFunc_DeinitDevInfo__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0005 );
#define	__Dt__DtFunc_InitDevInfo__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0006 );
#define	__Dt__DtFunc_InitDevInfo__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0007 );
#define	__Dt__DtFunc_InitDevInfo__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0008 );
#define	__Dt__DtFunc_InitDevInfo__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0009 );
#define	__Dt__DtFunc_SGXRunScript__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x000A );
#define	__Dt__DtFunc_SGXRunScript__DtStep_1	/*for*/	_TP_BusOut( __DtBaseAddress, 0x000B );
#define	__Dt__DtFunc_SGXRunScript__DtStep_2	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x000C );
#define	__Dt__DtFunc_SGXRunScript__DtStep_3	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x000D );
#define	__Dt__DtFunc_SGXRunScript__DtStep_4	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000E );
#define	__Dt__DtFunc_SGXRunScript__DtStep_5	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x000F );
#define	__Dt__DtFunc_SGXRunScript__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0010 );
#define	__Dt__DtFunc_SGXRunScript__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0011 );
#define	__Dt__DtFunc_SGXInitialise__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0012 );
#define	__Dt__DtFunc_SGXInitialise__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0013 );
#define	__Dt__DtFunc_SGXInitialise__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0014 );
#define	__Dt__DtFunc_SGXInitialise__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0015 );
#define	__Dt__DtFunc_SGXInitialise__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0016 );
#define	__Dt__DtFunc_SGXInitialise__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0017 );
#define	__Dt__DtFunc_SGXInitialise__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0018 );
#define	__Dt__DtFunc_SGXDeinitialise__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0019 );
#define	__Dt__DtFunc_SGXDeinitialise__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001A );
#define	__Dt__DtFunc_SGXDeinitialise__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001B );
#define	__Dt__DtFunc_SGXDeinitialise__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001C );
#define	__Dt__DtFunc_SGXDeinitialise__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001D );
#define	__Dt__DtFunc_DevInitSGXPart1__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x001E );
#define	__Dt__DtFunc_DevInitSGXPart1__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001F );
#define	__Dt__DtFunc_DevInitSGXPart1__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0020 );
#define	__Dt__DtFunc_DevInitSGXPart1__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0021 );
#define	__Dt__DtFunc_DevInitSGXPart1__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0022 );
#define	__Dt__DtFunc_DevInitSGXPart1__DtStep_5	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0023 );
#define	__Dt__DtFunc_DevInitSGXPart1__DtStep_6	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0024 );
#define	__Dt__DtFunc_DevInitSGXPart1__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0025 );
#define	__Dt__DtFunc_DevInitSGXPart1__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0026 );
#define	__Dt__DtFunc_DevInitSGXPart1__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0027 );
#define	__Dt__DtFunc_DevInitSGXPart1__DtStep_10	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0028 );
#define	__Dt__DtFunc_SGXGetInfoForSrvinitKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x002A );
#define	__Dt__DtFunc_SGXGetInfoForSrvinitKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002B );
#define	__Dt__DtFunc_SGXGetInfoForSrvinitKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002C );
#define	__Dt__DtFunc_SGXGetInfoForSrvinitKM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002D );
#define	__Dt__DtFunc_DevInitSGXPart2KM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x002E );
#define	__Dt__DtFunc_DevInitSGXPart2KM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002F );
#define	__Dt__DtFunc_DevInitSGXPart2KM__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0031 );
#define	__Dt__DtFunc_DevInitSGXPart2KM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0032 );
#define	__Dt__DtFunc_DevInitSGXPart2KM__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0033 );
#define	__Dt__DtFunc_DevInitSGXPart2KM__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0034 );
#define	__Dt__DtFunc_DevInitSGXPart2KM__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0035 );
#define	__Dt__DtFunc_DevInitSGXPart2KM__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0036 );
#define	__Dt__DtFunc_DevInitSGXPart2KM__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0037 );
#define	__Dt__DtFunc_DevInitSGXPart2KM__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0038 );
#define	__Dt__DtFunc_DevInitSGXPart2KM__DtStep_10	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0039 );
#define	__Dt__DtFunc_DevInitSGXPart2KM__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003A );
#define	__Dt__DtFunc_DevInitSGXPart2KM__DtStep_12	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003B );
#define	__Dt__DtFunc_DevInitSGXPart2KM__DtStep_13	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003C );
#define	__Dt__DtFunc_DevInitSGXPart2KM__DtStep_14	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003D );
#define	__Dt__DtFunc_DevInitSGXPart2KM__DtStep_15	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003E );
#define	__Dt__DtFunc_DevInitSGXPart2KM__DtStep_16	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003F );
#define	__Dt__DtFunc_DevInitSGXPart2KM__DtStep_17	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0040 );
#define	__Dt__DtFunc_DevInitSGXPart2KM__DtStep_18	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0041 );
#define	__Dt__DtFunc_DevDeInitSGX__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0042 );
#define	__Dt__DtFunc_DevDeInitSGX__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0043 );
#define	__Dt__DtFunc_DevDeInitSGX__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0044 );
#define	__Dt__DtFunc_DevDeInitSGX__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0045 );
#define	__Dt__DtFunc_DevDeInitSGX__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0046 );
#define	__Dt__DtFunc_DevDeInitSGX__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0047 );
#define	__Dt__DtFunc_DevDeInitSGX__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0048 );
#define	__Dt__DtFunc_DevDeInitSGX__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0049 );
#define	__Dt__DtFunc_DevDeInitSGX__DtStep_8	/*for*/	_TP_BusOut( __DtBaseAddress, 0x004A );
#define	__Dt__DtFunc_DevDeInitSGX__DtStep_9	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x004B );
#define	__Dt__DtFunc_DevDeInitSGX__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004C );
#define	__Dt__DtFunc_DevDeInitSGX__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004D );
#define	__Dt__DtFunc_DevDeInitSGX__DtStep_12	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004E );
#define	__Dt__DtFunc_DevDeInitSGX__DtStep_13	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004F );
#define	__Dt__DtFunc_DevDeInitSGX__DtStep_14	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0050 );
#define	__Dt__DtFunc_DevDeInitSGX__DtStep_15	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0051 );
#define	__Dt__DtFunc_DevDeInitSGX__DtStep_16	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0052 );
#define	__Dt__DtFunc_DevDeInitSGX__DtStep_17	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0053 );
#define	__Dt__DtFunc_DevDeInitSGX__DtStep_18	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0054 );
#define	__Dt__DtFunc_DevDeInitSGX__DtStep_19	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0055 );
#define	__Dt__DtFunc_DevDeInitSGX__DtStep_20	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0056 );
#define	__Dt__DtFunc_DevDeInitSGX__DtStep_21	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0057 );
#define	__Dt__DtFunc_SGXDumpDebugReg__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0058 );
#define	__Dt__DtFunc_SGXDumpDebugReg__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0059 );
#define	__Dt__DtFunc_SGXDumpDebugInfo__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x005A );
#define	__Dt__DtFunc_SGXDumpDebugInfo__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005B );
#define	__Dt__DtFunc_SGXDumpDebugInfo__DtStep_2	/*for*/	_TP_BusOut( __DtBaseAddress, 0x005C );
#define	__Dt__DtFunc_SGXDumpDebugInfo__DtStep_3	/*for*/	_TP_BusOut( __DtBaseAddress, 0x005D );
#define	__Dt__DtFunc_SGXDumpDebugInfo__DtStep_4	/*for*/	_TP_BusOut( __DtBaseAddress, 0x005E );
#define	__Dt__DtFunc_SGXDumpDebugInfo__DtStep_5	/*for*/	_TP_BusOut( __DtBaseAddress, 0x005F );
#define	__Dt__DtFunc_SGXDumpDebugInfo__DtStep_6	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0060 );
#define	__Dt__DtFunc_SGXDumpDebugInfo__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0061 );
#define	__Dt__DtFunc_HWRecoveryResetSGX__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0062 );
#define	__Dt__DtFunc_HWRecoveryResetSGX__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0063 );
#define	__Dt__DtFunc_HWRecoveryResetSGX__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0064 );
#define	__Dt__DtFunc_HWRecoveryResetSGX__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0065 );
#define	__Dt__DtFunc_HWRecoveryResetSGX__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0066 );
#define	__Dt__DtFunc_SGXOSTimer__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0067 );
#define	__Dt__DtFunc_SGXOSTimer__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0068 );
#define	__Dt__DtFunc_SGXOSTimer__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0069 );
#define	__Dt__DtFunc_SGXOSTimer__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006A );
#define	__Dt__DtFunc_SGXOSTimer__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006B );
#define	__Dt__DtFunc_SGXOSTimer__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006C );
#define	__Dt__DtFunc_SGXOSTimer__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006D );
#define	__Dt__DtFunc_SGX_ISRHandler__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x006E );
#define	__Dt__DtFunc_SGX_ISRHandler__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006F );
#define	__Dt__DtFunc_SGX_ISRHandler__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0070 );
#define	__Dt__DtFunc_SGX_ISRHandler__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0071 );
#define	__Dt__DtFunc_SGX_ISRHandler__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0072 );
#define	__Dt__DtFunc_SGX_ISRHandler__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0073 );
#define	__Dt__DtFunc_SGX_ISRHandler__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0074 );
#define	__Dt__DtFunc_SGX_ISRHandler__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0075 );
#define	__Dt__DtFunc_SGX_MISRHandler__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0076 );
#define	__Dt__DtFunc_SGX_MISRHandler__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0077 );
#define	__Dt__DtFunc_SGX_MISRHandler__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0078 );
#define	__Dt__DtFunc_SGX_MISRHandler__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0079 );
#define	__Dt__DtFunc_SGX_AllocMemTilingRange__DtStep_0	/*for*/	_TP_BusOut( __DtBaseAddress, 0x007A );
#define	__Dt__DtFunc_SGX_AllocMemTilingRange__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007B );
#define	__Dt__DtFunc_SGX_AllocMemTilingRange__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007C );
#define	__Dt__DtFunc_SGX_AllocMemTilingRange__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007D );
#define	__Dt__DtFunc_SGX_AllocMemTilingRange__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007E );
#define	__Dt__DtFunc_SGX_FreeMemTilingRange__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007F );
#define	__Dt__DtFunc_SGX_FreeMemTilingRange__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0081 );
#define	__Dt__DtFunc_SGX_FreeMemTilingRange__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0082 );
#define	__Dt__DtFunc_SGX_FreeMemTilingRange__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0083 );
#define	__Dt__DtFunc_SGXRegisterDevice__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0084 );
#define	__Dt__DtFunc_SGXRegisterDevice__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0085 );
#define	__Dt__DtFunc_SGXRegisterDevice__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0086 );
#define	__Dt__DtFunc_SGXRegisterDevice__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0087 );
#define	__Dt__DtFunc_SGXResetPDump__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0088 );
#define	__Dt__DtFunc_SGXResetPDump__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0089 );
#define	__Dt__DtFunc_SGXGetClientInfoKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x008A );
#define	__Dt__DtFunc_SGXGetClientInfoKM__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x008B );
#define	__Dt__DtFunc_SGXPanic__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x008C );
#define	__Dt__DtFunc_SGXPanic__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x008D );
#define	__Dt__DtFunc_SGXDevInitCompatCheck__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008E );
#define	__Dt__DtFunc_SGXDevInitCompatCheck__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008F );
#define	__Dt__DtFunc_SGXDevInitCompatCheck__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0091 );
#define	__Dt__DtFunc_SGXDevInitCompatCheck__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0092 );
#define	__Dt__DtFunc_SGXDevInitCompatCheck__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0093 );
#define	__Dt__DtFunc_SGXDevInitCompatCheck__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0094 );
#define	__Dt__DtFunc_SGXDevInitCompatCheck__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0095 );
#define	__Dt__DtFunc_SGXDevInitCompatCheck__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0096 );
#define	__Dt__DtFunc_SGXDevInitCompatCheck__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0097 );
#define	__Dt__DtFunc_SGXDevInitCompatCheck__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0098 );
#define	__Dt__DtFunc_SGXDevInitCompatCheck__DtStep_10	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0099 );
#define	__Dt__DtFunc_SGXDevInitCompatCheck__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009A );
#define	__Dt__DtFunc_SGXDevInitCompatCheck__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009B );
#define	__Dt__DtFunc_SGXDevInitCompatCheck__DtStep_13	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009C );
#define	__Dt__DtFunc_SGXDevInitCompatCheck__DtStep_14	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009D );
#define	__Dt__DtFunc_SGXDevInitCompatCheck__DtStep_15	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009E );
#define	__Dt__DtFunc_SGXDevInitCompatCheck__DtStep_16	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009F );
#define	__Dt__DtFunc_SGXDevInitCompatCheck__DtStep_17	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A1 );
#define	__Dt__DtFunc_SGXDevInitCompatCheck__DtStep_18	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A2 );
#define	__Dt__DtFunc_SGXDevInitCompatCheck__DtStep_19	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A3 );
#define	__Dt__DtFunc_SGXDevInitCompatCheck__DtStep_20	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A4 );
#define	__Dt__DtFunc_SGXGetMiscInfoUkernel__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00A5 );
#define	__Dt__DtFunc_SGXGetMiscInfoUkernel__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A6 );
#define	__Dt__DtFunc_SGXGetMiscInfoUkernel__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A7 );
#define	__Dt__DtFunc_SGXGetMiscInfoUkernel__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A8 );
#define	__Dt__DtFunc_SGXGetMiscInfoUkernel__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A9 );
#define	__Dt__DtFunc_SGXGetMiscInfoUkernel__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00AA );
#define	__Dt__DtFunc_SGXGetMiscInfoUkernel__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00AB );
#define	__Dt__DtFunc_SGXGetMiscInfoUkernel__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00AC );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00AD );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_1	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00AE );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00AF );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B1 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B2 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B3 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B4 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B5 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B6 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_9	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00B7 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_10	/*for*/	_TP_BusOut( __DtBaseAddress, 0x00B8 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B9 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00BA );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_13	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00BB );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_14	/*do*/	_TP_BusOut( __DtBaseAddress, 0x00BC );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_15	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00BD );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_16	/*do*/	_TP_BusOut( __DtBaseAddress, 0x00BE );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_17	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00BF );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_18	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C0 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_19	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00C1 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_20	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C2 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_21	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C3 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_22	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00C4 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_23	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C5 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_24	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C6 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_25	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C7 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_26	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00C8 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_27	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C9 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_28	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00CA );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_29	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00CB );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_30	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CC );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_31	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00CD );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_32	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00CE );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_33	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CF );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_34	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D0 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_35	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D1 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_36	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D2 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_37	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D3 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_38	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D4 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_39	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D5 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_40	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00D6 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_41	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D7 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_42	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D8 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_43	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00D9 );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_44	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DA );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_45	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00DB );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_46	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DC );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_47	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DD );
#define	__Dt__DtFunc_SGXGetMiscInfoKM__DtStep_48	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DE );
#define	__Dt__DtFunc_SGXReadHWPerfCBKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00DF );
#define	__Dt__DtFunc_SGXReadHWPerfCBKM__DtStep_1	/*for*/	_TP_BusOut( __DtBaseAddress, 0x00E0 );
#define	__Dt__DtFunc_SGXReadHWPerfCBKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E1 );

#endif	/* __DT_sgxinit_h__ */

