#ifndef FRAMEQUEUE_H
#define FRAMEQUEUE_H

#include <queue>
#include <QMutex>
#include <QWaitCondition>
#include "avcommon.h"

class FrameQueue
{
public:
    static const int capacity = 30;
    FrameQueue();
    bool enQueue(const AVFrame* frame);
    AVFrame * deQueue();
    int getQueueSize();
    void queueFlush();
private:
    std::queue<AVFrame*> queue;
    QMutex mutex;
    QWaitCondition cond;
};

#endif // FRAMEQUEUE_H
