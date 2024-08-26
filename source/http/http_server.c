
#include "http/http_server.h"
#include "http/http_common.h"

#include <kernel.h>
#include <network.h>

bool serverIsRunning = false;
unsigned long serverLastAccessTime = 0;
int serverStopCode = 0;

int serverHandle = 0;

char* httpServerReceiveHeaderRaw(int socket)
{
  int currentLength = 1;
  char* buffer = (char*)malloc(currentLength);

  do
  {
    if (sceNetRecv(socket, (char*)(buffer + currentLength - 1), 1, 0) <= 0)
    {
      free(buffer);
      return NULL;
    }
    currentLength++;

    char* tmpBuffer = (char*)realloc(buffer, currentLength);
    if (tmpBuffer == NULL)
    {
      free(buffer);
      return NULL;
    }
    buffer = tmpBuffer;
  } while (strncmp((char*)(buffer + currentLength - 5), "\r\n\r\n", 4) != 0);

  buffer[currentLength - 1] = '\0';
  return buffer;
}

char* httpServerReceiveContentRaw(int socket, HttpRequest* request)
{
  char* contentLengthStr = httpAttributeGet(&request->attributeList, "Content-Length");
  if (contentLengthStr == NULL)
  {
    return NULL;
  }

  int contentLength = atoi(contentLengthStr);
  char* buffer = (char*)malloc(contentLength + 1);
  int totalLength = 0;

  do
  {
    totalLength += sceNetRecv(socket, (char*)(buffer + totalLength), contentLength - totalLength, 0);
  } while (totalLength < contentLength);

  buffer[contentLength] = '\0';
  return buffer;
}

HttpHandlerDesc* httpServerFindHandlerFor(HttpRequest* request, HttpHandlerDesc* handlers, int handlerCount)
{
  for (int i = 0; i < handlerCount; i++)
  {
    if (strcmp(request->method, handlers[i].method) == 0
      && strcmp(request->path, handlers[i].path) == 0)
    {
      return &handlers[i];
    }
  }

  return NULL;
}

void httpServerSendResponse(int socket, HttpResponse* response)
{
  sceNetSend(socket, (char*)"HTTP/1.1 ", 9, 0);
  sceNetSend(socket, (char*)response->code, strlen(response->code), 0);
  sceNetSend(socket, (char*)"\r\n", 2, 0);

  httpAttributeSet(&response->attributeList, "Access-Control-Allow-Origin", "*");

  for (int i = 0; i < response->attributeList.count; i++)
  {
    HttpAttribute entry = response->attributeList.entries[i];

    sceNetSend(socket, (char*)entry.name, strlen(entry.name), 0);
    sceNetSend(socket, (char*)": ", 2, 0);
    sceNetSend(socket, (char*)entry.value, strlen(entry.value), 0);
    sceNetSend(socket, (char*)"\r\n", 2, 0);
  }

  sceNetSend(socket, (char*)"\r\n", 2, 0);

  if (response->content != NULL)
  {
    sceNetSend(socket, (char*)response->content, strlen(response->content), 0);
  }
}

bool httpServerIsRunning()
{
  return serverIsRunning;
}

unsigned long httpServerGetLastAccessTime()
{
  return serverLastAccessTime;
}

int httpServerGetStopCode()
{
  return serverStopCode;
}

int httpServerStart(int port, HttpHandlerDesc* handlers, int handlerCount)
{
  serverHandle = sceNetSocket("httpServer", AF_INET, SOCK_STREAM, 0);
  if (serverHandle == -1)
  {
    serverHandle = 0;
    return -1;
  }

  struct sockaddr_in serverAddress = { 0 };
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = sceNetHtons(port);
  serverAddress.sin_addr.s_addr = sceNetHtonl(IN_ADDR_ANY);

  if (sceNetBind(serverHandle, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) != 0)
  {
    sceNetSocketClose(serverHandle);
    serverHandle = 0;
    return -2;
  }

  if (sceNetListen(serverHandle, 10) != 0)
  {
    sceNetSocketClose(serverHandle);
    serverHandle = 0;
    return -3;
  }

  serverIsRunning = true;
  while (httpServerIsRunning())
  {
    serverLastAccessTime = sceKernelGetProcessTime();

    struct sockaddr_in clientAddress = { 0 };
    int clientAddressLength = sizeof(clientAddress);
    int clientHandle = sceNetAccept(serverHandle, (struct sockaddr*)&clientAddress, (socklen_t*)&clientAddressLength);
    if (clientHandle < 0)
    {
      continue;
    }

    char* rawHeader = httpServerReceiveHeaderRaw(clientHandle);
    if (rawHeader == NULL)
    {
      sceNetSocketClose(clientHandle);
      continue;
    }

    HttpRequest request = { 0 };
    if (!httpRequestParseRawHeader(&request, rawHeader))
    {
      sceNetSocketClose(clientHandle);

      HTTP_FREE(rawHeader);
      continue;
    }

    char* rawContent = httpServerReceiveContentRaw(clientHandle, &request);
    if (rawContent != NULL && !httpRequestParseRawContent(&request, rawContent))
    {
      sceNetSocketClose(clientHandle);
      httpRequestFreeRaw(&request);

      HTTP_FREE(rawHeader);
      HTTP_FREE(rawContent);
      continue;
    }

    HttpResponse response = { 0 };
    HttpHandlerDesc* handler = httpServerFindHandlerFor(&request, handlers, handlerCount);

    if (handler == NULL)
    {
      httpResponseSetCode(&response, "404 Not Found");
      httpServerSendResponse(clientHandle, &response);
    }
    else
    {
      httpResponseSetCode(&response, "200 OK");

      if (handler->preHandler != NULL)
      {
        (*handler->preHandler)(&request, &response);
      }

      httpServerSendResponse(clientHandle, &response);

      if (handler->postHandler != NULL)
      {
        (*handler->postHandler)(&request, &response);
      }
    }

    sceNetSocketClose(clientHandle);
    httpRequestFreeRaw(&request);
    httpResponseFree(&response);

    HTTP_FREE(rawHeader);
    HTTP_FREE(rawContent);
  }

  sceNetSocketClose(serverHandle);
  serverHandle = 0;

  return httpServerGetStopCode();
}

int httpServerStop(int code)
{
  if (!httpServerIsRunning())
  {
    return httpServerGetStopCode();
  }

  serverIsRunning = false;
  serverStopCode = code;

  sceNetSocketAbort(serverHandle, 0);
  sceNetSocketClose(serverHandle);
  serverHandle = 0;

  return httpServerGetStopCode();
}
