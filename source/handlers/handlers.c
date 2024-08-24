
#include "handlers/handlers.h"
#include "handlers/status_handler.h"
#include "handlers/start_install_handler.h"
#include "handlers/pause_install_handler.h"
#include "handlers/resume_install_handler.h"
#include "handlers/stop_install_handler.h"
#include "ext/types_ex.h"

#include <libc.h>

void svcHttpHandlerInsert(HttpHandlerDesc* handlers, int index, char* method, char* path, HttpPreHandler* preHandler, HttpPostHandler* postHandler)
{
  handlers[index].method = method;
  handlers[index].path = path;
  handlers[index].preHandler = preHandler;
  handlers[index].postHandler = postHandler;
}

HttpHandlerDesc* svcHttpHandlersAlloc(int* count) 
{
  *count = 5;
  HttpHandlerDesc* handlers = (HttpHandlerDesc*)malloc(sizeof(HttpHandlerDesc) * (*count));

  svcHttpHandlerInsert(handlers, 0, "GET", "/", &svcHttpStatusHandler, NULL);
  svcHttpHandlerInsert(handlers, 1, "POST", "/api/start_install", &svcHttpStartInstallHandler, NULL);
  svcHttpHandlerInsert(handlers, 2, "POST", "/api/pause_install", &svcHttpPauseInstallHandler, NULL);
  svcHttpHandlerInsert(handlers, 3, "POST", "/api/resume_install", &svcHttpResumeInstallHandler, NULL);
  svcHttpHandlerInsert(handlers, 4, "POST", "/api/stop_install", &svcHttpStopInstallHandler, NULL);

  return handlers;
}

bool svcHttpHandlersFree(HttpHandlerDesc* handlers)
{
  free(handlers);
  return true;
}
