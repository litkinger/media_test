#ifndef READPACKETSTHREAD_H
#define READPACKETSTHREAD_H

#include <QThread>
#include <QMutex>

class ReadPacketsThread : public QThread
{
    Q_OBJECT
public:
    static ReadPacketsThread * getInstance() {
        static ReadPacketsThread rpt;
        return &rpt;
    }

    ~ReadPacketsThread();

    float currentPos = 0;
    bool isSeek = false;
    void setPlaying(bool isPlaying);
    inline bool getIsPlaying() {
        return isPlay;
    }

    void run();

public slots:
    void receivePos(float pos);

private:
    ReadPacketsThread();
    QMutex mutex;
    bool isPlay = false;
};

#endif // READPACKETSTHREAD_H
