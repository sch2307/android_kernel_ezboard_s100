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

#ifndef	__DT_bridged_sgx_bridge_h__
#define	__DT_bridged_sgx_bridge_h__

#ifdef	__cplusplus
	#define	__DtEXTERN	extern "C"
#else
	#define	__DtEXTERN	extern
#endif

/* TestPoint MacroCode -----------------------------------------------------*/
#define		__DtBaseAddress		0x280
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
#define		__DtFunc_SGXGetClientInfoBW		0
#define		__DtFunc_SGXReleaseClientInfoBW		1
#define		__DtFunc_SGXGetInternalDevInfoBW		2
#define		__DtFunc_SGXDoKickBW		3
#define		__DtFunc_SGXScheduleProcessQueuesBW		4
#define		__DtFunc_SGXSubmitTransferBW		5
#define		__DtFunc_SGXSubmit2DBW		6
#define		__DtFunc_SGXGetMiscInfoBW		7
#define		__DtFunc_SGXReadHWPerfCBBW		8
#define		__DtFunc_SGXDevInitPart2BW		9
#define		__DtFunc_SGXRegisterHWRenderContextBW		10
#define		__DtFunc_SGXUnregisterHWRenderContextBW		11
#define		__DtFunc_SGXRegisterHWTransferContextBW		12
#define		__DtFunc_SGXUnregisterHWTransferContextBW		13
#define		__DtFunc_SGXRegisterHW2DContextBW		14
#define		__DtFunc_SGXUnregisterHW2DContextBW		15
#define		__DtFunc_SGXFlushHWRenderTargetBW		16
#define		__DtFunc_SGX2DQueryBlitsCompleteBW		17
#define		__DtFunc_SGXFindSharedPBDescBW		18
#define		__DtFunc_SGXUnrefSharedPBDescBW		19
#define		__DtFunc_SGXAddSharedPBDescBW		20
#define		__DtFunc_SGXGetInfoForSrvinitBW		21
#define		__DtFunc_DumpBufferArray		22
#define		__DtFunc_SGXPDumpBufferArrayBW		23
#define		__DtFunc_SGXPDump3DSignatureRegistersBW		24
#define		__DtFunc_SGXPDumpCounterRegistersBW		25
#define		__DtFunc_SGXPDumpTASignatureRegistersBW		26
#define		__DtFunc_SGXPDumpHWPerfCBBW		27
#define		__DtFunc_SGXPDumpSaveMemBW		28
#define		__DtFunc_SetSGXDispatchTableEntry		29

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
#define		__DtStep_42		42
#define		__DtStep_43		43
#define		__DtStep_44		44
#define		__DtStep_45		45
#define		__DtStep_46		46
#define		__DtStep_47		47
#define		__DtStep_48		48
#define		__DtStep_49		49
#define		__DtStep_50		50
#define		__DtStep_51		51
#define		__DtStep_52		52
#define		__DtStep_53		53
#define		__DtStep_54		54
#define		__DtStep_55		55
#define		__DtStep_56		56
#define		__DtStep_57		57
#define		__DtStep_58		58
#define		__DtStep_59		59
#define		__DtStep_60		60
#define		__DtStep_61		61
#define		__DtStep_62		62
#define		__DtStep_63		63
#define		__DtStep_64		64
#define		__DtStep_65		65
#define		__DtStep_66		66
#define		__DtStep_67		67
#define		__DtStep_68		68
#define		__DtStep_69		69
#define		__DtStep_70		70
#define		__DtStep_71		71
#define		__DtStep_72		72
#define		__DtStep_73		73
#define		__DtStep_74		74
#define		__DtStep_75		75
#define		__DtStep_76		76

