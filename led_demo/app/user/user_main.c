/*
 * File	: user_main.c
 * This file is part of Espressif's AT+ command set program.
 * Copyright (C) 2013 - 2016, Espressif Systems
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of version 3 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
//#include "ets_sys.h"
//#include "driver/uart.h"
//#include "osapi.h"
//#include "at.h"
//
//extern uint8_t at_wifiMode;
//extern void user_esp_platform_load_param(void *param, uint16 len);
//
//void user_init(void)
//{
//  uint8_t userbin;
//  uint32_t upFlag;
//  at_uartType tempUart;
//
//  user_esp_platform_load_param((uint32 *)&tempUart, sizeof(at_uartType));
//  if(tempUart.saved == 1)
//  {
//    uart_init(tempUart.baud, BIT_RATE_115200);
//  }
//  else
//  {
//    uart_init(BIT_RATE_115200, BIT_RATE_115200);
//  }
//  at_wifiMode = wifi_get_opmode();
//  os_printf("\r\nready!!!\r\n");
//  uart0_sendStr("\r\nready\r\n");
//  at_init();
//}


// led test
#include "ets_sys.h"
#include "gpio.h"
#include "osapi.h"
#include "os_type.h"
#include "user_interface.h"
#include "espconn.h"

LOCAL os_timer_t timer;
char zt = 1;

void timer_callback(){
    if(zt == 1){
        GPIO_OUTPUT_SET(GPIO_ID_PIN(2), 0);
        zt = 0;
    }else{
        GPIO_OUTPUT_SET(GPIO_ID_PIN(2), 1);
        zt = 1;
    }
}

void user_init(void)
{
     //os_printf("SDK version:%s\n", system_get_sdk_version());
     PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U,FUNC_GPIO2);

     os_timer_disarm(&timer);
     os_timer_setfn(&timer,(os_timer_func_t *)timer_callback,NULL);
     os_timer_arm(&timer,1000,1);


}

void user_rf_pre_init(){}
