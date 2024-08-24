
#include "utils/handler_utils.h"

#include <libc.h>
#include <sysutil.h>

const char* getRequestPropertyString(const json_t* root, const char* key, HttpResponse* response)
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

int getRequestPropertyInt(const json_t* root, const char* key, HttpResponse* response)
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

