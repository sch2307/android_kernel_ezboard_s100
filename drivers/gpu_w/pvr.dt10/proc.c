#include	"DT_proc.h"	/* For DynamicTracer-TestPoint */
/**********************************************************************
 *
 * Copyright(c) 2008 Imagination Technologies Ltd. All rights reserved.
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope it will be useful but, except 
 * as otherwise stated in writing, without any warranty; without even the 
 * implied warranty of merchantability or fitness for a particular purpose. 
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 * 
 * The full GNU General Public License is included in this distribution in
 * the file called "COPYING".
 *
 * Contact Information:
 * Imagination Technologies Ltd. <gpl-support@imgtec.com>
 * Home Park Estate, Kings Langley, Herts, WD4 8LZ, UK 
 *
 ******************************************************************************/

#ifndef AUTOCONF_INCLUDED
 #include <linux/config.h>
#endif

#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#include "services_headers.h"

#include "queue.h"
#include "resman.h"
#include "pvrmmap.h"
#include "pvr_debug.h"
#include "pvrversion.h"
#include "proc.h"
#include "perproc.h"
#include "env_perproc.h"
#include "linkage.h"

#include "lists.h"

static struct proc_dir_entry * dir;

static const IMG_CHAR PVRProcDirRoot[] = "pvr";

static IMG_INT pvr_proc_open(struct inode *inode,struct file *file);
static void *pvr_proc_seq_start (struct seq_file *m, loff_t *pos);
static void pvr_proc_seq_stop (struct seq_file *m, void *v);
static void *pvr_proc_seq_next (struct seq_file *m, void *v, loff_t *pos);
static int pvr_proc_seq_show (struct seq_file *m, void *v);
static ssize_t pvr_proc_write(struct file *file, const char __user *buffer, size_t count, loff_t *ppos);

