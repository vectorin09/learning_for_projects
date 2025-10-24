/*
RTX executes task1,task2 in the
round robin fashion.
LED1 will blink 4 times in 2 sec.
LED2 will blink 1 times in 2 sec.
*/

#include <rtl.h>
#include <LPC214X.H>
#define LED1 (1<<0)  //P0.0
#define LED2 (1<<1)  //P0.1



OS_TID tid1,tid2;

__task void task1 (void);
__task void task2 (void);

unsigned int count1,count2,count3;

__task void task1 (void)
{
	tid1 = os_tsk_self();
	tid2 = os_tsk_create (task2,1);
	for (;;)
	{
		IOSET0 = LED1;
		os_dly_wait (25);//250ms
		IOCLR0 = LED1;
		os_dly_wait (25);
	}	
}	
	
__task void task2 (void)
{
	for (;;)
	{
		IOSET0 = LED2;
		os_dly_wait (200);//2sec
		IOCLR0 = LED2;
		os_dly_wait (200);//2sec
	}
}



main ()
{
	IODIR0 |= LED1|LED2;//P0.0 & P0.1 are OUTPUT pins 
	os_sys_init(task1);
}