/* TestPoint DisableList ---------------------------------------------------*/
#define	__Dt__DtFunc_SGXGetClientInfoBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0000 );
#define	__Dt__DtFunc_SGXGetClientInfoBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0001 );
#define	__Dt__DtFunc_SGXGetClientInfoBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0002 );
#define	__Dt__DtFunc_SGXReleaseClientInfoBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0003 );
#define	__Dt__DtFunc_SGXReleaseClientInfoBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0004 );
#define	__Dt__DtFunc_SGXReleaseClientInfoBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0005 );
#define	__Dt__DtFunc_SGXGetInternalDevInfoBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0006 );
#define	__Dt__DtFunc_SGXGetInternalDevInfoBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0007 );
#define	__Dt__DtFunc_SGXGetInternalDevInfoBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0008 );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0009 );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000A );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000B );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000C );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_4	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000D );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x000E );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_6	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x000F );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0010 );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_8	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0011 );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0012 );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_10	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0013 );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_11	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0014 );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_12	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0015 );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_13	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0016 );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_14	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0017 );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_15	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0018 );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_16	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0019 );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_17	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001A );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_18	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001B );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_19	/*for*/	_TP_BusOut( __DtBaseAddress, 0x001C );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_20	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001D );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_21	/*if*/	_TP_BusOut( __DtBaseAddress, 0x001E );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_22	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x001F );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_23	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0020 );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_24	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0021 );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_25	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0022 );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_26	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0023 );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_27	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0024 );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_28	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0025 );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_29	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0026 );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_30	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0027 );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_31	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0028 );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_32	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002A );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_33	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002B );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_34	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002C );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_35	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002D );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_36	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x002E );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_37	/*if*/	_TP_BusOut( __DtBaseAddress, 0x002F );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_38	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0031 );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_39	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0032 );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_40	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0033 );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_41	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0034 );
#define	__Dt__DtFunc_SGXDoKickBW__DtStep_42	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0035 );
#define	__Dt__DtFunc_SGXScheduleProcessQueuesBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0036 );
#define	__Dt__DtFunc_SGXScheduleProcessQueuesBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0037 );
#define	__Dt__DtFunc_SGXScheduleProcessQueuesBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0038 );
#define	__Dt__DtFunc_SGXSubmitTransferBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0039 );
#define	__Dt__DtFunc_SGXSubmitTransferBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003A );
#define	__Dt__DtFunc_SGXSubmitTransferBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003B );
#define	__Dt__DtFunc_SGXSubmitTransferBW__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003C );
#define	__Dt__DtFunc_SGXSubmitTransferBW__DtStep_4	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003D );
#define	__Dt__DtFunc_SGXSubmitTransferBW__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x003E );
#define	__Dt__DtFunc_SGXSubmitTransferBW__DtStep_6	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x003F );
#define	__Dt__DtFunc_SGXSubmitTransferBW__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0040 );
#define	__Dt__DtFunc_SGXSubmitTransferBW__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0041 );
#define	__Dt__DtFunc_SGXSubmitTransferBW__DtStep_9	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0042 );
#define	__Dt__DtFunc_SGXSubmitTransferBW__DtStep_10	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0043 );
#define	__Dt__DtFunc_SGXSubmitTransferBW__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0044 );
#define	__Dt__DtFunc_SGXSubmitTransferBW__DtStep_12	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0045 );
#define	__Dt__DtFunc_SGXSubmitTransferBW__DtStep_13	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0046 );
#define	__Dt__DtFunc_SGXSubmitTransferBW__DtStep_14	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0047 );
#define	__Dt__DtFunc_SGXSubmitTransferBW__DtStep_15	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0048 );
#define	__Dt__DtFunc_SGXSubmit2DBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0049 );
#define	__Dt__DtFunc_SGXSubmit2DBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004A );
#define	__Dt__DtFunc_SGXSubmit2DBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004B );
#define	__Dt__DtFunc_SGXSubmit2DBW__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004C );
#define	__Dt__DtFunc_SGXSubmit2DBW__DtStep_4	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004D );
#define	__Dt__DtFunc_SGXSubmit2DBW__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x004E );
#define	__Dt__DtFunc_SGXSubmit2DBW__DtStep_6	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x004F );
#define	__Dt__DtFunc_SGXSubmit2DBW__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0050 );
#define	__Dt__DtFunc_SGXSubmit2DBW__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0051 );
#define	__Dt__DtFunc_SGXSubmit2DBW__DtStep_9	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0052 );
#define	__Dt__DtFunc_SGXSubmit2DBW__DtStep_10	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0053 );
#define	__Dt__DtFunc_SGXSubmit2DBW__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0054 );
#define	__Dt__DtFunc_SGXSubmit2DBW__DtStep_12	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0055 );
#define	__Dt__DtFunc_SGXSubmit2DBW__DtStep_13	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0056 );
#define	__Dt__DtFunc_SGXGetMiscInfoBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0057 );
#define	__Dt__DtFunc_SGXGetMiscInfoBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0058 );
#define	__Dt__DtFunc_SGXGetMiscInfoBW__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0059 );
#define	__Dt__DtFunc_SGXGetMiscInfoBW__DtStep_3	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005A );
#define	__Dt__DtFunc_SGXGetMiscInfoBW__DtStep_4	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005B );
#define	__Dt__DtFunc_SGXGetMiscInfoBW__DtStep_5	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005C );
#define	__Dt__DtFunc_SGXGetMiscInfoBW__DtStep_6	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005D );
#define	__Dt__DtFunc_SGXGetMiscInfoBW__DtStep_7	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005E );
#define	__Dt__DtFunc_SGXGetMiscInfoBW__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x005F );
#define	__Dt__DtFunc_SGXReadHWPerfCBBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0060 );
#define	__Dt__DtFunc_SGXReadHWPerfCBBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0061 );
#define	__Dt__DtFunc_SGXReadHWPerfCBBW__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0062 );
#define	__Dt__DtFunc_SGXReadHWPerfCBBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0063 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0064 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0065 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0066 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_3	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0067 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0068 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0069 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006A );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006B );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006C );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006D );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_10	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006E );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x006F );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_12	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0070 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_13	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0071 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_14	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0072 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_15	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0073 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_16	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0074 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_17	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0075 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_18	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0076 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_19	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0077 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_20	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0078 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_21	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0079 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_22	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007A );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_23	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007B );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_24	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007C );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_25	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007D );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_26	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007E );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_27	/*if*/	_TP_BusOut( __DtBaseAddress, 0x007F );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_28	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0081 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_29	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0082 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_30	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0083 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_31	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0084 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_32	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0085 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_33	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0086 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_34	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0087 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_35	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0088 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_36	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0089 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_37	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008A );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_38	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008B );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_39	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008C );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_40	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008D );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_41	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008E );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_42	/*if*/	_TP_BusOut( __DtBaseAddress, 0x008F );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_43	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0091 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_44	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0092 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_45	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0093 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_46	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0094 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_47	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0095 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_48	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0096 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_49	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0097 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_50	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0098 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_51	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0099 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_52	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009A );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_53	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009B );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_54	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009C );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_55	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009D );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_56	/*if*/	_TP_BusOut( __DtBaseAddress, 0x009E );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_57	/*for*/	_TP_BusOut( __DtBaseAddress, 0x009F );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_58	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A1 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_59	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A2 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_60	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00A3 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_61	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A4 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_62	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A5 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_63	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A6 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_64	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A7 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_65	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A8 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_66	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00A9 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_67	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00AA );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_68	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00AB );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_69	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00AC );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_70	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00AD );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_71	/*for*/	_TP_BusOut( __DtBaseAddress, 0x00AE );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_72	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00AF );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_73	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B1 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_74	/*for*/	_TP_BusOut( __DtBaseAddress, 0x00B2 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_75	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B3 );
#define	__Dt__DtFunc_SGXDevInitPart2BW__DtStep_76	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B4 );
#define	__Dt__DtFunc_SGXRegisterHWRenderContextBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00B5 );
#define	__Dt__DtFunc_SGXRegisterHWRenderContextBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B6 );
#define	__Dt__DtFunc_SGXRegisterHWRenderContextBW__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00B7 );
#define	__Dt__DtFunc_SGXRegisterHWRenderContextBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B8 );
#define	__Dt__DtFunc_SGXRegisterHWRenderContextBW__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00B9 );
#define	__Dt__DtFunc_SGXUnregisterHWRenderContextBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00BA );
#define	__Dt__DtFunc_SGXUnregisterHWRenderContextBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BB );
#define	__Dt__DtFunc_SGXUnregisterHWRenderContextBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BC );
#define	__Dt__DtFunc_SGXUnregisterHWRenderContextBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BD );
#define	__Dt__DtFunc_SGXRegisterHWTransferContextBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00BE );
#define	__Dt__DtFunc_SGXRegisterHWTransferContextBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00BF );
#define	__Dt__DtFunc_SGXRegisterHWTransferContextBW__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C0 );
#define	__Dt__DtFunc_SGXRegisterHWTransferContextBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C1 );
#define	__Dt__DtFunc_SGXRegisterHWTransferContextBW__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C2 );
#define	__Dt__DtFunc_SGXUnregisterHWTransferContextBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00C3 );
#define	__Dt__DtFunc_SGXUnregisterHWTransferContextBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C4 );
#define	__Dt__DtFunc_SGXUnregisterHWTransferContextBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C5 );
#define	__Dt__DtFunc_SGXUnregisterHWTransferContextBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C6 );
#define	__Dt__DtFunc_SGXRegisterHW2DContextBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00C7 );
#define	__Dt__DtFunc_SGXRegisterHW2DContextBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00C8 );
#define	__Dt__DtFunc_SGXRegisterHW2DContextBW__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00C9 );
#define	__Dt__DtFunc_SGXRegisterHW2DContextBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CA );
#define	__Dt__DtFunc_SGXRegisterHW2DContextBW__DtStep_4	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CB );
#define	__Dt__DtFunc_SGXUnregisterHW2DContextBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00CC );
#define	__Dt__DtFunc_SGXUnregisterHW2DContextBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CD );
#define	__Dt__DtFunc_SGXUnregisterHW2DContextBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CE );
#define	__Dt__DtFunc_SGXUnregisterHW2DContextBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00CF );
#define	__Dt__DtFunc_SGXFlushHWRenderTargetBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00D0 );
#define	__Dt__DtFunc_SGXFlushHWRenderTargetBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D1 );
#define	__Dt__DtFunc_SGXFlushHWRenderTargetBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D2 );
#define	__Dt__DtFunc_SGX2DQueryBlitsCompleteBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00D3 );
#define	__Dt__DtFunc_SGX2DQueryBlitsCompleteBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D4 );
#define	__Dt__DtFunc_SGX2DQueryBlitsCompleteBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D5 );
#define	__Dt__DtFunc_SGX2DQueryBlitsCompleteBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00D6 );
#define	__Dt__DtFunc_SGXFindSharedPBDescBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00D7 );
#define	__Dt__DtFunc_SGXFindSharedPBDescBW__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00D8 );
#define	__Dt__DtFunc_SGXFindSharedPBDescBW__DtStep_2	/*for*/	_TP_BusOut( __DtBaseAddress, 0x00D9 );
#define	__Dt__DtFunc_SGXFindSharedPBDescBW__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00DA );
#define	__Dt__DtFunc_SGXFindSharedPBDescBW__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00DB );
#define	__Dt__DtFunc_SGXFindSharedPBDescBW__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00DC );
#define	__Dt__DtFunc_SGXFindSharedPBDescBW__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00DD );
#define	__Dt__DtFunc_SGXFindSharedPBDescBW__DtStep_7	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00DE );
#define	__Dt__DtFunc_SGXUnrefSharedPBDescBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00DF );
#define	__Dt__DtFunc_SGXUnrefSharedPBDescBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E0 );
#define	__Dt__DtFunc_SGXUnrefSharedPBDescBW__DtStep_2	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E1 );
#define	__Dt__DtFunc_SGXUnrefSharedPBDescBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00E2 );
#define	__Dt__DtFunc_SGXAddSharedPBDescBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00E3 );
#define	__Dt__DtFunc_SGXAddSharedPBDescBW__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E4 );
#define	__Dt__DtFunc_SGXAddSharedPBDescBW__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E5 );
#define	__Dt__DtFunc_SGXAddSharedPBDescBW__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E6 );
#define	__Dt__DtFunc_SGXAddSharedPBDescBW__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E7 );
#define	__Dt__DtFunc_SGXAddSharedPBDescBW__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E8 );
#define	__Dt__DtFunc_SGXAddSharedPBDescBW__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00E9 );
#define	__Dt__DtFunc_SGXAddSharedPBDescBW__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00EA );
#define	__Dt__DtFunc_SGXAddSharedPBDescBW__DtStep_8	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00EB );
#define	__Dt__DtFunc_SGXAddSharedPBDescBW__DtStep_9	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00EC );
#define	__Dt__DtFunc_SGXAddSharedPBDescBW__DtStep_10	/*for*/	_TP_BusOut( __DtBaseAddress, 0x00ED );
#define	__Dt__DtFunc_SGXAddSharedPBDescBW__DtStep_11	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00EE );
#define	__Dt__DtFunc_SGXAddSharedPBDescBW__DtStep_12	/*for*/	_TP_BusOut( __DtBaseAddress, 0x00EF );
#define	__Dt__DtFunc_SGXAddSharedPBDescBW__DtStep_13	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F1 );
#define	__Dt__DtFunc_SGXAddSharedPBDescBW__DtStep_14	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F2 );
#define	__Dt__DtFunc_SGXAddSharedPBDescBW__DtStep_15	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F3 );
#define	__Dt__DtFunc_SGXAddSharedPBDescBW__DtStep_16	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F4 );
#define	__Dt__DtFunc_SGXAddSharedPBDescBW__DtStep_17	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F5 );
#define	__Dt__DtFunc_SGXGetInfoForSrvinitBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00F6 );
#define	__Dt__DtFunc_SGXGetInfoForSrvinitBW__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00F7 );
#define	__Dt__DtFunc_SGXGetInfoForSrvinitBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F8 );
#define	__Dt__DtFunc_SGXGetInfoForSrvinitBW__DtStep_3	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00F9 );
#define	__Dt__DtFunc_SGXGetInfoForSrvinitBW__DtStep_4	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00FA );
#define	__Dt__DtFunc_SGXGetInfoForSrvinitBW__DtStep_5	/*for*/	_TP_BusOut( __DtBaseAddress, 0x00FB );
#define	__Dt__DtFunc_SGXGetInfoForSrvinitBW__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00FC );
#define	__Dt__DtFunc_SGXGetInfoForSrvinitBW__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x00FD );
#define	__Dt__DtFunc_SGXGetInfoForSrvinitBW__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x00FE );
#define	__Dt__DtFunc_DumpBufferArray__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x00FF );
#define	__Dt__DtFunc_DumpBufferArray__DtStep_1	/*for*/	_TP_BusOut( __DtBaseAddress, 0x0100 );
#define	__Dt__DtFunc_DumpBufferArray__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0101 );
#define	__Dt__DtFunc_DumpBufferArray__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0102 );
#define	__Dt__DtFunc_DumpBufferArray__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0103 );
#define	__Dt__DtFunc_DumpBufferArray__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0104 );
#define	__Dt__DtFunc_DumpBufferArray__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0105 );
#define	__Dt__DtFunc_DumpBufferArray__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0106 );
#define	__Dt__DtFunc_DumpBufferArray__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0107 );
#define	__Dt__DtFunc_SGXPDumpBufferArrayBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0108 );
#define	__Dt__DtFunc_SGXPDumpBufferArrayBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0109 );
#define	__Dt__DtFunc_SGXPDumpBufferArrayBW__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x010A );
#define	__Dt__DtFunc_SGXPDumpBufferArrayBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x010B );
#define	__Dt__DtFunc_SGXPDumpBufferArrayBW__DtStep_4	/*for*/	_TP_BusOut( __DtBaseAddress, 0x010C );
#define	__Dt__DtFunc_SGXPDumpBufferArrayBW__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x010D );
#define	__Dt__DtFunc_SGXPDumpBufferArrayBW__DtStep_6	/*if*/	_TP_BusOut( __DtBaseAddress, 0x010E );
#define	__Dt__DtFunc_SGXPDumpBufferArrayBW__DtStep_7	/*if*/	_TP_BusOut( __DtBaseAddress, 0x010F );
#define	__Dt__DtFunc_SGXPDumpBufferArrayBW__DtStep_8	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0110 );
#define	__Dt__DtFunc_SGXPDump3DSignatureRegistersBW__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0111 );
#define	__Dt__DtFunc_SGXPDump3DSignatureRegistersBW__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0112 );
#define	__Dt__DtFunc_SGXPDump3DSignatureRegistersBW__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0113 );
#define	__Dt__DtFunc_SGXPDump3DSignatureRegistersBW__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0114 );
#define	__Dt__DtFunc_SGXPDump3DSignatureRegistersBW__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0115 );
#define	__Dt__DtFunc_SGXPDump3DSignatureRegistersBW__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0116 );
#define	__Dt__DtFunc_SGXPDumpCounterRegistersBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0117 );
#define	__Dt__DtFunc_SGXPDumpCounterRegistersBW__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0118 );
#define	__Dt__DtFunc_SGXPDumpCounterRegistersBW__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0119 );
#define	__Dt__DtFunc_SGXPDumpCounterRegistersBW__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x011A );
#define	__Dt__DtFunc_SGXPDumpCounterRegistersBW__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x011B );
#define	__Dt__DtFunc_SGXPDumpCounterRegistersBW__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x011C );
#define	__Dt__DtFunc_SGXPDumpCounterRegistersBW__DtStep_6	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x011D );
#define	__Dt__DtFunc_SGXPDumpTASignatureRegistersBW__DtStep_0	/*if*/	_TP_BusOut( __DtBaseAddress, 0x011E );
#define	__Dt__DtFunc_SGXPDumpTASignatureRegistersBW__DtStep_1	/*if*/	_TP_BusOut( __DtBaseAddress, 0x011F );
#define	__Dt__DtFunc_SGXPDumpTASignatureRegistersBW__DtStep_2	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0120 );
#define	__Dt__DtFunc_SGXPDumpTASignatureRegistersBW__DtStep_3	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0121 );
#define	__Dt__DtFunc_SGXPDumpTASignatureRegistersBW__DtStep_4	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0122 );
#define	__Dt__DtFunc_SGXPDumpTASignatureRegistersBW__DtStep_5	/*if*/	_TP_BusOut( __DtBaseAddress, 0x0123 );
#define	__Dt__DtFunc_SGXPDumpHWPerfCBBW__DtStep_0	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0124 );
#define	__Dt__DtFunc_SGXPDumpHWPerfCBBW__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0125 );
#define	__Dt__DtFunc_SGXPDumpHWPerfCBBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0126 );
#define	__Dt__DtFunc_SGXPDumpHWPerfCBBW__DtStep_3	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x0127 );
#define	__Dt__DtFunc_SGXPDumpSaveMemBW__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x0128 );
#define	__Dt__DtFunc_SGXPDumpSaveMemBW__DtStep_1	/*if+FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x012A );
#define	__Dt__DtFunc_SGXPDumpSaveMemBW__DtStep_2	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x012B );
#define	__Dt__DtFunc_SetSGXDispatchTableEntry__DtStep_0	/*FuncIn*/	_TP_BusOut( __DtBaseAddress, 0x012C );
#define	__Dt__DtFunc_SetSGXDispatchTableEntry__DtStep_1	/*FuncOut*/	_TP_BusOut( __DtBaseAddress, 0x012D );

#endif	/* __DT_bridged_sgx_bridge_h__ */

