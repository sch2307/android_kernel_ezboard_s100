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

#ifndef	__DT_pdump_common_h__
#define	__DT_pdump_common_h__

#ifdef	__cplusplus
	#define	__DtEXTERN	extern "C"
#else
	#define	__DtEXTERN	extern
#endif

/* TestPoint MacroCode -----------------------------------------------------*/
#define		__DtBaseAddress		0x1B0
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
#define		__DtFunc__PDumpIsPersistent		0
#define		__DtFunc__PDumpIsProcessActive		1
#define		__DtFunc__PDumpGetPID		2
#define		__DtFunc_GetTempBuffer		3
#define		__DtFunc_FreeTempBuffer		4
#define		__DtFunc_PDumpInitCommon		5
#define		__DtFunc_PDumpDeInitCommon		6
#define		__DtFunc_PDumpIsSuspended		7
#define		__DtFunc_PDumpIsCaptureFrameKM		8
#define		__DtFunc_PDumpSetFrameKM		9
#define		__DtFunc_PDumpRegWithFlagsKM		10
#define		__DtFunc_PDumpRegKM		11
#define		__DtFunc_PDumpRegPolWithFlagsKM		12
#define		__DtFunc_PDumpRegPolKM		13
#define		__DtFunc_PDumpMallocPages		14
#define		__DtFunc_PDumpMallocPageTable		15
#define		__DtFunc_PDumpFreePages		16
#define		__DtFunc_PDumpFreePageTable		17
#define		__DtFunc_PDumpPDRegWithFlags		18
#define		__DtFunc_PDumpPDReg		19
#define		__DtFunc_PDumpMemPolKM		20
#define		__DtFunc_PDumpMemKM		21
#define		__DtFunc_PDumpMemPDEntriesKM		22
#define		__DtFunc_PDumpMemPTEntriesKM		23
#define		__DtFunc_PDumpPDDevPAddrKM		24
#define		__DtFunc_PDumpCommentKM		25
#define		__DtFunc_PDumpCommentWithFlags		26
#define		__DtFunc_PDumpComment		27
#define		__DtFunc_PDumpDriverInfoKM		28
#define		__DtFunc_PDumpBitmapKM		29
#define		__DtFunc_PDumpReadRegKM		30
#define		__DtFunc_PDumpTestNextFrame		31
#define		__DtFunc_PDumpSignatureRegister		32
#define		__DtFunc_PDumpRegisterRange		33
#define		__DtFunc_PDump3DSignatureRegisters		34
#define		__DtFunc_PDumpTASignatureRegisters		35
#define		__DtFunc_PDumpCounterRegisters		36
#define		__DtFunc_PDumpRegRead		37
#define		__DtFunc_PDumpSaveMemKM		38
#define		__DtFunc_PDumpCycleCountRegRead		39
#define		__DtFunc_PDumpSignatureBuffer		40
#define		__DtFunc_PDumpHWPerfCBKM		41
#define		__DtFunc_PDumpCBP		42
#define		__DtFunc_PDumpIDLWithFlags		43
#define		__DtFunc_PDumpIDL		44
#define		__DtFunc_PDumpMemUM		45
#define		__DtFunc__PdumpAllocMMUContext		46
#define		__DtFunc__PdumpFreeMMUContext		47
#define		__DtFunc_PDumpSetMMUContext		48
#define		__DtFunc_PDumpClearMMUContext		49
#define		__DtFunc_PDumpStoreMemToFile		50
#define		__DtFunc_PDumpRegBasedCBP		51
#define		__DtFunc_PDumpConnectionNotify		52
#define		__DtFunc_DbgWrite		53

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

