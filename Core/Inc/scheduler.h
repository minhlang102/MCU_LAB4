/*
 * scheduler.h
 *
 *  Created on: Nov 30, 2022
 *      Author: minhl
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "main.h"
#include "global.h"
#include "stdio.h"

#define SCH_MAX_TASKS 	40
#define NO_TASK_ID		0

#define ERROR_SCH_TOO_MANY_TASK								1
#define ERROR_SCH_WAITING_FOR_SLAVE_TO_ACK					2
#define ERROR_SCH_WAITING_FOR_START_COMMAND_FROM_MASTER		3
#define ERROR_SCH_ONE_OR_MORE_SLAVES_DID_NOT_START			4
#define ERROR_SCH_LOST_SLAVE								5
#define ERROR_SCH_CAN_BUS_ERROR								6
#define ERROR_I2C_WRITE_BYTE_AT24C64						7
#define ERROR_SCH_TOO_MANY_TASKS							8
#define RETURN_ERROR										(unsigned)"ERROR"
#define RETURN_NORMAL										(unsigned)"NORMAL"
#define ERROR_SCH_CANNOT_DELETE_TASK						(unsigned)"CANNOT DELETE TASK"

typedef struct {
		void (*pTask) (void);
		uint32_t Delay;
		uint32_t Period;
		uint8_t RunMe;
		uint32_t TaskID;
}sTask;

void SCH_Init(void);

unsigned char SCH_Add_Task ( void (*pFunction)() , uint32_t DELAY, uint32_t PERIOD);

void SCH_Update(void);

void SCH_Dispatch_Tasks(void);

unsigned char SCH_Delete_Task(const uint8_t TASK_INDEX);

void SCH_Report_Status();

void SCH_Go_To_Sleep();

void Timer_init();

#endif /* INC_SCHEDULER_H_ */
