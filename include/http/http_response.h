
#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include "http_attribute.h"

typedef struct HttpResponse_t
{
  char* code;

  HttpAttributeList attributeList;

  char* content;
} HttpResponse;

char* httpResponseGetCode(HttpResponse* response);
void httpResponseSetCode(HttpResponse* response, char* code);

char* httpResponseGetContent(HttpResponse* response);
int httpResponsePrintContent(HttpResponse* response, char* content);

void httpResponseFree(HttpResponse* response);

#endif
