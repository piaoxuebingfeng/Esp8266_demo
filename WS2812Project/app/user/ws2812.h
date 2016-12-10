#ifndef _WS2812_H
#define _WS2812_H

#define WSGPIO 0
#define LED_COUNT 24
#include "c_types.h"
#include "user_interface.h"
#include "ets_sys.h"
#include "gpio.h"


typedef struct
{
    uint8_t  r;            // [0,255]
    uint8_t  g;            // [0,255]
    uint8_t  b;            // [0,255]

} COLOR_RGB;
typedef struct
{
    uint8_t  g;            // [0,255]
    uint8_t  r;            // [0,255]
    uint8_t  b;            // [0,255]

} COLOR_GRB;
typedef struct//??hsv???
{
    int 	h;		// [0,360]
    float s;		// [0,1]
    float v;		// [0,1]
} COLOR_HSV;

typedef struct
{
    int h;         // [0,360]
    float s;       // [0,1]
    float l;       // [0,1]
} COLOR_HSL;


extern uint8_t rgb1[LED_COUNT][3];	//Array that will store color data
extern uint8_t led_Colors[LED_COUNT];
//You will have to 	os_intr_lock();  	os_intr_unlock();
void send24_GRB(u8 Green,u8 Red,u8 Blue,u8 count);
void WS2812OutBuffer( uint8_t * buffer, uint16_t length );
void send_GRB(u8 Green,u8 Red,u8 Blue);
void ws2812_send_data(uint8_t *led_Colors,uint16_t len);
void HSV_to_RGB(const COLOR_HSV *hsv,COLOR_RGB *rgb);
void rainbow_loop();
#endif

