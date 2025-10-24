/*
*****High priority task demonstration*****
Here Task2 is highest priority task and
starts execution at the moment task2 is 
created.
When the SW is pressed, task2 goes to WAIT_STATE
and task1(lowest pri) will get chance to
execute for 5secs (blinks led 10times).
*/
#include <rtl.h>
#include <LPC214X.H>
#define LED1 (1<<0)  //P0.0
#define SW ((IOPIN0&(1<<16))) //P0.16



OS_TID tid1,tid2;

__task void task1 (void);
__task void task2 (void);

unsigned int count1,count2,count3;

__task void task1 (void)
{
	tid1 = os_tsk_self();
	tid2 = os_tsk_create (task2,2);
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
		if (SW == 0)
			os_dly_wait (500);//5sec
	}
}



main ()
{
	IODIR0 |= LED1;//P0.0 is OUTPUT & P0.16 is INPUT
	os_sys_init(task1);
}
