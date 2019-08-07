#ifndef _TB_GLOBAL_H_
#define _TB_GLOBAL_H_

#include <stddef.h>

extern void* (*tb_malloc)(size_t size);
extern void  (*tb_free)(void *memblock);
extern void* (*tb_realloc)(void *memblock, size_t size);
extern void* (*tb_calloc)(size_t num, size_t size);

#endif //_TB_GLOBAL_H_