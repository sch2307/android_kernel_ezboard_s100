#include	"DT_falinux_debug.h"	/* For DynamicTracer-TestPoint */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/sched.h>

#define VAR_FAFS_DEBUG
#include "falinux_debug.h"


static int 	level_tab 				= 0;
int 		fafs_debug_disp_option	= FAFS_DEBUG_DISP_ALL;	
int         __fa_msg_on             = 1; 

void 	__disp_msg_off(void)
{
	__DtTestPoint(__DtFunc___disp_msg_off, __DtStep_0)
	__fa_msg_on = 0;
	__DtTestPoint(__DtFunc___disp_msg_off, __DtStep_1)
}

void 	__disp_msg_on (void)
{
	__DtTestPoint(__DtFunc___disp_msg_on, __DtStep_0)
	__fa_msg_on = 1;
	__DtTestPoint(__DtFunc___disp_msg_on, __DtStep_1)
}

void 	__disp_msg_rst(void)
{
	__DtTestPoint(__DtFunc___disp_msg_rst, __DtStep_0)
	__fa_msg_on = 1;
	level_tab   = 0;
	__DtTestPoint(__DtFunc___disp_msg_rst, __DtStep_1)
}

void __enter_tab(void)
{
	__DtTestPoint(__DtFunc___enter_tab, __DtStep_0)
	level_tab++;
	__DtTestPoint(__DtFunc___enter_tab, __DtStep_1)
}

void __exit_tab(void)
{
	__DtTestPoint(__DtFunc___exit_tab, __DtStep_0)
	if( level_tab ) 
	{	
		__DtTestPoint(__DtFunc___exit_tab, __DtStep_1)
		level_tab--;
	}
	else
	{
		__DtTestPoint(__DtFunc___exit_tab, __DtStep_2)
		if ( __fa_msg_on ) printk( "[ENTER/EXIT dismatching...]" );
	}		
	__DtTestPoint(__DtFunc___exit_tab, __DtStep_3)
}

void __disp_tab(void)
{
	int lp;
	
	__DtTestPoint(__DtFunc___disp_tab, __DtStep_0)
	if( fafs_debug_disp_option & FAFS_DEBUG_DISP_PID )
	{
		__DtTestPoint(__DtFunc___disp_tab, __DtStep_1)
		if ( __fa_msg_on ) printk( "%03d>", current->pid );
	}	
	
	if ( __fa_msg_on ) 
		for( lp = 0; lp < level_tab * FAFS_TAB_LEN; lp++ ) printk(" " );
	__DtTestPoint(__DtFunc___disp_tab, __DtStep_2)
}


// ;g?k9} sample
#if 0
int mvbd_open(struct block_device *bdev, fmode_t fmode )
{
	__DtTestPoint(__DtFunc_mvbd_open, __DtStep_0)
	ENTER();
	EXIT();
    __DtTestPoint(__DtFunc_mvbd_open, __DtStep_1)
    return 0;
}

int mvbd_release(struct gendisk *bdev, fmode_t fmode )
{
	__DtTestPoint(__DtFunc_mvbd_release, __DtStep_0)
	ENTER();
	EXIT();
    __DtTestPoint(__DtFunc_mvbd_release, __DtStep_1)
    return 0;
}

int mvbd_getgeo(struct block_device *bdev, struct hd_geometry *geo)
{
	__DtTestPoint(__DtFunc_mvbd_getgeo, __DtStep_0)
	ENTER();
	
	geo->cylinders 	= mvbd_cylinders;
	geo->heads     	= mvbd_heads;
	geo->sectors   	= mvbd_sectors;
	geo->start      = 0;

	DTAB(); printk("geo->cylinders = %d\n",geo->cylinders );
	DTAB(); printk("geo->heads     = %d\n",geo->heads );
	DTAB(); printk("geo->sectors   = %d\n",geo->sectors );

	EXIT();
	__DtTestPoint(__DtFunc_mvbd_getgeo, __DtStep_1)
	return 0;
}
#endif

