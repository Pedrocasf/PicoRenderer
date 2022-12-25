#include "matrix.h"
void normalize(void *p, int type){
    fixedpt w;
    switch (type){
        case VEC4:{
            Vec4* v =  (Vec4*)p;
            w = fixedpt_sqrt(fixedpt_mul(v->x,v->x)+fixedpt_mul(v->y,v->y)+fixedpt_mul(v->z,v->z)+fixedpt_mul(v->w,v->w));
            break;
        }case VEC3:{
            Vec3* v =  (Vec3*)p;
            w = fixedpt_sqrt(fixedpt_mul(v->x,v->x)+fixedpt_mul(v->y,v->y)+fixedpt_mul(v->z,v->z));
            break;
        }case VEC2:{
            Vec2* v =  (Vec2*)p;
            w = fixedpt_sqrt(fixedpt_mul(v->x,v->x)+fixedpt_mul(v->y,v->y));
            break;
        }default:
            break;
    }
    switch (type){
        case VEC4:
            ((Vec4*)p)->w = fixedpt_xdiv(((Vec4*)p)->w,w);
        case VEC3:
            ((Vec3*)p)->z = fixedpt_xdiv(((Vec3*)p)->z,w);
        case VEC2:
            ((Vec2*)p)->x = fixedpt_xdiv(((Vec2*)p)->x,w);
            ((Vec2*)p)->y = fixedpt_xdiv(((Vec2*)p)->y,w); 
        default:
            break;
    }
}
void mul(void* dst, void* src1,void* src2, int type){
    switch (type){
        case VEC4:
            ((Vec4*)dst)->w = fixedpt_xmul(((Vec4*)src1)->w,((Vec4*)src2)->w);
        case VEC3:
            ((Vec3*)dst)->z = fixedpt_xmul(((Vec3*)src1)->z,((Vec3*)src2)->z);
        case VEC2:
            ((Vec2*)dst)->y = fixedpt_xmul(((Vec2*)src1)->y,((Vec2*)src2)->y);
            ((Vec2*)dst)->x = fixedpt_xmul(((Vec2*)src1)->x,((Vec2*)src2)->x);
            break;
        case MAT4:
            mul(&((Mat4*)dst)->w,&((Mat4*)src1)->w,&((Mat4*)src2)->w, VEC4);
            ((Mat4*)dst)->x.w = fixedpt_xmul(((Mat4*)src1)->x.w,((Mat4*)src2)->x.w);
            ((Mat4*)dst)->y.w = fixedpt_xmul(((Mat4*)src1)->y.w,((Mat4*)src2)->y.w);
            ((Mat4*)dst)->z.w = fixedpt_xmul(((Mat4*)src1)->z.w,((Mat4*)src2)->z.w);
        case MAT3:
            mul(&((Mat3*)dst)->z,&((Mat3*)src1)->z,&((Mat3*)src2)->z, VEC3);
            ((Mat3*)dst)->x.z = fixedpt_xmul(((Mat3*)src1)->x.z,((Mat3*)src2)->x.z);
            ((Mat3*)dst)->y.z = fixedpt_xmul(((Mat3*)src1)->y.z,((Mat3*)src2)->y.z);
        case MAT2:
            mul(&((Mat2*)dst)->y,&((Mat2*)src1)->y,&((Mat2*)src2)->y, VEC2);
            mul(&((Mat2*)dst)->x,&((Mat2*)src1)->x,&((Mat2*)src2)->x, VEC2);
        default:
            break;
    }
}
fixedpt dot(void* src1,void* src2, int type){
    fixedpt s = 0;
    switch (type){
        case VEC4:
            s += fixedpt_xmul(((Vec4*)src1)->w,((Vec4*)src2)->w);
        case VEC3:
            s += fixedpt_xmul(((Vec3*)src1)->z,((Vec3*)src2)->z);
        case VEC2:
            s += fixedpt_xmul(((Vec2*)src1)->y,((Vec2*)src2)->y);
            s += fixedpt_xmul(((Vec2*)src1)->x,((Vec2*)src2)->x); 
        default:
            return s;
    }
}
void sub(void* dst, void* src1,void* src2, int type){
    switch (type){
        case VEC4:
            ((Vec4*)dst)->w = ((Vec4*)src1)->w-((Vec4*)src2)->w;
        case VEC3:
            ((Vec3*)dst)->z = ((Vec3*)src1)->z-((Vec3*)src2)->z;
        case VEC2:
            ((Vec2*)dst)->x = ((Vec2*)src1)->x-((Vec2*)src2)->x;
            ((Vec2*)dst)->y = ((Vec2*)src1)->y-((Vec2*)src2)->y;
        default:
            break;
    }
};
void cross(void* dst, void* src1,void* src2, int type){
    if(type == VEC3){
        ((Vec3*)dst)->x = fixedpt_xmul(((Vec3*)src1)->y,((Vec3*)src2)->z) - fixedpt_xmul(((Vec3*)src1)->z,((Vec3*)src2)->y);
        ((Vec3*)dst)->y = fixedpt_xmul(((Vec3*)src1)->z,((Vec3*)src2)->x) - fixedpt_xmul(((Vec3*)src1)->x,((Vec3*)src2)->z);
        ((Vec3*)dst)->z = fixedpt_xmul(((Vec3*)src1)->x,((Vec3*)src2)->y) - fixedpt_xmul(((Vec3*)src1)->y,((Vec3*)src2)->x);
    };
    if(type == VEC3I){
        ((Vec3i*)dst)->x = (((Vec3i*)src1)->y*((Vec3i*)src2)->z) - (((Vec3i*)src1)->z*((Vec3i*)src2)->y);
        ((Vec3i*)dst)->y = (((Vec3i*)src1)->z*((Vec3i*)src2)->x) - (((Vec3i*)src1)->x*((Vec3i*)src2)->z);
        ((Vec3i*)dst)->z = (((Vec3i*)src1)->x*((Vec3i*)src2)->y) - (((Vec3i*)src1)->y*((Vec3i*)src2)->x);
    };
};