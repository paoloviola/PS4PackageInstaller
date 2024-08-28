
#include "http/http_server.h"
#include "utils/tiny-json.h"

#ifndef HANDLER_UTILS_H
#define HANDLER_UTILS_H

const char* getRequiredStringProperty(const json_t* root, const char* key, HttpResponse* response);

int64_t getRequiredIntProperty(const json_t* root, const char* key, HttpResponse* response);

const char* getOptionalStringProperty(const json_t* root, const char* key, const char* defaultValue);

int64_t getOptionalIntProperty(const json_t* root, const char* key, int64_t defaultValue);

#endif
