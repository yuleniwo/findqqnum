#ifndef _TB_ATOMIC_H_
#define _TB_ATOMIC_H_

#include "tb_defs.h"

#define ATOMIC_INLINE static inline
#define LG_SIZEOF_INT 2
#if (__SIZEOF_POINTER__ == 8) || defined(_WIN64)
	#define LG_SIZEOF_PTR 3
	#define JEMALLOC_ATOMIC_U64
#else
	#define LG_SIZEOF_PTR 2
#endif

#if (__STDC_VERSION__ >= 201112L) && !defined(__STDC_NO_ATOMICS__)
	#include "atomic_c11.h"
#elif __GNUC__>4 || (__GNUC__==4 && __GNUC_MINOR__>=8)
	#include "atomic_gcc_atomic.h"
#elif __GNUC__==4 && __GNUC_MINOR__>=1
	#include "atomic_gcc_sync.h"
#elif defined(_MSC_VER)
	#include "atomic_msvc.h"
#else
	#error "Don't have atomics implemented on this platform."
#endif

/*
 * This header gives more or less a backport of C11 atomics. The user can write
 * JEMALLOC_GENERATE_ATOMICS(type, short_type, lg_sizeof_type); to generate
 * counterparts of the C11 atomic functions for type, as so:
 *   JEMALLOC_GENERATE_ATOMICS(int *, pi, 3);
 * and then write things like:
 *   int *some_ptr;
 *   atomic_pi_t atomic_ptr_to_int;
 *   atomic_store_pi(&atomic_ptr_to_int, some_ptr, ATOMIC_RELAXED);
 *   int *prev_value = atomic_exchange_pi(&ptr_to_int, NULL, ATOMIC_ACQ_REL);
 *   assert(some_ptr == prev_value);
 * and expect things to work in the obvious way.
 *
 * Also included (with naming differences to avoid conflicts with the standard
 * library):
 *   atomic_fence(atomic_memory_order_t) (mimics C11's atomic_thread_fence).
 *   ATOMIC_INIT (mimics C11's ATOMIC_VAR_INIT).
 */

/*
 * Pure convenience, so that we don't have to type "atomic_memory_order_"
 * quite so often.
 */
#define ATOMIC_RELAXED atomic_memory_order_relaxed
#define ATOMIC_ACQUIRE atomic_memory_order_acquire
#define ATOMIC_RELEASE atomic_memory_order_release
#define ATOMIC_ACQ_REL atomic_memory_order_acq_rel
#define ATOMIC_SEQ_CST atomic_memory_order_seq_cst


JEMALLOC_GENERATE_ATOMICS(void *, p, LG_SIZEOF_PTR)

/*
 * There's no actual guarantee that sizeof(tb_bool) == 1, but it's tb_true on the only
 * platform that actually needs to know the size, MSVC.
 */
//JEMALLOC_GENERATE_ATOMICS(tb_bool, b, 0)
JEMALLOC_GENERATE_INT_ATOMICS(tb_uint8, u8, 0)
JEMALLOC_GENERATE_INT_ATOMICS(tb_uint16, u16, 1)
//JEMALLOC_GENERATE_INT_ATOMICS(size_t, zu, LG_SIZEOF_PTR)
//JEMALLOC_GENERATE_INT_ATOMICS(ssize_t, zd, LG_SIZEOF_PTR)
JEMALLOC_GENERATE_INT_ATOMICS(tb_uint32, u32, 2)

#ifdef JEMALLOC_ATOMIC_U64
JEMALLOC_GENERATE_INT_ATOMICS(tb_uint64, u64, 3)
#endif

#undef ATOMIC_INLINE

#endif // _TB_ATOMIC_H_