static struct file_operations pvr_proc_operations =
{
	.open		= pvr_proc_open,
	.read		= seq_read,
	.write		= pvr_proc_write,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

static struct seq_operations pvr_proc_seq_operations =
{
	.start =	pvr_proc_seq_start,
	.next =		pvr_proc_seq_next,
	.stop =		pvr_proc_seq_stop,
	.show =		pvr_proc_seq_show,
};

static struct proc_dir_entry* g_pProcQueue;
static struct proc_dir_entry* g_pProcVersion;
static struct proc_dir_entry* g_pProcSysNodes;

#ifdef DEBUG
static struct proc_dir_entry* g_pProcDebugLevel;
#endif

#ifdef PVR_MANUAL_POWER_CONTROL
static struct proc_dir_entry* g_pProcPowerLevel;
#endif


static void ProcSeqShowVersion(struct seq_file *sfile,void* el);

static void ProcSeqShowSysNodes(struct seq_file *sfile,void* el);
static void* ProcSeqOff2ElementSysNodes(struct seq_file * sfile, loff_t off);

off_t printAppend(IMG_CHAR * buffer, size_t size, off_t off, const IMG_CHAR * format, ...)
{
    IMG_INT n;
    size_t space = size - (size_t)off;
    va_list ap;

    __DtTestPoint(__DtFunc_printAppend, __DtStep_0)
    va_start (ap, format);

    n = vsnprintf (buffer+off, space, format, ap);

    va_end (ap);
    
    if (n >= (IMG_INT)space || n < 0)
    {
	
        __DtTestPoint(__DtFunc_printAppend, __DtStep_1)
        buffer[size - 1] = 0;
        __DtTestPoint(__DtFunc_printAppend, __DtStep_2)
        return (off_t)(size - 1);
    }
    else
    {
        __DtTestPoint(__DtFunc_printAppend, __DtStep_3)
        return (off + (off_t)n);
    }
	__DtTestPoint(__DtFunc_printAppend, __DtStep_4)
}


void* ProcSeq1ElementOff2Element(struct seq_file *sfile, loff_t off)
{
	__DtTestPoint(__DtFunc_ProcSeq1ElementOff2Element, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(sfile);
	
	if(!off)
		return (void*)2;
	__DtTestPoint(__DtFunc_ProcSeq1ElementOff2Element, __DtStep_1)
	return NULL;
}


void* ProcSeq1ElementHeaderOff2Element(struct seq_file *sfile, loff_t off)
{
	__DtTestPoint(__DtFunc_ProcSeq1ElementHeaderOff2Element, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(sfile);

	if(!off)
	{
		__DtTestPoint(__DtFunc_ProcSeq1ElementHeaderOff2Element, __DtStep_1)
		return PVR_PROC_SEQ_START_TOKEN;
	}

	
	if(off == 1)
		return (void*)2;

	__DtTestPoint(__DtFunc_ProcSeq1ElementHeaderOff2Element, __DtStep_2)
	return NULL;
}


static IMG_INT pvr_proc_open(struct inode *inode,struct file *file)
{
	IMG_INT ret = seq_open(file, &pvr_proc_seq_operations);

	struct seq_file *seq = (struct seq_file*)file->private_data;
	struct proc_dir_entry* pvr_proc_entry = PDE(inode);

	
	__DtTestPoint(__DtFunc_pvr_proc_open, __DtStep_0)
	seq->private = pvr_proc_entry->data;
	__DtTestPoint(__DtFunc_pvr_proc_open, __DtStep_1)
	return ret;
}

static ssize_t pvr_proc_write(struct file *file, const char __user *buffer,
		size_t count, loff_t *ppos)
{
	struct inode *inode = file->f_path.dentry->d_inode;
	struct proc_dir_entry * dp;

	__DtTestPoint(__DtFunc_pvr_proc_write, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(ppos);
	dp = PDE(inode);

	if (!dp->write_proc)
		return -EIO;

	__DtTestPoint(__DtFunc_pvr_proc_write, __DtStep_1)
	return dp->write_proc(file, buffer, count, dp->data);
}


static void *pvr_proc_seq_start (struct seq_file *proc_seq_file, loff_t *pos)
{
	PVR_PROC_SEQ_HANDLERS *handlers = (PVR_PROC_SEQ_HANDLERS*)proc_seq_file->private;
	__DtTestPoint(__DtFunc_pvr_proc_seq_start, __DtStep_0)
	if(handlers->startstop != NULL)
		handlers->startstop(proc_seq_file, IMG_TRUE);
	__DtTestPoint(__DtFunc_pvr_proc_seq_start, __DtStep_1)
	return handlers->off2element(proc_seq_file, *pos);
}

static void pvr_proc_seq_stop (struct seq_file *proc_seq_file, void *v)
{
	PVR_PROC_SEQ_HANDLERS *handlers = (PVR_PROC_SEQ_HANDLERS*)proc_seq_file->private;
	__DtTestPoint(__DtFunc_pvr_proc_seq_stop, __DtStep_0)
	PVR_UNREFERENCED_PARAMETER(v);

	if(handlers->startstop != NULL)
		handlers->startstop(proc_seq_file, IMG_FALSE);
	__DtTestPoint(__DtFunc_pvr_proc_seq_stop, __DtStep_1)
}

static void *pvr_proc_seq_next (struct seq_file *proc_seq_file, void *v, loff_t *pos)
{
	PVR_PROC_SEQ_HANDLERS *handlers = (PVR_PROC_SEQ_HANDLERS*)proc_seq_file->private;
	__DtTestPoint(__DtFunc_pvr_proc_seq_next, __DtStep_0)
	(*pos)++;
	if( handlers->next != NULL)
		return handlers->next( proc_seq_file, v, *pos );
	__DtTestPoint(__DtFunc_pvr_proc_seq_next, __DtStep_1)
	return handlers->off2element(proc_seq_file, *pos);
}

static int pvr_proc_seq_show (struct seq_file *proc_seq_file, void *v)
{
	PVR_PROC_SEQ_HANDLERS *handlers = (PVR_PROC_SEQ_HANDLERS*)proc_seq_file->private;
	__DtTestPoint(__DtFunc_pvr_proc_seq_show, __DtStep_0)
	handlers->show( proc_seq_file,v );
    __DtTestPoint(__DtFunc_pvr_proc_seq_show, __DtStep_1)
    return 0;
}



static struct proc_dir_entry* CreateProcEntryInDirSeq(
									   struct proc_dir_entry *pdir,
									   const IMG_CHAR * name,
    								   IMG_VOID* data,
									   pvr_next_proc_seq_t next_handler,
									   pvr_show_proc_seq_t show_handler,
									   pvr_off2element_proc_seq_t off2element_handler,
									   pvr_startstop_proc_seq_t startstop_handler,
									   write_proc_t whandler
									   )
{

    struct proc_dir_entry * file;
	mode_t mode;

    __DtTestPoint(__DtFunc_CreateProcEntryInDirSeq, __DtStep_0)
    if (!dir)
    {
        __DtTestPoint(__DtFunc_CreateProcEntryInDirSeq, __DtStep_1)
        PVR_DPF((PVR_DBG_ERROR, "CreateProcEntryInDirSeq: cannot make proc entry /proc/%s/%s: no parent", PVRProcDirRoot, name));
        __DtTestPoint(__DtFunc_CreateProcEntryInDirSeq, __DtStep_2)
        return NULL;
    }

	mode = S_IFREG;

    if (show_handler)
    {
		__DtTestPoint(__DtFunc_CreateProcEntryInDirSeq, __DtStep_3)
		mode |= S_IRUGO;
    }

    if (whandler)
    {
		__DtTestPoint(__DtFunc_CreateProcEntryInDirSeq, __DtStep_4)
		mode |= S_IWUSR;
    }

	file=create_proc_entry(name, mode, pdir);

    if (file)
    {
		PVR_PROC_SEQ_HANDLERS *seq_handlers;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,30))
        file->owner = THIS_MODULE;
#endif

		file->proc_fops = &pvr_proc_operations;
		file->write_proc = whandler;

		
		file->data =  kmalloc(sizeof(PVR_PROC_SEQ_HANDLERS), GFP_KERNEL);
		if(file->data)
		{
			__DtTestPoint(__DtFunc_CreateProcEntryInDirSeq, __DtStep_5)
			seq_handlers = (PVR_PROC_SEQ_HANDLERS*)file->data;
			seq_handlers->next = next_handler;
			seq_handlers->show = show_handler;
			seq_handlers->off2element = off2element_handler;
			seq_handlers->startstop = startstop_handler;
			seq_handlers->data = data;

        	__DtTestPoint(__DtFunc_CreateProcEntryInDirSeq, __DtStep_6)
        	return file;
		}
    }

    PVR_DPF((PVR_DBG_ERROR, "CreateProcEntryInDirSeq: cannot make proc entry /proc/%s/%s: no memory", PVRProcDirRoot, name));
    __DtTestPoint(__DtFunc_CreateProcEntryInDirSeq, __DtStep_7)
    return NULL;
}


struct proc_dir_entry* CreateProcReadEntrySeq (
								const IMG_CHAR * name,
								IMG_VOID* data,
								pvr_next_proc_seq_t next_handler,
								pvr_show_proc_seq_t show_handler,
								pvr_off2element_proc_seq_t off2element_handler,
								pvr_startstop_proc_seq_t startstop_handler
							   )
{
	__DtTestPoint(__DtFunc_CreateProcReadEntrySeq, __DtStep_0)
	return CreateProcEntrySeq(name,
							  data,
							  next_handler,
							  show_handler,
							  off2element_handler,
							  startstop_handler,
							  NULL);
}

struct proc_dir_entry* CreateProcEntrySeq (
											const IMG_CHAR * name,
											IMG_VOID* data,
											pvr_next_proc_seq_t next_handler,
											pvr_show_proc_seq_t show_handler,
											pvr_off2element_proc_seq_t off2element_handler,
											pvr_startstop_proc_seq_t startstop_handler,
											write_proc_t whandler
										  )
{
	__DtTestPoint(__DtFunc_CreateProcEntrySeq, __DtStep_0)
	return CreateProcEntryInDirSeq(
								   dir,
								   name,
								   data,
								   next_handler,
								   show_handler,
								   off2element_handler,
								   startstop_handler,
								   whandler
								  );
}



struct proc_dir_entry* CreatePerProcessProcEntrySeq (
									  const IMG_CHAR * name,
    								  IMG_VOID* data,
									  pvr_next_proc_seq_t next_handler,
									  pvr_show_proc_seq_t show_handler,
									  pvr_off2element_proc_seq_t off2element_handler,
									  pvr_startstop_proc_seq_t startstop_handler,
									  write_proc_t whandler
									 )
{
    PVRSRV_ENV_PER_PROCESS_DATA *psPerProc;
    IMG_UINT32 ui32PID;

    __DtTestPoint(__DtFunc_CreatePerProcessProcEntrySeq, __DtStep_0)
    if (!dir)
    {
        __DtTestPoint(__DtFunc_CreatePerProcessProcEntrySeq, __DtStep_1)
        PVR_DPF((PVR_DBG_ERROR, "CreatePerProcessProcEntrySeq: /proc/%s doesn't exist", PVRProcDirRoot));
        __DtTestPoint(__DtFunc_CreatePerProcessProcEntrySeq, __DtStep_2)
        return NULL;
    }

    ui32PID = OSGetCurrentProcessIDKM();

    psPerProc = PVRSRVPerProcessPrivateData(ui32PID);
    if (!psPerProc)
    {
        __DtTestPoint(__DtFunc_CreatePerProcessProcEntrySeq, __DtStep_3)
        PVR_DPF((PVR_DBG_ERROR, "CreatePerProcessProcEntrySeq: no per process data"));

        __DtTestPoint(__DtFunc_CreatePerProcessProcEntrySeq, __DtStep_4)
        return NULL;
    }

    if (!psPerProc->psProcDir)
    {
        IMG_CHAR dirname[16];
        IMG_INT ret;

        __DtTestPoint(__DtFunc_CreatePerProcessProcEntrySeq, __DtStep_5)
        ret = snprintf(dirname, sizeof(dirname), "%u", ui32PID);

		if (ret <=0 || ret >= (IMG_INT)sizeof(dirname))
		{
			__DtTestPoint(__DtFunc_CreatePerProcessProcEntrySeq, __DtStep_6)
			PVR_DPF((PVR_DBG_ERROR, "CreatePerProcessProcEntries: couldn't generate per process proc directory name \"%u\"", ui32PID));
			__DtTestPoint(__DtFunc_CreatePerProcessProcEntrySeq, __DtStep_7)
			return NULL;
		}
		else
		{
			__DtTestPoint(__DtFunc_CreatePerProcessProcEntrySeq, __DtStep_8)
			psPerProc->psProcDir = proc_mkdir(dirname, dir);
			if (!psPerProc->psProcDir)
			{
				__DtTestPoint(__DtFunc_CreatePerProcessProcEntrySeq, __DtStep_9)
				PVR_DPF((PVR_DBG_ERROR, "CreatePerProcessProcEntries: couldn't create per process proc directory /proc/%s/%u",
						PVRProcDirRoot, ui32PID));
				__DtTestPoint(__DtFunc_CreatePerProcessProcEntrySeq, __DtStep_10)
				return NULL;
			}
		}
    }

    __DtTestPoint(__DtFunc_CreatePerProcessProcEntrySeq, __DtStep_11)
    return CreateProcEntryInDirSeq(psPerProc->psProcDir, name, data, next_handler,
								   show_handler,off2element_handler,startstop_handler,whandler);
}


IMG_VOID RemoveProcEntrySeq( struct proc_dir_entry* proc_entry )
{
    __DtTestPoint(__DtFunc_RemoveProcEntrySeq, __DtStep_0)
    if (dir)
    {
		void* data = proc_entry->data ;
        __DtTestPoint(__DtFunc_RemoveProcEntrySeq, __DtStep_1)
        PVR_DPF((PVR_DBG_MESSAGE, "Removing /proc/%s/%s", PVRProcDirRoot, proc_entry->name));

        remove_proc_entry(proc_entry->name, dir);
		if( data)
			kfree( data );

    }
	__DtTestPoint(__DtFunc_RemoveProcEntrySeq, __DtStep_2)
}

IMG_VOID RemovePerProcessProcEntrySeq(struct proc_dir_entry* proc_entry)
{
    PVRSRV_ENV_PER_PROCESS_DATA *psPerProc;

    __DtTestPoint(__DtFunc_RemovePerProcessProcEntrySeq, __DtStep_0)
    psPerProc = LinuxTerminatingProcessPrivateData();
    if (!psPerProc)
    {
        __DtTestPoint(__DtFunc_RemovePerProcessProcEntrySeq, __DtStep_1)
        psPerProc = PVRSRVFindPerProcessPrivateData();
        if (!psPerProc)
        {
            __DtTestPoint(__DtFunc_RemovePerProcessProcEntrySeq, __DtStep_2)
            PVR_DPF((PVR_DBG_ERROR, "CreatePerProcessProcEntries: can't "
                                    "remove %s, no per process data", proc_entry->name));
            __DtTestPoint(__DtFunc_RemovePerProcessProcEntrySeq, __DtStep_3)
            return;
        }
    }

    if (psPerProc->psProcDir)
    {
		void* data = proc_entry->data ;
        __DtTestPoint(__DtFunc_RemovePerProcessProcEntrySeq, __DtStep_4)
        PVR_DPF((PVR_DBG_MESSAGE, "Removing proc entry %s from %s", proc_entry->name, psPerProc->psProcDir->name));

        remove_proc_entry(proc_entry->name, psPerProc->psProcDir);
		if(data)
			kfree( data );
    }
	__DtTestPoint(__DtFunc_RemovePerProcessProcEntrySeq, __DtStep_5)
}

static IMG_INT pvr_read_proc(IMG_CHAR *page, IMG_CHAR **start, off_t off,
                         IMG_INT count, IMG_INT *eof, IMG_VOID *data)
{
	 
    pvr_read_proc_t *pprn = (pvr_read_proc_t *)data;

    off_t len = pprn (page, (size_t)count, off);

    __DtTestPoint(__DtFunc_pvr_read_proc, __DtStep_0)
    if (len == END_OF_FILE)
    {
        __DtTestPoint(__DtFunc_pvr_read_proc, __DtStep_1)
        len  = 0;
        *eof = 1;
    }
    else if (!len)             
    {
        __DtTestPoint(__DtFunc_pvr_read_proc, __DtStep_2)
        *start = (IMG_CHAR *) 0;   
    }
    else
    {
        __DtTestPoint(__DtFunc_pvr_read_proc, __DtStep_3)
        *start = (IMG_CHAR *) 1;
    }

    __DtTestPoint(__DtFunc_pvr_read_proc, __DtStep_4)
    return len;
}


static IMG_INT CreateProcEntryInDir(struct proc_dir_entry *pdir, const IMG_CHAR * name, read_proc_t rhandler, write_proc_t whandler, IMG_VOID *data)
{
    struct proc_dir_entry * file;
    mode_t mode;

    __DtTestPoint(__DtFunc_CreateProcEntryInDir, __DtStep_0)
    if (!pdir)
    {
        __DtTestPoint(__DtFunc_CreateProcEntryInDir, __DtStep_1)
        PVR_DPF((PVR_DBG_ERROR, "CreateProcEntryInDir: parent directory doesn't exist"));

        __DtTestPoint(__DtFunc_CreateProcEntryInDir, __DtStep_2)
        return -ENOMEM;
    }

    mode = S_IFREG;

    if (rhandler)
    {
	__DtTestPoint(__DtFunc_CreateProcEntryInDir, __DtStep_3)
	mode |= S_IRUGO;
    }

    if (whandler)
    {
	__DtTestPoint(__DtFunc_CreateProcEntryInDir, __DtStep_4)
	mode |= S_IWUSR;
    }

    file = create_proc_entry(name, mode, pdir);

    if (file)
    {
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,30))
        file->owner = THIS_MODULE;
#endif
		file->read_proc = rhandler;
		file->write_proc = whandler;
		file->data = data;

		PVR_DPF((PVR_DBG_MESSAGE, "Created proc entry %s in %s", name, pdir->name));

        __DtTestPoint(__DtFunc_CreateProcEntryInDir, __DtStep_5)
        return 0;
    }

