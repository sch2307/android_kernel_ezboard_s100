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

#ifndef	__DT_mmu_h__
#define	__DT_mmu_h__

#ifdef	__cplusplus
	#define	__DtEXTERN	extern "C"
#else
	#define	__DtEXTERN	extern
#endif

/* TestPoint MacroCode -----------------------------------------------------*/
#define		__DtBaseAddress		0x290
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
#define		__DtFunc_DumpPT		0
#define		__DtFunc_CheckPT		1
#define		__DtFunc_MMU_IsHeapShared		4
#define		__DtFunc_EnableHostAccess		5
#define		__DtFunc_DisableHostAccess		6
#define		__DtFunc_MMU_InvalidateSystemLevelCache		7
#define		__DtFunc_MMU_InvalidateDirectoryCache		8
#define		__DtFunc_MMU_InvalidatePageTableCache		9
#define		__DtFunc__AllocPageTableMemory		10
#define		__DtFunc__FreePageTableMemory		11
#define		__DtFunc__DeferredFreePageTable		12
#define		__DtFunc__DeferredFreePageTables		13
#define		__DtFunc__DeferredAllocPagetables		14
#define		__DtFunc_MMU_GetPDumpContextID		15
#define		__DtFunc_MMU_SetPDumpAttribs		16
#define		__DtFunc_MMU_Initialise		17
#define		__DtFunc_MMU_Finalise		18
#define		__DtFunc_MMU_InsertHeap		19
#define		__DtFunc_MMU_UnmapPagesAndFreePTs		20
#define		__DtFunc_MMU_FreePageTables		21
#define		__DtFunc_MMU_Create		22
#define		__DtFunc_MMU_Delete		23
#define		__DtFunc_MMU_Alloc		24
#define		__DtFunc_MMU_Free		25
#define		__DtFunc_MMU_Enable		26
#define		__DtFunc_MMU_Disable		27
#define		__DtFunc_MMU_PDumpPageTables		28
#define		__DtFunc_MMU_MapPage		29
#define		__DtFunc_MMU_MapScatter		30
#define		__DtFunc_MMU_MapPages		31
#define		__DtFunc_MMU_MapShadow		32
#define		__DtFunc_MMU_UnmapPages		33
#define		__DtFunc_MMU_GetPhysPageAddr		34
#define		__DtFunc_MMU_GetPDDevPAddr		35
#define		__DtFunc_SGXGetPhysPageAddrKM		36
#define		__DtFunc_SGXGetMMUPDAddrKM		37
#define		__DtFunc_MMU_BIFResetPDAlloc		38
#define		__DtFunc_MMU_BIFResetPDFree		39
#define		__DtFunc_WorkaroundBRN22997Alloc		40
#define		__DtFunc_WorkaroundBRN22997ReadHostPort		41
#define		__DtFunc_WorkaroundBRN22997Free		42
#define		__DtFunc_MMU_MapExtSystemCacheRegs		43
#define		__DtFunc_MMU_UnmapExtSystemCacheRegs		44
#define		__DtFunc_PageTest		45

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

