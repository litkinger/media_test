
#ifndef __AUDIO_H_
#define __AUDIO_H_

#include "avcommon.h"
#include "packetqueue.h"

class Audio{
public:
    Audio();
    ~Audio();
    const uint32_t BUFFERSIZE = 192000;
    bool audioPlay();
    bool audioClose();//音频关闭
    double getCurrentAudioClock();//获取当前音频时钟
    int getStreamIndex();
    void setStreamIndex(const int streamIndex);
    int getAudioQueueSize();
    void enqueuePacket(const AVPacket pkt);
    AVPacket dequeuePacket();
    uint8_t* getAudioBuff();
    void setAudioBuff(uint8_t*& audioBuff);
    uint32_t getAudioBuffSize();
    void setAudioBuffSize(uint32_t audioBuffSize);
    uint32_t getAudioBuffIndex();
    void setAudioBuffIndex(uint32_t audioBuffIndex);
    double getAudioClock();
    void setAudioClock(const double &audioClock);
    AVStream *getStream();
    void setStream(AVStream *&stream);
    AVCodecContext *getAVCodecContext();//获取音频解码器上下文
    void setAVCodecContext(AVCodecContext *audioContext);
    bool getIsPlaying();
    void setPlaying(bool isPlaying);
    void clearPacket();
    void setVolume(int volume);

private:

    AVCodecContext *audioContext;//音频解码器上下文
    AVStream *stream;   //音频流
    double audioClock; // audio clock
    PacketQueue audiaPackets;
    uint8_t *audioBuff;       // 解码后数据的缓冲空间
    uint32_t audioBuffSize;  // buffer中的字节数
    uint32_t audioBuffIndex; // buffer中未发送数据的index
    int streamIndex = -1;
    bool isPlay = false;
};

void audioCallback(void* userdata, Uint8 *stream, int len);
int audioDecodeFrame(Audio*audio, uint8_t *audioBuffer, int bufferSize);

#endif
