#include <stdint.h>
#include "fixedpt.h"
#define VOID 0
#define SCALAR 1
#define VEC2 2 
#define VEC3 4
#define VEC4 8
#define MAT2X2 16
#define MAT3X3 32
#define MAT4X4 64
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
void normalize(void*,int);
void sub(void*,void*,void*,int);
void cross(void* dst, void* src1,void* src2, int type);