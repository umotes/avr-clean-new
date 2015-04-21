/*   Copyright (c) 2009, Swedish Institute of Computer Science
 *  All rights reserved. 
 *
 *  Additional fixes for AVR contributed by:
 *
 *	Colin O'Flynn coflynn@newae.com
 *	Eric Gnoske egnoske@gmail.com
 *	Blake Leverett bleverett@gmail.com
 *	Mike Vidales mavida404@gmail.com
 *	Kevin Brown kbrown3@uccs.edu
 *	Nate Bohlmann nate@elfwerks.com
 *	David Kopf dak664@embarqmail.com
 *  Ivan Delamer delamer@ieee.com
 *
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of the copyright holders nor the names of
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 * 
*/

/**
 *   \addtogroup wireless
 *  @{
*/

/**
 *   \defgroup hal RF230 hardware level drivers
 *   @{
 */

/**
 *  \file
 *  This file contains low-level radio driver code.
 *  This version is optimized for use with the "barebones" RF230bb driver,
 *  which communicates directly with the contiki core MAC layer.
 *  It is optimized for speed at the expense of generality.
 */
#include "contiki-conf.h"
#if DEBUGFLOWSIZE
extern uint8_t debugflowsize,debugflow[DEBUGFLOWSIZE];
#define DEBUGFLOW(c) if (debugflowsize<(DEBUGFLOWSIZE-1)) debugflow[debugflowsize++]=c
#else
#define DEBUGFLOW(c)
#endif


/*============================ INCLUDE =======================================*/
#include <stdlib.h>

#include "hal.h"

#include <avr/io.h>
#include "atmega256rfr2_registermap.h"

/*============================ VARIABLES =====================================*/

volatile extern signed char rf230_last_rssi;

/*============================ CALLBACKS =====================================*/


/*============================ IMPLEMENTATION ================================*/

/* AVR1281 with internal RF231 radio */
#include <avr/interrupt.h>

 
/** \brief  This function initializes the Hardware Abstraction Layer.
 */

void
hal_init(void)
{
    /*Reset variables used in file.*/
    /* (none at the moment) */
}



/* Hack for internal radio registers. hal_register_read and hal_register_write are
   handled through defines, but the preprocesser can't parse a macro containing
   another #define with multiple arguments, e.g. using
   #define hal_subregister_read( address, mask, position ) (address&mask)>>position
   #define SR_TRX_STATUS         TRX_STATUS, 0x1f, 0
   the following only sees 1 argument to the macro
   return hal_subregister_read(SR_TRX_STATUS);
   
   Possible fix is through two defines:
   #define x_hal_subregister_read(x) hal_subregister_read(x);
   #define hal_subregister_read( address, mask, position ) (address&mask)>>position
   but the subregister defines in atmega128rfa1_registermap.h are currently set up without
   the _SFR_MEM8 attribute, for use by hal_subregister_write.
   
 */
uint8_t
hal_subregister_read(uint16_t address, uint8_t mask, uint8_t position)
{
    return (_SFR_MEM8(address)&mask)>>position;
}
void
hal_subregister_write(uint16_t address, uint8_t mask, uint8_t position,
                            uint8_t value)
{
    HAL_ENTER_CRITICAL_REGION();

    uint8_t register_value = _SFR_MEM8(address);
    register_value &= ~mask;
    value <<= position;
    value &= mask;
    value |= register_value;
    _SFR_MEM8(address) = value;

    HAL_LEAVE_CRITICAL_REGION();
}

/*----------------------------------------------------------------------------*/
/** \brief  Transfer a frame from the radio transceiver to a RAM buffer
 *
 *          This version is optimized for use with contiki RF230BB driver.
 *          The callback routine and CRC are left out for speed in reading the rx buffer.
 *          Any delays here can lead to overwrites by the next packet!
 *
 *          If the frame length is out of the defined bounds, the length, lqi and crc
 *          are set to zero.
 *
 *  \param  rx_frame    Pointer to the data structure where the frame is stored.
 */
void
hal_frame_read(hal_rx_frame_t *rx_frame)
{

    uint8_t frame_length,*rx_data,*rx_buffer;
 
    /* Get length from the TXT_RX_LENGTH register, not including LQI
     * Bypassing the length check can result in overrun if buffer is < 256 bytes.
     */
    frame_length = TST_RX_LENGTH;
    if ((frame_length < HAL_MIN_FRAME_LENGTH) || (frame_length > HAL_MAX_FRAME_LENGTH)) {
        /* Length test failed */
        rx_frame->length = 0;
        rx_frame->lqi    = 0;
        rx_frame->crc    = false;
        return;
    }
    rx_frame->length = frame_length;

    /* Start of buffer in I/O space, pointer to RAM buffer */
    rx_buffer=(uint8_t *)0x180;
    rx_data = (rx_frame->data);

    do{
        *rx_data++ = _SFR_MEM8(rx_buffer++);
    } while (--frame_length > 0);

    /*Read LQI value for this frame.*/
    rx_frame->lqi = *rx_buffer;

    /* If crc was calculated set crc field in hal_rx_frame_t accordingly.
     * Else show the crc has passed the hardware check.
     */
    rx_frame->crc   = true;
    
}

