
#include "http/http_request.h"
#include "http/http_common.h"

char* httpRequestGetMethod(HttpRequest* request)
{
  return request->method;
}

char* httpRequestGetPath(HttpRequest* request)
{
  return request->path;
}

char* httpRequestGetVersion(HttpRequest* request)
{
  return request->version;
}

char* httpRequestGetContent(HttpRequest* request)
{
return request->rawContent;
}

bool httpRequestParseRawHeader(HttpRequest* request, char* rawHeader)
{
  char* firstWhitespace = strchr(rawHeader, ' ');
  if (firstWhitespace == NULL)
  {
    return NULL;
  }

  char* secondWhitespace = strchr((char*)(firstWhitespace + 1), ' ');
  if (secondWhitespace == NULL)
  {
    return NULL;
  }

  char* lineEnd = strstr(rawHeader, "\r\n");
  if (lineEnd == NULL || lineEnd < firstWhitespace || lineEnd < secondWhitespace)
  {
    return NULL;
  }

  char* headerEnd = strstr(rawHeader, "\r\n\r\n");
  if (headerEnd == NULL || headerEnd < lineEnd)
  {
    return NULL;
  }

  request->method = httpAllocateSubstring(rawHeader, firstWhitespace);
  request->path = httpAllocateSubstring((char*)(firstWhitespace + 1), secondWhitespace);
  request->version = httpAllocateSubstring((char*)(secondWhitespace + 1), lineEnd);

  headerEnd[2] = '\0'; // Null terminate to help httpAttributeParseRaw find the end of the header
  httpAttributeParseRaw(&request->attributeList, (char*)(lineEnd + 2));
  headerEnd[2] = '\r'; // Revert null termination after parsing attributes

  request->rawHeader = rawHeader;
  return true;
}

bool httpRequestParseRawContent(HttpRequest* request, char* rawContent)
{
  request->rawContent = rawContent;
  return true;
}

void httpRequestFreeRaw(HttpRequest* request)
{
  HTTP_FREE(request->method);
  HTTP_FREE(request->path);
  HTTP_FREE(request->version);

  httpAttributeFreeRaw(&request->attributeList);
}
