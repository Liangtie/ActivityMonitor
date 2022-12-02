#include <QApplication>
#include <QIcon>
#include <QStyle>
#include <QTimer>
#include <memory>

#include "manager.h"

#include <qapplication.h>
#include <qdebug.h>

#include "view/logindialog.h"

// #include <QtDebug>
// #include <iostream>

// QString old_style = ui->message->styleSheet();
// ui->message->setStyleSheet("");
// QEventLoop loop;
// QElapsedTimer timer;
// timer.start();
// while (result.isRunning()) {
//     int length = (timer.elapsed() % 10000) / 500;
//     QString dots = "";
//     for (int i = 0; i < length; ++i)
//         dots += ".";
//     this->setMessage("Logging in " + dots);
//     loop.processEvents(QEventLoop::AllEvents, 1);
// }
// ui->message->setStyleSheet(old_style);
// this->setMessage(result.result());
// if (result.result() == "")
//     this->done(QDialog::Accepted);

Manager::Manager(QObject* parent)
    : QSystemTrayIcon(parent)
    , loginDialog_(std::make_unique<LoginDialog>())
{
    setToolTip("Brix Work Logger");
    activatedIcon_ = QIcon(":/resources/images/logo.png");
    QImage logo_img = QImage(":/resources/images/logo.png")
                          .convertToFormat(QImage::Format_ARGB32);
    for (int y = 0; y < logo_img.height(); ++y) {
        QRgb* scanLine = reinterpret_cast<QRgb*>(logo_img.scanLine(y));
        for (int x = 0; x < logo_img.width(); ++x) {
            QRgb pixel = *scanLine;
            int ci = qGray(pixel);
            *scanLine = qRgba(ci, ci, ci, qAlpha(pixel));
            ++scanLine;
        }
    }
    deactivatedIcon_ = QIcon(QPixmap::fromImage(logo_img));

    isActivated_ = false;
    setIcon(deactivatedIcon_);

    connect(this, &Manager::activated, this, &Manager::Actived);

    watchers_.append(std::shared_ptr<Watcher>(new WindowTitleWatcher()));
    watchers_.append(std::shared_ptr<Watcher>(new AppNameWatcher()));
    watchers_.append(std::shared_ptr<Watcher>(new AFKWatcher()));

    QTimer::singleShot(100,
                       this,
                       [&]()
                       {
                           Init();
                           // SwitchProject(0);
                       });

    eventHandler_ = [&](BrixClient::BrixAWEvent event)
    {
        BrixClient::APIResult result;
        if (event.eventId >= 0)
            result = BrixClient::extendEvent(user_, event);
        else
            result = BrixClient::postEvent(user_, event);
        if (result.error == 1) {
            QTimer::singleShot(1, this, &Manager::Init);
            return -1;
        } else if (result.error != 0) {
            return -1;
        }
        if (event.eventId >= 0)
            return event.eventId;
        return result.data["event_id"].toInt();
    };
    heartbeatThread_ = std::make_unique<std::thread>(
        [&]()
        {
            auto timestampMs = []()
            {
                using namespace std::chrono;
                auto ms = duration_cast<milliseconds>(
                    system_clock::now().time_since_epoch());
                return ms.count();
            };
            time_t last_check = timestampMs();
            while (running_) {
                if ((timestampMs() - last_check) > 60000 && user_.loggedIn) {
                    last_check = timestampMs();
                    BrixClient::APIResult result = BrixClient::heartbeat(user_);
                    if (result.error == 1) {
                        QTimer::singleShot(1, this, &Manager::Init);
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        });
}

Manager::~Manager()
{
    running_ = false;
    loginDialog_->close();
    if (heartbeatThread_)
        heartbeatThread_->join();
    if (isActivated_) {
        ToggleRecording();
        for (auto watcher : watchers_) {
            while (watcher->running())
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    if (user_.loggedIn)
        BrixClient::logOut(user_);
}

bool Manager::Init()
{
    if (loginDialog_->isVisible())
        return false;
    user_.loggedIn = false;
    if (isActivated_)
        ToggleRecording();
    if (user_.id != "")
        loginDialog_->setMessage("Session Expired, please re-login");
    loginDialog_->setCallback(
        [&](const QString& email,
            const QString& password,
            bool force,
            bool start)
        {
            BrixClient::APIResult loginResult;
            loginResult = BrixClient::logIn(email, password, force);
            if (loginResult.error == 0) {
                user_ = BrixClient::userFromVariantMap(loginResult.data);
                if (start && !isActivated_)
                    ToggleRecording();
                return QString("");
            }
            if (loginResult.message == "")
                return QString("Unknown Error");
            return loginResult.message;
        });
    loginDialog_->show();
    return false;
}

void Manager::showContextMenu(QPoint pos)
{
    contextMenu_.close();
    contextMenu_.clear();
    if (user_.loggedIn) {
        contextMenu_.addAction(
            isActivated_ ? "Stop" : "Start", this, SLOT(ToggleRecording()));
    }
    contextMenu_.addAction("Exit", this, [=]() { QApplication::quit(); });
    contextMenu_.exec(pos);
}

void Manager::Actived(QSystemTrayIcon::ActivationReason reason)
{
    if (loginDialog_->isVisible())
        loginDialog_->raise();
    else if (!user_.loggedIn && !Init())
        return;
    switch (reason) {
        case QSystemTrayIcon::DoubleClick:
            contextMenu_.close();
            ToggleRecording();
            break;
        default:
            showContextMenu(QCursor::pos());
            break;
    }
}

void Manager::ToggleRecording()
{
    if (!user_.loggedIn && !isActivated_)
        return;
    isActivated_ = !isActivated_;
    if (isActivated_) {
        for (auto watcher : watchers_)
            watcher->start(eventHandler_);
    } else {
        for (auto watcher : watchers_)
            watcher->stop();
    }
    setIcon(isActivated_ ? activatedIcon_ : deactivatedIcon_);
}
