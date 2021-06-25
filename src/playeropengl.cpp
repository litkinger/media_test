#include "playeropengl.h"
#include "ui_playeropengl.h"
#include "readpacketsthread.h"

#include <QPainter>

static QImage* image;

PlayerOpenGL::PlayerOpenGL(QWidget *parent) :
    QOpenGLWidget(parent),
    ui(new Ui::PlayerOpenGL)
{
    ui->setupUi(this);
}

PlayerOpenGL::~PlayerOpenGL()
{
    delete ui;
}

void PlayerOpenGL::setVideoImage(QImage* img) {
    if (ReadPacketsThread::getInstance()->getIsPlaying()) {
        image = img;
        this->update();
    }
}

void PlayerOpenGL::paintEvent(QPaintEvent *e)
{
    if (image == nullptr)
        return;

    QPainter painter;
    painter.begin(this);
    painter.drawImage(QPoint(0, 0), *image);
    painter.end();
}
