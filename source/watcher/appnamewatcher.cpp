#include <QString>
#include <string>

#include "appnamewatcher.h"
enum
{
    MAX_CHAR_COUNT = 1024
};

#if defined(Q_OS_WIN)
#    pragma comment(lib, "kernel32.lib")
#    pragma comment(lib, "user32.lib")
#    pragma comment(lib, "gdi32.lib")
#    pragma comment(lib, "comdlg32.lib")
#    include <QString>

#    include <windows.h>
//! SECTION
#    include <psapi.h>

std::string getActiveAppName()
{
    char buf[MAX_CHAR_COUNT];
    HWND active_hwnd = GetForegroundWindow();
    DWORD dwPID;
    GetWindowThreadProcessId(active_hwnd, &dwPID);

    HANDLE Handle =
        OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPID);
    GetProcessImageFileName(Handle, buf, MAX_CHAR_COUNT);
    CloseHandle(Handle);
    return buf;
}
#elif defined(Q_OS_MAC)
#    include "utils/macutilities.h"
std::string getActiveAppName()
{
    return MacUtilities::getActiveAppName();
}
#elif defined(Q_OS_UNIX)
#endif

AppNameWatcher::AppNameWatcher()
    : Watcher()
{
}

int AppNameWatcher::eventTypeId() const
{
    return 1;
}

std::string AppNameWatcher::pollEvent()
{
    return getActiveAppName();
}
