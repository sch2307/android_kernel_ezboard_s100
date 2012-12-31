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

#ifndef	__DT_mm_h__
#define	__DT_mm_h__

#ifdef	__cplusplus
	#define	__DtEXTERN	extern "C"
#else
	#define	__DtEXTERN	extern
#endif

/* TestPoint MacroCode -----------------------------------------------------*/
#define		__DtBaseAddress		0x100
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
#define		__DtFunc_LinuxMMInit		0
#define		__DtFunc_LinuxMMCleanup_MemAreas_ForEachCb		1
#define		__DtFunc_LinuxMMCleanup_MemRecords_ForEachVa		2
#define		__DtFunc_LinuxMMCleanup		3
#define		__DtFunc__KMallocWrapper		4
#define		__DtFunc__KFreeWrapper		5
#define		__DtFunc_DebugMemAllocRecordAdd		6
#define		__DtFunc_DebugMemAllocRecordRemove_AnyVaCb		7
#define		__DtFunc_DebugMemAllocRecordRemove		8
#define		__DtFunc_DebugMemAllocRecordTypeToString		9
#define		__DtFunc__VMallocWrapper		10
#define		__DtFunc__VFreeWrapper		11
#define		__DtFunc_NewVMallocLinuxMemArea		12
#define		__DtFunc_FreeVMallocLinuxMemArea		13
#define		__DtFunc_ReservePages		14
#define		__DtFunc_UnreservePages		15
#define		__DtFunc__IORemapWrapper		16
#define		__DtFunc__IOUnmapWrapper		17
#define		__DtFunc_NewIORemapLinuxMemArea		18
#define		__DtFunc_FreeIORemapLinuxMemArea		19
#define		__DtFunc_TreatExternalPagesAsContiguous		20
#define		__DtFunc_NewExternalKVLinuxMemArea		21
#define		__DtFunc_FreeExternalKVLinuxMemArea		22
#define		__DtFunc_NewIOLinuxMemArea		23
#define		__DtFunc_FreeIOLinuxMemArea		24
#define		__DtFunc_NewAllocPagesLinuxMemArea		25
#define		__DtFunc_FreeAllocPagesLinuxMemArea		26
#define		__DtFunc_LinuxMemAreaOffsetToPage		27
#define		__DtFunc_KMemCacheCreateWrapper		28
#define		__DtFunc_KMemCacheDestroyWrapper		29
#define		__DtFunc__KMemCacheAllocWrapper		30
#define		__DtFunc__KMemCacheFreeWrapper		31
#define		__DtFunc_KMemCacheNameWrapper		32
#define		__DtFunc_NewSubLinuxMemArea		33
#define		__DtFunc_FreeSubLinuxMemArea		34
#define		__DtFunc_LinuxMemAreaStructAlloc		35
#define		__DtFunc_LinuxMemAreaStructFree		36
#define		__DtFunc_LinuxMemAreaDeepFree		37
#define		__DtFunc_DebugLinuxMemAreaRecordAdd		38
#define		__DtFunc_MatchLinuxMemArea_AnyVaCb		39
#define		__DtFunc_DebugLinuxMemAreaRecordFind		40
#define		__DtFunc_DebugLinuxMemAreaRecordRemove		41
#define		__DtFunc_LinuxMemAreaToCpuVAddr		42
#define		__DtFunc_LinuxMemAreaToCpuPAddr		43
#define		__DtFunc_LinuxMemAreaPhysIsContig		44
#define		__DtFunc_LinuxMemAreaTypeToString		45
#define		__DtFunc_ProcSeqStartstopDebugMutex		46
#define		__DtFunc_DecOffMemAreaRec_AnyVaCb		47
#define		__DtFunc_ProcSeqNextMemArea		48
#define		__DtFunc_ProcSeqOff2ElementMemArea		49
#define		__DtFunc_ProcSeqShowMemArea		50
#define		__DtFunc_DecOffMemAllocRec_AnyVaCb		51
#define		__DtFunc_ProcSeqNextMemoryRecords		52
#define		__DtFunc_ProcSeqOff2ElementMemoryRecords		53
#define		__DtFunc_ProcSeqShowMemoryRecords		54
#define		__DtFunc_HAPFlagsToString		55

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

