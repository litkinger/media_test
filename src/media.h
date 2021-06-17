#ifndef MEDIA_H
#define MEDIA_H

#include <QMutex>
#include "avcommon.h"
class Audio;
class Video;

class Media
{
public:
    static Media *getInstance() {
            static Media media;
            return &media;
    }
    ~Media();
    Media * config();
    Media * setMediaFile(const char*filename);
    bool checkMediaSizeValid();
    int getVideoStreamIndex();
    int getAudioStreamIndex();
    void enqueueVideoPacket(const AVPacket &packet);
    void enqueueAudioPacket(const AVPacket &packet);
    void startAudioPlay();
    AVFormatContext *getAVFormatContext();
    Audio *audio;
    Video *video;
    void close();
    int totalMs = 0;
    int pts = 0;
private:
    AVFormatContext *pFormatCtx;
    Media();
    const char *filename;
    QMutex mutex;
};

#endif // MEDIA_H
