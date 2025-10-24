#include <rtl.h>

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
	count1++;
	os_tsk_delete_self ();
}	
	

__task void task2 (void)
{
	for (;;)
	{
		count2++;
	}
}

__task void task3 (void)
{
	for (;;)
	{
		count3++;
	}
}

main ()
{
	os_sys_init(task1);
}
