#ifndef BRIX_CLIENT_H
#define BRIX_CLIENT_H

#include <memory>
#include <unordered_map>

#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include <qobject.h>
#include <qobjectdefs.h>

#include "brix_client_request.hpp"
#include "core/brix_activity_watch_global.h"
#include "httpClient/brix_client_request.hpp"
#pragma once

BEGIN_BRIX_NAMESPACE
enum
{
    RETRY_COUNT = 5,
    SLEEP_TIME = 200,
    TIME_OUT = 5000
};
class BrixClient : public QObject
{
    Q_OBJECT
  public:
    BrixClient();
    BrixClient(BrixClient const&) = delete;
    BrixClient(BrixClient&&) = delete;
    BrixClient& operator=(BrixClient const&) = delete;
    BrixClient& operator=(BrixClient&&) = delete;
    ~BrixClient() override;

  public slots:
    void login(ParamLogin const& info);

    void logout(ParamLogout const& info);

    void postEvent(ParamPostEvent const& info);

    void extendEvent(ParamExtendEvent const& info);

  signals:
    void requestResult(APIResult const&, int retryCount);

  private:
    void sendRequest(API api, Json const& json);
    QNetworkAccessManager* networkAccessManager_ {};
};

END_BRIX_NAMESPACE
#endif