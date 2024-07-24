
#ifndef HTTP_COMMON_H
#define HTTP_COMMON_H

#include "ext/types_ex.h"

#include <libc.h>
#include <types.h>

#define HTTP_FREE(x) if(x) free(x)

char* httpAllocateSubstring(char* start, char* end);

#endif
