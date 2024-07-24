
#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "http_request.h"
#include "http_response.h"

typedef void HttpPreHandler(HttpRequest* request, HttpResponse* response);
typedef void HttpPostHandler(HttpRequest* request, HttpResponse* response);

typedef struct HttpHandlerDesc_t
{
  const char* method;
  const char* path;

  HttpPreHandler* preHandler;
  HttpPostHandler* postHandler;
} HttpHandlerDesc;

bool httpServerIsRunning();
unsigned long httpServerGetLastAccessTime();
int httpServerGetStopCode();

int httpServerStart(int port, HttpHandlerDesc* handlers, int handlerCount);
int httpServerStop(int code);

#endif