/* TestPoint DisableList ---------------------------------------------------*/
#define	__Dt__DtFunc__PDumpIsPersistent__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0000 );
#define	__Dt__DtFunc__PDumpIsPersistent__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0001 );
#define	__Dt__DtFunc__PDumpIsPersistent__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0002 );
#define	__Dt__DtFunc__PDumpIsProcessActive__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0003 );
#define	__Dt__DtFunc__PDumpIsProcessActive__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0004 );
#define	__Dt__DtFunc__PDumpIsProcessActive__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0005 );
#define	__Dt__DtFunc__PDumpGetPID__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0006 );
#define	__Dt__DtFunc__PDumpGetPID__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0007 );
#define	__Dt__DtFunc__PDumpGetPID__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0008 );
#define	__Dt__DtFunc_GetTempBuffer__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0009 );
#define	__Dt__DtFunc_GetTempBuffer__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000A );
#define	__Dt__DtFunc_GetTempBuffer__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000B );
#define	__Dt__DtFunc_GetTempBuffer__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000C );
#define	__Dt__DtFunc_FreeTempBuffer__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x000D );
#define	__Dt__DtFunc_FreeTempBuffer__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000E );
#define	__Dt__DtFunc_FreeTempBuffer__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000F );
#define	__Dt__DtFunc_FreeTempBuffer__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0010 );
#define	__Dt__DtFunc_FreeTempBuffer__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0011 );
#define	__Dt__DtFunc_PDumpInitCommon__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0012 );
#define	__Dt__DtFunc_PDumpInitCommon__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0013 );
#define	__Dt__DtFunc_PDumpDeInitCommon__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0014 );
#define	__Dt__DtFunc_PDumpDeInitCommon__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0015 );
#define	__Dt__DtFunc_PDumpIsSuspended__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0016 );
#define	__Dt__DtFunc_PDumpIsCaptureFrameKM__DtStep_0	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0017 );
#define	__Dt__DtFunc_PDumpIsCaptureFrameKM__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0018 );
#define	__Dt__DtFunc_PDumpSetFrameKM__DtStep_0	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0019 );
#define	__Dt__DtFunc_PDumpSetFrameKM__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001A );
#define	__Dt__DtFunc_PDumpRegWithFlagsKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x001B );
#define	__Dt__DtFunc_PDumpRegWithFlagsKM__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001C );
#define	__Dt__DtFunc_PDumpRegWithFlagsKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001D );
#define	__Dt__DtFunc_PDumpRegKM__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001E );
#define	__Dt__DtFunc_PDumpRegPolWithFlagsKM__DtStep_0	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001F );
#define	__Dt__DtFunc_PDumpRegPolWithFlagsKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0020 );
#define	__Dt__DtFunc_PDumpRegPolWithFlagsKM__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0021 );
#define	__Dt__DtFunc_PDumpRegPolWithFlagsKM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0022 );
#define	__Dt__DtFunc_PDumpRegPolKM__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0023 );
#define	__Dt__DtFunc_PDumpMallocPages__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0024 );
#define	__Dt__DtFunc_PDumpMallocPages__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0025 );
#define	__Dt__DtFunc_PDumpMallocPages__DtStep_2	/*while*/	_TP_BusOut( __DtBaseAddress, 0x0026 );
#define	__Dt__DtFunc_PDumpMallocPages__DtStep_3	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0027 );
#define	__Dt__DtFunc_PDumpMallocPages__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0028 );
#define	__Dt__DtFunc_PDumpMallocPageTable__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x002A );
#define	__Dt__DtFunc_PDumpMallocPageTable__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002B );
#define	__Dt__DtFunc_PDumpMallocPageTable__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002C );
#define	__Dt__DtFunc_PDumpMallocPageTable__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002D );
#define	__Dt__DtFunc_PDumpFreePages__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x002E );
#define	__Dt__DtFunc_PDumpFreePages__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002F );
#define	__Dt__DtFunc_PDumpFreePages__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0031 );
#define	__Dt__DtFunc_PDumpFreePages__DtStep_3	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0032 );
#define	__Dt__DtFunc_PDumpFreePages__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0033 );
#define	__Dt__DtFunc_PDumpFreePages__DtStep_5	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0034 );
#define	__Dt__DtFunc_PDumpFreePages__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0035 );
#define	__Dt__DtFunc_PDumpFreePages__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0036 );
#define	__Dt__DtFunc_PDumpFreePageTable__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0037 );
#define	__Dt__DtFunc_PDumpFreePageTable__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0038 );
#define	__Dt__DtFunc_PDumpFreePageTable__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0039 );
#define	__Dt__DtFunc_PDumpFreePageTable__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003A );
#define	__Dt__DtFunc_PDumpPDRegWithFlags__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x003B );
#define	__Dt__DtFunc_PDumpPDRegWithFlags__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003C );
#define	__Dt__DtFunc_PDumpPDRegWithFlags__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003D );
#define	__Dt__DtFunc_PDumpPDRegWithFlags__DtStep_3	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003E );
#define	__Dt__DtFunc_PDumpPDRegWithFlags__DtStep_4	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003F );
#define	__Dt__DtFunc_PDumpPDRegWithFlags__DtStep_5	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0040 );
#define	__Dt__DtFunc_PDumpPDRegWithFlags__DtStep_6	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0041 );
#define	__Dt__DtFunc_PDumpPDReg__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0042 );
#define	__Dt__DtFunc_PDumpMemPolKM__DtStep_0	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0043 );
#define	__Dt__DtFunc_PDumpMemPolKM__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0044 );
#define	__Dt__DtFunc_PDumpMemPolKM__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0045 );
#define	__Dt__DtFunc_PDumpMemPolKM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0046 );
#define	__Dt__DtFunc_PDumpMemKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0047 );
#define	__Dt__DtFunc_PDumpMemKM__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0048 );
#define	__Dt__DtFunc_PDumpMemKM__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0049 );
#define	__Dt__DtFunc_PDumpMemKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004A );
#define	__Dt__DtFunc_PDumpMemKM__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004B );
#define	__Dt__DtFunc_PDumpMemKM__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004C );
#define	__Dt__DtFunc_PDumpMemKM__DtStep_6	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004D );
#define	__Dt__DtFunc_PDumpMemKM__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004E );
#define	__Dt__DtFunc_PDumpMemKM__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004F );
#define	__Dt__DtFunc_PDumpMemKM__DtStep_9	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0050 );
#define	__Dt__DtFunc_PDumpMemKM__DtStep_10	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0051 );
#define	__Dt__DtFunc_PDumpMemKM__DtStep_11	/*while*/	_TP_BusOut( __DtBaseAddress, 0x0052 );
#define	__Dt__DtFunc_PDumpMemKM__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0053 );
#define	__Dt__DtFunc_PDumpMemKM__DtStep_13	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0054 );
#define	__Dt__DtFunc_PDumpMemKM__DtStep_14	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0055 );
#define	__Dt__DtFunc_PDumpMemKM__DtStep_15	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0056 );
#define	__Dt__DtFunc_PDumpMemKM__DtStep_16	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0057 );
#define	__Dt__DtFunc_PDumpMemPDEntriesKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0058 );
#define	__Dt__DtFunc_PDumpMemPDEntriesKM__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0059 );
#define	__Dt__DtFunc_PDumpMemPTEntriesKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x005A );
#define	__Dt__DtFunc_PDumpMemPTEntriesKM__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005B );
#define	__Dt__DtFunc_PDumpMemPTEntriesKM__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005C );
#define	__Dt__DtFunc_PDumpMemPTEntriesKM__DtStep_3	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005D );
#define	__Dt__DtFunc_PDumpMemPTEntriesKM__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005E );
#define	__Dt__DtFunc_PDumpMemPTEntriesKM__DtStep_5	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005F );
#define	__Dt__DtFunc_PDumpMemPTEntriesKM__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0060 );
#define	__Dt__DtFunc_PDumpMemPTEntriesKM__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0061 );
#define	__Dt__DtFunc_PDumpMemPTEntriesKM__DtStep_8	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0062 );
#define	__Dt__DtFunc_PDumpMemPTEntriesKM__DtStep_9	/*while*/	_TP_BusOut( __DtBaseAddress, 0x0063 );
#define	__Dt__DtFunc_PDumpMemPTEntriesKM__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0064 );
#define	__Dt__DtFunc_PDumpMemPTEntriesKM__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0065 );
#define	__Dt__DtFunc_PDumpMemPTEntriesKM__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0066 );
#define	__Dt__DtFunc_PDumpMemPTEntriesKM__DtStep_13	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0067 );
#define	__Dt__DtFunc_PDumpMemPTEntriesKM__DtStep_14	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0068 );
#define	__Dt__DtFunc_PDumpMemPTEntriesKM__DtStep_15	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0069 );
#define	__Dt__DtFunc_PDumpMemPTEntriesKM__DtStep_16	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006A );
#define	__Dt__DtFunc_PDumpMemPTEntriesKM__DtStep_17	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006B );
#define	__Dt__DtFunc_PDumpMemPTEntriesKM__DtStep_18	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006C );
#define	__Dt__DtFunc_PDumpMemPTEntriesKM__DtStep_19	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006D );
#define	__Dt__DtFunc_PDumpMemPTEntriesKM__DtStep_20	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006E );
#define	__Dt__DtFunc_PDumpMemPTEntriesKM__DtStep_21	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006F );
#define	__Dt__DtFunc_PDumpMemPTEntriesKM__DtStep_22	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0070 );
#define	__Dt__DtFunc_PDumpMemPTEntriesKM__DtStep_23	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0071 );
#define	__Dt__DtFunc_PDumpPDDevPAddrKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0072 );
#define	__Dt__DtFunc_PDumpPDDevPAddrKM__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0073 );
#define	__Dt__DtFunc_PDumpPDDevPAddrKM__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0074 );
#define	__Dt__DtFunc_PDumpPDDevPAddrKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0075 );
#define	__Dt__DtFunc_PDumpPDDevPAddrKM__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0076 );
#define	__Dt__DtFunc_PDumpPDDevPAddrKM__DtStep_5	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0077 );
#define	__Dt__DtFunc_PDumpPDDevPAddrKM__DtStep_6	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0078 );
#define	__Dt__DtFunc_PDumpPDDevPAddrKM__DtStep_7	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0079 );
#define	__Dt__DtFunc_PDumpPDDevPAddrKM__DtStep_8	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007A );
#define	__Dt__DtFunc_PDumpPDDevPAddrKM__DtStep_9	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007B );
#define	__Dt__DtFunc_PDumpPDDevPAddrKM__DtStep_10	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007C );
#define	__Dt__DtFunc_PDumpPDDevPAddrKM__DtStep_11	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007D );
#define	__Dt__DtFunc_PDumpPDDevPAddrKM__DtStep_12	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007E );
#define	__Dt__DtFunc_PDumpPDDevPAddrKM__DtStep_13	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007F );
#define	__Dt__DtFunc_PDumpPDDevPAddrKM__DtStep_14	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0081 );
#define	__Dt__DtFunc_PDumpPDDevPAddrKM__DtStep_15	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0082 );
#define	__Dt__DtFunc_PDumpCommentKM__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0083 );
#define	__Dt__DtFunc_PDumpCommentKM__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0084 );
#define	__Dt__DtFunc_PDumpCommentKM__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0085 );
#define	__Dt__DtFunc_PDumpCommentKM__DtStep_3	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0086 );
#define	__Dt__DtFunc_PDumpCommentKM__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0087 );
#define	__Dt__DtFunc_PDumpCommentWithFlags__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0088 );
#define	__Dt__DtFunc_PDumpCommentWithFlags__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0089 );
#define	__Dt__DtFunc_PDumpCommentWithFlags__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x008A );
#define	__Dt__DtFunc_PDumpComment__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x008B );
#define	__Dt__DtFunc_PDumpComment__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x008C );
#define	__Dt__DtFunc_PDumpComment__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x008D );
#define	__Dt__DtFunc_PDumpDriverInfoKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x008E );
#define	__Dt__DtFunc_PDumpDriverInfoKM__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x008F );
#define	__Dt__DtFunc_PDumpDriverInfoKM__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0091 );
#define	__Dt__DtFunc_PDumpDriverInfoKM__DtStep_3	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0092 );
#define	__Dt__DtFunc_PDumpDriverInfoKM__DtStep_4	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0093 );
#define	__Dt__DtFunc_PDumpDriverInfoKM__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0094 );
#define	__Dt__DtFunc_PDumpBitmapKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0095 );
#define	__Dt__DtFunc_PDumpBitmapKM__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0096 );
#define	__Dt__DtFunc_PDumpBitmapKM__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0097 );
#define	__Dt__DtFunc_PDumpBitmapKM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0098 );
#define	__Dt__DtFunc_PDumpReadRegKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0099 );
#define	__Dt__DtFunc_PDumpReadRegKM__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x009A );
#define	__Dt__DtFunc_PDumpReadRegKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x009B );
#define	__Dt__DtFunc_PDumpTestNextFrame__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x009C );
#define	__Dt__DtFunc_PDumpTestNextFrame__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x009D );
#define	__Dt__DtFunc_PDumpSignatureRegister__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x009E );
#define	__Dt__DtFunc_PDumpSignatureRegister__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x009F );
#define	__Dt__DtFunc_PDumpSignatureRegister__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A1 );
#define	__Dt__DtFunc_PDumpRegisterRange__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00A2 );
#define	__Dt__DtFunc_PDumpRegisterRange__DtStep_1	/*for*/	_TP_BusOut( __DtBaseAddress, 0x00A3 );
#define	__Dt__DtFunc_PDumpRegisterRange__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A4 );
#define	__Dt__DtFunc_PDump3DSignatureRegisters__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00A5 );
#define	__Dt__DtFunc_PDump3DSignatureRegisters__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A6 );
#define	__Dt__DtFunc_PDump3DSignatureRegisters__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A7 );
#define	__Dt__DtFunc_PDumpTASignatureRegisters__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00A8 );
#define	__Dt__DtFunc_PDumpTASignatureRegisters__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A9 );
#define	__Dt__DtFunc_PDumpTASignatureRegisters__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00AA );
#define	__Dt__DtFunc_PDumpCounterRegisters__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00AB );
#define	__Dt__DtFunc_PDumpCounterRegisters__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00AC );
#define	__Dt__DtFunc_PDumpCounterRegisters__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00AD );
#define	__Dt__DtFunc_PDumpRegRead__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00AE );
#define	__Dt__DtFunc_PDumpRegRead__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00AF );
#define	__Dt__DtFunc_PDumpRegRead__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B1 );
#define	__Dt__DtFunc_PDumpSaveMemKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00B2 );
#define	__Dt__DtFunc_PDumpSaveMemKM__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B3 );
#define	__Dt__DtFunc_PDumpSaveMemKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B4 );
#define	__Dt__DtFunc_PDumpCycleCountRegRead__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00B5 );
#define	__Dt__DtFunc_PDumpCycleCountRegRead__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B6 );
#define	__Dt__DtFunc_PDumpCycleCountRegRead__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B7 );
#define	__Dt__DtFunc_PDumpSignatureBuffer__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00B8 );
#define	__Dt__DtFunc_PDumpSignatureBuffer__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B9 );
#define	__Dt__DtFunc_PDumpHWPerfCBKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00BA );
#define	__Dt__DtFunc_PDumpHWPerfCBKM__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BB );
#define	__Dt__DtFunc_PDumpCBP__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00BC );
#define	__Dt__DtFunc_PDumpCBP__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BD );
#define	__Dt__DtFunc_PDumpCBP__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BE );
#define	__Dt__DtFunc_PDumpIDLWithFlags__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00BF );
#define	__Dt__DtFunc_PDumpIDLWithFlags__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C0 );
#define	__Dt__DtFunc_PDumpIDLWithFlags__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C1 );
#define	__Dt__DtFunc_PDumpIDL__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C2 );
#define	__Dt__DtFunc_PDumpMemUM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00C3 );
#define	__Dt__DtFunc_PDumpMemUM__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C4 );
#define	__Dt__DtFunc_PDumpMemUM__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C5 );
#define	__Dt__DtFunc_PDumpMemUM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C6 );
#define	__Dt__DtFunc_PDumpMemUM__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C7 );
#define	__Dt__DtFunc_PDumpMemUM__DtStep_5	/*for*/	_TP_BusOut( __DtBaseAddress, 0x00C8 );
#define	__Dt__DtFunc_PDumpMemUM__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C9 );
#define	__Dt__DtFunc_PDumpMemUM__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CA );
#define	__Dt__DtFunc_PDumpMemUM__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00CB );
#define	__Dt__DtFunc_PDumpMemUM__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00CC );
#define	__Dt__DtFunc_PDumpMemUM__DtStep_10	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CD );
#define	__Dt__DtFunc_PDumpMemUM__DtStep_11	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CE );
#define	__Dt__DtFunc__PdumpAllocMMUContext__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00CF );
#define	__Dt__DtFunc__PdumpAllocMMUContext__DtStep_1	/*for*/	_TP_BusOut( __DtBaseAddress, 0x00D0 );
#define	__Dt__DtFunc__PdumpAllocMMUContext__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D1 );
#define	__Dt__DtFunc__PdumpAllocMMUContext__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D2 );
#define	__Dt__DtFunc__PdumpAllocMMUContext__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D3 );
#define	__Dt__DtFunc__PdumpFreeMMUContext__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00D4 );
#define	__Dt__DtFunc__PdumpFreeMMUContext__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D5 );
#define	__Dt__DtFunc__PdumpFreeMMUContext__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D6 );
#define	__Dt__DtFunc__PdumpFreeMMUContext__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D7 );
#define	__Dt__DtFunc_PDumpSetMMUContext__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00D8 );
#define	__Dt__DtFunc_PDumpSetMMUContext__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D9 );
#define	__Dt__DtFunc_PDumpSetMMUContext__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DA );
#define	__Dt__DtFunc_PDumpSetMMUContext__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DB );
#define	__Dt__DtFunc_PDumpClearMMUContext__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00DC );
#define	__Dt__DtFunc_PDumpClearMMUContext__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00DD );
#define	__Dt__DtFunc_PDumpClearMMUContext__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DE );
#define	__Dt__DtFunc_PDumpClearMMUContext__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DF );
#define	__Dt__DtFunc_PDumpStoreMemToFile__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00E0 );
#define	__Dt__DtFunc_PDumpStoreMemToFile__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E1 );
#define	__Dt__DtFunc_PDumpRegBasedCBP__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00E2 );
#define	__Dt__DtFunc_PDumpRegBasedCBP__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E3 );
#define	__Dt__DtFunc_PDumpConnectionNotify__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00E4 );
#define	__Dt__DtFunc_PDumpConnectionNotify__DtStep_1	/*while*/	_TP_BusOut( __DtBaseAddress, 0x00E5 );
#define	__Dt__DtFunc_PDumpConnectionNotify__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E6 );
#define	__Dt__DtFunc_PDumpConnectionNotify__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E7 );
#define	__Dt__DtFunc_DbgWrite__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00E8 );
#define	__Dt__DtFunc_DbgWrite__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E9 );
#define	__Dt__DtFunc_DbgWrite__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00EA );
#define	__Dt__DtFunc_DbgWrite__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00EB );
#define	__Dt__DtFunc_DbgWrite__DtStep_4	/*while*/	_TP_BusOut( __DtBaseAddress, 0x00EC );
#define	__Dt__DtFunc_DbgWrite__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00ED );
#define	__Dt__DtFunc_DbgWrite__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00EE );
#define	__Dt__DtFunc_DbgWrite__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00EF );
#define	__Dt__DtFunc_DbgWrite__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F1 );
#define	__Dt__DtFunc_DbgWrite__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F2 );
#define	__Dt__DtFunc_DbgWrite__DtStep_10	/*while*/	_TP_BusOut( __DtBaseAddress, 0x00F3 );
#define	__Dt__DtFunc_DbgWrite__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F4 );
#define	__Dt__DtFunc_DbgWrite__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F5 );
#define	__Dt__DtFunc_DbgWrite__DtStep_13	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F6 );
#define	__Dt__DtFunc_DbgWrite__DtStep_14	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F7 );
#define	__Dt__DtFunc_DbgWrite__DtStep_15	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F8 );
#define	__Dt__DtFunc_DbgWrite__DtStep_16	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F9 );
#define	__Dt__DtFunc_DbgWrite__DtStep_17	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00FA );
#define	__Dt__DtFunc_DbgWrite__DtStep_18	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00FB );
#define	__Dt__DtFunc_DbgWrite__DtStep_19	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00FC );
#define	__Dt__DtFunc_DbgWrite__DtStep_20	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00FD );

#endif	/* __DT_pdump_common_h__ */

