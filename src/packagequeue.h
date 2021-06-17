#ifndef PacketQueue_H
#define PacketQueue_H

#include <queue>
#include <QWaitCondition>
#include <QMutex>

class AVPacket;

class PacketQueue
{
public:
    PacketQueue();
    ~PacketQueue();
    bool enQueue(const AVPacket& packet);
    AVPacket deQueue();
    uint32_t getPacketSize();
    void queueFlush();

private:
    std::queue<AVPacket> queue;
    uint32_t size;
    QMutex mutex;
    QWaitCondition cond;
};

#endif // PacketQueue_H
