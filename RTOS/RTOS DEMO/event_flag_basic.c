/*
	basic program:event flag
*/
#include <rtl.h>
#include <LPC214X.H>

OS_TID tid1,tid2;
unsigned int count1,count2;
__task void task1 (void);
__task void task2 (void);

__task void task1 (void)
{
	tid1=os_tsk_self ();
	tid2=os_tsk_create (task2,1);
	while (1)
	{
		count1++;
	}	
}

__task void task2 (void)
{
  OS_RESULT result;
	while (1)
	{
		result=os_evt_wait_or (0x1,0xffff);	
		if (result == OS_R_EVT)
			count2++;
	}	
}

void EINT0_Handler (void) __irq
{
	isr_evt_set (0x01, tid2); // Signal Task 2 with an event
	EXTINT = 0x00000001; // Clear the peripheral interrupt flag
	VICVectAddr = 0x00000000; // Signal end of interrupt to the VIC
}

void config_EINT ()
{
	PINSEL1 = 0x1;//P0.16 as EINT0 pin
	EXTMODE = 0x1;//EINT0 is Edge Sensitive
	EXTPOLAR= 0x1;//EINT0 is Rising edge sensitives
}

void config_vic ()
{
	VICIntSelect = 0x0;//All are IRQs
	VICVectCntl0 = 14 | (1<<5);
	VICVectAddr0 = (unsigned int) EINT0_Handler;
	VICIntEnable = 1<<14;//Enable EINT0 service
}

main ()
{
	config_EINT ();
	config_vic ();
	os_sys_init (task1);
}
