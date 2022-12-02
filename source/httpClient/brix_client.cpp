#include <QElapsedTimer>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <memory>

#include "brix_client.h"

#include <qthread.h>

#include "core/brix_activity_watch_global.h"
#include "httpClient/brix_client_request.hpp"

BRIX_USE_NAMESPACE

BrixClient::BrixClient()
    : networkAccessManager_(new QNetworkAccessManager(this))
{
}

BrixClient::~BrixClient() {}

void BrixClient::login(ParamLogin const& info)
{
    sendRequest(API::LOGIN, info);
}

void BrixClient::logout(ParamLogout const& info)
{
    sendRequest(API::LOGOUT, info);
}

void BrixClient::postEvent(ParamPostEvent const& info)
{
    sendRequest(API::POST_EVENT, info);
}

void BrixClient::extendEvent(ParamExtendEvent const& info)
{
    sendRequest(API::EXTEND_EVENT, info);
}

void BrixClient::sendRequest(API api, Json const& json)
{
    QUrl url = QUrl(kHost + QString::fromStdString(getEndPoint(api)));
    QNetworkRequest request(url);
    QSslConfiguration sslConfiguration(
        QSslConfiguration::defaultConfiguration());
    request.setSslConfiguration(sslConfiguration);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Connection", "close");
    APIResult result;
    for (auto tries = 0; tries < RETRY_COUNT; ++tries) {
        std::unique_ptr<QNetworkReply> reply;
        try {
            reply.reset(
                networkAccessManager_->post(request, json.dump().data()));
        } catch (const std::exception& e) {
            result.error = APIResult::TIMEOUT_OR_CONNECTION_FAILED;
            result.message = QString(e.what());
            emit requestResult(result, tries);

        } catch (...) {
            result.error = APIResult::TIMEOUT_OR_CONNECTION_FAILED;
            result.message = "Unknown error";
            emit requestResult(result, tries);
        }
        QElapsedTimer timer;
        timer.start();
        while (timer.elapsed() < TIME_OUT && !reply->isFinished())
            QThread::msleep(SLEEP_TIME);
        if (reply->isFinished() && reply->error() == QNetworkReply::NoError) {
            QJsonObject response_json =
                QJsonDocument::fromJson(reply->readAll()).object();
            result.error = response_json["error"].toInt();
            result.message = response_json["message"].toString();
            result.data = response_json["data"].toString();
            emit requestResult(result, tries);
        } else {
            result.error = APIResult::TIMEOUT_OR_CONNECTION_FAILED;
            if (reply->error() == QNetworkReply::NoError) {
                result.message = "Access timeout or connection failed";
            } else {
                result.message = reply->errorString() + " ("
                    + QString::number(reply->error()) + ")";
            }
            emit requestResult(result, tries);
        }
    }
}
