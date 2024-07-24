// https://github.com/flatz/ps4_stub_lib_maker_v2/blob/master/include/bgft.h

#ifndef BGFT_H
#define BGFT_H

#include <types.h>

#define SCE_BGFT_ERROR_SAME_APPLICATION_ALREADY_INSTALLED (0x80990088)

typedef int SceBgftTaskId;

enum _SceBgftTaskSubType {
  SCE_BGFT_TASK_SUB_TYPE_UNKNOWN = 0,
  SCE_BGFT_TASK_SUB_TYPE_PHOTO = 1,
  SCE_BGFT_TASK_SUB_TYPE_MUSIC = 2,
  SCE_BGFT_TASK_SUB_TYPE_VIDEO = 3,
  SCE_BGFT_TASK_SUB_TYPE_MARLIN_VIDEO = 4,
  SCE_BGFT_TASK_SUB_TYPE_UPDATA_ORBIS = 5,
  SCE_BGFT_TASK_SUB_TYPE_GAME = 6,
  SCE_BGFT_TASK_SUB_TYPE_GAME_AC = 7,
  SCE_BGFT_TASK_SUB_TYPE_GAME_PATCH = 8,
  SCE_BGFT_TASK_SUB_TYPE_GAME_LICENSE = 9,
  SCE_BGFT_TASK_SUB_TYPE_SAVE_DATA = 10,
  SCE_BGFT_TASK_SUB_TYPE_CRASH_REPORT = 11,
  SCE_BGFT_TASK_SUB_TYPE_PACKAGE = 12,
  SCE_BGFT_TASK_SUB_TYPE_MAX = 13,
};
typedef enum _SceBgftTaskSubType SceBgftTaskSubType;

enum _SceBgftTaskOpt {
  SCE_BGFT_TASK_OPT_NONE = 0x0,
  SCE_BGFT_TASK_OPT_DELETE_AFTER_UPLOAD = 0x1,
  SCE_BGFT_TASK_OPT_INVISIBLE = 0x2,
  SCE_BGFT_TASK_OPT_ENABLE_PLAYGO = 0x4,
  SCE_BGFT_TASK_OPT_FORCE_UPDATE = 0x8,
  SCE_BGFT_TASK_OPT_REMOTE = 0x10,
  SCE_BGFT_TASK_OPT_COPY_CRASH_REPORT_FILES = 0x20,
  SCE_BGFT_TASK_OPT_DISABLE_INSERT_POPUP = 0x40,
  SCE_BGFT_TASK_OPT_INTERNAL = 0x80, /* ignores release date */
  SCE_BGFT_TASK_OPT_DISABLE_CDN_QUERY_PARAM = 0x10000,
};
typedef enum _SceBgftTaskOpt SceBgftTaskOpt;

struct _SceBgftDownloadRegisterErrorInfo {
  /* TODO */
  uint8_t buf[0x100];
};
typedef struct _SceBgftDownloadRegisterErrorInfo SceBgftDownloadRegisterErrorInfo;

struct _SceBgftDownloadParam {
  int userId;
  int entitlementType;
  const char* id; /* max size = 0x30 */
  const char* contentUrl; /* max size = 0x800 */
  const char* contentExUrl;
  const char* contentName; /* max size = 0x259 */
  const char* iconPath; /* max size = 0x800 */
  const char* skuId;
  SceBgftTaskOpt option;
  const char* playgoScenarioId;
  const char* releaseDate;
  const char* packageType;
  const char* packageSubType;
  unsigned long packageSize;
};
typedef struct _SceBgftDownloadParam SceBgftDownloadParam;

struct _SceBgftDownloadParamEx {
  SceBgftDownloadParam params;
  unsigned int slot;
};
typedef struct _SceBgftDownloadParamEx SceBgftDownloadParamEx;

struct _SceBgftDownloadTaskInfo {
  char* contentTitle;
  char* iconPath;
  unsigned long notificationUtcTick;
};
typedef struct _SceBgftDownloadTaskInfo SceBgftDownloadTaskInfo;

