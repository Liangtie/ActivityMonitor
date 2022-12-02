#ifndef __BRIX_CLIENT_REQUEST_H__
#define __BRIX_CLIENT_REQUEST_H__

#include <QByteArray>
#include <QString>
#include <string>

#include "core/brix_activity_watch_global.h"
#include "utils/json.hpp"
BEGIN_BRIX_NAMESPACE

using Json = nlohmann::json;

using ByteArray = std::vector<std::byte>;

static const QString kHost = "https://project.joinbrix.com:10001/";

enum API
{
    LOGIN,
    LOGOUT,
    POST_EVENT,
    EXTEND_EVENT,
    KEEP_ALIVE,
};

inline auto getAPI(API api) -> std::string
{
    switch (api) {
        case LOGIN:
            return "login";
        case LOGOUT:
            return "logout";
        case POST_EVENT:
            return "post_event";
        case EXTEND_EVENT:
            return "extend_event";
        case KEEP_ALIVE:
            return "keep_alive";
    }
}

inline auto getEndPoint(API api) -> std::string
{
    static constexpr auto kApiPrefix = "api/";
    return getAPI(api) + kApiPrefix;
}

enum BrixAWEventType
{
    APPNAME = 1,
    WINDOW_TITLE,
    AFK,
};

enum
{
    NEW_EVENT_ID = -1,
};

struct BrixAWEvent
{
    //-1:for new event, other: extend existing event
    int event_id = NEW_EVENT_ID;
    BrixAWEventType event_type;
    std::string eventContent;
    uint64_t start_time = 0;
    uint64_t end_time = 1;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
        BrixAWEvent, event_id, event_type, eventContent, start_time, end_time)
};

struct APIResult
{
    enum
    {
        TIMEOUT_OR_CONNECTION_FAILED = -1,
    };
    int error {};
    QString message;
    QString data;
};

struct Project
{
    std::string id;
    std::string name;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Project, id, name)
};

struct User
{
    bool loggedIn = false;
    std::string id;
    std::string name;
    ByteArray token;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(User, loggedIn, id, name, token)
};

//! SECTION Params

struct ParamLogin
{
    std::string email;
    std::string password;
    bool force;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ParamLogin, email, password, force)
};
struct ParamLogout
{
    ByteArray token;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ParamLogout, token)
};

struct ParamPostEvent
{
    ByteArray token;
    BrixAWEventType event_type;
    std::string eventContent;
    uint64_t start_time = 0;
    uint64_t end_time = 1;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
        ParamPostEvent, token, event_type, eventContent, start_time, end_time)
};

struct ParamExtendEvent
{
    ByteArray token;
    int event_id = -1;
    uint64_t end_time = 1;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ParamExtendEvent, token, event_id, end_time)
};

END_BRIX_NAMESPACE

#endif  // __BRIX_CLIENT_REQUEST_H__