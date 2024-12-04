#include <stdlib.h>

void* safe_realloc(void* a, size_t new_size) {
  void* allocated_memory = realloc(a, new_size);
  if (allocated_memory == NULL) {
    exit(1);
  }
  return allocated_memory;
}

void* safe_malloc(size_t size) {
  void* allocated_memory = malloc(size);
  if (allocated_memory == NULL) {
    exit(1);
  }
  return allocated_memory;
}



