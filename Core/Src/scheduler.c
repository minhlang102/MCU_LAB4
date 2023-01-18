/*
 * scheduler.c
 *
 *  Created on: Nov 30, 2022
 *      Author: minhl
 */

#include "scheduler.h"

#include "scheduler.h"
#include "list.h"

list my_list;
int32_t TaskID_arr[SCH_MAX_TASK];
uint32_t curID;
uint8_t dispatch;

void SCH_Init(void) {
    sTask* tmp = my_list.head;
    while (tmp != NULL)
    {
        SCH_Delete_Task(tmp->TaskID);
    }
    for (int32_t i=0; i<SCH_MAX_TASK; i++) {
        TaskID_arr[i] = -1;
    }
    curID = 0;
    dispatch = 0;
    my_list.head = NULL;
    my_list.tail = NULL;
    my_list.size = 0;
}

unsigned char SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD) {
    uint32_t id_count = 0;
	node *cur = my_list.head, *prev = NULL, *last = NULL;

    while (TaskID_arr[curID] != -1 && id_count < SCH_MAX_TASK)
    {
        curID = (curID+1)%SCH_MAX_TASK;
        id_count++;
    }

    if (id_count == SCH_MAX_TASK) {
        return SCH_MAX_TASK;
    }

    if (my_list.size == 0) {
    	LIST_Add_Task(&my_list, pFunction, DELAY/CYCLE, PERIOD, curID, prev, cur);
    } else {
    	uint32_t sum = 0;
    	while (DELAY >= sum + cur->data.DELAY && cur != NULL) {
    		sum += cur->data.DELAY;
    		prev = cur;
    		cur = cur->next;
    	}
    	uint32_t DELAY_NEW = DELAY - sum;
    	if (cur != NULL) {
    		cur->data.DELAY -= DELAY_NEW;
    	}
    	LIST_Add_Task(&my_list, pFunction, DELAY_NEW/CYCLE, PERIOD, curID, prev, cur);
    }

    TaskID_arr[curID] = curID;
    return curID;
}

unsigned char SCH_Delete_Task(uint32_t TaskID) {
    if (TaskID_arr[TaskID] == -1) {
        return SCH_MAX_TASK;
    }
    LIST_Delete_Head_Task(&my_list);
    TaskID_arr[TaskID] = -1;
    return TaskID;
}

void SCH_Update_Task() {
    node* tmp = my_list.head;
    if (tmp != NULL && dispatch == 0) {
        if (tmp->data.DELAY != 0) {
            tmp->data.DELAY--;
        } else {
        	dispatch = 1;
        }
    }
}

void SCH_Dispatch_Task() {
    node* tmp = my_list.head;
    if (tmp->data.DELAY == 0 && tmp != NULL) {
        (*tmp->data.pTask)();
        if (tmp->data.PERIOD == 0) {
            SCH_Delete_Task(tmp->data.TaskID);
            return;
        } else {
            void (*pTask) (void) = tmp->data.pTask;
            uint32_t DELAY = tmp->data.DELAY;
            uint32_t PERIOD = tmp->data.PERIOD;
            uint32_t TaskID = tmp->data.TaskID;
            uint8_t RunMe = tmp->data.RunMe;
            SCH_Delete_Task(tmp->data.TaskID);
            SCH_Add_Task(pTask, PERIOD, PERIOD);
        }
        dispatch = 0;
    }
}

