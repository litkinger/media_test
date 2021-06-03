

#ifndef __AV_COMMON_H_
#define __AV_COMMON_H_

#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>

#ifdef _WIN32
//Windows
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "libavutil/log.h"
#include "SDL2/SDL.h"
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/log.h>
#include <SDL2/SDL.h>
#ifdef __cplusplus
};
#endif
#endif


namespace AvLog {

class Avcommon {
public:
    static void init_ffmpeg_log(int level = AV_LOG_INFO);

private:
    static void FFMPEG_Callback(void* ptr, int level, const char* fmt, va_list vl);
    Avcommon();
    Avcommon(const Avcommon &avcommon);
    Avcommon& operator = (const Avcommon &avcommon);
    static pthread_key_t key;
};
}

#endif
