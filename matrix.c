#include "matrix.h"
void normalize(void *p, int type){
    fixedpt w;
    switch (type)
    {
        case VEC4:
        {
            Vec4* v =  (Vec4*)p;
            w = fixedpt_sqrt(fixedpt_mul(v->x,v->x)+fixedpt_mul(v->y,v->y)+fixedpt_mul(v->z,v->z)+fixedpt_mul(v->w,v->w));
            break;
        }
        case VEC3:
        {
            Vec3* v =  (Vec3*)p;
            w = fixedpt_sqrt(fixedpt_mul(v->x,v->x)+fixedpt_mul(v->y,v->y)+fixedpt_mul(v->z,v->z));
            break;
        }
        case VEC2:
        {
            Vec2* v =  (Vec2*)p;
            w = fixedpt_sqrt(fixedpt_mul(v->x,v->x)+fixedpt_mul(v->y,v->y));
            break;
        }
        default:
            break;
    }
    if (type == VEC4){
        ((Vec4*)p)->w = fixedpt_xdiv(((Vec4*)p)->w,w);
    }
    if(type == VEC4||type ==VEC3){
        ((Vec3*)p)->z = fixedpt_xdiv(((Vec3*)p)->z,w);
    }
    if(type == VEC4||type ==VEC3||type==VEC2){
        ((Vec2*)p)->x = fixedpt_xdiv(((Vec2*)p)->x,w);
        ((Vec2*)p)->y = fixedpt_xdiv(((Vec2*)p)->y,w); 
    }
}
void sub(void* dst, void* src1,void* src2, int type){
    if (type == VEC4){
        ((Vec4*)dst)->w = ((Vec4*)src1)->w-((Vec4*)src2)->w;
    }
    if(type == VEC4||type ==VEC3){
        ((Vec3*)dst)->z = ((Vec3*)src1)->z-((Vec3*)src2)->z;
    }
    if(type == VEC4||type ==VEC3||type==VEC2){
        ((Vec2*)dst)->x = ((Vec2*)src1)->x-((Vec2*)src2)->x;
        ((Vec2*)dst)->y = ((Vec2*)src1)->y-((Vec2*)src2)->y;  
    }
};
void cross(void* dst, void* src1,void* src2, int type){
    if(type == VEC3){
        ((Vec3*)dst)->x = fixedpt_xmul(((Vec3*)src1)->y,((Vec3*)src2)->z) - fixedpt_xmul(((Vec3*)src1)->z,((Vec3*)src2)->y);
        ((Vec3*)dst)->y = fixedpt_xmul(((Vec3*)src1)->z,((Vec3*)src2)->x) - fixedpt_xmul(((Vec3*)src1)->x,((Vec3*)src2)->z);
        ((Vec3*)dst)->z = fixedpt_xmul(((Vec3*)src1)->x,((Vec3*)src2)->y) - fixedpt_xmul(((Vec3*)src1)->y,((Vec3*)src2)->x);
    };
};