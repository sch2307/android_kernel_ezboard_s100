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

#ifndef	__DT_resman_h__
#define	__DT_resman_h__

#ifdef	__cplusplus
	#define	__DtEXTERN	extern "C"
#else
	#define	__DtEXTERN	extern
#endif

/* TestPoint MacroCode -----------------------------------------------------*/
#define		__DtBaseAddress		0x240
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
#define		__DtFunc_ResManInit		0
#define		__DtFunc_ResManDeInit		1
#define		__DtFunc_PVRSRVResManConnect		2
#define		__DtFunc_PVRSRVResManDisconnect		3
#define		__DtFunc_ResManRegisterRes		4
#define		__DtFunc_ResManFreeResByPtr		5
#define		__DtFunc_ResManFreeResByCriteria		6
#define		__DtFunc_ResManDissociateRes		7
#define		__DtFunc_ResManFindResourceByPtr_AnyVaCb		8
#define		__DtFunc_ResManFindResourceByPtr		9
#define		__DtFunc_FreeResourceByPtr		10
#define		__DtFunc_FreeResourceByCriteria_AnyVaCb		11
#define		__DtFunc_FreeResourceByCriteria		12
#define		__DtFunc_ValidateResList		13

/* TestPoint StepList ------------------------------------------------------*/
#define		__DtStep_0		0
#define		__DtStep_1		1
#define		__DtStep_2		2
#define		__DtStep_3		3
#define		__DtStep_4		4
#define		__DtStep_5		5
#define		__DtStep_6		6
#define		__DtStep_7		7

/* TestPoint DisableList ---------------------------------------------------*/
#define	__Dt__DtFunc_ResManInit__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0000 );
#define	__Dt__DtFunc_ResManInit__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0001 );
#define	__Dt__DtFunc_ResManInit__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0002 );
#define	__Dt__DtFunc_ResManInit__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0003 );
#define	__Dt__DtFunc_ResManDeInit__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0004 );
#define	__Dt__DtFunc_ResManDeInit__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0005 );
#define	__Dt__DtFunc_ResManDeInit__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0006 );
#define	__Dt__DtFunc_PVRSRVResManConnect__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0007 );
#define	__Dt__DtFunc_PVRSRVResManConnect__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0008 );
#define	__Dt__DtFunc_PVRSRVResManConnect__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0009 );
#define	__Dt__DtFunc_PVRSRVResManConnect__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000A );
#define	__Dt__DtFunc_PVRSRVResManDisconnect__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x000B );
#define	__Dt__DtFunc_PVRSRVResManDisconnect__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000C );
#define	__Dt__DtFunc_PVRSRVResManDisconnect__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000D );
#define	__Dt__DtFunc_ResManRegisterRes__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x000E );
#define	__Dt__DtFunc_ResManRegisterRes__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000F );
#define	__Dt__DtFunc_ResManRegisterRes__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0010 );
#define	__Dt__DtFunc_ResManRegisterRes__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0011 );
#define	__Dt__DtFunc_ResManRegisterRes__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0012 );
#define	__Dt__DtFunc_ResManRegisterRes__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0013 );
#define	__Dt__DtFunc_ResManFreeResByPtr__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0014 );
#define	__Dt__DtFunc_ResManFreeResByPtr__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0015 );
#define	__Dt__DtFunc_ResManFreeResByPtr__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0016 );
#define	__Dt__DtFunc_ResManFreeResByPtr__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0017 );
#define	__Dt__DtFunc_ResManFreeResByCriteria__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0018 );
#define	__Dt__DtFunc_ResManFreeResByCriteria__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0019 );
#define	__Dt__DtFunc_ResManDissociateRes__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x001A );
#define	__Dt__DtFunc_ResManDissociateRes__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001B );
#define	__Dt__DtFunc_ResManDissociateRes__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001C );
#define	__Dt__DtFunc_ResManDissociateRes__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001D );
#define	__Dt__DtFunc_ResManDissociateRes__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001E );
#define	__Dt__DtFunc_ResManDissociateRes__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001F );
#define	__Dt__DtFunc_ResManDissociateRes__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0020 );
#define	__Dt__DtFunc_ResManDissociateRes__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0021 );
#define	__Dt__DtFunc_ResManFindResourceByPtr_AnyVaCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0022 );
#define	__Dt__DtFunc_ResManFindResourceByPtr_AnyVaCb__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0023 );
#define	__Dt__DtFunc_ResManFindResourceByPtr__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0024 );
#define	__Dt__DtFunc_ResManFindResourceByPtr__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0025 );
#define	__Dt__DtFunc_ResManFindResourceByPtr__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0026 );
#define	__Dt__DtFunc_ResManFindResourceByPtr__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0027 );
#define	__Dt__DtFunc_ResManFindResourceByPtr__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0028 );
#define	__Dt__DtFunc_ResManFindResourceByPtr__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002A );
#define	__Dt__DtFunc_FreeResourceByPtr__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x002B );
#define	__Dt__DtFunc_FreeResourceByPtr__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002C );
#define	__Dt__DtFunc_FreeResourceByPtr__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002D );
#define	__Dt__DtFunc_FreeResourceByPtr__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002E );
#define	__Dt__DtFunc_FreeResourceByPtr__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002F );
#define	__Dt__DtFunc_FreeResourceByPtr__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0031 );
#define	__Dt__DtFunc_FreeResourceByPtr__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0032 );
#define	__Dt__DtFunc_FreeResourceByCriteria_AnyVaCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0033 );
#define	__Dt__DtFunc_FreeResourceByCriteria_AnyVaCb__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0034 );
#define	__Dt__DtFunc_FreeResourceByCriteria_AnyVaCb__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0035 );
#define	__Dt__DtFunc_FreeResourceByCriteria_AnyVaCb__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0036 );
#define	__Dt__DtFunc_FreeResourceByCriteria__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0037 );
#define	__Dt__DtFunc_FreeResourceByCriteria__DtStep_1	/*while*/	_TP_BusOut( __DtBaseAddress, 0x0038 );
#define	__Dt__DtFunc_FreeResourceByCriteria__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0039 );
#define	__Dt__DtFunc_ValidateResList__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x003A );
#define	__Dt__DtFunc_ValidateResList__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003B );
#define	__Dt__DtFunc_ValidateResList__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003C );
#define	__Dt__DtFunc_ValidateResList__DtStep_3	/*while*/	_TP_BusOut( __DtBaseAddress, 0x003D );
#define	__Dt__DtFunc_ValidateResList__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003E );
#define	__Dt__DtFunc_ValidateResList__DtStep_5	/*while*/	_TP_BusOut( __DtBaseAddress, 0x003F );
#define	__Dt__DtFunc_ValidateResList__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0040 );
#define	__Dt__DtFunc_ValidateResList__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0041 );

#endif	/* __DT_resman_h__ */