    PVR_DPF((PVR_DBG_ERROR, "CreateProcEntry: cannot create proc entry %s in %s", name, pdir->name));

    __DtTestPoint(__DtFunc_CreateProcEntryInDir, __DtStep_6)
    return -ENOMEM;
}


IMG_INT CreateProcEntry(const IMG_CHAR * name, read_proc_t rhandler, write_proc_t whandler, IMG_VOID *data)
{
    __DtTestPoint(__DtFunc_CreateProcEntry, __DtStep_0)
    return CreateProcEntryInDir(dir, name, rhandler, whandler, data);
}


IMG_INT CreatePerProcessProcEntry(const IMG_CHAR * name, read_proc_t rhandler, write_proc_t whandler, IMG_VOID *data)
{
    PVRSRV_ENV_PER_PROCESS_DATA *psPerProc;
    IMG_UINT32 ui32PID;

    __DtTestPoint(__DtFunc_CreatePerProcessProcEntry, __DtStep_0)
    if (!dir)
    {
        __DtTestPoint(__DtFunc_CreatePerProcessProcEntry, __DtStep_1)
        PVR_DPF((PVR_DBG_ERROR, "CreatePerProcessProcEntries: /proc/%s doesn't exist", PVRProcDirRoot));

        __DtTestPoint(__DtFunc_CreatePerProcessProcEntry, __DtStep_2)
        return -ENOMEM;
    }

    ui32PID = OSGetCurrentProcessIDKM();

    psPerProc = PVRSRVPerProcessPrivateData(ui32PID);
    if (!psPerProc)
    {
        __DtTestPoint(__DtFunc_CreatePerProcessProcEntry, __DtStep_3)
        PVR_DPF((PVR_DBG_ERROR, "CreatePerProcessProcEntries: no per process data"));

        __DtTestPoint(__DtFunc_CreatePerProcessProcEntry, __DtStep_4)
        return -ENOMEM;
    }

    if (!psPerProc->psProcDir)
    {
        IMG_CHAR dirname[16];
        IMG_INT ret;

        __DtTestPoint(__DtFunc_CreatePerProcessProcEntry, __DtStep_5)
        ret = snprintf(dirname, sizeof(dirname), "%u", ui32PID);

		if (ret <=0 || ret >= (IMG_INT)sizeof(dirname))
		{
			__DtTestPoint(__DtFunc_CreatePerProcessProcEntry, __DtStep_6)
			PVR_DPF((PVR_DBG_ERROR, "CreatePerProcessProcEntries: couldn't generate per process proc directory name \"%u\"", ui32PID));

					__DtTestPoint(__DtFunc_CreatePerProcessProcEntry, __DtStep_7)
					return -ENOMEM;
		}
		else
		{
			__DtTestPoint(__DtFunc_CreatePerProcessProcEntry, __DtStep_8)
			psPerProc->psProcDir = proc_mkdir(dirname, dir);
			if (!psPerProc->psProcDir)
			{
			__DtTestPoint(__DtFunc_CreatePerProcessProcEntry, __DtStep_9)
			PVR_DPF((PVR_DBG_ERROR, "CreatePerProcessProcEntries: couldn't create per process proc directory /proc/%s/%u", PVRProcDirRoot, ui32PID));

			__DtTestPoint(__DtFunc_CreatePerProcessProcEntry, __DtStep_10)
			return -ENOMEM;
			}
		}
    }

    __DtTestPoint(__DtFunc_CreatePerProcessProcEntry, __DtStep_11)
    return CreateProcEntryInDir(psPerProc->psProcDir, name, rhandler, whandler, data);
}


