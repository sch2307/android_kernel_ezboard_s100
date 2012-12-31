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

#ifndef	__DT_sgxpower_h__
#define	__DT_sgxpower_h__

#ifdef	__cplusplus
	#define	__DtEXTERN	extern "C"
#else
	#define	__DtEXTERN	extern
#endif

/* TestPoint MacroCode -----------------------------------------------------*/
#define		__DtBaseAddress		0x2E0
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
#define		__DtFunc_SGXAddTimer		0
#define		__DtFunc_SGXUpdateTimingInfo		1
#define		__DtFunc_SGXStartTimer		2
#define		__DtFunc_SGXPollForClockGating		3
#define		__DtFunc_SGXPrePowerState		4
#define		__DtFunc_SGXPostPowerState		5
#define		__DtFunc_SGXPreClockSpeedChange		6
#define		__DtFunc_SGXPostClockSpeedChange		7

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
#define	__Dt__DtFunc_SGXAddTimer__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0000 );
#define	__Dt__DtFunc_SGXAddTimer__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0001 );
#define	__Dt__DtFunc_SGXAddTimer__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0002 );
#define	__Dt__DtFunc_SGXAddTimer__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0003 );
#define	__Dt__DtFunc_SGXUpdateTimingInfo__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0004 );
#define	__Dt__DtFunc_SGXUpdateTimingInfo__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0005 );
#define	__Dt__DtFunc_SGXUpdateTimingInfo__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0006 );
#define	__Dt__DtFunc_SGXUpdateTimingInfo__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0007 );
#define	__Dt__DtFunc_SGXUpdateTimingInfo__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0008 );
#define	__Dt__DtFunc_SGXUpdateTimingInfo__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0009 );
#define	__Dt__DtFunc_SGXUpdateTimingInfo__DtStep_6	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000A );
#define	__Dt__DtFunc_SGXUpdateTimingInfo__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000B );
#define	__Dt__DtFunc_SGXUpdateTimingInfo__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000C );
#define	__Dt__DtFunc_SGXStartTimer__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000D );
#define	__Dt__DtFunc_SGXStartTimer__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000E );
#define	__Dt__DtFunc_SGXPollForClockGating__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x000F );
#define	__Dt__DtFunc_SGXPollForClockGating__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0010 );
#define	__Dt__DtFunc_SGXPollForClockGating__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0011 );
#define	__Dt__DtFunc_SGXPrePowerState__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0012 );
#define	__Dt__DtFunc_SGXPrePowerState__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0013 );
#define	__Dt__DtFunc_SGXPrePowerState__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0014 );
#define	__Dt__DtFunc_SGXPrePowerState__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0015 );
#define	__Dt__DtFunc_SGXPrePowerState__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0016 );
#define	__Dt__DtFunc_SGXPrePowerState__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0017 );
#define	__Dt__DtFunc_SGXPrePowerState__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0018 );
#define	__Dt__DtFunc_SGXPrePowerState__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0019 );
#define	__Dt__DtFunc_SGXPrePowerState__DtStep_8	/*for*/	_TP_BusOut( __DtBaseAddress, 0x001A );
#define	__Dt__DtFunc_SGXPrePowerState__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001B );
#define	__Dt__DtFunc_SGXPrePowerState__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001C );
#define	__Dt__DtFunc_SGXPrePowerState__DtStep_11	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001D );
#define	__Dt__DtFunc_SGXPrePowerState__DtStep_12	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001E );
#define	__Dt__DtFunc_SGXPostPowerState__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x001F );
#define	__Dt__DtFunc_SGXPostPowerState__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0020 );
#define	__Dt__DtFunc_SGXPostPowerState__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0021 );
#define	__Dt__DtFunc_SGXPostPowerState__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0022 );
#define	__Dt__DtFunc_SGXPostPowerState__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0023 );
#define	__Dt__DtFunc_SGXPostPowerState__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0024 );
#define	__Dt__DtFunc_SGXPostPowerState__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0025 );
#define	__Dt__DtFunc_SGXPostPowerState__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0026 );
#define	__Dt__DtFunc_SGXPostPowerState__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0027 );
#define	__Dt__DtFunc_SGXPostPowerState__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0028 );
#define	__Dt__DtFunc_SGXPostPowerState__DtStep_10	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002A );
#define	__Dt__DtFunc_SGXPreClockSpeedChange__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x002B );
#define	__Dt__DtFunc_SGXPreClockSpeedChange__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002C );
#define	__Dt__DtFunc_SGXPreClockSpeedChange__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002D );
#define	__Dt__DtFunc_SGXPreClockSpeedChange__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002E );
#define	__Dt__DtFunc_SGXPreClockSpeedChange__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002F );
#define	__Dt__DtFunc_SGXPreClockSpeedChange__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0031 );
#define	__Dt__DtFunc_SGXPostClockSpeedChange__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0032 );
#define	__Dt__DtFunc_SGXPostClockSpeedChange__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0033 );
#define	__Dt__DtFunc_SGXPostClockSpeedChange__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0034 );
#define	__Dt__DtFunc_SGXPostClockSpeedChange__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0035 );
#define	__Dt__DtFunc_SGXPostClockSpeedChange__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0036 );
#define	__Dt__DtFunc_SGXPostClockSpeedChange__DtStep_5	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0037 );
#define	__Dt__DtFunc_SGXPostClockSpeedChange__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0038 );
#define	__Dt__DtFunc_SGXPostClockSpeedChange__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0039 );

#endif	/* __DT_sgxpower_h__ */

