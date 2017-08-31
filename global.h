#ifndef GLOBAL_H
#define GLOBAL_H

#include <cmath>
#include <algorithm>
using std::min;
using std::max;
using std::swap;
using std::sort;
#include <iostream>
#include <vector>

using std::ostream;
#include <assert.h>
#ifdef M_PI
#undef M_PI
#endif
#define M_PI    3.14159265358979323846f
#define INV_PI  0.31830988618379067154f
#define INV_TWOPI  0.15915494309189533577f
#ifndef INFINITY
#define INFINITY FLT_MAX
#endif


typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;

inline float Lerp(float t, float v1, float v2) {
    return (1.f - t) * v1 + t * v2;
}
inline float Clamp(float val, float low, float high) {
    if (val < low) return low;
    else if (val > high) return high;
    else return val;
}
inline int Clamp(int val, int low, int high) {
    if (val < low) return low;
    else if (val > high) return high;
    else return val;
}
inline int Mod(int a, int b) {
    int n = int(a/b);
    a -= n*b;
    if (a < 0)
        a += b;
    return a;
}
inline float Radians(float deg) {
    return ((float)M_PI/180.f) * deg;
}
inline float Degrees(float rad) {
    return (180.f/(float)M_PI) * rad;
}
inline float Log2(float x) {
    static float invLog2 = 1.f / logf(2.f);
    return logf(x) * invLog2;
}
inline int Log2Int(float v) {
    return ((*(int *) &v) >> 23) - 127;
}
inline bool IsPowerOf2(int v) {
    return (v & (v - 1)) == 0;
}
inline u_int RoundUpPow2(u_int v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    return v+1;
}
#endif // GLOBAL_H
