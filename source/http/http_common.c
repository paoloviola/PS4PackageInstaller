
#include "http/http_common.h"

char* httpAllocateSubstring(char* start, char* end)
{
  int length = end - start;
  char* str = (char*)malloc(length + 1);
  strncpy(str, start, length);
  str[length] = '\0';

  return str;
}
