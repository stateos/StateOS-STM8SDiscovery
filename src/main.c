#include <os.h>
#include <led.h>

OS_SEM(sem, 0, semNormal);

OS_TSK_DEF(sla, 0)
{
	sem_wait(sem);
	LED_Tick();
}

OS_TSK_DEF(mas, 0)
{
	tsk_delay(SEC);
	sem_give(sem);
}

int main(void)
{
	LED_Init();

	sys_init();
	tsk_start(sla);
	tsk_start(mas);
	tsk_stop();
}
