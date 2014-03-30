#include "sfr_r827.h"
#include "lcdlib.h"
#include "serial.h"

volatile unsigned char lcd_pos=0;
volatile unsigned char top_length=0;
volatile unsigned char bottom_length=0;

/*  This function takes the char array 'message' and displays it on the 
    top line of the LCD  */
void lcd_display_top (char message[])
{ 
  int i = 0;
  p5_3 = 0;		//disable sensors
  lcd_command_write(0x80 + lcd_pos);   // set position to top line
  while(0 != message[i])               // send the charactors
    lcd_char_write(message[i++]);
  top_length = i + 1;                  // counter for the length of the message
  for (i = 0; 9 > top_length + i; i++)
    lcd_char_write(' ');               // make rest of line blank
  p5_3 = 1;		//enable sensors
  prc2 = 1;
  pd0_0 = 0;
  prc2 = 1;
  pd0_1 = 0;
  prc2 = 1;
  pd0_2 = 0;
  prc2 = 1;
  pd0_3 = 0;
}

/*  This function takes the char array 'message' and displays it on the 
    bottom line of the LCD  */
void lcd_display_bottom (char message[])
{ 
  int i = 0;
  char temp;
  p5_3 = 0;		//disable sensors
  lcd_command_write(0xA8);             // set position to bottom line
  while(0 != message[i])               // send the charactors
    lcd_char_write(message[i++]);
  bottom_length = i + 1;               // counter for the length of the bottom line
  for (i = 0; 9 > bottom_length + i; i++)
    lcd_char_write(' ');               // make rest of line blank;
  for(i = 0; 8 > i; i++)               // copy top line to beginning
  { temp = lcd_char_read(lcd_pos + i);
    lcd_command_write(0x80 + i);
    lcd_char_write(temp);
  }
  lcd_pos = 0;
  lcd_command_write(0x02);             // reset scrolling to home position
  p5_3 = 1;		//enable sensors
  prc2 = 1;
  pd0_0 = 0;
  prc2 = 1;
  pd0_1 = 0;
  prc2 = 1;
  pd0_2 = 0;
  prc2 = 1;
  pd0_3 = 0;
}

/*  This function scrolls only the bottom row of the LCD display once
    to the left. If it is already at the end of the message, it resets
	the display to the home position  */
void lcd_scroll (void)
{ int i=0;
  char temp; 
  p5_3 = 0;		//disable sensors   
  if(bottom_length - 9 == lcd_pos)    // if end of scrolling line
  { for(i = 0; 8 > i; i++)            // copy top line to beginning
      { temp = lcd_char_read(lcd_pos + i);
        lcd_command_write(0x80 + i);
        lcd_char_write(temp);
      }
      lcd_command_write(0x02);        // change scroll to beginning
      lcd_pos = 0;                    // reset position
  }
  else if (9 < bottom_length)         // if not the end, scroll once to the left
  { for(i = 8; 0 < i; i--)            // copy the top line one place ahead
    { temp = lcd_char_read(lcd_pos + i - 1);
      lcd_command_write(0x80 + lcd_pos + i);
      lcd_char_write(temp);
    }
    lcd_command_write (0x18);         // now scroll
    lcd_pos++;
  }
  p5_3 = 1;		//enable sensors
  prc2 = 1;
  pd0_0 = 0;
  prc2 = 1;
  pd0_1 = 0;
  prc2 = 1;
  pd0_2 = 0;
  prc2 = 1;
  pd0_3 = 0;
}

/*  This function reads whatever is currently displayed in the top row
    of the LCD display, and transfers it over serial port 1 */
void lcd_to_serial(void)
{
  int i;
   p5_3 = 0;		//disable sensors
  for(i = 0; 8 > i; i++)
    serial_byte_transmit(lcd_char_read (i + lcd_pos));
  p5_3 = 1;		//enable sensors
  prc2 = 1;
  pd0_0 = 0;
  prc2 = 1;
  pd0_1 = 0;
  prc2 = 1;
  pd0_2 = 0;
  prc2 = 1;
  pd0_3 = 0;
}

/*  This function clears the LCD display and resets the position to home */
void lcd_clear(void)
{ p5_3 = 0;		//disable sensors
  lcd_command_write(0x01);    // clear display and go to home position
  lcd_pos = 0;                // reset position
  p5_3 = 1;		//enable sensors
  prc2 = 1;
  pd0_0 = 0;
  prc2 = 1;
  pd0_1 = 0;
  prc2 = 1;
  pd0_2 = 0;
  prc2 = 1;
  pd0_3 = 0;
}