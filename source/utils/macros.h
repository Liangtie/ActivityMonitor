#ifndef __MACROS_H__
#define __MACROS_H__

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define DISALLOW_ASSIGN(TypeName) TypeName& operator=(const TypeName&) = delete;

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&) = delete; \
    DISALLOW_ASSIGN(TypeName)

#define MOVE_ONLY_WITH_DEFAULT_CONSTRUCTORS(TypeName) \
    TypeName() = default; \
    MOVE_ONLY_NO_DEFAULT_CONSTRUCTOR(TypeName)

#define MOVE_ONLY_NO_DEFAULT_CONSTRUCTOR(TypeName) \
    TypeName(TypeName&&) = default; \
    TypeName& operator=(TypeName&&) = default; \
    DISALLOW_COPY_AND_ASSIGN(TypeName)

#define DECLARE_STATIC_STRING_AND_INIT(x, y) static char const x[] = y;

#define DECLARE_STATIC_STRING_WITH_DEFAULT(x) static char const x[] = #x;

#define SAFE_DELETE(x) \
    if (x) \
        delete x; \
    x = nullptr;
#define FromStdString(x) QString::fromStdString(x)
#define Q_STRING_TO_CSTR(x) x.toStdString().c_str()

#endif  // __MACROS_H__
