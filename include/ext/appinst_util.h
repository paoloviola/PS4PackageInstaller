// https://github.com/flatz/ps4_stub_lib_maker_v2/blob/master/include/appinst_util.h

#ifndef APPINST_UTIL_H
#define APPINST_UTIL_H

#include <types.h>

enum SceAppInstUtilDelete {
  SCE_APP_INST_UTIL_DELETE_PATCH = (1u << 0),
  SCE_APP_INST_UTIL_DELETE_ADDCONT_EXTRA_DATA = (1u << 1),
  SCE_APP_INST_UTIL_DELETE_TEMP_DATA = (1u << 2),
  SCE_APP_INST_UTIL_DELETE_DOWNLOAD_DATA = (1u << 3),
  SCE_APP_INST_UTIL_DELETE_USERS_SAVE_DATA = (1u << 4),
  SCE_APP_INST_UTIL_DELETE_ALL_USERS_SAVE_DATA = (1u << 5),
};

extern int (*sceAppInstUtilInitialize)(void);
extern int (*sceAppInstUtilTerminate)(void);

extern int (*sceAppInstUtilGetTitleIdFromPkg)(const char* pkgPath, char* titleId, int* isApp);
extern int (*sceAppInstUtilGetPrimaryAppSlot)(const char* titleId, int* slot);

extern int (*sceAppInstUtilAppPrepareOverwritePkg)(const char* pkgPath);

extern int (*sceAppInstUtilAppInstallPkg)(const char* pkgPath, void* reserved);

extern int (*sceAppInstUtilAppUnInstall)(const char* titleId);
extern int (*sceAppInstUtilAppUnInstallByUser)(const char* titleId, int userId);
extern int (*sceAppInstUtilAppUnInstallPat)(const char* titleId);
extern int (*sceAppInstUtilAppUnInstallTypes)(const char* titleId, unsigned int deleteTypes);
extern int (*sceAppInstUtilAppUnInstallAddcont)(const char* titleId, const char* addcontName);
extern int (*sceAppInstUtilAppUnInstallTheme)(const char* contentId);

extern bool (*sceAppInstUtilAppIsInInstalling)(const char* contentId);
extern int (*sceAppInstUtilAppIsInUpdating)(const char* titleId, int* updating);
extern bool (*sceAppInstUtilAppIsInUpdating2)(const char* titleId);
extern int (*sceAppInstUtilAppExists)(const char* titleId, int* exists);
extern int (*sceAppInstUtilAppIsInstalledAddcontExist)(const char* titleId, bool* exists);
extern int (*sceAppInstUtilAppGetSize)(const char* titleId, unsigned long* size);

extern int (*sceAppInstUtilAppRecoverApp)(const char* titleId);

extern int (*sceAppInstUtilGetInstallProgress)(const char* contentId, unsigned int* progress);
extern int (*sceAppInstUtilGetInstallProgressInfo)(const char* contentId, unsigned int* state, unsigned int* progress, unsigned long* progressSize, unsigned long* totalSize, unsigned int* restSec);
extern int (*sceAppInstUtilGetInstallProgressInfo2)(const char* contentId, unsigned int* state, unsigned int* subState, unsigned int* progress, unsigned long* progressSize, unsigned long* totalSize, unsigned int* restSec, char* appVersion);

void initAppInstUtil(void);

#endif
