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
// WS2812 test
#include "ets_sys.h"
#include "gpio.h"
#include "osapi.h"
#include "os_type.h"
#include "user_interface.h"
#include "espconn.h"
#include "ws2812.h"

LOCAL os_timer_t timer;

uint16_t i=1;
uint16_t j=1;

void timer_callback()
{
	i++;
	j++;
	send24_GRB(i,j,i+j,24);
	if(i>120)
		i=0;
	if(j>120)
		j=0;
}
void user_init(void)
{

	//send24_GRB(0,0,0,24);
    os_timer_disarm(&timer);
    os_timer_setfn(&timer,(os_timer_func_t *)timer_callback,NULL);
    os_timer_arm(&timer,20,1);
}
void user_rf_pre_init(){}
