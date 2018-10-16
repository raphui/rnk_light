#include <stdarg.h>
#include <errno.h>
#include <kernel/syscall.h>
#include <kernel/scheduler.h>
#include <kernel/kmutex.h>
#include <kernel/kqueue.h>
#include <kernel/ksem.h>
#include <kernel/ktime.h>

struct syscall syscall_table[] = {
	{SYSCALL_THREAD_SWITCH,	(unsigned int *)&schedule_yield,	SYSCALL_PRIVILEGE_OPERATION},
	{SYSCALL_THREAD_CREATE, (unsigned int *)&thread_create,		SYSCALL_PRIVILEGE_OPERATION},
	{SYSCALL_THREAD_JOIN,	(unsigned int *)&thread_join,		SYSCALL_PRIVILEGE_OPERATION},
	{SYSCALL_THREAD_STOP,	(unsigned int *)&schedule_thread_stop,	SYSCALL_PRIVILEGE_OPERATION},
	{SYSCALL_MUTEX_CREATE,	(unsigned int *)&kmutex_init,		SYSCALL_PRIVILEGE_OPERATION},
	{SYSCALL_MUTEX_ACQUIRE, (unsigned int *)&kmutex_lock,		SYSCALL_PRIVILEGE_ELEVATION},
	{SYSCALL_MUTEX_RELEASE, (unsigned int *)&kmutex_unlock,		SYSCALL_PRIVILEGE_ELEVATION},
	{SYSCALL_SEM_CREATE,	(unsigned int *)&ksem_init,		SYSCALL_PRIVILEGE_OPERATION},
	{SYSCALL_SEM_WAIT,	(unsigned int *)&ksem_wait,		SYSCALL_PRIVILEGE_ELEVATION},
	{SYSCALL_SEM_POST,	(unsigned int *)&ksem_post,		SYSCALL_PRIVILEGE_ELEVATION},
	{SYSCALL_QUEUE_CREATE,	(unsigned int *)&kqueue_init,		SYSCALL_PRIVILEGE_OPERATION},
	{SYSCALL_QUEUE_POST,	(unsigned int *)&kqueue_post,		SYSCALL_PRIVILEGE_ELEVATION},
	{SYSCALL_QUEUE_RECEIVE, (unsigned int *)&kqueue_receive,	SYSCALL_PRIVILEGE_ELEVATION},
	{SYSCALL_QUEUE_UPDATE,	(unsigned int *)&kqueue_update,		SYSCALL_PRIVILEGE_OPERATION},
	{SYSCALL_QUEUE_DESTROY, (unsigned int *)&kqueue_destroy,	SYSCALL_PRIVILEGE_OPERATION},
};