/* TestPoint DisableList ---------------------------------------------------*/
#define	__Dt__DtFunc_LinuxMMInit__DtStep_0	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0000 );
#define	__Dt__DtFunc_LinuxMMInit__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0001 );
#define	__Dt__DtFunc_LinuxMMInit__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0002 );
#define	__Dt__DtFunc_LinuxMMInit__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0003 );
#define	__Dt__DtFunc_LinuxMMInit__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0004 );
#define	__Dt__DtFunc_LinuxMMCleanup_MemAreas_ForEachCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0005 );
#define	__Dt__DtFunc_LinuxMMCleanup_MemAreas_ForEachCb__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0006 );
#define	__Dt__DtFunc_LinuxMMCleanup_MemRecords_ForEachVa__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0007 );
#define	__Dt__DtFunc_LinuxMMCleanup_MemRecords_ForEachVa__DtStep_1	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0008 );
#define	__Dt__DtFunc_LinuxMMCleanup_MemRecords_ForEachVa__DtStep_2	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0009 );
#define	__Dt__DtFunc_LinuxMMCleanup_MemRecords_ForEachVa__DtStep_3	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x000A );
#define	__Dt__DtFunc_LinuxMMCleanup_MemRecords_ForEachVa__DtStep_4	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x000B );
#define	__Dt__DtFunc_LinuxMMCleanup_MemRecords_ForEachVa__DtStep_5	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x000C );
#define	__Dt__DtFunc_LinuxMMCleanup_MemRecords_ForEachVa__DtStep_6	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x000D );
#define	__Dt__DtFunc_LinuxMMCleanup_MemRecords_ForEachVa__DtStep_7	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x000E );
#define	__Dt__DtFunc_LinuxMMCleanup_MemRecords_ForEachVa__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000F );
#define	__Dt__DtFunc_LinuxMMCleanup__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0010 );
#define	__Dt__DtFunc_LinuxMMCleanup__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0011 );
#define	__Dt__DtFunc_LinuxMMCleanup__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0012 );
#define	__Dt__DtFunc__KMallocWrapper__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0013 );
#define	__Dt__DtFunc__KMallocWrapper__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0014 );
#define	__Dt__DtFunc__KFreeWrapper__DtStep_0	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0015 );
#define	__Dt__DtFunc_DebugMemAllocRecordAdd__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0016 );
#define	__Dt__DtFunc_DebugMemAllocRecordAdd__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0017 );
#define	__Dt__DtFunc_DebugMemAllocRecordAdd__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0018 );
#define	__Dt__DtFunc_DebugMemAllocRecordAdd__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0019 );
#define	__Dt__DtFunc_DebugMemAllocRecordAdd__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001A );
#define	__Dt__DtFunc_DebugMemAllocRecordAdd__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001B );
#define	__Dt__DtFunc_DebugMemAllocRecordAdd__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001C );
#define	__Dt__DtFunc_DebugMemAllocRecordRemove_AnyVaCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x001D );
#define	__Dt__DtFunc_DebugMemAllocRecordRemove_AnyVaCb__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001E );
#define	__Dt__DtFunc_DebugMemAllocRecordRemove_AnyVaCb__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001F );
#define	__Dt__DtFunc_DebugMemAllocRecordRemove_AnyVaCb__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0020 );
#define	__Dt__DtFunc_DebugMemAllocRecordRemove_AnyVaCb__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0021 );
#define	__Dt__DtFunc_DebugMemAllocRecordRemove_AnyVaCb__DtStep_5	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0022 );
#define	__Dt__DtFunc_DebugMemAllocRecordRemove_AnyVaCb__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0023 );
#define	__Dt__DtFunc_DebugMemAllocRecordRemove__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0024 );
#define	__Dt__DtFunc_DebugMemAllocRecordRemove__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0025 );
#define	__Dt__DtFunc_DebugMemAllocRecordRemove__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0026 );
#define	__Dt__DtFunc_DebugMemAllocRecordTypeToString__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0027 );
#define	__Dt__DtFunc__VMallocWrapper__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0028 );
#define	__Dt__DtFunc__VMallocWrapper__DtStep_1	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x002A );
#define	__Dt__DtFunc__VMallocWrapper__DtStep_2	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x002B );
#define	__Dt__DtFunc__VMallocWrapper__DtStep_3	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x002C );
#define	__Dt__DtFunc__VMallocWrapper__DtStep_4	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x002D );
#define	__Dt__DtFunc__VMallocWrapper__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002E );
#define	__Dt__DtFunc__VMallocWrapper__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002F );
#define	__Dt__DtFunc__VFreeWrapper__DtStep_0	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0031 );
#define	__Dt__DtFunc_NewVMallocLinuxMemArea__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0032 );
#define	__Dt__DtFunc_NewVMallocLinuxMemArea__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0033 );
#define	__Dt__DtFunc_NewVMallocLinuxMemArea__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0034 );
#define	__Dt__DtFunc_NewVMallocLinuxMemArea__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0035 );
#define	__Dt__DtFunc_NewVMallocLinuxMemArea__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0036 );
#define	__Dt__DtFunc_FreeVMallocLinuxMemArea__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0037 );
#define	__Dt__DtFunc_FreeVMallocLinuxMemArea__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0038 );
#define	__Dt__DtFunc_ReservePages__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0039 );
#define	__Dt__DtFunc_ReservePages__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003A );
#define	__Dt__DtFunc_UnreservePages__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x003B );
#define	__Dt__DtFunc_UnreservePages__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003C );
#define	__Dt__DtFunc__IORemapWrapper__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x003D );
#define	__Dt__DtFunc__IORemapWrapper__DtStep_1	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x003E );
#define	__Dt__DtFunc__IORemapWrapper__DtStep_2	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x003F );
#define	__Dt__DtFunc__IORemapWrapper__DtStep_3	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0040 );
#define	__Dt__DtFunc__IORemapWrapper__DtStep_4	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0041 );
#define	__Dt__DtFunc__IORemapWrapper__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0042 );
#define	__Dt__DtFunc__IORemapWrapper__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0043 );
#define	__Dt__DtFunc__IOUnmapWrapper__DtStep_0	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0044 );
#define	__Dt__DtFunc_NewIORemapLinuxMemArea__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0045 );
#define	__Dt__DtFunc_NewIORemapLinuxMemArea__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0046 );
#define	__Dt__DtFunc_NewIORemapLinuxMemArea__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0047 );
#define	__Dt__DtFunc_NewIORemapLinuxMemArea__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0048 );
#define	__Dt__DtFunc_FreeIORemapLinuxMemArea__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0049 );
#define	__Dt__DtFunc_FreeIORemapLinuxMemArea__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004A );
#define	__Dt__DtFunc_TreatExternalPagesAsContiguous__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x004B );
#define	__Dt__DtFunc_TreatExternalPagesAsContiguous__DtStep_1	/*for*/	_TP_BusOut( __DtBaseAddress, 0x004C );
#define	__Dt__DtFunc_TreatExternalPagesAsContiguous__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004D );
#define	__Dt__DtFunc_TreatExternalPagesAsContiguous__DtStep_3	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004E );
#define	__Dt__DtFunc_TreatExternalPagesAsContiguous__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004F );
#define	__Dt__DtFunc_TreatExternalPagesAsContiguous__DtStep_5	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0050 );
#define	__Dt__DtFunc_TreatExternalPagesAsContiguous__DtStep_6	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0051 );
#define	__Dt__DtFunc_TreatExternalPagesAsContiguous__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0052 );
#define	__Dt__DtFunc_NewExternalKVLinuxMemArea__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0053 );
#define	__Dt__DtFunc_NewExternalKVLinuxMemArea__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0054 );
#define	__Dt__DtFunc_NewExternalKVLinuxMemArea__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0055 );
#define	__Dt__DtFunc_NewExternalKVLinuxMemArea__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0056 );
#define	__Dt__DtFunc_FreeExternalKVLinuxMemArea__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0057 );
#define	__Dt__DtFunc_FreeExternalKVLinuxMemArea__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0058 );
#define	__Dt__DtFunc_NewIOLinuxMemArea__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0059 );
#define	__Dt__DtFunc_NewIOLinuxMemArea__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005A );
#define	__Dt__DtFunc_FreeIOLinuxMemArea__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x005B );
#define	__Dt__DtFunc_FreeIOLinuxMemArea__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005C );
#define	__Dt__DtFunc_NewAllocPagesLinuxMemArea__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x005D );
#define	__Dt__DtFunc_NewAllocPagesLinuxMemArea__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005E );
#define	__Dt__DtFunc_NewAllocPagesLinuxMemArea__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005F );
#define	__Dt__DtFunc_NewAllocPagesLinuxMemArea__DtStep_3	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0060 );
#define	__Dt__DtFunc_NewAllocPagesLinuxMemArea__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0061 );
#define	__Dt__DtFunc_NewAllocPagesLinuxMemArea__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0062 );
#define	__Dt__DtFunc_NewAllocPagesLinuxMemArea__DtStep_6	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0063 );
#define	__Dt__DtFunc_NewAllocPagesLinuxMemArea__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0064 );
#define	__Dt__DtFunc_FreeAllocPagesLinuxMemArea__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0065 );
#define	__Dt__DtFunc_FreeAllocPagesLinuxMemArea__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0066 );
#define	__Dt__DtFunc_LinuxMemAreaOffsetToPage__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0067 );
#define	__Dt__DtFunc_LinuxMemAreaOffsetToPage__DtStep_1	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0068 );
#define	__Dt__DtFunc_LinuxMemAreaOffsetToPage__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0069 );
#define	__Dt__DtFunc_LinuxMemAreaOffsetToPage__DtStep_3	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x006A );
#define	__Dt__DtFunc_LinuxMemAreaOffsetToPage__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006B );
#define	__Dt__DtFunc_LinuxMemAreaOffsetToPage__DtStep_5	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006C );
#define	__Dt__DtFunc_LinuxMemAreaOffsetToPage__DtStep_6	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x006D );
#define	__Dt__DtFunc_LinuxMemAreaOffsetToPage__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006E );
#define	__Dt__DtFunc_LinuxMemAreaOffsetToPage__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006F );
#define	__Dt__DtFunc_KMemCacheDestroyWrapper__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0070 );
#define	__Dt__DtFunc_KMemCacheDestroyWrapper__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0071 );
#define	__Dt__DtFunc__KMemCacheAllocWrapper__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0072 );
#define	__Dt__DtFunc__KMemCacheFreeWrapper__DtStep_0	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0073 );
#define	__Dt__DtFunc_KMemCacheNameWrapper__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0074 );
#define	__Dt__DtFunc_KMemCacheNameWrapper__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0075 );
#define	__Dt__DtFunc_NewSubLinuxMemArea__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0076 );
#define	__Dt__DtFunc_NewSubLinuxMemArea__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0077 );
#define	__Dt__DtFunc_FreeSubLinuxMemArea__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0078 );
#define	__Dt__DtFunc_FreeSubLinuxMemArea__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0079 );
#define	__Dt__DtFunc_LinuxMemAreaStructAlloc__DtStep_0	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007A );
#define	__Dt__DtFunc_LinuxMemAreaStructFree__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x007B );
#define	__Dt__DtFunc_LinuxMemAreaStructFree__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007C );
#define	__Dt__DtFunc_LinuxMemAreaDeepFree__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x007D );
#define	__Dt__DtFunc_LinuxMemAreaDeepFree__DtStep_1	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x007E );
#define	__Dt__DtFunc_LinuxMemAreaDeepFree__DtStep_2	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x007F );
#define	__Dt__DtFunc_LinuxMemAreaDeepFree__DtStep_3	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0081 );
#define	__Dt__DtFunc_LinuxMemAreaDeepFree__DtStep_4	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0082 );
#define	__Dt__DtFunc_LinuxMemAreaDeepFree__DtStep_5	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0083 );
#define	__Dt__DtFunc_LinuxMemAreaDeepFree__DtStep_6	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0084 );
#define	__Dt__DtFunc_LinuxMemAreaDeepFree__DtStep_7	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0085 );
#define	__Dt__DtFunc_LinuxMemAreaDeepFree__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0086 );
#define	__Dt__DtFunc_DebugLinuxMemAreaRecordAdd__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0087 );
#define	__Dt__DtFunc_DebugLinuxMemAreaRecordAdd__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0088 );
#define	__Dt__DtFunc_DebugLinuxMemAreaRecordAdd__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0089 );
#define	__Dt__DtFunc_DebugLinuxMemAreaRecordAdd__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008A );
#define	__Dt__DtFunc_DebugLinuxMemAreaRecordAdd__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008B );
#define	__Dt__DtFunc_DebugLinuxMemAreaRecordAdd__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008C );
#define	__Dt__DtFunc_DebugLinuxMemAreaRecordAdd__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x008D );
#define	__Dt__DtFunc_MatchLinuxMemArea_AnyVaCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x008E );
#define	__Dt__DtFunc_MatchLinuxMemArea_AnyVaCb__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x008F );
#define	__Dt__DtFunc_MatchLinuxMemArea_AnyVaCb__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0091 );
#define	__Dt__DtFunc_MatchLinuxMemArea_AnyVaCb__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0092 );
#define	__Dt__DtFunc_DebugLinuxMemAreaRecordFind__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0093 );
#define	__Dt__DtFunc_DebugLinuxMemAreaRecordFind__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0094 );
#define	__Dt__DtFunc_DebugLinuxMemAreaRecordRemove__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0095 );
#define	__Dt__DtFunc_DebugLinuxMemAreaRecordRemove__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0096 );
#define	__Dt__DtFunc_DebugLinuxMemAreaRecordRemove__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0097 );
#define	__Dt__DtFunc_DebugLinuxMemAreaRecordRemove__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0098 );
#define	__Dt__DtFunc_DebugLinuxMemAreaRecordRemove__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0099 );
#define	__Dt__DtFunc_LinuxMemAreaToCpuVAddr__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x009A );
#define	__Dt__DtFunc_LinuxMemAreaToCpuVAddr__DtStep_1	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x009B );
#define	__Dt__DtFunc_LinuxMemAreaToCpuVAddr__DtStep_2	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x009C );
#define	__Dt__DtFunc_LinuxMemAreaToCpuVAddr__DtStep_3	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x009D );
#define	__Dt__DtFunc_LinuxMemAreaToCpuVAddr__DtStep_4	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x009E );
#define	__Dt__DtFunc_LinuxMemAreaToCpuVAddr__DtStep_5	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x009F );
#define	__Dt__DtFunc_LinuxMemAreaToCpuVAddr__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A1 );
#define	__Dt__DtFunc_LinuxMemAreaToCpuVAddr__DtStep_7	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A2 );
#define	__Dt__DtFunc_LinuxMemAreaToCpuVAddr__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A3 );
#define	__Dt__DtFunc_LinuxMemAreaToCpuPAddr__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00A4 );
#define	__Dt__DtFunc_LinuxMemAreaToCpuPAddr__DtStep_1	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00A5 );
#define	__Dt__DtFunc_LinuxMemAreaToCpuPAddr__DtStep_2	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00A6 );
#define	__Dt__DtFunc_LinuxMemAreaToCpuPAddr__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A7 );
#define	__Dt__DtFunc_LinuxMemAreaToCpuPAddr__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A8 );
#define	__Dt__DtFunc_LinuxMemAreaToCpuPAddr__DtStep_5	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00A9 );
#define	__Dt__DtFunc_LinuxMemAreaToCpuPAddr__DtStep_6	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00AA );
#define	__Dt__DtFunc_LinuxMemAreaToCpuPAddr__DtStep_7	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00AB );
#define	__Dt__DtFunc_LinuxMemAreaToCpuPAddr__DtStep_8	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00AC );
#define	__Dt__DtFunc_LinuxMemAreaToCpuPAddr__DtStep_9	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00AD );
#define	__Dt__DtFunc_LinuxMemAreaToCpuPAddr__DtStep_10	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00AE );
#define	__Dt__DtFunc_LinuxMemAreaPhysIsContig__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00AF );
#define	__Dt__DtFunc_LinuxMemAreaPhysIsContig__DtStep_1	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B1 );
#define	__Dt__DtFunc_LinuxMemAreaPhysIsContig__DtStep_2	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B2 );
#define	__Dt__DtFunc_LinuxMemAreaPhysIsContig__DtStep_3	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B3 );
#define	__Dt__DtFunc_LinuxMemAreaPhysIsContig__DtStep_4	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B4 );
#define	__Dt__DtFunc_LinuxMemAreaPhysIsContig__DtStep_5	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00B5 );
#define	__Dt__DtFunc_LinuxMemAreaPhysIsContig__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B6 );
#define	__Dt__DtFunc_LinuxMemAreaTypeToString__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00B7 );
#define	__Dt__DtFunc_LinuxMemAreaTypeToString__DtStep_1	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B8 );
#define	__Dt__DtFunc_LinuxMemAreaTypeToString__DtStep_2	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B9 );
#define	__Dt__DtFunc_LinuxMemAreaTypeToString__DtStep_3	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BA );
#define	__Dt__DtFunc_LinuxMemAreaTypeToString__DtStep_4	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BB );
#define	__Dt__DtFunc_LinuxMemAreaTypeToString__DtStep_5	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BC );
#define	__Dt__DtFunc_LinuxMemAreaTypeToString__DtStep_6	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BD );
#define	__Dt__DtFunc_LinuxMemAreaTypeToString__DtStep_7	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x00BE );
#define	__Dt__DtFunc_LinuxMemAreaTypeToString__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BF );
#define	__Dt__DtFunc_ProcSeqStartstopDebugMutex__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00C0 );
#define	__Dt__DtFunc_ProcSeqStartstopDebugMutex__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C1 );
#define	__Dt__DtFunc_ProcSeqStartstopDebugMutex__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C2 );
#define	__Dt__DtFunc_ProcSeqStartstopDebugMutex__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C3 );
#define	__Dt__DtFunc_DecOffMemAreaRec_AnyVaCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00C4 );
#define	__Dt__DtFunc_DecOffMemAreaRec_AnyVaCb__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C5 );
#define	__Dt__DtFunc_DecOffMemAreaRec_AnyVaCb__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C6 );
#define	__Dt__DtFunc_DecOffMemAreaRec_AnyVaCb__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C7 );
#define	__Dt__DtFunc_ProcSeqNextMemArea__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00C8 );
#define	__Dt__DtFunc_ProcSeqNextMemArea__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C9 );
#define	__Dt__DtFunc_ProcSeqOff2ElementMemArea__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00CA );
#define	__Dt__DtFunc_ProcSeqOff2ElementMemArea__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CB );
#define	__Dt__DtFunc_ProcSeqOff2ElementMemArea__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CC );
#define	__Dt__DtFunc_ProcSeqShowMemArea__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00CD );
#define	__Dt__DtFunc_ProcSeqShowMemArea__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CE );
#define	__Dt__DtFunc_DecOffMemAllocRec_AnyVaCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00CF );
#define	__Dt__DtFunc_DecOffMemAllocRec_AnyVaCb__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D0 );
#define	__Dt__DtFunc_DecOffMemAllocRec_AnyVaCb__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D1 );
#define	__Dt__DtFunc_DecOffMemAllocRec_AnyVaCb__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D2 );
#define	__Dt__DtFunc_ProcSeqNextMemoryRecords__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00D3 );
#define	__Dt__DtFunc_ProcSeqNextMemoryRecords__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D4 );
#define	__Dt__DtFunc_ProcSeqOff2ElementMemoryRecords__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00D5 );
#define	__Dt__DtFunc_ProcSeqOff2ElementMemoryRecords__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D6 );
#define	__Dt__DtFunc_ProcSeqOff2ElementMemoryRecords__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D7 );
#define	__Dt__DtFunc_ProcSeqShowMemoryRecords__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00D8 );
#define	__Dt__DtFunc_ProcSeqShowMemoryRecords__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D9 );
#define	__Dt__DtFunc_ProcSeqShowMemoryRecords__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00DA );
#define	__Dt__DtFunc_ProcSeqShowMemoryRecords__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DB );
#define	__Dt__DtFunc_HAPFlagsToString__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00DC );
#define	__Dt__DtFunc_HAPFlagsToString__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00DD );
#define	__Dt__DtFunc_HAPFlagsToString__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00DE );
#define	__Dt__DtFunc_HAPFlagsToString__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00DF );
#define	__Dt__DtFunc_HAPFlagsToString__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E0 );
#define	__Dt__DtFunc_HAPFlagsToString__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E1 );
#define	__Dt__DtFunc_HAPFlagsToString__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E2 );
#define	__Dt__DtFunc_HAPFlagsToString__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E3 );
#define	__Dt__DtFunc_HAPFlagsToString__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E4 );
#define	__Dt__DtFunc_HAPFlagsToString__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E5 );
#define	__Dt__DtFunc_HAPFlagsToString__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E6 );
#define	__Dt__DtFunc_HAPFlagsToString__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E7 );
#define	__Dt__DtFunc_HAPFlagsToString__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E8 );
#define	__Dt__DtFunc_HAPFlagsToString__DtStep_13	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E9 );

#endif	/* __DT_mm_h__ */

