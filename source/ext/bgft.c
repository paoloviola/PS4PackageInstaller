#include "ext/bgft.h"

#include <kernel.h>
#include <module.h>

int (*sceBgftServiceInit)(SceBgftInitParams* params);
int (*sceBgftServiceTerm)(void);

int (*sceBgftServiceDownloadFindTaskByContentId)(const char* contentId, SceBgftTaskSubType subType, SceBgftTaskId* taskId);
int (*sceBgftServiceDownloadFindActivePatchTask)(const char* unk, SceBgftTaskId* taskId);
int (*sceBgftServiceDownloadFindActivePupTask)(SceBgftTaskId* taskId);

int (*sceBgftServiceDownloadRegisterTask)(SceBgftDownloadParam* params, SceBgftTaskId* taskId);

int (*sceBgftServiceDownloadStartTask)(SceBgftTaskId taskId);
int (*sceBgftServiceDownloadStartTaskAll)(void);
int (*sceBgftServiceDownloadPauseTask)(SceBgftTaskId taskId);
int (*sceBgftServiceDownloadPauseTaskAll)(void);
int (*sceBgftServiceDownloadResumeTask)(SceBgftTaskId taskId);
int (*sceBgftServiceDownloadResumeTaskAll)(void);
int (*sceBgftServiceDownloadStopTask)(SceBgftTaskId taskId);
int (*sceBgftServiceDownloadStopTaskAll)(void);

int (*sceBgftServiceDownloadGetProgress)(SceBgftTaskId taskId, SceBgftTaskProgress* progress);

int (*sceBgftServiceIntInit)(SceBgftInitParams* params);
int (*sceBgftServiceIntTerm)(void);

int (*sceBgftServiceIntDownloadRegisterTask)(SceBgftDownloadParam* params, SceBgftTaskId* taskId);
int (*sceBgftServiceIntDownloadRegisterTaskByStorageEx)(SceBgftDownloadParamEx* params, SceBgftTaskId* taskId);
int (*sceBgftServiceIntDownloadUnregisterTask)(SceBgftTaskId taskId);
int (*sceBgftServiceIntDownloadReregisterTaskPatch)(SceBgftTaskId oldTaskId, SceBgftTaskId* newTaskId);

int (*sceBgftServiceIntDownloadStartTask)(SceBgftTaskId taskId);
int (*sceBgftServiceIntDownloadStopTask)(SceBgftTaskId taskId);
int (*sceBgftServiceIntDownloadPauseTask)(SceBgftTaskId taskId);
int (*sceBgftServiceIntDownloadResumeTask)(SceBgftTaskId taskId);

int (*sceBgftServiceIntDownloadGetTaskInfo)(SceBgftTaskId taskId, SceBgftDownloadTaskInfo* taskInfo);

int (*sceBgftServiceIntDownloadGetProgress)(SceBgftTaskId taskId, SceBgftTaskProgress* progress);
int (*sceBgftServiceIntDownloadGetPatchProgress)(const char* contentId, SceBgftTaskProgress* progress);
int (*sceBgftServiceIntDownloadGetPlayGoProgress)(const char* contentId, SceBgftTaskProgress* progress);
int (*sceBgftServiceIntDownloadGetGameAndGameAcProgress)(const char* contentId, SceBgftTaskProgress* progress);

int (*sceBgftServiceIntDownloadFindActiveGameAndGameAcTask)(const char* contentId, SceBgftTaskId* taskId);
int (*sceBgftServiceIntDownloadFindActiveTask)(const char* contentId, SceBgftTaskSubType subType, SceBgftTaskId* taskId);

int (*sceBgftServiceIntDebugDownloadRegisterPkg)(SceBgftDownloadParam* params, SceBgftTaskId* taskId);

static int libBgft;
void initBgft(void)
{
  if (libBgft)
  {
    return;
  }

  libBgft = sceKernelLoadStartModule("/system/common/lib/libSceBgft.sprx", 0, 0, 0, NULL, NULL);

  RESOLVE(libBgft, sceBgftServiceInit);
  RESOLVE(libBgft, sceBgftServiceTerm);

  RESOLVE(libBgft, sceBgftServiceDownloadFindTaskByContentId);
  RESOLVE(libBgft, sceBgftServiceDownloadFindActivePatchTask);
  RESOLVE(libBgft, sceBgftServiceDownloadFindActivePupTask);

  RESOLVE(libBgft, sceBgftServiceDownloadRegisterTask);

  RESOLVE(libBgft, sceBgftServiceDownloadStartTask);
  RESOLVE(libBgft, sceBgftServiceDownloadStartTaskAll);
  RESOLVE(libBgft, sceBgftServiceDownloadPauseTask);
  RESOLVE(libBgft, sceBgftServiceDownloadPauseTaskAll);
  RESOLVE(libBgft, sceBgftServiceDownloadResumeTask);
  RESOLVE(libBgft, sceBgftServiceDownloadResumeTaskAll);
  RESOLVE(libBgft, sceBgftServiceDownloadStopTask);
  RESOLVE(libBgft, sceBgftServiceDownloadStopTaskAll);

  RESOLVE(libBgft, sceBgftServiceDownloadGetProgress);

  RESOLVE(libBgft, sceBgftServiceIntInit);
  RESOLVE(libBgft, sceBgftServiceIntTerm);

  RESOLVE(libBgft, sceBgftServiceIntDownloadRegisterTask);
  RESOLVE(libBgft, sceBgftServiceIntDownloadRegisterTaskByStorageEx);
  RESOLVE(libBgft, sceBgftServiceIntDownloadUnregisterTask);
  RESOLVE(libBgft, sceBgftServiceIntDownloadReregisterTaskPatch);

  RESOLVE(libBgft, sceBgftServiceIntDownloadStartTask);
  RESOLVE(libBgft, sceBgftServiceIntDownloadStopTask);
  RESOLVE(libBgft, sceBgftServiceIntDownloadPauseTask);
  RESOLVE(libBgft, sceBgftServiceIntDownloadResumeTask);

  RESOLVE(libBgft, sceBgftServiceIntDownloadGetTaskInfo);

  RESOLVE(libBgft, sceBgftServiceIntDownloadGetProgress);
  RESOLVE(libBgft, sceBgftServiceIntDownloadGetPatchProgress);
  RESOLVE(libBgft, sceBgftServiceIntDownloadGetPlayGoProgress);
  RESOLVE(libBgft, sceBgftServiceIntDownloadGetGameAndGameAcProgress);

  RESOLVE(libBgft, sceBgftServiceIntDownloadFindActiveGameAndGameAcTask);
  RESOLVE(libBgft, sceBgftServiceIntDownloadFindActiveTask);

  RESOLVE(libBgft, sceBgftServiceIntDebugDownloadRegisterPkg);
}
