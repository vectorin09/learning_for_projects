/*
event flags with IR rx,buzzer & leds
Connections:
Task1 LED1 = P0.0
Task2 LED2 = P0.1
Task2 Buzzer = P0.7
IR Rx D0 = P0.16 (which is EINT0)

*/
#include <rtl.h>
#include <LPC214X.H>
#define LED1 (1<<0) //P0.0
#define LED2 (1<<1) //P0.1
#define BUZZER (1<<7) //P0.7
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
			IOSET0 = LED1;
			os_dly_wait (100);//1sec	
			IOCLR0 = LED1;
			os_dly_wait (100);
	}	
}

__task void task2 (void)
{
	char i;	
  OS_RESULT result;
	while (1)
	{
		result=os_evt_wait_or (0x1,0xffff);	
		if (result == OS_R_EVT)
		{	
			for (i = 0; i < 10; i++)
			{
				IOSET0 = LED2|BUZZER;
				os_dly_wait (10);//100ms	
				IOCLR0 = LED2|BUZZER;
				os_dly_wait (10);	
			}	
		}	
		
	}	
}

void EINT0_Handler (void) __irq
{
	isr_evt_set (0x01, tid2); // Signal Task 3 with an event
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
	IODIR0 = LED1|LED2|BUZZER;//P0.0 to P0.2 are Output pins
	config_EINT ();
	config_vic ();
	os_sys_init (task1);
}
