
#include "handlers/install_handler.h"
#include "utils/tiny-json.h"

#include <libc.h>
#include <sysutil.h>
#include "ext/bgft.h"

static const char* getJsonPropertyString(const json_t* root, const char* key, HttpResponse* response)
{
  const char* value = json_getPropertyValue(root, key);
  if (value == NULL)
  {
    printf_debug("Wrong request. \"%s\" field was not found!\n", key);
    httpResponseSetCode(response, "400 Bad Request");

    char content[256] = { 0 };
    snprintf(content, sizeof(content), "{\"error\":\"Wrong request. \\\"%s\\\" field was not found!\"}", key);
    httpResponsePrintContent(response, content);
  }
  return value;
}

static int getJsonPropertyInt(const json_t* root, const char* key, HttpResponse* response)
{
  const json_t* property = json_getProperty(root, key);
  if (property == NULL)
  {
    printf_debug("Wrong request. \"%s\" field was not found!\n", key);
    httpResponseSetCode(response, "400 Bad Request");
    
    char content[256] = { 0 };
    snprintf(content, sizeof(content), "{\"error\":\"Wrong request. \\\"%s\\\" field was not found!\"}", key);
    httpResponsePrintContent(response, content);
    return -1;
  }
  return json_getInteger(property);
}

void svcHttpInstallHandler(HttpRequest* request, HttpResponse* response)
{
  char* jsonStr = httpRequestGetContent(request);

  json_t pool[0x100] = { 0 };
  const json_t* root = json_create(jsonStr, pool, sizeof(pool) / sizeof(pool[0]));
  if (root == NULL)
  {
    printf_debug("Could not create json object.\n");
    httpResponseSetCode(response, "500 Internal Server Error");
    httpResponsePrintContent(response, "{\"error\": \"Could not create json object.\"}");
    return;
  }

  const char* contentId = getJsonPropertyString(root, "contentId", response);
  const char* contentUrl = getJsonPropertyString(root, "contentUrl", response);
  const char* contentName = getJsonPropertyString(root, "contentName", response);
  const char* packageType = getJsonPropertyString(root, "packageType", response);
  const char* packageSubType = getJsonPropertyString(root, "packageSubType", response);
  int packageSize = getJsonPropertyInt(root, "packageSize", response);

  if (contentId == NULL || contentUrl == NULL || contentName == NULL 
    || packageType == NULL || packageSubType == NULL || packageSize == -1)
  {
    return;
  }

  printf_debug("Downloading %s with id %s from %s...\n", contentName, contentId, contentUrl);

  // https://www.psdevwiki.com/ps4/Talk:Error_Codes
  SceBgftDownloadParam params = { 0 };

  params.entitlementType = 5;
  params.userId = getUserID();
  params.id = contentId;
  params.contentUrl = contentUrl;
  params.contentName = contentName;
  params.iconPath = "";
  params.playgoScenarioId = "0";
  params.option = SCE_BGFT_TASK_OPT_NONE;
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
  snprintf(content, sizeof(content), "{\"taskId\":\"%d\"}", taskId);
  httpResponsePrintContent(response, content);

  printf_debug("Finished: %s with task id %d\n", contentId, taskId);
}
