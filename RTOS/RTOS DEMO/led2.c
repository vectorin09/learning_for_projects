/*
RTX executes task1,task2,task3 in the
round robin fashion.
LED1 will blink 5 times in 1 sec.
LED2 will blink 2 times in 1 sec.
LED3 will blink 1 times in 2 sec.
*/
#include <rtl.h>
#include <LPC214X.H>
#define LED1 (1<<0)  //P0.0
#define LED2 (1<<1)  //P0.1
#define LED3 (1<<2)  //P0.2


OS_TID tid1,tid2,tid3;

__task void task1 (void);
__task void task2 (void);
__task void task3 (void);

unsigned int count1,count2,count3;

__task void task1 (void)
{
	tid1 = os_tsk_self();
	tid2 = os_tsk_create (task2,1);
	tid3 = os_tsk_create (task3,1);
	for (;;)
	{
		IOSET0 = LED1;
		os_dly_wait (10);//100ms
		IOCLR0 = LED1;
		os_dly_wait (10);
	}	
}	
	

__task void task2 (void)
{
	for (;;)
	{
		IOSET0 = LED2;
		os_dly_wait (25);//250ms
		IOCLR0 = LED2;
		os_dly_wait (25);
	}
}

__task void task3 (void)
{
	for (;;)
	{
		IOSET0 = LED3;
		os_dly_wait (100);//1sec
		IOCLR0 = LED3;
		os_dly_wait (100);
	}
}

main ()
{
	IODIR0 |= LED1|LED2|LED3; 
	os_sys_init(task1);
}
