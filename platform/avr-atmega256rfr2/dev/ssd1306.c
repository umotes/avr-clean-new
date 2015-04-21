#include "dev/ssd1306.h"
#include "font.h"
#include <avr/io.h>

void ssd1306_reset_clear(void)
        {
        
   #ifdef EXT_3
        DDRD |= (1<<PORTD7);
        PORTD &= ~(1<<PORTD7);
   #endif

   #ifdef EXT_1
        SSD_RES_DIR |= (1<<SSD_RES_PIN); // Reset Pin is set as Output
        SSD_RES_PORT &=~(1<<SSD_RES_PIN); // Reset  Pin Pulled Low
   #endif
         }


 void ssd1306_reset_set(void)
        {
   
   #ifdef EXT_3
        DDRD |= (1<<PORTD7);
        PORTD |= (1<<PORTD7);

   #endif

   #ifdef EXT_1
        SSD_RES_DIR |= (1<<SSD_RES_PIN); // Reset Pin is set as Output
        SSD_RES_PORT |= (1<<SSD_RES_PIN); // Reset  Pin Pulled HigH

   #endif
        }


 void ssd1306_sel_cmd(void)
        {
//Set High  for Data

   #ifdef EXT_3
       DDRB |= (1<<PORTB7);
       PORTB &= ~(1<<PORTB7);
   #endif
   #ifdef EXT_1
       SSD_DC_DIR |= (1<<SSD_DC_PIN); // Reset Pin is set as Output
       SSD_DC_PORT &= ~(1<<SSD_DC_PIN); // Reset  Pin Pulled Low
   #endif
        }

 void ssd1306_sel_data(void)
        {
//Low for COmmand
      
   #ifdef EXT_3
       DDRB |= (1<<PB7);
       PORTB |= (1<<PB7);

   #endif
   
   #ifdef EXT_1
       SSD_DC_DIR |= (1<<SSD_DC_PIN); // Reset Pin is set as Output
       SSD_DC_PORT |= (1<<SSD_DC_PIN); // Reset        Pin Pulled Low

   #endif
        }

 void spi_deselect_device(void)
        {
   
   #ifdef EXT_3
DDRG |= (1<<PORTG4);
PORTG |= (1<<PORTG4);

   #endif


   #ifdef EXT_1
       SSD_CS_DIR |= (1<<SSD_CS_PIN);
       SSD_CS_PORT|= (1<<SSD_CS_PIN);

   #endif
        }

 void spi_select_device(void)
         {
   #ifdef EXT_3
   DDRG |= (1<<PORTG4);
   PORTG &= ~(1<<PORTG4);
   #endif

   #ifdef EXT_1
   SSD_CS_DIR |= (1<<SSD_CS_PIN);
   SSD_CS_PORT &= ~(1<<SSD_CS_PIN);
   #endif

        }

 void spi_write_single(unsigned char data )
        {
        SPDR=data;
        SPI_WAITFOREOTx();
        }

 void ssd1306_write_command(uint8_t command)
 {
     spi_select_device();
     ssd1306_sel_cmd();
     spi_write_single(command);
  //    delay_us(SSD1306_LATENCY); // At lest 3?s

     clock_delay_usec(SSD1306_LATENCY);
     spi_deselect_device();

}
 void ssd1306_write_data(uint8_t data)
 {

    spi_select_device();
    ssd1306_sel_data();
    spi_write_single(data);
    clock_delay_usec(SSD1306_LATENCY);
    spi_deselect_device();

 }

 uint8_t ssd1306_read_data(void)
 {
     return 0;
 }

 uint8_t ssd1306_get_status(void)
 {
     return 0;
 }



 void ssd1306_hard_reset(void)
 {
   ssd1306_reset_clear();
        //ioport_set_pin_low(SSD1306_RES_PIN);
   clock_delay_usec(SSD1306_LATENCY);
   ssd1306_reset_set();
   //  ioport_set_pin_high(SSD1306_RES_PIN);
   clock_delay_usec(SSD1306_LATENCY);
 }



 void ssd1306_sleep_enable(void)
 {
     ssd1306_write_command(SSD1306_CMD_SET_DISPLAY_OFF);
 }

 void ssd1306_sleep_disable(void)
 {
     ssd1306_write_command(SSD1306_CMD_SET_DISPLAY_ON);
 }




 void ssd1306_set_page_address(uint8_t address)
 {
     // Make sure that the address is 4 bits (only 8 pages)
     address &= 0x0F;
     ssd1306_write_command(SSD1306_CMD_SET_PAGE_START_ADDRESS(address));
 }

 void ssd1306_set_column_address(uint8_t address)
 {
     // Make sure the address is 7 bits
     address &= 0x7F;
     ssd1306_write_command(SSD1306_CMD_SET_HIGH_COL(address >> 4));
     ssd1306_write_command(SSD1306_CMD_SET_LOW_COL(address & 0x0F));
 }

 void ssd1306_set_display_start_line_address(uint8_t address)
 {
     // Make sure address is 6 bits
     address &= 0x3F;
     ssd1306_write_command(SSD1306_CMD_SET_START_LINE(address));
 }



 void ssd1306_display_on(void)
 {
     ssd1306_write_command(SSD1306_CMD_SET_DISPLAY_ON);
 }

 void ssd1306_display_off(void)
 {
     ssd1306_write_command(SSD1306_CMD_SET_DISPLAY_OFF);
 }

 uint8_t ssd1306_set_contrast(uint8_t contrast)
 {
     ssd1306_write_command(SSD1306_CMD_SET_CONTRAST_CONTROL_FOR_BANK0);
     ssd1306_write_command(contrast);
     return contrast;
 }

 void ssd1306_display_invert_enable(void)
 {
     ssd1306_write_command(SSD1306_CMD_SET_INVERSE_DISPLAY);
 }

 void ssd1306_display_invert_disable(void)
 {
     ssd1306_write_command(SSD1306_CMD_SET_NORMAL_DISPLAY);
 }


 void ssd1306_clear(void)
 {
     uint8_t page = 0;
     uint8_t col = 0;

     for (page = 0; page < 4; ++page)
     {
         ssd1306_set_page_address(page);
         ssd1306_set_column_address(0);
         for (col = 0; col < 128; ++col)
         {
             ssd1306_write_data(0x00);
         }
     }
 }

 
 void  ssd1306_spi_init(void)
 {
   /* Initalize ports for communication with SPI units. */
   /* CSN=SS and must be output when master! */
 //  DDRB  |= (1<<PORTB2) | (1<<PORTB1);
  SSD_MOSI_DIR |= (1<<SSD_MOSI_PIN) | (1<<SSD_CLK_PIN);
//   PORTB  |= (1<<PORTB2) | (1<<PORTB1);

//   DDRG  |= (1<<SSD_CS_PIN);
//   PORTG  &= ~ (1<<SSD_CS_PIN);

   SSD_CS_DIR  |= (1<<SSD_CS_PIN);
   SSD_CS_PORT  &= ~ (1<<SSD_CS_PIN);
   /* Enables SPI, selects "master", clock rate FCK / 2, and SPI mode 0 */
   SPCR = (1<<SPE) | (1<<MSTR)|(1<<SPR0);
   SPSR = (1<<SPI2X);
 }

 
  void ssd1306_interface_init(void)
 {
spi_select_device();
ssd1306_spi_init();

 }
 
 void ssd1306_init(void)
 {
     // Do a hard reset of the OLED display controller
     ssd1306_hard_reset();
 
     // Initialize the interface
     ssd1306_interface_init();
 
     // 1/32 Duty (0x0F~0x3F)
     ssd1306_write_command(SSD1306_CMD_SET_MULTIPLEX_RATIO);
     ssd1306_write_command(0x1F);
 
     // Shift Mapping RAM Counter (0x00~0x3F)
     ssd1306_write_command(SSD1306_CMD_SET_DISPLAY_OFFSET);
     ssd1306_write_command(0x00);
 
     // Set Mapping RAM Display Start Line (0x00~0x3F)
     ssd1306_write_command(SSD1306_CMD_SET_START_LINE(0x00));
 
     // Set Column Address 0 Mapped to SEG0
     ssd1306_write_command(SSD1306_CMD_SET_SEGMENT_RE_MAP_COL127_SEG0);
 
     // Set COM/Row Scan Scan from COM63 to 0
     ssd1306_write_command(SSD1306_CMD_SET_COM_OUTPUT_SCAN_DOWN);
 
     // Set COM Pins hardware configuration
     ssd1306_write_command(SSD1306_CMD_SET_COM_PINS);
     ssd1306_write_command(0x02);
 
     ssd1306_set_contrast(0x8F);
 
     // Disable Entire display On
     ssd1306_write_command(SSD1306_CMD_ENTIRE_DISPLAY_AND_GDDRAM_ON);
 
     ssd1306_display_invert_disable();
 
     // Set Display Clock Divide Ratio / Oscillator Frequency (Default => 0x80)
     ssd1306_write_command(SSD1306_CMD_SET_DISPLAY_CLOCK_DIVIDE_RATIO);
     ssd1306_write_command(0x80);
 
     // Enable charge pump regulator
     ssd1306_write_command(SSD1306_CMD_SET_CHARGE_PUMP_SETTING);
     ssd1306_write_command(0x14);
 
     // Set VCOMH Deselect Level
     ssd1306_write_command(SSD1306_CMD_SET_VCOMH_DESELECT_LEVEL);
     ssd1306_write_command(0x40); // Default => 0x20 (0.77*VCC)
 
     // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
     ssd1306_write_command(SSD1306_CMD_SET_PRE_CHARGE_PERIOD);
     ssd1306_write_command(0xF1);
 
     ssd1306_display_on();
 }
 
 void ssd1306_write_text(const char *string)
 {
     uint8_t *char_ptr;
     uint8_t i;
 
     while (*string != 0) {
         char_ptr = font_table[(*string - 32) & 0x7F];
         for (i = 1; i <= char_ptr[0]; i++) {
             ssd1306_write_data(char_ptr[i]);
         }
         ssd1306_write_data(0x00);
         string++;
     }
 }
