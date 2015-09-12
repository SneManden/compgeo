#ifndef __UTIL_H
#define __UTIL_H

#define DO_PRAGMA(x) _Pragma (#x)
#define TODO(x) DO_PRAGMA(message ("TODO - " #x))
// Usage:
// TODO(Remember to fix this)

#define true 1
#define false 0

#endif /* __UTIL_H */