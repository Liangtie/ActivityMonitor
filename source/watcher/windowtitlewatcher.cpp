#include <QString>
#include <string>

#include "windowtitlewatcher.h"

#include "utils/helper_ui.h"

enum
{
    MAX_CHAR_COUNT = 1024
};

#if defined(Q_OS_WIN)
#    include <windows.h>
std::string getActiveWindowTitle()
{
    char buf[MAX_CHAR_COUNT];
    GetWindowText(GetForegroundWindow(), buf, MAX_CHAR_COUNT);
    return buf;
}
#elif defined(Q_OS_MAC)
#    include "utils/macutilities.h"
std::string getActiveWindowTitle()
{
    return MacUtilities::getActiveWindowTitle();
}
#elif defined(Q_OS_UNIX)
#endif

WindowTitleWatcher::WindowTitleWatcher()
    : Watcher()
{
}

int WindowTitleWatcher::eventTypeId() const
{
    return 2;
}

std::string WindowTitleWatcher::pollEvent()
{
    return getActiveWindowTitle();
}
