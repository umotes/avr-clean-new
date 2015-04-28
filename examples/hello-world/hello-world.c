/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         A very simple Contiki application showing how Contiki programs look
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include <avr/io.h>
#include "contiki.h"
//#include "dev/leds.h"
#include "dev/ssd1306.h"
#include "dev/temperature-sensor.h"
#include "dev/adc.h"
#include <stdio.h> /* For printf() */
#include <stdlib.h> /* For printf() */
/*---------------------------------------------------------------------------*/


PROCESS(hello_world_process, "Hello world process");
PROCESS(blink_process, "Blink portb4 led");
//static struct etimer et_blink;
//static uint16_t count;

//AUTOSTART_PROCESSES(&hello_world_process);
AUTOSTART_PROCESSES(&hello_world_process, &blink_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
  static struct etimer timer;
  static int count=0;
  etimer_set(&timer, CLOCK_CONF_SECOND);

  PROCESS_BEGIN();
  while(1)
 {
 PROCESS_WAIT_EVENT();
 
 if (ev==PROCESS_EVENT_TIMER)
   {

  // DDRB |= (1<<PORTB4);
  // PORTB ^= (1<<PORTB4);   

printf("Hello World # %i\n", count);
   count++;
//   etimer_reset(&timer);
 
  }

}

  
  PROCESS_END();
}

PROCESS_THREAD(blink_process, ev, data)
{

  static struct etimer et_blink;
  etimer_set(&et_blink, CLOCK_SECOND);
  PROCESS_BEGIN();
static char lux_value[6];
static char lux_value2[6];
static char raw_value[6];
static char raw_value2[6];
static  char temperature_val[6];
static  uint16_t  adc_data=0;
static  uint16_t  adc_data2=0;
static  float adc_lux=0;
static  float adc_lux2=0;
static  float measured_temp=0;
//  static int blinks = 0;

  ssd1306_clear(); 
  ssd1306_set_page_address(0);
  ssd1306_set_column_address(2);
  ssd1306_write_text("Lumen in Lux:");
   
  ssd1306_set_page_address(1);
  ssd1306_set_column_address(2);
  ssd1306_write_text("Raw ADC out :");
 
  ssd1306_set_page_address(2);
  ssd1306_set_column_address(2);
  ssd1306_write_text("Ambient Temperature measured:");


  while(1) {

    PROCESS_WAIT_EVENT();
if (ev==PROCESS_EVENT_TIMER)
   {
   DDRB |=(1<<PORTB4);
   PORTB ^= (1<<PORTB4);
   adc_init();
   adc_data=get_adc(0);
   adc_lux=adc_data*0.9765625;
     
 /*
   amps=adc_volt/10000.0;
   microamps=amps/1000000;
   lux_data=microamps*2;
*/
  itoa(adc_lux, lux_value, 10);
  ssd1306_set_page_address(0);
  ssd1306_set_column_address(73);
  ssd1306_write_text(lux_value);

  itoa(adc_data, raw_value, 10);
  ssd1306_set_page_address(1);
  ssd1306_set_column_address(73);
  ssd1306_write_text(raw_value);

  adc_init_full(ADC_CHAN_ADC0, ADC_TRIG_FREE_RUN, ADC_REF_AVCC, ADC_PS_64);
  adc_conversion_start();
 
  adc_lux2=adc_data2*0.9765626;

  itoa(adc_lux2, lux_value2, 10);
  ssd1306_set_page_address(0);
  ssd1306_set_column_address(95);
  ssd1306_write_text(lux_value2);

  itoa(adc_data2, raw_value2, 10);
  ssd1306_set_page_address(1);
  ssd1306_set_column_address(95);
  ssd1306_write_text(raw_value2);

  measured_temp=ReadTempVal();
  itoa(measured_temp, temperature_val, 10);
  ssd1306_set_page_address(2);
  ssd1306_set_column_address(95);
  ssd1306_write_text(temperature_val);

  }


  }

  PROCESS_END();
}

/*-------------------------------------------------------------------------*/
