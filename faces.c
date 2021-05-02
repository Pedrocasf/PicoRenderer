#include "faces.h"
Vec3 baricentro(Vec3i pts[3], Vec3i p){
    Vec3 s[2] = {{0,0,0},{0,0,0}};
    Vec3 r;
    s[1].x = fixedpt_fromint(pts[2].y - pts[0].y);
    s[1].y = fixedpt_fromint(pts[1].y - pts[0].y);
    s[1].z = fixedpt_fromint(pts[0].y - p.y);

    s[0].x = fixedpt_fromint(pts[2].x - pts[0].x);
    s[0].y = fixedpt_fromint(pts[1].x - pts[0].x);
    s[0].z = fixedpt_fromint(pts[0].x - p.x);
    
    Vec3 u;
    cross(&u,&s[0], &s[1],VEC3);
    if(fixedpt_abs(u.z)<0){
        r.x = -FIXEDPT_ONE;
        r.y = FIXEDPT_ONE;
        r.z = FIXEDPT_ONE;
    }else{
        r.x = FIXEDPT_ONE-fixedpt_div(u.x+u.y, u.z);
        r.y = fixedpt_div(u.y,u.z);
        r.z = fixedpt_div(u.x,u.z);
    };
    return r;
};
fixedpt max(fixedpt x, fixedpt y){
  return (x > y) ? x : y;
};
fixedpt min(fixedpt x, fixedpt y)
{
  return (x < y) ? x : y;
}
void triangulo(Vec3i pts[3] ,fixedpt* zbuffer,uint16_t*display, uint16_t cor){
    Vec2i bboxmin, bboxmax, clamp;
    bboxmin.x = 0x7FFFFFFF;
    bboxmin.y = 0x7FFFFFFF;
    bboxmax.x = -0x7FFFFFFF;;
    bboxmax.y = -0x7FFFFFFF;;
    clamp.x = WIDTH-1;
    clamp.y = HEIGHT-1;
    for (int i = 0;i<3;i++){
        bboxmin.x  = max(0,min(bboxmin.x,pts[i].x));
        bboxmax.x = min(clamp.x,max(bboxmax.x,pts[i].x));

        bboxmin.y  = max(0,min(bboxmin.y,pts[i].y));
        bboxmax.y = min(clamp.y,max(bboxmax.y,pts[i].y));
    };
    Vec3i p = {0,0,0};
    while (p.x <= bboxmax.x){
        p.y = bboxmin.y;
        while (p.y <= bboxmax.y){
            Vec3 bc_screen= baricentro(pts,p);
            if(bc_screen.x>0&&bc_screen.y>0&&bc_screen.z>0){
                p.z = 0;
                p.z += fixedpt_mul(pts[1].z,bc_screen.x);
                p.z += fixedpt_mul(pts[0].z,bc_screen.y);
                p.z += fixedpt_mul(pts[2].z,bc_screen.z);
                if(zbuffer[p.x + (p.y * WIDTH)]<p.z){
                    zbuffer[p.x + (p.y * WIDTH)] = p.z;
                    display[p.x + (p.y * WIDTH)] = cor;
                }
            }
            p.y += 1;
        }
        p.x += 1;
    }
};