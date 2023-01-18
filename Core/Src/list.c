/*
 * list.c
 *
 *  Created on: Jan 18, 2023
 *      Author: minhl
 */

#include "list.h"

unsigned char LIST_Add_Task(list* my_list, void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD, uint32_t TaskID, node* prev, node* cur) {
	node* task = (node*)malloc(sizeof(node));
	task->data.pTask = pFunction;
	task->data.DELAY = DELAY;
	task->data.PERIOD = PERIOD;
	task->data.TaskID = TaskID;
	task->data.RunMe = 0;
	task->next = NULL;

	if (my_list->size == 0) {
		my_list->head = task;
		my_list->tail = task;
		my_list->size++;
		return TaskID;
	} else if (prev == NULL) {
		task->next = my_list->head;
		my_list->head = task;
		my_list->size++;
		return TaskID;
	} else if (cur == NULL) {
		my_list->tail->next = task;
		my_list->tail = task;
		my_list->size++;
		return TaskID;
	} else {
		prev->next = task;
		task->next = cur;
		my_list->size++;
		return TaskID;
	}
}

unsigned char LIST_Delete_Task(list* my_list, uint32_t TaskID) {
    node* tmp = my_list->head;
    node* prev = NULL;
    if (my_list->size <= 0) {
        return SCH_MAX_TASK;
    }

    if (my_list->size == 1) {
        my_list->head = NULL;
        my_list->tail = NULL;
        my_list->size--;
        free(tmp);
        return TaskID;
    }

    while (tmp->data.TaskID != TaskID || tmp != NULL) {
    	prev = tmp;
    	tmp = tmp->next;
    }

    if (tmp == NULL) {
    	return SCH_MAX_TASK;
    } else if (prev == NULL) {
    	my_list->head = my_list->head->next;
    	my_list->size--;
    	free(tmp);
    	return TaskID;
    } else if (tmp->next == NULL) {
    	prev->next = NULL;
    	my_list->tail = prev;
    	my_list->size--;
    	free(tmp);
    	return TaskID;
    } else {
    	prev->next = tmp->next;
    	tmp->next = NULL;
    	free(tmp);
    	my_list->size--;
    	return TaskID;
    }
    return TaskID;
}

unsigned char LIST_Delete_Head_Task(list* my_list) {
    if (my_list->size <= 0) {
        return SCH_MAX_TASK;
    } else if (my_list->size == 1) {
    	node* tmp = my_list->head;
    	uint32_t TaskID = tmp->data.TaskID;
        my_list->head = NULL;
        my_list->tail = NULL;
        my_list->size--;
        free(tmp);
        return TaskID;
    } else {
    	node*tmp = my_list->head;
    	uint32_t TaskID = tmp->data.TaskID;
    	my_list->head = my_list->head->next;
    	my_list->size--;
    	free(tmp);
    	return TaskID;
    }
}
