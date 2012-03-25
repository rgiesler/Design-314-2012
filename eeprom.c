#include "sfr_r827.h"
 
char eeprom_skryf (char eeprom_adres, unsigned int data_adres, char data);
char eeprom_lees (char eeprom_adres, unsigned int data_adres, char *data);
void end_write (void);
void i2c_init(void);

/*  This function writes the charactor data to the eeprom at eeprom_adres
    at the address data_adres */
char eeprom_skryf(char eeprom_adres, unsigned int data_adres, char data)
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
	//SEND FIRST ADDRESS BYTE
	//(7)set the transmit data after 2nd byte (except the last byte)
	icdrt = data_adres>>8;
	//(8)wait until the ICRDT register is empty
	while(0 == tdre_icsr);
	//SEND SECOND ADDRESS BYTE
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

/*  This function reads a charactor from the eeprom at eeprom_adres from
    the address data_adres into the charactor data */
char eeprom_lees (char eeprom_adres, unsigned int data_adres, char *data)
{
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
	//SEND FIRST ADDRESS BYTE
	//(7)set the transmit data after 2nd byte (except the last byte)
	icdrt = data_adres>>8;
	//(8)wait until the ICRDT register is empty
	while(tdre_icsr==0){}
	//SEND SECOND ADDRESS BYTE
	//(9)set the transmit data after 2nd byte (except the last byte)
	icdrt = data_adres;
	//(10)wait until the ICRDT register is empty
	while(tend_icsr==0){}
    //(11)set the TEND bit to 0
	tend_icsr=0;
	//SEND DEV SELECT AND READ MODE
	//(2)set to master transmit mode
	iccr1 = iccr1|0x30;
	//(3)generate start condition
	iccr2 = (iccr2|0x80)&0xBF;
	//(4)set the transmit data of the first byte (slave address + rnw)
	icdrt = (eeprom_adres<<1) + 0x1;
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
	
	//(1)set the TEND bit to 0 and set to master recieve mode, set the TDRE bit to 0
	tend_icsr = 0;
	trs_iccr1 = 0;
	tdre_icsr = 0;
	//(7)set the ackbt bit of the last byte and set disable continuous recieve operation (RCVD = 1)
	ackbt_icier = 1;
	rcvd_iccr1 = 1;
	//(8)read the recieve data of (last byte -1)
	*data = icdrr;
	//(9)wait until last byte is recieved
	while(rdrf_icsr==0){}
	//(10)set the stop bit to 0
	stop_icsr = 0;
	//(11)generate stop condition
	iccr2 = iccr2&(0x3F);
	//(12)wait until stop condition is generated
	while(stop_icsr==0){}
	//(13)read the recieve data of last byte
	*data = icdrr;
	//(14)set the RCVD bit to 0
	rcvd_iccr1 = 0;
	//(15)set to slave recieve mode
	mst_iccr1 = 0;
	
	return 1;
}


void i2c_init()
{
    sar = 0x10;                         // set processor to slave address 6, and i2c
    iicsel = 1;                         // select i2c
    ice_iccr1 = 1;                       // enable transfer
    iccr1 = ((iccr1 & 0xF0) | (0x0D));  // f1/200
}