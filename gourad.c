#include "shader.h"
#include "tank.vertices.bin.h"
#include "tank.faces.bin.h"
Vec3 light_dir = {FIXEDPT_ONE,FIXEDPT_ONE,FIXEDPT_ONE};
Vec3       eye = {FIXEDPT_ONE,FIXEDPT_ONE,FIXEDPT_ONE*3};
Vec3    center = {0,0,0};
Vec3        up = {0,FIXEDPT_ONE,0};
static Vec3* varyingIntensity;
static Vec4 vertex(int iface, int nthvert){
    switch(nthvert&3){
        case(0):
        (*varyingIntensity).x = max(0,tank_vertices_bin_start[(iface*3)+nthvert]);
        break;
        case(1):
        (*varyingIntensity).y = max(0,tank_vertices_bin_start[(iface*3)+nthvert]);
        break;
        case(2):
        (*varyingIntensity).z = max(0,tank_vertices_bin_start[(iface*3)+nthvert]);
        break;
    }
    uint8_t face = tank_faces_bin_start[iface*3];
    Vec4 glVertex = {tank_vertices_bin_start[face],tank_vertices_bin_start[face+1],tank_vertices_bin_start[face+2], 0};
    return glVertex;
}
static bool fragment(Vec3 bar, uint16_t* color){
    fixedpt intensity = dot(&bar,varyingIntensity, VEC3);
    *color = fixedpt_mul(fixedpt_fromint(0xFFFF), intensity);
    return false; 

}
Shader Gourad = {
    &vertex,
    &fragment,   
};