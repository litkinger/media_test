
#include <iostream>
#include <QApplication>
#include <QtWidgets/QApplication>

//#define WEBRTC_WIN 1

#include "logging/log.h"
#include "logging/loghandler.h"
#include "avcommon.h"
#include "playerui.h"

std::shared_ptr<LogHandler> logHandler(new LogHandlerImpl());

#undef main
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    PlayerUi w;
    w.show();
    return a.exec();
}


