#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/scanvideo.h"
#include "hardware/irq.h"
#include "pico/scanvideo/composable_scanline.h"
#include "pico/sync.h"
#include "fixedpt.h"
#include "models.h"
#include "faces.h"
#include "tank.faces.bin.h"
#include "tank.uvs.bin.h"
#include "tank.vertices.bin.h"
#include "tank.txc.bin.h"
#include "texture.raw.h"
#include "hardware/clocks.h"
#define vga_mode vga_mode_160x120_60
const uint Left = 19;
const uint Right = 26;
void render();
void left();
void right();
static semaphore_t video_initted;
static uint16_t display[WIDTH*HEIGHT] = {0};
static int8_t zbuffer[WIDTH*HEIGHT] = {0};
static Vec3i screen_coords[3] = {{0,0,0},{0,0,0},{0,0,0}};
static Vec3 v;
static uint8_t i;
static fixedpt intensity;
static fixedpt scale = -FIXEDPT_ONE_HALF;
static fixedpt depth = FIXEDPT_ONE;
int main(void) {
    set_sys_clock_khz(250000,true);
    stdio_init_all();
    sem_init(&video_initted, 0, 1);
    memset(zbuffer, -128, sizeof zbuffer);
    multicore_launch_core1(render);
    sem_acquire_blocking(&video_initted);
    printf("Program started");
    while (true) {
      scanvideo_wait_for_vblank();
      for (int y = 0;y<vga_mode.height;y++){
        scanvideo_scanline_buffer_t *scanline_buffer = scanvideo_begin_scanline_generation(true);
        uint16_t *p = (uint16_t *) scanline_buffer->data;
        *p++ = COMPOSABLE_RAW_RUN;
        *p++ = display[y* WIDTH];
        *p++ = vga_mode.width;
        memcpy(p, &display[y*WIDTH], sizeof(display[y])*WIDTH);
        p[vga_mode.width+3]  = COMPOSABLE_EOL_SKIP_ALIGN;
        p[vga_mode.width+4]  = 0;
        scanline_buffer->data_used = (vga_mode.width + 6)>>1;
        assert(scanline_buffer->data_used < scanline_buffer->data_max);
        scanline_buffer->status = SCANLINE_OK;
        scanvideo_end_scanline_generation(scanline_buffer);
      }
    }
}
void render(){
  scanvideo_setup(&vga_mode);
  scanvideo_timing_enable(true);
  multicore_fifo_clear_irq();
  stdio_init_all();
  sem_release(&video_initted);
  gpio_init(Left);
  gpio_set_dir(Left, GPIO_IN);
  gpio_pull_up(Left);
  gpio_init(Right);
  gpio_set_dir(Right, GPIO_IN);
  gpio_pull_up(Right);
  Vec3 l = {0,0,FIXEDPT_ONE};
  while (1){
    for(i = 0;i<tank_faces_bin_size;i+=3){
      Vec3 world_coords [3] = {{0,0,0},{0,0,0},{0,0,0}};
      uint8_t face[3] = {tank_faces_bin_start[i],tank_faces_bin_start[i+1],tank_faces_bin_start[i+2]};
      uint8_t txc[3] = {tank_txc_bin_start[i], tank_txc_bin_start[i+1], tank_txc_bin_start[i+2]};
      Vec2 uv[3];
      for(uint8_t j = 0; j<3;j++){
          v.x = fixedpt_mul(tank_vertices_bin_start[(face[j]*3)], scale);
          v.y = fixedpt_mul(tank_vertices_bin_start[(face[j]*3)+1], scale);
          v.z = fixedpt_mul(tank_vertices_bin_start[(face[j]*3)+2], scale);
          uv[j].x = fixedpt_fromint(tank_uvs_bin_start[txc[j]*2]);
          uv[j].y = fixedpt_fromint(tank_uvs_bin_start[(txc[j]*2)+1]);
          //printf("%x %x", uv[j].x,uv[j].y);
          screen_coords[j] = (Vec3i){
            fixedpt_toint(fixedpt_mul(v.x+FIXEDPT_ONE,fixedpt_fromint(WIDTH>>1))),
            fixedpt_toint(fixedpt_mul(v.y+FIXEDPT_ONE,fixedpt_fromint(HEIGHT>>1))),
            v.z
          };
          world_coords[j] = v;
      }
      Vec3 n,n0,n1;
      sub(&n0,&world_coords[0], &world_coords[2], VEC3);
      sub(&n1,&world_coords[1], &world_coords[0], VEC3);
      cross(&n,&n0,&n1,VEC3);
      normalize(&n, VEC3);
      intensity = fixedpt_mul(n.x,l.x)+fixedpt_mul(n.y,l.y)+fixedpt_mul(n.z,l.z);
      if(intensity > 0){
        triangulo(screen_coords,zbuffer,display,uv,texture_raw_start, texture_raw_size);
      }
    }
    if(!gpio_get(Left)){
      memset(zbuffer, -128, sizeof zbuffer);
      memset(display, 0, sizeof display);
      scale +=FIXEDPT_ONE_HALF;                                                                                                                                                                                                                                            
    }
    if(!gpio_get(Right)){
      memset(zbuffer, -128, sizeof zbuffer);
      memset(display, 0, sizeof display);
      scale -=FIXEDPT_ONE_HALF>>1;
    }
  }
}