IMG_INT CreateProcReadEntry(const IMG_CHAR * name, pvr_read_proc_t handler)
{
    struct proc_dir_entry * file;

    __DtTestPoint(__DtFunc_CreateProcReadEntry, __DtStep_0)
    if (!dir)
    {
        __DtTestPoint(__DtFunc_CreateProcReadEntry, __DtStep_1)
        PVR_DPF((PVR_DBG_ERROR, "CreateProcReadEntry: cannot make proc entry /proc/%s/%s: no parent", PVRProcDirRoot, name));

        __DtTestPoint(__DtFunc_CreateProcReadEntry, __DtStep_2)
        return -ENOMEM;
    }

	 
    file = create_proc_read_entry (name, S_IFREG | S_IRUGO, dir, pvr_read_proc, (IMG_VOID *)handler);

    if (file)
    {
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,30))
        file->owner = THIS_MODULE;
#endif
        return 0;
    }

    PVR_DPF((PVR_DBG_ERROR, "CreateProcReadEntry: cannot make proc entry /proc/%s/%s: no memory", PVRProcDirRoot, name));

    __DtTestPoint(__DtFunc_CreateProcReadEntry, __DtStep_3)
    return -ENOMEM;
}


IMG_INT CreateProcEntries(IMG_VOID)
{
    __DtTestPoint(__DtFunc_CreateProcEntries, __DtStep_0)
    dir = proc_mkdir (PVRProcDirRoot, NULL);

    if (!dir)
    {
        __DtTestPoint(__DtFunc_CreateProcEntries, __DtStep_1)
        PVR_DPF((PVR_DBG_ERROR, "CreateProcEntries: cannot make /proc/%s directory", PVRProcDirRoot));

        __DtTestPoint(__DtFunc_CreateProcEntries, __DtStep_2)
        return -ENOMEM;
    }

	g_pProcQueue = CreateProcReadEntrySeq("queue", NULL, NULL, ProcSeqShowQueue, ProcSeqOff2ElementQueue, NULL);
	g_pProcVersion = CreateProcReadEntrySeq("version", NULL, NULL, ProcSeqShowVersion, ProcSeq1ElementHeaderOff2Element, NULL);
	g_pProcSysNodes = CreateProcReadEntrySeq("nodes", NULL, NULL, ProcSeqShowSysNodes, ProcSeqOff2ElementSysNodes, NULL);

	if(!g_pProcQueue || !g_pProcVersion || !g_pProcSysNodes)
    {
        __DtTestPoint(__DtFunc_CreateProcEntries, __DtStep_3)
        PVR_DPF((PVR_DBG_ERROR, "CreateProcEntries: couldn't make /proc/%s files", PVRProcDirRoot));

        __DtTestPoint(__DtFunc_CreateProcEntries, __DtStep_4)
        return -ENOMEM;
    }


#ifdef DEBUG

	g_pProcDebugLevel = CreateProcEntrySeq("debug_level", NULL, NULL,
											ProcSeqShowDebugLevel,
											ProcSeq1ElementOff2Element, NULL,
										    (IMG_VOID*)PVRDebugProcSetLevel);
	if(!g_pProcDebugLevel)
    {
        __DtTestPoint(__DtFunc_CreateProcEntries, __DtStep_5)
        PVR_DPF((PVR_DBG_ERROR, "CreateProcEntries: couldn't make /proc/%s/debug_level", PVRProcDirRoot));

        __DtTestPoint(__DtFunc_CreateProcEntries, __DtStep_6)
        return -ENOMEM;
    }

#ifdef PVR_MANUAL_POWER_CONTROL
	g_pProcPowerLevel = CreateProcEntrySeq("power_control", NULL, NULL,
											ProcSeqShowPowerLevel,
											ProcSeq1ElementOff2Element, NULL,
										    PVRProcSetPowerLevel);
	if(!g_pProcPowerLevel)
    {
        __DtTestPoint(__DtFunc_CreateProcEntries, __DtStep_7)
        PVR_DPF((PVR_DBG_ERROR, "CreateProcEntries: couldn't make /proc/%s/power_control", PVRProcDirRoot));

        __DtTestPoint(__DtFunc_CreateProcEntries, __DtStep_8)
        return -ENOMEM;
    }
#endif
#endif

    return 0;
}


