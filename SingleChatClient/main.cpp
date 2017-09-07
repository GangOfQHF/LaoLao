#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "singlechatclient.h"

int main(int argc,char *argv[])
{
    QGuiApplication app(argc, argv);

    SingleChatClient *sc = new SingleChatClient();

    QQmlApplicationEngine engine;
//    qRegisterMetaType<std::string>("string");
    engine.rootContext()->setContextProperty("singlechat",sc);
    engine.rootContext()->setContextProperty("friendList",sc->getFriendList());
    engine.rootContext()->setContextProperty("listModel",sc->getFriendList()->getFriendItemModel());

    engine.rootContext()->setContextProperty("msgHandler",sc->getMsgModel());
    engine.rootContext()->setContextProperty("messageModel",sc->getMsgModel()->getMessageModel());

    engine.rootContext()->setContextProperty("chatMsgModel",sc->getChatMsgModel());
    engine.rootContext()->setContextProperty("chatMsgItemModel",sc->getChatMsgModel()->getChatMsgItemModel());

//    engine.rootContext()->setContextProperty("addNoticeModel",sc->getAddNoticeModel());
    engine.rootContext()->setContextProperty("addNoticeItemModel",sc->getAddNoticeModel()->getAddFriendNoticeItemModel());
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();

}
