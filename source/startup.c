
#include "startup.h"
#include "ext/types_ex.h"

#include <ps4.h>
#include "ext/bgft.h"
#include "ext/appinst_util.h"
#include "ext/libc_ex.h"

static bool appInstUtilInitialized;
static bool bgftInitialized;

static SceBgftInitParams bgftInitParams;

void resolveLibraries(void) 
{
  initKernel();
  initLibc();
  initLibcEx();
  initNetwork();
  initPthread();

  jailbreak();
  mmap_patch();

  initSysUtil();

  // Load libraries which need jailbreak
  initAppInstUtil();
  initBgft();
}

bool svcInitializeService(void)
{
  resolveLibraries();

  printf_debug("Starting service...\n");
  if (sceAppInstUtilInitialize()) 
  {
    printf_debug("Failed to initialize appinst_util!\n");
    return false;
  }
  appInstUtilInitialized = true;

  memset(&bgftInitParams, 0, sizeof(bgftInitParams));
  {
    bgftInitParams.heapSize = 1024 * 1024;
    bgftInitParams.heap = malloc(bgftInitParams.heapSize);
    memset(bgftInitParams.heap, 0, bgftInitParams.heapSize);
  }

  if (sceBgftServiceInit(&bgftInitParams)) 
  {
    printf_debug("Failed to initialize bgft!\n");
    return false;
  }
  bgftInitialized = true;
  return true;
}

bool svcTerminateService(void) 
{
  printf_debug("Terminating service...\n");

  if (appInstUtilInitialized) 
  {
    sceAppInstUtilTerminate();
    appInstUtilInitialized = false;
  }

  if (bgftInitialized) 
  {
    sceBgftServiceTerm();
    free(bgftInitParams.heap);
    bgftInitialized = false;
  }

  return true;
}
