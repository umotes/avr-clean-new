/** 
 * \brief Compiler and Platform specific definitions and typedefs common to
 * all platforms.  
 *
 * platform-common.h provides PLATFORM_HEADER defaults and common definitions.
 * This head should never be included directly, it should only be included
 * by the specific PLATFORM_HEADER used by your platform.
 *
 * See platform-common.h for source code.
 *@{
 */

/**
 * @addtogroup stm32w-cpu
 * @{ */

/** \file hal/micro/generic/compiler/platform-common.h
 * See \ref platform_common for detailed documentation.
 *
 * <!--(C) COPYRIGHT 2010 STMicroelectronics. All rights reserved.        -->
 */


#ifndef PLATFORMCOMMON_H_
#define PLATFORMCOMMON_H_
////////////////////////////////////////////////////////////////////////////////
// Many of the common definitions must be explicitly enabled by the 
//  particular PLATFORM_HEADER being used
////////////////////////////////////////////////////////////////////////////////

//#include <avr/io.h>
#include "dev/compiler.h"
#include <avr/io.h>

#define TRUE  1
#define HIGH 1
#define FALSE 0
#define LOW 0
#ifndef NULL
#define NULL ((void *)0)
#endif

#define BIT(x) (1 << (x))  // Unsigned avoids compiler warnings re BIT(15)

#define SETBIT(reg, bit)      reg |= BIT(bit)
#define SETBITS(reg, bits)    reg |= (bits)
#define CLEARBIT(reg, bit)    reg &= ~(BIT(bit))
#define CLEARBITS(reg, bits)  reg &= ~(bits)
#define READBIT(reg, bit)     (reg & (BIT(bit)))
#define READBITS(reg, bits)   (reg & (bits))

#define EXT_3

//======================User Button Assignment======================

#define  BUTTOM_SW_PORT  PORTB
#define  BUTTON_SW_PIN   PB4
#define  BUTTON_SW_DIR   DDRB

//=================SPI PORT Configuration for OLED Disply===========

#ifdef EXT_1
#define SSD_MOSI_PORT		  PORTB
#define SSD_MOSI_DIR		  DDRB
#define SSD_MOSI_PIN		  PORTB2

#define SSD_CLK_PORT		  PORTB
#define SSD_CLK_DIR 		  DDRB
#define SSD_CLK_PIN 		  PORTB1

#define SSD_CS_PORT               PORTG
#define SSD_CS_DIR                DDRG
#define SSD_CS_PIN                PORTG0

#define SSD_RES_PORT              PORTD
#define SSD_RES_DIR               DDRD
#define SSD_RES_PIN               PORTD5

#define SSD_DC_PORT               PORTE
#define SSD_DC_DIR                DDRE
#define SSD_DC_PIN                PORTE2

#elif EXT_3

#define SSD_MOSI_PORT             PORTB
#define SSD_MOSI_DIR              DDRB
#define SSD_MOSI_PIN              PORTB2

#define SSD_CLK_PORT              PORTB
#define SSD_CLK_DIR               DDRB
#define SSD_CLK_PIN               PORTB1

#define SSD_RES_PORT              PORTD
#define SSD_RES_DIR               DDRD
#define SSD_RES_PIN               PORTD7

#define SSD_DC_PORT               PORTB
#define SSD_DC_DIR                DDRB
#define SSD_DC_PIN                PORTB7

#define SSD_CS_PORT               PORTG
#define SSD_CS_DIR                DDRG
#define SSD_CS_PIN                PORTG2
#endif


//========= RESOURCES AVAILABLE ON SENSOR BOARD================//



#define PLATFORM_HAS_BATTERY                    1
#define PLATFORM_HAS_BUTTON                     1
#define PLATFORM_HAS_LEDS                       1
#define PLATFORM_HAS_TEMP_SENSOR_1              1
#define PLATFORM_HAS_TEMP_SENSOR_2              1
#define PLATFORM_HAS_LIGHT_SENSOR               1
#define PLATFORM_HAS_RADIO                      1



/** @} */
#endif
