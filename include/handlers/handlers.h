
#ifndef HANDLERS_H
#define HANDLERS_H

#include "http/http_server.h"

HttpHandlerDesc* svcHttpHandlersAlloc(int* count);
bool svcHttpHandlersFree(HttpHandlerDesc* handlers);

#endif
