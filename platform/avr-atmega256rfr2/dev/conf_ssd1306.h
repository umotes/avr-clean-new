  #ifndef CONF_SSD1306_H_INCLUDED
  #define CONF_SSD1306_H_INCLUDED


  #define SSD_DC_PORT           PORTE
  #define SSD_DC_DIR            DDRE
  #define SSD_DC_PIN            PORTE2

  #define SSD_RES_PORT          PORTD
  #define SSD_RES_DIR           DDRD
  #define SSD_RES_PIN       PORTD5

  #define SSD_CS_PORT       PORTG
  #define SSD_CS_DDR            DDRG
  #define SSD_CS_PIN        PORTG0

  #define SSD_MOSI_PORT         PORTB
  #define SSD_MOSI_DIR          DDRB
  #define SSD_MOSI_PIN          PORTB2

  #define SSD_MISO_PORT         PORTB
  #define SSD_MISO_DIR          DDRB
  #define SSD_MISO_PIN          PORTB3

  #define SSD_CLK_PORT          PORTB
  #define SSD_CLK_DIR           DDRB
  #define SSD_CLK_PIN           PORTB1
  
  #define UG_2832HSWEG04_DATA_CMD   SSD_DC_PIN
  #define UG_2832HSWEG04_RESET          SSD_RES_PIN
  #define UG_2832HSWEG04_SS         SSD_CS_PIN
  #define UG_2832HSWEG04_SPI_SCK    SSD_CLK_PIN
  #define UG_2832HSWEG04_SPI_MOSI   SSD_MOSI_PIN
  #define UG_2832HSWEG04_SPI_MISO   SSD_MISO_PIN

  #define SSD1306_USART_SPI       UG_2832HSWEG04_SPI
  #define SSD1306_DC_PIN          UG_2832HSWEG04_DATA_CMD
  #define SSD1306_RES_PIN         UG_2832HSWEG04_RESET
  #define SSD1306_CS_PIN          UG_2832HSWEG04_SS

  // Minimum clock period is 50ns@3.3V -> max frequency is 20MHz
  //OLED  Display Properties
  #define UG_2832HSWEG04_BAUDRATE  12000000
  #define SSD1306_CLOCK_SPEED      UG_2832HSWEG04_BAUDRATE
  #define SSD1306_DISPLAY_CONTRAST_MAX 40
  #define SSD1306_DISPLAY_CONTRAST_MIN 30
  #define LCD_WIDTH_PIXELS      128
  #define LCD_HEIGHT_PIXEl              32

  #endif /* CONF_SSD1306_H_INCLUDED */


  
