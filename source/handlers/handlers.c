
#include "handlers/handlers.h"
#include "handlers/status_handler.h"
#include "handlers/install_handler.h"
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
  *count = 2;
  HttpHandlerDesc* handlers = (HttpHandlerDesc*)malloc(sizeof(HttpHandlerDesc) * (*count));

  svcHttpHandlerInsert(handlers, 0, "GET", "/", &svcHttpStatusHandler, NULL);
  svcHttpHandlerInsert(handlers, 1, "POST", "/api/install", &svcHttpInstallHandler, NULL);

  return handlers;
}

bool svcHttpHandlersFree(HttpHandlerDesc* handlers)
{
  free(handlers);
  return true;
}
