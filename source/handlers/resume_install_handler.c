
#include "handlers/stop_install_handler.h"
#include "utils/handler_utils.h"
#include "utils/tiny-json.h"

#include <libc.h>
#include <sysutil.h>
#include "ext/bgft.h"

void svcHttpResumeInstallHandler(HttpRequest* request, HttpResponse* response)
{
  char* jsonStr = httpRequestGetContent(request);

  json_t pool[0x100] = { 0 };
  const json_t* root = json_create(jsonStr, pool, sizeof(pool) / sizeof(pool[0]));
  if (root == NULL)
  {
    printf_debug("Could not create json object.\n");
    httpResponseSetCode(response, "400 Bad Request");
    httpResponsePrintContent(response, "{\"error\": \"Could not create json object.\"}");
    return;
  }

  SceBgftTaskId taskId = getRequestPropertyInt(root, "taskId", response);
  if (taskId == -1)
  {
    return;
  }

  printf_debug("Resuming download with task id %d...\n", taskId);
  sceBgftServiceDownloadResumeTask(taskId);
  
  char content[256] = { 0 };
  snprintf(content, sizeof(content), "{\"taskId\":\"%d\"}", taskId);
  httpResponsePrintContent(response, content);

  printf_debug("Resumed download with task id %d\n", taskId);
}
