#include "sfr_r827.h"

servo_init()
{
	//count source f8
	trccr1 = 0xba;
	//period
	trcgra_addr = 49999;
	trcgrb_addr = 3624;			// 0 degrees
	trcgrc_addr = 0;
	//start
	pinsr3 = 0x27;
	trcmr = 0x8f;
	eb_trcoer = 0;				//enable output
}

servo_direction(int dir)
{
	//27.928 per degree, 11.11us per degree
	// -60 = 783us = 1963, dir = 00
	// +60 = 2117us = 5314, dir = 78 = 120
  
  //limits
  if(dir > 110)
    dir = 110;
  if(dir < 10)
    dir = 10;
  
	tstart_trcmr = 0; 			//stop timer
	trcgrb_addr = 1963 + dir*27.928;
	tstart_trcmr = 1; 			//start timer
}