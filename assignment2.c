;/***********************************************************************/
/*                                                                     */
/*  FILE        :assignment2.c                                         */
/*  DATE        :Tue, Feb 07, 2012                                     */
/*  DESCRIPTION :main program file.                                    */
/*  CPU GROUP   :27                                                    */
/*                                                                     */
/*  This file is generated by Renesas Project Generator (Ver.4.18).    */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/***********************************************************************/
#include "timers.h"
#include "sfr_r827.h"
#include "lcdlib.h"
#include "serial.h"
#include "lcd.h"
#include "eeprom.h"
#include "magnet.h"

void clock_init(void);
extern volatile unsigned char sec;
extern volatile unsigned char halfsec;
extern volatile unsigned char serial1_message;
extern volatile unsigned char magnet_ready;
volatile signed int mag_x[8] = {0};
volatile signed int mag_y[8] = {0};
volatile signed int mag_x_temp = 0;
volatile signed int mag_y_temp = 0;
volatile signed int mag_x_tot = 0;
volatile signed int mag_y_tot = 0;
volatile signed int mag_z_temp = 0;
volatile signed int mag_count = 0;
volatile unsigned char storage_position = 0;
unsigned int eeprom_addres_count = 0;
int i;
char c;
char temp[9];
volatile unsigned char storage [100] = {0};

/*  This is the main program loop  */
void main(void)
{ clock_init();          // initialise statements
  timer_a_init();
  lcd_init();
  serial_init();
  pd1_3=1;               // set led port to output
  lcd_clear();
  i2c_init();
  magnet_initialise();
    
  while(1)
  { // LED flasher
    if(0 == sec%2)
      p1_3=1;
    else
      p1_3 = 0;
      
    //magnet sensor display
    if(magnet_ready)
    {
        magnet_ready = 0;       //reset interrupt flag
        //read the magnet data
        while(0 == magnet_read (0x0E, &mag_x_temp, &mag_y_temp, &mag_z_temp));
        mag_x_tot -= mag_x[mag_count];      //subtract last value from average
        mag_x[mag_count] = mag_x_temp/8;    //set new value
        mag_x_tot += mag_x[mag_count];      //add new value to average
        mag_y_tot -= mag_y[mag_count];
        mag_y[mag_count] = mag_y_temp/8;
        mag_y_tot += mag_y[mag_count];
        mag_count = (mag_count+1)%8;        // ring buffer counter increment

        format_text(mag_x_tot, temp); //display x results to top line of LCD
        lcd_display_top(temp);
        format_text(mag_y_tot, temp); //display y results to bottom line of LCD
        lcd_display_bottom(temp);
    }
		
	// scroll LCD
	if(1 == halfsec)
	{ lcd_scroll();
	  halfsec=0;
	}
	 
	// receive serial message, process it using switch case
	if(0x00 != serial1_message)
	{ 
      switch (serial1_message) 
      {
      case 0x84:                        // eeprom message recieved
        for(i = 0; i< eeprom_addres_count; i++)
            while(0==eeprom_skryf(0x50, i, storage[i]));
        while(0==eeprom_skryf(0x50, eeprom_addres_count, 0));
	    eeprom_addres_count=0;
      break;
      
      case 0x85:                        // tramsit eeprom data through serial
        eeprom_addres_count=0;
        do {
            while(0==eeprom_lees(0x50, eeprom_addres_count, &c));
            eeprom_addres_count++;
            serial_byte_transmit(c);
        }
        while (0 != c);
        eeprom_addres_count = 0;
      break;
      
      case 0x86:                            //transmit x magnetic data through serial
      case 0x87:
        format_text(mag_x_tot, temp);   //format data
        serial_transmit(temp);              //transmit
      break;
      
      case 0x88:
      case 0x89:
        format_text(mag_y_tot, temp);   //format data
        serial_transmit(temp);              //transmit
      break;
	 
      default:	                        // add byte to eeprom
	    storage[eeprom_addres_count++] = serial1_message;
      break;
      }
	  serial1_message = 0;
    }
  }
}
     
	
void clock_init(void)
{ asm("FCLR I");			  // stop all interrupts
  prc0 = 1;             // enables write to certain addresses (See p126 (pdf data))
  fra00=1;              // high speed on chip oscillator  = on (p102) 
  fra01=1;              // select high speed oscillator as clock (p102)
  ocd2=1;    
  cm06=0;               // (p111) f1             
  prc0 = 0;					    // write disable (p126)
  asm("FSET I");			  // enable interrupts
}