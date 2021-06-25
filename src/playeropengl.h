#ifndef PLAYEROPENGL_H
#define PLAYEROPENGL_H

#include <QOpenGLWidget>
#include <QImage>
#include <QPaintEvent>

namespace Ui {
class PlayerOpenGL;
}

class PlayerOpenGL : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit PlayerOpenGL(QWidget *parent = nullptr);
    ~PlayerOpenGL();

public slots:
    void setVideoImage(QImage *img);

private:
    void paintEvent(QPaintEvent *e);
    Ui::PlayerOpenGL *ui;
};

#endif // PLAYEROPENGL_H
