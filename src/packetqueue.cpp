#include "packetqueue.h"

static bool isExit = false;

PacketQueue::PacketQueue()
{
    this->size = 0;
}

PacketQueue::~PacketQueue()
{
    QMutexLocker locker(&mutex);
    isExit = true;
}

bool PacketQueue::enQueue(const AVPacket packet)
{
    QMutexLocker locker(&mutex);
    queue.push(packet);
    size += packet.size;
    cond.wakeOne();
    return true;
}

AVPacket PacketQueue::deQueue()
{
    bool ret = false;
    AVPacket pkt;
    QMutexLocker locker(&mutex);
    while (true)
    {
        if (!queue.empty())
        {
            pkt = queue.front();
            queue.pop();
            size -= pkt.size;
            ret = true;
            break;
        }
        else
        {
            cond.wait(&mutex);
        }
    }
    return pkt;
}

uint32_t PacketQueue::getPacketSize()
{
    QMutexLocker locker(&mutex);
    return size;
}

void PacketQueue::queueFlush() {
     while (!queue.empty())
     {
         AVPacket pkt = deQueue();
         av_free_packet(&pkt);
     }
}
