/*
 * thread.c
 *
 *  Created on: 13 juil. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __linux)
#include <swt.h>
#include <unistd.h>
#include <pthread.h>
#include <dlfcn.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
struct _w_mutex {
	union {
		void *handle[0x10];
		struct {
			pthread_mutex_t mutex;
		};
	};
};
struct _w_condition {
	union {
		void *handle[0x10];
		struct {
			w_mutex *mutex;
			pthread_cond_t cond;
		};
	};
};
wresult _w_mutex_result(int err) {
	switch (err) {
	case EBUSY:
		return W_ERROR_MUTEX_BUSY;
		break;
	case EPERM:
		return W_ERROR_MUTEX_UNLOCKED;
		break;
	case EDEADLK:
		return W_ERROR_MUTEX_DEAD_LOCK;
	case EINVAL:
		break;
	case ETIMEDOUT:
		return W_ERROR_MUTEX_TIMEOUT;
	case 0:
		return W_ERROR_MUTEX_NO_ERROR;
	}
	return W_ERROR_MUTEX_MISC_ERROR;
}
void w_mutex_init(w_mutex *mutex){

}
wresult w_mutex_create(w_mutex *mutex, w_mutex_type type) {
	pthread_mutexattr_t _attr, *attr = NULL;
	if (type == W_MUTEX_RECURSIVE) {
		pthread_mutexattr_init(&_attr);
		pthread_mutexattr_settype(&_attr, PTHREAD_MUTEX_RECURSIVE);
		attr = &_attr;
	}
	int err = pthread_mutex_init(&((struct _w_mutex*) mutex)->mutex, attr);
	if (err == 0) {
		return W_OK;
	} else {
		return W_ERROR_NO_HANDLES;
	}
}
void w_mutex_dispose(w_mutex *mutex) {
	pthread_mutex_destroy(&((struct _w_mutex*) mutex)->mutex);
}
wresult w_mutex_lock(w_mutex *mutex) {
	return _w_mutex_result(
			pthread_mutex_lock(&((struct _w_mutex*) mutex)->mutex));
}
#define MSEC_IN_SEC   1000
#define  NSEC_IN_MSEC  1000000
#define  NSEC_IN_USEC  1000
#define NSEC_IN_SEC   (MSEC_IN_SEC * NSEC_IN_MSEC)
wresult w_mutex_lock_timeout(w_mutex *mutex, unsigned long ms) {
	struct timespec ts;
	time_t seconds = ms / MSEC_IN_SEC;
	long nanoseconds = (ms % MSEC_IN_SEC) * NSEC_IN_MSEC;
	ts.tv_sec = time(NULL);
	ts.tv_sec += seconds;
	ts.tv_nsec += nanoseconds;
	if (ts.tv_nsec > NSEC_IN_SEC) {
		ts.tv_sec += 1;
		ts.tv_nsec -= NSEC_IN_SEC;
	}
	return _w_mutex_result(
			pthread_mutex_timedlock(&((struct _w_mutex*) mutex)->mutex, &ts));

}
wresult w_mutex_try_lock(w_mutex *mutex) {
	return _w_mutex_result(
			pthread_mutex_trylock(&((struct _w_mutex*) mutex)->mutex));

}
wresult w_mutex_unlock(w_mutex *mutex) {
	return _w_mutex_result(
			pthread_mutex_unlock(&((struct _w_mutex*) mutex)->mutex));
}
void w_condition_init(w_condition *condition){

}
wresult w_condition_create(w_condition *condition, w_mutex *mutex) {
	int err = pthread_cond_init(&((struct _w_condition*) condition)->cond,
	NULL /* default attributes */);
	if (err == 0) {
		return W_OK;
	} else {
		return W_ERROR_NO_HANDLES;
	}
}
void w_condition_dispose(w_condition *condition) {
	pthread_cond_destroy(&((struct _w_condition*) condition)->cond);
}
wresult w_condition_wait(w_condition *condition) {
	int err = pthread_cond_wait(&((struct _w_condition*) condition)->cond,
			(pthread_mutex_t*) ((struct _w_condition*) condition)->mutex);
	if (err != 0) {
		return W_ERROR_MUTEX_MISC_ERROR;
	} else
		return W_OK;
}
wresult w_condition_wait_timeout(w_condition *condition,
		unsigned long milliseconds) {
	struct timespec ts;
	time_t seconds = milliseconds / MSEC_IN_SEC;
	long nanoseconds = (milliseconds % MSEC_IN_SEC) * NSEC_IN_MSEC;
	ts.tv_sec = time(NULL);
	ts.tv_sec += seconds;
	ts.tv_nsec += nanoseconds;
	if (ts.tv_nsec > NSEC_IN_SEC) {
		ts.tv_sec += 1;
		ts.tv_nsec -= NSEC_IN_SEC;
	}
	int err = pthread_cond_timedwait(&((struct _w_condition*) condition)->cond,
			(pthread_mutex_t*) ((struct _w_condition*) condition)->mutex, &ts);
	if (err != 0) {
		return W_ERROR_MUTEX_MISC_ERROR;
	} else
		return W_OK;
}
wresult w_condition_signal(w_condition *condition) {
	int err = pthread_cond_signal(&((struct _w_condition*) condition)->cond);
	if (err != 0) {
		return W_ERROR_MUTEX_MISC_ERROR;
	} else
		return W_OK;
}
wresult w_condition_broadcast(w_condition *condition) {
	int err = pthread_cond_broadcast(&((struct _w_condition*) condition)->cond);
	if (err != 0) {
		return W_ERROR_MUTEX_MISC_ERROR;
	} else
		return W_OK;
}
wresult w_semaphore_create(w_semaphore *semaphore, int initialcount,
		int maxcount) {
}
void w_semaphore_dispose(w_semaphore *semaphore) {

}
wresult w_semaphore_wait(w_semaphore *semaphore) {
}
wresult w_semaphore_try_wait(w_semaphore *semaphore) {
}
wresult w_semaphore_wait_timeout(w_semaphore *semaphore,
		unsigned long milliseconds) {
}
wresult w_semaphore_post(w_semaphore *semaphore) {
}
void w_thread_init(w_thread *thread) {
	thread->id = 0;
	thread->args = 0;
	thread->start_proc = 0;
}
wresult w_threadid_equal(w_threadid thread1, w_threadid thread2) {
	return pthread_equal(thread1, thread2);
}
wresult w_thread_equal(w_thread *thread1, w_thread *thread2) {
	return pthread_equal(thread1->id, thread2->id);
}
void w_thread_dispose(w_thread *thread) {
	if (thread != 0 && thread->id != 0) {
		pthread_cancel(thread->id);
		thread->id = 0;
	}
}
static pthread_key_t _pthread_key = 0;
static void _w_cleanup_handler(void *arg) {
	w_thread *thread = (w_thread*) arg;
	w_thread_cleanup *clean = thread->cleanup;
	while (clean != 0) {
		clean->routine(clean->args);
		clean = clean->prev;
	}
}
void w_thread_cleanup_push(w_thread_cleanup *thread_cleanup,
		void (*routine)(void*), void *arg) {
	w_thread *thread = (w_thread*) pthread_getspecific(_pthread_key);
	if (thread == 0) {
		return;
	}
	thread_cleanup->args = arg;
	thread_cleanup->routine = routine;
	thread_cleanup->prev = thread->cleanup;
	thread->cleanup = thread_cleanup;
}
void w_thread_cleanup_pop(int execute) {
	w_thread *thread = (w_thread*) pthread_getspecific(_pthread_key);
	if (thread == 0) {
		return;
	}
	w_thread_cleanup *thread_cleanup = thread->cleanup;
	if (thread_cleanup == 0)
		return;
	thread->cleanup = thread_cleanup->prev;
	if (execute) {
		thread_cleanup->routine(thread_cleanup->args);
	}
}
void* _w_start_routine(void *_thread) {
	w_thread *thread = (w_thread*) _thread;
	int ret;
	pthread_setspecific(_pthread_key, thread);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);
	thread->cleanup = 0;
	pthread_cleanup_push(_w_cleanup_handler,thread);
		ret = thread->start_proc(thread->args);
		pthread_setspecific(_pthread_key, 0);
		pthread_cleanup_pop(0);
	return (void*) ((intptr_t) ret);
}
wresult w_thread_create(w_thread *thread, w_thread_start start_proc,
		void *args, size_t stackSize) {
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	if (stackSize)
		pthread_attr_setstacksize(&attr, stackSize);
	if (_pthread_key == 0) {
		pthread_key_create(&_pthread_key, NULL);
	}
	thread->start_proc = start_proc;
	thread->args = args;
	thread->cleanup = 0;
	int rc = pthread_create((pthread_t*) &thread->id, &attr, _w_start_routine,
			thread);
	pthread_attr_destroy(&attr);
	if (rc == 0) {
		return W_ERROR_NO_HANDLES;
	} else {
		return W_TRUE;
	}
}
wresult w_thread_cancel(w_thread *thread) {
	pthread_cancel(thread->id);
	return W_TRUE;
}
wresult w_thread_join(w_thread *thread) {
	pthread_join(thread->id, NULL);
	return W_TRUE;
}
wresult w_thread_join_timeout(w_thread *thread, size_t ms) {
	struct timespec tm;
	tm.tv_sec = ms / 1000;
	tm.tv_nsec = (ms % 1000) * 1000;
	pthread_timedjoin_np(thread->id, NULL, &tm);
	return W_TRUE;
}
wuint w_thread_get_priority(w_thread *thread) {
	return 0;
}
wresult w_thread_set_priority(w_thread *thread, wuint prio) {
	return W_FALSE;
}
//static function
w_thread* w_thread_get_current() {
	return (w_thread*) pthread_getspecific(_pthread_key);
}
w_threadid w_thread_get_current_id() {
	return pthread_self();
}
wresult w_thread_is_current(w_thread *thread) {
	return pthread_equal(pthread_self(), thread->id);
}
void w_thread_yield() {
#ifdef HAVE_SCHED_YIELD
    sched_yield();
#else
	//yield();
#endif
}
void w_thread_sleep(size_t milliseconds) {
	struct timespec ts;
	ts.tv_sec = milliseconds / 1000;
	ts.tv_nsec = (milliseconds % 1000) * 1000000;
	nanosleep(&ts, &ts);
}
int number_of_processors = 0;
size_t w_thread_get_cpu_count() {
	if (number_of_processors == 0)
		number_of_processors = sysconf(_SC_NPROCESSORS_ONLN);
	return number_of_processors;
}
wresult w_thread_set_concurrency(size_t level) {
}
void w_app_init_thread(w_app *app) {
	w_thread_init(&app->mainthread);
	app->mainthread.id = pthread_self();
}
#endif
