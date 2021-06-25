#include <QMutexLocker>
#include "mediaplayer.h"
#include "media.h"
#include "video.h"
#include "audio.h"

static const double SYNC_THRESHOLD = 0.01;//同步临界值
static const double NOSYNC_THRESHOLD = 10.0;//非同步临界值

static CommonLog commonLogger(logHandler,"mediaplayer");
#define MEDIAPLAYER_LOG(level) COMMON_LOG(commonLogger, level)

MediaPlayer::MediaPlayer()
{
    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(syncMedia()));
    timer->start(20);
    buf = new uchar[imageWidth*imageHeight * 4];
    img = new QImage(buf, imageWidth, imageHeight, QImage::Format_ARGB32);
}

MediaPlayer::~MediaPlayer()
{
    if(timer)
        delete timer;
}

void MediaPlayer::syncMedia()
{
    if (!isPlay) {
        QMutexLocker locker(&mutex);
        if (!isPlay) {
            return;//just return
        }
    }

    if (Media::getInstance()->video->getVideoFrameSiez() == 0) {
        timer->setInterval(20);//视频队列没帧就设置下次检查
        return;
    }

    static QImage::Format format = img->format();

    if (img == NULL)
        {
            if (buf != nullptr) {
                delete buf;
                buf = nullptr;
            }
            if (format == QImage::Format_ARGB32) {
                buf = new uchar[imageWidth*imageHeight * 4];//彩色视频设置
                img = new QImage(buf, imageWidth , imageHeight, QImage::Format_ARGB32);
            }
            else {
                buf = new uchar[imageWidth*imageHeight];
                img = new QImage(buf, imageWidth, imageHeight, QImage::Format_Grayscale8);//黑白视频设置
            }

        }

    AVFrame *vFrame = Media::getInstance()->video->dequeueFrame();

    // 将视频同步到音频上，计算下一帧的延迟时间
    double current_pts = *(double*)vFrame->opaque;
    double delay = current_pts - Media::getInstance()->video->getFrameLastPts();
    if (delay <= 0 || delay >= 1.0)
        delay = Media::getInstance()->video->getFrameLastDelay();
    Media::getInstance()->video->setFrameLastDelay(delay);
    Media::getInstance()->video->setFrameLastPts(current_pts);
    // 当前显示帧的PTS来计算显示下一帧的延迟
    double ref_clock = Media::getInstance()->audio->getAudioClock();
    double diff = current_pts - ref_clock;// diff < 0 => video slow,diff > 0 => video quick
    double threshold = (delay > SYNC_THRESHOLD) ? delay : SYNC_THRESHOLD;
    if (fabs(diff) < NOSYNC_THRESHOLD) // 不同步
    {
        if (diff <= -threshold) // 慢了，delay设为0
            delay = 0;
        else if (diff >= threshold) // 快了，加倍delay
            delay *= 2;
    }
    Media::getInstance()->video->setFrameTimer(Media::getInstance()->video->getFrameTimer() + delay) ;

    double actual_delay = Media::getInstance()->video->getFrameTimer() - static_cast<double>(av_gettime()) / 1000000.0;
    if (actual_delay <= 0.010)
        actual_delay = 0.010;
    timer->setInterval(static_cast<int>(actual_delay * 1000 + 0.5));
    AVCodecContext *videoCtx = Media::getInstance()->video->getVideoStream()->codec;

    uint8_t *data[AV_NUM_DATA_POINTERS] = { 0 };
    data[0] = (uint8_t *)img->bits();
    int linesize[AV_NUM_DATA_POINTERS] = { 0 };

    if (img->format() == QImage::Format_Grayscale8) {//灰度视频
        Media::getInstance()->video->swsContext = sws_getCachedContext(Media::getInstance()->video->swsContext, videoCtx->width,
            videoCtx->height,
            videoCtx->pix_fmt,
            imageWidth, imageHeight,
            AV_PIX_FMT_GRAY8,
            SWS_BICUBIC,
            NULL, NULL, NULL
        );

        linesize[0] = imageWidth;
    }
    else {//彩色视频
        Media::getInstance()->video->swsContext = sws_getCachedContext(Media::getInstance()->video->swsContext, videoCtx->width,
            videoCtx->height,
            videoCtx->pix_fmt,
            imageWidth, imageHeight,
            AV_PIX_FMT_BGRA,
            SWS_BICUBIC,
            NULL, NULL, NULL
        );

        linesize[0] = imageWidth * 4;
    }

    int h = sws_scale(Media::getInstance()->video->swsContext, vFrame->data, vFrame->linesize, 0, videoCtx->height,
        data,
        linesize
    );//缩放图片
    img->setColorCount(1);
    emit updateFrame(img);//发送信号到ui线程播放视频

    av_frame_unref(vFrame);
    av_frame_free(&vFrame);

}

void MediaPlayer::play(bool isPlay)
{
    QMutexLocker locker(&mutex);
    this->isPlay = isPlay;
    if (isPlay) {
        timer->setInterval(0);
    }
}

