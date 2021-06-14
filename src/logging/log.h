#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <log4cxx/logger.h>
#include <ostream>
#include <mutex>
#include <stack>
#include <atomic>

namespace sunhi {
namespace common {
namespace log {

class LogHandler {//interface log handler
public:
    virtual ~LogHandler() {}
    virtual void logMessage(int level, const char* module, const char* msg) = 0;
};

class CommonLog {
public:
    enum {
        FATAL,
        ERR,
        WARN,
        INFO,
        DEBUG
    };

    class LogRecord {
    public:
        LogRecord(CommonLog &log, int level);
        LogRecord(const LogRecord &rhs);
        LogRecord &operator=(const LogRecord &rhs);
        ~LogRecord();

        const std::shared_ptr<LogHandler> &handler();

        std::ostream &stream();

    private:
        class LogStreamEntity :
                public std::streambuf,
                public std::enable_shared_from_this<LogStreamEntity>  {
        public:
            LogStreamEntity();
            ~LogStreamEntity();

            static std::shared_ptr<LogStreamEntity> get(const std::shared_ptr<LogHandler> handler);

            std::ostream &stream();

            void ref();
            void unref();
            void setLogLevel(int level);
            void setModule(const std::string& module);

        private:
            char buf[10240];
            void release();

            int level;
            std::string module;

            std::weak_ptr<LogHandler> whandler;
            std::atomic<int> _ref;
            std::iostream ostream;

            class LogStreamCache {
            public:
                LogStreamCache();
                ~LogStreamCache();

                std::shared_ptr<LogStreamEntity> get();
                void put(std::shared_ptr<LogStreamEntity> entity);

            private:
                std::mutex mutex;
                std::stack<std::shared_ptr<LogStreamEntity>> cache;
            };

            static LogStreamCache globalCache;
        };

        std::shared_ptr<LogStreamEntity> logEntity;
    };

    CommonLog(const std::shared_ptr<LogHandler>& logHandler, const char* module);

    CommonLog(const std::shared_ptr<LogHandler>& logHandler, const std::string& module);

    LogRecord record(int level);

    int getLogLevel() const;
    void setLogLevel(int level);

    const std::string &getModule() const;

    const std::shared_ptr<LogHandler>& handler();

    CommonLog(const CommonLog& log) = delete;
    CommonLog &operator=(const CommonLog& log) = delete ;
    virtual ~CommonLog() {}

private:
    std::shared_ptr<LogHandler> _handler;
    std::string module;

    volatile int LOG_LEVEL;
};

}}}

using namespace sunhi::common::log;

#define COMMON_LOG(log, level) \
    if (log.getLogLevel() >= CommonLog::level ) log.record(CommonLog::level).stream()

#endif // LOG_H
