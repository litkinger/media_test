/**
 * 最简单的基于FFmpeg的视频播放器2(SDL升级版)
 * Simplest FFmpeg Player 2(SDL Update)
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 第2版使用SDL2.0取代了第一版中的SDL1.2
 * Version 2 use SDL 2.0 instead of SDL 1.2 in version 1.
 *
 * 本程序实现了视频文件的解码和显示(支持HEVC，H.264，MPEG2等)。
 * 是最简单的FFmpeg视频解码方面的教程。
 * 通过学习本例子可以了解FFmpeg的解码流程。
 * 本版本中使用SDL消息机制刷新视频画面。
 * This software is a simplest video player based on FFmpeg.
 * Suitable for beginner of FFmpeg.
 *
 * 备注:
 * 标准版在播放视频的时候，画面显示使用延时40ms的方式。这么做有两个后果：
 * （1）SDL弹出的窗口无法移动，一直显示是忙碌状态
 * （2）画面显示并不是严格的40ms一帧，因为还没有考虑解码的时间。
 * SU（SDL Update）版在视频解码的过程中，不再使用延时40ms的方式，而是创建了
 * 一个线程，每隔40ms发送一个自定义的消息，告知主函数进行解码显示。这样做之后：
 * （1）SDL弹出的窗口可以移动了
 * （2）画面显示是严格的40ms一帧
 * Remark:
 * Standard Version use's SDL_Delay() to control video's frame rate, it has 2
 * disadvantages:
 * (1)SDL's Screen can't be moved and always "Busy".
 * (2)Frame rate can't be accurate because it doesn't consider the time consumed 
 * by avcodec_decode_video2()
 * SU（SDL Update）Version solved 2 problems above. It create a thread to send SDL 
 * Event every 40ms to tell the main loop to decode and show video frames.
 */
 
#include <stdio.h>
#include <iostream>
#include <QApplication>

#define WEBRTC_WIN 1

#include "logging/log.h"
#include "logging/loghandler.h"
#include "api/scoped_refptr.h"
#include "api/ref_counted_base.h"
 
#define __STDC_CONSTANT_MACROS

#include "audio.h"
#include "video.h"
#include "avcommon.h"
 
//Refresh Event
#define SFM_REFRESH_EVENT  (SDL_USEREVENT + 1)
 
#define SFM_BREAK_EVENT  (SDL_USEREVENT + 2)
 
int thread_exit=0;
int thread_pause=0;
 
int sfp_refresh_thread(void *opaque) {

    thread_exit=0;
    thread_pause=0;
 
    while (!thread_exit) {
        if(!thread_pause){
            SDL_Event event;
            event.type = SFM_REFRESH_EVENT;
            SDL_PushEvent(&event);
        }
        SDL_Delay(40);
    }
    thread_exit=0;
    thread_pause=0;

    //Break
    SDL_Event event;
    event.type = SFM_BREAK_EVENT;
    SDL_PushEvent(&event);
 
    return 0;
} 

extern AVInputFormat  ff_h264_demuxer;
std::shared_ptr<LogHandler> logHandler(new LogHandlerImpl());
static CommonLog commonLogger(logHandler,"main");

#define MAIN_LOG(level) COMMON_LOG(commonLogger, level)
 
