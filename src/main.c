#include <os.h>
#include <led.h>

OS_SEM(sem, 0, semCounting);

void slave()
{
	sem_wait(sem);

	LED_Tick();
}

void master()
{
	tsk_delay(SEC);

	sem_give(sem);
}

OS_TSK(sla, 0, slave);
OS_TSK(mas, 0, master);

void main()
{
	LED_Config();

	sys_init();
	tsk_start(sla);
	tsk_start(mas);
	tsk_stop();
}
