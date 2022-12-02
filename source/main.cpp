#include <QApplication>
#include <QTextStream>

#include <qdebug.h>
#include <qfile.h>
#include <qlocale.h>
#include <qwidget.h>

#include "view/manager.h"
int main(int argc, char* argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    // app.setStyleSheet((
    //     []() -> QString
    //     {
    //         QFile read_style(":/resources/dark/darkstyle.qss");
    //         if (read_style.open(QIODevice::ReadOnly)) {
    //             QTextStream stream(&read_style);
    //             return stream.readAll();
    //         }
    //         return {};
    //     })());
    Manager w;
    w.show();
    QApplication::setQuitOnLastWindowClosed(false);
    return QApplication::exec();
}
