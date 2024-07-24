
#include "handlers/status_handler.h"

void svcHttpStatusHandler(HttpRequest* request, HttpResponse* response)
{
  UNUSED(request);
  httpResponseSetCode(response, "200 OK");
}
