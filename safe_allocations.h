#ifndef SAFE_ALLOCATIONS_H
#define SAFE_ALLOCATIONS_H
#include <stdlib.h>

void* safe_realloc(void* a, size_t new_size);

void* safe_malloc(size_t size);
#endif