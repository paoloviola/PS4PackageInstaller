
#include <ps4.h>

#include "startup.h"
#include "handlers/handlers.h"
#include "http/http_server.h"

#define SERVER_PORT 51067
#define MAX_IDLE_SECONDS 60 * 1

#define TICKS_PER_SECOND 1000000

void* svcHttpServerThread(void* arg)
{
  int handlerCount;
  HttpHandlerDesc* handlers = svcHttpHandlersAlloc(&handlerCount);

  int* exitCode = (int*)arg;
  *exitCode = httpServerStart(SERVER_PORT, handlers, handlerCount);
  printf_debug("Server exited with code %d\n", *exitCode);

  svcHttpHandlersFree(handlers);
  scePthreadExit(NULL);
  return NULL;
}

int _main(struct thread* thread) 
{
  UNUSED(thread);

  if (!svcInitializeService()) 
  {
    printf_debug("Failed to start service!\n");
    printf_notification("Failed to start service!");
    svcTerminateService();
    return 1;
  }

  int exitCode = 0;

  ScePthread serverThread;
  scePthreadCreate(&serverThread, NULL, svcHttpServerThread, &exitCode, "svcHttpServerThread");

  do
  {
    sceKernelSleep(10);

    unsigned long elapsedSeconds = (sceKernelGetProcessTime() - httpServerGetLastAccessTime()) / TICKS_PER_SECOND;
    if (elapsedSeconds > MAX_IDLE_SECONDS) 
    {
      httpServerStop(0);

      printf_debug("Stopped service because of idleness!\n");
      printf_notification("Stopped service because of idleness!");
    }
  }
  while (httpServerIsRunning());

  scePthreadJoin(serverThread, NULL);

  svcTerminateService();
  return exitCode;
}