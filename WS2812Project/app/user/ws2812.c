#include "ws2812.h"
#include "ets_sys.h"
#include "osapi.h"

#define GPIO_OUTPUT_SET(gpio_no, bit_value) \
	gpio_output_set(bit_value<<gpio_no, ((~bit_value)&0x01)<<gpio_no, 1<<gpio_no,0)

uint8_t rgb1[LED_COUNT][3];						//Array that will store color data
uint8_t led_Colors[LED_COUNT];
//I just used a scope to figure out the right time periods.
void Delay_ms(uint16_t ms)
{
	uint16_t i,j;
	for(i=ms;i>0;i--)
		for(j=0;j<60000;j++);
}
void  SEND_WS_0()
{
	uint8_t time;
	time = 3; while(time--) WRITE_PERI_REG( PERIPHS_GPIO_BASEADDR + GPIO_ID_PIN(WSGPIO), 1 );
	time = 8; while(time--) WRITE_PERI_REG( PERIPHS_GPIO_BASEADDR + GPIO_ID_PIN(WSGPIO), 0 );
}

void  SEND_WS_1()
{
	uint8_t time; 
	time = 7; while(time--) WRITE_PERI_REG( PERIPHS_GPIO_BASEADDR + GPIO_ID_PIN(WSGPIO), 1 );
	time = 5; while(time--) WRITE_PERI_REG( PERIPHS_GPIO_BASEADDR + GPIO_ID_PIN(WSGPIO), 0 );
}

void SEND_RESET()
{
	uint8_t time=52;
	WRITE_PERI_REG( PERIPHS_GPIO_BASEADDR + GPIO_ID_PIN(WSGPIO), 1 );
    while(time--) WRITE_PERI_REG( PERIPHS_GPIO_BASEADDR + GPIO_ID_PIN(WSGPIO), 0 );
    WRITE_PERI_REG( PERIPHS_GPIO_BASEADDR + GPIO_ID_PIN(WSGPIO), 1 );
}
void HSV_to_RGB(const COLOR_HSV *hsv,COLOR_RGB *rgb)
{
    int   h = hsv->h;
    float s = hsv->s;
    float v = hsv->v;
    float r = 0.0;
    float b = 0.0;
    float g = 0.0;
    int red,green,blue;

    u8 flag = ( int) abs( h / 60.0 );
    float f = h / 60.0 - flag;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);

    switch ( flag)
    {
    case 0:
        b = p;
        g = t;
        r = v;
        break;
    case 1:
        b = p;
        g = v;
        r = q;
        break;
    case 2:
        b = t;
        g = v;
        r = p;
        break;
    case 3:
        b = v;
        g = q;
        r = p;
        break;
    case 4:
        b = v;
        g = p;
        r = t;
        break;
    case 5:
        b = q;
        g = p;
        r = v;
        break;
    default:
        break;
    }

    blue = (int)( b * 255);
    rgb->b = ( blue >= 255) ? 255 : blue;
    rgb->b = ( blue <= 0) ? 0 : rgb->b;

    green = (int)( g * 255);
    rgb->g = ( green >= 255) ? 255 : green;
    rgb->g = ( green <= 0) ? 0 :rgb->g;

    red = (int)( r * 255);
    rgb->r = ( red >= 255) ? 255 : red;
    rgb->r = ( red <= 0) ? 0 : rgb->r;
}

void send_GRB(u8 Green,u8 Red,u8 Blue)
{
	u8 color_i;
	for(color_i=0;color_i<8;color_i++)
	{
		if(((Green&0x80)>>7)==1)  //1000 0000
			SEND_WS_1();
		else
			SEND_WS_0();
		Green=Green<<1;
	}
	for(color_i=0;color_i<8;color_i++)
	{
		if(((Red&0x80)>>7)==1)
			SEND_WS_1();
		else
			SEND_WS_0();
		Red=Red<<1;
	}
	for(color_i=0;color_i<8;color_i++)
	{
		if(((Blue&0x80)>>7)==1)
			SEND_WS_1();
		else
			SEND_WS_0();
		Blue=Blue<<1;
	}
	GPIO_OUTPUT_SET(GPIO_ID_PIN(WSGPIO), 0);
}
void send24_GRB(u8 Green,u8 Red,u8 Blue,u8 count)
{
	u8 i;
	GPIO_OUTPUT_SET(GPIO_ID_PIN(WSGPIO), 0);
	ets_intr_lock();
	for(i=0;i<count;i++)
		{
			send_GRB(Green,Red,Blue);
		}
	SEND_RESET();
	GPIO_OUTPUT_SET(GPIO_ID_PIN(WSGPIO), 0);
	ets_intr_unlock();
}
void ws2812_send_data(uint8_t *led_Colors,uint16_t len)
{
    uint8_t i, j = 0;
    uint16_t buffersize = (24 * len) + 42, memaddr = 0;
    uint8_t temp, led = 0;
	GPIO_OUTPUT_SET(GPIO_ID_PIN(WSGPIO), 0);
	ets_intr_lock();
    while(len)
    {
        for (i = 0; i < 3; i++)  						// Set RGB LED color R -> i=0, G -> i=1, B -> i=2
        {
            temp = rgb1[led_Colors[led]][i];
            for (j = 0; j < 8; j++)  					// Set 8 bits of color
            {
                if ((temp) & 0x80)  					// Data sent MSB first, j = 0 is MSB j = 7 is LSB
                {
                	 SEND_WS_1();
                }
                else
                {
                	 SEND_WS_0();
                }
                temp = temp << 1;
            }
        }

        led++;
        len--;
    }
	ets_intr_unlock();
}
void rainbow_loop()
{
	COLOR_HSV hsv;
	COLOR_RGB rgb;
	uint16_t i,j,k;

    hsv.s = 1;
    hsv.v = 0.1;

    for(i = 0; i < 360; i++)
    {
		for(j = 0; j < 24 ; j ++)
        {
                hsv.h = (i + j) % 360;
                hsv.s = 1;
                hsv.v = 0.5;
                HSV_to_RGB(&hsv, &rgb);
                rgb1[j][0] = rgb.r;// (uint8_t)floor(r/20+j);
                rgb1[j][1] = rgb.g;//(uint8_t)floor(g/20+j);
                rgb1[j][2] = rgb.b;//(uint8_t)floor(b/20+j);
                led_Colors[j] = j;
        }

		ws2812_send_data(led_Colors, LED_COUNT);
		Delay_ms(50000);
    }
}
void WS2812OutBuffer( uint8_t * buffer, uint16_t length )
{
	uint16_t i;
	GPIO_OUTPUT_SET(GPIO_ID_PIN(WSGPIO), 0);

	ets_intr_lock(); 

	for( i = 0; i < length; i++ )
	{
		uint8_t mask = 0x80;
		uint8_t byte = buffer[i];
		while (mask) 
		{
			if( byte & mask ) SEND_WS_1(); else SEND_WS_0();
			mask >>= 1;
        }
	}

	ets_intr_unlock(); 
}



