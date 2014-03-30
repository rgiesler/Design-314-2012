#include "sfr_r827.h"
#include "math.h"
#define PI 3.1415926536

char magnet_read (char eeprom_adres, int *x, int *y, int *z)
{
    int k;
    unsigned int data_adres = 0x01;
	stop_icsr = 0;
	//(1)judge the state of the SCL and SDA line
	while(bbsy_iccr2==1){}
	//(2)set to master transmit mode
	iccr1 = iccr1|0x30;
	//(3)generate start condition
	iccr2 = (iccr2|0x80)&0xBF;
	//(4)set the transmit data of the first byte (slave address + rnw)
	icdrt = eeprom_adres<<1;
	//(5)wait for 1 byte to be transmitted
	while(tend_icsr==0){}
	//(6)Judge the ACKBR bit from specified slave device
	if(ackbr_icier==1)
	{
		//(11)set the TEND bit to 0
		tend_icsr = 0;
		//(12)set the STOP bit to 0
		stop_icsr = 0;
		//(13)generate stop condition
		iccr2 = iccr2&(0x3F);
		//(14)wait until the stop condition is generated
		while(stop_icsr==0){}
		//(15)Set to slave recieve mode, set the TDRE bit to 0
		iccr1 = iccr1&(0xCF);
		tdre_icsr=0;
		return 0;//return failure
	}
	//SEND ADDRESS BYTE
	//(7)set the transmit data after 2nd byte (except the last byte)
	icdrt = data_adres;
	//(8)wait until the ICRDT register is empty
	while(tdre_icsr==0){}
    while(tend_icsr==0);
    //(11)set the TEND bit to 0
	tend_icsr=0;
	//SEND DEV SELECT AND READ MODE
	//(2)set to master transmit mode
    for(k = 0; k<100; k++);
	iccr1 = iccr1|0x30;
	//(3)generate start condition
	iccr2 = (iccr2|0x80)&0xBF;
	//(4)set the transmit data of the first byte (slave address + rnw)
	icdrt = (eeprom_adres<<1) + 0x1;
	//(5)wait for 1 byte to be transmitted
    while(0==tdre_icsr);
	while(tend_icsr==0){}
	//(6)Judge the ACKBR bit from specified slave device
	if(ackbr_icier==1)
	{
		//(11)set the TEND bit to 0
		tend_icsr = 0;
		//(12)set the STOP bit to 0
		stop_icsr = 0;
		//(13)generate stop condition
		iccr2 = iccr2&(0x3F);
		//(14)wait until the stop condition is generated
		while(stop_icsr==0){}
		//(15)Set to slave recieve mode, set the TDRE bit to 0
		iccr1 = iccr1&(0xCF);
		tdre_icsr=0;
		return 0;//return failure
	}
	
	//(1)set the TEND bit to 0 and set to master recieve mode, set the TDRE bit to 0
	tend_icsr = 0;
	trs_iccr1 = 0;
	tdre_icsr = 0;
    //(2) set the ACKBT bit to the transmit device
    ackbt_icier = 0;
    //(3) dummy read the ICDRR register
    *x = icdrr;
    //(4) wait for 1 byte to be received
    while(0==rdrf_icsr);
    //(5) judge last recieve - 1
    //(6) read the recieve data (msb of x)
    *x = icdrr;
    *x = *x<<8;
    //(4) wait for 1 byte to be received
    while(0==rdrf_icsr);
    //(5) judge last recieve - 1
    //(6) read the recieve data (lsb of x)
    *x += icdrr;
    //(4) wait for 1 byte to be received
    while(0==rdrf_icsr);
    //(5) judge last recieve - 1
    //(6) read the recieve data (msb of y)
    *y = icdrr;
    *y = *y<<8;
    //(4) wait for 1 byte to be received
    while(0==rdrf_icsr);
    //(5) judge last recieve - 1
    //(6) read the recieve data (lsb of y)
    *y += icdrr;
	//(7)set the ackbt bit of the last byte and set disable continuous recieve operation (RCVD = 1)
	ackbt_icier = 1;
	rcvd_iccr1 = 1;
	//(8)read the recieve data of (last byte -1) (msb of z)
	*z = icdrr;
    *z = *z<<8;
	//(9)wait until last byte is recieved
	while(rdrf_icsr==0){}
	//(10)set the stop bit to 0
	stop_icsr = 0;
	//(11)generate stop condition
	iccr2 = iccr2&(0x3F);
	//(12)wait until stop condition is generated
	while(stop_icsr==0){}
	//(13)read the recieve data of last byte (lsb of z)
	*z += icdrr;
	//(14)set the RCVD bit to 0
	rcvd_iccr1 = 0;
	//(15)set to slave recieve mode
	mst_iccr1 = 0;
	
	return 1;
}

