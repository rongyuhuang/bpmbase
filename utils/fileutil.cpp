#include "fileutil.h"

#define BPM_USE_FILESYSTEM

#if !defined(BPM_USE_FILESYSTEM)

#include <stdlib.h> // free
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#include <io.h>
#else
#include <stdarg.h>
#include <sys/stat.h>
#endif

#ifdef _WIN32
#define ACCESS _access
#define MKDIR(a) _mkdir((a))
#else
#define ACCESS access
#define MKDIR(a) mkdir((a), 0755)
#endif

int createDir(const char* pDir)
{
    int i = 0;
    int iRet;
    int iLen;
    char* pszDir;

    if (NULL == pDir) {
        return 0;
    }

    pszDir = strdup(pDir);
    iLen = strlen(pszDir);

    // 创建中间目录
    for (i = 0; i < iLen; i++) {
        if (pszDir[i] == '\\' || pszDir[i] == '/') {
            pszDir[i] = '\0';

            //如果不存在,创建
            iRet = ACCESS(pszDir, 0);
            if (iRet != 0) {
                iRet = MKDIR(pszDir);
                if (iRet != 0) {
                    return -1;
                }
            }
            //支持linux,将所有\换成/
            pszDir[i] = '/';
        }
    }

    iRet = MKDIR(pszDir);
    free(pszDir);
    return iRet;
}

#else

#include "utils/stdfs.h"

int bpm_createDir(const char *pDir){
    stdfs::path path(pDir);
    return stdfs::create_directories(path);
}
#endif
