#include <QEventLoop>

#include "logindialog.h"

#include "httpClient/brixclient.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(CallbackFunc callback, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->callback_ = callback;
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

QString LoginDialog::getEmail()
{
    return ui->emailLineEdit->text();
}

QString LoginDialog::getPassword()
{
    return ui->passwordLineEdit->text();
}

bool LoginDialog::getForce()
{
    return ui->forceCheckBox->isChecked();
}

bool LoginDialog::getStart()
{
    return ui->startCheckBox->isChecked();
}

void LoginDialog::setMessage(const QString& text)
{
    ui->message->setText(text);
}

void LoginDialog::setCallback(CallbackFunc callback)
{
    callback_ = callback;
}

void LoginDialog::accept()
{
    QFuture<QString> result = QtConcurrent::run(this->callback_,
                                                this->getEmail(),
                                                this->getPassword(),
                                                this->getForce(),
                                                this->getStart());
    QString old_style = ui->message->styleSheet();
    ui->message->setStyleSheet("");
    QEventLoop loop;
    QElapsedTimer timer;
    timer.start();
    while (result.isRunning()) {
        int length = (timer.elapsed() % 10000) / 500;
        QString dots = "";
        for (int i = 0; i < length; ++i)
            dots += ".";
        this->setMessage("Logging in " + dots);
        loop.processEvents(QEventLoop::AllEvents, 1);
    }
    ui->message->setStyleSheet(old_style);
    this->setMessage(result.result());
    if (result.result() == "")
        this->done(QDialog::Accepted);
    emit loginClicked();
}