IMG_VOID RemoveProcEntry(const IMG_CHAR * name)
{
    __DtTestPoint(__DtFunc_RemoveProcEntry, __DtStep_0)
    if (dir)
    {
        __DtTestPoint(__DtFunc_RemoveProcEntry, __DtStep_1)
        remove_proc_entry(name, dir);
        PVR_DPF((PVR_DBG_MESSAGE, "Removing /proc/%s/%s", PVRProcDirRoot, name));
    }
	__DtTestPoint(__DtFunc_RemoveProcEntry, __DtStep_2)
}


IMG_VOID RemovePerProcessProcEntry(const IMG_CHAR *name)
{
    PVRSRV_ENV_PER_PROCESS_DATA *psPerProc;

    __DtTestPoint(__DtFunc_RemovePerProcessProcEntry, __DtStep_0)
    psPerProc = LinuxTerminatingProcessPrivateData();
    if (!psPerProc)
    {
        __DtTestPoint(__DtFunc_RemovePerProcessProcEntry, __DtStep_1)
        psPerProc = PVRSRVFindPerProcessPrivateData();
        if (!psPerProc)
        {
            __DtTestPoint(__DtFunc_RemovePerProcessProcEntry, __DtStep_2)
            PVR_DPF((PVR_DBG_ERROR, "CreatePerProcessProcEntries: can't "
                                    "remove %s, no per process data", name));
            __DtTestPoint(__DtFunc_RemovePerProcessProcEntry, __DtStep_3)
            return;
        }
    }

    if (psPerProc->psProcDir)
    {
        __DtTestPoint(__DtFunc_RemovePerProcessProcEntry, __DtStep_4)
        remove_proc_entry(name, psPerProc->psProcDir);

        PVR_DPF((PVR_DBG_MESSAGE, "Removing proc entry %s from %s", name, psPerProc->psProcDir->name));
    }
	__DtTestPoint(__DtFunc_RemovePerProcessProcEntry, __DtStep_5)
}


