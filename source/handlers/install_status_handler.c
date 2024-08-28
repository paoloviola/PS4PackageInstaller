
#include "handlers/install_status_handler.h"
#include "utils/handler_utils.h"
#include "utils/tiny-json.h"

#include <libc.h>
#include <sysutil.h>
#include "ext/bgft.h"

void svcHttpInstallStatusHandler(HttpRequest* request, HttpResponse* response)
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

  SceBgftTaskId taskId = getRequiredIntProperty(root, "taskId", response);
  if (taskId == -1)
  {
    return;
  }

  printf_debug("Get download status of task id %d...\n", taskId);

  SceBgftTaskProgress taskProgress = { 0 };
  uint32_t error = sceBgftServiceDownloadGetProgress(taskId, &taskProgress);
  
  if (error != 0)
  {
    printf_debug("Could not get task progress of task %d: %u.\n", taskId, error);
    httpResponseSetCode(response, "500 Internal Server Error");
    
    char content[256] = { 0 };
    snprintf(content, sizeof(content), "{\"error\": \"Could not get task progress of task %d: %u\"}", taskId, error);
    httpResponsePrintContent(response, content);
    return;
  }

  const char* jsonFormat =
"{\
\"bits\": %d, \
\"errorResult\": %d, \
\"length\": %d, \
\"transferred\": %d, \
\"lengthTotal\": %d, \
\"transferredTotal\": %d, \
\"numIndex\": %d, \
\"numTotal\": %d, \
\"restSec\": %d, \
\"restSecTotal\": %d, \
\"preparingPercent\": %d, \
\"localCopyPercent\": %d\
}";

  char content[256] = { 0 };
  snprintf(content, sizeof(content), jsonFormat, 
    taskProgress.bits,
    taskProgress.errorResult,
    taskProgress.length,
    taskProgress.transferred,
    taskProgress.lengthTotal,
    taskProgress.transferredTotal,
    taskProgress.numIndex,
    taskProgress.numTotal,
    taskProgress.restSec,
    taskProgress.restSecTotal,
    taskProgress.preparingPercent,
    taskProgress.localCopyPercent
  );
  httpResponsePrintContent(response, content);

  printf_debug("Get download status of task id %d: %s\n", taskId, content);
}
