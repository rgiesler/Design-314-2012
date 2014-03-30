#include "sfr_r827.h"

motor_init(int speed)
{ 
  // 0 < speed < 99, where speed is the width of the PWM
  tstart_trbcr = 0;     //stop timer
  if(speed!=0)          //only if %PWM is >0, stop timer if it is 0
  {                     //(waveform cannot have 0%PWM for this timer)
    trbmr = 0x11;     //f8 source
    topl_trbioc = 1;  //switch polarity
	  trbpre = 255;       //set correct pwm
    trbsc = 100-speed;
	  trbpr = speed-1;
	  trbic = 0x07;			//enable timerb interrupt
	  trbcr = 0x01;   //enable timer b
  }
}
