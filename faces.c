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
        r.x = fixedpt_div(u.y,u.z);
        r.y = fixedpt_div(u.x,u.z);
        r.z = FIXEDPT_ONE-r.x-r.y;
    };
    return r;
};
void triangulo(Vec3i pts[3] ,int8_t* zbuffer,uint16_t*display, Vec2 uv[3], const unsigned char *tx, const size_t tx_len){
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
            Vec3 bc= baricentro(pts,p);
            if(bc.x>0&&bc.y>0&&bc.z>0){
                p.z = 0;
                p.z += fixedpt_mul(pts[1].z,bc.x);
                p.z += fixedpt_mul(pts[0].z,bc.y);
                p.z += fixedpt_mul(pts[2].z,bc.z);
                if(zbuffer[p.x + (p.y * WIDTH)]<p.z>>16){
                    Vec2 uvf = {
                        fixedpt_mul(bc.x,uv[0].x)+
                        fixedpt_mul(bc.y,uv[1].x)+
                        fixedpt_mul(bc.z,uv[2].x),
                        fixedpt_mul(bc.x,uv[0].y)+
                        fixedpt_mul(bc.y,uv[1].y)+
                        fixedpt_mul(bc.z,uv[2].y),
                    };
                    zbuffer[p.x + (p.y * WIDTH)] = p.z>>16;
                    size_t idx = tx_len - (fixedpt_toint(uvf.x) + ((fixedpt_toint(uvf.y)*TEXTURE_SZ))) * 2;
                    uint16_t val = ((uint16_t)(tx[idx+1])<<8) + (uint16_t)(tx[idx]);
                    display[p.x + (p.y * WIDTH)] = val;
                    //printf("%x ", val);
                }
            }
            p.y += 1;
        }
        p.x += 1;
    }
};