/* TestPoint DisableList ---------------------------------------------------*/
#define	__Dt__DtFunc_DumpPT__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0000 );
#define	__Dt__DtFunc_DumpPT__DtStep_1	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0001 );
#define	__Dt__DtFunc_DumpPT__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0002 );
#define	__Dt__DtFunc_CheckPT__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0003 );
#define	__Dt__DtFunc_CheckPT__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0004 );
#define	__Dt__DtFunc_CheckPT__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0005 );
#define	__Dt__DtFunc_DumpPT__DtStep_3	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0006 );
#define	__Dt__DtFunc_DumpPT__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0007 );
#define	__Dt__DtFunc_CheckPT__DtStep_3	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0008 );
#define	__Dt__DtFunc_CheckPT__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0009 );
#define	__Dt__DtFunc_MMU_IsHeapShared__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x000A );
#define	__Dt__DtFunc_MMU_IsHeapShared__DtStep_1	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000B );
#define	__Dt__DtFunc_MMU_IsHeapShared__DtStep_2	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000C );
#define	__Dt__DtFunc_MMU_IsHeapShared__DtStep_3	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x000D );
#define	__Dt__DtFunc_MMU_IsHeapShared__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000E );
#define	__Dt__DtFunc_MMU_IsHeapShared__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000F );
#define	__Dt__DtFunc_EnableHostAccess__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0010 );
#define	__Dt__DtFunc_EnableHostAccess__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0011 );
#define	__Dt__DtFunc_DisableHostAccess__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0012 );
#define	__Dt__DtFunc_DisableHostAccess__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0013 );
#define	__Dt__DtFunc_MMU_InvalidateSystemLevelCache__DtStep_0	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0014 );
#define	__Dt__DtFunc_MMU_InvalidateDirectoryCache__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0015 );
#define	__Dt__DtFunc_MMU_InvalidateDirectoryCache__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0016 );
#define	__Dt__DtFunc_MMU_InvalidatePageTableCache__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0017 );
#define	__Dt__DtFunc_MMU_InvalidatePageTableCache__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0018 );
#define	__Dt__DtFunc__AllocPageTableMemory__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0019 );
#define	__Dt__DtFunc__AllocPageTableMemory__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001A );
#define	__Dt__DtFunc__AllocPageTableMemory__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001B );
#define	__Dt__DtFunc__AllocPageTableMemory__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001C );
#define	__Dt__DtFunc__AllocPageTableMemory__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001D );
#define	__Dt__DtFunc__AllocPageTableMemory__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001E );
#define	__Dt__DtFunc__AllocPageTableMemory__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001F );
#define	__Dt__DtFunc__AllocPageTableMemory__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0020 );
#define	__Dt__DtFunc__AllocPageTableMemory__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0021 );
#define	__Dt__DtFunc__AllocPageTableMemory__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0022 );
#define	__Dt__DtFunc__AllocPageTableMemory__DtStep_10	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0023 );
#define	__Dt__DtFunc__AllocPageTableMemory__DtStep_11	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0024 );
#define	__Dt__DtFunc__AllocPageTableMemory__DtStep_12	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0025 );
#define	__Dt__DtFunc__FreePageTableMemory__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0026 );
#define	__Dt__DtFunc__FreePageTableMemory__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0027 );
#define	__Dt__DtFunc__FreePageTableMemory__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0028 );
#define	__Dt__DtFunc__FreePageTableMemory__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002A );
#define	__Dt__DtFunc__DeferredFreePageTable__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x002B );
#define	__Dt__DtFunc__DeferredFreePageTable__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002C );
#define	__Dt__DtFunc__DeferredFreePageTable__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002D );
#define	__Dt__DtFunc__DeferredFreePageTable__DtStep_3	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x002E );
#define	__Dt__DtFunc__DeferredFreePageTable__DtStep_4	/*while*/	_TP_BusOut( __DtBaseAddress, 0x002F );
#define	__Dt__DtFunc__DeferredFreePageTable__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0031 );
#define	__Dt__DtFunc__DeferredFreePageTable__DtStep_6	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0032 );
#define	__Dt__DtFunc__DeferredFreePageTable__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0033 );
#define	__Dt__DtFunc__DeferredFreePageTable__DtStep_8	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0034 );
#define	__Dt__DtFunc__DeferredFreePageTable__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0035 );
#define	__Dt__DtFunc__DeferredFreePageTable__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0036 );
#define	__Dt__DtFunc__DeferredFreePageTable__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0037 );
#define	__Dt__DtFunc__DeferredFreePageTable__DtStep_12	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0038 );
#define	__Dt__DtFunc__DeferredFreePageTable__DtStep_13	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0039 );
#define	__Dt__DtFunc__DeferredFreePageTable__DtStep_14	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003A );
#define	__Dt__DtFunc__DeferredFreePageTable__DtStep_15	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003B );
#define	__Dt__DtFunc__DeferredFreePageTable__DtStep_16	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003C );
#define	__Dt__DtFunc__DeferredFreePageTable__DtStep_17	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003D );
#define	__Dt__DtFunc__DeferredFreePageTables__DtStep_0	/*for*/	_TP_BusOut( __DtBaseAddress, 0x003E );
#define	__Dt__DtFunc__DeferredFreePageTables__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003F );
#define	__Dt__DtFunc__DeferredAllocPagetables__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0040 );
#define	__Dt__DtFunc__DeferredAllocPagetables__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0041 );
#define	__Dt__DtFunc__DeferredAllocPagetables__DtStep_2	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0042 );
#define	__Dt__DtFunc__DeferredAllocPagetables__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0043 );
#define	__Dt__DtFunc__DeferredAllocPagetables__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0044 );
#define	__Dt__DtFunc__DeferredAllocPagetables__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0045 );
#define	__Dt__DtFunc__DeferredAllocPagetables__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0046 );
#define	__Dt__DtFunc__DeferredAllocPagetables__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0047 );
#define	__Dt__DtFunc__DeferredAllocPagetables__DtStep_8	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0048 );
#define	__Dt__DtFunc__DeferredAllocPagetables__DtStep_9	/*while*/	_TP_BusOut( __DtBaseAddress, 0x0049 );
#define	__Dt__DtFunc__DeferredAllocPagetables__DtStep_10	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x004A );
#define	__Dt__DtFunc__DeferredAllocPagetables__DtStep_11	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x004B );
#define	__Dt__DtFunc__DeferredAllocPagetables__DtStep_12	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004C );
#define	__Dt__DtFunc__DeferredAllocPagetables__DtStep_13	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004D );
#define	__Dt__DtFunc_MMU_GetPDumpContextID__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x004E );
#define	__Dt__DtFunc_MMU_GetPDumpContextID__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004F );
#define	__Dt__DtFunc_MMU_SetPDumpAttribs__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0050 );
#define	__Dt__DtFunc_MMU_SetPDumpAttribs__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0051 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0052 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0053 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0054 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0055 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0056 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0057 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0058 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0059 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005A );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005B );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005C );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005D );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005E );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_13	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005F );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_14	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0060 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_15	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0061 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_16	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0062 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_17	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0063 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_18	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0064 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_19	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0065 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_20	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0066 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_21	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0067 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_22	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0068 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_23	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0069 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_24	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006A );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_25	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006B );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_26	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006C );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_27	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006D );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_28	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006E );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_29	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006F );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_30	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0070 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_31	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0071 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_32	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0072 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_33	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0073 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_34	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0074 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_35	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0075 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_36	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0076 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_37	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0077 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_38	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0078 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_39	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0079 );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_40	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007A );
#define	__Dt__DtFunc_MMU_Initialise__DtStep_41	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007B );
#define	__Dt__DtFunc_MMU_Finalise__DtStep_0	/*for*/	_TP_BusOut( __DtBaseAddress, 0x007C );
#define	__Dt__DtFunc_MMU_Finalise__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007D );
#define	__Dt__DtFunc_MMU_Finalise__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007E );
#define	__Dt__DtFunc_MMU_Finalise__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007F );
#define	__Dt__DtFunc_MMU_Finalise__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0081 );
#define	__Dt__DtFunc_MMU_Finalise__DtStep_5	/*while*/	_TP_BusOut( __DtBaseAddress, 0x0082 );
#define	__Dt__DtFunc_MMU_Finalise__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0083 );
#define	__Dt__DtFunc_MMU_Finalise__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0084 );
#define	__Dt__DtFunc_MMU_InsertHeap__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0085 );
#define	__Dt__DtFunc_MMU_InsertHeap__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0086 );
#define	__Dt__DtFunc_MMU_UnmapPagesAndFreePTs__DtStep_0	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0087 );
#define	__Dt__DtFunc_MMU_UnmapPagesAndFreePTs__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0088 );
#define	__Dt__DtFunc_MMU_UnmapPagesAndFreePTs__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0089 );
#define	__Dt__DtFunc_MMU_UnmapPagesAndFreePTs__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008A );
#define	__Dt__DtFunc_MMU_UnmapPagesAndFreePTs__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008B );
#define	__Dt__DtFunc_MMU_UnmapPagesAndFreePTs__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008C );
#define	__Dt__DtFunc_MMU_UnmapPagesAndFreePTs__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008D );
#define	__Dt__DtFunc_MMU_UnmapPagesAndFreePTs__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008E );
#define	__Dt__DtFunc_MMU_UnmapPagesAndFreePTs__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x008F );
#define	__Dt__DtFunc_MMU_FreePageTables__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0091 );
#define	__Dt__DtFunc_MMU_FreePageTables__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0092 );
#define	__Dt__DtFunc_MMU_Create__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0093 );
#define	__Dt__DtFunc_MMU_Create__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0094 );
#define	__Dt__DtFunc_MMU_Create__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0095 );
#define	__Dt__DtFunc_MMU_Create__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0096 );
#define	__Dt__DtFunc_MMU_Create__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0097 );
#define	__Dt__DtFunc_MMU_Create__DtStep_5	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0098 );
#define	__Dt__DtFunc_MMU_Create__DtStep_6	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0099 );
#define	__Dt__DtFunc_MMU_Create__DtStep_7	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x009A );
#define	__Dt__DtFunc_MMU_Create__DtStep_8	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x009B );
#define	__Dt__DtFunc_MMU_Create__DtStep_9	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x009C );
#define	__Dt__DtFunc_MMU_Create__DtStep_10	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x009D );
#define	__Dt__DtFunc_MMU_Create__DtStep_11	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x009E );
#define	__Dt__DtFunc_MMU_Create__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009F );
#define	__Dt__DtFunc_MMU_Create__DtStep_13	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A1 );
#define	__Dt__DtFunc_MMU_Create__DtStep_14	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A2 );
#define	__Dt__DtFunc_MMU_Create__DtStep_15	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A3 );
#define	__Dt__DtFunc_MMU_Create__DtStep_16	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A4 );
#define	__Dt__DtFunc_MMU_Create__DtStep_17	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A5 );
#define	__Dt__DtFunc_MMU_Delete__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00A6 );
#define	__Dt__DtFunc_MMU_Delete__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A7 );
#define	__Dt__DtFunc_MMU_Delete__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A8 );
#define	__Dt__DtFunc_MMU_Delete__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A9 );
#define	__Dt__DtFunc_MMU_Alloc__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00AA );
#define	__Dt__DtFunc_MMU_Alloc__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00AB );
#define	__Dt__DtFunc_MMU_Alloc__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00AC );
#define	__Dt__DtFunc_MMU_Alloc__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00AD );
#define	__Dt__DtFunc_MMU_Alloc__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00AE );
#define	__Dt__DtFunc_MMU_Alloc__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00AF );
#define	__Dt__DtFunc_MMU_Alloc__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B1 );
#define	__Dt__DtFunc_MMU_Free__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00B2 );
#define	__Dt__DtFunc_MMU_Free__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B3 );
#define	__Dt__DtFunc_MMU_Free__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B4 );
#define	__Dt__DtFunc_MMU_Free__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B5 );
#define	__Dt__DtFunc_MMU_Free__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B6 );
#define	__Dt__DtFunc_MMU_Free__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B7 );
#define	__Dt__DtFunc_MMU_Enable__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00B8 );
#define	__Dt__DtFunc_MMU_Enable__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B9 );
#define	__Dt__DtFunc_MMU_Disable__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00BA );
#define	__Dt__DtFunc_MMU_Disable__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BB );
#define	__Dt__DtFunc_MMU_PDumpPageTables__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00BC );
#define	__Dt__DtFunc_MMU_PDumpPageTables__DtStep_1	/*while*/	_TP_BusOut( __DtBaseAddress, 0x00BD );
#define	__Dt__DtFunc_MMU_PDumpPageTables__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00BE );
#define	__Dt__DtFunc_MMU_PDumpPageTables__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00BF );
#define	__Dt__DtFunc_MMU_PDumpPageTables__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C0 );
#define	__Dt__DtFunc_MMU_MapPage__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00C1 );
#define	__Dt__DtFunc_MMU_MapPage__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C2 );
#define	__Dt__DtFunc_MMU_MapPage__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C3 );
#define	__Dt__DtFunc_MMU_MapPage__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C4 );
#define	__Dt__DtFunc_MMU_MapPage__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C5 );
#define	__Dt__DtFunc_MMU_MapPage__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C6 );
#define	__Dt__DtFunc_MMU_MapPage__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C7 );
#define	__Dt__DtFunc_MMU_MapPage__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C8 );
#define	__Dt__DtFunc_MMU_MapScatter__DtStep_0	/*for*/	_TP_BusOut( __DtBaseAddress, 0x00C9 );
#define	__Dt__DtFunc_MMU_MapScatter__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CA );
#define	__Dt__DtFunc_MMU_MapPages__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00CB );
#define	__Dt__DtFunc_MMU_MapPages__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00CC );
#define	__Dt__DtFunc_MMU_MapPages__DtStep_2	/*for*/	_TP_BusOut( __DtBaseAddress, 0x00CD );
#define	__Dt__DtFunc_MMU_MapPages__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CE );
#define	__Dt__DtFunc_MMU_MapShadow__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00CF );
#define	__Dt__DtFunc_MMU_MapShadow__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D0 );
#define	__Dt__DtFunc_MMU_MapShadow__DtStep_2	/*for*/	_TP_BusOut( __DtBaseAddress, 0x00D1 );
#define	__Dt__DtFunc_MMU_MapShadow__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D2 );
#define	__Dt__DtFunc_MMU_MapShadow__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D3 );
#define	__Dt__DtFunc_MMU_MapShadow__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D4 );
#define	__Dt__DtFunc_MMU_UnmapPages__DtStep_0	/*for*/	_TP_BusOut( __DtBaseAddress, 0x00D5 );
#define	__Dt__DtFunc_MMU_UnmapPages__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D6 );
#define	__Dt__DtFunc_MMU_UnmapPages__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D7 );
#define	__Dt__DtFunc_MMU_UnmapPages__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D8 );
#define	__Dt__DtFunc_MMU_UnmapPages__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D9 );
#define	__Dt__DtFunc_MMU_GetPhysPageAddr__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00DA );
#define	__Dt__DtFunc_MMU_GetPhysPageAddr__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00DB );
#define	__Dt__DtFunc_MMU_GetPhysPageAddr__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DC );
#define	__Dt__DtFunc_MMU_GetPhysPageAddr__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DD );
#define	__Dt__DtFunc_MMU_GetPDDevPAddr__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DE );
#define	__Dt__DtFunc_SGXGetPhysPageAddrKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00DF );
#define	__Dt__DtFunc_SGXGetPhysPageAddrKM__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E0 );
#define	__Dt__DtFunc_SGXGetMMUPDAddrKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00E1 );
#define	__Dt__DtFunc_SGXGetMMUPDAddrKM__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E2 );
#define	__Dt__DtFunc_SGXGetMMUPDAddrKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E3 );
#define	__Dt__DtFunc_MMU_BIFResetPDAlloc__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00E4 );
#define	__Dt__DtFunc_MMU_BIFResetPDAlloc__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E5 );
#define	__Dt__DtFunc_MMU_BIFResetPDAlloc__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E6 );
#define	__Dt__DtFunc_MMU_BIFResetPDAlloc__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E7 );
#define	__Dt__DtFunc_MMU_BIFResetPDAlloc__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E8 );
#define	__Dt__DtFunc_MMU_BIFResetPDAlloc__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E9 );
#define	__Dt__DtFunc_MMU_BIFResetPDAlloc__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00EA );
#define	__Dt__DtFunc_MMU_BIFResetPDAlloc__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00EB );
#define	__Dt__DtFunc_MMU_BIFResetPDAlloc__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00EC );
#define	__Dt__DtFunc_MMU_BIFResetPDAlloc__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00ED );
#define	__Dt__DtFunc_MMU_BIFResetPDAlloc__DtStep_10	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00EE );
#define	__Dt__DtFunc_MMU_BIFResetPDAlloc__DtStep_11	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00EF );
#define	__Dt__DtFunc_MMU_BIFResetPDFree__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00F1 );
#define	__Dt__DtFunc_MMU_BIFResetPDFree__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F2 );
#define	__Dt__DtFunc_MMU_BIFResetPDFree__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F3 );
#define	__Dt__DtFunc_MMU_BIFResetPDFree__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F4 );
#define	__Dt__DtFunc_WorkaroundBRN22997Alloc__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00F5 );
#define	__Dt__DtFunc_WorkaroundBRN22997Alloc__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F6 );
#define	__Dt__DtFunc_WorkaroundBRN22997Alloc__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F7 );
#define	__Dt__DtFunc_WorkaroundBRN22997Alloc__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F8 );
#define	__Dt__DtFunc_WorkaroundBRN22997Alloc__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F9 );
#define	__Dt__DtFunc_WorkaroundBRN22997Alloc__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00FA );
#define	__Dt__DtFunc_WorkaroundBRN22997Alloc__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00FB );
#define	__Dt__DtFunc_WorkaroundBRN22997Alloc__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00FC );
#define	__Dt__DtFunc_WorkaroundBRN22997Alloc__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00FD );
#define	__Dt__DtFunc_WorkaroundBRN22997Alloc__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00FE );
#define	__Dt__DtFunc_WorkaroundBRN22997Alloc__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00FF );
#define	__Dt__DtFunc_WorkaroundBRN22997Alloc__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0100 );
#define	__Dt__DtFunc_WorkaroundBRN22997Alloc__DtStep_12	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0101 );
#define	__Dt__DtFunc_WorkaroundBRN22997Alloc__DtStep_13	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0102 );
#define	__Dt__DtFunc_WorkaroundBRN22997Alloc__DtStep_14	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0103 );
#define	__Dt__DtFunc_WorkaroundBRN22997Alloc__DtStep_15	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0104 );
#define	__Dt__DtFunc_WorkaroundBRN22997ReadHostPort__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0105 );
#define	__Dt__DtFunc_WorkaroundBRN22997ReadHostPort__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0106 );
#define	__Dt__DtFunc_WorkaroundBRN22997ReadHostPort__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0107 );
#define	__Dt__DtFunc_WorkaroundBRN22997ReadHostPort__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0108 );
#define	__Dt__DtFunc_WorkaroundBRN22997Free__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0109 );
#define	__Dt__DtFunc_WorkaroundBRN22997Free__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x010A );
#define	__Dt__DtFunc_WorkaroundBRN22997Free__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x010B );
#define	__Dt__DtFunc_WorkaroundBRN22997Free__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x010C );
#define	__Dt__DtFunc_WorkaroundBRN22997Free__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x010D );
#define	__Dt__DtFunc_WorkaroundBRN22997Free__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x010E );
#define	__Dt__DtFunc_WorkaroundBRN22997Free__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x010F );
#define	__Dt__DtFunc_MMU_MapExtSystemCacheRegs__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0110 );
#define	__Dt__DtFunc_MMU_MapExtSystemCacheRegs__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0111 );
#define	__Dt__DtFunc_MMU_MapExtSystemCacheRegs__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0112 );
#define	__Dt__DtFunc_MMU_MapExtSystemCacheRegs__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0113 );
#define	__Dt__DtFunc_MMU_MapExtSystemCacheRegs__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0114 );
#define	__Dt__DtFunc_MMU_MapExtSystemCacheRegs__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0115 );
#define	__Dt__DtFunc_MMU_MapExtSystemCacheRegs__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0116 );
#define	__Dt__DtFunc_MMU_MapExtSystemCacheRegs__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0117 );
#define	__Dt__DtFunc_MMU_MapExtSystemCacheRegs__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0118 );
#define	__Dt__DtFunc_MMU_MapExtSystemCacheRegs__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0119 );
#define	__Dt__DtFunc_MMU_MapExtSystemCacheRegs__DtStep_10	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x011A );
#define	__Dt__DtFunc_MMU_MapExtSystemCacheRegs__DtStep_11	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x011B );
#define	__Dt__DtFunc_MMU_UnmapExtSystemCacheRegs__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x011C );
#define	__Dt__DtFunc_MMU_UnmapExtSystemCacheRegs__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x011D );
#define	__Dt__DtFunc_MMU_UnmapExtSystemCacheRegs__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x011E );
#define	__Dt__DtFunc_MMU_UnmapExtSystemCacheRegs__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x011F );
#define	__Dt__DtFunc_MMU_UnmapExtSystemCacheRegs__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0120 );
#define	__Dt__DtFunc_MMU_UnmapExtSystemCacheRegs__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0121 );
#define	__Dt__DtFunc_PageTest__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0122 );
#define	__Dt__DtFunc_PageTest__DtStep_1	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0123 );
#define	__Dt__DtFunc_PageTest__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0124 );
#define	__Dt__DtFunc_PageTest__DtStep_3	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0125 );
#define	__Dt__DtFunc_PageTest__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0126 );
#define	__Dt__DtFunc_PageTest__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0127 );
#define	__Dt__DtFunc_PageTest__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0128 );
#define	__Dt__DtFunc_PageTest__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x012A );

#endif	/* __DT_mmu_h__ */

