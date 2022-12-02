#ifndef __BETTER_ENUM_H__
#define __BETTER_ENUM_H__

#include "enum.h"

#define Enum(name, ...) BETTER_ENUM(name, int, __VA_ARGS__);

#endif  // __BETTER_ENUM_H__