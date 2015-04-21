/*
 * Copyright (c) 2010, Swedish Institute of Computer Science.
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
 */

/**
 * \file
 *         Sensor driver for reading the temperature sensor in the Atmel Xplained Pro Board
 * \author
 *         Dag Björklund <dag@iki.fi>
 */

#include <util/twi.h>
#include <twiDriver.h>



void TWIInit(void)
{

    // reset TWI Registers, this is  required for proper initialization after wakeup from Sleep 
    TWCR = TWSR = TWDR = TWAR = TWAMR = 0;

    //set SCL to 400kHz
    TWSR = 0x00;
    //TWBR = 0x06;  
    TWBR = (F_CPU / 100000UL -16) / 2;
    //  SCL frequncy = CPU Clock frequency / (16+2*TWBR*4^TWPS)
    //enable TWI
   // TWCR = (1<<TWEN); // Disbaled  TWI enable as it is enabled in TWI_Start function // KD210415
}

// Send start signal
void TWIStart(void)
{
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
}

//send stop signal
 void TWIStop(void)
{
    TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
    while (TWCR & (1<<TWINT));
}

 void TWIWrite(uint8_t u8data)
{
  TWDR = u8data;
    TWCR = (1<<TWINT)|(1<<TWEN);
    //Wait until transmisson is completed 
    while (!(TWCR & (1<<TWINT)));
}

 uint8_t TWIReadACK(void)
{
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
    while (!(TWCR & (1<<TWINT)));
    return TWDR;
}



/**
 * @brief Read the last byte of the slave device.
 *
 * @return  byte read from slave
 */
uint8_t TWIReadNACK(void)
{
   TWCR = (1 << TWINT) | (1 << TWEN);

   /* Wait until transmission completed */
   while(!(TWCR & (1 << TWINT)));

   return TWDR;
}

 uint8_t TWIGetStatus(void)
{
    uint8_t status;
    //mask status
    status = TWSR & 0xF8;
    return status;
}
