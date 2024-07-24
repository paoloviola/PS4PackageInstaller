
#include "http/http_attribute.h"
#include "http/http_common.h"

static HttpAttribute* httpAttributeGetEntry(HttpAttributeList* attributeList, char* name)
{
  for (int i = 0; i < attributeList->count; i++)
  {
    if (strcmp(attributeList->entries[i].name, name) == 0)
    {
      return &attributeList->entries[i];
    }
  }

  return NULL;
}

bool httpAttributeSet(HttpAttributeList* attributeList, char* name, char* value)
{
  HttpAttribute* entry = httpAttributeGetEntry(attributeList, name);
  if (entry != NULL)
  {
    entry->value = value;
    return true;
  }

  attributeList->count++;

  HttpAttribute* tmpEntries = (HttpAttribute*)realloc(attributeList->entries, sizeof(HttpAttribute) * attributeList->count);
  if (tmpEntries == NULL)
  {
    return false;
  }
  attributeList->entries = tmpEntries;

  int index = attributeList->count - 1;
  attributeList->entries[index].name = name;
  attributeList->entries[index].value = value;
  return true;
}

char* httpAttributeGet(HttpAttributeList* attributeList, char* name)
{
  HttpAttribute* entry = httpAttributeGetEntry(attributeList, name);
  return (entry == NULL) ? NULL : entry->value;
}

bool httpAttributeParseRaw(HttpAttributeList* attributeList, char* raw)
{
  char* currentLineStart = raw;

  do
  {
    char* currentLineEnd = strstr(currentLineStart, "\r\n");
    if (currentLineEnd == NULL)
    {
      return true;
    }

    char* currentColon = strstr(currentLineStart, ": ");
    if (currentColon == NULL || currentLineEnd < currentColon)
    {
      httpAttributeFreeRaw(attributeList);
      return false;
    }

    char* name = httpAllocateSubstring(currentLineStart, currentColon);
    char* value = httpAllocateSubstring((char*)(currentColon + 2), currentLineEnd);

    httpAttributeSet(attributeList, name, value);

    currentLineStart = (char*)(currentLineEnd + 2);
  } while (1);
}

void httpAttributeFreeRaw(HttpAttributeList* attributeList)
{
  for (int i = 0; i < attributeList->count; i++)
  {
    HTTP_FREE(attributeList->entries[i].name);
    HTTP_FREE(attributeList->entries[i].value);
  }

  httpAttributeFree(attributeList);
}

void httpAttributeFree(HttpAttributeList* attributeList)
{
  HTTP_FREE(attributeList->entries);
}
