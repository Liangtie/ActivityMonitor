#include "afkwatcher.h"
#if defined(Q_OS_WIN)
#    include <windows.h>

#    include "winuser.h"
uint64_t getAfkTimeMs()
{
    PLASTINPUTINFO plii = new LASTINPUTINFO();
    plii->cbSize = sizeof(LASTINPUTINFO);
    GetLastInputInfo(plii);
    return GetTickCount() - plii->dwTime;
}
#elif defined(Q_OS_MAC)
#    include "utils/macutilities.h"
uint64_t getAfkTimeMs()
{
    return MacUtilities::msSinceLastInput();
}
#elif defined(Q_OS_UNIX)
#endif

AFKWatcher::AFKWatcher()
    : Watcher()
{
}

int AFKWatcher::eventTypeId() const
{
    return 3;
}

std::string AFKWatcher::pollEvent()
{
    return getAfkTimeMs() > 120000 ? "1" : "0";
}
