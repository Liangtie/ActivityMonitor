#ifndef BRIXHTTPCLIENT_H
#define BRIXHTTPCLIENT_H

#include <QtDebug>

namespace BrixClient
{
struct BrixAWEvent
{
    int eventId = -1;  //-1:for new event, other: extend existing event
    int eventType;  // 1:appname 2:windowtitle 3:afk
    std::string eventContent = "";
    uint64_t startTime = 0, endTime = 1;
};

struct APIResult
{
    int error;
    QString message;
    QVariantMap data;
};

//    struct Project
//    {
//        QString id, name;
//        void *data = nullptr;
//    };
struct User
{
    bool loggedIn = false;
    QString id = "", name;
    QByteArray token;
    //        QList<Project> projects;
    void* data = nullptr;
};

APIResult logIn(QString username, QString password, bool force = false);
void logOut(User& user);
User userFromVariantMap(const QVariantMap& data);

APIResult postEvent(const User& user, const BrixAWEvent& event);
APIResult extendEvent(const User& user, const BrixAWEvent& event);
APIResult heartbeat(const User& user);

};  // namespace BrixClient

#endif  // BRIXHTTPCLIENT_H
