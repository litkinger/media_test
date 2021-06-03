#include "loghandler.h"
#include <log4cxx/logger.h>

namespace sunhi { namespace common { namespace log {

LogHandlerImpl::LogHandlerImpl() {
    std::shared_ptr<LogHandlerChain> header (new Log4cxxHandler(CommonLog::INFO, "com.foo"));
    handlers = header;
    std::shared_ptr<LogHandlerChain> next(new ConsoleLogHandler(CommonLog::INFO));
    header->setNextLogChain(next);
    header = next;
}

void LogHandlerImpl::logMessage(int level, const char* module, const char* msg) {
    this->handlers->logMessage(level, module, msg);
}

void Log4cxxHandler::logMessage(int level, const char* module, const char* msg) {

    std::string module_("");

    if (module && module[0] != 0) {
        module_ += "[";
        module_ += module;
        module_ += "]";
    }

    if (level <= this->level) {
        if (level == CommonLog::DEBUG) {
            LOG4CXX_DEBUG(this->logger, module_ << msg);
        } else if (level == CommonLog::INFO){
            LOG4CXX_INFO(this->logger, module_ << msg);
        } else if (level == CommonLog::WARN){
            LOG4CXX_WARN(this->logger, module_ << msg);
        } else if (level == CommonLog::ERROR) {
            LOG4CXX_ERROR(this->logger, module_ << msg);
        }else if (level == CommonLog::FATAL) {
            LOG4CXX_FATAL(this->logger, module_ << msg);
        }
    }

    if (this->next) {
        this->next->logMessage(level, module, msg);
    }
}

void ConsoleLogHandler::logMessage(int level, const char* module, const char* msg) {
    std::string module_("");

    if (module && module[0] != 0) {
        module_ += "[";
        module_ += module;
        module_ += "]";
    }

    if (level <= this->level) {
        std::cout << module_ << msg  << std::flush;
    }

    if (this->next) {
        this->next->logMessage(level, module, msg);
    }
}

}}}
