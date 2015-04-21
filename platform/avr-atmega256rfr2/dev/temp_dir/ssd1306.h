#ifndef SSD1306_H_INCLUDED
 #define SSD1306_H_INCLUDED
 
 #include "sys/clock.h"
// #include "contiki-conf.h"
 #include "conf_ssd1306.h"
 #include "avr/io.h"
// #include "dev/clock-avr.h"

 // controller and OLED configuration file

  #define SSD1306_LATENCY 10
  #define SPI_TXBUF SPDR
  #define SPI_RXBUF SPDR
  #define BV(bitno) (1<<bitno)

  #define SPI_WAITFOREOTx() do { while (!(SPSR & BV(SPIF))); } while (0)
  #define SPI_WAITFOREORx() do { while (!(SPSR & BV(SPIF))); } while (0)

  //OLED  Display Properties
  #define UG_2832HSWEG04_BAUDRATE  12000000
  #define SSD1306_CLOCK_SPEED      UG_2832HSWEG04_BAUDRATE
  #define SSD1306_DISPLAY_CONTRAST_MAX 40
  #define SSD1306_DISPLAY_CONTRAST_MIN 30
  #define LCD_WIDTH_PIXELS      128
  #define LCD_HEIGHT_PIXEl              32
 

 
 #define SSD1306_CMD_SET_LOW_COL(column)             (0x00 | (column))
 #define SSD1306_CMD_SET_HIGH_COL(column)            (0x10 | (column))
 #define SSD1306_CMD_SET_MEMORY_ADDRESSING_MODE      0x20
 #define SSD1306_CMD_SET_COLUMN_ADDRESS              0x21
 #define SSD1306_CMD_SET_PAGE_ADDRESS                0x22
 #define SSD1306_CMD_SET_START_LINE(line)            (0x40 | (line))
 #define SSD1306_CMD_SET_CONTRAST_CONTROL_FOR_BANK0  0x81
 #define SSD1306_CMD_SET_CHARGE_PUMP_SETTING         0x8D
 #define SSD1306_CMD_SET_SEGMENT_RE_MAP_COL0_SEG0    0xA0
 #define SSD1306_CMD_SET_SEGMENT_RE_MAP_COL127_SEG0  0xA1
 #define SSD1306_CMD_ENTIRE_DISPLAY_AND_GDDRAM_ON    0xA4
 #define SSD1306_CMD_ENTIRE_DISPLAY_ON               0xA5
 #define SSD1306_CMD_SET_NORMAL_DISPLAY              0xA6
 #define SSD1306_CMD_SET_INVERSE_DISPLAY             0xA7
 #define SSD1306_CMD_SET_MULTIPLEX_RATIO             0xA8
 #define SSD1306_CMD_SET_DISPLAY_ON                  0xAF
 #define SSD1306_CMD_SET_DISPLAY_OFF                 0xAE
 #define SSD1306_CMD_SET_PAGE_START_ADDRESS(page)    (0xB0 | (page & 0x07))
 #define SSD1306_CMD_SET_COM_OUTPUT_SCAN_UP          0xC0
 #define SSD1306_CMD_SET_COM_OUTPUT_SCAN_DOWN        0xC8
 #define SSD1306_CMD_SET_DISPLAY_OFFSET              0xD3
 #define SSD1306_CMD_SET_DISPLAY_CLOCK_DIVIDE_RATIO  0xD5
 #define SSD1306_CMD_SET_PRE_CHARGE_PERIOD           0xD9
 #define SSD1306_CMD_SET_COM_PINS                    0xDA
 #define SSD1306_CMD_SET_VCOMH_DESELECT_LEVEL        0xDB
 #define SSD1306_CMD_NOP                             0xE3
 
 
 #define SSD1306_CMD_SCROLL_H_RIGHT                  0x26
 #define SSD1306_CMD_SCROLL_H_LEFT                   0x27
 #define SSD1306_CMD_CONTINUOUS_SCROLL_V_AND_H_RIGHT 0x29
 #define SSD1306_CMD_CONTINUOUS_SCROLL_V_AND_H_LEFT  0x2A
 #define SSD1306_CMD_DEACTIVATE_SCROLL               0x2E
 #define SSD1306_CMD_ACTIVATE_SCROLL                 0x2F
 #define SSD1306_CMD_SET_VERTICAL_SCROLL_AREA        0xA3
 
 
 
 void ssd1306_spi_init(void);
 void ssd1306_interface_init(void);
 void ssd1306_init(void);
 void ssd1306_reset_clear(void);
 void ssd1306_reset_set(void);
 void ssd1306_sel_data(void);
 void ssd1306_sel_cmd(void);
 void spi_deselect_device(void);
 void spi_select_device(void);
 void spi_write_single(unsigned char data );
 void ssd1306_write_command(uint8_t command);
 void ssd1306_write_data(uint8_t data);
 uint8_t ssd1306_read_data(void);
 uint8_t ssd1306_get_status(void);
 void ssd1306_hard_reset(void);
 void ssd1306_sleep_enable(void);
 void ssd1306_sleep_disable(void);
 void ssd1306_set_page_address(uint8_t address);
 void ssd1306_set_column_address(uint8_t address);
 void ssd1306_set_display_start_line_address(uint8_t address);
 void ssd1306_display_on(void);
 void ssd1306_display_off(void);
 uint8_t ssd1306_set_contrast(uint8_t contrast);
 void ssd1306_display_invert_enable(void);
 void ssd1306_display_invert_disable(void);
 void ssd1306_clear(void);
 void ssd1306_write_text(const char *string);
 
 #endif /* SSD1306_H_INCLUDED */
