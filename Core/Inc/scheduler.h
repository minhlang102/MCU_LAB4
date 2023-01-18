/*
 * scheduler.h
 *
 *  Created on: Nov 30, 2022
 *      Author: minhl
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "global.h"
#include "main.h"

#define SCH_MAX_TASK    30

void SCH_Init(void);

unsigned char SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD);

unsigned char SCH_Delete_Task(uint32_t TaskID);

void SCH_Update_Task();

void SCH_Dispatch_Task();

#endif /* INC_SCHEDULER_H_ */
