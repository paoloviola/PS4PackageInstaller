
#ifndef HTTP_ATTRIBUTE_H
#define HTTP_ATTRIBUTE_H

#include <types.h>

typedef struct HttpAttribute_t
{
  char* name;
  char* value;
} HttpAttribute;

typedef struct HttpAttributeList_t
{
  int count;
  HttpAttribute* entries;
} HttpAttributeList;

bool httpAttributeSet(HttpAttributeList* attributeList, char* name, char* value);
char* httpAttributeGet(HttpAttributeList* attributeList, char* name);

bool httpAttributeParseRaw(HttpAttributeList* attributeList, char* raw);
void httpAttributeFreeRaw(HttpAttributeList* attributeList);

void httpAttributeFree(HttpAttributeList* attributeList);

#endif