IMG_VOID RemovePerProcessProcDir(PVRSRV_ENV_PER_PROCESS_DATA *psPerProc)
{
    __DtTestPoint(__DtFunc_RemovePerProcessProcDir, __DtStep_0)
    if (psPerProc->psProcDir)
    {
        __DtTestPoint(__DtFunc_RemovePerProcessProcDir, __DtStep_1)
        while (psPerProc->psProcDir->subdir)
        {
            __DtTestPoint(__DtFunc_RemovePerProcessProcDir, __DtStep_2)
            PVR_DPF((PVR_DBG_WARNING, "Belatedly removing /proc/%s/%s/%s", PVRProcDirRoot, psPerProc->psProcDir->name, psPerProc->psProcDir->subdir->name));

            RemoveProcEntry(psPerProc->psProcDir->subdir->name);
        }
        RemoveProcEntry(psPerProc->psProcDir->name);
    }
	__DtTestPoint(__DtFunc_RemovePerProcessProcDir, __DtStep_3)
}

IMG_VOID RemoveProcEntries(IMG_VOID)
{
#ifdef DEBUG
	RemoveProcEntrySeq( g_pProcDebugLevel );
#ifdef PVR_MANUAL_POWER_CONTROL
	RemoveProcEntrySeq( g_pProcPowerLevel );
#endif 
#endif

	RemoveProcEntrySeq(g_pProcQueue);
	RemoveProcEntrySeq(g_pProcVersion);
	RemoveProcEntrySeq(g_pProcSysNodes);

	while (dir->subdir)
	{
		__DtTestPoint(__DtFunc_RemoveProcEntries, __DtStep_0)
		PVR_DPF((PVR_DBG_WARNING, "Belatedly removing /proc/%s/%s", PVRProcDirRoot, dir->subdir->name));

		RemoveProcEntry(dir->subdir->name);
	}

	remove_proc_entry(PVRProcDirRoot, NULL);
	__DtTestPoint(__DtFunc_RemoveProcEntries, __DtStep_1)
}

