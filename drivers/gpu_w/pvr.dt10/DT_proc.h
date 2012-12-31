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

#ifndef	__DT_proc_h__
#define	__DT_proc_h__

#ifdef	__cplusplus
	#define	__DtEXTERN	extern "C"
#else
	#define	__DtEXTERN	extern
#endif

/* TestPoint MacroCode -----------------------------------------------------*/
#define		__DtBaseAddress		0x1E0
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
#define		__DtFunc_printAppend		0
#define		__DtFunc_ProcSeq1ElementOff2Element		1
#define		__DtFunc_ProcSeq1ElementHeaderOff2Element		2
#define		__DtFunc_pvr_proc_open		3
#define		__DtFunc_pvr_proc_write		4
#define		__DtFunc_pvr_proc_seq_start		5
#define		__DtFunc_pvr_proc_seq_stop		6
#define		__DtFunc_pvr_proc_seq_next		7
#define		__DtFunc_pvr_proc_seq_show		8
#define		__DtFunc_CreateProcEntryInDirSeq		9
#define		__DtFunc_CreateProcReadEntrySeq		10
#define		__DtFunc_CreateProcEntrySeq		11
#define		__DtFunc_CreatePerProcessProcEntrySeq		12
#define		__DtFunc_RemoveProcEntrySeq		13
#define		__DtFunc_RemovePerProcessProcEntrySeq		14
#define		__DtFunc_pvr_read_proc		15
#define		__DtFunc_CreateProcEntryInDir		16
#define		__DtFunc_CreateProcEntry		17
#define		__DtFunc_CreatePerProcessProcEntry		18
#define		__DtFunc_CreateProcReadEntry		19
#define		__DtFunc_CreateProcEntries		20
#define		__DtFunc_RemoveProcEntry		21
#define		__DtFunc_RemovePerProcessProcEntry		22
#define		__DtFunc_RemovePerProcessProcDir		23
#define		__DtFunc_RemoveProcEntries		24
#define		__DtFunc_ProcSeqShowVersion		25
#define		__DtFunc_deviceTypeToString		26
#define		__DtFunc_deviceClassToString		27
#define		__DtFunc_DecOffPsDev_AnyVaCb		28
#define		__DtFunc_ProcSeqShowSysNodes		29
#define		__DtFunc_ProcSeqOff2ElementSysNodes		30

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

