
#include "http/http_response.h"
#include "http/http_common.h"

char* httpResponseGetCode(HttpResponse* response)
{
  return response->code;
}

void httpResponseSetCode(HttpResponse* response, char* code)
{
  response->code = code;
}

char* httpResponseGetContent(HttpResponse* response)
{
  return response->content;
}

int httpResponsePrintContent(HttpResponse* response, char* content)
{
  int addedContentLength = strlen(content);

  if (response->content == NULL)
  {
    response->content = httpAllocateSubstring(content, (char*)(content + addedContentLength));
    return addedContentLength;
  }

  int oldContentLength = strlen(response->content);

  char* tmpContent = (char*)realloc(response->content, oldContentLength + addedContentLength + 1);
  if (tmpContent == NULL)
  {
    return 0;
  }
  response->content = tmpContent;

  strncpy((char*)(response->content + oldContentLength), content, addedContentLength);
  response->content[oldContentLength + addedContentLength] = '\0';
  
  return addedContentLength;
}

void httpResponseFree(HttpResponse* response)
{
  httpAttributeFree(&response->attributeList);
  HTTP_FREE(response->content);
}
