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

#ifndef	__DT_queue_h__
#define	__DT_queue_h__

#ifdef	__cplusplus
	#define	__DtEXTERN	extern "C"
#else
	#define	__DtEXTERN	extern
#endif

/* TestPoint MacroCode -----------------------------------------------------*/
#define		__DtBaseAddress		0x220
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
#define		__DtFunc_ProcSeqShowQueue		0
#define		__DtFunc_ProcSeqOff2ElementQueue		1
#define		__DtFunc_QueueDumpCmdComplete		2
#define		__DtFunc_QueueDumpDebugInfo_ForEachCb		3
#define		__DtFunc_QueueDumpDebugInfo		4
#define		__DtFunc_NearestPower2		5
#define		__DtFunc_PVRSRVCreateCommandQueueKM		6
#define		__DtFunc_PVRSRVDestroyCommandQueueKM		7
#define		__DtFunc_PVRSRVGetQueueSpaceKM		8
#define		__DtFunc_PVRSRVInsertCommandKM		9
#define		__DtFunc_PVRSRVSubmitCommandKM		10
#define		__DtFunc_PVRSRVProcessCommand		11
#define		__DtFunc_PVRSRVProcessQueues_ForEachCb		12
#define		__DtFunc_PVRSRVProcessQueues		13
#define		__DtFunc_PVRSRVFreeCommandCompletePacketKM		14
#define		__DtFunc_PVRSRVCommandCompleteKM		15
#define		__DtFunc_PVRSRVRegisterCmdProcListKM		16
#define		__DtFunc_PVRSRVRemoveCmdProcListKM		17

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

