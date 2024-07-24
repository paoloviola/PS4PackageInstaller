#include "ext/libc_ex.h"

#include <kernel.h>
#include <module.h>

long long int (*strtoll)(const char* str, char** endptr, int base);
double (*strtod)(const char* str, char** endptr);
int (*isxdigit)(int arg);

static int libC;
void initLibcEx(void)
{
  if (libC)
  {
    return;
  }

  libC = sceKernelLoadStartModule("libSceLibcInternal.sprx", 0, 0, 0, NULL, NULL);

  RESOLVE(libC, strtoll);
  RESOLVE(libC, strtod);
  RESOLVE(libC, isxdigit);
}
