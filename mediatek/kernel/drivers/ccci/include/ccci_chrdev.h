#ifndef __CCCI_CHRDEV_H__
#define __CCCI_CHRDEV_H__
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/wait.h>
#include <asm/atomic.h>
#include <ccci_layer.h>

#define MAX_BUFFER_MESSAGES  ((1<<20)/sizeof(CCCI_BUFF_T))
#define DEFAULT_BUFFER (CCCI_FIFO_MAX_LEN*2)

#define recv_array_is_full(array)	({	\
	unsigned int __nr=(array)->nr;	\
	unsigned int __idx=(array)->in_idx; \
	WARN_ON(__idx>__nr); \
	__nr==__idx ; 	})

#define recv_array_is_empty(array)	({	\
	unsigned int __nr=(array)->in_idx;	\
	unsigned int __idx=(array)->out_idx; \
	WARN_ON(__idx>__nr); \
	__nr==__idx ; })

#define reset_recv_array(array)  ({ \
	(array)->in_idx=0;	\
	(array)->out_idx=0; })


#define get_first_entry(list,type,mem)	({ \
		type * __ret;\
		if (list_empty(list)) __ret=NULL;\
		else __ret=list_entry((list)->next,type,mem);\
		__ret; })

#define get_last_entry(list,type,mem) ({ \
		type * __ret;\
		if (list_empty(list)) __ret=NULL;\
		else __ret=list_entry((list)->prev,type,mem);\
		__ret; })

#define ccci_get_first(_list)  get_first_entry(_list,struct recv_array,list)
#define ccci_get_last(_list)   get_last_entry(_list,struct recv_array,list)
#define ccci_is_last_recv(list) list_is_last((list)->next,list)


struct recv_array{
	CCCI_BUFF_T *array;
	unsigned int nr;
	unsigned int in_idx;
	unsigned int out_idx;
	struct list_head list;

};

struct ccci_dev_client{
	spinlock_t lock;
	atomic_t user;
	pid_t pid;
	int ch_num;
	unsigned int  used_nr;
	unsigned int  free_nr;
	struct list_head buff_list;
	struct list_head free_list;
	struct list_head dev_list;
	wait_queue_head_t wait_q;
	struct fasync_struct *fasync;
	volatile unsigned int wakeup_waitq;
};

extern int ccci_chrdev_init(void);
extern void ccci_chrdev_exit(void);














#endif