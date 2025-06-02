#ifndef FACES_H
#define FACES_H
#include "matrix.h"
#include <stdio.h>
#define WIDTH 160
#define HEIGHT 120
#define TEXTURE_SZ 128
Vec3 baricentro(Vec3i[3], Vec3i);
void triangulo(Vec3i[3],int8_t*,uint16_t*, Vec2[2], const unsigned char*, const size_t);
#endif