/*----------------------------------------------------------------------------*/
/** \brief  This function will download a frame to the radio transceiver's frame
 *          buffer.
 *
 *  \param  write_buffer    Pointer to data that is to be written to frame buffer.
 *  \param  length          Length of data. The maximum length is 127 bytes.
 */
void
hal_frame_write(uint8_t *write_buffer, uint8_t length)
{
    uint8_t *tx_buffer;
    tx_buffer=(uint8_t *)0x180;  //start of fifo in i/o space
    /* Write frame length, including the two byte checksum */
    /* The top bit of the length field shall be set to 0 for IEEE 802.15.4 compliant frames */
    /* It should already be clear, so bypassing the masking is sanity check of the uip stack */
//  length &= 0x7f;
    _SFR_MEM8(tx_buffer++) = length;
    
    /* Download to the Frame Buffer.
     * When the FCS is autogenerated there is no need to transfer the last two bytes
     * since they will be overwritten.
     */
#if !RF230_CONF_CHECKSUM
    length -= 2;
#endif
    do  _SFR_MEM8(tx_buffer++)= *write_buffer++; while (--length);

}


/*----------------------------------------------------------------------------*/
/* This #if compile switch is used to provide a "standard" function body for the */
/* doxygen documentation. */
#if defined(DOXYGEN)
/** \brief ISR for the radio IRQ line, triggered by the input capture.
 *  This is the interrupt service routine for timer1.ICIE1 input capture.
 *  It is triggered of a rising edge on the radio transceivers IRQ line.
 */
void RADIO_VECT(void);
#else  /* !DOXYGEN */
/* These link to the RF230BB driver in rf230.c */
void rf230_interrupt(void);

extern hal_rx_frame_t rxframe[RF230_CONF_RX_BUFFERS];
extern uint8_t rxframe_head,rxframe_tail;

/* rf230interruptflag can be printed in the main idle loop for debugging */
#define DEBUG 0
#if DEBUG
volatile char rf230interruptflag;
#define INTERRUPTDEBUG(arg) rf230interruptflag=arg
#else
#define INTERRUPTDEBUG(arg)
#endif

/* The atmega128rfa1 has individual interrupts for the integrated radio'
 * Whichever are enabled by the RF230 driver must be present even if not used!
 */
/* Received packet interrupt */
ISR(TRX24_RX_END_vect)
{
/* Get the rssi from ED if extended mode */
#if RF230_CONF_AUTOACK
	rf230_last_rssi=hal_register_read(RG_PHY_ED_LEVEL);
#endif

/* Buffer the frame and call rf230_interrupt to schedule poll for rf230 receive process */
/* Is a ram buffer available? */
	if (rxframe[rxframe_tail].length) {DEBUGFLOW('0');} else /*DEBUGFLOW('1')*/;

#ifdef RF230_MIN_RX_POWER		 
/* Discard packets weaker than the minimum if defined. This is for testing miniature meshes */
/* This does not prevent an autoack. TODO:rfa1 radio can be set up to not autoack weak packets */
	if (rf230_last_rssi >= RF230_MIN_RX_POWER) {
#else
	if (1) {
#endif
//		DEBUGFLOW('2');
		hal_frame_read(&rxframe[rxframe_tail]);
		rxframe_tail++;if (rxframe_tail >= RF230_CONF_RX_BUFFERS) rxframe_tail=0;
		rf230_interrupt();
	}
}
/* Preamble detected, starting frame reception */
ISR(TRX24_RX_START_vect)
{
//	DEBUGFLOW('3');
/* Save RSSI for this packet if not in extended mode, scaling to 1dB resolution */
#if !RF230_CONF_AUTOACK
    rf230_last_rssi = 3 * hal_subregister_read(SR_RSSI);
#endif

}

/* PLL has locked, either from a transition out of TRX_OFF or a channel change while on */
ISR(TRX24_PLL_LOCK_vect)
{
//	DEBUGFLOW('4');
}

/* PLL has unexpectedly unlocked */
ISR(TRX24_PLL_UNLOCK_vect)
{
	DEBUGFLOW('5');
}
/* Flag is set by the following interrupts */
extern volatile uint8_t rf230_wakewait, rf230_txendwait,rf230_ccawait;

/* Wake has finished */
ISR(TRX24_AWAKE_vect)
{
//	DEBUGFLOW('6');
  rf230_wakewait=0;
}

/* Transmission has ended */
ISR(TRX24_TX_END_vect)
{
//	DEBUGFLOW('7');
  rf230_txendwait=0;
}

/* Frame address has matched ours */
ISR(TRX24_XAH_AMI_vect)
{
//	DEBUGFLOW('8');
}

/* CCAED measurement has completed */
ISR(TRX24_CCA_ED_DONE_vect)
{
	DEBUGFLOW('4');
	rf230_ccawait=0;
}

#   endif /* defined(DOXYGEN) */

/** @} */
/** @} */

/*EOF*/
