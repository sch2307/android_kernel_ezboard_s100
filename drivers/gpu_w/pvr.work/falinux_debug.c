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
	__fa_msg_on = 0;
}

void 	__disp_msg_on (void)
{
	__fa_msg_on = 1;
}

void 	__disp_msg_rst(void)
{
	__fa_msg_on = 1;
	level_tab   = 0;
}

void __enter_tab(void)
{
	level_tab++;
}

void __exit_tab(void)
{
	if( level_tab ) 
	{	
		level_tab--;
	}
	else
	{
		if ( __fa_msg_on ) printk( "[ENTER/EXIT dismatching...]" );
	}		
}

void __disp_tab(void)
{
	int lp;
	
	if( fafs_debug_disp_option & FAFS_DEBUG_DISP_PID )
	{
		if ( __fa_msg_on ) printk( "%03d>", current->pid );
	}	
	
	if ( __fa_msg_on ) 
		for( lp = 0; lp < level_tab * FAFS_TAB_LEN; lp++ ) printk(" " );
}


// »ç¿ë¹ý sample
#if 0
int mvbd_open(struct block_device *bdev, fmode_t fmode )
{
	ENTER();
	EXIT();
    return 0;
}

int mvbd_release(struct gendisk *bdev, fmode_t fmode )
{
	ENTER();
	EXIT();
    return 0;
}

int mvbd_getgeo(struct block_device *bdev, struct hd_geometry *geo)
{
	ENTER();
	
	geo->cylinders 	= mvbd_cylinders;
	geo->heads     	= mvbd_heads;
	geo->sectors   	= mvbd_sectors;
	geo->start      = 0;

	DTAB(); printk("geo->cylinders = %d\n",geo->cylinders );
	DTAB(); printk("geo->heads     = %d\n",geo->heads );
	DTAB(); printk("geo->sectors   = %d\n",geo->sectors );

	EXIT();
	return 0;
}
#endif

