#include "sfr_r827.h"

/*  This function initiates the serial port by setting all the necessary
    registers */
void serial_init(void)
{ pinsr1 = 0x05; // use p3_7 and p4_5
  u1pinsel = 1;
  txd1sel = 1;
  u1brg = 0x20;  // 38400 baud
  u1mr = 0x05;   // 8 bit, 1 stop bit, no parity
                 // u1c0 default is f1, cmos output, falling edge, lsb first
  u1c1 = 0x05;   // recieve enable, transmit enable, non-continuous
  s1ric = 0x07;  // interrupt level 7
}

/*  This function transmits a single character over Serial Port 1 */
void serial_byte_transmit(char message)
{ u1tb = message;       // send message
  while(0 == ti_u1c1);  // wait for message to be sent
}

/*  This function transmits a string of characters, 'message'
    over the serial port using the 'serial_byte_transmit' function */
void serial_transmit(char message [])
{ int i = 0;
  while(0 != message[i])
    serial_byte_transmit(message[i++]);  // send message  
}