char magnet_write (char eeprom_adres, char data_adres, char data)
{
    //(1)judge the state of the SCL and SDA line
	while(1 == bbsy_iccr2);
	//(2)set to master transmit mode
	iccr1 = iccr1|0x30;
	//(3)generate start condition
	iccr2 = (iccr2|0x80)&0xBF;
	//(4)set the transmit data of the first byte (slave address + rnw)
	icdrt = eeprom_adres<<1;
	//(5)wait for 1 byte to be transmitted
	while(0 == tend_icsr);
	//(6)Judge the ACKBR bit from specified slave device
	if(ackbr_icier==1)
	{
		//(11)set the TEND bit to 0
		tend_icsr = 0;
		//(12)set the STOP bit to 0
		stop_icsr = 0;
		//(13)generate stop condition
		iccr2 = iccr2&(0b00111111);
		//(14)wait until the stop condition is generated
		while(stop_icsr==0){}
		//(15)Set to slave recieve mode, set the TDRE bit to 0
		iccr1 = iccr1&(0b11001111);
		return 0;//return failure
	}
	//SEND ADDRESS BYTE
	//(7)set the transmit data after 2nd byte (except the last byte)
	icdrt = data_adres;
	//(8)wait until the ICRDT register is empty
	while(0 == tdre_icsr);
	//(9)set the transmit data of the last byte
	icdrt = data;
	//(10)wait for end of transmission of the last byte
	while(0 == tend_icsr){}
	//(11)set the TEND bit to 0
	tend_icsr = 0;
	//(12)set the STOP bit to 0
	stop_icsr = 0;
	//(13)generate stop condition
	iccr2 = iccr2&(0b00111111);
	//(14)wait until the stop condition is generated
	while(0 == stop_icsr);
	//(15)Set to slave recieve mode, set the TDRE bit to 0
	iccr1 = iccr1&(0b11001111);
	tdre_icsr = 0;
	return 1;
}

magnet_initialise()
{
    // Register 2
    // Automatic megnetic sensor reset, no user offset
    while(0 == magnet_write(0x0E, 0x11, 0xA0));
    // Register 1
    // 10Hz output rate, max ADC rate of 1280Hz, full 16-bit (fast read disabled)
    // Normal, active mode;
    while(0 == magnet_write(0x0E, 0x10, 0x19));
    
    int1sel = 0;
    int1ic = 0x07;
    int1en = 1;
    
}

void format_text (int mag, char *temp)   //Custom function because sprintf takes a lot of memory
{
    unsigned char i=5, j;          //Counter variable (assuming 16 bit signed int)
    char val;
    char temp2 [8] = {' '};
    
    if(mag < 0)                 //output sign
    {
        temp [0] = '-';
        mag = -mag;             //remove sign
    }
    else
        temp [0] = '+';
    
    while (mag > 0)             //add numbers to array
    {
        temp2 [i--] = (mag % 10) | '0';
        mag /= 10;
    }
    
    while (i > 0)               //fill blanks
        temp2[i--] = ' ';
    temp2[0] = ' ';
    
    i = 1;                      //copy over to remove blanks
    for ( j = 0; j<8; j++ )
        if (temp2[j] != ' ')
            temp[i++] = temp2[j];
        
    temp [i] = 0;            //add null terminator
}

double get_angle (double mag_x_def, double mag_y_def)
{
  double mag_angle;
  double tan_arg;
  
  if(mag_x_def < 0.1 && mag_x_def > -0.1)
    mag_x_def = 0.1;
  if(mag_y_def < 0.1 && mag_y_def > -0.1)
    mag_y_def = 0.1;
  tan_arg = mag_y_def/mag_x_def; 
  
        tan_arg = mag_y_def/mag_x_def; 
        if (tan_arg <= 1 && tan_arg >= -1) 
        { 
          mag_angle = atan(tan_arg)*180/PI;
        }
        else
        {
          tan_arg = mag_x_def/mag_y_def;
          if (tan_arg >= 0)
            mag_angle = 90 - atan(tan_arg)*180/PI;
          else mag_angle = -90 - atan(tan_arg)*180/PI; 
        }        
        if(mag_x_def > 0)
        {
          if(mag_y_def>0)
            mag_angle = mag_angle - 180;
          else
            mag_angle = mag_angle + 180;
        }
        
   return mag_angle;
}