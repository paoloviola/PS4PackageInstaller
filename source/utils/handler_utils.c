
#include "utils/handler_utils.h"

#include <libc.h>
#include <sysutil.h>

const char* getOptionalStringProperty(const json_t* root, const char* key, const char* defaultValue)
{
  const char* value = json_getPropertyValue(root, key);
  return value == NULL ? defaultValue : value;
}

int64_t getOptionalIntProperty(const json_t* root, const char* key, int64_t defaultValue)
{
  const json_t* property = json_getProperty(root, key);
  if (property == NULL)
  {
    return defaultValue;
  }

  return json_getInteger(property);
}

const char* getRequiredStringProperty(const json_t* root, const char* key, HttpResponse* response)
{
  const char* value = getOptionalStringProperty(root, key, NULL);
  if (value == NULL && response != NULL)
  {
    printf_debug("Wrong request. \"%s\" field was not found!\n", key);
    httpResponseSetCode(response, "400 Bad Request");

    char content[256] = { 0 };
    snprintf(content, sizeof(content), "{\"error\":\"Wrong request. \\\"%s\\\" field was not found!\"}", key);
    httpResponsePrintContent(response, content);
  }
  return value;
}

int64_t getRequiredIntProperty(const json_t* root, const char* key, HttpResponse* response)
{
  const int64_t value = getOptionalIntProperty(root, key, -1);
  if (value == -1 && response != NULL)
  {
    printf_debug("Wrong request. \"%s\" field was not found!\n", key);
    httpResponseSetCode(response, "400 Bad Request");
    
    char content[256] = { 0 };
    snprintf(content, sizeof(content), "{\"error\":\"Wrong request. \\\"%s\\\" field was not found!\"}", key);
    httpResponsePrintContent(response, content);
  }
  return value;
}
