#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <functional>

#include <qobjectdefs.h>

#include <QtConcurrent/QtConcurrent>

namespace Ui
{
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT
  private:
    using CallbackFunc =
        std::function<QString(const QString&, const QString&, bool, bool)>;

  public:
    explicit LoginDialog(
        CallbackFunc callback = [](const QString&, const QString&, bool, bool)
        { return ""; },
        QWidget* parent = nullptr);
    ~LoginDialog() override;

    QString getEmail();
    QString getPassword();
    bool getForce();
    bool getStart();
    void setMessage(const QString& text);
    void setCallback(CallbackFunc callback);

  private:
    void accept() override;
    CallbackFunc callback_;

  private:
    Ui::LoginDialog* ui;

  signals:
    void loginClicked();
};

#endif  // LOGINDIALOG_H
