#ifndef MANAGER_H
#define MANAGER_H

#include <QCloseEvent>
#include <QMenu>
#include <QSystemTrayIcon>
#include <array>
#include <memory>

#include <qthread.h>

#include "httpClient/brixclient.h"
#include "logindialog.h"
#include "watcher/afkwatcher.h"
#include "watcher/appnamewatcher.h"
#include "watcher/windowtitlewatcher.h"

class Manager : public QSystemTrayIcon
{
    Q_OBJECT

  public:
    Manager(QObject* parent = nullptr);
    ~Manager() override;

  private slots:
    void Actived(QSystemTrayIcon::ActivationReason reason);
    void ToggleRecording();

  private:
    bool Init();
    void showContextMenu(QPoint pos);

    std::atomic<bool> isActivated_ = false, running_ = true;

    QMenu contextMenu_;

    // const BrixClient::Project *selectedProject = nullptr;
    BrixClient::User user_;
    // QMenu *project_menu = new QMenu("Projects");
    QIcon activatedIcon_;
    QIcon deactivatedIcon_;
    std::unique_ptr<LoginDialog> loginDialog_;
    QList<std::shared_ptr<Watcher>> watchers_;
    std::function<int(BrixClient::BrixAWEvent event)> eventHandler_;
    std::unique_ptr<std::thread> heartbeatThread_ = {};
};

#endif  // MANAGER_H
