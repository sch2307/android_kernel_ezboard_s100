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

#ifndef	__DT_falinux_debug_h__
#define	__DT_falinux_debug_h__

#ifdef	__cplusplus
	#define	__DtEXTERN	extern "C"
#else
	#define	__DtEXTERN	extern
#endif

/* TestPoint MacroCode -----------------------------------------------------*/
#define		__DtBaseAddress		0x80
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
#define		__DtFunc___disp_msg_off		0
#define		__DtFunc___disp_msg_on		1
#define		__DtFunc___disp_msg_rst		2
#define		__DtFunc___enter_tab		3
#define		__DtFunc___exit_tab		4
#define		__DtFunc___disp_tab		5
#define		__DtFunc_mvbd_open		6
#define		__DtFunc_mvbd_release		7
#define		__DtFunc_mvbd_getgeo		8

/* TestPoint StepList ------------------------------------------------------*/
#define		__DtStep_0		0
#define		__DtStep_1		1
#define		__DtStep_2		2
#define		__DtStep_3		3

/* TestPoint DisableList ---------------------------------------------------*/
#define	__Dt__DtFunc___disp_msg_off__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0000 );
#define	__Dt__DtFunc___disp_msg_off__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0001 );
#define	__Dt__DtFunc___disp_msg_on__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0002 );
#define	__Dt__DtFunc___disp_msg_on__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0003 );
#define	__Dt__DtFunc___disp_msg_rst__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0004 );
#define	__Dt__DtFunc___disp_msg_rst__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0005 );
#define	__Dt__DtFunc___enter_tab__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0006 );
#define	__Dt__DtFunc___enter_tab__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0007 );
#define	__Dt__DtFunc___exit_tab__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0008 );
#define	__Dt__DtFunc___exit_tab__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0009 );
#define	__Dt__DtFunc___exit_tab__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000A );
#define	__Dt__DtFunc___exit_tab__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000B );
#define	__Dt__DtFunc___disp_tab__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x000C );
#define	__Dt__DtFunc___disp_tab__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000D );
#define	__Dt__DtFunc___disp_tab__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000E );
#define	__Dt__DtFunc_mvbd_open__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x000F );
#define	__Dt__DtFunc_mvbd_open__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0010 );
#define	__Dt__DtFunc_mvbd_release__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0011 );
#define	__Dt__DtFunc_mvbd_release__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0012 );
#define	__Dt__DtFunc_mvbd_getgeo__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0013 );
#define	__Dt__DtFunc_mvbd_getgeo__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0014 );

#endif	/* __DT_falinux_debug_h__ */

