 
/* 
// Sample tesp Application for the  LCD Display Attached  to the ATMEL Development Board 
// This  will be further   changed  later  on the   process based concept  as in COntiki
//  Added  by Kanchan 

*/

 #include <ssd1306.h>
 #include "avr/io.h"
 #include "avr/interrupt.h"
 #include "sys/clock.h"
 #include "dev/clock-avr.h"
 #include "sys/etimer.h"



 
 int main(void)
 {
     uint8_t page_address;
     uint8_t column_address;
     uint8_t start_line_address = 0;
     volatile uint16_t delay = 10000;
 
     board_init();
     sysclk_init();
 
     // Initialize SPI and SSD1306 controller
     ssd1306_init();
 
     // set addresses at beginning of display
     ssd1306_set_page_address(0);
     ssd1306_set_column_address(0);
 
     // fill display with lines
     for (page_address = 0; page_address <= 4; page_address++) {
         ssd1306_set_page_address(page_address);
         for (column_address = 0; column_address < 128; column_address++) {
             ssd1306_set_column_address(column_address);
             /* fill every other pixel in the display. This will produce
             horizontal lines on the display. */
             ssd1306_write_data(0x6F);
         }
     }
 
     // scroll the display using hardware support in the LCD controller
     while (true) {
         ssd1306_set_display_start_line_address(start_line_address++);
        clock_delay_msec(250);
     }
 }

