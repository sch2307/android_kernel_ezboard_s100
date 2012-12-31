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

#ifndef	__DT_handle_h__
#define	__DT_handle_h__

#ifdef	__cplusplus
	#define	__DtEXTERN	extern "C"
#else
	#define	__DtEXTERN	extern
#endif

/* TestPoint MacroCode -----------------------------------------------------*/
#define		__DtBaseAddress		0x90
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
#define		__DtFunc_HandleListInit		0
#define		__DtFunc_InitParentList		1
#define		__DtFunc_InitChildEntry		2
#define		__DtFunc_HandleListIsEmpty		3
#define		__DtFunc_NoChildren		4
#define		__DtFunc_NoParent		5
#define		__DtFunc_ParentHandle		6
#define		__DtFunc_HandleListInsertBefore		7
#define		__DtFunc_AdoptChild		8
#define		__DtFunc_HandleListRemove		9
#define		__DtFunc_UnlinkFromParent		10
#define		__DtFunc_HandleListIterate		11
#define		__DtFunc_IterateOverChildren		12
#define		__DtFunc_GetHandleStructure		13
#define		__DtFunc_ParentIfPrivate		14
#define		__DtFunc_InitKey		15
#define		__DtFunc_ReallocHandleArray		16
#define		__DtFunc_FreeHandleArray		17
#define		__DtFunc_FreeHandle		18
#define		__DtFunc_FreeAllHandles		19
#define		__DtFunc_FreeHandleBase		20
#define		__DtFunc_FindHandle		21
#define		__DtFunc_IncreaseHandleArraySize		22
#define		__DtFunc_EnsureFreeHandles		23
#define		__DtFunc_AllocHandle		24
#define		__DtFunc_PVRSRVAllocHandle		25
#define		__DtFunc_PVRSRVAllocSubHandle		26
#define		__DtFunc_PVRSRVFindHandle		27
#define		__DtFunc_PVRSRVLookupHandleAnyType		28
#define		__DtFunc_PVRSRVLookupHandle		29
#define		__DtFunc_PVRSRVLookupSubHandle		30
#define		__DtFunc_PVRSRVGetParentHandle		31
#define		__DtFunc_PVRSRVLookupAndReleaseHandle		32
#define		__DtFunc_PVRSRVReleaseHandle		33
#define		__DtFunc_PVRSRVNewHandleBatch		34
#define		__DtFunc_PVRSRVHandleBatchCommitOrRelease		35
#define		__DtFunc_PVRSRVCommitHandleBatch		36
#define		__DtFunc_PVRSRVReleaseHandleBatch		37
#define		__DtFunc_PVRSRVSetMaxHandle		38
#define		__DtFunc_PVRSRVGetMaxHandle		39
#define		__DtFunc_PVRSRVEnableHandlePurging		40
#define		__DtFunc_PVRSRVPurgeHandles		41
#define		__DtFunc_PVRSRVAllocHandleBase		42
#define		__DtFunc_PVRSRVFreeHandleBase		43
#define		__DtFunc_PVRSRVHandleInit		44
#define		__DtFunc_PVRSRVHandleDeInit		45

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

