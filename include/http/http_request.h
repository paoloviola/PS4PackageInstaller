
#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include "http_attribute.h"

typedef struct HttpRequest_t
{
  char* method;
  char* path;
  char* version;

  HttpAttributeList attributeList;

  char* rawHeader;
  char* rawContent;
} HttpRequest;

char* httpRequestGetMethod(HttpRequest* request);
char* httpRequestGetPath(HttpRequest* request);
char* httpRequestGetVersion(HttpRequest* request);
char* httpRequestGetContent(HttpRequest* request);

bool httpRequestParseRawHeader(HttpRequest* request, char* rawHeader);
bool httpRequestParseRawContent(HttpRequest* request, char* rawContent);

void httpRequestFreeRaw(HttpRequest* request);

#endif
