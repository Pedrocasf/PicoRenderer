#ifndef SHADER_H
#define SHADER_H
#include "matrix.h"
#include <stdbool.h>
typedef struct{
    Vec4 (*vertex)(int iface, int nthvert);
    bool (*fragment)(Vec3 bar, uint16_t* color);
}Shader;
#endif