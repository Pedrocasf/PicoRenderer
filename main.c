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
#define vga_mode vga_mode_160x120_60
const uint Left = 19;
const uint Right = 26;
void render();
void left();
void right();
static semaphore_t video_initted;
static uint16_t display[WIDTH*HEIGHT] = {0};
static fixedpt zbuffer[WIDTH*HEIGHT] = {0};
static Vec3i screen_coords[3] = {{0,0,0},{0,0,0},{0,0,0}};
static Vec3 v;
static uint8_t i;
static fixedpt intensity;
static fixedpt scale = FIXEDPT_ONE;
static fixedpt depth = FIXEDPT_ONE;
int main(void) {
    set_sys_clock_khz(250000,true);
    stdio_init_all();
    sem_init(&video_initted, 0, 1);
    multicore_launch_core1(render);
    memset(zbuffer, -0x80000000, sizeof zbuffer);
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
    for(i = 0;i<rock_faces_len;i+=3){
      Vec3 world_coords [3] = {{0,0,0},{0,0,0},{0,0,0}};
      uint8_t face[3] = {rock_faces[i],rock_faces[i+1],rock_faces[i+2]};
      for(uint8_t j = 0; j<3;j++){
          v.x = rock_vertices[(face[j]*3)];
          v.y = rock_vertices[(face[j]*3)+1];
          v.z = rock_vertices[(face[j]*3)+2] ;
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
        uint16_t r = fixedpt_toint(fixedpt_mul(intensity, fixedpt_fromint(31))); 
        uint16_t g = fixedpt_toint(fixedpt_mul(intensity, fixedpt_fromint(63)))<<5; 
        uint16_t b = fixedpt_toint(fixedpt_mul(intensity, fixedpt_fromint(31)))<<11; 
        triangulo(screen_coords,zbuffer,display,b|g|r);
      }
    }
    if(!gpio_get(Left)){
      //memset(zbuffer, -0x80000000, sizeof zbuffer);
      //memset(display, 0, sizeof display);
      scale -=1;                                                                                                                                                                                                                                            
    }
    if(!gpio_get(Right)){
      //memset(zbuffer, -0x80000000, sizeof zbuffer);
      //memset(display, 0, sizeof display);
      scale +=1;
    }
    int c = getchar_timeout_us(0);
    switch (c) {
      case ' ':
        for(int j = 0;j<3;j++){
          printf("x:%d\n", screen_coords[j].x);
          printf("y:%d\n", screen_coords[j].y);
          printf("z:%d\n", fixedpt_toint(screen_coords[j].z));
          printf("i:%d\n", intensity);
        }
          break;
      };
  }
}