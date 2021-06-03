
#include "avcommon.h"
#include "logging/log.h"

extern std::shared_ptr<LogHandler> logHandler;
static CommonLog commonLogger(logHandler,"");

#define FFMPEG_LOG(level) COMMON_LOG(commonLogger, level)

namespace AvLog {

log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("com.foo"));
pthread_key_t Avcommon::key;

void Avcommon::init_ffmpeg_log(int level)
{
    av_log_set_level(level);
    pthread_key_create(&key,NULL);      //创建线程私有数据, 暂时不考虑多线程并发的情况
    av_log_set_callback(&Avcommon::FFMPEG_Callback);
}

void Avcommon::FFMPEG_Callback(void* ptr, int level, const char* fmt, va_list vl)
{
    static int buffersize = 10240;

    char* buffer = (char*)pthread_getspecific(key);
    if (buffer) {

    } else {
        buffer = new char [buffersize];
        buffer[0] = 0;
        buffer[buffersize - 1] = 0;
        pthread_setspecific(key,buffer);
    }

    int bufferLen = strlen(buffer);
    vsnprintf(buffer + bufferLen, 10240 - 2 - bufferLen, fmt, vl);
    bufferLen = strlen(buffer);

    if (buffer[bufferLen - 2] != '\n' &&
        bufferLen < 10240 - 3) {
        return;
    }

    if (buffer[bufferLen - 2] != '\n') {
        buffer[bufferLen - 1] = '>';
    }

    switch(level) {
    case AV_LOG_INFO:
        FFMPEG_LOG(INFO) << buffer;
        break;
    case AV_LOG_WARNING:
        FFMPEG_LOG(WARN) << buffer;
        break;
    case AV_LOG_ERROR:
        FFMPEG_LOG(ERROR) << buffer;
        break;
    case AV_LOG_FATAL:
    case AV_LOG_PANIC:
    case AV_LOG_QUIET:
        FFMPEG_LOG(FATAL) << buffer;
        break;
    case AV_LOG_DEBUG:
    default:
        FFMPEG_LOG(DEBUG) << buffer;
        break;
    }

    //std::cout << level << std::endl;
}
}


