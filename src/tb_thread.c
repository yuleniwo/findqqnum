#include "tb_global.h"
#include "tb_thread.h"

//线程库

static void* TB_API exec_thread(void *param)
{
	tb_thread_t* thread = (tb_thread_t*)param;
	if(TFN_2PARAM == thread->which_fn)
		return ((FNFUNC2PARAM)thread->fn)(thread->param1, thread->param2);
	else
		return ((FNFUNC3PARAM)thread->fn)(thread->param1, thread->param2, thread->param3);
}

tb_thread_t* TB_API tb_thread_create()
{
	tb_thread_t* thread;
	thread = (tb_thread_t*)tb_malloc(sizeof(tb_thread_t));
	if(thread != NULL && tb_thread_init(thread) != 0)
		thread = NULL;
	return thread;
}

tb_int32 TB_API tb_thread_destroy(tb_thread_t* thread)
{
	tb_int32 ret = -1;
	if(thread != NULL)
	{
		ret = tb_thread_uninit(thread);
		tb_free(thread);
	}
	return ret;
}

tb_int32 TB_API tb_thread_init(tb_thread_t* thread)
{
	thread->handle = (hthread_t)NULL;
	return 0;
}

tb_int32 TB_API tb_thread_uninit(tb_thread_t* thread)
{
#ifdef _WIN32
	tb_int32 ret = -1;
	if((hthread_t)NULL != thread->handle)
	{
		CloseHandle(thread->handle);
		thread->handle = NULL;
		ret = 0;
	}
	return ret;
#else
	thread->handle = (hthread_t)NULL;
	return 0;
#endif
}

tb_int32 TB_API tb_thread_run1(tb_thread_t* thread, FNFUNC1PARAM fn, void *param1)
{
	tb_int32 ret = 0;
	if((hthread_t)NULL == thread->handle)
	{
#ifdef _WIN32
		thread->handle = (hthread_t)_beginthreadex(NULL, 0, (FNTHREADFUNC)fn, param1, 0, NULL);
#else
		if(pthread_create(&thread->handle, NULL, (FNTHREADFUNC)fn, param1) != 0)
			thread->handle = (hthread_t)NULL;
#endif
		ret = thread->handle != (hthread_t)NULL ? 0 : -1;
	}
	return ret;
}

tb_int32 TB_API tb_thread_run2(tb_thread_t* thread, FNFUNC2PARAM fn, void *param1, void *param2)
{
	tb_int32 ret = 0;
	if((hthread_t)NULL == thread->handle)
	{
		thread->fn = (FNFUNC1PARAM)fn;
		thread->param1 = param1;
		thread->param2 = param2;
		thread->which_fn = TFN_2PARAM;
		ret = tb_thread_run1(thread, &exec_thread, thread);
	}
	return ret;
}

tb_int32 TB_API tb_thread_run3(tb_thread_t* thread, FNFUNC3PARAM fn, void *param1, void *param2, void *param3)
{
	tb_int32 ret = 0;
	if((hthread_t)NULL == thread->handle)
	{
		thread->fn = (FNFUNC1PARAM)fn;
		thread->param1 = param1;
		thread->param2 = param2;
		thread->param3 = param3;
		thread->which_fn = TFN_3PARAM;
		ret = tb_thread_run1(thread, &exec_thread, thread);
	}
	return ret;
}

tb_int32 TB_API tb_thread_join(tb_thread_t* thread)
{
	tb_int32 ret = 0;
	if(thread->handle != (hthread_t)NULL)
	{
#ifdef _WIN32
		WaitForSingleObject(thread->handle, INFINITE);
		ret = CloseHandle(thread->handle) ? 0 : -1;
#else
		ret = pthread_join(thread->handle, NULL);
#endif
		thread->handle = (hthread_t)NULL;
	}
	return ret;
}
