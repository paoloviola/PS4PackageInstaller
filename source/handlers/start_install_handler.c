
#include "handlers/start_install_handler.h"
#include "utils/handler_utils.h"
#include "utils/tiny-json.h"

#include <libc.h>
#include <sysutil.h>
#include "ext/bgft.h"

void svcHttpStartInstallHandler(HttpRequest* request, HttpResponse* response)
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

  // Required values
  const char* contentId = getRequiredStringProperty(root, "contentId", response);
  const char* contentUrl = getRequiredStringProperty(root, "contentUrl", response);

  if (contentId == NULL || contentUrl == NULL)
  {
    return;
  }

  // Optional values
  const char* contentName = getOptionalStringProperty(root, "contentName", "");
  const char* iconPath = getOptionalStringProperty(root, "iconPath", "");
  const char* playgoScenarioId = getOptionalStringProperty(root, "playgoScenarioId", "0");
  int64_t option = getOptionalIntProperty(root, "option", 0);
  const char* packageType = getOptionalStringProperty(root, "packageType", "");
  const char* packageSubType = getOptionalStringProperty(root, "packageSubType", "");
  int64_t packageSize = getOptionalIntProperty(root, "packageSize", 0);

  printf_debug("Downloading %s with id %s from %s...\n", contentName, contentId, contentUrl);

  // https://www.psdevwiki.com/ps4/Talk:Error_Codes
  SceBgftDownloadParam params = { 0 };

  params.entitlementType = 5;
  params.userId = getUserID();
  params.id = contentId;
  params.contentUrl = contentUrl;
  params.contentName = contentName;
  params.iconPath = iconPath;
  params.playgoScenarioId = playgoScenarioId;
  params.option = option;
  params.packageType = packageType;
  params.packageSubType = packageSubType;
  params.packageSize = packageSize;

  SceBgftTaskId taskId = SCE_BGFT_INVALID_TASK_ID;
  uint32_t error = sceBgftServiceDownloadRegisterTask(&params, &taskId);

  if (taskId == SCE_BGFT_INVALID_TASK_ID)
  {
    printf_debug("Could not register download task: %u.\n", error);
    httpResponseSetCode(response, "500 Internal Server Error");
    
    char content[256] = { 0 };
    snprintf(content, sizeof(content), "{\"error\": \"Could not register download task: %u\"}", error);
    httpResponsePrintContent(response, content);
    return;
  }

  sceBgftServiceDownloadStartTask(taskId);

  char content[256] = { 0 };
  snprintf(content, sizeof(content), "{\"taskId\": %d}", taskId);
  httpResponsePrintContent(response, content);

  printf_debug("Finished: %s with task id %d\n", contentId, taskId);
}
