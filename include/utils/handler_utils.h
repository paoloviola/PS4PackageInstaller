
#include "http/http_server.h"
#include "utils/tiny-json.h"

#ifndef HANDLER_UTILS_H
#define HANDLER_UTILS_H

const char* getRequestPropertyString(const json_t* root, const char* key, HttpResponse* response);

int getRequestPropertyInt(const json_t* root, const char* key, HttpResponse* response);

#endif
