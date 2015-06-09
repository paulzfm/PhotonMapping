#ifndef COMMON_H_
#define COMMON_H_

const static double PI = 3.14159;
const static double EPS = 1 >> 20;

#ifndef MAX
#define MAX(x, y) (x > y ? (x) : (y))
#endif // MAX

#ifndef MIN
#define MIN(x, y) (x < y ? (x) : (y))
#endif // MIN

#ifndef ABS
#define ABS(x) (x > 0 ? (x) : (-(x)))
#endif // ABS

#ifndef DISABLE_COPYING
#define DISABLE_COPYING(cls) \
    cls(const cls&); \
    cls& operator = (const cls&);
#endif // DISABLE_COPYING

#endif // COMMON_H_