static void ProcSeqShowVersion(struct seq_file *sfile,void* el)
{
	SYS_DATA *psSysData;
	IMG_CHAR *pszSystemVersionString = "None";

	__DtTestPoint(__DtFunc_ProcSeqShowVersion, __DtStep_0)
	if(el == PVR_PROC_SEQ_START_TOKEN)
	{
		__DtTestPoint(__DtFunc_ProcSeqShowVersion, __DtStep_1)
		seq_printf(sfile,
				"Version %s (%s) %s\n",
				PVRVERSION_STRING,
				PVR_BUILD_TYPE, PVR_BUILD_DIR);
		__DtTestPoint(__DtFunc_ProcSeqShowVersion, __DtStep_2)
		return;
	}

	psSysData = SysAcquireDataNoCheck();
	if(psSysData != IMG_NULL && psSysData->pszVersionString != IMG_NULL)
	{
		__DtTestPoint(__DtFunc_ProcSeqShowVersion, __DtStep_3)
		pszSystemVersionString = psSysData->pszVersionString;
	}

	seq_printf( sfile, "System Version String: %s\n", pszSystemVersionString);
	__DtTestPoint(__DtFunc_ProcSeqShowVersion, __DtStep_4)
}

static const IMG_CHAR *deviceTypeToString(PVRSRV_DEVICE_TYPE deviceType)
{
    __DtTestPoint(__DtFunc_deviceTypeToString, __DtStep_0)
    switch (deviceType)
    {
        default:
        {
            static IMG_CHAR text[10];

            __DtTestPoint(__DtFunc_deviceTypeToString, __DtStep_1)
            sprintf(text, "?%x", (IMG_UINT)deviceType);

            __DtTestPoint(__DtFunc_deviceTypeToString, __DtStep_2)
            return text;
        }
    }
	__DtTestPoint(__DtFunc_deviceTypeToString, __DtStep_3)
}


