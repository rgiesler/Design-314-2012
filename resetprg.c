/***********************************************************************/
/*                                                                     */
/*  FILE        :resetprg.c                                            */
/*  DATE        :Tue, Feb 21, 2012                                     */
/*  DESCRIPTION :initialize for C language.                            */
/*  CPU GROUP   :27                                                    */
/*                                                                     */
/*  This file is generated by Renesas Project Generator (Ver.4.18).    */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/***********************************************************************/

/*******************************************************************
 *  COMPILER for R8C/Tiny
 *  Copyright(c) 2004 Renesas Technology Corp.
 *  And Renesas Solutions Corp.,All Rights Reserved. 
 *
 *  resetprg.c : startup file 
 *
 *  Function:initialize each function
 *
 * $Date: 2006/09/29 05:58:34 $
 * $Revision: 1.8 $
 *******************************************************************/
#include "sfr_r827.h"
#include "typedefine.h"
#include "resetprg.h"

DEF_SBREGISTER;
extern _UINT	_stack_top,_istack_top;
#pragma entry start
void start(void);
extern void initsct(void);
extern void _init(void);
void exit(void);
void main(void);

#pragma section program interrupt 

void start(void)
{
	_isp_	= &_istack_top; 	// set interrupt stack pointer
	prcr	= 0x02;		// change protect mode register
	pm0	= 0x00;		// set processor mode register
	prcr	= 0x00;		// change protect mode register
	_flg_	= __F_value__;	// set flag register
#if __STACKSIZE__!=0
	_sp_	= &_stack_top; 	// set user stack pointer
#endif
	_sb_	= 0x400;	// 400H fixation (Do not change)
	_intbh_ = 0x00; 	// set variable vector's address
	_asm("	ldc	#(topof vector)&0FFFFh,INTBL");

	initsct();		// initlalize each sections

#if __HEAPSIZE__ != 0
	heap_init();		// initialize heap 
#endif
#if __STANDARD_IO__ != 0
	_init();			// initialize standard I/O
#endif
	_fb_ = 0;		// initialize FB registe for debugger
	main();			// call main routine

	exit();		// call exit
}

void exit(void)
{
	while(1);		//infinite loop
}
