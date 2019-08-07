#ifndef _TB_THREAD_H_
#define _TB_THREAD_H_

#include "tb_defs.h"

#ifdef _WIN32
	#include <Windows.h>
	#include <process.h> //_beginthreadex()
	typedef HANDLE hthread_t;
	typedef unsigned int (TB_API *FNTHREADFUNC)(void *param);
#else
	#include <pthread.h>
	typedef pthread_t hthread_t;
	typedef void* (*FNTHREADFUNC)(void *param);
#endif

typedef void* (TB_API *FNFUNC1PARAM)(void *param);
typedef void* (TB_API *FNFUNC2PARAM)(void *param1, void *param2);
typedef void* (TB_API *FNFUNC3PARAM)(void *param1, void *param2, void *param3);
typedef void* (TB_API *FNFUNCTMR0PARAM)(tb_uint32 timer_id);
typedef void* (TB_API *FNFUNCTMR1PARAM)(void *param, tb_uint32 timer_id);
typedef void* (TB_API *FNFUNCTMR2PARAM)(void *param1, void *param2, tb_uint32 timer_id);
enum {
	TFN_1PARAM=1, TFN_2PARAM, TFN_3PARAM
};

typedef struct
{
	FNFUNC1PARAM fn;
	void* param1;
	void* param2;
	void* param3;
	hthread_t handle;
	tb_int8 which_fn;
} tb_thread_t;

#ifdef __cplusplus
extern "C" {
#endif

//堆变量初始化，使用完后需调用tb_thread_destroy释放。
tb_thread_t* TB_API tb_thread_create();
tb_int32 TB_API tb_thread_destroy(tb_thread_t* thread);
//栈变量初始化，使用完后需调用tb_thread_uninit释放。
tb_int32 TB_API tb_thread_init(tb_thread_t* thread);
tb_int32 TB_API tb_thread_uninit(tb_thread_t* thread);
//运行线程
tb_int32 TB_API tb_thread_run1(tb_thread_t* thread, FNFUNC1PARAM fn, void *param1);
tb_int32 TB_API tb_thread_run2(tb_thread_t* thread, FNFUNC2PARAM fn, void *param1, void *param2);
tb_int32 TB_API tb_thread_run3(tb_thread_t* thread, FNFUNC3PARAM fn, void *param1, void *param2, void *param3);
//等待线程结束
tb_int32 TB_API tb_thread_join(tb_thread_t* thread);

#ifdef __cplusplus
}
#endif

#endif //_TB_THREAD_H_