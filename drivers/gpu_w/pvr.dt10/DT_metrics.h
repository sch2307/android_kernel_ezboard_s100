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

#ifndef	__DT_metrics_h__
#define	__DT_metrics_h__

#ifdef	__cplusplus
	#define	__DtEXTERN	extern "C"
#else
	#define	__DtEXTERN	extern
#endif

/* TestPoint MacroCode -----------------------------------------------------*/
#define		__DtBaseAddress		0xF0
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
#define		__DtFunc_PVRSRVTimeNow		0
#define		__DtFunc_PVRSRVGetCPUFreq		1
#define		__DtFunc_PVRSRVSetupMetricTimers		2
#define		__DtFunc_PVRSRVOutputMetricTotals		3

/* TestPoint StepList ------------------------------------------------------*/
#define		__DtStep_0		0
#define		__DtStep_1		1
#define		__DtStep_2		2
#define		__DtStep_3		3
#define		__DtStep_4		4
#define		__DtStep_5		5

/* TestPoint DisableList ---------------------------------------------------*/
#define	__Dt__DtFunc_PVRSRVTimeNow__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0000 );
#define	__Dt__DtFunc_PVRSRVTimeNow__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0001 );
#define	__Dt__DtFunc_PVRSRVTimeNow__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0002 );
#define	__Dt__DtFunc_PVRSRVTimeNow__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0003 );
#define	__Dt__DtFunc_PVRSRVGetCPUFreq__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0004 );
#define	__Dt__DtFunc_PVRSRVGetCPUFreq__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0005 );
#define	__Dt__DtFunc_PVRSRVSetupMetricTimers__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0006 );
#define	__Dt__DtFunc_PVRSRVSetupMetricTimers__DtStep_1	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0007 );
#define	__Dt__DtFunc_PVRSRVSetupMetricTimers__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0008 );
#define	__Dt__DtFunc_PVRSRVOutputMetricTotals__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0009 );
#define	__Dt__DtFunc_PVRSRVOutputMetricTotals__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000A );
#define	__Dt__DtFunc_PVRSRVOutputMetricTotals__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000B );
#define	__Dt__DtFunc_PVRSRVOutputMetricTotals__DtStep_3	/*for*/	_TP_BusOut( __DtBaseAddress, 0x000C );
#define	__Dt__DtFunc_PVRSRVOutputMetricTotals__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000D );
#define	__Dt__DtFunc_PVRSRVOutputMetricTotals__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000E );

#endif	/* __DT_metrics_h__ */

