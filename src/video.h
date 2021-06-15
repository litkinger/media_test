#include <QThread>
#include "avcommon.h"
#include "framequeue.h"
#include "packagequeue.h"

class Video:public QThread
{
public:
    Video();
    ~Video();
    void run();
    double synchronizeVideo(AVFrame *&srcFrame, double &pts);
    int getStreamIndex();
    AVCodecContext * getAVCodecCotext();
    void enqueuePacket(const AVPacket &pkt);

    AVFrame * dequeueFrame();
    void setStreamIndex(const int &streamIndex);
    int getVideoQueueSize();
    void setVideoStream(AVStream *& stream);
    AVStream * getVideoStream();
    void setAVCodecCotext(AVCodecContext *avCodecContext);
    void setFrameTimer(const double &frameTimer);
    double getFrameTimer();
    void setFrameLastPts(const double &frameLastPts);
    double getFrameLastPts();
    void setFrameLastDelay(const double &frameLastDelay);
    double getFrameLastDelay();
    void setVideoClock(const double &videoClock);
    double getVideoClock();
    int getVideoFrameSiez();
    SwsContext *swsContext = NULL;
    void clearFrames();
    void clearPackets();
private:
    double frameTimer;         // Sync fields
    double frameLastPts;
    double frameLastDelay;
    double videoClock;
    PacketQueue *videoPackets;
    FrameQueue frameQueue;
    AVStream *stream;
    int streamIndex = -1;
    QMutex mutex;
    AVCodecContext *videoContext;
};
