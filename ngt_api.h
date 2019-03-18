#ifndef PHP_NGT_API_H
#define PHP_NGT_API_H

#include <string.h>
#include <stdint.h>

#ifdef __cplusplus

#include "ngt.h"
#include "json.hpp"

extern "C" {

#endif /* __cplusplus */

#ifndef NGT_API
#   if defined(_WIN32) || defined(_WIN64)
#       define NGT_API __declspec(dllimport)
#   else
#       define NGT_API extern
#   endif /* defined(_WIN32) || defined(_WIN64) */
#endif /* NGT_API */

#ifndef NGT_VERSION
#    define NGT_VERSION 12
#endif /* NGT_VERSION */

#define NGT_TRUE           (1)
#define NGT_FALSE          (0)

struct _NGTStr {
    size_t len;
    char *buff;
};

typedef void *NgtHandle;
typedef struct _NGTStr *NGTStr;

NGT_API int NgtVersion();
NGT_API int NgtSize();
NGT_API NgtHandle NgtCreate();
NGT_API void NgtFree(NgtHandle handle);
NGT_API void NgtStrFree(NGTStr str);
NGT_API void NgtCreateDB(
    const char* path,
    int dimension,
    int edgeSizeForCreation,
    int edgeSizeForSearch,
    const char* distanceType,
    const char* objectType
);
NGT_API void NgtOpen(NgtHandle handle, const char* path, int rdOnly);
NGT_API void NgtInsert(
    NgtHandle handle, 
    const float* data,
    int objectCount, 
    int numThreads
);
NGT_API NGTStr NgtSearch(
    NgtHandle handle, 
    const float* query, 
    int row,
    float epsilon,
    int edgeSize
);
NGT_API void NgtRemove(NgtHandle handle, int id);
NGT_API NGTStr NgtGetObject(NgtHandle handle, int id);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PHP_NGT_API_H */
