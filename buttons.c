#include "sfr_r827.h"
void buttons_init(){
  //p1_0 -> KI0 -> top button
  //p1_1 -> KI1 -> bottom button
  
  pd1 = pd1 & 0xFC;   //set to input
  pu02 = 1;           //use internal pullup
  ki0en = 1;          //enable key interrupt for ki0
  ki1en = 1;          //enable key interrupt for ki2
  kupic = 0x07;       //set interrupt level
}