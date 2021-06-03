QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

INCLUDEPATH += /usr/local/include src

LIBS += -L/usr/local/lib -llog4cxx

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/logging/log.cc \
    src/logging/loghandler.cc \
    src/common/sigslot.cc \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    src/common/atomic_ops.h \
    src/common/constructor_magic.h \
    src/common/ref_count.h \
    src/common/ref_counter.h \
    src/common/sigslot.h \
    src/logging/log.h \
    src/logging/loghandler.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
