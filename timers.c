#include "sfr_r827.h"
void timer_a_init(void)
{ tracr=0x04;  // stop
  tramr=0x10;  // timer ,f8
  trapre=99;
  tra=249;     // 20000000/8/100/250=100 Hz
  traic=0x07;  // intr priority =7
  tracr=0x01;  // start
}