#undef main
int main(int argc, char* argv[])
{
    AVFormatContext *pFormatCtx;
    AVCodecContext  *pVideoCodecCtx, *pAudioCodecCtx;
    AVCodec         *pVideoCodec, *pAudioCodec;
    AVFrame *pFrame,*pFrameYUV, *pFrameAudio;
    AVPacket *packet;
    unsigned char *out_buffer;
    int ret, got_picture;
    int i, videoindex, audioindex;
 
    //------------SDL----------------
    int screen_w,screen_h;
    SDL_Window *screen; 
    SDL_Renderer* sdlRenderer;
    SDL_Texture* sdlTexture;
    SDL_Rect sdlRect;
    SDL_Thread *video_tid;
    SDL_Event event;
 
    struct SwsContext *img_convert_ctx;
#if _WINDOWS
    char filepath[]="kfc.mp4";
#else
    char filepath[]="/Users/yaoping.zheng/Documents/mediademo/kfc.mp4";
#endif

    MAIN_LOG(INFO) << "zyptest " << std::endl;

    MAIN_LOG(WARN) <<  "starting...." << (const char*)argv[0] << std::endl;

    av_register_all();
    avcodec_register_all();

    AvLog::Avcommon::init_ffmpeg_log();

    //avformat_network_init();

    pFormatCtx = avformat_alloc_context();
 
    if(avformat_open_input(&pFormatCtx,filepath,NULL,NULL)!=0){
        MAIN_LOG(INFO) <<  "Couldn't open input stream" << std::endl;
        return -1;
    }
    if(avformat_find_stream_info(pFormatCtx,NULL)<0){
        MAIN_LOG(INFO) <<  "Couldn't find stream information" << std::endl;
        return -1;
    }
    videoindex=-1;
    audioindex = -1;
    for(i=0; i<pFormatCtx->nb_streams; i++) 
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO && videoindex == -1){
            videoindex=i;
        } else if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioindex = i;
        }
    if(videoindex==-1){
        MAIN_LOG(INFO) <<  "Didn't find a video stream" << std::endl;
        return -1;
    }
    if(audioindex == -1) {
        MAIN_LOG(INFO) <<  "Didn't find a audio stream.\n";
        return -1;
    }
    pVideoCodecCtx=pFormatCtx->streams[videoindex]->codec;
    pVideoCodec=avcodec_find_decoder(pVideoCodecCtx->codec_id);
    if(pVideoCodec==NULL){
        MAIN_LOG(INFO) <<  "Codec not found.\n";
        return -1;
    }

    if(avcodec_open2(pVideoCodecCtx, pVideoCodec,NULL)<0) {
        MAIN_LOG(INFO) <<  "Could not open codec.\n";
        return -1;
    }

    pAudioCodecCtx = pFormatCtx->streams[audioindex]->codec;
    pAudioCodec = avcodec_find_decoder(pAudioCodecCtx->codec_id);
    if (!pAudioCodec) {
        MAIN_LOG(INFO) <<  "audio codec not found code id = " << pAudioCodecCtx->codec_id << std::endl;
        return -1;
    }

    if(avcodec_open2(pAudioCodecCtx, pAudioCodec,NULL)<0) {
        MAIN_LOG(INFO) <<  "Could not open codec.\n";
        return -1;
    }

    pFrame=av_frame_alloc();
    pFrameAudio=av_frame_alloc();
    pFrameYUV=av_frame_alloc();
 
    out_buffer=(unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P,  pVideoCodecCtx->width, pVideoCodecCtx->height,1));
    av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize,out_buffer,
        AV_PIX_FMT_YUV420P,pVideoCodecCtx->width, pVideoCodecCtx->height,1);
 
    //Output Info-----------------------------
    MAIN_LOG(INFO) <<  "---------------- File Information ---------------\n";
    av_dump_format(pFormatCtx,0,filepath,0);
    MAIN_LOG(INFO) <<  "-------------------------------------------------\n";
    
    img_convert_ctx = sws_getContext(pVideoCodecCtx->width, pVideoCodecCtx->height, pVideoCodecCtx->pix_fmt,
        pVideoCodecCtx->width, pVideoCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
    
 
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {  
        MAIN_LOG(INFO) <<   "Could not initialize SDL, error=" << SDL_GetError() << std::endl;
        return -1;
    } 
    //SDL 2.0 Support for multiple windows
    screen_w = pVideoCodecCtx->width;
    screen_h = pVideoCodecCtx->height;
    screen = SDL_CreateWindow("Simplest ffmpeg player's Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        screen_w, screen_h,SDL_WINDOW_OPENGL);
 
    if(!screen) {  
        MAIN_LOG(INFO) <<  "SDL: could not create window - exiting:" << SDL_GetError() << std::endl;
        return -1;
    }
    sdlRenderer = SDL_CreateRenderer(screen, -1, 0);  
    //IYUV: Y + U + V  (3 planes)
    //YV12: Y + V + U  (3 planes)
    sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,pVideoCodecCtx->width,pVideoCodecCtx->height);
 
    sdlRect.x=0;
    sdlRect.y=0;
    sdlRect.w=screen_w;
    sdlRect.h=screen_h;
 
    packet=(AVPacket *)av_malloc(sizeof(AVPacket));
 
    video_tid = SDL_CreateThread(sfp_refresh_thread,NULL,NULL);
    //------------SDL End------------
    //Event Loop
    
    for (;;) {
        //Wait
        SDL_WaitEvent(&event);
        if(event.type==SFM_REFRESH_EVENT){
            while(1){
                if(av_read_frame(pFormatCtx, packet)<0)
                    thread_exit=1;
 
                if(packet->stream_index==videoindex)
                    break;
            }
            ret = avcodec_decode_video2(pVideoCodecCtx, pFrame, &got_picture, packet);
            if(ret < 0){
                MAIN_LOG(INFO) <<  "Decode Error.\n";
                return -1;
            }
            if(got_picture){
                sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pVideoCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
                //SDL---------------------------
                SDL_UpdateTexture( sdlTexture, NULL, pFrameYUV->data[0], pFrameYUV->linesize[0] );  
                SDL_RenderClear( sdlRenderer );  
                //SDL_RenderCopy( sdlRenderer, sdlTexture, &sdlRect, &sdlRect );  
                SDL_RenderCopy( sdlRenderer, sdlTexture, NULL, NULL);  
                SDL_RenderPresent( sdlRenderer );  
                //SDL End-----------------------
            }
            av_free_packet(packet);
        }else if(event.type==SDL_KEYDOWN){
            //Pause
            if(event.key.keysym.sym==SDLK_SPACE)
                thread_pause=!thread_pause;
        }else if(event.type==SDL_QUIT){
            thread_exit=1;
        }else if(event.type==SFM_BREAK_EVENT){
            break;
        }
 
    }
 
    sws_freeContext(img_convert_ctx);
 
    SDL_Quit();
    //--------------
    av_frame_free(&pFrameYUV);
    av_frame_free(&pFrame);
    avcodec_close(pVideoCodecCtx);
    avformat_close_input(&pFormatCtx);
 
    return 0;
}


