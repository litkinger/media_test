
#ifndef __AUDIO_H_
#define __AUDIO_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <libavformat/avformat.h>

int audioInit(AVCodecContext* pAudioCodecContext);
void audioDestroy();

#ifdef __cplusplus
}
#endif


#endif
