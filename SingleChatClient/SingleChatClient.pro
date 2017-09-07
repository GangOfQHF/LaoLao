TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    singlechatclient.cpp \
    friendlist.cpp \
    messagemodel.cpp \
    chatmsgmodel.cpp \
    addfriendnoticemodel.cpp

RESOURCES += qml.qrc \
    image.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    singlechatclient.h \
    QQmlPtrPropertyHelpers.h \
    QQmlObjectListModel.h \
    QQmlHelpersCommon.h \
    QQmlAutoPropertyHelpers.h \
    frienditem.h \
    protocol.h \
    friendlist.h \
    messageitem.h \
    messagemodel.h \
    chatmsgmodel.h \
    chatmsgitem.h \
    addfriendnoticemodel.h \
    addfriendnoticeItem.h

DISTFILES +=

LIBS += -lboost_system
LIBS += -lboost_thread
