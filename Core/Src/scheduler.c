/*
 * scheduler.c
 *
 *  Created on: Nov 30, 2022
 *      Author: minhl
 */

#include "scheduler.h"
sTask SCH_tasks_G[SCH_MAX_TASKS];
unsigned char Error_code_G;;

void SCH_Init(void) {
	unsigned char i;
	for (i = 0; i<SCH_MAX_TASKS; i++) {
		SCH_Delete_Task(i);
	}
	Error_code_G = 0;
//	Timer_init();
//	Watchdog_init();
}

unsigned char SCH_Add_Task (void (*pFunction)() , uint32_t DELAY, uint32_t PERIOD){
	unsigned char Index = 0;
	while ((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS)) {
		Index++;
	}
	if (Index == SCH_MAX_TASKS) {
		Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
		return SCH_MAX_TASKS;
	}
	SCH_tasks_G[Index].pTask = pFunction;
	SCH_tasks_G[Index].Delay = DELAY/CYCLE;
	SCH_tasks_G[Index].Period = PERIOD/CYCLE;
	SCH_tasks_G[Index].RunMe = 0;

	return Index;
}

void SCH_Update(void) {
	unsigned char Index;
	for (Index = 0; Index < SCH_MAX_TASKS; Index++) {
		if (SCH_tasks_G[Index].pTask) {
			if (SCH_tasks_G[Index].Delay == 0) {
				SCH_tasks_G[Index].RunMe +=1 ;
				if (SCH_tasks_G[Index].Period) {
					SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
				}
			} else {
				SCH_tasks_G[Index].Delay--;
			}
		}
	}
}

void SCH_Dispatch_Tasks(void){
	unsigned char Index;
	for (Index = 0; Index < SCH_MAX_TASKS; Index++) {
		if (SCH_tasks_G[Index].RunMe > 0) {
			(*SCH_tasks_G[Index].pTask)();
			SCH_tasks_G[Index].RunMe -= 1;
			if (SCH_tasks_G[Index].Period == 0) {
				SCH_Delete_Task(Index);
			}
		}
	}
//	SCH_Report_Status();
//	SCH_Go_To_Sleep();
}

unsigned char SCH_Delete_Task(const uint8_t TASK_INDEX) {
	unsigned char Return_code;
	if (SCH_tasks_G[TASK_INDEX].pTask == 0) {
		Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;
		Return_code = RETURN_ERROR;
	} else {
		Return_code = RETURN_NORMAL;
	}
	SCH_tasks_G[TASK_INDEX].pTask = 0x0000;
	SCH_tasks_G[TASK_INDEX].Delay = 0;
	SCH_tasks_G[TASK_INDEX].Period = 0;
	SCH_tasks_G[TASK_INDEX].RunMe = 0;
	return Return_code;
}

void SCH_Report_Status(void) {
#ifdef SCH_REPORT_ERRORS
	if (Error_code_G != Last_error_code_G) {
		Error_port = 255 - Error_code_G;
		Last_error_code_G = Error_code_G;
		if (Error_code_G != 0) {
			Error_tick_count_G = 60000;
		} else {
			Error_tick_count_G = 0;
		}
	} else {
		if (Error_tick_count_G != 0) {
			if (--Error_tick_count_G == 0) {
				Error_code_G = 0;
			}
		}
	}
#endif
}

void SCH_Go_To_Sleep() {

}

