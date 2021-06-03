#include "log.h"
#include <iostream>
#include <exception>

namespace sunhi { namespace common { namespace log {

CommonLog::CommonLog(const std::shared_ptr<LogHandler>& logHandler, const char* module) :
    _handler(logHandler),
    module(module),
    LOG_LEVEL(DEBUG)
{}

CommonLog::CommonLog(const std::shared_ptr<LogHandler>& logHandler, const std::string& module) :
    _handler(logHandler),
    module(module),
    LOG_LEVEL(DEBUG)
{}

const std::shared_ptr<LogHandler> &CommonLog::handler() {
    return _handler;
}

int CommonLog::getLogLevel() const {
    return CommonLog::LOG_LEVEL;
}

void CommonLog::setLogLevel(int level) {
    CommonLog::LOG_LEVEL = level;
}

const std::string& CommonLog::getModule() const {
    return module;
}

CommonLog::LogRecord CommonLog::record(int level) {
    return CommonLog::LogRecord(*this, level);
}

CommonLog::LogRecord::LogRecord(CommonLog &log, int level) :
    logEntity(CommonLog::LogRecord::LogStreamEntity::get(log.handler()))
{
    if (logEntity) {
        logEntity->setLogLevel(level);
        logEntity->setModule(log.module);
    }
}

CommonLog::LogRecord::LogRecord(const CommonLog::LogRecord &rhs)
{
    if (rhs.logEntity) {
        rhs.logEntity->ref();
    }
    this->logEntity = rhs.logEntity;
}

CommonLog::LogRecord &CommonLog::LogRecord::operator=(const CommonLog::LogRecord &rLogRecord)
{
    if (logEntity) {
        logEntity->unref();
    }

    if (rLogRecord.logEntity) {
        rLogRecord.logEntity->ref();
    }
    logEntity = rLogRecord.logEntity;
    return *this;
}

CommonLog::LogRecord::~LogRecord() {
    if (logEntity) {
        logEntity->unref();
    }
}

std::ostream &CommonLog::LogRecord::stream()
{
    if (logEntity)
        return logEntity->stream();
    return std::cout;
}


CommonLog::LogRecord::LogStreamEntity::LogStreamEntity() :
    level(INFO),
    module(256, '\n'),
    _ref(1),
    ostream(this) {
    setp(buf, buf + sizeof(buf) - 1);
}

std::shared_ptr<CommonLog::LogRecord::LogStreamEntity>
CommonLog::LogRecord::LogStreamEntity::get(const std::shared_ptr<LogHandler> handler)
{
    std::shared_ptr<CommonLog::LogRecord::LogStreamEntity> entity = globalCache.get();
    if (entity) {
        entity->whandler = handler;
    }
    return entity;
}

void CommonLog::LogRecord::LogStreamEntity::ref() {
    ++ _ref;
}

void CommonLog::LogRecord::LogStreamEntity::unref() {
    if (-- _ref == 0) {
        release();
    }
}

void CommonLog::LogRecord::LogStreamEntity::setLogLevel(int level) {
    this->level = level;
}

void CommonLog::LogRecord::LogStreamEntity::setModule(const std::string& module)
{
    this->module = module;
}

CommonLog::LogRecord::LogStreamEntity::~LogStreamEntity() {
}

int CommonLog::LogRecord::LogStreamEntity::overflow(int c) {
    std::shared_ptr<LogHandler> shandler = whandler.lock();

    *pptr() = '\0';

    if (shandler) {
        shandler->logMessage(level, module.c_str(), pbase());
    }

    int nbytes = pptr() - pbase();
    pbump(-nbytes);

    if (c == EOF)
        return EOF;

    *pptr() = c;
    pbump(1);

    return 0;
}

int CommonLog::LogRecord::LogStreamEntity::sync()
{
    return 0;
}

void CommonLog::LogRecord::LogStreamEntity::release()
{
    std::shared_ptr<LogHandler> shandler = whandler.lock();

    int nbytes = pptr() - pbase();

    if (nbytes > 0)
    {
        *pptr() = '\0';
        if (shandler) {
            shandler->logMessage(level, module.c_str(), pbase());
        }
        pbump(-nbytes);
    }

    try {
        std::shared_ptr<CommonLog::LogRecord::LogStreamEntity> thisS = shared_from_this();
        globalCache.put(thisS);
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

std::ostream &CommonLog::LogRecord::LogStreamEntity::stream() {
    return ostream;
}

CommonLog::LogRecord::LogStreamEntity::LogStreamCache CommonLog::LogRecord::LogStreamEntity::globalCache;

CommonLog::LogRecord::LogStreamEntity::LogStreamCache::LogStreamCache()
{
}

CommonLog::LogRecord::LogStreamEntity::LogStreamCache::~LogStreamCache() {
    while(!cache.empty()) {
        std::shared_ptr<LogStreamEntity> entity = cache.top();
        cache.pop();
    }
}

std::shared_ptr<CommonLog::LogRecord::LogStreamEntity> CommonLog::LogRecord::LogStreamEntity::LogStreamCache::get() {

    std::lock_guard<std::mutex> lock(mutex);

    if (!cache.empty()) {
        std::shared_ptr<CommonLog::LogRecord::LogStreamEntity> entity = cache.top();
        cache.pop();
        entity->ref();
        return entity;
    } else {
        std::shared_ptr<CommonLog::LogRecord::LogStreamEntity> sp(new CommonLog::LogRecord::LogStreamEntity());
        return sp;
    }
}

void CommonLog::LogRecord::LogStreamEntity::LogStreamCache::put(std::shared_ptr<CommonLog::LogRecord::LogStreamEntity> entity) {
    std::lock_guard<std::mutex> lock(mutex);
    cache.push(entity);
}

}}}
