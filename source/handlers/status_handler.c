
#include "handlers/status_handler.h"

void svcHttpStatusHandler(HttpRequest* request, HttpResponse* response)
{
  UNUSED(request);
  httpResponsePrintContent(response, "{\"health\": \"OK\"}");
}
