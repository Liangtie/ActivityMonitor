#ifndef MACUTILITIES_H
#define MACUTILITIES_H

#include <string>
typedef unsigned long long uint64_t;

class MacUtilities
{
public:
    static uint64_t msSinceLastInput();
    static std::string getActiveWindowTitle();
    static std::string getActiveAppName();
};

#endif // MACUTILITIES_H
