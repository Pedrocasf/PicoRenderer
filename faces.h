#include <stdint.h>
#include "fixedpt.h"
#include <stdio.h>
#define WIDTH 160
#define HEIGHT 120
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
void normalize(Vec3*);
Vec3 sub(Vec3,Vec3);
Vec3 cross(Vec3,Vec3);
Vec3 baricentro(Vec3i[3], Vec3i);
void triangulo(Vec3i[3],fixedpt*,uint16_t*, uint16_t);