static const IMG_CHAR *deviceClassToString(PVRSRV_DEVICE_CLASS deviceClass)
{
    __DtTestPoint(__DtFunc_deviceClassToString, __DtStep_0)
    switch (deviceClass)
    {
	case PVRSRV_DEVICE_CLASS_3D:
	{
	    __DtTestPoint(__DtFunc_deviceClassToString, __DtStep_1)
	    return "3D";
	}
	case PVRSRV_DEVICE_CLASS_DISPLAY:
	{
	    __DtTestPoint(__DtFunc_deviceClassToString, __DtStep_2)
	    return "display";
	}
	case PVRSRV_DEVICE_CLASS_BUFFER:
	{
	    __DtTestPoint(__DtFunc_deviceClassToString, __DtStep_3)
	    return "buffer";
	}
	default:
	{
	    static IMG_CHAR text[10];

	    __DtTestPoint(__DtFunc_deviceClassToString, __DtStep_4)
	    sprintf(text, "?%x", (IMG_UINT)deviceClass);
	    __DtTestPoint(__DtFunc_deviceClassToString, __DtStep_5)
	    return text;
	}
    }
	__DtTestPoint(__DtFunc_deviceClassToString, __DtStep_6)
}

static IMG_VOID* DecOffPsDev_AnyVaCb(PVRSRV_DEVICE_NODE *psNode, va_list va)
{
	off_t *pOff = va_arg(va, off_t*);
	__DtTestPoint(__DtFunc_DecOffPsDev_AnyVaCb, __DtStep_0)
	if (--(*pOff))
	{
		__DtTestPoint(__DtFunc_DecOffPsDev_AnyVaCb, __DtStep_1)
		return IMG_NULL;
	}
	else
	{
		__DtTestPoint(__DtFunc_DecOffPsDev_AnyVaCb, __DtStep_2)
		return psNode;
	}
	__DtTestPoint(__DtFunc_DecOffPsDev_AnyVaCb, __DtStep_3)
}

static void ProcSeqShowSysNodes(struct seq_file *sfile,void* el)
{
	PVRSRV_DEVICE_NODE *psDevNode;

	__DtTestPoint(__DtFunc_ProcSeqShowSysNodes, __DtStep_0)
	if(el == PVR_PROC_SEQ_START_TOKEN)
	{
		__DtTestPoint(__DtFunc_ProcSeqShowSysNodes, __DtStep_1)
		seq_printf( sfile,
						"Registered nodes\n"
						"Addr     Type     Class    Index Ref pvDev     Size Res\n");
		__DtTestPoint(__DtFunc_ProcSeqShowSysNodes, __DtStep_2)
		return;
	}

	psDevNode = (PVRSRV_DEVICE_NODE*)el;

	seq_printf( sfile,
			  "%p %-8s %-8s %4d  %2u  %p  %3u  %p\n",
			  psDevNode,
			  deviceTypeToString(psDevNode->sDevId.eDeviceType),
			  deviceClassToString(psDevNode->sDevId.eDeviceClass),
			  psDevNode->sDevId.eDeviceClass,
			  psDevNode->ui32RefCount,
			  psDevNode->pvDevice,
			  psDevNode->ui32pvDeviceSize,
			  psDevNode->hResManContext);
	__DtTestPoint(__DtFunc_ProcSeqShowSysNodes, __DtStep_3)
}

static void* ProcSeqOff2ElementSysNodes(struct seq_file * sfile, loff_t off)
{
    SYS_DATA *psSysData;
    PVRSRV_DEVICE_NODE*psDevNode = IMG_NULL;
    
    __DtTestPoint(__DtFunc_ProcSeqOff2ElementSysNodes, __DtStep_0)
    PVR_UNREFERENCED_PARAMETER(sfile);
    
    if(!off)
    {
	__DtTestPoint(__DtFunc_ProcSeqOff2ElementSysNodes, __DtStep_1)
	return PVR_PROC_SEQ_START_TOKEN;
    }

    psSysData = SysAcquireDataNoCheck();
    if (psSysData != IMG_NULL)
    {
	
	__DtTestPoint(__DtFunc_ProcSeqOff2ElementSysNodes, __DtStep_2)
	psDevNode = (PVRSRV_DEVICE_NODE*)
			List_PVRSRV_DEVICE_NODE_Any_va(psSysData->psDeviceNodeList,
													DecOffPsDev_AnyVaCb,
													&off);
    }

    
    __DtTestPoint(__DtFunc_ProcSeqOff2ElementSysNodes, __DtStep_3)
    return (void*)psDevNode;
}

