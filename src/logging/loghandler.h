#ifndef LOGHANDLER_H
#define LOGHANDLER_H

#include "log.h"

//TODO 以后需要分到不同的模块

namespace sunhi { namespace common { namespace log {

class LogHandlerChain : private std::enable_shared_from_this<LogHandlerChain> {//interface log handler
protected:
    std::shared_ptr<LogHandlerChain> next;

public:
    virtual ~LogHandlerChain() {}

    void setNextLogChain(std::shared_ptr<LogHandlerChain> next) {
        this->next = next;
    }

    virtual void logMessage(int level, const char* module, const char* msg) = 0;
};

class LogHandlerImpl : public LogHandler
{
public:
    LogHandlerImpl();

    virtual void logMessage(int level, const char* module, const char* msg);
private:
    std::shared_ptr<LogHandlerChain> handlers;
};

class Log4cxxHandler : public LogHandlerChain {
public:
    Log4cxxHandler(int level, const char* loggerName) :
        level(level),
        logger(log4cxx::Logger::getLogger(loggerName)){}

    void logMessage(int level, const char* module, const char* msg);

private:
    int level;
    log4cxx::LoggerPtr logger;
};

class ConsoleLogHandler : public LogHandlerChain {
public:
    ConsoleLogHandler(int level) :
        level(level) {}

    virtual void logMessage(int level, const char* module, const char* msg);
private:
    int level;
};

}}}

#endif // LOGHANDLER_H
