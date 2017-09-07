#-------------------------------------------------
#
# Project created by QtCreator 2017-05-07T12:22:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = singleChatServer
TEMPLATE = app


SOURCES += main.cpp\
    account.cpp \
    group.cpp \
    friend.cpp \
    chatrecord.cpp \
    message.cpp \
    database.cpp \
    singlechatserver.cpp

HEADERS  += \
    account.h \
    group.h \
    friend.h \
    chatrecord.h \
    message.h \
    database.h \
    singlechatserver.h \
    protocol.h

FORMS    +=

LIBS += -lboost_system -lmysqlclient -lboost_thread
