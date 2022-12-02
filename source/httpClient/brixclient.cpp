#include <QElapsedTimer>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "brixclient.h"

namespace BrixClient
{
// const QString Host = "https://aw.b-stack.com/";
const QString Host = "https://project.joinbrix.com:10001/";
APIResult accessAPI(const QString& endpoint, const QVariantMap& data)
{
    APIResult result;

    static QNetworkAccessManager manager;
    QEventLoop loop;
    QUrl url = QUrl(Host + endpoint);
    QNetworkRequest request(url);
    static QSslConfiguration sslConfiguration(
        QSslConfiguration::defaultConfiguration());
    request.setSslConfiguration(sslConfiguration);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Connection", "close");
    QJsonObject jsonBody = QJsonObject::fromVariantMap(data);
    for (int tries = 0; tries < 5; ++tries) {
        std::unique_ptr<QNetworkReply> reply;
        try {
            reply.reset(
                manager.post(request, QJsonDocument(jsonBody).toJson()));
        } catch (const std::exception& e) {
            result.error = -1;
            result.message = QString(e.what());
            return result;
        } catch (...) {
            result.error = -1;
            result.message = "Unknown error";
            return result;
        }
        QElapsedTimer timer;
        timer.start();
        while (timer.elapsed() < 5000 && !reply->isFinished()) {
            loop.processEvents(QEventLoop::AllEvents, 1);
        }
        if (reply->isFinished() && reply->error() == QNetworkReply::NoError) {
            QJsonObject response_json =
                QJsonDocument::fromJson(reply->readAll()).object();
            result.error = response_json["error"].toInt();
            result.message = response_json["message"].toString();
            result.data = response_json["data"].toObject().toVariantMap();
            return result;
        } else {
            result.error = -1;
            if (reply->error() == QNetworkReply::NoError) {
                result.message = "Access timeout or connection failed";
            } else {
                result.message = reply->errorString() + " ("
                    + QString::number(reply->error()) + ")";
            }
        }
    }
    return result;
}

User userFromVariantMap(const QVariantMap& data)
{
    User result;
    result.id = data["id"].toString();
    result.data = nullptr;
    result.name = data["name"].toString();
    result.token = data["token"].toByteArray();
    result.loggedIn = true;
    return result;
}

APIResult logIn(QString email, QString password, bool force)
{
    static const QString endpoint = "api/login";
    QVariantMap data;
    data.insert("email", email);
    data.insert("password", password);
    data.insert("force", force);
    return accessAPI(endpoint, data);
}

void logOut(User& user)
{
    static const QString endpoint = "api/logout";
    QVariantMap data;
    data.insert("token", user.token);
    accessAPI(endpoint, data);
}

APIResult postEvent(const User& user, const BrixAWEvent& event)
{
    static const QString endpoint = "api/post_event";
    QVariantMap data;
    data.insert("token", user.token);
    data.insert("event_type", event.eventType);
    data.insert("event_content", QString::fromStdString(event.eventContent));
    data.insert("start_time", event.startTime);
    data.insert("end_time", event.endTime);
    return accessAPI(endpoint, data);
}

APIResult extendEvent(const User& user, const BrixAWEvent& event)
{
    static const QString endpoint = "api/extend_event";
    QVariantMap data;
    data.insert("token", user.token);
    data.insert("event_id", event.eventId);
    data.insert("end_time", event.endTime);
    return accessAPI(endpoint, data);
}

APIResult heartbeat(const User& user)
{
    static const QString endpoint = "api/keep_alive";
    QVariantMap data;
    data.insert("token", user.token);
    return accessAPI(endpoint, data);
}

};  // namespace BrixClient