struct _SceBgftTaskProgress {
  unsigned int bits;
  int errorResult;
  unsigned long length;
  unsigned long transferred;
  unsigned long lengthTotal;
  unsigned long transferredTotal;
  unsigned int numIndex;
  unsigned int numTotal;
  unsigned int restSec;
  unsigned int restSecTotal;
  int preparingPercent;
  int localCopyPercent;
};
typedef struct _SceBgftTaskProgress SceBgftTaskProgress;

struct _SceBgftInitParams {
  void* heap;
  size_t heapSize;
};
typedef struct _SceBgftInitParams SceBgftInitParams;

#define SCE_BGFT_INVALID_TASK_ID (-1)

extern int (*sceBgftServiceInit)(SceBgftInitParams* params);
extern int (*sceBgftServiceTerm)(void);

extern int (*sceBgftServiceDownloadFindTaskByContentId)(const char* contentId, SceBgftTaskSubType subType, SceBgftTaskId* taskId);
extern int (*sceBgftServiceDownloadFindActivePatchTask)(const char* unk, SceBgftTaskId* taskId);
extern int (*sceBgftServiceDownloadFindActivePupTask)(SceBgftTaskId* taskId);

extern int (*sceBgftServiceDownloadRegisterTask)(SceBgftDownloadParam* params, SceBgftTaskId* taskId);

extern int (*sceBgftServiceDownloadStartTask)(SceBgftTaskId taskId);
extern int (*sceBgftServiceDownloadStartTaskAll)(void);
extern int (*sceBgftServiceDownloadPauseTask)(SceBgftTaskId taskId);
extern int (*sceBgftServiceDownloadPauseTaskAll)(void);
extern int (*sceBgftServiceDownloadResumeTask)(SceBgftTaskId taskId);
extern int (*sceBgftServiceDownloadResumeTaskAll)(void);
extern int (*sceBgftServiceDownloadStopTask)(SceBgftTaskId taskId);
extern int (*sceBgftServiceDownloadStopTaskAll)(void);

extern int (*sceBgftServiceDownloadGetProgress)(SceBgftTaskId taskId, SceBgftTaskProgress* progress);

extern int (*sceBgftServiceIntInit)(SceBgftInitParams* params);
extern int (*sceBgftServiceIntTerm)(void);

extern int (*sceBgftServiceIntDownloadRegisterTask)(SceBgftDownloadParam* params, SceBgftTaskId* taskId);
extern int (*sceBgftServiceIntDownloadRegisterTaskByStorageEx)(SceBgftDownloadParamEx* params, SceBgftTaskId* taskId);
extern int (*sceBgftServiceIntDownloadUnregisterTask)(SceBgftTaskId taskId);
extern int (*sceBgftServiceIntDownloadReregisterTaskPatch)(SceBgftTaskId oldTaskId, SceBgftTaskId* newTaskId);

extern int (*sceBgftServiceIntDownloadStartTask)(SceBgftTaskId taskId);
extern int (*sceBgftServiceIntDownloadStopTask)(SceBgftTaskId taskId);
extern int (*sceBgftServiceIntDownloadPauseTask)(SceBgftTaskId taskId);
extern int (*sceBgftServiceIntDownloadResumeTask)(SceBgftTaskId taskId);

extern int (*sceBgftServiceIntDownloadGetTaskInfo)(SceBgftTaskId taskId, SceBgftDownloadTaskInfo* taskInfo);

extern int (*sceBgftServiceIntDownloadGetProgress)(SceBgftTaskId taskId, SceBgftTaskProgress* progress);
extern int (*sceBgftServiceIntDownloadGetPatchProgress)(const char* contentId, SceBgftTaskProgress* progress);
extern int (*sceBgftServiceIntDownloadGetPlayGoProgress)(const char* contentId, SceBgftTaskProgress* progress);
extern int (*sceBgftServiceIntDownloadGetGameAndGameAcProgress)(const char* contentId, SceBgftTaskProgress* progress);

extern int (*sceBgftServiceIntDownloadFindActiveGameAndGameAcTask)(const char* contentId, SceBgftTaskId* taskId);
extern int (*sceBgftServiceIntDownloadFindActiveTask)(const char* contentId, SceBgftTaskSubType subType, SceBgftTaskId* taskId);

extern int (*sceBgftServiceIntDebugDownloadRegisterPkg)(SceBgftDownloadParam* params, SceBgftTaskId* taskId);

void initBgft(void);

#endif