/* TestPoint DisableList ---------------------------------------------------*/
#define	__Dt__DtFunc_HandleListInit__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0000 );
#define	__Dt__DtFunc_HandleListInit__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0001 );
#define	__Dt__DtFunc_InitParentList__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0002 );
#define	__Dt__DtFunc_InitParentList__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0003 );
#define	__Dt__DtFunc_InitChildEntry__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0004 );
#define	__Dt__DtFunc_InitChildEntry__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0005 );
#define	__Dt__DtFunc_HandleListIsEmpty__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0006 );
#define	__Dt__DtFunc_NoChildren__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0007 );
#define	__Dt__DtFunc_NoChildren__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0008 );
#define	__Dt__DtFunc_NoParent__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0009 );
#define	__Dt__DtFunc_NoParent__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000A );
#define	__Dt__DtFunc_NoParent__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000B );
#define	__Dt__DtFunc_NoParent__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000C );
#define	__Dt__DtFunc_NoParent__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000D );
#define	__Dt__DtFunc_ParentHandle__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000E );
#define	__Dt__DtFunc_HandleListInsertBefore__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x000F );
#define	__Dt__DtFunc_HandleListInsertBefore__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0010 );
#define	__Dt__DtFunc_AdoptChild__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0011 );
#define	__Dt__DtFunc_AdoptChild__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0012 );
#define	__Dt__DtFunc_HandleListRemove__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0013 );
#define	__Dt__DtFunc_HandleListRemove__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0014 );
#define	__Dt__DtFunc_HandleListRemove__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0015 );
#define	__Dt__DtFunc_UnlinkFromParent__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0016 );
#define	__Dt__DtFunc_HandleListIterate__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0017 );
#define	__Dt__DtFunc_HandleListIterate__DtStep_1	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0018 );
#define	__Dt__DtFunc_HandleListIterate__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0019 );
#define	__Dt__DtFunc_HandleListIterate__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001A );
#define	__Dt__DtFunc_IterateOverChildren__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001B );
#define	__Dt__DtFunc_GetHandleStructure__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x001C );
#define	__Dt__DtFunc_GetHandleStructure__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001D );
#define	__Dt__DtFunc_GetHandleStructure__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001E );
#define	__Dt__DtFunc_GetHandleStructure__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001F );
#define	__Dt__DtFunc_GetHandleStructure__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0020 );
#define	__Dt__DtFunc_GetHandleStructure__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0021 );
#define	__Dt__DtFunc_GetHandleStructure__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0022 );
#define	__Dt__DtFunc_GetHandleStructure__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0023 );
#define	__Dt__DtFunc_ParentIfPrivate__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0024 );
#define	__Dt__DtFunc_InitKey__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0025 );
#define	__Dt__DtFunc_InitKey__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0026 );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0027 );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0028 );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002A );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002B );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002C );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002D );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002E );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002F );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_8	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0031 );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0032 );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_10	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0033 );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0034 );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0035 );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_13	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0036 );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_14	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0037 );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_15	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0038 );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_16	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0039 );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_17	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003A );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_18	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003B );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_19	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003C );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_20	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003D );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_21	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003E );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_22	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003F );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_23	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0040 );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_24	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0041 );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_25	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0042 );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_26	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0043 );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_27	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0044 );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_28	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0045 );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_29	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0046 );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_30	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0047 );
#define	__Dt__DtFunc_ReallocHandleArray__DtStep_31	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0048 );
#define	__Dt__DtFunc_FreeHandleArray__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0049 );
#define	__Dt__DtFunc_FreeHandle__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x004A );
#define	__Dt__DtFunc_FreeHandle__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004B );
#define	__Dt__DtFunc_FreeHandle__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004C );
#define	__Dt__DtFunc_FreeHandle__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004D );
#define	__Dt__DtFunc_FreeHandle__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004E );
#define	__Dt__DtFunc_FreeHandle__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004F );
#define	__Dt__DtFunc_FreeHandle__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0050 );
#define	__Dt__DtFunc_FreeHandle__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0051 );
#define	__Dt__DtFunc_FreeHandle__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0052 );
#define	__Dt__DtFunc_FreeHandle__DtStep_9	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0053 );
#define	__Dt__DtFunc_FreeAllHandles__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0054 );
#define	__Dt__DtFunc_FreeAllHandles__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0055 );
#define	__Dt__DtFunc_FreeAllHandles__DtStep_2	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0056 );
#define	__Dt__DtFunc_FreeAllHandles__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0057 );
#define	__Dt__DtFunc_FreeAllHandles__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0058 );
#define	__Dt__DtFunc_FreeAllHandles__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0059 );
#define	__Dt__DtFunc_FreeAllHandles__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005A );
#define	__Dt__DtFunc_FreeHandleBase__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x005B );
#define	__Dt__DtFunc_FreeHandleBase__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005C );
#define	__Dt__DtFunc_FreeHandleBase__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005D );
#define	__Dt__DtFunc_FreeHandleBase__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005E );
#define	__Dt__DtFunc_FreeHandleBase__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005F );
#define	__Dt__DtFunc_FreeHandleBase__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0060 );
#define	__Dt__DtFunc_FreeHandleBase__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0061 );
#define	__Dt__DtFunc_FreeHandleBase__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0062 );
#define	__Dt__DtFunc_FreeHandleBase__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0063 );
#define	__Dt__DtFunc_FreeHandleBase__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0064 );
#define	__Dt__DtFunc_FindHandle__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0065 );
#define	__Dt__DtFunc_FindHandle__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0066 );
#define	__Dt__DtFunc_IncreaseHandleArraySize__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0067 );
#define	__Dt__DtFunc_IncreaseHandleArraySize__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0068 );
#define	__Dt__DtFunc_IncreaseHandleArraySize__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0069 );
#define	__Dt__DtFunc_IncreaseHandleArraySize__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006A );
#define	__Dt__DtFunc_IncreaseHandleArraySize__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006B );
#define	__Dt__DtFunc_IncreaseHandleArraySize__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006C );
#define	__Dt__DtFunc_IncreaseHandleArraySize__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006D );
#define	__Dt__DtFunc_EnsureFreeHandles__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x006E );
#define	__Dt__DtFunc_EnsureFreeHandles__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006F );
#define	__Dt__DtFunc_EnsureFreeHandles__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0070 );
#define	__Dt__DtFunc_EnsureFreeHandles__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0071 );
#define	__Dt__DtFunc_EnsureFreeHandles__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0072 );
#define	__Dt__DtFunc_AllocHandle__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0073 );
#define	__Dt__DtFunc_AllocHandle__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0074 );
#define	__Dt__DtFunc_AllocHandle__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0075 );
#define	__Dt__DtFunc_AllocHandle__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0076 );
#define	__Dt__DtFunc_AllocHandle__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0077 );
#define	__Dt__DtFunc_AllocHandle__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0078 );
#define	__Dt__DtFunc_AllocHandle__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0079 );
#define	__Dt__DtFunc_AllocHandle__DtStep_7	/*for*/	_TP_BusOut( __DtBaseAddress, 0x007A );
#define	__Dt__DtFunc_AllocHandle__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007B );
#define	__Dt__DtFunc_AllocHandle__DtStep_9	/*for*/	_TP_BusOut( __DtBaseAddress, 0x007C );
#define	__Dt__DtFunc_AllocHandle__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007D );
#define	__Dt__DtFunc_AllocHandle__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007E );
#define	__Dt__DtFunc_AllocHandle__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007F );
#define	__Dt__DtFunc_AllocHandle__DtStep_13	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0081 );
#define	__Dt__DtFunc_AllocHandle__DtStep_14	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0082 );
#define	__Dt__DtFunc_AllocHandle__DtStep_15	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0083 );
#define	__Dt__DtFunc_AllocHandle__DtStep_16	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0084 );
#define	__Dt__DtFunc_AllocHandle__DtStep_17	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0085 );
#define	__Dt__DtFunc_AllocHandle__DtStep_18	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0086 );
#define	__Dt__DtFunc_AllocHandle__DtStep_19	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0087 );
#define	__Dt__DtFunc_PVRSRVAllocHandle__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0088 );
#define	__Dt__DtFunc_PVRSRVAllocHandle__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0089 );
#define	__Dt__DtFunc_PVRSRVAllocHandle__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008A );
#define	__Dt__DtFunc_PVRSRVAllocHandle__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008B );
#define	__Dt__DtFunc_PVRSRVAllocHandle__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008C );
#define	__Dt__DtFunc_PVRSRVAllocHandle__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x008D );
#define	__Dt__DtFunc_PVRSRVAllocHandle__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008E );
#define	__Dt__DtFunc_PVRSRVAllocHandle__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x008F );
#define	__Dt__DtFunc_PVRSRVAllocHandle__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0091 );
#define	__Dt__DtFunc_PVRSRVAllocHandle__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0092 );
#define	__Dt__DtFunc_PVRSRVAllocSubHandle__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0093 );
#define	__Dt__DtFunc_PVRSRVAllocSubHandle__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0094 );
#define	__Dt__DtFunc_PVRSRVAllocSubHandle__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0095 );
#define	__Dt__DtFunc_PVRSRVAllocSubHandle__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0096 );
#define	__Dt__DtFunc_PVRSRVAllocSubHandle__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0097 );
#define	__Dt__DtFunc_PVRSRVAllocSubHandle__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0098 );
#define	__Dt__DtFunc_PVRSRVAllocSubHandle__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0099 );
#define	__Dt__DtFunc_PVRSRVAllocSubHandle__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009A );
#define	__Dt__DtFunc_PVRSRVAllocSubHandle__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x009B );
#define	__Dt__DtFunc_PVRSRVAllocSubHandle__DtStep_9	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x009C );
#define	__Dt__DtFunc_PVRSRVAllocSubHandle__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009D );
#define	__Dt__DtFunc_PVRSRVAllocSubHandle__DtStep_11	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x009E );
#define	__Dt__DtFunc_PVRSRVFindHandle__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x009F );
#define	__Dt__DtFunc_PVRSRVFindHandle__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A1 );
#define	__Dt__DtFunc_PVRSRVFindHandle__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A2 );
#define	__Dt__DtFunc_PVRSRVLookupHandleAnyType__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00A3 );
#define	__Dt__DtFunc_PVRSRVLookupHandleAnyType__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A4 );
#define	__Dt__DtFunc_PVRSRVLookupHandleAnyType__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A5 );
#define	__Dt__DtFunc_PVRSRVLookupHandleAnyType__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A6 );
#define	__Dt__DtFunc_PVRSRVLookupHandle__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00A7 );
#define	__Dt__DtFunc_PVRSRVLookupHandle__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A8 );
#define	__Dt__DtFunc_PVRSRVLookupHandle__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A9 );
#define	__Dt__DtFunc_PVRSRVLookupHandle__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00AA );
#define	__Dt__DtFunc_PVRSRVLookupSubHandle__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00AB );
#define	__Dt__DtFunc_PVRSRVLookupSubHandle__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00AC );
#define	__Dt__DtFunc_PVRSRVLookupSubHandle__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00AD );
#define	__Dt__DtFunc_PVRSRVLookupSubHandle__DtStep_3	/*for*/	_TP_BusOut( __DtBaseAddress, 0x00AE );
#define	__Dt__DtFunc_PVRSRVLookupSubHandle__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00AF );
#define	__Dt__DtFunc_PVRSRVLookupSubHandle__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B1 );
#define	__Dt__DtFunc_PVRSRVLookupSubHandle__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B2 );
#define	__Dt__DtFunc_PVRSRVGetParentHandle__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00B3 );
#define	__Dt__DtFunc_PVRSRVGetParentHandle__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B4 );
#define	__Dt__DtFunc_PVRSRVGetParentHandle__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B5 );
#define	__Dt__DtFunc_PVRSRVGetParentHandle__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B6 );
#define	__Dt__DtFunc_PVRSRVLookupAndReleaseHandle__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00B7 );
#define	__Dt__DtFunc_PVRSRVLookupAndReleaseHandle__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B8 );
#define	__Dt__DtFunc_PVRSRVLookupAndReleaseHandle__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B9 );
#define	__Dt__DtFunc_PVRSRVLookupAndReleaseHandle__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BA );
#define	__Dt__DtFunc_PVRSRVReleaseHandle__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00BB );
#define	__Dt__DtFunc_PVRSRVReleaseHandle__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00BC );
#define	__Dt__DtFunc_PVRSRVReleaseHandle__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BD );
#define	__Dt__DtFunc_PVRSRVReleaseHandle__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BE );
#define	__Dt__DtFunc_PVRSRVNewHandleBatch__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00BF );
#define	__Dt__DtFunc_PVRSRVNewHandleBatch__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C0 );
#define	__Dt__DtFunc_PVRSRVNewHandleBatch__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C1 );
#define	__Dt__DtFunc_PVRSRVNewHandleBatch__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C2 );
#define	__Dt__DtFunc_PVRSRVNewHandleBatch__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C3 );
#define	__Dt__DtFunc_PVRSRVNewHandleBatch__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C4 );
#define	__Dt__DtFunc_PVRSRVNewHandleBatch__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C5 );
#define	__Dt__DtFunc_PVRSRVNewHandleBatch__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C6 );
#define	__Dt__DtFunc_PVRSRVHandleBatchCommitOrRelease__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00C7 );
#define	__Dt__DtFunc_PVRSRVHandleBatchCommitOrRelease__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C8 );
#define	__Dt__DtFunc_PVRSRVHandleBatchCommitOrRelease__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C9 );
#define	__Dt__DtFunc_PVRSRVHandleBatchCommitOrRelease__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00CA );
#define	__Dt__DtFunc_PVRSRVHandleBatchCommitOrRelease__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00CB );
#define	__Dt__DtFunc_PVRSRVHandleBatchCommitOrRelease__DtStep_5	/*while*/	_TP_BusOut( __DtBaseAddress, 0x00CC );
#define	__Dt__DtFunc_PVRSRVHandleBatchCommitOrRelease__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00CD );
#define	__Dt__DtFunc_PVRSRVHandleBatchCommitOrRelease__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00CE );
#define	__Dt__DtFunc_PVRSRVHandleBatchCommitOrRelease__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00CF );
#define	__Dt__DtFunc_PVRSRVHandleBatchCommitOrRelease__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D0 );
#define	__Dt__DtFunc_PVRSRVHandleBatchCommitOrRelease__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D1 );
#define	__Dt__DtFunc_PVRSRVHandleBatchCommitOrRelease__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D2 );
#define	__Dt__DtFunc_PVRSRVHandleBatchCommitOrRelease__DtStep_12	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D3 );
#define	__Dt__DtFunc_PVRSRVHandleBatchCommitOrRelease__DtStep_13	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D4 );
#define	__Dt__DtFunc_PVRSRVCommitHandleBatch__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D5 );
#define	__Dt__DtFunc_PVRSRVReleaseHandleBatch__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00D6 );
#define	__Dt__DtFunc_PVRSRVReleaseHandleBatch__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D7 );
#define	__Dt__DtFunc_PVRSRVSetMaxHandle__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00D8 );
#define	__Dt__DtFunc_PVRSRVSetMaxHandle__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D9 );
#define	__Dt__DtFunc_PVRSRVSetMaxHandle__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DA );
#define	__Dt__DtFunc_PVRSRVSetMaxHandle__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00DB );
#define	__Dt__DtFunc_PVRSRVSetMaxHandle__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DC );
#define	__Dt__DtFunc_PVRSRVSetMaxHandle__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00DD );
#define	__Dt__DtFunc_PVRSRVSetMaxHandle__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DE );
#define	__Dt__DtFunc_PVRSRVSetMaxHandle__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00DF );
#define	__Dt__DtFunc_PVRSRVSetMaxHandle__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E0 );
#define	__Dt__DtFunc_PVRSRVGetMaxHandle__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E1 );
#define	__Dt__DtFunc_PVRSRVEnableHandlePurging__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00E2 );
#define	__Dt__DtFunc_PVRSRVEnableHandlePurging__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E3 );
#define	__Dt__DtFunc_PVRSRVEnableHandlePurging__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E4 );
#define	__Dt__DtFunc_PVRSRVEnableHandlePurging__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E5 );
#define	__Dt__DtFunc_PVRSRVEnableHandlePurging__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E6 );
#define	__Dt__DtFunc_PVRSRVEnableHandlePurging__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E7 );
#define	__Dt__DtFunc_PVRSRVPurgeHandles__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00E8 );
#define	__Dt__DtFunc_PVRSRVPurgeHandles__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E9 );
#define	__Dt__DtFunc_PVRSRVPurgeHandles__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00EA );
#define	__Dt__DtFunc_PVRSRVPurgeHandles__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00EB );
#define	__Dt__DtFunc_PVRSRVPurgeHandles__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00EC );
#define	__Dt__DtFunc_PVRSRVPurgeHandles__DtStep_5	/*for*/	_TP_BusOut( __DtBaseAddress, 0x00ED );
#define	__Dt__DtFunc_PVRSRVPurgeHandles__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00EE );
#define	__Dt__DtFunc_PVRSRVPurgeHandles__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00EF );
#define	__Dt__DtFunc_PVRSRVPurgeHandles__DtStep_8	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F1 );
#define	__Dt__DtFunc_PVRSRVPurgeHandles__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F2 );
#define	__Dt__DtFunc_PVRSRVAllocHandleBase__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00F3 );
#define	__Dt__DtFunc_PVRSRVAllocHandleBase__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F4 );
#define	__Dt__DtFunc_PVRSRVAllocHandleBase__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F5 );
#define	__Dt__DtFunc_PVRSRVAllocHandleBase__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F6 );
#define	__Dt__DtFunc_PVRSRVAllocHandleBase__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F7 );
#define	__Dt__DtFunc_PVRSRVAllocHandleBase__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F8 );
#define	__Dt__DtFunc_PVRSRVFreeHandleBase__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00F9 );
#define	__Dt__DtFunc_PVRSRVFreeHandleBase__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00FA );
#define	__Dt__DtFunc_PVRSRVFreeHandleBase__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00FB );
#define	__Dt__DtFunc_PVRSRVHandleInit__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00FC );
#define	__Dt__DtFunc_PVRSRVHandleInit__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00FD );
#define	__Dt__DtFunc_PVRSRVHandleInit__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00FE );
#define	__Dt__DtFunc_PVRSRVHandleInit__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00FF );
#define	__Dt__DtFunc_PVRSRVHandleInit__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0100 );
#define	__Dt__DtFunc_PVRSRVHandleDeInit__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0101 );
#define	__Dt__DtFunc_PVRSRVHandleDeInit__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0102 );
#define	__Dt__DtFunc_PVRSRVHandleDeInit__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0103 );
#define	__Dt__DtFunc_PVRSRVHandleDeInit__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0104 );
#define	__Dt__DtFunc_PVRSRVHandleDeInit__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0105 );

#endif	/* __DT_handle_h__ */

