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

#ifndef	__DT_buffer_manager_h__
#define	__DT_buffer_manager_h__

#ifdef	__cplusplus
	#define	__DtEXTERN	extern "C"
#else
	#define	__DtEXTERN	extern
#endif

/* TestPoint MacroCode -----------------------------------------------------*/
#define		__DtBaseAddress		0x30
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
#define		__DtFunc_AllocMemory		0
#define		__DtFunc_WrapMemory		1
#define		__DtFunc_ZeroBuf		2
#define		__DtFunc_FreeBuf		3
#define		__DtFunc_BM_DestroyContext_AnyCb		4
#define		__DtFunc_BM_DestroyContext		5
#define		__DtFunc_BM_DestroyContextCallBack_AnyVaCb		6
#define		__DtFunc_BM_DestroyContextCallBack		7
#define		__DtFunc_BM_CreateContext_IncRefCount_AnyVaCb		8
#define		__DtFunc_BM_CreateContext_InsertHeap_ForEachVaCb		9
#define		__DtFunc_BM_CreateContext		10
#define		__DtFunc_BM_CreateHeap_AnyVaCb		11
#define		__DtFunc_BM_CreateHeap		12
#define		__DtFunc_BM_DestroyHeap		13
#define		__DtFunc_BM_Reinitialise		14
#define		__DtFunc_BM_Alloc		15
#define		__DtFunc_ValidSysPAddrArrayForDev		16
#define		__DtFunc_ValidSysPAddrRangeForDev		17
#define		__DtFunc_BM_Wrap		18
#define		__DtFunc_BM_Export		19
#define		__DtFunc_BM_FreeExport		20
#define		__DtFunc_BM_Free		21
#define		__DtFunc_BM_HandleToCpuVaddr		22
#define		__DtFunc_BM_HandleToDevVaddr		23
#define		__DtFunc_BM_HandleToSysPaddr		24
#define		__DtFunc_BM_HandleToOSMemHandle		25
#define		__DtFunc_DevMemoryAlloc		26
#define		__DtFunc_DevMemoryFree		27
#define		__DtFunc_BM_ImportMemory		28
#define		__DtFunc_BM_FreeMemory		29
#define		__DtFunc_BM_GetPhysPageAddr		30
#define		__DtFunc_BM_GetMMUContext		31
#define		__DtFunc_BM_GetMMUContextFromMemContext		32
#define		__DtFunc_BM_GetMMUHeap		33
#define		__DtFunc_BM_GetDeviceNode		34
#define		__DtFunc_BM_GetMappingHandle		35

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
#define	__Dt__DtFunc_AllocMemory__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0000 );
#define	__Dt__DtFunc_AllocMemory__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0001 );
#define	__Dt__DtFunc_AllocMemory__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0002 );
#define	__Dt__DtFunc_AllocMemory__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0003 );
#define	__Dt__DtFunc_AllocMemory__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0004 );
#define	__Dt__DtFunc_AllocMemory__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0005 );
#define	__Dt__DtFunc_AllocMemory__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0006 );
#define	__Dt__DtFunc_AllocMemory__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0007 );
#define	__Dt__DtFunc_AllocMemory__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0008 );
#define	__Dt__DtFunc_AllocMemory__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0009 );
#define	__Dt__DtFunc_AllocMemory__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000A );
#define	__Dt__DtFunc_AllocMemory__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000B );
#define	__Dt__DtFunc_AllocMemory__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000C );
#define	__Dt__DtFunc_AllocMemory__DtStep_13	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000D );
#define	__Dt__DtFunc_AllocMemory__DtStep_14	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000E );
#define	__Dt__DtFunc_AllocMemory__DtStep_15	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000F );
#define	__Dt__DtFunc_AllocMemory__DtStep_16	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0010 );
#define	__Dt__DtFunc_AllocMemory__DtStep_17	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0011 );
#define	__Dt__DtFunc_AllocMemory__DtStep_18	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0012 );
#define	__Dt__DtFunc_AllocMemory__DtStep_19	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0013 );
#define	__Dt__DtFunc_AllocMemory__DtStep_20	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0014 );
#define	__Dt__DtFunc_AllocMemory__DtStep_21	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0015 );
#define	__Dt__DtFunc_AllocMemory__DtStep_22	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0016 );
#define	__Dt__DtFunc_AllocMemory__DtStep_23	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0017 );
#define	__Dt__DtFunc_AllocMemory__DtStep_24	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0018 );
#define	__Dt__DtFunc_WrapMemory__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0019 );
#define	__Dt__DtFunc_WrapMemory__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001A );
#define	__Dt__DtFunc_WrapMemory__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001B );
#define	__Dt__DtFunc_WrapMemory__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001C );
#define	__Dt__DtFunc_WrapMemory__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001D );
#define	__Dt__DtFunc_WrapMemory__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001E );
#define	__Dt__DtFunc_WrapMemory__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001F );
#define	__Dt__DtFunc_WrapMemory__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0020 );
#define	__Dt__DtFunc_WrapMemory__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0021 );
#define	__Dt__DtFunc_WrapMemory__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0022 );
#define	__Dt__DtFunc_WrapMemory__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0023 );
#define	__Dt__DtFunc_WrapMemory__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0024 );
#define	__Dt__DtFunc_WrapMemory__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0025 );
#define	__Dt__DtFunc_WrapMemory__DtStep_13	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0026 );
#define	__Dt__DtFunc_WrapMemory__DtStep_14	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0027 );
#define	__Dt__DtFunc_WrapMemory__DtStep_15	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0028 );
#define	__Dt__DtFunc_WrapMemory__DtStep_16	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002A );
#define	__Dt__DtFunc_WrapMemory__DtStep_17	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002B );
#define	__Dt__DtFunc_WrapMemory__DtStep_18	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002C );
#define	__Dt__DtFunc_WrapMemory__DtStep_19	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002D );
#define	__Dt__DtFunc_WrapMemory__DtStep_20	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002E );
#define	__Dt__DtFunc_WrapMemory__DtStep_21	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002F );
#define	__Dt__DtFunc_WrapMemory__DtStep_22	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0031 );
#define	__Dt__DtFunc_WrapMemory__DtStep_23	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0032 );
#define	__Dt__DtFunc_WrapMemory__DtStep_24	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0033 );
#define	__Dt__DtFunc_WrapMemory__DtStep_25	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0034 );
#define	__Dt__DtFunc_WrapMemory__DtStep_26	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0035 );
#define	__Dt__DtFunc_WrapMemory__DtStep_27	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0036 );
#define	__Dt__DtFunc_WrapMemory__DtStep_28	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0037 );
#define	__Dt__DtFunc_ZeroBuf__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0038 );
#define	__Dt__DtFunc_ZeroBuf__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0039 );
#define	__Dt__DtFunc_ZeroBuf__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003A );
#define	__Dt__DtFunc_ZeroBuf__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003B );
#define	__Dt__DtFunc_ZeroBuf__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003C );
#define	__Dt__DtFunc_ZeroBuf__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003D );
#define	__Dt__DtFunc_ZeroBuf__DtStep_6	/*while*/	_TP_BusOut( __DtBaseAddress, 0x003E );
#define	__Dt__DtFunc_ZeroBuf__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003F );
#define	__Dt__DtFunc_ZeroBuf__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0040 );
#define	__Dt__DtFunc_ZeroBuf__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0041 );
#define	__Dt__DtFunc_ZeroBuf__DtStep_10	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0042 );
#define	__Dt__DtFunc_FreeBuf__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0043 );
#define	__Dt__DtFunc_FreeBuf__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0044 );
#define	__Dt__DtFunc_FreeBuf__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0045 );
#define	__Dt__DtFunc_FreeBuf__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0046 );
#define	__Dt__DtFunc_FreeBuf__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0047 );
#define	__Dt__DtFunc_FreeBuf__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0048 );
#define	__Dt__DtFunc_FreeBuf__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0049 );
#define	__Dt__DtFunc_FreeBuf__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004A );
#define	__Dt__DtFunc_FreeBuf__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004B );
#define	__Dt__DtFunc_FreeBuf__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004C );
#define	__Dt__DtFunc_FreeBuf__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004D );
#define	__Dt__DtFunc_FreeBuf__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004E );
#define	__Dt__DtFunc_FreeBuf__DtStep_12	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x004F );
#define	__Dt__DtFunc_FreeBuf__DtStep_13	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0050 );
#define	__Dt__DtFunc_FreeBuf__DtStep_14	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0051 );
#define	__Dt__DtFunc_FreeBuf__DtStep_15	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0052 );
#define	__Dt__DtFunc_FreeBuf__DtStep_16	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0053 );
#define	__Dt__DtFunc_FreeBuf__DtStep_17	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0054 );
#define	__Dt__DtFunc_FreeBuf__DtStep_18	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0055 );
#define	__Dt__DtFunc_FreeBuf__DtStep_19	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0056 );
#define	__Dt__DtFunc_BM_DestroyContext_AnyCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0057 );
#define	__Dt__DtFunc_BM_DestroyContext_AnyCb__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0058 );
#define	__Dt__DtFunc_BM_DestroyContext_AnyCb__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0059 );
#define	__Dt__DtFunc_BM_DestroyContext_AnyCb__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005A );
#define	__Dt__DtFunc_BM_DestroyContext_AnyCb__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005B );
#define	__Dt__DtFunc_BM_DestroyContext_AnyCb__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005C );
#define	__Dt__DtFunc_BM_DestroyContext__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x005D );
#define	__Dt__DtFunc_BM_DestroyContext__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005E );
#define	__Dt__DtFunc_BM_DestroyContext__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005F );
#define	__Dt__DtFunc_BM_DestroyContext__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0060 );
#define	__Dt__DtFunc_BM_DestroyContext__DtStep_4	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0061 );
#define	__Dt__DtFunc_BM_DestroyContext__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0062 );
#define	__Dt__DtFunc_BM_DestroyContext__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0063 );
#define	__Dt__DtFunc_BM_DestroyContext__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0064 );
#define	__Dt__DtFunc_BM_DestroyContext__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0065 );
#define	__Dt__DtFunc_BM_DestroyContext__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0066 );
#define	__Dt__DtFunc_BM_DestroyContext__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0067 );
#define	__Dt__DtFunc_BM_DestroyContext__DtStep_11	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0068 );
#define	__Dt__DtFunc_BM_DestroyContextCallBack_AnyVaCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0069 );
#define	__Dt__DtFunc_BM_DestroyContextCallBack_AnyVaCb__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006A );
#define	__Dt__DtFunc_BM_DestroyContextCallBack_AnyVaCb__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006B );
#define	__Dt__DtFunc_BM_DestroyContextCallBack_AnyVaCb__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006C );
#define	__Dt__DtFunc_BM_DestroyContextCallBack_AnyVaCb__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006D );
#define	__Dt__DtFunc_BM_DestroyContextCallBack_AnyVaCb__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006E );
#define	__Dt__DtFunc_BM_DestroyContextCallBack__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x006F );
#define	__Dt__DtFunc_BM_DestroyContextCallBack__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0070 );
#define	__Dt__DtFunc_BM_DestroyContextCallBack__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0071 );
#define	__Dt__DtFunc_BM_DestroyContextCallBack__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0072 );
#define	__Dt__DtFunc_BM_DestroyContextCallBack__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0073 );
#define	__Dt__DtFunc_BM_DestroyContextCallBack__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0074 );
#define	__Dt__DtFunc_BM_DestroyContextCallBack__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0075 );
#define	__Dt__DtFunc_BM_CreateContext_IncRefCount_AnyVaCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0076 );
#define	__Dt__DtFunc_BM_CreateContext_IncRefCount_AnyVaCb__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0077 );
#define	__Dt__DtFunc_BM_CreateContext_IncRefCount_AnyVaCb__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0078 );
#define	__Dt__DtFunc_BM_CreateContext_IncRefCount_AnyVaCb__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0079 );
#define	__Dt__DtFunc_BM_CreateContext_InsertHeap_ForEachVaCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x007A );
#define	__Dt__DtFunc_BM_CreateContext_InsertHeap_ForEachVaCb__DtStep_1	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x007B );
#define	__Dt__DtFunc_BM_CreateContext_InsertHeap_ForEachVaCb__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007C );
#define	__Dt__DtFunc_BM_CreateContext__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x007D );
#define	__Dt__DtFunc_BM_CreateContext__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007E );
#define	__Dt__DtFunc_BM_CreateContext__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007F );
#define	__Dt__DtFunc_BM_CreateContext__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0081 );
#define	__Dt__DtFunc_BM_CreateContext__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0082 );
#define	__Dt__DtFunc_BM_CreateContext__DtStep_5	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0083 );
#define	__Dt__DtFunc_BM_CreateContext__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0084 );
#define	__Dt__DtFunc_BM_CreateContext__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0085 );
#define	__Dt__DtFunc_BM_CreateContext__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0086 );
#define	__Dt__DtFunc_BM_CreateContext__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0087 );
#define	__Dt__DtFunc_BM_CreateContext__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0088 );
#define	__Dt__DtFunc_BM_CreateContext__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0089 );
#define	__Dt__DtFunc_BM_CreateContext__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008A );
#define	__Dt__DtFunc_BM_CreateContext__DtStep_13	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008B );
#define	__Dt__DtFunc_BM_CreateContext__DtStep_14	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008C );
#define	__Dt__DtFunc_BM_CreateContext__DtStep_15	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x008D );
#define	__Dt__DtFunc_BM_CreateContext__DtStep_16	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x008E );
#define	__Dt__DtFunc_BM_CreateHeap_AnyVaCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x008F );
#define	__Dt__DtFunc_BM_CreateHeap_AnyVaCb__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0091 );
#define	__Dt__DtFunc_BM_CreateHeap_AnyVaCb__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0092 );
#define	__Dt__DtFunc_BM_CreateHeap_AnyVaCb__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0093 );
#define	__Dt__DtFunc_BM_CreateHeap__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0094 );
#define	__Dt__DtFunc_BM_CreateHeap__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0095 );
#define	__Dt__DtFunc_BM_CreateHeap__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0096 );
#define	__Dt__DtFunc_BM_CreateHeap__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0097 );
#define	__Dt__DtFunc_BM_CreateHeap__DtStep_4	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0098 );
#define	__Dt__DtFunc_BM_CreateHeap__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0099 );
#define	__Dt__DtFunc_BM_CreateHeap__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x009A );
#define	__Dt__DtFunc_BM_CreateHeap__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009B );
#define	__Dt__DtFunc_BM_CreateHeap__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009C );
#define	__Dt__DtFunc_BM_CreateHeap__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009D );
#define	__Dt__DtFunc_BM_CreateHeap__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009E );
#define	__Dt__DtFunc_BM_CreateHeap__DtStep_11	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x009F );
#define	__Dt__DtFunc_BM_CreateHeap__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A1 );
#define	__Dt__DtFunc_BM_CreateHeap__DtStep_13	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A2 );
#define	__Dt__DtFunc_BM_DestroyHeap__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00A3 );
#define	__Dt__DtFunc_BM_DestroyHeap__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A4 );
#define	__Dt__DtFunc_BM_DestroyHeap__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A5 );
#define	__Dt__DtFunc_BM_DestroyHeap__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A6 );
#define	__Dt__DtFunc_BM_DestroyHeap__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A7 );
#define	__Dt__DtFunc_BM_DestroyHeap__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A8 );
#define	__Dt__DtFunc_BM_DestroyHeap__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A9 );
#define	__Dt__DtFunc_BM_DestroyHeap__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00AA );
#define	__Dt__DtFunc_BM_Reinitialise__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00AB );
#define	__Dt__DtFunc_BM_Reinitialise__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00AC );
#define	__Dt__DtFunc_BM_Alloc__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00AD );
#define	__Dt__DtFunc_BM_Alloc__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00AE );
#define	__Dt__DtFunc_BM_Alloc__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00AF );
#define	__Dt__DtFunc_BM_Alloc__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B1 );
#define	__Dt__DtFunc_BM_Alloc__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B2 );
#define	__Dt__DtFunc_BM_Alloc__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B3 );
#define	__Dt__DtFunc_BM_Alloc__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B4 );
#define	__Dt__DtFunc_BM_Alloc__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B5 );
#define	__Dt__DtFunc_BM_Alloc__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B6 );
#define	__Dt__DtFunc_BM_Alloc__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B7 );
#define	__Dt__DtFunc_ValidSysPAddrArrayForDev__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00B8 );
#define	__Dt__DtFunc_ValidSysPAddrArrayForDev__DtStep_1	/*for*/	_TP_BusOut( __DtBaseAddress, 0x00B9 );
#define	__Dt__DtFunc_ValidSysPAddrArrayForDev__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BA );
#define	__Dt__DtFunc_ValidSysPAddrArrayForDev__DtStep_3	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BB );
#define	__Dt__DtFunc_ValidSysPAddrArrayForDev__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BC );
#define	__Dt__DtFunc_ValidSysPAddrRangeForDev__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00BD );
#define	__Dt__DtFunc_ValidSysPAddrRangeForDev__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BE );
#define	__Dt__DtFunc_ValidSysPAddrRangeForDev__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BF );
#define	__Dt__DtFunc_ValidSysPAddrRangeForDev__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C0 );
#define	__Dt__DtFunc_BM_Wrap__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00C1 );
#define	__Dt__DtFunc_BM_Wrap__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C2 );
#define	__Dt__DtFunc_BM_Wrap__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C3 );
#define	__Dt__DtFunc_BM_Wrap__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C4 );
#define	__Dt__DtFunc_BM_Wrap__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C5 );
#define	__Dt__DtFunc_BM_Wrap__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C6 );
#define	__Dt__DtFunc_BM_Wrap__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C7 );
#define	__Dt__DtFunc_BM_Wrap__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C8 );
#define	__Dt__DtFunc_BM_Wrap__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C9 );
#define	__Dt__DtFunc_BM_Wrap__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00CA );
#define	__Dt__DtFunc_BM_Wrap__DtStep_10	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CB );
#define	__Dt__DtFunc_BM_Wrap__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00CC );
#define	__Dt__DtFunc_BM_Wrap__DtStep_12	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CD );
#define	__Dt__DtFunc_BM_Wrap__DtStep_13	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00CE );
#define	__Dt__DtFunc_BM_Wrap__DtStep_14	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CF );
#define	__Dt__DtFunc_BM_Wrap__DtStep_15	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D0 );
#define	__Dt__DtFunc_BM_Wrap__DtStep_16	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D1 );
#define	__Dt__DtFunc_BM_Wrap__DtStep_17	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D2 );
#define	__Dt__DtFunc_BM_Wrap__DtStep_18	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D3 );
#define	__Dt__DtFunc_BM_Wrap__DtStep_19	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D4 );
#define	__Dt__DtFunc_BM_Export__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00D5 );
#define	__Dt__DtFunc_BM_Export__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D6 );
#define	__Dt__DtFunc_BM_FreeExport__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00D7 );
#define	__Dt__DtFunc_BM_FreeExport__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D8 );
#define	__Dt__DtFunc_BM_Free__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00D9 );
#define	__Dt__DtFunc_BM_Free__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00DA );
#define	__Dt__DtFunc_BM_Free__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DB );
#define	__Dt__DtFunc_BM_Free__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00DC );
#define	__Dt__DtFunc_BM_Free__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00DD );
#define	__Dt__DtFunc_BM_Free__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DE );
#define	__Dt__DtFunc_BM_HandleToCpuVaddr__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00DF );
#define	__Dt__DtFunc_BM_HandleToCpuVaddr__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E0 );
#define	__Dt__DtFunc_BM_HandleToCpuVaddr__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E1 );
#define	__Dt__DtFunc_BM_HandleToCpuVaddr__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E2 );
#define	__Dt__DtFunc_BM_HandleToDevVaddr__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00E3 );
#define	__Dt__DtFunc_BM_HandleToDevVaddr__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E4 );
#define	__Dt__DtFunc_BM_HandleToDevVaddr__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E5 );
#define	__Dt__DtFunc_BM_HandleToDevVaddr__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E6 );
#define	__Dt__DtFunc_BM_HandleToSysPaddr__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00E7 );
#define	__Dt__DtFunc_BM_HandleToSysPaddr__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E8 );
#define	__Dt__DtFunc_BM_HandleToSysPaddr__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E9 );
#define	__Dt__DtFunc_BM_HandleToSysPaddr__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00EA );
#define	__Dt__DtFunc_BM_HandleToOSMemHandle__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00EB );
#define	__Dt__DtFunc_BM_HandleToOSMemHandle__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00EC );
#define	__Dt__DtFunc_BM_HandleToOSMemHandle__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00ED );
#define	__Dt__DtFunc_BM_HandleToOSMemHandle__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00EE );
#define	__Dt__DtFunc_DevMemoryAlloc__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00EF );
#define	__Dt__DtFunc_DevMemoryAlloc__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F1 );
#define	__Dt__DtFunc_DevMemoryAlloc__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F2 );
#define	__Dt__DtFunc_DevMemoryAlloc__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F3 );
#define	__Dt__DtFunc_DevMemoryAlloc__DtStep_4	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00F4 );
#define	__Dt__DtFunc_DevMemoryAlloc__DtStep_5	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00F5 );
#define	__Dt__DtFunc_DevMemoryAlloc__DtStep_6	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00F6 );
#define	__Dt__DtFunc_DevMemoryAlloc__DtStep_7	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00F7 );
#define	__Dt__DtFunc_DevMemoryAlloc__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F8 );
#define	__Dt__DtFunc_DevMemoryFree__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F9 );
#define	__Dt__DtFunc_DevMemoryFree__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00FA );
#define	__Dt__DtFunc_DevMemoryFree__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00FB );
#define	__Dt__DtFunc_BM_ImportMemory__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00FC );
#define	__Dt__DtFunc_BM_ImportMemory__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00FD );
#define	__Dt__DtFunc_BM_ImportMemory__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00FE );
#define	__Dt__DtFunc_BM_ImportMemory__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00FF );
#define	__Dt__DtFunc_BM_ImportMemory__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0100 );
#define	__Dt__DtFunc_BM_ImportMemory__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0101 );
#define	__Dt__DtFunc_BM_ImportMemory__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0102 );
#define	__Dt__DtFunc_BM_ImportMemory__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0103 );
#define	__Dt__DtFunc_BM_ImportMemory__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0104 );
#define	__Dt__DtFunc_BM_ImportMemory__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0105 );
#define	__Dt__DtFunc_BM_ImportMemory__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0106 );
#define	__Dt__DtFunc_BM_ImportMemory__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0107 );
#define	__Dt__DtFunc_BM_ImportMemory__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0108 );
#define	__Dt__DtFunc_BM_ImportMemory__DtStep_13	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0109 );
#define	__Dt__DtFunc_BM_ImportMemory__DtStep_14	/*if*/	_TP_BusOut( __DtBaseAddress, 0x010A );
#define	__Dt__DtFunc_BM_ImportMemory__DtStep_15	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x010B );
#define	__Dt__DtFunc_BM_ImportMemory__DtStep_16	/*if*/	_TP_BusOut( __DtBaseAddress, 0x010C );
#define	__Dt__DtFunc_BM_ImportMemory__DtStep_17	/*if*/	_TP_BusOut( __DtBaseAddress, 0x010D );
#define	__Dt__DtFunc_BM_ImportMemory__DtStep_18	/*if*/	_TP_BusOut( __DtBaseAddress, 0x010E );
#define	__Dt__DtFunc_BM_ImportMemory__DtStep_19	/*if*/	_TP_BusOut( __DtBaseAddress, 0x010F );
#define	__Dt__DtFunc_BM_ImportMemory__DtStep_20	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0110 );
#define	__Dt__DtFunc_BM_ImportMemory__DtStep_21	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0111 );
#define	__Dt__DtFunc_BM_ImportMemory__DtStep_22	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0112 );
#define	__Dt__DtFunc_BM_ImportMemory__DtStep_23	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0113 );
#define	__Dt__DtFunc_BM_FreeMemory__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0114 );
#define	__Dt__DtFunc_BM_FreeMemory__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0115 );
#define	__Dt__DtFunc_BM_FreeMemory__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0116 );
#define	__Dt__DtFunc_BM_FreeMemory__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0117 );
#define	__Dt__DtFunc_BM_FreeMemory__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0118 );
#define	__Dt__DtFunc_BM_FreeMemory__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0119 );
#define	__Dt__DtFunc_BM_FreeMemory__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x011A );
#define	__Dt__DtFunc_BM_FreeMemory__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x011B );
#define	__Dt__DtFunc_BM_FreeMemory__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x011C );
#define	__Dt__DtFunc_BM_FreeMemory__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x011D );
#define	__Dt__DtFunc_BM_GetPhysPageAddr__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x011E );
#define	__Dt__DtFunc_BM_GetPhysPageAddr__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x011F );
#define	__Dt__DtFunc_BM_GetMMUContext__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0120 );
#define	__Dt__DtFunc_BM_GetMMUContext__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0121 );
#define	__Dt__DtFunc_BM_GetMMUContextFromMemContext__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0122 );
#define	__Dt__DtFunc_BM_GetMMUContextFromMemContext__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0123 );
#define	__Dt__DtFunc_BM_GetMMUHeap__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0124 );
#define	__Dt__DtFunc_BM_GetMMUHeap__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0125 );
#define	__Dt__DtFunc_BM_GetDeviceNode__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0126 );
#define	__Dt__DtFunc_BM_GetDeviceNode__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0127 );
#define	__Dt__DtFunc_BM_GetMappingHandle__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0128 );
#define	__Dt__DtFunc_BM_GetMappingHandle__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x012A );

#endif	/* __DT_buffer_manager_h__ */

