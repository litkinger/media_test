#include "PlayerUi.h"
#include "ui_playerui.h"
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QDir>
#include <QImage>
#include "media.h"
#include "logging/log.h"
#include "audio.h"
#include "video.h"
#include "mediaplayer.h"
#include "readpacketsthread.h"

static CommonLog commonLogger(logHandler,"player");
#define PLAYER_LOG(level) COMMON_LOG(commonLogger, level)

PlayerUi::PlayerUi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerUi),
    isPlay(false)
{
    ui->setupUi(this);

    setMinimumWidth(600);
    setMinimumHeight(400);

    QObject::connect(MediaPlayer::getInstance(),//信号发出的对象
        SIGNAL(updateFrame(QImage*)),//信号
        ui->openGLWidget,//槽接收的对象
        SLOT(setVideoImage(QImage*))//槽
    );

    QAction *openFileAction = new QAction(tr("Open File..."), this);
    openFileAction->setShortcut(QKeySequence(tr("Ctrl+O")));
    QAction *openNetworkAction = new QAction(tr("Open Network..."), this);
    openNetworkAction->setShortcut(QKeySequence(tr("Ctrl+N")));

    QMenuBar *menuBar = new QMenuBar(0);
    QMenu *wnd = menuBar->addMenu(tr("File"));

    wnd->addAction(openFileAction);
    wnd->addAction(openNetworkAction);
    wnd->addSeparator();


    QObject::connect(openFileAction, SIGNAL(triggered()), this, SLOT(onOpenFileClicked()));

    ReadPacketsThread *readPacketsThread = ReadPacketsThread::getInstance();
    readPacketsThread->start();

    Media::getInstance()->video->start();
}

PlayerUi::~PlayerUi()
{
    delete ui;
}


void PlayerUi::onOpenFileClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("open file"),
                                                    QDir::homePath() + "/Documents",
                                                    tr("(*mov *mp4 *rmvb *flv *avi *mkv)"));
    PLAYER_LOG(INFO) << "filename : "<< fileName.toStdString();
    if (fileName.isEmpty()) {
        return;
    }
    std::string file = fileName.toLocal8Bit().data();//防止有中文
    Media *media = Media::getInstance()
        ->setMediaFile(file.c_str())
        ->config();

    play();
}

void PlayerUi::play() {
    isPlay = !isPlay;
    MediaPlayer::getInstance()->play(isPlay);
    if (isPlay)
    {
        if(Media::getInstance()->getAVFormatContext())
            Media::getInstance()->audio->audioPlay();
    }
    else
    {
        if (Media::getInstance()->getAVFormatContext())
            SDL_CloseAudio();
    }
}
