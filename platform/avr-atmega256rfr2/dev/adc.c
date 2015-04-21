/*
 * Copyright (c) 2010, University of Colombo School of Computing
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

#include "dev/adc.h"

// ADC conversion at Lower frequncy leads to better and precise result, so set prescaler to higher value 
/*---------------------------------------------------------------------------*/
void
adc_init()
{
  ADMUX = 0;
  /* AVCC with external capacitor at AREF pin. */
  ADMUX |= _BV(REFS0);
  /* Disable ADC interrupts. */
  ADCSRA &= ~( _BV(ADIE) | _BV(ADIF) );
  /* Set ADC prescaler to 64 and clear interrupt flag. */
  ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADIE);

}
/*---------------------------------------------------------------------------*/
/* Poll based approach. The interrupt based adc is currently not used.
   The ADC result is right adjusted. First 8 bits(from left) are in ADCL and
   other two bits are in ADCH. See Atmega128 datasheet page 228. */
uint16_t
get_adc(int channel)
{
  uint16_t reading;

  ADMUX |= (channel & 0x1F);

  /* Disable ADC interrupts. */
  ADCSRA &= ~_BV(ADIE);
  /* Clear previous interrupts. */
  ADCSRA |= _BV(ADIF);
  /* Enable ADC and start conversion. */
  ADCSRA |= _BV(ADEN) | _BV(ADSC);
  /* Wait until conversion is completed. */
  while ( ADCSRA & _BV(ADSC) );
  /* Get first 8 bits.  */
  reading = ADCL;
  /* Get last two bits. */
  reading |= (ADCH & 3) << 8;
  /* Disable ADC. */
  ADCSRA &= ~_BV(ADEN);
  return reading;
}



static bool adc_initialized;
static bool adc_conversion_started;

/*---------------------------------------------------------------------------*/

/**
 *   \brief This function will init the ADC with the following parameters.
 *
 *   \param chan Determines the ADC channel to open.
 *   \param trig Sets what type of trigger is needed.
 *   \param ref Sets the proper reference voltage.
 *   \param prescale Sets the prescale to be used against the XTAL choice.
 *
 *   \return 0
*/
int
adc_init_full(adc_chan_t chan, adc_trig_t trig, adc_ref_t ref, adc_ps_t prescale)
{
    /* Enable ADC module */
    PRR0 &= ~(1 << PRADC);

    /* Configure */
    ADCSRA = (1<<ADEN)|prescale;
    ADCSRB &= ~(1<<MUX5);
    ADMUX = (uint8_t)ref|(uint8_t)chan;
    ADCSRB = trig;

    adc_initialized = true;
    adc_conversion_started = false;

    return 0;
}

/*---------------------------------------------------------------------------*/

/**
 *   \brief This will disable the adc.
*/
void
adc_deinit(void)
{
    /* Disable ADC */
    ADCSRA &= ~(1<<ADEN);
    PRR0 |= (1 << PRADC);

    adc_initialized = false;
    adc_conversion_started = false;
}

/*---------------------------------------------------------------------------*/

/**
 *   \brief This will start an ADC conversion
 *
 *   \return 0
*/
int
adc_conversion_start(void)
{
    if (adc_initialized == false){
        return EOF;
    }
    adc_conversion_started = true;
    ADCSRA |= (1<<ADSC);
    return 0;
}

/*---------------------------------------------------------------------------*/

/**
 *   \brief This will read the ADC result during the ADC conversion and return
 *   the raw ADC conversion result.
 *
 *   \param adjust This will Left or Right Adjust the ADC conversion result.
 *
 *   \return ADC raw 16-byte ADC conversion result.
*/
int16_t
adc_result_get(adc_adj_t adjust)
{
  uint16_t reading;
    if (adc_conversion_started == false){
        return EOF;
    }
    if (ADCSRA & (1<<ADSC)){
        return EOF;
    }
    adc_conversion_started = false;
    ADMUX |= (adjust<<ADLAR);
    return (int16_t)ADC;
//reading = ADCL;
  /* Get last two bits. */
  //reading |= (ADCH & 3) << 8;
//return reading;
}

/** \}   */










































/*---------------------------------------------------------------------------*/
