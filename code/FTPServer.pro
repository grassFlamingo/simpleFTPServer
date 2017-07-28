TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lpthread libwsock32 libws2_32

SOURCES += main.cpp \
    FTPHandler.cpp \
    ReQueue.cpp \
    Socket.cpp \
    Thread.cpp \
    ThreadPool.cpp \
    exception.cpp \
    Rwexception.cpp \
    ftpcommand.cpp \
    Session.cpp \
    mystring.cpp \
    setting.cpp \
    site.cpp

HEADERS += \
    FTPHandler.h \
    ReQueue.h \
    Socket.h \
    Thread.h \
    ThreadPool.h \
    exception.h \
    Rwexception.h \
    locks.h \
    ftpcommand.h \
    Session.h \
    mystring.h \
    setting.h \
    json.hpp \
    site.h

DISTFILES += \
    setting.json
