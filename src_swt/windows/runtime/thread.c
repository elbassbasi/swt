/*
 * thread.c
 *
 *  Created on: 20 août 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include <swt.h>
#include <windows.h>
#include "tlhelp32.h"
typedef struct _w_mutex {
	HANDLE mutex;
	w_mutex_type type;
	DWORD owningThread;
} _w_mutex;
wresult w_mutex_create(w_mutex *mutex, w_mutex_type type) {
	((_w_mutex*) mutex)->mutex = CreateMutexW(
	NULL,       // default secutiry attributes
			FALSE,      // not initially locked
			NULL        // no name
			);
	if (((_w_mutex*) mutex)->mutex != 0) {
		((_w_mutex*) mutex)->type = type;
		((_w_mutex*) mutex)->owningThread = 0;
		return W_TRUE;
	} else {
		return W_FALSE;
	}
}
void w_mutex_dispose(w_mutex *mutex) {
	if (((_w_mutex*) mutex)->mutex != 0) {
		CloseHandle(((_w_mutex*) mutex)->mutex);
	}
}
wresult w_mutex_lock_timeout(w_mutex *mutex, size_t ms) {
	if (((_w_mutex*) mutex)->type == W_MUTEX_DEFAULT) {
		// Don't allow recursive
		if (((_w_mutex*) mutex)->owningThread != 0) {
			if (((_w_mutex*) mutex)->owningThread == GetCurrentThreadId())
				return W_ERROR_MUTEX_DEAD_LOCK;
		}
	}

	DWORD rc = WaitForSingleObject(((_w_mutex*) mutex)->mutex, ms);
	switch (rc) {
	case WAIT_ABANDONED:
		// the previous caller died without releasing the mutex, so even
		// though we did get it, log a message about this
		//wxLogDebug(wxT("WaitForSingleObject() returned WAIT_ABANDONED"));
		//wxFALLTHROUGH;

	case WAIT_OBJECT_0:
		// ok
		break;

	case WAIT_TIMEOUT:
		return W_ERROR_MUTEX_TIMEOUT;

	default:
		//wxFAIL_MSG(wxT("impossible return value in wxMutex::Lock"));
		//wxFALLTHROUGH;

	case WAIT_FAILED:
		//wxLogLastError(wxT("WaitForSingleObject(mutex)"));
		return W_ERROR_MUTEX_MISC_ERROR;
	}
	if (((_w_mutex*) mutex)->type == W_MUTEX_DEFAULT) {
		// required for checking recursiveness
		((_w_mutex*) mutex)->owningThread = GetCurrentThreadId();
	}
	return W_TRUE;
}
wresult w_mutex_lock(w_mutex *mutex) {
	return w_mutex_lock_timeout(mutex, INFINITE);
}

wresult w_mutex_try_lock(w_mutex *mutex) {
	wresult rc = w_mutex_lock_timeout(mutex, 0);
	// we have a special return code for timeout in this case
	return rc == W_ERROR_MUTEX_TIMEOUT ? W_ERROR_MUTEX_BUSY : rc;
}
wresult w_mutex_unlock(w_mutex *mutex) {
	((_w_mutex*) mutex)->owningThread = 0;

	if (!ReleaseMutex(((_w_mutex*) mutex)->mutex)) {
		//wxLogLastError(wxT("ReleaseMutex()"));
		return W_ERROR_MUTEX_MISC_ERROR;
	}
	return W_TRUE;
}
typedef struct _w_condition {
	w_mutex *mutex;
	LONG numWaiters;
	HANDLE semaphore;
} _w_condition;
wresult w_condition_create(w_condition *condition, w_mutex *mutex) {
	((_w_condition*) condition)->mutex = mutex;
	((_w_condition*) condition)->numWaiters = 0;
	((_w_condition*) condition)->semaphore = CreateSemaphoreW(
	NULL,           // default security attributes
			0, INT_MAX,
			NULL            // no name
			);
	if (((_w_condition*) condition)->semaphore) {
		return W_TRUE;
	} else {
		return W_FALSE;
	}
}
void w_condition_dispose(w_condition *condition) {
	if (((_w_condition*) condition)->semaphore) {
		CloseHandle(((_w_condition*) condition)->semaphore);
	}
}

wresult w_condition_wait_timeout(w_condition *condition, size_t milliseconds) {
	InterlockedIncrement(&((_w_condition*) condition)->numWaiters);
	w_mutex_unlock(((_w_condition*) condition)->mutex);
	DWORD rc = WaitForSingleObject(((_w_condition*) condition)->semaphore,
			milliseconds);
	if (rc == WAIT_TIMEOUT) {
		DWORD rc = WaitForSingleObject(((_w_condition*) condition)->semaphore,
				0);
		if (rc != WAIT_OBJECT_0) {
			InterlockedDecrement(&((_w_condition*) condition)->numWaiters);
		}
	}
	w_mutex_lock(((_w_condition*) condition)->mutex);
	// return err == wxSEMA_NO_ERROR ? wxCOND_NO_ERROR : wxCOND_MISC_ERROR;
	return W_TRUE;
}
wresult w_condition_wait(w_condition *condition) {
	return w_condition_wait_timeout(condition, INFINITE);
}
wresult w_condition_signal(w_condition *condition) {
	if (((_w_condition*) condition)->numWaiters > 0) {
		if (!ReleaseSemaphore(((_w_condition*) condition)->semaphore, 1,
				&((_w_condition*) condition)->numWaiters))
			//return wxCOND_MISC_ERROR;
			return W_FALSE;
		InterlockedDecrement(&((_w_condition*) condition)->numWaiters);
		//m_numWaiters--;
	}

	return W_TRUE;
}
wresult w_condition_broadcast(w_condition *condition) {
	/* wxCriticalSectionLocker lock(m_csWaiters);

	 while ( m_numWaiters > 0 )
	 {
	 if ( m_semaphore.Post() != wxSEMA_NO_ERROR )
	 return wxCOND_MISC_ERROR;

	 m_numWaiters--;
	 }

	 return wxCOND_NO_ERROR;*/
	//if (((_w_condition*) condition)->numWaiters > 0) {
	if (!ReleaseSemaphore(((_w_condition*) condition)->semaphore,
			((_w_condition*) condition)->numWaiters,
			&((_w_condition*) condition)->numWaiters))
		//return wxCOND_MISC_ERROR;
		return W_FALSE;
	((_w_condition*) condition)->numWaiters = 0;
	//m_numWaiters--;
	//}
	return W_TRUE;
}
typedef struct _w_semaphore {
	HANDLE semaphore;
} _w_semaphore;
wresult w_semaphore_create(w_semaphore *semaphore, int initialcount,
		int maxcount) {
	if (maxcount == 0) {
		// make it practically infinite
		maxcount = INT_MAX;
	}
	((_w_semaphore*) semaphore)->semaphore = CreateSemaphoreW(
	NULL,           // default security attributes
			initialcount, maxcount,
			NULL            // no name
			);
	if (!((_w_semaphore*) semaphore)->semaphore) {
		return W_FALSE;
	} else
		return W_TRUE;
}
void w_semaphore_dispose(w_semaphore *semaphore) {
	if (((_w_semaphore*) semaphore)->semaphore) {
		CloseHandle(((_w_semaphore*) semaphore)->semaphore);
	}
}
wresult w_semaphore_wait_timeout(w_semaphore *semaphore, size_t milliseconds) {
	DWORD rc = WaitForSingleObject(((_w_semaphore*) semaphore)->semaphore,
			milliseconds);

	switch (rc) {
	case WAIT_OBJECT_0:
		return W_TRUE;

		//case WAIT_TIMEOUT:
		//return wxSEMA_TIMEOUT;

		//default:
		//wxLogLastError(wxT("WaitForSingleObject(semaphore)"));
	}
	return W_FALSE;
	//return wxSEMA_MISC_ERROR;
}
wresult w_semaphore_wait(w_semaphore *semaphore) {
	return w_semaphore_wait_timeout(semaphore, INFINITE);
}
wresult w_semaphore_try_wait(w_semaphore *semaphore) {
	wresult rc = w_semaphore_wait_timeout(semaphore, 0);
	/*if ( rc == wxSEMA_TIMEOUT )
	 rc = wxSEMA_BUSY;*/

	return rc;

}
wresult w_semaphore_post(w_semaphore *semaphore) {
	if (!ReleaseSemaphore(((_w_semaphore*) semaphore)->semaphore, 1,
	NULL)) {
		if (GetLastError() == ERROR_TOO_MANY_POSTS) {
			// return wxSEMA_OVERFLOW;
		} else {
			//wxLogLastError(wxT("ReleaseSemaphore"));
			// return wxSEMA_MISC_ERROR;
		}
		return W_FALSE;
	}

	return W_TRUE;
}
void w_thread_init(w_thread *thread) {
	/*((_w_thread*) thread)->start_proc = 0;
	((_w_thread*) thread)->args = 0;
	((_w_thread*) thread)->handle = INVALID_HANDLE_VALUE;
	((_w_thread*) thread)->threadId = 0;*/
}
wresult w_thread_equal(w_thread *thread1, w_thread *thread2) {
	//return ((_w_thread*) thread1)->threadId == ((_w_thread*) thread2)->threadId;
}
void w_thread_dispose(w_thread *thread) {

}
DWORD dwTlsIndex = TLS_OUT_OF_INDEXES;
DWORD WINAPI w_thread_start_function(LPVOID lpThreadParameter) {
	/*_w_thread *thread = (_w_thread*) lpThreadParameter;
	TlsSetValue(dwTlsIndex, thread);
	DWORD ret = thread->start_proc(thread->args);
	TlsSetValue(dwTlsIndex, 0);
	return ret;*/
}
wresult w_thread_create(w_thread *thread, w_thread_start start_proc,
		void *args, size_t stackSize) {
	/*if (dwTlsIndex == TLS_OUT_OF_INDEXES) {
		dwTlsIndex = TlsAlloc();
	}
	((_w_thread*) thread)->args = args;
	((_w_thread*) thread)->start_proc = start_proc;
	((_w_thread*) thread)->cleanup = 0;
	((_w_thread*) thread)->handle = CreateThread(NULL, stackSize,
			(LPTHREAD_START_ROUTINE) w_thread_start_function, thread, 0,
			&((_w_thread*) thread)->threadId);
	if (((_w_thread*) thread)->handle == INVALID_HANDLE_VALUE)
		return W_ERROR_NO_HANDLES;
	else {
		return W_TRUE;
	}*/
}
wresult w_thread_delete(w_thread *thread, int *exitcode) {
	/*DWORD exit = 0;
	TerminateThread(((_w_thread*) thread)->handle, exit);
	if (exitcode != 0)
		*exitcode = exit;*/
	return W_TRUE;
}
void _w_thread_cleanup_all(w_thread *thread) {
	/*_w_thread_cleanup *cleanup = ((_w_thread*) thread)->cleanup;
	while (cleanup != 0) {
		cleanup->routine(cleanup->prev);
		cleanup = cleanup->prev;
	}*/
}
void _w_thread_cancel_callback() {
	/*w_thread *thread = (w_thread*) TlsGetValue(dwTlsIndex);
	if (thread != 0) {
		_w_thread_cleanup_all(thread);
	}
	((_w_thread*) thread)->threadId = 0;
	((_w_thread*) thread)->handle = INVALID_HANDLE_VALUE;
	ExitThread(1);*/
}
DWORD _w_thread_register_cancelation(HANDLE threadH) {
	CONTEXT context;
	context.ContextFlags = CONTEXT_CONTROL;
	GetThreadContext(threadH, &context);
	context.Rip = (DWORD_PTR) _w_thread_cancel_callback;
	SetThreadContext(threadH, &context);
	return 0;
}
wresult w_thread_cancel(w_thread *thread) {
	/*if (GetCurrentThreadId() == ((_w_thread*) thread)->threadId)
		return W_ERROR_FAILED_EXEC;
	HANDLE threadH = ((_w_thread*) thread)->handle;
	SuspendThread(threadH);
	if (WaitForSingleObject(threadH, 0) == WAIT_TIMEOUT) {
		_w_thread_register_cancelation(((_w_thread*) thread)->handle);
		ResumeThread(threadH);
		return W_TRUE;
	} else {
		return W_FALSE;
	}*/
}
wresult _w_thread_wait1_0(w_thread *thread, HANDLE *handles, size_t count,
		DWORD dwMilliseconds) {
	/*size_t j;
	for (size_t i = 0; i < count; i++) {
		handles[j++] = ((_w_thread*) (&thread[i]))->handle;
		if (handles[j] == INVALID_HANDLE_VALUE)
			j--;
	}
	if (j == 0)
		return W_ERROR_NO_HANDLES;
	DWORD result = WaitForMultipleObjects(count, (HANDLE*) handles, TRUE,
			dwMilliseconds);
	if (result == WAIT_FAILED)
		return W_FALSE;*/
	return W_TRUE;
}
wresult _w_thread_wait1_1(w_thread *thread, size_t count, DWORD dwMilliseconds) {
	HANDLE h[count];
	return _w_thread_wait1_0(thread, h, count, dwMilliseconds);
}
wresult w_thread_wait1(w_thread *thread, size_t count) {
	//return w_thread_wait1_timeout(thread, count, INFINITE);
}
wresult w_thread_wait1_timeout(w_thread *thread, size_t count,
		size_t milliseconds) {
	HANDLE *handles;
	if (count < 0x50) {
		return _w_thread_wait1_1(thread, count, milliseconds);
	} else {
		handles = malloc(count * sizeof(HANDLE));
		if (handles == 0)
			return W_ERROR_NO_MEMORY;
		wresult ret = _w_thread_wait1_0(thread, handles, count, milliseconds);
		free(handles);
		return ret;
	}
}
wresult _w_thread_wait0_0(w_thread **thread, HANDLE *handles, size_t count,
		DWORD dwMilliseconds) {
	/*size_t j = 0;
	for (size_t i = 0; i < count; i++) {
		if (thread[i] != 0) {
			handles[j++] = ((_w_thread*) (thread[i]))->handle;
			if (handles[j] == INVALID_HANDLE_VALUE)
				j--;
		}
	}
	if (j == 0)
		return W_ERROR_NO_HANDLES;
	DWORD result = WaitForMultipleObjects(j, (HANDLE*) handles, TRUE,
			dwMilliseconds);
	if (result == WAIT_FAILED)
		return W_FALSE;*/
	return W_TRUE;
}
wresult _w_thread_wait0_1(w_thread **thread, size_t count,
		DWORD dwMilliseconds) {
	HANDLE h[count];
	return _w_thread_wait0_0(thread, h, count, dwMilliseconds);
}
wresult w_thread_wait0(w_thread **thread, size_t count) {
	//return w_thread_wait0_timeout(thread, count, INFINITE);
}
wresult w_thread_wait0_timeout(w_thread **thread, size_t count,
		size_t milliseconds) {
	HANDLE *handles;
	if (count < 0x50) {
		return _w_thread_wait0_1(thread, count, milliseconds);
	} else {
		handles = malloc(count * sizeof(HANDLE));
		if (handles == 0)
			return W_ERROR_NO_MEMORY;
		wresult ret = _w_thread_wait0_0(thread, handles, count, milliseconds);
		free(handles);
		return ret;
	}
}
wuint w_thread_get_priority(w_thread *thread) {
	//return GetThreadPriority(((_w_thread*) thread)->handle);
}
wresult w_thread_set_priority(w_thread *thread, wuint prio) {
	//SetThreadPriority(((_w_thread*) thread)->handle, prio);
	return W_TRUE;
}
//static function
w_thread* w_thread_get_current() {
	return (w_thread*) TlsGetValue(dwTlsIndex);
}
wresult w_thread_is_current(w_thread *thread) {
	return GetCurrentThreadId() == thread->id;
}
w_threadid w_thread_get_current_id() {
	return GetCurrentThreadId();
}
void w_thread_yield() {
	Sleep(0);
	//Yield();
}
void w_thread_sleep(size_t milliseconds) {
	Sleep(milliseconds);
}
size_t w_thread_get_cpu_count() {
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return info.dwNumberOfProcessors;
}
wresult w_thread_set_concurrency(size_t level) {
	// ok only for the default one
	if (level == 0)
		return 0;
	// get system affinity mask first
	HANDLE hProcess = GetCurrentProcess();
	DWORD_PTR dwProcMask, dwSysMask;
	if (GetProcessAffinityMask(hProcess, &dwProcMask, &dwSysMask) == 0) {
		//wxLogLastError(wxT("GetProcessAffinityMask"));
		return W_FALSE;
	}
	// how many CPUs have we got?
	if (dwSysMask == 1) {
		// don't bother with all this complicated stuff - on a single
		// processor system it doesn't make much sense anyhow
		return level == 1;
	}
	// calculate the process mask: it's a bit vector with one bit per
	// processor; we want to schedule the process to run on first level
	// CPUs
	DWORD bit = 1;
	while (bit) {
		if (dwSysMask & bit) {
			// ok, we can set this bit
			dwProcMask |= bit;
			// another process added
			if (--level == 0) {
				// and that's enough
				break;
			}
		}
		// next bit
		bit <<= 1;
	}
	// could we set all bits?
	if (level != 0) {
		//wxLogDebug(wxT("bad level %u in wxThread::SetConcurrency()"), level);
		return W_FALSE;
	}
	if (SetProcessAffinityMask(hProcess, dwProcMask) == 0) {
		// wxLogLastError(wxT("SetProcessAffinityMask"));
		return W_FALSE;
	}
	return W_TRUE;
}
#endif
