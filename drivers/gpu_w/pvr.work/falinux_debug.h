
#define FAFS_TAB_LEN				2

#define	FAFS_DEBUG_DISP_ALL			0xFFFFFFFF
#define	FAFS_DEBUG_DISP_PID			(1<<0)

#ifndef VAR_FAFS_DEBUG
extern	int 	fafs_debug_disp_option;	
extern	int 	__fa_msg_on;	
#endif

extern 	void 	__enter_tab	  (void);
extern 	void 	__exit_tab	  (void);
extern 	void 	__disp_tab	  (void);
extern 	void 	__disp_msg_off(void);
extern 	void 	__disp_msg_on (void);
extern 	void 	__disp_msg_rst(void);


//#ifdef CONFIG_FALINUX_ZEROBOOT
#if 0 

#define _IF_MSG_		if ( __fa_msg_on ) 

#define	DTAB_MARK(mark)	do{__disp_tab(); _IF_MSG_ printk(mark); }while(0)
#define	DTAB()			DTAB_MARK("| ")

#define DLN()			do{ _IF_MSG_ printk("\n"); }while(0)

#define	DTAB_PRN(fmt,args...)	\
						do {DTAB_MARK("| "); _IF_MSG_ printk( fmt, ## args );}while(0)

                		
#define	FA_TRACE()			do{ \
								DTAB_MARK("| "); \
	            		   		_IF_MSG_ printk("TR %s(%d)\n",__FUNCTION__,__LINE__ ); \
	            		   }while(0)
                		
                		
#define FA_ENTER() 		do{ \
								DTAB_MARK("+ "); \
	            		   		_IF_MSG_ printk("EN %s(%d)\n",__FUNCTION__,__LINE__ ); \
	            		   		__enter_tab(); \
	            		   }while(0)
	            		   
#define FA_EXIT() 		do{ \
							__exit_tab(); \
	            		  		DTAB_MARK("< "); \
	            		   		_IF_MSG_ printk("EX %s(%d)\n",__FUNCTION__,__LINE__ ); \
	            		}while(0)

#define FA_MSG_OFF() 	do{ __disp_msg_off(); }while(0)
#define FA_MSG_ON() 	do{ __disp_msg_on();  }while(0)
#define FA_MSG_RST() 	do{ __disp_msg_rst(); }while(0)

#else

#define	DTAB_MARK(mark)			do{} while(0)
#define	DTAB()					do{} while(0)
#define DLN()					do{} while(0)
#define	DTAB_PRN(fmt,args...)	do{} while(0)
#define	FA_TRACE()				do{} while(0)
#define FA_ENTER() 				do{} while(0)
#define FA_EXIT() 				do{} while(0)

#define FA_MSG_OFF() 			do{} while(0)
#define FA_MSG_ON() 			do{} while(0)
#define FA_MSG_RST() 			do{} while(0)

#endif
	               