/* TestPoint DisableList ---------------------------------------------------*/
#define	__Dt__DtFunc_printAppend__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0000 );
#define	__Dt__DtFunc_printAppend__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0001 );
#define	__Dt__DtFunc_printAppend__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0002 );
#define	__Dt__DtFunc_printAppend__DtStep_3	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0003 );
#define	__Dt__DtFunc_printAppend__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0004 );
#define	__Dt__DtFunc_ProcSeq1ElementOff2Element__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0005 );
#define	__Dt__DtFunc_ProcSeq1ElementOff2Element__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0006 );
#define	__Dt__DtFunc_ProcSeq1ElementHeaderOff2Element__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0007 );
#define	__Dt__DtFunc_ProcSeq1ElementHeaderOff2Element__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0008 );
#define	__Dt__DtFunc_ProcSeq1ElementHeaderOff2Element__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0009 );
#define	__Dt__DtFunc_pvr_proc_open__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x000A );
#define	__Dt__DtFunc_pvr_proc_open__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000B );
#define	__Dt__DtFunc_pvr_proc_write__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x000C );
#define	__Dt__DtFunc_pvr_proc_write__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000D );
#define	__Dt__DtFunc_pvr_proc_seq_start__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x000E );
#define	__Dt__DtFunc_pvr_proc_seq_start__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000F );
#define	__Dt__DtFunc_pvr_proc_seq_stop__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0010 );
#define	__Dt__DtFunc_pvr_proc_seq_stop__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0011 );
#define	__Dt__DtFunc_pvr_proc_seq_next__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0012 );
#define	__Dt__DtFunc_pvr_proc_seq_next__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0013 );
#define	__Dt__DtFunc_pvr_proc_seq_show__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0014 );
#define	__Dt__DtFunc_pvr_proc_seq_show__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0015 );
#define	__Dt__DtFunc_CreateProcEntryInDirSeq__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0016 );
#define	__Dt__DtFunc_CreateProcEntryInDirSeq__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0017 );
#define	__Dt__DtFunc_CreateProcEntryInDirSeq__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0018 );
#define	__Dt__DtFunc_CreateProcEntryInDirSeq__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0019 );
#define	__Dt__DtFunc_CreateProcEntryInDirSeq__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001A );
#define	__Dt__DtFunc_CreateProcEntryInDirSeq__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001B );
#define	__Dt__DtFunc_CreateProcEntryInDirSeq__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001C );
#define	__Dt__DtFunc_CreateProcEntryInDirSeq__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001D );
#define	__Dt__DtFunc_CreateProcReadEntrySeq__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001E );
#define	__Dt__DtFunc_CreateProcEntrySeq__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001F );
#define	__Dt__DtFunc_CreatePerProcessProcEntrySeq__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0020 );
#define	__Dt__DtFunc_CreatePerProcessProcEntrySeq__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0021 );
#define	__Dt__DtFunc_CreatePerProcessProcEntrySeq__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0022 );
#define	__Dt__DtFunc_CreatePerProcessProcEntrySeq__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0023 );
#define	__Dt__DtFunc_CreatePerProcessProcEntrySeq__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0024 );
#define	__Dt__DtFunc_CreatePerProcessProcEntrySeq__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0025 );
#define	__Dt__DtFunc_CreatePerProcessProcEntrySeq__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0026 );
#define	__Dt__DtFunc_CreatePerProcessProcEntrySeq__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0027 );
#define	__Dt__DtFunc_CreatePerProcessProcEntrySeq__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0028 );
#define	__Dt__DtFunc_CreatePerProcessProcEntrySeq__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002A );
#define	__Dt__DtFunc_CreatePerProcessProcEntrySeq__DtStep_10	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002B );
#define	__Dt__DtFunc_CreatePerProcessProcEntrySeq__DtStep_11	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002C );
#define	__Dt__DtFunc_RemoveProcEntrySeq__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x002D );
#define	__Dt__DtFunc_RemoveProcEntrySeq__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002E );
#define	__Dt__DtFunc_RemoveProcEntrySeq__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002F );
#define	__Dt__DtFunc_RemovePerProcessProcEntrySeq__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0031 );
#define	__Dt__DtFunc_RemovePerProcessProcEntrySeq__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0032 );
#define	__Dt__DtFunc_RemovePerProcessProcEntrySeq__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0033 );
#define	__Dt__DtFunc_RemovePerProcessProcEntrySeq__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0034 );
#define	__Dt__DtFunc_RemovePerProcessProcEntrySeq__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0035 );
#define	__Dt__DtFunc_RemovePerProcessProcEntrySeq__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0036 );
#define	__Dt__DtFunc_pvr_read_proc__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0037 );
#define	__Dt__DtFunc_pvr_read_proc__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0038 );
#define	__Dt__DtFunc_pvr_read_proc__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0039 );
#define	__Dt__DtFunc_pvr_read_proc__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003A );
#define	__Dt__DtFunc_pvr_read_proc__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003B );
#define	__Dt__DtFunc_CreateProcEntryInDir__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x003C );
#define	__Dt__DtFunc_CreateProcEntryInDir__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003D );
#define	__Dt__DtFunc_CreateProcEntryInDir__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003E );
#define	__Dt__DtFunc_CreateProcEntryInDir__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003F );
#define	__Dt__DtFunc_CreateProcEntryInDir__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0040 );
#define	__Dt__DtFunc_CreateProcEntryInDir__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0041 );
#define	__Dt__DtFunc_CreateProcEntryInDir__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0042 );
#define	__Dt__DtFunc_CreateProcEntry__DtStep_0	/*FuncIn+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0043 );
#define	__Dt__DtFunc_CreatePerProcessProcEntry__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0044 );
#define	__Dt__DtFunc_CreatePerProcessProcEntry__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0045 );
#define	__Dt__DtFunc_CreatePerProcessProcEntry__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0046 );
#define	__Dt__DtFunc_CreatePerProcessProcEntry__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0047 );
#define	__Dt__DtFunc_CreatePerProcessProcEntry__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0048 );
#define	__Dt__DtFunc_CreatePerProcessProcEntry__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0049 );
#define	__Dt__DtFunc_CreatePerProcessProcEntry__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004A );
#define	__Dt__DtFunc_CreatePerProcessProcEntry__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004B );
#define	__Dt__DtFunc_CreatePerProcessProcEntry__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004C );
#define	__Dt__DtFunc_CreatePerProcessProcEntry__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004D );
#define	__Dt__DtFunc_CreatePerProcessProcEntry__DtStep_10	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004E );
#define	__Dt__DtFunc_CreatePerProcessProcEntry__DtStep_11	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004F );
#define	__Dt__DtFunc_CreateProcReadEntry__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0050 );
#define	__Dt__DtFunc_CreateProcReadEntry__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0051 );
#define	__Dt__DtFunc_CreateProcReadEntry__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0052 );
#define	__Dt__DtFunc_CreateProcReadEntry__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0053 );
#define	__Dt__DtFunc_CreateProcEntries__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0054 );
#define	__Dt__DtFunc_CreateProcEntries__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0055 );
#define	__Dt__DtFunc_CreateProcEntries__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0056 );
#define	__Dt__DtFunc_CreateProcEntries__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0057 );
#define	__Dt__DtFunc_CreateProcEntries__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0058 );
#define	__Dt__DtFunc_CreateProcEntries__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0059 );
#define	__Dt__DtFunc_CreateProcEntries__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005A );
#define	__Dt__DtFunc_CreateProcEntries__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005B );
#define	__Dt__DtFunc_CreateProcEntries__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005C );
#define	__Dt__DtFunc_RemoveProcEntry__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x005D );
#define	__Dt__DtFunc_RemoveProcEntry__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005E );
#define	__Dt__DtFunc_RemoveProcEntry__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005F );
#define	__Dt__DtFunc_RemovePerProcessProcEntry__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0060 );
#define	__Dt__DtFunc_RemovePerProcessProcEntry__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0061 );
#define	__Dt__DtFunc_RemovePerProcessProcEntry__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0062 );
#define	__Dt__DtFunc_RemovePerProcessProcEntry__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0063 );
#define	__Dt__DtFunc_RemovePerProcessProcEntry__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0064 );
#define	__Dt__DtFunc_RemovePerProcessProcEntry__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0065 );
#define	__Dt__DtFunc_RemovePerProcessProcDir__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0066 );
#define	__Dt__DtFunc_RemovePerProcessProcDir__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0067 );
#define	__Dt__DtFunc_RemovePerProcessProcDir__DtStep_2	/*while*/	_TP_BusOut( __DtBaseAddress, 0x0068 );
#define	__Dt__DtFunc_RemovePerProcessProcDir__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0069 );
#define	__Dt__DtFunc_RemoveProcEntries__DtStep_0	/*while*/	_TP_BusOut( __DtBaseAddress, 0x006A );
#define	__Dt__DtFunc_RemoveProcEntries__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006B );
#define	__Dt__DtFunc_ProcSeqShowVersion__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x006C );
#define	__Dt__DtFunc_ProcSeqShowVersion__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006D );
#define	__Dt__DtFunc_ProcSeqShowVersion__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006E );
#define	__Dt__DtFunc_ProcSeqShowVersion__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006F );
#define	__Dt__DtFunc_ProcSeqShowVersion__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0070 );
#define	__Dt__DtFunc_deviceTypeToString__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0071 );
#define	__Dt__DtFunc_deviceTypeToString__DtStep_1	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0072 );
#define	__Dt__DtFunc_deviceTypeToString__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0073 );
#define	__Dt__DtFunc_deviceTypeToString__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0074 );
#define	__Dt__DtFunc_deviceClassToString__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0075 );
#define	__Dt__DtFunc_deviceClassToString__DtStep_1	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0076 );
#define	__Dt__DtFunc_deviceClassToString__DtStep_2	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0077 );
#define	__Dt__DtFunc_deviceClassToString__DtStep_3	/*switch+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0078 );
#define	__Dt__DtFunc_deviceClassToString__DtStep_4	/*switch*/	_TP_BusOut( __DtBaseAddress, 0x0079 );
#define	__Dt__DtFunc_deviceClassToString__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007A );
#define	__Dt__DtFunc_deviceClassToString__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007B );
#define	__Dt__DtFunc_DecOffPsDev_AnyVaCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x007C );
#define	__Dt__DtFunc_DecOffPsDev_AnyVaCb__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007D );
#define	__Dt__DtFunc_DecOffPsDev_AnyVaCb__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007E );
#define	__Dt__DtFunc_DecOffPsDev_AnyVaCb__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007F );
#define	__Dt__DtFunc_ProcSeqShowSysNodes__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0081 );
#define	__Dt__DtFunc_ProcSeqShowSysNodes__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0082 );
#define	__Dt__DtFunc_ProcSeqShowSysNodes__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0083 );
#define	__Dt__DtFunc_ProcSeqShowSysNodes__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0084 );
#define	__Dt__DtFunc_ProcSeqOff2ElementSysNodes__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0085 );
#define	__Dt__DtFunc_ProcSeqOff2ElementSysNodes__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0086 );
#define	__Dt__DtFunc_ProcSeqOff2ElementSysNodes__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0087 );
#define	__Dt__DtFunc_ProcSeqOff2ElementSysNodes__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0088 );

#endif	/* __DT_proc_h__ */

