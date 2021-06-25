#ifndef MediaPLAYER_H
#define MediaPLAYER_H

#include <QTimer>
#include <QImage>
#include <QMutex>

class MediaPlayer:public QObject
{
    Q_OBJECT
public:
    static MediaPlayer *getInstance() {
        static MediaPlayer mediaPlayer;
        return &mediaPlayer;
    }
    ~MediaPlayer();
    void play(bool isPlay);
public slots:
    void syncMedia();
private:
    MediaPlayer();
    QTimer *timer;
    QMutex mutex;
    QImage *img;
    uchar *buf;
    int imageWidth = 800;
    int imageHeight = 600;
    bool isPlay = false;
signals:
    void updateFrame(QImage* img);
};

#endif // MediaPLAYER_H
