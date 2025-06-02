#ifndef MATRIX_H
#define MATRIX_H
#include <stdint.h>
#include "fixedpt.h"
#define VOID 0
#define SCALAR 1
#define VEC2 2 
#define VEC3 4
#define VEC4 8
#define MAT2 16
#define MAT3 32
#define MAT4 64
#define VEC3I 128
typedef struct {
    fixedpt x,y;
}Vec2;
typedef struct {
    int32_t x,y;
}Vec2i;
typedef struct {
    fixedpt x,y,z;
}Vec3;
typedef struct {
    int32_t x,y,z;
}Vec3i;
typedef struct {
    fixedpt x,y,z,w;
}Vec4;
typedef struct {
    int32_t x,y,z,w;
}Vec4i;
typedef struct {
    Vec4 x,y,z,w;
}Mat4;
typedef struct {
    Vec3 x,y,z;
}Mat3;
typedef struct {
    Vec2 x,y;
}Mat2;
void normalize(void*,int);
void mul(void* dst, void* src1,void* src2, int type);
fixedpt dot(void* src1,void* src2, int type);
void sub(void*,void*,void*,int);
void cross(void* dst, void* src1,void* src2, int type);
fixedpt max(fixedpt x, fixedpt y);
fixedpt min(fixedpt x, fixedpt y);
#endif