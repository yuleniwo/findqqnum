#include <stdlib.h>
#include "tb_global.h"

void* (*tb_malloc)(size_t size) = malloc;
void  (*tb_free)(void *memblock) = free;
void* (*tb_realloc)(void *memblock, size_t size) = realloc;
void* (*tb_calloc)(size_t num, size_t size) = calloc;