/* TestPoint DisableList ---------------------------------------------------*/
#define	__Dt__DtFunc_ProcSeqShowQueue__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0000 );
#define	__Dt__DtFunc_ProcSeqShowQueue__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0001 );
#define	__Dt__DtFunc_ProcSeqShowQueue__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0002 );
#define	__Dt__DtFunc_ProcSeqShowQueue__DtStep_3	/*while*/	_TP_BusOut( __DtBaseAddress, 0x0003 );
#define	__Dt__DtFunc_ProcSeqShowQueue__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0004 );
#define	__Dt__DtFunc_ProcSeqShowQueue__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0005 );
#define	__Dt__DtFunc_ProcSeqOff2ElementQueue__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0006 );
#define	__Dt__DtFunc_ProcSeqOff2ElementQueue__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0007 );
#define	__Dt__DtFunc_ProcSeqOff2ElementQueue__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0008 );
#define	__Dt__DtFunc_ProcSeqOff2ElementQueue__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0009 );
#define	__Dt__DtFunc_QueueDumpCmdComplete__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x000A );
#define	__Dt__DtFunc_QueueDumpCmdComplete__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000B );
#define	__Dt__DtFunc_QueueDumpCmdComplete__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000C );
#define	__Dt__DtFunc_QueueDumpCmdComplete__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000D );
#define	__Dt__DtFunc_QueueDumpDebugInfo_ForEachCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x000E );
#define	__Dt__DtFunc_QueueDumpDebugInfo_ForEachCb__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000F );
#define	__Dt__DtFunc_QueueDumpDebugInfo_ForEachCb__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0010 );
#define	__Dt__DtFunc_QueueDumpDebugInfo_ForEachCb__DtStep_3	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0011 );
#define	__Dt__DtFunc_QueueDumpDebugInfo_ForEachCb__DtStep_4	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0012 );
#define	__Dt__DtFunc_QueueDumpDebugInfo_ForEachCb__DtStep_5	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0013 );
#define	__Dt__DtFunc_QueueDumpDebugInfo_ForEachCb__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0014 );
#define	__Dt__DtFunc_QueueDumpDebugInfo_ForEachCb__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0015 );
#define	__Dt__DtFunc_QueueDumpDebugInfo__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0016 );
#define	__Dt__DtFunc_QueueDumpDebugInfo__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0017 );
#define	__Dt__DtFunc_NearestPower2__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0018 );
#define	__Dt__DtFunc_NearestPower2__DtStep_1	/*while*/	_TP_BusOut( __DtBaseAddress, 0x0019 );
#define	__Dt__DtFunc_NearestPower2__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001A );
#define	__Dt__DtFunc_PVRSRVCreateCommandQueueKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x001B );
#define	__Dt__DtFunc_PVRSRVCreateCommandQueueKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001C );
#define	__Dt__DtFunc_PVRSRVCreateCommandQueueKM__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001D );
#define	__Dt__DtFunc_PVRSRVCreateCommandQueueKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001E );
#define	__Dt__DtFunc_PVRSRVCreateCommandQueueKM__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001F );
#define	__Dt__DtFunc_PVRSRVCreateCommandQueueKM__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0020 );
#define	__Dt__DtFunc_PVRSRVCreateCommandQueueKM__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0021 );
#define	__Dt__DtFunc_PVRSRVCreateCommandQueueKM__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0022 );
#define	__Dt__DtFunc_PVRSRVCreateCommandQueueKM__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0023 );
#define	__Dt__DtFunc_PVRSRVCreateCommandQueueKM__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0024 );
#define	__Dt__DtFunc_PVRSRVCreateCommandQueueKM__DtStep_10	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0025 );
#define	__Dt__DtFunc_PVRSRVDestroyCommandQueueKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0026 );
#define	__Dt__DtFunc_PVRSRVDestroyCommandQueueKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0027 );
#define	__Dt__DtFunc_PVRSRVDestroyCommandQueueKM__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0028 );
#define	__Dt__DtFunc_PVRSRVDestroyCommandQueueKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002A );
#define	__Dt__DtFunc_PVRSRVDestroyCommandQueueKM__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002B );
#define	__Dt__DtFunc_PVRSRVDestroyCommandQueueKM__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002C );
#define	__Dt__DtFunc_PVRSRVDestroyCommandQueueKM__DtStep_6	/*while*/	_TP_BusOut( __DtBaseAddress, 0x002D );
#define	__Dt__DtFunc_PVRSRVDestroyCommandQueueKM__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002E );
#define	__Dt__DtFunc_PVRSRVDestroyCommandQueueKM__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002F );
#define	__Dt__DtFunc_PVRSRVDestroyCommandQueueKM__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0031 );
#define	__Dt__DtFunc_PVRSRVDestroyCommandQueueKM__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0032 );
#define	__Dt__DtFunc_PVRSRVDestroyCommandQueueKM__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0033 );
#define	__Dt__DtFunc_PVRSRVDestroyCommandQueueKM__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0034 );
#define	__Dt__DtFunc_PVRSRVDestroyCommandQueueKM__DtStep_13	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0035 );
#define	__Dt__DtFunc_PVRSRVGetQueueSpaceKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0036 );
#define	__Dt__DtFunc_PVRSRVGetQueueSpaceKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0037 );
#define	__Dt__DtFunc_PVRSRVGetQueueSpaceKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0038 );
#define	__Dt__DtFunc_PVRSRVGetQueueSpaceKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0039 );
#define	__Dt__DtFunc_PVRSRVGetQueueSpaceKM__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003A );
#define	__Dt__DtFunc_PVRSRVGetQueueSpaceKM__DtStep_5	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003B );
#define	__Dt__DtFunc_PVRSRVGetQueueSpaceKM__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003C );
#define	__Dt__DtFunc_PVRSRVGetQueueSpaceKM__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003D );
#define	__Dt__DtFunc_PVRSRVInsertCommandKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x003E );
#define	__Dt__DtFunc_PVRSRVInsertCommandKM__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003F );
#define	__Dt__DtFunc_PVRSRVInsertCommandKM__DtStep_2	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0040 );
#define	__Dt__DtFunc_PVRSRVInsertCommandKM__DtStep_3	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0041 );
#define	__Dt__DtFunc_PVRSRVInsertCommandKM__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0042 );
#define	__Dt__DtFunc_PVRSRVSubmitCommandKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0043 );
#define	__Dt__DtFunc_PVRSRVSubmitCommandKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0044 );
#define	__Dt__DtFunc_PVRSRVSubmitCommandKM__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0045 );
#define	__Dt__DtFunc_PVRSRVSubmitCommandKM__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0046 );
#define	__Dt__DtFunc_PVRSRVProcessCommand__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0047 );
#define	__Dt__DtFunc_PVRSRVProcessCommand__DtStep_1	/*while*/	_TP_BusOut( __DtBaseAddress, 0x0048 );
#define	__Dt__DtFunc_PVRSRVProcessCommand__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0049 );
#define	__Dt__DtFunc_PVRSRVProcessCommand__DtStep_3	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004A );
#define	__Dt__DtFunc_PVRSRVProcessCommand__DtStep_4	/*while*/	_TP_BusOut( __DtBaseAddress, 0x004B );
#define	__Dt__DtFunc_PVRSRVProcessCommand__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004C );
#define	__Dt__DtFunc_PVRSRVProcessCommand__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004D );
#define	__Dt__DtFunc_PVRSRVProcessCommand__DtStep_7	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004E );
#define	__Dt__DtFunc_PVRSRVProcessCommand__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004F );
#define	__Dt__DtFunc_PVRSRVProcessCommand__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0050 );
#define	__Dt__DtFunc_PVRSRVProcessCommand__DtStep_10	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0051 );
#define	__Dt__DtFunc_PVRSRVProcessCommand__DtStep_11	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0052 );
#define	__Dt__DtFunc_PVRSRVProcessCommand__DtStep_12	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0053 );
#define	__Dt__DtFunc_PVRSRVProcessCommand__DtStep_13	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0054 );
#define	__Dt__DtFunc_PVRSRVProcessCommand__DtStep_14	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0055 );
#define	__Dt__DtFunc_PVRSRVProcessQueues_ForEachCb__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0056 );
#define	__Dt__DtFunc_PVRSRVProcessQueues_ForEachCb__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0057 );
#define	__Dt__DtFunc_PVRSRVProcessQueues_ForEachCb__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0058 );
#define	__Dt__DtFunc_PVRSRVProcessQueues__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0059 );
#define	__Dt__DtFunc_PVRSRVProcessQueues__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005A );
#define	__Dt__DtFunc_PVRSRVProcessQueues__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005B );
#define	__Dt__DtFunc_PVRSRVProcessQueues__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005C );
#define	__Dt__DtFunc_PVRSRVProcessQueues__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005D );
#define	__Dt__DtFunc_PVRSRVProcessQueues__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x005E );
#define	__Dt__DtFunc_PVRSRVProcessQueues__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005F );
#define	__Dt__DtFunc_PVRSRVProcessQueues__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0060 );
#define	__Dt__DtFunc_PVRSRVProcessQueues__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0061 );
#define	__Dt__DtFunc_PVRSRVProcessQueues__DtStep_9	/*while*/	_TP_BusOut( __DtBaseAddress, 0x0062 );
#define	__Dt__DtFunc_PVRSRVProcessQueues__DtStep_10	/*while*/	_TP_BusOut( __DtBaseAddress, 0x0063 );
#define	__Dt__DtFunc_PVRSRVProcessQueues__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0064 );
#define	__Dt__DtFunc_PVRSRVProcessQueues__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0065 );
#define	__Dt__DtFunc_PVRSRVProcessQueues__DtStep_13	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0066 );
#define	__Dt__DtFunc_PVRSRVProcessQueues__DtStep_14	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0067 );
#define	__Dt__DtFunc_PVRSRVFreeCommandCompletePacketKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0068 );
#define	__Dt__DtFunc_PVRSRVFreeCommandCompletePacketKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0069 );
#define	__Dt__DtFunc_PVRSRVFreeCommandCompletePacketKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006A );
#define	__Dt__DtFunc_PVRSRVCommandCompleteKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x006B );
#define	__Dt__DtFunc_PVRSRVCommandCompleteKM__DtStep_1	/*for*/	_TP_BusOut( __DtBaseAddress, 0x006C );
#define	__Dt__DtFunc_PVRSRVCommandCompleteKM__DtStep_2	/*for*/	_TP_BusOut( __DtBaseAddress, 0x006D );
#define	__Dt__DtFunc_PVRSRVCommandCompleteKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006E );
#define	__Dt__DtFunc_PVRSRVCommandCompleteKM__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x006F );
#define	__Dt__DtFunc_PVRSRVRegisterCmdProcListKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0070 );
#define	__Dt__DtFunc_PVRSRVRegisterCmdProcListKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0071 );
#define	__Dt__DtFunc_PVRSRVRegisterCmdProcListKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0072 );
#define	__Dt__DtFunc_PVRSRVRegisterCmdProcListKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0073 );
#define	__Dt__DtFunc_PVRSRVRegisterCmdProcListKM__DtStep_4	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0074 );
#define	__Dt__DtFunc_PVRSRVRegisterCmdProcListKM__DtStep_5	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0075 );
#define	__Dt__DtFunc_PVRSRVRegisterCmdProcListKM__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0076 );
#define	__Dt__DtFunc_PVRSRVRegisterCmdProcListKM__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0077 );
#define	__Dt__DtFunc_PVRSRVRegisterCmdProcListKM__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0078 );
#define	__Dt__DtFunc_PVRSRVRegisterCmdProcListKM__DtStep_9	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0079 );
#define	__Dt__DtFunc_PVRSRVRemoveCmdProcListKM__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x007A );
#define	__Dt__DtFunc_PVRSRVRemoveCmdProcListKM__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007B );
#define	__Dt__DtFunc_PVRSRVRemoveCmdProcListKM__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x007C );
#define	__Dt__DtFunc_PVRSRVRemoveCmdProcListKM__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007D );
#define	__Dt__DtFunc_PVRSRVRemoveCmdProcListKM__DtStep_4	/*for*/	_TP_BusOut( __DtBaseAddress, 0x007E );
#define	__Dt__DtFunc_PVRSRVRemoveCmdProcListKM__DtStep_5	/*for*/	_TP_BusOut( __DtBaseAddress, 0x007F );
#define	__Dt__DtFunc_PVRSRVRemoveCmdProcListKM__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0081 );
#define	__Dt__DtFunc_PVRSRVRemoveCmdProcListKM__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0082 );

#endif	/* __DT_queue_h__ */

