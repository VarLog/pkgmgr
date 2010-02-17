#include <stdlib.h>
#include "utils.h"

void* xcalloc(int nmemb, size_t size) {
  void* ptr = calloc(nmemb, size);
  if(ptr == NULL) 
    exit(EALLOC);
  return ptr;
}    


