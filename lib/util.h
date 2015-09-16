#ifndef __UTIL_H
#define __UTIL_H

#include <stdbool.h>

#define DO_PRAGMA(x) _Pragma (#x)
#define TODO(x) DO_PRAGMA(message ("TODO - " #x))
// Usage:
// TODO(Remember to fix this)

#endif /* __UTIL_H */