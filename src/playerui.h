#ifndef PLAYERUI_H
#define PLAYERUI_H

#include <QWidget>
#include <QMenu>

namespace Ui {
class PlayerUi;
}

class PlayerUi : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerUi(QWidget *parent = nullptr);
    ~PlayerUi();

    void play();

public slots:
    void onOpenFileClicked();

private:
    Ui::PlayerUi *ui;
    bool isPlay;
};

#endif // PLAYERUI_H
